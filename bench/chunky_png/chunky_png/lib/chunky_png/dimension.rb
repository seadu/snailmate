module ChunkyPNG
  
  # Creates a {ChunkyPNG::Dimension} instance using arguments that can be interpreted 
  # as width and height.
  # 
  # @overload Dimension(width, height)
  #   @param [Integer] width The width-component of the dimension.
  #   @param [Integer] height The height-component of the dimension.
  #   @return [ChunkyPNG::Dimension] The instantiated dimension.
  #
  # @overload Dimension(string)
  #   @param [String] string A string from which a width and height value can be parsed, e.g.
  #      <tt>'10x20'</tt> or <tt>'[10, 20]'</tt>.
  #   @return [ChunkyPNG::Dimension] The instantiated dimension.
  #
  # @overload Dimension(ary)
  #   @param [Array] ary An array with the desired width as first element and the
  #      desired height as second element, e.g. <tt>[10, 20]</tt>. 
  #   @return [ChunkyPNG::Dimension] The instantiated dimension.
  #
  # @overload Dimension(hash)
  #   @param [Hash] hash An hash with a <tt>'height'</tt> or <tt>:height</tt> key for the
  #      desired height and with a <tt>'width'</tt> or <tt>:width</tt> key for the desired
  #      width.
  #   @return [ChunkyPNG::Dimension] The instantiated dimension.
  #
  # @return [ChunkyPNG::Dimension] The dimension created by this factory method.
  # @raise [ArgumentError] If the argument(s) given where not understood as a dimension.
  # @see ChunkyPNG::Dimension
  def self.Dimension(*args)

    case args.length
      when 2; ChunkyPNG::Dimension.new(*args)
      when 1; case source = args.first
          when ChunkyPNG::Dimension; source
          when ChunkyPNG::Point; ChunkyPNG::Dimension.new(source.x, source.y)
          when Array; ChunkyPNG::Dimension.new(source[0], source[1])
          when Hash;  ChunkyPNG::Dimension.new(source[:width] || source['width'], source[:height] || source['height'])
          when ChunkyPNG::Dimension::DIMENSION_REGEXP; ChunkyPNG::Dimension.new($1, $2)
          else
            if source.respond_to?(:width) && source.respond_to?(:height)
              ChunkyPNG::Dimension.new(source.width, source.height)
            else
              raise ArgumentError, "Don't know how to construct a point from #{source.inspect}!"
            end
        end
      else raise ArgumentError, "Don't know how to construct a point from #{args.inspect}!"
    end
  end
  
  # Class that represents the dimension of something, e.g. a {ChunkyPNG::Canvas}.
  #
  # This class contains some methods to simplify performing dimension related checks.
  class Dimension

    # @return [Regexp] The rege