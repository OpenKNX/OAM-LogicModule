# get all definitions for this project
$settings = scripts/OpenKNX-Build-Settings.ps1 $args[0]

# result structure
$result = @{}

# calculate version string
$result.OpenKnxIdHex=Select-String -Path "$($settings.knxprod)" -Pattern MAIN_OpenKnxId
$result.OpenKnxIdHex=$result.OpenKnxIdHex.ToString().Split()[-1]
$result.OpenKnxId=[Int32]$result.OpenKnxIdHex
$result.AppNumber=Select-String -Path "$($settings.knxprod)" -Pattern MAIN_ApplicationNumber
$result.AppNumber=$result.AppNumber.ToString().Split()[-1]
$result.AppVersion=Select-String -Path "$($settings.knxprod)" -Pattern MAIN_ApplicationVersion
$result.AppVersion=$result.AppVersion.ToString().Split()[-1]
$result.AppVersionHex=([Int32]$result.AppVersion).ToString("X2")
$result.AppMajor=[math]::Floor($result.AppVersion/16)
$result.AppMinor=$result.AppVersion%16
$result.AppRev=Select-String -Path src/main.cpp -Pattern "const uint8_t firmwareRevision"
$result.AppRev=$result.AppRev.ToString().Split()[-1].Replace(";","")
$result.AppRevHex=([Int32]$result.AppRev).ToString("X2")
$result.AppVersionString="$($result.AppMajor).$($result.AppMinor)"
if ($result.AppRev -gt 0) {
    $result.AppVersionString="$($result.AppVersionString).$($result.AppRev)"
}
$result.AppFullVersionString="$($result.OpenKnxId)$($result.AppNumberHex)-$($result.AppVersionString)" 
$result.UF2Version="$($result.OpenKnxId),$($result.AppNumber),$($result.AppVersion),$($result.AppRev),"

return $result