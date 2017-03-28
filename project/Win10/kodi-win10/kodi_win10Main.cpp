#include "pch.h"
#include "kodi_win10Main.h"

using namespace kodi_win10;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

#include "platform/win10/Win10Main.h"
#pragma comment(lib, "libkodi.lib")

// Loads and initializes application assets when the application is loaded.
kodi_win10Main::kodi_win10Main() :
	m_pointerLocationX(0.0f)
{
	// Register to be notified if the Device is lost or recreated
  auto deviceResources = getKodiDeviceResources();
  deviceResources->RegisterDeviceNotify(this);
}

kodi_win10Main::~kodi_win10Main()
{
	// Deregister device notification
  auto deviceResources = getKodiDeviceResources();
  deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)
void kodi_win10Main::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.
	//m_sceneRenderer->CreateWindowSizeDependentResources();
}

void kodi_win10Main::StartRenderLoop(Windows::UI::Xaml::Controls::Panel^ swapChainPanel)
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

void kodi_win10Main::StopRenderLoop()
{
	m_renderLoopWorker->Cancel();
}

// Process all input from the user before updating game state
void kodi_win10Main::ProcessInput()
{
	// TODO: Add per frame input handling here.
	//m_sceneRenderer->TrackingUpdate(m_pointerLocationX);
}

#if 0
// Renders the current frame according to the current application state.
// Returns true if the frame was rendered and is ready to be displayed.
bool kodi_win10Main::Render() 
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.
	// TODO: Replace this with your app's content rendering functions.
	//m_sceneRenderer->Render();
	//m_fpsTextRenderer->Render();

	return true;
}

#endif

// Notifies renderers that device resources need to be released.
void kodi_win10Main::OnDeviceLost()
{
	//m_sceneRenderer->ReleaseDeviceDependentResources();
	//m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.
void kodi_win10Main::OnDeviceRestored()
{
	//m_sceneRenderer->CreateDeviceDependentResources();
	//m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
