#pragma once

#if defined XBMC_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C"
{
  DECLDIR int Add(int a, int b);
}



