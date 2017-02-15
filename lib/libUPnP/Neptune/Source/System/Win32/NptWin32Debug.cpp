/*****************************************************************
|
|   Neptune - Debug Support: Win32 Implementation
|
|   (c) 2002-2006 Gilles Boccon-Gibod
|   Author: Gilles Boccon-Gibod (bok@bok.net)
|
 ****************************************************************/

/*----------------------------------------------------------------------
|   includes
+---------------------------------------------------------------------*/
#include <stdio.h>
#if defined(_XBOX)
#include <xtl.h>
#else
#include <windows.h>
#endif

#include "NptConfig.h"
#include "NptDefs.h"
#include "NptTypes.h"
#include "NptDebug.h"
#include "NptUtils.h"

/*----------------------------------------------------------------------
|   NPT_DebugOutput
+---------------------------------------------------------------------*/
void
NPT_DebugOutput(const char* message)
{
#ifdef MS_UWP
  OutputDebugString(win32ConvertUtf8ToW(message).c_str());
#else
#if !defined(_WIN32_WCE)
    OutputDebugString(message);
#endif
    printf("%s", message);
#endif // MS_UWP
}

