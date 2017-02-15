/*****************************************************************
|
|   Neptune - Console Support: Win32 Implementation
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include <string>

#include "NptConfig.h"
#include "NptConsole.h"
#include "NptUtils.h"



/*----------------------------------------------------------------------
|   NPT_Console::Output
+---------------------------------------------------------------------*/
void
NPT_Console::Output(const char* message)
{
#ifdef MS_UWP
  OutputDebugString(win32ConvertUtf8ToW(message).c_str());
#else
    OutputDebugString(message);
    printf("%s", message);
#endif
}

