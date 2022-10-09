require 'power_assert/configuration'
require 'io/console/size'

module PowerAssert
  class InspectedValue
    def initialize(value)
      @value = value
    end

    def inspect
      @value
    end
  end
  private_constant :Inspec