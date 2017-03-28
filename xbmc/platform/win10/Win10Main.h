#pragma once

#include "xbmcexport.h"
#include "rendering/dx/DeviceResources.h"
#include <memory>

DECLDIR std::shared_ptr<DX::DeviceResources> getKodiDeviceResources();
DECLDIR int Win10Main(Windows::UI::Core::CoreDispatcher^ dispatcher);



