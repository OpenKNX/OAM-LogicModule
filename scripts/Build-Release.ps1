# Release indication
$releaseIndication = $args[0]
# set product names
$targetName="LogicModule"
$sourceName="Logikmodul"
if ($releaseIndication) {
    $releaseName="$sourceName-$releaseIndication"
    $appRelease=$releaseIndication
} else {
    $releaseName="$sourceName"
    $appRelease="Beta"
}

# check for working dir
if (Test-Path -Path release) {
    # clean working dir
    Remove-Item -Recurse release\*
} else {
    New-Item -Path release -ItemType Directory | Out-Null
}

# create required directories
Copy-Item -Recurse scripts/data release

# get xml for kxnprod
~/bin/OpenKNXproducer.exe create --Debug --Output="release/$targetName.knxprod" --HeaderFileName="src/$sourceName.h" "src/$releaseName.xml"
if (!$?) {
    Write-Host "Error in knxprod, Release was not built!"
    exit 1
}
Move-Item "src/$releaseName.debug.xml" "release/data/$targetName.xml"

# build firmware based on generated headerfile for PICO
scripts/Build-Step.ps1 release_RP2040 firmware uf2
if (!$?) { exit 1 }

# build firmware based on generated headerfile for SAMD
scripts/Build-Step.ps1 release_SAMD_v31 firmware-v31 bin
if (!$?) { exit 1 }

scripts/Build-Step.ps1 release_SAMD_v30 firmware-v30 bin
if (!$?) { exit 1 }

# add necessary scripts
Copy-Item scripts/Readme-Release.txt release/
Copy-Item scripts/Build-knxprod.ps1 release/
Copy-Item scripts/Upload-Firmware*.ps1 release/

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
