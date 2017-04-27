//
// DirectXPage.xaml.cpp
// Implementation of the DirectXPage class.
//

#include "pch.h"
#include "DirectXPage.xaml.h"
#include "platform/win10/Win10Main.h"

using namespace kodi_win10;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Display;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace concurrency;
using namespace Windows::UI::Core;
using namespace Windows::UI::ViewManagement;

#pragma comment(lib, "libkodi.lib")

DirectXPage::DirectXPage()
	: m_windowVisible(true)
{
	InitializeComponent();

	// Register event handlers for page lifecycle.
	CoreWindow^ window = Window::Current->CoreWindow;

	window->VisibilityChanged +=
		ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &DirectXPage::OnVisibilityChanged);

	DisplayInformation^ currentDisplayInformation = DisplayInformation::GetForCurrentView();

	currentDisplayInformation->DpiChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDpiChanged);

	currentDisplayInformation->OrientationChanged +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnOrientationChanged);

	DisplayInformation::DisplayContentsInvalidated +=
		ref new TypedEventHandler<DisplayInformation^, Object^>(this, &DirectXPage::OnDisplayContentsInvalidated);


	// At this point we have access to the device. 
	// We can create the device-dependent resources.
  auto deviceResources = getKodiDeviceResources();
  deviceResources->SetSwapChainPanel(swapChainPanel);

	StartRenderLoop(swapChainPanel);
}

DirectXPage::~DirectXPage()
{
	// Stop rendering and processing events on destruction.
	StopRenderLoop();
}

void DirectXPage::StartRenderLoop(Windows::UI::Xaml::Controls::SwapChainPanel^ swapChainPanel)
{
  // If the animation render loop is already running then do not start another thread.
  if (m_renderLoopWorker != nullptr && m_renderLoopWorker->Status == AsyncStatus::Started)
  {
    return;
  }

  auto dispatcher = Windows::UI::Xaml::Window::Current->CoreWindow->Dispatcher;
  // Create a task that will be run on a background thread.
  auto workItemHandler = ref new WorkItemHandler([this, dispatcher, swapChainPanel](IAsyncAction ^ action)
  {
    Win10Main(dispatcher);
#if 0
    // Calculate the updated frame and render once per vertical blanking interval.
    while (action->Status == AsyncStatus::Started)
    {
      critical_section::scoped_lock lock(m_criticalSection);
      Update();
      if (Render())
      {
        m_deviceResources->Present();
      }
    }
#endif // 0

  });

  // Run task on a dedicated high priority background thread.
  m_renderLoopWorker = ThreadPool::RunAsync(workItemHandler, WorkItemPriority::High, WorkItemOptions::TimeSliced);
}

void DirectXPage::StopRenderLoop()
{
  m_renderLoopWorker->Cancel();
}


// Saves the current state of the app for suspend and terminate events.
void DirectXPage::SaveInternalState(IPropertySet^ state)
{
  auto deviceResources = getKodiDeviceResources();
	critical_section::scoped_lock lock(deviceResources->GetCriticalSection());
  deviceResources->Trim();

	// Stop rendering when the app is suspended.
	StopRenderLoop();

	// Put code to save app state here.
}

// Loads the current state of the app for resume events.
void DirectXPage::LoadInternalState(IPropertySet^ state)
{
	// Put code to load app state here.

	// Start rendering when the app is resumed.
	StartRenderLoop(swapChainPanel);
}

// Window event handlers.

void DirectXPage::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
{
	m_windowVisible = args->Visible;
	if (m_windowVisible)
	{
		StartRenderLoop(swapChainPanel);
	}
	else
	{
		StopRenderLoop();
	}
}

// DisplayInformation event handlers.

void DirectXPage::OnDpiChanged(DisplayInformation^ sender, Object^ args)
{
  auto deviceResources = getKodiDeviceResources();
	critical_section::scoped_lock lock(deviceResources->GetCriticalSection());
	// Note: The value for LogicalDpi retrieved here may not match the effective DPI of the app
	// if it is being scaled for high resolution devices. Once the DPI is set on DeviceResources,
	// you should always retrieve it using the GetDpi method.
	// See DeviceResources.cpp for more details.
  deviceResources->SetDpi(sender->LogicalDpi);
	//m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnOrientationChanged(DisplayInformation^ sender, Object^ args)
{
  auto deviceResources = getKodiDeviceResources();
  critical_section::scoped_lock lock(deviceResources->GetCriticalSection());
  deviceResources->SetCurrentOrientation(sender->CurrentOrientation);
	//m_main->CreateWindowSizeDependentResources();
}

void DirectXPage::OnDisplayContentsInvalidated(DisplayInformation^ sender, Object^ args)
{
  auto deviceResources = getKodiDeviceResources();
  critical_section::scoped_lock lock(deviceResources->GetCriticalSection());
  deviceResources->ValidateDevice();
}



