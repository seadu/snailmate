require 'test/unit/color'

module Test
  module Unit
    class ColorScheme
      include Enumerable

      TERM_256 = /
        [+-]256color|
        \A(?:
          alacritty|
          iTerm\s?\d*\.app|
          kitty|
          mintty|
          ms-terminal|
          nsterm-build\d+|
          nsterm|
          terminator|
          terminology(?:-[0-9.]+)?|
          termite|
          vscode
        )\z/x

      class << self
        def default
          if available_colors >= 256
            default_for_256_colors
          else
            default_for_8_colors
          end
        end

        @@default_for_8_colors = nil
        def default_for_8_colors
          @@default_for_8_colors ||=
            new("pass" => Color.new("green", :background => true) +
                          Color.new("white", :bold => true),
                "pass-marker" => Color.new("green", :bold => true),
                "failure" => Color.new("red", :background => true) +
                             Color.new("white", :bold => true),
                "failure-marker" => Color.new("red"),
                "pending" => Color.new("magenta", :background => true) +
                             Color.new("white", :bold => true),
                "pending-marker" => Color.new("magenta"),
                "omission" => Color.new("blue", :background => true) +
                             Color.new("white", :bold => true),
                "omission-marker" => Color.new("blue"),
                "notification" => Color.new("cyan", :background => true) +
                                  Color.new("white", :bold => true),
                "notification-marker" => Color.new("cyan"),
                "error" => Color.new("black", :background => true) +
                           Color.new("yellow", :bold => true),
                "error-marker" => Color.new("yellow"),
                "case" => Color.new("blue", :background => true) +
                          Color.new("white", :bold => true),
                "suite" => Color.new("green", :background => true) +
                           Color.new("white", :bold => true),
                "diff-inserted-tag" => Color.new("red", :background => true) +
                                       Color.new("black", :bold => true),
                "diff-deleted-tag" => Color.new("green", :background => true) +
                                      Color.new("black", :bold => true),
                "diff-difference-tag" => Color.new("cyan", :background => true) +
                                         Color.new("white", :bold => true),
                "diff-inserted" => Color.new("red", :background => true) +
                                   Color.new("white", :bold => true),
                "diff-deleted" =>  Color.new("green", :background => true) +
                                   Color.new("white", :bold => true))
        end

        @@default_for_256_colors = nil
        def default_for_256_colors
          @@default_for_256_colors ||=
            new("pass" => Color.new("030", :background => true) +
                          Color.new("555", :bold => true),
                "pass-marker" => Color.new("050", :bold => true),
                "failure" => Color.new("300", :background => true) +
                             Color.new("555", :bold => true),
                "failure-marker" => Color.new("500"),
                "pending" => Color.new("303", :background => true) +
                              Color.