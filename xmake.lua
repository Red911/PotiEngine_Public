add_rules("mode.debug", "mode.release")
add_rules("plugin.vsxmake.autoupdate")

add_requires("fmt")
add_requires("libsdl")
add_requires("libsdl_image")
add_requires("entt")
add_requires("nlohmann_json")
add_requires("chipmunk2d")
add_requires("dr_wav")
add_requires("openal-soft", { configs = { shared = true }})
add_requires("imgui", { configs = { sdl2 = true, sdl2_renderer = true }})
add_requireconfs("libsdl","**.libsdl", {configs = {sdlmain = not is_mode("debug")}})


set_project("PotiEngine")

--set_encodings("utf-8")
set_languages("cxx20")
set_exceptions("cxx")
set_rundir(".")
set_warnings("allextra")
add_includedirs("include")

add_cxflags("/w4251")

target("PotiEngine")
    set_kind("shared")
    add_headerfiles("include/**.h")
    add_files("src/PotiEngine/**.cpp")
    add_packages("imgui","fmt","libsdl","libsdl_image","entt", "openal-soft", "dr_wav", {public = true})
    add_packages("nlohmann_json",  {public = true})
    add_packages("chipmunk2d",  {public = true})
    
    add_defines("POTI_ENGINE_BUILD")

target("PotiGame")
    set_kind("binary")
    add_files("src/main.cpp")
    add_deps("PotiEngine")

target("PotiTest")
    set_kind("binary")
    add_files("src/test.cpp")
    add_deps("PotiEngine")
    add_packages("nlohmann_json")
    add_packages("chipmunk2d")
    add_packages("openal-soft")
--
-- If you want to known more usage about xmake, please see https://xmake.io
--
-- ## FAQ
--
-- You can enter the project directory firstly before building project.
--
--   $ cd projectdir
--
-- 1. How to build project?
--
--   $ xmake
--
-- 2. How to configure project?
--
--   $ xmake f -p [macosx|linux|iphoneos ..] -a [x86_64|i386|arm64 ..] -m [debug|release]
--
-- 3. Where is the build output directory?
--
--   The default output directory is `./build` and you can configure the output directory.
--
--   $ xmake f -o outputdir
--   $ xmake
--
-- 4. How to run and debug target after building project?
--
--   $ xmake run [targetname]
--   $ xmake run -d [targetname]
--
-- 5. How to install target to the system directory or other output directory?
--
--   $ xmake install
--   $ xmake install -o installdir
--
-- 6. Add some frequently-used compilation flags in xmake.lua
--
-- @code
--    -- add debug and release modes
--    add_rules("mode.debug", "mode.release")
--
--    -- add macro definition
--    add_defines("NDEBUG", "_GNU_SOURCE=1")
--
--    -- set warning all as error
--    set_warnings("all", "error")
--
--    -- set language: c99, c++11
--    set_languages("c99", "c++11")
--
--    -- set optimization: none, faster, fastest, smallest
--    set_optimize("fastest")
--
--    -- add include search directories
--    add_includedirs("/usr/include", "/usr/local/include")
--
--    -- add link libraries and search directories
--    add_links("tbox")
--    add_linkdirs("/usr/local/lib", "/usr/lib")
--
--    -- add system link libraries
--    add_syslinks("z", "pthread")
--
--    -- add compilation and link flags
--    add_cxflags("-stdnolib", "-fno-strict-aliasing")
--    add_ldflags("-L/usr/local/lib", "-lpthread", {force = true})
--
-- @endcode
--

