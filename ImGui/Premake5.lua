project "ImGui"
    kind "StaticLib"
    language "C++"

    targetdir ("lib/%{cfg.buildcfg}")
	objdir ("lib/%{cfg.buildcfg}/bin-int")

    files
	{
		"imgui.cpp",
        "imgui_widgets.cpp",
        "imgui_draw.cpp",
        "imgui_tables.cpp",
        "ImguiSFML/imgui-SFML.cpp",
	}

    includedirs
    {
        "%{IncludeDir.IMGUI}/",
		"%{IncludeDir.SFML}/",
    }

	libdirs 
	{
		"%{LibraryDir.SFML}/%{cfg.buildcfg}/",
	}

	links
	{
		"sfml-audio.lib",
		"sfml-graphics.lib",
		"sfml-main.lib",
		"sfml-network.lib",
		"sfml-system.lib",
		"sfml-window.lib",
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "off"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++20"
		staticruntime "off"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
