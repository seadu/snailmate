describe :array_binary_union, shared: true do
  it "returns an array of elements that appear in either array (union)" do
    [].send(@method, []).should == []
    [1, 2].send(@method, []).should == [1, 2]
    [].send(@method, [1, 2]).should == [1, 2]
    [ 1, 2, 3, 4 ].send(@method, [ 3, 4, 5 ]).should == [1, 2, 3, 4, 5]
  end

  it "creates an array with no duplicates" do
    [ 1, 2, 3, 1, 4, 5 ].send(@method, [ 1, 3, 4, 5, 3, 6 ]).should == [1, 2, 3, 4, 5, 6]
  end

  it "creates an array with elements in order they are first encountered" do
    [ 1, 2, 3, 1 ].send(@method, [ 1, 3, 4, 5 ]).should == [1, 2, 3, 4, 5]
  end

  it "properly handles recursive arrays" do
    empty = ArraySpecs.empty_recursive_array
    empty.send(@method, empty).should == empty

    array = ArraySpecs.recursive_array
    array.send(@method, []).should == [1, 'two', 3.0, array]
    [].send(@method, array).should == [1, 'two', 3.0, array]
    array.send(@method, array).should == [1, 'two', 3.0, array]
    array.send(@method, empty).should == [1, 'two'