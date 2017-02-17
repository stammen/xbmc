if(CORE_SYSTEM_NAME STREQUAL windowsstore)
	set(SOURCES GUIWindowTestPatternDX.cpp
							RenderSystemDXWin10.cpp)

	set(HEADERS GUIWindowTestPatternDX.h
							RenderSystemDXWin10.h)
else()
	set(SOURCES GUIWindowTestPatternDX.cpp
							RenderSystemDX.cpp)

	set(HEADERS GUIWindowTestPatternDX.h
							RenderSystemDX.h)
endif()

core_add_library(rendering_dx)
