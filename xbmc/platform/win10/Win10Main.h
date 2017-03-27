#pragma once

#if defined XBMC_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

extern "C"
{
  DECLDIR int KodiAdd(int a, int b);
  DECLDIR int Win10Main(Windows::UI::Core::CoreDispatcher^ dispatcher, Windows::UI::Xaml::Controls::Panel^ swapChainPanel);
}



