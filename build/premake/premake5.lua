local projname              = "base_library"
local version               = ".0.0.0"
local task_queue_name       = projname .. "_task_queue"

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

local function os_is_posix()
  return os.get() ~= "windows"
end

solution "build"
  configurations { "release", "debug" }
  location (build)

filter {"configurations:*"}
  if os.get() == "linux" then
    defines {"BL_USE_CLOCK_MONOTONIC_RAW"}
  end

filter {"configurations:*release*"}
  defines {"NDEBUG"}
  optimize "On"

filter {"configurations:*debug*"}
  flags {"Symbols"}
  defines {"DEBUG"}
  optimize "Off"

filter {"configurations:*"}
  flags {"MultiProcessorCompile"}
  includedirs { repo_include }

filter {"configurations:*"}
  if os_is_posix() then
    postbuildcommands {
      "cd %{cfg.buildtarget.directory} && "..
      "ln -sf %{cfg.buildtarget.name} " ..
      "%{cfg.buildtarget.name:gsub ('.%d+.%d+.%d+', '')}"
    }
  end
--[[
filter {"kind:SharedLib", "configurations:*debug*"}
  if os_is_posix() then
    targetextension (".d" .. ".so" .. version)
  end
  --this method of setting the output dir won't be practical if there are
  --a lot of platforms...
  targetdir (stage .. "/debug/lib")

filter {"kind:SharedLib", "configurations:*release*"}
  if os_is_posix() then
    targetextension (".so" .. version)
  end
  --this method of setting the output dir won't be practical if there are
  --a lot of platforms...
  targetdir (stage .. "/release/lib")
]]--
filter {"kind:StaticLib", "configurations:*debug*"}
  if os_is_posix() then
    targetextension (".d" .. ".a" .. version )
  end
  --this method of setting the output dir will be impractical if there are
  --a lot of platforms...
  targetdir (stage .. "/debug/lib")

filter {"kind:StaticLib", "configurations:*release*"}
  if os_is_posix() then
    targetextension (".a" .. version)
  end
  --this method of setting the output dir won't be practical if there are
  --a lot of platforms...
  targetdir (stage .. "/release/lib")

filter {"kind:ConsoleApp", "configurations:*debug*"}
  if os_is_posix() then
    targetextension ("_d" .. version)
  end
  --this method of setting the output dir won't be practical if there are
  --a lot of platforms...
  targetdir (stage .. "/debug/bin")

filter {"kind:ConsoleApp", "configurations:*release*"}
  if os_is_posix() then
    targetextension (version)
  end
  --this method of setting the output dir won't be practical if there are
  --a lot of platforms...
  targetdir (stage .. "/release/bin")

--toolset filters and file filters broken in alpha
--dirty workaround (specifying compiler for each os) follows ...

-- WORKAROUND --
local function is_gcc()
  return os.get() ~= "windows"
end

filter {"language:C"}
  if is_gcc() then
    buildoptions {"-Wfatal-errors"}
    buildoptions {"-std=c11"}
    defines {"_XOPEN_SOURCE=700"}
  end

filter {"configurations:*", "kind:*Lib"}
  if is_gcc() then
    buildoptions {"-fvisibility=hidden"}
  end

filter {"kind:ConsoleApp"}
  if is_gcc() then
    links {"pthread", "rt"}
  end
-- WORKAROUND END --

--LIBRARY PROJECTS
project (projname)
  kind "StaticLib"
  language "C" --TODO: with visual studio might be C++
  files {base_src  .. "/**"}

project (task_queue_name)
  kind "StaticLib"
  includedirs {repo_src}
  language "C" --TODO: with visual studio might be C++
  files {task_queue_src  .. "/**"}

--TEST PROJECTS
project (projname .. "_test")
  kind "ConsoleApp"
  language "C" --TODO: with visual studio might be C++
  includedirs {cmocka .. "/include", repo_src, repo_test_src}
  files { base_test_src .. "/**" }
  links {cmocka .. "/lib/cmocka"}

project (task_queue_name .. "_test")
  kind "ConsoleApp"
  language "C" --TODO: with visual studio might be C++
  includedirs {cmocka .. "/include", repo_src, repo_test_src}
  files { task_queue_test_src .. "/**" }
  links {cmocka .. "/lib/cmocka", projname}

project "semaphore_stress"
  kind "ConsoleApp"
  language "C" --TODO: with visual studio might be C++
  includedirs {repo_src, repo_test_src}
  files {sem_stress_src .. "/**"}

project (task_queue_name .. "_stress")
  kind "ConsoleApp"
  language "C" --TODO: with visual studio might be C++
  includedirs {repo_src, repo_test_src}
  files {task_queue_stress_src .. "/**"}
  links {projname}

