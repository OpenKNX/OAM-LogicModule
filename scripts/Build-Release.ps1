
# Define target hardware
$opknxhw = @(
    @{Target_Ini= "platformio.custom.ini"; OAM_Support_ToMatch = "oam_logicmodule" } 
)

# Define firmware build information
$builds = @(
    @{Release="release_PiPico_BCU_Connector"; Firmware="firmware-PiPico-BCU-Connector"; Type="uf2"},
    @{Release="release_OKNXHW_REG2_PiPico"; Firmware="firmware-OKNXHW-REG2-PiPico"; Type="uf2"},
    @{Release="release_OKNXHW_REG2_PiPico_W"; Firmware="firmware-OpenKNX-REG2-PiPico_W"; Type="uf2"}
    @{Release="release_1TE_RP2040_SmartMF"; Firmware="firmware-1TE-RP2040-SmartMF"; Type="uf2"},
    @{Release="release_OpenKNX_REG1_Controller2040"; Firmware="firmware-OpenKNX-REG1-BASE-V0"; Type="uf2"},
    @{Release="release_OpenKNX_REG1_BASE_V1"; Firmware="firmware-OpenKNX-REG1-BASE-V1"; Type="uf2"},
    @{Release="release_Sensormodul_Breakout_RP2040"; Firmware="firmware-Sensormodul-Breakout-RP2040"; Type="uf2"; Extra="Sensormodul-Breakout-Board-Just-for-testers"},
    @{Release="release_Sensormodul_v40_RP2040"; Firmware="firmware-Sensormodul-v40-RP2040"; Type="uf2"},
    @{Release="release_Sensormodul_v31_SAMD"; Firmware="firmware-Sensormodul-v31-SAMD"; Type="bin"},
    @{Release="release_Sensormodul_v30_SAMD"; Firmware="firmware-Sensormodul-v30-SAMD"; Type="bin"}
)

# Build firmware for each defined build
$parentDir = Split-Path -Path $PSScriptRoot -Parent
Write-Host "ScriptPath: $parentDir"
lib/OGM-Common/scripts/setup/reusable/Build-Release.ps1 -CustomArgument $args[0] -builds $builds -opknxhws $opknxhw -projectDir $parentDir
if (!$?) { exit 1 }
