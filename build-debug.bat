if "%VCINSTALLDIR%"=="" call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

rem Try compiling with clang first, to get a second opinion on warnings/errors
clang-cl -DDEBUG -I/mpir -Werror -Wimplicit-fallthrough -Wno-assume -Wno-deprecated-declarations -Wno-switch -c -ferror-limit=1 *.cc
if errorlevel 1 goto :eof

rem Then compile with Microsoft C++
cl /DDEBUG /MTd /WX /Zi /std:c++17 *.cc dbghelp.lib
if errorlevel 1 goto :eof
