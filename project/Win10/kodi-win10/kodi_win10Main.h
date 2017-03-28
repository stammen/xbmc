#pragma once

#include "rendering/dx/DeviceResources.h"

// Renders Direct2D and 3D content on the screen.
namespace kodi_win10
{
	class kodi_win10Main : public DX::IDeviceNotify
	{
	public:
		kodi_win10Main();
		~kodi_win10Main();
		void CreateWindowSizeDependentResources();
		void StartTracking() {
      // m_sceneRenderer->StartTracking(); 
    }
		void TrackingUpdate(float positionX) { m_pointerLocationX = positionX; }
		void StopTracking() { 
      // m_sceneRenderer->StopTracking(); 
    }
		bool IsTracking() { 
      // return m_sceneRenderer->IsTracking()
      return false; 
    }
		void StartRenderLoop(Windows::UI::Xaml::Controls::Panel^ swapChainPanel);
		void StopRenderLoop();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;

		// Track current input pointer position.
		float m_pointerLocationX;
	};
}