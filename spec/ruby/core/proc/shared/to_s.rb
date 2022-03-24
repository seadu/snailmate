describe :proc_to_s, shared: true do
  describe "for a proc created with Proc.new" do
    it "returns a description including file and line number" do
      Proc.new { "hello" }.send(@method).should =~ /^#<Proc:([^ ]*?) #{Regexp.escape __F