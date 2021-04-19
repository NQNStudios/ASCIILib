# Msys dependencies:
# pacman -S base-devel mingw-w64-x86_64-toolchain vim mingw-w64-x86_64-cmake
(cd SDL && ./configure && make && make install)
MINGW_ROOT=C:/msys64/mingw64 HXCPP_M64=true haxe build.hxml