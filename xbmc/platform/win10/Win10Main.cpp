/*
 *      Copyright (C) 2005-2013 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */

#include "CompileInfo.h"
#include "threads/Thread.h"
#include "threads/platform/win/Win32Exception.h"
#include "platform/xbmc.h"
#include "utils/CPUInfo.h"
#include "utils/Environment.h"
#include "utils/CharsetConverter.h" // Required to initialize converters before usage
#include "utils/log.h" 

#include <dbghelp.h>
#include <shellapi.h>

#include"Win10Main.h"

#ifdef __cplusplus
extern "C" int win10_main(int argc, char* argv[]);
#endif

extern "C"
{
  DECLDIR int KodiAdd(int a, int b)
  {
    //CLog::Log(LOGNOTICE, "KodiAdd");
    return(a + b);
  }


  // Minidump creation function
  LONG WINAPI CreateMiniDump(EXCEPTION_POINTERS* pEp)
  {
    CLog::Log(LOGNOTICE, "Windows Store apps starcktrace and minidump are collected by the Windows Store");
    return pEp->ExceptionRecord->ExceptionCode;
  }

  //-----------------------------------------------------------------------------
  // Name: WinMain()
  // Desc: The application's entry point
  //-----------------------------------------------------------------------------
  DECLDIR int Win10Main(LPWSTR commandLine)
  {
    // this fixes crash if OPENSSL_CONF is set to existed openssl.cfg  
    // need to set it as soon as possible  
    CEnvironment::unsetenv("OPENSSL_CONF");

    int status = 0;

    // Initialise Winsock
    WSADATA wd;
    WSAStartup(MAKEWORD(2, 2), &wd);

    // Create and run the app
    status = win10_main(0, nullptr);

    WSACleanup();

    return status;
  }
}

