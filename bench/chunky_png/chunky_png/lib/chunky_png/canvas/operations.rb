module ChunkyPNG
  class Canvas
    # The ChunkyPNG::Canvas::Operations module defines methods to perform
    # operations on a {ChunkyPNG::Canvas}. The module is included into the
    # Canvas class so all these methods are available on every canvas.
    #
    # Note that some of these operations modify the canvas, while some
    # operations return a new canvas and leave the original intact.
    #
    # @see ChunkyPNG::Canvas
    module Operations
      # Converts the canvas to grayscale.
      #
      # This method will modify the canvas. The obtain a new canvas and leave
      # the current instance intact, use {#grayscale} instead.
      #
      # @return [ChunkyPNG::Canvas] Returns itself, converted to grayscale.
      # @see {#grayscale}
      # @see {ChunkyPNG::Color#to_grayscale}
      def grayscale!
        pixels.map! { |pixel| ChunkyPNG::Color.to_grayscale(pixel) }
        self
      end

      # Converts the canvas to grayscale, returning a new canvas.
      #
      # This method will not modify the canvas. To modift the current canvas,
      # use {#grayscale!} instead.
      #
      # @return [ChunkyPNG::Canvas] A copy of the canvas, converted to
      #   grayscale.
      # @see {#grayscale!}
      # @see {ChunkyPNG::Color#to_grayscale}
      def grayscale
        dup.grayscale!
      end

      # Composes another image onto this image using alpha blending. This will
      # modify the current canvas.
      #
      # If you simply want to replace pixels or when the other image does not
      # have transparency, it is faster to use {#replace!}.
      #
      # @param [ChunkyPNG::Canvas] other The foreground canvas to compose on
      #   the current canvas, using alpha compositing.
      # @param [Integer] offset_x The x-offset to apply the new foreground on.
      # @param [Integer] offset_y The y-offset to apply the new foreground on.
      # @return [ChunkyPNG::Canvas] Returns itself, but with the other canvas
      #   composed onto it.
      # @raise [ChunkyPNG::OutOfBounds] when the other canvas doesn't fit on
      #   this one, given the offset and size of the other canvas.
      # @see #replace!
      # @see #compose
      def compose!(other, offset_x = 0, offset_y = 0)
        check_size_constraints!(other, offset_x, offset_y)

        for y in 0...other.height do
          for x in 0...other.width do
            set_pixel(x + offset_x,
                      y + offset_y,
                      ChunkyPNG::Color.compose(other.get_pixel(x, y),
                                               get_pixel(x + offset_x,
                                                         y + offset_y)))
          end
        end
        self
      end

      # Composes another image onto this image using alpha blending. This will
      # return a new canvas and leave the original intact.
      #
      # If you simply want to replace pixels or when the other image does not
      # have transparency, it is faster to use {#replace}.
      #
      # @param (see #compose!)
      # @return [ChunkyPNG::Canvas] Returns the new canvas, composed of the
      #   other 2.
      # @raise [ChunkyPNG::OutOfBounds] when the other canvas doesn't fit on
      #   this one, given the offset and size of the other canvas.
      #
      # @note API changed since 1.0 - This method now no longer is in place,
      #   but returns a new canvas and leaves the original intact. Use
      #   {#compose!} if you want to compose on the canvas in place.
      # @see #replace
      def compose(other, offset_x = 0, offset_y = 0)
        dup.compose!(other, offset_x, offset_y)
      end

      # Replaces pixels on this image by pixels from another pixels, on a given
      # offset. This method will modify the current canvas.
      #
      # This will completely replace the pixels of the background image. If you
      # want to blend them with semi-transparent pixels from the foreground
      # image, see {#compose!}.
      #
      # @param [ChunkyPNG::Canvas] other The foreground canvas to get the
      #   pixels from.
      # @param [Integer] offset_x The x-offset to apply the new foreground on.
      # @param [Integer] offset_y The y-offset to apply the new foreground on.
      # @return [ChunkyPNG::Canvas] Returns itself, but with the other canvas
      #   placed onto it.
      # @raise [ChunkyPNG::OutOfBounds] when the other canvas doesn't fit on
      #   this one, given the offset and size of the other canvas.
      # @see #compose!
      # @see #replace
      def replace!(other, offset_x = 0, offset_y = 0)
        check_size_constraints!(other, offset_x, offset_y)

        for y in 0...other.height do
          for d in 0...other.width
            pixels[(y + offset_y) * width + offset_x + d] = other.pixels[y * other.width + d]
          end
        end
        self
      end

      # Replaces pixels on this image by pixels from another pixels, on a given
      # offset. This method will modify the current canvas.
      #
      # This will completely replace the pixels of the background image. If you
      # want to blend them with semi-transparent pixels from the foreground
      # image, see {#compose!}.
      #
      # @param (see #replace!)
      # @return [ChunkyPNG::Canvas] Returns a new, combined canvas.
      # @raise [ChunkyPNG::OutOfBounds] when the other canvas doesn't fit on
      #   this one, given the offset and size of the other canvas.
      #
      # @note API changed since 1.0 - This method now no longer is in place,
      #   but returns a new canvas and leaves the original intact. Use
      #   {#replace!} if you want to replace pixels on the canvas in place.
      # @see #compose
      def replace(other, offset_x = 0, offset_y = 0)
        dup.replace!(other, offset_x, offset_y)
      end

      # Crops an image, given the coordinates and size of the image that needs
      # to be cut out. This will leave the original image intact and return a
      # new, cropped image with pixels copied from the original