# clean working dir
Remove-Item release\*
# get xml for kxnprod
~\bin\OpenKNXproducer.exe create --Debug --Output=release/LogicModule.knxprod --HeaderFileName=Logikmodul.h src/Logikmodul-Release.xml
Move-Item src\Logikmodul-Release.debug.xml release\LogicModule.xml

# build firmware based on generated headerfile
~\.platformio\penv\Scripts\pio.exe run -e build

Copy-Item .pio\build\build\firmware.uf2 release\
# copy .pio\build\build\firmware.elf release\

Copy-Item scripts\BuildKNXprod.ps1 release\
Copy-Item scripts\UploadFirmware*.ps1 release\

# cleanup
Remove-Item release\LogicModule.knxprod

# create package 
Compress-Archive -Path release\* -DestinationPath Release.zip
Remove-Item release\*
Move-Item Release.zip release\
