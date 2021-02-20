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
      -3.3448803488331067402e-143.send(@method).should == "-3.3448803488331067e-143"
      -2.2740074343500832557e-168.send(@method).should == "-2.2740074343500833e-168"
      7.0587971678048535732e+191.send(@method).should == "7.058797167804854e+191"
      -284438.88327586348169.send(@method).should == "-284438.8832758635"
      3.953272468476091301e+105.send(@method).should == "3.9532724684760913e+105"
      -3.6361359552959847853e+100.send(@method).should == "-3.636135955295985e+100"
      -1.3222325865575206185e-31.send(@method).should == "-1.3222325865575206e-31"
      1.1440138916932761366e+130.send(@method).should == "1.1440138916932761e+130"
      4.8750891560387561157e-286.send(@method).should == "4.875089156038756e-286"
      5.6101113356591453525e-257.send(@method).should == "5.610111335659145e-257"
      -3.829644279545809575e-100.send(@method).should == "-3.8296442795458096e-100"
      1.5342839401396406117e-194.send(@method).should == "1.5342839401396406e-194"
      2.2284972755169921402e-144.send(@method).should == "2.228497275516992e-144"
      2.1825655917065601737e-61.send(@method).should == "2.1825655917065602e-61"
      -2.6672271363524338322e-62.send(@method).should == "-2.667227136352434e-62"
      -1.9257995160119059415e+21.send(@method).should == "-1.925799516011906e+21"
      -8.9096732962887121718e-198.send(@method).should == "-8.909673296288712e-198"
      2.0202075376548644959e-90.send(@method).should == "2.0202075376548645e-90"
      -7.7341602581786258961e-266.send(@method).should == "-7.734160258178626e-266"
      3.5134482598733635046e+98.send(@method).should == "3.5134482598733635e+98"
      -2.124411722371029134e+154.send(@method).should == "-2.124411722371029e+154"
      -4.573908787355718687e+110.send(@method).should == "-4.573908787355719e+110"
      -1.9344425934170969879e-232.send(@method).should == "-1.934442593417097e-232"
      -1.3274227399979271095e+171.send(@method).should == "-1.3274227399979271e+171"
      9.3495270482104442383e-283.send(@method).should == "9.349527048210444e-283"
      -4.2046059371986483233e+307.send(@method).should == "-4.2046059371986483e+307"
      3.6133547278583543004e-117.send(@method).should == "3.613354727858354e-117"
      4.9247416523566613499e-08.send(@method).should == "4.9247416523566613e-08"
      1.6936145488250064007e-71.send(@method).should == "1.6936145488250064e-71"
      2.4455483206829433098e+96.send(@method).should == "2.4455483206829433e+96"
      7.9797449851436455384e+124.send(@method).should == "7.979744985143646e+124"
      -1.3873689634457876774e-129.send(@method).should == "-1.3873689634457877e-129"
      3.9761102037533483075e+284.send(@method).should == "3.976110203753348e+284"
      -4.2819791952139402486e-303.send(@method).should == "-4.28197919521394e-303"
      -5.7981017546689831298e-116.send(@method).should == "-5.798101754668983e-116"
      -3.953266497860534199e-28.send(@method).should == "-3.953266497860534e-28"
      -2.0659852720290440959e-243.send(@method).should == "-2.065985272029044e-243"
      8.9670488995878688018e-05.send(@method).should == "8.967048899587869e-05"
      -1.2317943708113061768e-98.send(@method).should == "-1.2317943708113062e-98"
      -3.8930768307633080463e+248.send(@method).should == "-3.893076830763308e+248"
      6.5854032671803925627e-239.send(@method).should == "6.5854032671803926e-239"
      4.6257022188980878952e+177.send(@method).should == "4.625702218898088e+177"
      -1.9397155125507235603e-187.send(@method).should == "-1.9397155125507236e-187"
      8.5752156951245705056e+117.send(@method).should == "8.57521569512457e+117"
      -2.4784875958162501671e-132.send(@method).should == "-2.4784875958162502e-132"
      -4.4125691841230058457e-203.send(@method).should == "-4.412569184123006e-203"
    end

    it "random examples in human ranges" do
      # 50.times do
      #   formatted = ''
      #   rand(1..3).times do
      #     formatted << rand(10).to_s
      #   end
      #   formatted << '.'
      #   rand(1..9).times do
      #     formatted << rand(10).to_s
      #   end
      #   float = formatted.to_f
      #   puts "#{'%.20f' % float}.send(@method).should == #{float.send(@method).inspect}"
      # end

      5.17869899999999994122.send(@method).should == "5.178699"
      905.62695729999995819526.send(@method).should == "905.6269573"
      62.75999999999999801048.send(@method).should == "62.76"
      6.93856795800000014651.send(@method).should == "6.938567958"
      4.95999999999999996447.send(@method).should == "4.96"
      32.77993899999999882766.send(@method).should == "32.779939"
      544.12756779999995160324.send(@method).should == "544.1275678"
      66.25801119999999855281.send(@method).should == "66.2580112"
      7.90000000000000035527.send(@method).should == "7.9"
      5.93100000000000004974.send(@method).should == "5.931"
      5.21229313600000043749.send(@method).should == "5.212293136"
      503.44173809000000119340.send(@method).should == "503.44173809"
      79.26000000000000511591.send(@method).should == "79.26"
      8.51524999999999998579.send(@method).should == "8.51525"
      174.00000000000000000000.send(@method).should == "174.0"
      50.39580000000000126192.send(@method).should == "50.3958"
      35.28999999999999914735.send(@method).should == "35.29"
      5.43136675399999990788.send(@method).should == "5.431366754"
      654.07680000000004838512.send(@method).should == "654.0768"
      6.07423700000000010846.send(@method).should == "6.074237"
      102.25779799999999397642.send(@method).should == "102.257798"
      5.08129999999999970584.send(@method).should == "5.0813"
      6.00000000000000000000.send(@method).should == "6.0"
      8.30000000000000071054.send(@method).should == "8.3"
      32.68345999999999662577.send(@method).should == "32.68346"
      581.11170000000004165486.send(@method).should == "581.1117"
      76.31342999999999676675.send(@method).should == "76.31343"
      438.30826000000001840817.send(@method).should == "438.30826"
      482.06631994000002805478.send(@method).should == "482.06631994"
      55.92721026899999969828.send(@method).should == "55.927210269"
      4.00000000000000000000.send(@method).should == "4.0"
      55.86693999999999959982.send(@method).should == "55.86694"
      787.98299999999994724931.send(@method).should == "787.983"
      5.73810511000000023074.send(@method).should == "5.73810511"
      74.51926810000000500622.send(@method).should == "74.5192681"
      892.89999999999997726263.send(@method).should == "892.9"
      68.27299999999999613465.send(@method).should == "68.273"
      904.10000000000002273737.send(@method).should == "904.1"
      5.23200000000000020606.send(@method).should =