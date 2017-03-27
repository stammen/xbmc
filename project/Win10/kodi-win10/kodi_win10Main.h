#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.
namespace kodi_win10
{
	class kodi_win10Main : public DX::IDeviceNotify
	{
	public:
		kodi_win10Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
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
		Concurrency::critical_section& GetCriticalSection() { return m_criticalSection; }

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		void ProcessInput();
		void Update();
		bool Render();

		// Cached pointer to device resources.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		//std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		//std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		Windows::Foundation::IAsyncAction^ m_renderLoopWorker;
		Concurrency::critical_section m_criticalSection;

		// Rendering loop timer.
		DX::StepTimer m_timer;

		// Track current input pointer position.
		float m_pointerLocationX;
	};
}