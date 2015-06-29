
cd /Users/tjf/Library/Developer/Xcode/DerivedData/SocksTest-dammgxwrrimwhwfzvrottlpoomuc/Build/Products/Debug
sudo cp -R SocksTest.kext /tmp
cd /tmp
sudo chown -R root:wheel SocksTest.kext
sudo kextload SocksTest.kext
