@echo off
setlocal

REM clean build directory
echo STEP 1: CLEANING BUILD DIRECTORY
if exist build (
    echo Deleting existing build folder...
    rmdir /s /q build
)

REM create build directory
mkdir build

REM configure with CMake
echo.
echo STEP 2: CONFIGURING PROJECT WITH CMAKE
cmake -B build -G Ninja
if %ERRORLEVEL% neq 0 (
    echo Configuration failed!
    pause
    exit /b 1
)

REM compile with Ninja
echo.
echo STEP 3: COMPILING FIRMWARE WITH NINJA
ninja -C build
if %ERRORLEVEL% neq 0 (
    echo Compile failed!
    pause
    exit /b 1
)

pushd build
if exist *.bin (
    for %%f in (*.bin) do (
        if not "%%f"=="app_firmware.bin" (
            ren "%%f" "app_firmware.bin"
        )
    )
)
popd


REM flash 
echo.
echo STEP 4: FLASHING FIRMWARE TO TARGET MCU
STM32_Programmer_CLI -c port=SWD sn=31FF6F064243333051530357 -w build/app_firmware.bin 0x08000000 -v -rst
