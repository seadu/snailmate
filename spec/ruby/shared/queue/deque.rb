describe :queue_deq, shared: true do
  it "removes an item from the queue" do
    q = @object.call
    q << Object.new
    q.size.should == 1
    q.send @method
    q.size.should == 0
  end

  it "returns items in the order they were added" do
    q = @object.call
    q << 1
    q << 2
    q.send(@method).should == 1
    q.send(@method).should == 2
  end

  it "blocks the thread until there are items in the queue" do
    q = @object