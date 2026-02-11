@echo off
call "C:\Qt\6.7.3\mingw_64\bin\qtenv2.bat"
"C:\Qt\Tools\CMake_64\bin\cmake.exe" -S "c:/Users/MEGA-PC/Downloads/transaction/transaction" -B "c:/Users/MEGA-PC/Downloads/transaction/transaction/build/Desktop_Qt_6_7_3_MinGW_64_bit-Debug" -G Ninja
echo EXIT %ERRORLEVEL%
pause
