#/bin/sh
if [[ $# == 1 ]]; then
  cd /Users/tjf/Library/Developer/Xcode/DerivedData/$1-*/Build/Products/Debug
  sudo cp -R $1.kext /tmp
  cd /tmp
  sudo chown -R root:wheel $1.kext
  sudo kextload $1.kext
fi

if [[ x$2 == x"-u" ]]; then
  sudo kextunload -b kingfree.$1
fi
