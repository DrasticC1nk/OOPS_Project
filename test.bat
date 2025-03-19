@echo off
setlocal enabledelayedexpansion

set "TEST_FOLDER=load"
set "LOGFILE=log.txt"
set "PROGRAM_PATH=src\bin\maze.exe"

echo Test Run Log > "%LOGFILE%"
echo ==================== >> "%LOGFILE%"

if not exist "%TEST_FOLDER%" (
    echo ERROR: Test folder "%TEST_FOLDER%" does not exist! >> "%LOGFILE%"
    echo ERROR: Test folder "%TEST_FOLDER%" does not exist!
    exit /b
)

for /D %%F in (%TEST_FOLDER%\*) do (
    echo Processing folder: %%F >> "%LOGFILE%"
    echo -------------------- >> "%LOGFILE%"

    set "MAZE_FILE="
    for %%M in (%%F\Maze2D.txt %%F\Maze2D.dat) do (
        if exist "%%M" set "MAZE_FILE=%%M"
    )

    if not defined MAZE_FILE (
        echo No maze file found in %%F. Skipping... >> "%LOGFILE%"
        echo. >> "%LOGFILE%"
        continue
    )

    set "PATH_FILE="
    for %%P in (%%F\Path1D.txt %%F\Path1D.dat) do (
        if exist "%%P" set "PATH_FILE=%%P"
    )

    if not defined PATH_FILE (
        echo No path file found in %%F. Skipping... >> "%LOGFILE%"
        echo. >> "%LOGFILE%"
        continue
    )

    copy /Y "!MAZE_FILE!" "%TEST_FOLDER%\maze.txt" > nul
    copy /Y "!PATH_FILE!" "%TEST_FOLDER%\path.txt" > nul

    echo Running: "%PROGRAM_PATH%" "maze.txt" "path.txt" >> "%LOGFILE%"
    "%PROGRAM_PATH%" "maze.txt" "path.txt" >> "%LOGFILE%" 2>&1 & echo. >> "%LOGFILE%"

    del "%TEST_FOLDER%\maze.txt"
    del "%TEST_FOLDER%\path.txt"

    echo -------------------- >> "%LOGFILE%"
    echo. >> "%LOGFILE%"
)

echo All tests completed. Check "%LOGFILE%" for results.
exit /b
