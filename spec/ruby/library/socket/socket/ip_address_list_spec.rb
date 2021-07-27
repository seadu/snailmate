
require_relative '../spec_helper'

describe 'Socket.ip_address_list' do
  it 'returns an Array' do
    Socket.ip_address_list.should be_an_instance_of(Array)
  end

  describe 'the returned Array' do
    before do
      @array = Socket.ip_address_list