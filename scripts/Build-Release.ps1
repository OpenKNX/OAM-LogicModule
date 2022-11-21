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
lib/OGM-Common/setup-scripts/reusable/Build-Release-Preprocess.ps1 $args[0]
if (!$?) { exit 1 }

# build firmware based on generated headerfile 

# build generic RP2040 firmware (currently just for tests)
lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_RP2040 firmware-Sensormodul-RP2040 uf2
if (!$?) { exit 1 }

# build firmware for PiPico-BCU-Connector
lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_PiPico_BCU_Connector firmware-PiPico-BCU-Connector uf2
if (!$?) { exit 1 }

# build firmware for 1TE-RP2040-SmartMF
lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_1TE_RP2040_SmartMF firmware-1TE-RP2040-SmartMF uf2
if (!$?) { exit 1 }

# build firmware for OpenKNX-REG1-Base
lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_OpenKNX_REG1_Base firmware-OpenKNX-REG1-Base uf2
if (!$?) { exit 1 }

# build firmware based on generated headerfile for SAMD
lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_SAMD_v31 firmware-Sensormodul-v31 bin
if (!$?) { exit 1 }

lib/OGM-Common/setup-scripts/reusable/Build-Step.ps1 release_Sensormodul_SAMD_v30 firmware-Sensormodul-v30 bin
if (!$?) { exit 1 }

# execute generic post-build steps
lib/OGM-Common/setup-scripts/reusable/Build-Release-Postprocess.ps1 $args[0]
if (!$?) { exit 1 }
