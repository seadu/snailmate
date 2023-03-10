
require_relative '../spec_helper'

describe "Addrinfo#ipv6_multicast?" do
  describe "for an ipv4 socket" do
    before :each do
      @multicast = Addrinfo.tcp("224.0.0.1", 80)
      @other     = Addrinfo.tcp("0.0.0.0", 80)
    end

    it "returns true for a multicast address" do
      @multicast.ipv6_multicast?.should be_false
    end

    it "returns false for another address" do
      @other.ipv6_multicast?.should be_false
    end
  end

  describe "for an ipv6 socket" do
    it "returns true for a multicast address" do
      Addrinfo.ip('ff00::').should.ipv6_multicast?
      Addrinfo.ip('ff00::1').should.ipv6_multicast?
      Addrinfo.ip('ff08::1').should.ipv6_multicast?
      Addrinfo.ip('fff8::1').should.ipv6_multicast?

      Addrinfo.ip('ff02::').should.ipv6_multicast?
      Addrinfo.ip('ff02::1').should.ipv6_multicast?
      Addrinfo.ip('ff0f::').should.ipv6_multicast?
    end

    it "returns false for another address" do
      Addrinfo.ip('::1').should_not.ipv6_multicast?
      Addrinfo.ip('fe80::').should_not.ipv6_multicast?
    end
  end

  with_feature :unix_socket do
    describe "for a unix socket" do
      before :each do
        @addrinfo = Addrinfo.unix("/tmp/sock")
      end

      it "returns false" do
        @addrinfo.ipv6_multicast?.should be_false
      end
    end
  end
end