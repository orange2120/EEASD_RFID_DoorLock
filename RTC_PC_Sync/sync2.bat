set com=COM6
mode %com% BAUD=9600 PARITY=n DATA=8
@echo off
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j
set hours=%ldt:~8,2%
set minutes=%ldt:~10,2%
set /A seconds=%ldt:~12,2%+6
set miliSeconds=%ldt:~15,3%
:loop
if %TIME% LSS %hours%:%minutes%:%seconds%.00 goto loop
for /F "usebackq tokens=1,2 delims==" %%i in (`wmic os get LocalDateTime /VALUE 2^>NUL`) do if '.%%i.'=='.LocalDateTime.' set ldt=%%j

rem next line adds 1 second, CREATES NEW VARIABLE
set /a "newsec=%ldt:~12,2% +1"

echo S%newsec%, >%com%
echo D%ldt:~10,2%, >%com%
echo H%ldt:~8,2%, >%com%

echo %newsec%
echo S%ldt:~12,2%,
echo D%ldt:~10,2%,
echo H%ldt:~8,2%,
set /A seconds=%ldt:~12,2%+6
:loop2
if %TIME% LSS %hours%:%minutes%:%seconds%.00 goto loop2

echo T%ldt:~6,2%, >%com%
echo M%ldt:~4,2%, >%com%
echo J%ldt:~0,4%, >%com%

echo T%ldt:~6,2%,
echo M%ldt:~4,2%,
echo J%ldt:~0,4%,

@pause