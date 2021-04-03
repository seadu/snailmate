require "reline"

module IRB
  class << self
    class Vec
      def initialize(x, y, z)
        @x, @y, @z = x, y, z
      end

      attr_reader :x, :y, :z

      def sub(other)
        Vec.new(@x - other.x, @y - other.y, @z - other.z)
      end

      def dot(other)
        @x*other.x + @y*other.y + @z*other.z
      end

      def cross(other)
        ox, oy, oz = other.x, other.y, other.z
        Vec.new(@y*oz-@z*oy, @z*ox-@x*oz, @x*oy-@y*ox)
      end

      def normalize
        r = Math.sqrt(self.dot(self))
        Vec.new(@x / r, @y / r, @z / r)
      end
    end

    class Canvas
      def initialize((h, w))
        @data = (0..h-2).map { [0] * w }
        @scale = [w / 2.0, h-2].min
        @center = Complex(w / 2, h-2)
      end

      def line((x1, y1), (x2, y2))
        p1 = Complex(x1, y1) / 2 * @scale + @center
        p2 = Complex(x2, y2) / 2 * @scale + @center
        line0(p1, p2)
      end

      private def line0(p1, p2)
        mid = (p1 + p2) / 2
        if (p1 - p2).abs < 1
          x, y = mid.rect
          @data[y / 2][x] |= (y % 2 > 1 ? 2 : 1)
        else
          line0(p1, mid)
          line0(p2, mid)
        end
      end

      def