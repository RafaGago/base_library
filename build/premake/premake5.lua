local base_name             = "base_library"
local task_queue_name       = base_name .. "_task_queue"
local nonblock_name         = base_name .. "_nonblock"
local serial_name           = base_name .. "_serial"
local version               = ".0.0.0"

local repo                  = path.getabsolute ("../..")
local repo_build            = repo .. "/build"
local stage                 = repo_build .. "/stage/" .. os.get()
local build                 = repo_build .. "/" .. os.get()

local cmocka                = repo .. "/dependencies/cmocka"

local repo_include          = repo .. "/include"
local repo_src              = repo .. "/src"
local repo_test_src         = repo .. "/test/src"

local base_src              = repo_src .. "/bl/lib/base"
local base_test_src         = repo_test_src .. "/bl/base"
local sem_stress_src        = repo_test_src .. "/bl/semaphore_stress"

local task_queue_src        = repo_src .. "/bl/lib/task_queue"
local task_queue_test_src   = repo_test_src .. "/bl/task_queue"
local task_queue_stress_src = repo_test_src .. "/bl/task_queue_stress"

local nonblock_src          = repo_src .. "/bl/lib/nonblock"
local nonblock_test_src     = repo_test_src .. "/bl/nonblock"

local serial_src            = repo_src .. "/bl/lib/serial"

local function os_is_posix()
  return os.get() ~= "windows"
end

solution "build"
  configurations { "release", "debug" }
  location (build)

filter {"system:linux"}
  defines {"BL_USE_CLOCK_MONOTONIC_RAW"}

filter {"configurations:*release*"}  
  optimize "On"

filter {"configurations:*release*", "kind:*Lib", }
  defines {"NDEBUG"}
  
filter {"configurations:*debug*"}
  flags {"Symbols"}
  defines {"DEBUG"}
  optimize "Off"

filter {"configurations:*"}
  flags {"MultiProcessorCompile"}
  includedirs { repo_include }

filter {"system:not windows"}
  postbuildcommands {
    "cd %{cfg.buildtarget.directory} && "..
    "ln -sf %{cfg.buildtarget.name} " ..
    "%{cfg.buildtarget.name:gsub ('.%d+.%d+.%d+', '')}"
    }
  
filter {"kind:ConsoleApp", "system:windows"}
  links {"winmm.lib"}
  
filter {"kind:ConsoleApp", "system:not windows"}
  links {"pthread", "rt"}
    
filter {"kind:SharedLib", "configurations:debug", "system:not windows"}
  targetextension (".so" .. ".d" .. version)
  
filter {"kind:SharedLib", "configurations:release", "system:not windows"}
    targetextension (".so" .. version)

filter {"kind:StaticLib", "configurations:debug", "system:not windows"}
  targetextension (".a" .. ".d" .. version )

filter {"kind:StaticLib", "configurations:debug"}
  targetdir (stage .. "/debug/lib")
  
filter {"kind:StaticLib", "configurations:release", "system:not windows"}
  targetextension (".a" .. version )

filter {"kind:StaticLib", "configurations:release"}
  targetdir (stage .. "/release/lib")

filter {"kind:ConsoleApp", "configurations:debug", "system:not windows"}
  targetextension ("_d" .. version)

filter {"kind:ConsoleApp", "configurations:debug"}
  targetdir (stage .. "/debug/bin")
  
filter {"kind:ConsoleApp", "configurations:release", "system:not windows"}
  targetextension (version )

filter {"kind:ConsoleApp", "configurations:release"}
  targetdir (stage .. "/release/bin")  
   
filter {"action:gmake"}
  buildoptions {"-fvisibility=hidden"}
  buildoptions {"-Wfatal-errors"}
  buildoptions {"-std=gnu11"}
  
filter {"action:vs*"}
  buildoptions {"/TP"}

--LIBRARY PROJECTS
project (base_name)
  kind "StaticLib"
  language "C"  
  files {base_src  .. "/**"}  

project (nonblock_name)
  kind "StaticLib"
  language "C"
  includedirs {repo_src}
  files {nonblock_src  .. "/**"}
  
project (task_queue_name)
  kind "StaticLib"
  language "C"
  includedirs {repo_src}
  files {task_queue_src  .. "/**"}
  
project (serial_name)
  kind "StaticLib"
  language "C"
  defines {"NDEBUG"}
  includedirs {repo_src}
  files {serial_src  .. "/**"}

--TEST PROJECTS
project (base_name .. "_test")
  kind "ConsoleApp"
  language "C"
  includedirs {cmocka .. "/include", repo_src, repo_test_src}
  files { base_test_src .. "/**" }
  links {cmocka .. "/lib/cmocka"}

project (nonblock_name .. "_test")
  kind "ConsoleApp"
  language "C"
  includedirs {cmocka .. "/include", repo_src, repo_test_src}
  files { nonblock_test_src .. "/**" }
  links {cmocka .. "/lib/cmocka"}

project (task_queue_name .. "_test")
  kind "ConsoleApp"
  language "C"
  includedirs {cmocka .. "/include", repo_src, repo_test_src}
  files { task_queue_test_src .. "/**" }
  links {cmocka .. "/lib/cmocka", base_name, nonblock_name}

project (base_name .. "_semaphore_stress")
  kind "ConsoleApp"
  language "C"
  includedirs {repo_src, repo_test_src}
  files {sem_stress_src .. "/**"}

project (task_queue_name .. "_stress")
  kind "ConsoleApp"
  language "C"
  includedirs {repo_src, repo_test_src}
  files {task_queue_stress_src .. "/**"}
  links {nonblock_name}

