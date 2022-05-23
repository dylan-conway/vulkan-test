workspace "c-project"
    configurations { "Debug", "Release" }

project "vulkan-test"
    kind "ConsoleApp"
    language "C"
    makesettings { "CC=gcc" }
    targetdir "build/%{cfg.buildcfg}"

    includedirs { "$(VK_SDK_PATH)/Include/", "include/" }
    libdirs { "$(VK_SDK_PATH)/Lib/" }
    links { "mingw32", "SDL2main", "SDL2", "vulkan-1", "shaderc_shared" }

    files { "include/**.h", "src/**.c" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        warnings "Extra"
        disablewarnings { "unused-parameter", "unused-function" }
        symbols "On"

    filter "configurations:Release"
        optimize "On"
        symbols "Off"