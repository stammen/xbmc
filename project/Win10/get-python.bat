@echo off

if exist cpython (
  rm -rf cpython
)

if exist system\python (
  rm -rf system\python
)

if exist BuildDependencies\include\python (
  rm -rf BuildDependencies\include\python
)

if exist BuildDependencies\lib\python27_d.lib (
  rm -rf BuildDependencies\lib\python27_d.lib
)

if exist BuildDependencies\lib\python27.lib (
  rm -rf BuildDependencies\lib\python27.lib
)

if exist BuildDependencies\bin\python27_d.dll (
  rm -rf BuildDependencies\bin\python27_d.dll
)

if exist BuildDependencies\bin\python27.dll (
  rm -rf BuildDependencies\bin\python27.dll
)

mkdir system\python
mkdir BuildDependencies\lib
mkdir BuildDependencies\bin
mkdir BuildDependencies\include\python

rem goto:install

echo Cloning cpython...

call git clone https://github.com/stammen/cpython.git
pushd cpython
  call git checkout 2.7-winrt
  pushd PCBuild
  echo Building and installing cpython
	call "%VS140COMNTOOLS%vsvars32.bat"
  	call msbuild pcbuild.sln /p:Configuration="Release" /p:Platform="Win32" /m  
  	call msbuild pcbuild.sln /p:Configuration="Debug" /p:Platform="Win32" /m  
  popd
popd


:install 

echo Copying python include files...
pushd cpython
  xcopy Include ..\BuildDependencies\include\python\ /iycqs  
  xcopy PC\pyconfig.h ..\BuildDependencies\include\python\ /iycq  
popd

echo Copying python binaries...
pushd cpython\pcbuild
  xcopy python27_d.lib ..\..\BuildDependencies\lib\ /iycq  
  xcopy python27.lib ..\..\BuildDependencies\lib\ /iycq  
  xcopy python27.dll ..\..\BuildDependencies\bin\ /iycq  
  xcopy python27_d.dll ..\..\BuildDependencies\bin\ /iycq  
  
  xcopy _ctypes.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _elementtree.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _hashlib.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _multiprocessing.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _socket.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _sqlite3.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy _ssl.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy bz2.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy pyexpat.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy select.pyd ..\..\system\python\DLLs\ /iycq  
  xcopy unicodedata.pyd ..\..\system\python\DLLs\ /iycq  
popd

echo Copying python LIB files...
pushd cpython
  xcopy Lib ..\system\python\Lib /iycqs  
popd

echo Removing unused python LIB files...
pushd system\python\Lib
  rm -rf plat-aix3
  rm -rf plat-aix4
  rm -rf plat-atheos
  rm -rf plat-beos5
  rm -rf plat-darwin
  rm -rf plat-freebsd4
  rm -rf plat-freebsd5
  rm -rf plat-freebsd6
  rm -rf plat-freebsd7
  rm -rf plat-freebsd8
  rm -rf plat-irix5
  rm -rf plat-irix6
  rm -rf plat-linux2
  rm -rf plat-mac
  rm -rf plat-netbsd1
  rm -rf plat-next3
  rm -rf plat-os2emx
  rm -rf plat-riscos
  rm -rf plat-sunos5
  rm -rf plat-unixware7
popd


echo cpython installed



