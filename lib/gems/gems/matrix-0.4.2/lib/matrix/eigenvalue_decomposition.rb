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
          if (