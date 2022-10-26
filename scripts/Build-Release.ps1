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

# execute generic pre-build steps
../OGM-Common/build-scripts/Build-Release-Preprocess.ps1 $sourceName $targetName $args[0]
if (!$?) { exit 1 }

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

# execute generic post-build steps
../OGM-Common/build-scripts/Build-Release-Postprocess.ps1 $sourceName $targetName $args[0]
if (!$?) { exit 1 }
