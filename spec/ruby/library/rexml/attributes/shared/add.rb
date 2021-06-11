describe :rexml_attribute_add, shared: true do
  before :each do
    @e = REXML::Element.new("root")
    @attr = REXML::Attributes.new(@e)
    @name = REXML::Attribute.new("name", "Joe")
  end

  it "adds an attribute" do
    @attr.send(@method, @name)
    @attr["na