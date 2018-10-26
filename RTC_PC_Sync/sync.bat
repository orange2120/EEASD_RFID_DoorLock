mode COM6 BAUD=9600 PARITY=n DATA=8

@echo off

for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j

set hours=%ldt:~8,2%

set minutes=%ldt:~10,2%

set /A seconds=%ldt:~12,2%+2

set miliSeconds=%ldt:~15,3%

:loop

if "%TIME%" LSS "%hours%:%minutes%:%seconds%,00" goto loop

for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j

set /p x="S%ldt:~12,2%,D%ldt:~10,2%,H%ldt:~8,2%," <nul >\\.\COM6

echo %TIME%

echo S%ldt:~12,2%,

echo D%ldt:~10,2%,

echo H%ldt:~8,2%,

set /A seconds=%ldt:~12,2%+4

:loop2

if "%TIME%" LSS "%hours%:%minutes%:%seconds%,00" goto loop2

set /p x="T%ldt:~6,2%,M%ldt:~4,2%,J%ldt:~0,4%," <nul >\\.\COM6

echo %TIME%

echo T%ldt:~6,2%,

echo M%ldt:~4,2%,

echo J%ldt:~0,4%,

pause