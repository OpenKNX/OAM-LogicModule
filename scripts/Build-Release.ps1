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
# $settings = scripts/OpenKNX-Build-Settings.ps1

# execute generic pre-build steps
lib/OGM-Common/scripts/setup/reusable/Build-Release-Preprocess.ps1 $args[0]
if (!$?) { exit 1 }

# build firmware based on generated headerfile 

# build firmware for PiPico-BCU-Connector
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_PiPico_BCU_Connector firmware-PiPico-BCU-Connector uf2
if (!$?) { exit 1 }

# build firmware for 1TE-RP2040-SmartMF
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_1TE_RP2040_SmartMF firmware-1TE-RP2040-SmartMF uf2
if (!$?) { exit 1 }

# build firmware for OpenKNX-REG1-Base
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_OpenKNX_REG1_Controller2040 firmware-OpenKNX-REG1-BASE-V0 uf2
if (!$?) { exit 1 }

# build firmware for OpenKNX-REG1-Base
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_OpenKNX_REG1_BASE_V1 firmware-OpenKNX-REG1-BASE-V1 uf2
if (!$?) { exit 1 }

# build firmware for RP2040 sensormodule breakout board
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_Breakout_RP2040 firmware-Sensormodul-Breakout-RP2040 uf2 Sensormodul-Breakout-Board-Just-for-testers
if (!$?) { exit 1 }

# build firmware for RP2040 sensormodule 
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_v40_RP2040 firmware-Sensormodul-v40-RP2040 uf2 Sensormodul-v4.x-RP2040
if (!$?) { exit 1 }

# build firmware based on generated headerfile for SAMD
lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_v31_SAMD firmware-Sensormodul-v31-SAMD bin Sensormodul-v3.1-SAMD
if (!$?) { exit 1 }

lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_v30_SAMD firmware-Sensormodul-v30-SAMD bin Sensormodul-v3.0-SAMD
if (!$?) { exit 1 }

# TEMPORARY: We use our own generic updload files for this version
Copy-Item scripts/data/* release/data -Force

# execute generic post-build steps
lib/OGM-Common/scripts/setup/reusable/Build-Release-Postprocess.ps1 $args[0]
if (!$?) { exit 1 }

if (Test-Path -Path release-collection -PathType Container) {
    Copy-Item release/* release-collection/
}
