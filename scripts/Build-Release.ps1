# check for working dir
if (Test-Path -Path release) {
    # clean working dir
    Remove-Item -Recurse release\*
} else {
    New-Item -Path release -ItemType Directory | Out-Null
}

# create required directories
New-Item -Path release/data -ItemType Directory | Out-Null

# get xml for kxnprod
~/bin/OpenKNXproducer.exe create --Debug --Output=release/LogicModule.knxprod --HeaderFileName=src/Logikmodul.h src/Logikmodul-Release.xml
if (!$?) {
    Write-Host "Error in knxprod, Release was not built!"
    exit 1
}
Move-Item src/Logikmodul-Release.debug.xml release/data/LogicModule.xml

# build firmware based on generated headerfile for PICO
~/.platformio/penv/Scripts/pio.exe run -e build_RP2040
if (!$?) {
    Write-Host "RP2040 build failed, Release was not built!"
    exit 1
}
Copy-Item .pio/build/build_RP2040/firmware.uf2 release/data/

# build firmware based on generated headerfile for SAMD
~/.platformio/penv/Scripts/pio.exe run -e build_SAMD
if (!$?) {
    Write-Host "SAMD build failed, Release was not built!"
    exit 1
}
Copy-Item .pio/build/build_SAMD/firmware.bin release/data/

# add necessary scripts
Copy-Item scripts/Readme-Release.txt release/
Copy-Item scripts/Build-knxprod.ps1 release/
Copy-Item scripts/Upload-Firmware*.ps1 release/

# cleanup
Remove-Item release/LogicModule.knxprod

# create package 
Compress-Archive -Path release/* -DestinationPath Release.zip
Remove-Item -Recurse release/*
Move-Item Release.zip release/LogicModule.zip

Write-Host "Release successfully created!"
