# clean working dir
Remove-Item -Recurse release\*

# create required directories
New-Item -Path release/tools -ItemType Directory | Out-Null
New-Item -Path release/data -ItemType Directory | Out-Null

# add necessary executables - will be moved to an extra package in future
Copy-Item ~/bin/OpenKNXproducer.exe release/tools/
Copy-Item ~/bin/bossac.exe release/tools/

# get xml for kxnprod
~/bin/OpenKNXproducer.exe create --Debug --Output=release/LogicModule.knxprod --HeaderFileName=src/Logikmodul.h src/Logikmodul-Release.xml
Move-Item src/Logikmodul-Release.debug.xml release/data/LogicModule.xml

# build firmware based on generated headerfile for PICO
~/.platformio/penv/Scripts/pio.exe run -e build_RP2040
Copy-Item .pio/build/build_RP2040/firmware.uf2 release/data/

# build firmware based on generated headerfile for SAMD
~/.platformio/penv/Scripts/pio.exe run -e build_SAMD
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
Move-Item Release.zip release/
