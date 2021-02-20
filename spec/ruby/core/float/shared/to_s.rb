describe :float_to_s, shared: true do
  it "returns 'NaN' for NaN" do
    nan_value().send(@method).should == 'NaN'
  end

  it "returns 'Infinity' for positive infinity" do
    infinity_value().send(@method).should == 'Infinity'
  end

  it "returns '-Infinity' for negative infinity" do
    (-infinity_value()).send(@method).should == '-Infinity'
  end

  it "returns '0.0' for 0.0" do
    0.0.send(@method).should == "0.0"
  end

  platform_is_not :openbsd do
    it "emits '-' for -0.0" do
      -0.0.send(@method).should == "-0.0"
    end
  end

  it "emits a '-' for negative values" do
    -3.14.send(@method).should == "-3.14"
  end

  it "emits a trailing '.0' for a whole number" do
    50.0.send(@method).should == "50.0"
  end

  it "emits a trailing '.0' for the mantissa in e format" do
    1.0e20.send(@method).should == "1.0e+20"
  end

  it "uses non-e format for a positive value with fractional part having 5 significant figures" do
    0.0001.send(@method).should == "0.0001"
  end

  it "uses non-e format for a negative value with fractional part having 5 significant figures" do
    -0.0001.send(@method).should == "-0.0001"
  end

  it "uses e format for a positive value with fractional part having 6 significant figures" do
    0.00001.send(@method).should == "1.0e-05"
  end

  it "uses e format for a negative value with fractional part having 6 significant figures" do
    -0.00001.send(@method).should == "-1.0e-05"
  end

  it "uses non-e format for a positive value with whole part having 15 significant figures" do
    10000000000000.0.send(@method).should == "10000000000000.0"
  end

  it "uses non-e format for a negative value with whole part having 15 significant figures" do
    -10000000000000.0.send(@method).should == "-10000000000000.0"
  end

  it "uses non-e format for a positive value with whole part having 16 significant figures" do
    100000000000000.0.send(@method).should == "100000000000000.0"
  end

  it "uses non-e format for a negative value with whole part having 16 significant figures" do
    -100000000000000.0.send(@method).should == "-100000000000000.0"
  end

  it "uses e format for a positive value with whole part having 18 significant figures" do
    10000000000000000.0.send(@method).should == "1.0e+16"
  end

  it "uses e format for a negative value with whole part having 18 significant figures" do
    -10000000000000000.0.send(@method).should == "-1.0e+16"
  end

  it "uses e format for a positive value with whole part having 17 significant figures" do
    1000000000000000.0.send(@method).should == "1.0e+15"
  end

  it "uses e format for a negative value with whole part having 17 significant figures" do
    -1000000000000000.0.send(@method).should == "-1.0e+15"
  end

  # #3273
  it "outputs the minimal, unique form necessary to recreate the value" do
    value = 0.21611564636388508
    string = "0.21611564636388508"

    value.send(@method).should == string
    string.to_f.should == value
  end

  it "outputs the minimal, unique form to represent the value" do
    0.56.send(@method).should == "0.56"
  end

  describe "matches" do
    it "random examples in all ranges" do
      # 50.times do
      #   bytes = (0...8).map { rand(256) }
      #   string = bytes.pack('C8')
      #   float = string.unpack('D').first
      #   puts "#{'%.20g' % float}.send(@method).should == #{float.send(@method).inspect}"
      # end

      2.5540217314354050325e+163.send(@method).should == "2.554021731435405e+163"
      2.5492588360356597544e-172.send(@method).should == "2.5492588360356598e-172"
      1.742770260934704852e-82.send(@method).should == "1.7427702609347049e-82"
      6.2108093676180883209e-104.send(@method).should == "6.210809367618088e-104"
      -3.3448803488331067402e-143.send(@method)