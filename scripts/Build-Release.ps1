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
$settings = scripts/OpenKNX-Build-Settings.ps1 $args[0]
$releaseIndication = $settings["appRelease"]

# execute generic pre-build steps
lib/OGM-Common/scripts/setup/reusable/Build-Release-Preprocess.ps1 $args[0]
if (!$?) { exit 1 }

if ($releaseIndication -eq "Beta") {
    # build BETA firmware

    # build firmware for 1TE-RP2040-SmartMF
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_1TE_REG firmware-1TE-RP2040-SmartMF uf2
    if (!$?) { exit 1 }

    # build firmware for PiPico-BCU-Connector
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_PIPICO_BCU_CONNECTOR firmware-PiPico-BCU-Connector uf2
    if (!$?) { exit 1 }

}

if ($releaseIndication -eq "Release") {
    # ESP32
    # build firmware for REG1_LAN_TP_BASE
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG1_LAN_TP_Base firmware-REG1-LAN-TP-BASE esp32
    if (!$?) { exit 1 }

    # RP2040
    # build firmware for OpenKNX-REG1-Base-V0
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG1_BASE_V0 firmware-OpenKNX-REG1-BASE-V0 uf2
    if (!$?) { exit 1 }

    # build firmware for OpenKNX-REG1-Base-V1
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG1_BASE firmware-OpenKNX-REG1-BASE uf2
    if (!$?) { exit 1 }

    # build firmware for PiPico-BCU-Connector
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_PIPICO_BCU_CONNECTOR firmware-PiPico-BCU-Connector uf2
    if (!$?) { exit 1 }

    # build firmware for SEN_UP1_8XTH
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SEN_UP1_8XTH firmware-SEN-UP1-8xTH uf2
    if (!$?) { exit 1 }

    # build firmware for REG1-SEN-Multi
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG1_SEN_MULTI firmware-REG1-Multisensor uf2
    if (!$?) { exit 1 }

    # build firmware for SmartMF-S0-Zaehlermodul
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_1TE_BE_3CH firmware-SmartMF-S0-Zaehlermodul uf2
    if (!$?) { exit 1 }

    # build firmware for SmartMF-eHZ-Schnittstelle
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_2SML_3BE firmware-SmartMF-eHZ-Schnittstelle uf2
    if (!$?) { exit 1 }

    # build firmware for 1TE-RP2040-SmartMF
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_1TE_REG firmware-1TE-RP2040-SmartMF uf2
    if (!$?) { exit 1 }

    # build firmware for REG2-PiPico-V1-DeviceDisplay-Meter
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG2_PIPICO_V1_DD_METER firmware-REG2-PiPico-V1-DeviceDisplay-Meter uf2
    if (!$?) { exit 1 }

    # # build firmware for RP2040 sensormodule breakout board
    # lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_Breakout_RP2040 firmware-Sensormodul-Breakout-RP2040 uf2 Sensormodul-Breakout-Board-Just-for-testers
    # if (!$?) { exit 1 }

    # build firmware for RP2040 sensormodule
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_SENSORMODUL firmware-SmartMF-Sensormodul-RP2040 uf2
    if (!$?) { exit 1 }

    # build firmware for SmartMF-RealPresence-V2
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_SMARTMF_REALPRESENCE_V2 firmware-SmartMF-RealPresence-V2 uf2
    if (!$?) { exit 1 }

    # # build firmware based on generated headerfile for SAMD
    # lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_v31_SAMD firmware-Sensormodul-v31-SAMD bin Sensormodul-v3.1-SAMD
    # if (!$?) { exit 1 }

    # lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_Sensormodul_v30_SAMD firmware-Sensormodul-v30-SAMD bin Sensormodul-v3.0-SAMD
    # if (!$?) { exit 1 }

    # build firmware for REG1_BASE_IP
    lib/OGM-Common/scripts/setup/reusable/Build-Step.ps1 release_DEVICE_REG1_BASE_IP firmware-REG1-BASE-IP uf2
    if (!$?) { exit 1 }
}

# # TEMPORARY: We use our own generic updload files for this version
# Copy-Item scripts/data/* release/data -Force

# execute generic post-build steps
lib/OGM-Common/scripts/setup/reusable/Build-Release-Postprocess.ps1 $args[0]
if (!$?) { exit 1 }

if (Test-Path -Path release-collection -PathType Container) {
    Copy-Item release/* release-collection/
}
