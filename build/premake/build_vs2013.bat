@echo off
set usage="usage: [build|rebuild|clean] [debug|release] [win32|win64]"

if "%3"=="" (
  echo %usage%
  goto end
)

if not "%1"=="build" (
  if not "%1"=="rebuild" (
    if not "%1"=="clean" (
      echo %usage%
      goto end
    )
  )
)

if not "%2"=="debug" (
  if not "%2"=="release" (
    echo %usage%
    goto end
  )
)

if not "%3"=="win32" (
  if not "%3"=="win64" (
    echo %usage%
    goto end
  )
)

if not defined DevEnvDir (
  call "%ProgramFiles(x86)%\Microsoft Visual Studio 12.0\VC\vcvarsall" x86
)

devenv /%1 "%2|%3" "..\windows\build.sln"

:end

