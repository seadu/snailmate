# frozen_string_literal: false
#
# filehandler.rb -- FileHandler Module
#
# Author: IPR -- Internet Programming with Ruby -- writers
# Copyright (c) 2001 TAKAHASHI Masayoshi, GOTOU Yuuzou
# Copyright (c) 2003 Internet Programming with Ruby writers. All rights
# reserved.
#
# $IPR: filehandler.rb,v 1.44 2003/06/07 01:34:51 gotoyuzo Exp $

require 'time'

require_relative '../htmlutils'
require_relative '../httputils'
require_relative '../httpstatus'

module WEBrick
  module HTTPServlet

    ##
    # Servlet for serving a single file.  You probably want to use the
    # FileHandler servlet instead as it handles directories and fancy indexes.
    #
    # Example:
    #
    #   server.mount('/my_page.txt', WEBrick::HTTPServlet::DefaultFileHandler,
    #                '/path/to/my_page.txt')
    #
    # This servlet handles If-Modified-Since and Range requests.

    class DefaultFileHandler < AbstractServlet

      ##
      # Creates a DefaultFileHandler instance for the file at +local_path+.

      def initialize(server, local_path)
        super(server, local_path)
        @local_path = local_path
      end

      # :stopdoc:

      def do_GET(req, res)
        st = File::stat(@local_path)
        mtime = st.mtime
        res['etag'] = sprintf("%x-%x-%x", st.ino, st.size, st.mtime.to_i)

        if not_modified?(req, res, mtime, res['etag'])
          res.body = ''
          raise HTTPStatus::NotModified
        elsif req['range']
          make_partial_content(req, res, @local_path, st.size)
          raise HTTPStatus::PartialContent
        else
          mtype = HTTPUtils::mime_type(@local_path, @config[:MimeTypes])
          res['content-type'] = mtype
          res['content-length'] = st.size.to_s
          res['last-modified'] = mtime.httpdate
          res.body = File.open(@local_path, "rb")
        end
      end

      def not_modified?(req, res, mtime, etag)
        if ir = req['if-range']
          begin
            if Time.httpdate(ir) >= mtime
              return true
            end
          rescue
            if HTTPUtils::split_header_value(ir).member?(res['etag'])
              return true
            end
          end
        end

        if (ims = req['if-modified-since']) && Time.parse(ims) >= mtime
          return true
        end

        if (inm = req['if-none-match']) &&
           HTTPUtils::split_header_value(inm).member?(res['etag'])
          return true
        end

        return false
      end

      # returns a lambda for webrick/httpresponse.rb send_body_proc
      def multipart_body(body, parts, boundary, mtype, filesize)
        lambda do |socket|
          begin
            begin
              first = parts.shift
              last = parts.shift
              socket.write(
                "--#{boundary}#{CRLF}" \
                "Content-Type: #{mtype}#{CRLF}" \
                "Content-Range: bytes #{first}-#{last}/#{filesize}#{CRLF}" \
                "#{CRLF}"
              )

              begin
                IO.copy_stream(body, socket, last - first + 1, first)
              rescue NotImplementedError
                body.seek(first, IO::SEEK_SET)
                IO.copy_stream(body, socket, last - first + 1)
              end
              socket.write(CRLF)
            end while parts[0]
            socket.write("--#{boundary}--#{CRLF}")
          ensure
            body.close
          end
        end
      end

      def make_partial_content(req, res, filename, filesize)
        mtype = HTTPUtils::mime_type(filename, @config[:MimeTypes])
        unless ranges = HTTPUtils::parse_range_header(req['range'])
          raise HTTPStatus::BadRequest,
            "Unrecognized range-spec: \"#{req['range']}\""
        end
        File.open(filename, "rb"){|io|
          if ranges.size > 1
            time = Time.now
            boundary = "#{time.sec}_#{time.usec}_#{Process::pid}"
            parts = []
            ranges.each {|range|
              prange = prepare_range(range, filesize)
              next if prange[0] < 0
              parts.concat(prange)
            }
            raise HTTPStatus::RequestRangeNotSatisfiable if parts.empty?
            res["content-type"] = "multipart/byteranges; boundary=#{boundary}"
            if req.http_version < '1.1'
              res['connection'] = 'close'
            else
              res.chunked = true
            end
            res.body = multipart_body(io.dup, parts, boundary, mtype, filesize)
          elsif range = ranges[0]
            first, last = prepare_range(range, filesize)
            raise HTTPStatus::RequestRangeNotSatisfiable if first < 0
            res['content-type'] = mtype
            res['content-range'] = "bytes #{first}-#{last}/#{filesize}"
            res['content-length'] = (last - first + 1).to_s
            res.body = io.dup
          else
            raise HTTPStatus::BadRequest
          end
        }
      end

      def prepare_range(range, filesize)
        first = range.first < 0 ? filesize + range.first : range.first
        return -1, -1 if first < 0 || first >= filesize
        last = range.last < 0 ? filesize + range.last : range.last
        last = filesize - 1 if last >= filesize
        return first, last
      end

      # :startdoc:
    end

    ##
    # Serves a directory including fancy indexing and a variety of other
    # options.
    #
    # Example:
    #
    #   server.mount('/assets', WEBrick::HTTPServlet::FileHandler,
    #                '/path/to/assets')

    class FileHandler < AbstractServlet
      HandlerTable = Hash.new # :nodoc:

      ##
      # Allow custom handling of requests for files with +suffix+ by class
      # +handler+

      def self.add_handler(suffix, handler)
        HandlerTable[suffix] = handler
      end

      ##
      # Remove custom handling of requests for files with +suffix+

      def self.remove_handler(suffix)
        HandlerTable.delete(suffix)
      end

      ##
      # Creates a FileHandler servlet on +server+ that serves files starting
      # at directory +root+
      #
      # +options+ may be a Hash containing keys from
      # WEBrick::Config::FileHandler or +true+ or +false+.
      #
      # If +options+ is true or false then +:FancyIndexing+ is enabled or
      # disabled respectively.

      def initialize(server, root, options={}, default=Config::FileHandler)
        @config = server.config
        @logger = @config[:Logger]
        @root = File.expand_path(root)
        if options == true || options == false
          options = { :FancyIndexing => options }
        end
        @options = default.dup.update(options)
      end

      # :stopdoc:

      def set_filesystem_encoding(str)
        enc = Encoding.find('filesystem')
        if enc == Encoding::US_ASCII
          str.b
        else
          str.dup.force_encoding(enc)
        end
      end

      def service(req, res)
        # if this class is mounted on "/" and /~username is requested.
        # we're going to override path information before invoking service.
        if defined?(Etc) && @options[:UserDir] && req.script_name.empty?
          if %r|^(/~([^/]+))| =~ req.path_info
            script_name, user = $1, $2
            path_info = $'
            begin
              passwd = Etc::getpwnam(user)
              @root = File::join(passwd.dir, @options[:UserDir])
              req.script_name = script_name
              req.path_info = path_info
            rescue
              @logger.debug "#{self.class}#do_GET: getpwnam(#{user}) failed"
            end
          end
        end
        prevent_directory_traversal(req, res)
        super(req, res)
      end

      def do_GET(req, res)
        unless exec_handler(req, res)
          set_dir_list(req, res)
        end
      end

      def do_POST(req, res)
        unless exec_handler(req, res)
          raise HTTPStatus::NotFound, "`#{req.path}' not found."
        end
      end

      def do_OPTIONS(req, res)
        unless exec_handler(req, res)
          super(req, res)
        end
      end

      # ToDo
      # RFC2518: HTTP Extensions for Distributed Authoring -- WEBDAV
      #
      # PROPFIND PROPPATCH MKCOL DELETE PUT COPY MOVE
      # LOCK UNLOCK

      # RFC3253: Versioning Extensions to WebDAV
      #          (Web Distributed Authoring and Versioning)
   