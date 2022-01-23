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
          res.