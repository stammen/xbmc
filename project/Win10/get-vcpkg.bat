@echo off

if exist vcpkg (
  rm -rf vcpkg
)

echo Building and installing vcpkg...
call git clone git@github.com:stammen/vcpkg.git
pushd vcpkg
  call git checkout xbmc
  echo A | powershell -exec bypass scripts\bootstrap.ps1
  echo Building and installing xbmc-deps:x86-uwp
  rem need to install windows version first to create include files with win32 tools
  call vcpkg install libfribidi
  call vcpkg install xbmc-deps:x86-uwp
popd
echo xbmc-deps:x86-uwp installed
