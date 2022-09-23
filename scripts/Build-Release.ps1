# This script is just a template and has to be copied and modified per project
# This script should be called from .vscode/tasks.json with
#
#   scripts/Build-Release.ps1            - for Beta builds
#   scripts/Build-Release.ps1 Release    - for Release builds
#
# {
#     "label": "Build-Release",
#     "type": "shell",
#     "command": "scripts/Build-Release.ps1 Release",
#     "args": [],
#     "problemMatcher": [],
#     "group": "test"
# },
# {
#     "label": "Build-Beta",
#     "type": "shell",
#     "command": "scripts/Build-Release.ps1 ",
#     "args": [],
#     "problemMatcher": [],
#     "group": "test"
# }



# set product names, allows mapping of (devel) name in Project to a more consistent name in release
$sourceName="Logikmodul"  
$targetName="LogicModule" 

# Release indication, set according names for Release or Beta
$releaseIndication = $args[0]
if ($releaseIndication) {
    $releaseName="$sourceName-$releaseIndication"
    $appRelease=$releaseIndication
} else {
    $releaseName="$sourceName"
    $appRelease="Beta"
}

# check and cleanup working dir
if (Test-Path -Path release) {
    # clean working dir
    Remove-Item -Recurse release\*
} else {
    New-Item -Path release -ItemType Directory | Out-Null
}

# create required directories
Copy-Item -Recurse ../OGM-Common/setup-scripts/reusable/data release

# get xml for kxnprod, always first step which also generates headerfile for release
~/bin/OpenKNXproducer.exe create --Debug --Output="release/$targetName.knxprod" --HeaderFileName="src/$sourceName.h" "src/$releaseName.xml"
if (!$?) {
    Write-Host "Error in knxprod, Release was not built!"
    exit 1
}
Move-Item "src/$releaseName.debug.xml" "release/data/$targetName.xml"

# build firmware based on generated headerfile 

# build generic RP2040 firmware (currently just for tests)
../OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_RP2040 firmware-Sensormodul-RP2040 uf2
if (!$?) { exit 1 }

# build firmware for PiPico-BCU-Connector
../OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_PiPico_BCU_Connector firmware-PiPico-BCU-Connector uf2
if (!$?) { exit 1 }

# build firmware based on generated headerfile for SAMD
../OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_SAMD_v31 firmware-Sensormodul-v31 bin
if (!$?) { exit 1 }

../OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_SAMD_v30 firmware-Sensormodul-v30 bin
if (!$?) { exit 1 }

# add necessary scripts
Copy-Item ../OGM-Common/setup-scripts/reusable/Readme-Release.txt release/
Copy-Item ../OGM-Common/setup-scripts/reusable/Build-knxprod.ps1 release/
Copy-Item scripts/Upload-Firmware*.ps1 release/

# add optional files
if (Test-Path -Path scripts/Readme-Hardware.html -PathType Leaf) {
    Copy-Item scripts/Readme-Hardware.html release/
}

# cleanup
Remove-Item "release/$targetName.knxprod"

# calculate version string
$appVersion=Select-String -Path src/$sourceName.h -Pattern MAIN_ApplicationVersion
$appVersion=$appVersion.ToString().Split()[-1]
$appMajor=[math]::Floor($appVersion/16)
$appMinor=$appVersion%16
$appRev=Select-String -Path src/main.cpp -Pattern "const uint8_t firmwareRevision"
$appRev=$appRev.ToString().Split()[-1].Replace(";","")
$appVersion="$appMajor.$appMinor"
if ($appRev -gt 0) {
    $appVersion="$appVersion.$appRev"
}

# create package 
Compress-Archive -Path release/* -DestinationPath Release.zip
Remove-Item -Recurse release/*
Move-Item Release.zip "release/$targetName-$appRelease-$appVersion.zip"

Write-Host "Release $targetName-$appRelease-$appVersion successfully created!"
