# frozen_string_literal: false
#
#   irb/locale.rb - internationalization module
#   	$Release Version: 0.9.6$
#   	$Revision$
#   	by Keiju ISHITSUKA(keiju@ruby-lang.org)
#
# --
#
#
#
module IRB # :nodoc:
  class Locale

    LOCALE_NAME_RE = %r[
      (?<language>[[:alpha:]]{2,3})
      (?:_  (?<territory>[[:alpha:]]{2,3}) )?
      (?:\. (?<codeset>[^@]+) )?
      (?:@  (?<modifier>.*) )?
    ]x
    LOCALE_DIR = "/lc/"

    @@legacy_encoding_alias_map = {}.freeze
    @@loaded = []

    def initialize(locale = nil)
      @override_encoding = nil
      @lang = @territory = @encoding_name = @modifier = nil
      @locale = locale || ENV["IRB_LANG"] || ENV["LC_MESSAGES"] || ENV["LC_ALL"] || ENV["LANG"] || "C"
      if m = LOCALE_NAME_RE.match(@locale)
        @lang, @territory, @encoding_name, @modifier = m[:language], m[:territory], m[:codeset], m[:modifier]

        if @encoding_name
          begin load 'irb/encoding_aliases.rb'; rescue LoadError; end
          if @encoding = @@legacy_encoding_alias_map[@encoding_name]
            warn(("%s is obsolete. use %s" % ["#{@lang}_#{@territory}.#{@encoding_name}", "#{@lang}_#{@territory}.#{@encoding.name}"]), uplevel: 1)
          end
          @encoding = Encoding.find(@encoding_name) rescue nil
        end
      end
      @encoding ||= (Encoding.find('locale') rescue Encoding::ASCII_8BIT)
    end

    attr_reader :lang, :territory, :modifier

 