# frozen_string_literal: false
class Matrix
  # Adapted from JAMA: http://math.nist.gov/javanumerics/jama/

  # Eigenvalues and eigenvectors of a real matrix.
  #
  # Computes the eigenvalues and eigenvectors of a matrix A.
  #
  # If A is diagonalizable, this provides matrices V and D
  # such that A = V*D*V.inv, where D is the diagonal matrix with entries
  # equal to the eigenvalues and V is formed by the eigenvectors.
  #
  # If A is symmetric, then V is orthogonal and thus A = V*D*V.t

  class EigenvalueDecomposition

    # Constructs the eigenvalue decomposition for a square matrix +A+
    #
    def initialize(a)
      # @d, @e: Arrays for internal storage of eigenvalues.
      # @v: Array for internal storage of eigenvectors.
      # @h: Array for internal storage of nonsymmetric Hessenberg form.
      raise TypeError, "Expected Matrix but got #{a.class}" unless a.is_a?(Matrix)
      @size = a.row_count
      @d = Array.new(@size, 0)
      @e = Array.new(@size, 0)

      if (@symmetric = a.symmetric?)
        @v = a.to_a
        tridiagonalize
        diagonalize
      else
        @v = Array.new(@size) { Array.new(@size, 0) }
        @h = a.to_a
        @ort = Array.new(@size, 0)
        reduce_to_hessenberg
        hessenberg_to_real_schur
      end
    end

    # Returns the eigenvector matrix +V+
    #
    def eigenvector_matrix
      Matrix.send(:new, build_eigenvectors.transpose)
    end
    alias_method :v, :eigenvector_matrix

    # Returns the inverse of the eigenvector matrix +V+
    #
    def eigenvector_matrix_inv
      r = Matrix.send(:new, build_eigenvectors)
      r = r.transpose.inverse unless @symmetric
      r
    end
    alias_method :v_inv, :eigenvector_matrix_inv

    # Returns the eigenvalues in an array
    #
    def eigenvalues
      values = @d.dup
      @e.each_with_index{|imag, i| values[i] = Complex(values[i], imag) unless imag == 0}
      values
    end

    # Returns an array of the eigenvectors
    #
    def eigenvectors
      build_eigenvectors.map{|ev| Vector.send(:new, ev)}
    end

    # Returns the block diagonal eigenvalue matrix +D+
    #
    def eigenvalue_matrix
      Matrix.diagonal(*eigenvalues)
    end
    alias_method :d, :eigenvalue_matrix

    # Returns [eigenvector_matrix, eigenvalue_matrix, eigenvector_matrix_inv]
    #
    def to_ary
      [v, d, v_inv]
    end
    alias_method :to_a, :to_ary


    private def build_eigenvectors
      # JAMA stores complex eigenvectors in a strange way
      # See http://web.archive.org/web/20111016032731/http://cio.nist.gov/esd/emaildir/lists/jama/msg01021.html
      @e.each_with_index.map do |imag, i|
        if imag == 0
          Array.new(@size){|j| @v[j][i]}
        elsif imag > 0
          Array.new(@size){|j| Complex(@v[j][i], @v[j][i+1])}
        else
          Array.new(@size){|j| Complex(@v[j][i-1], -@v[j][i])}
        end
      end
    end

    # Complex scalar division.

    private def cdiv(xr, xi, yr, yi)
      if (yr.abs > yi.abs)
        r = yi/yr
        d = yr + r*yi
        [(xr + r*xi)/d, (xi - r*xr)/d]
      else
        r = yr/yi
        d = yi + r*yr
        [(r*xr + xi)/d, (r*xi - xr)/d]
      end
    end


    # Symmetric Householder reduction to tridiagonal form.

    private def tridiagonalize

      #  This is derived from the Algol procedures tred2 by
      #  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
      #  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
      #  Fortran subroutine in EISPACK.

      @size.times do |j|
        @d[j] = @v[@size-1][j]
      end

      # Householder reduction to tridiagonal form.

      (@size-1).downto(0+1) do |i|

        # Scale to avoid under/overflow.

        scale = 0.0
        h = 0.0
        i.times do |k|
          scale = scale + @d[k].abs
        end
        if (scale == 0.0)
          @e[i] = @d[i-1]
          i.times do |j|
            @d[j] = @v[i-1][j]
            @v[i][j] = 0.0
            @v[j][i] = 0.0
          end
        else

          # Generate Householder vector.

          i.times do |k|
            @d[k] /= scale
            h += @d[k] * @d[k]
          end
          f = @d[i-1]
          g = Math.sqrt(h)
          if (f > 0)
            g = -g
          end
          @e[i] = scale * g
          h -= f * g
          @d[i-1] = f - g
          i.times do |j|
            @e[j] = 0.0
          end

          # Apply similarity transformation to remaining columns.

          i.times do |j|
            f = @d[j]
            @v[j][i] = f
            g = @e[j] + @v[j][j] * f
            (j+1).upto(i-1) do |k|
              g += @v[k][j] * @d[k]
              @e[k] += @v[k][j] * f
            end
            @e[j] = g
          end
          f = 0.0
          i.times do |j|
            @e[j] /= h
            f += @e[j] * @d[j]
          end
          hh = f / (h + h)
          i.times do |j|
            @e[j] -= hh * @d[j]
          end
          i.times do |j|
            f = @d[j]
            g = @e[j]
            j.upto(i-1) do |k|
              @v[k][j] -= (f * @e[k] + g * @d[k])
            end
            @d[j] = @v[i-1][j]
            @v[i][j] = 0.0
          end
        end
        @d[i] = h
      end

      # Accumulate transformations.

      0.upto(@size-1-1) do |i|
        @v[@size-1][i] = @v[i][i]
        @v[i][i] = 1.0
        h = @d[i+1]
        if (h != 0.0)
          0.upto(i) do |k|
            @d[k] = @v[k][i+1] / h
          end
          0.upto(i) do |j|
            g = 0.0
            0.upto(i) do |k|
              g += @v[k][i+1] * @v[k][j]
            end
            0.upto(i) do |k|
              @v[k][j] -= g * @d[k]
            end
          end
        end
        0.upto(i) do |k|
          @v[k][i+1] = 0.0
        end
      end
      @size.times do |j|
        @d[j] = @v[@size-1][j]
        @v[@size-1][j] = 0.0
      end
      @v[@size-1][@size-1] = 1.0
      @e[0] = 0.0
    end


    # Symmetric tridiagonal QL algorithm.

    private def diagonalize
      #  This is derived from the Algol procedures tql2, by
      #  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
      #  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
      #  Fortran subroutine in EISPACK.

      1.upto(@size-1) do |i|
        @e[i-1] = @e[i]
      end
      @e[@size-1] = 0.0

      f = 0.0
      tst1 = 0.0
      eps = Float::EPSILON
      @size.times do |l|

        # Find small subdiagonal element

        tst1 = [tst1, @d[l].abs + @e[l].abs].max
        m = l
        while (m < @size) do
          if (@e[m].abs <= eps*tst1)
            break
          end
          m+=1
        end

        # If m == l, @d[l] is an eigenvalue,
        # otherwise, iterate.

        if (m > l)
          iter = 0
          begin
            iter = iter + 1  # (Could check iteration count here.)

            # Compute implicit shift

            g = @d[l]
            p = (@d[l+1] - g) / (2.0 * @e[l])
            r = Math.hypot(p, 1.0)
            if (p < 0)
              r = -r
            end
            @d[l] = @e[l] / (p + r)
            @d[l+1] = @e[l] * (p + r)
            dl1 = @d[l+1]
            h = g - @d[l]
            (l+2).upto(@size-1) do |i|
              @d[i] -= h
            end
            f += h

            # Implicit QL transformation.

            p = @d[m]
            c = 1.0
            c2 = c
            c3 = c
            el1 = @e[l+1]
            s = 0.0
            s2 = 0.0
            (m-1).downto(l) do |i|
              c3 = c2
              c2 = c
              s2 = s
              g = c * @e[i]
              h = c * p
              r = Math.hypot(p, @e[i])
              @e[i+1] = s * r
              s = @e[i] / r
              c = p / r
              p = c * @d[i] - s * g
              @d[i+1] = h + s * (c * g + s * @d[i])

              # Accumulate transformation.

              @size.times do |k|
                h = @v[k][i+1]
                @v[k][i+1] = s * @v[k][i] + c * h
                @v[k][i] = c * @v[k][i] - s * h
              end
            end
            p = -s * s2 * c3 * el1 * @e[l] / dl1
            @e[l] = s * p
            @d[l] = c * p

            # Check for convergence.

          end while (@e[l].abs > eps*tst1)
        end
        @d[l] = @d[l] + f
        @e[l] = 0.0
      end

      # Sort eigenvalues and corresponding vectors.

      0.upto(@size-2) do |i|
        k = i
        p = @d[i]
        (i+1).upto(@size-1) do |j|
          if (@d[j] < p)
            k = j
            p = @d[j]
          end
        end
        if (k != i)
          @d[k] = @d[i]
          @d[i] = p
          @size.times do |j|
            p = @v[j][i]
            @v[j][i] = @v[j][k]
            @v[j][k] = p
          end
        end
      end
    end

    # Nonsymmetric reduction to Hessenberg form.

    private def reduce_to_hessenberg
      #  This is derived from the Algol procedures orthes and ortran,
      #  by Martin and Wilkinson, Handbook for Auto. Comp.,
      #  Vol.ii-Linear Algebra, and the corresponding
      #  Fortran subroutines in EISPACK.

      low = 0
      high = @size-1

      (low+1).upto(high-1) do |m|

        # Scale column.

        scale = 0.0
        m.upto(high) do |i|
          scale = scale + @h[i][m-1].abs
        end
        if (scale != 0.0)

          # Compute Householder transformation.

          h = 0.0
          high.downto(m) do |i|
            @ort[i] = @h[i][m-1]/scale
            h += @ort[i] * @ort[i]
          end
          g = Math.sqrt(h)
          if (@ort[m] > 0)
            g = -g
          end
          h -= @ort[m] * g
          @ort[m] = @ort[m] - g

          # Apply Householder similarity transformation
          # @h = (I-u*u'/h)*@h*(I-u*u')/h)

          m.upto(@size-1) do |j|
            f = 0.0
            high.downto(m) do |i|
              f += @ort[i]*@h[i][j]
            end
            f = f/h
            m.upto(high) do |i|
              @h[i][j] -= f*@ort[i]
            end
          end

          0.upto(high) do |i|
            f = 0.0
            high.downto(m) do |j|
              f += @ort[j]*@h[i][j]
            end
            f = f/h
            m.upto(high) do |j|
              @h[i][j] -= f*@ort[j]
            end
          end
          @ort[m] = scale*@ort[m]
          @h[m][m-1] = scale*g
        end
      end

      # Accumulate transformations (Algol's ortran).

      @size.times do |i|
        @size.times do |j|
          @v[i][j] = (i == j ? 1.0 : 0.0)
        end
      end

      (high-1).downto(low+1) do |m|
        if (@h[m][m-1] != 0.0)
          (m+1).upto(high) do |i|
            @ort[i] = @h[i][m-1]
          end
          m.upto(high) do |j|
            g = 0.0
            m.upto(high) do |i|
              g += @ort[i] * @v[i][j]
    