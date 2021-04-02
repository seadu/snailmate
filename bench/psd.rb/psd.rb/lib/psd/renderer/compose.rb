class PSD
  # Collection of methods that composes two RGBA pixels together
  # in various ways based on Photoshop blend modes.
  #
  # Mostly based on similar code from libpsd.
  module Compose
    extend self

    DEFAULT_OPTS = {
      opacity: 255,
      fill_opacity: 255
    }

    #
    # Normal blend modes
    #

    def normal(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))
      new_r = blend_channel(r(bg), r(fg), mix_alpha)
      new_g = blend_channel(g(bg), g(fg), mix_alpha)
      new_b = blend_channel(b(bg), b(fg), mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end
    alias_method :passthru, :normal

    #
    # Subtractive blend modes
    #

    def darken(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))
      new_r = r(fg) <= r(bg) ? blend_channel(r(bg), r(fg), mix_alpha) : r(bg)
      new_g = g(fg) <= g(bg) ? blend_channel(g(bg), g(fg), mix_alpha) : g(bg)
      new_b = b(fg) <= b(bg) ? blend_channel(b(bg), b(fg), mix_alpha) : b(bg)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def multiply(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))
      new_r = blend_channel(r(bg), r(fg) * r(bg) >> 8, mix_alpha)
      new_g = blend_channel(g(bg), g(fg) * g(bg) >> 8, mix_alpha)
      new_b = blend_channel(b(bg), b(fg) * b(bg) >> 8, mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def color_burn(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))

      calculate_foreground = Proc.new do |b, f|
        if f > 0
          f = ((255 - b) << 8) / f
          f > 255 ? 0 : (255 - f)
        else
          b
        end
      end

      new_r = blend_channel(r(bg), calculate_foreground.call(r(bg), r(fg)), mix_alpha)
      new_g = blend_channel(g(bg), calculate_foreground.call(g(bg), g(fg)), mix_alpha)
      new_b = blend_channel(b(bg), calculate_foreground.call(b(bg), b(fg)), mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def linear_burn(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))

      new_r = blend_channel(r(bg), (r(fg) < (255 - r(bg))) ? 0 : r(fg) - (255 - r(bg)), mix_alpha)
      new_g = blend_channel(g(bg), (g(fg) < (255 - g(bg))) ? 0 : g(fg) - (255 - g(bg)), mix_alpha)
      new_b = blend_channel(b(bg), (b(fg) < (255 - b(bg))) ? 0 : b(fg) - (255 - b(bg)), mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    #
    # Additive blend modes
    #

    def lighten(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))

      new_r = r(fg) >= r(bg) ? blend_channel(r(bg), r(fg), mix_alpha) : r(bg)
      new_g = g(fg) >= g(bg) ? blend_channel(g(bg), g(fg), mix_alpha) : g(bg)
      new_b = b(fg) >= b(bg) ? blend_channel(b(bg), b(fg), mix_alpha) : b(bg)
      
      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def screen(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))

      new_r = blend_channel(r(bg), 255 - ((255 - r(bg)) * (255 - r(fg)) >> 8), mix_alpha)
      new_g = blend_channel(g(bg), 255 - ((255 - g(bg)) * (255 - g(fg)) >> 8), mix_alpha)
      new_b = blend_channel(b(bg), 255 - ((255 - b(bg)) * (255 - b(fg)) >> 8), mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def color_dodge(fg, bg, opts={})
      return apply_opacity(fg, opts) if fully_transparent?(bg)
      return bg if fully_transparent?(fg)

      mix_alpha, dst_alpha = calculate_alphas(fg, bg, DEFAULT_OPTS.merge(opts))

      calculate_foreground = Proc.new do |b, f|
        f < 255 ? [(b << 8) / (255 - f), 255].min : b
      end

      new_r = blend_channel(r(bg), calculate_foreground.call(r(bg), r(fg)), mix_alpha)
      new_g = blend_channel(g(bg), calculate_foreground.call(g(bg), g(fg)), mix_alpha)
      new_b = blend_channel(b(bg), calculate_foreground.call(b(bg), b(fg)), mix_alpha)

      rgba(new_r, new_g, new_b, dst_alpha)
    end

    def lin