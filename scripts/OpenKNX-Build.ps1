param (
  [Parameter(Mandatory = $false, HelpMessage="Initiate debug build, -DebugBuild should be first odr last parameter")]
  [switch]$DebugBuild,

  [Parameter(Mandatory = $false, HelpMessage="-Monitor starts pio monitor after build, env useCurrent is supported")]
  [switch]$Monitor,

  [Parameter(Mandatory = $true)]
  [ValidateNotNullOrEmpty()]
  [string]$env,

  [Parameter(Mandatory = $false)]
  [string]$target
)

# function SearchComPortForDevice($name, $vid) {
#     Write-Host "Suche COM-Port fuer $name (VID: $vid)"
#     $portList = get-pnpdevice -class Ports
#     if ($portList) {
#         foreach ($usbDevice in $portList) {
#             if ($usbDevice.Present) {
#                 # $isDevice = $usbDevice.InstanceId.StartsWith("USB\VID_$vid")
#                 $pattern = "USB\\VID_($vid)"
#                 $isDevice = $usbDevice.InstanceId -match $pattern
#                 $isCom = $usbDevice.Name -match "COM\d{1,3}"
#                 if ($isDevice -and $isCom) {
#                     # $port = $Matches[1]
#                     $port = $Matches[0]
#                     Write-Host "COM-Port Gefunden: $port" -ForegroundColor Green
#                     return $port
#                 }
#             }
#         }
#     }
#     Write-Host "Keinen COM-Port für $name mit VID_$vid gefunden!"
#     return $null
# }

function SearchComPortForDevice {
    param(
        [Parameter(Mandatory = $true)]
        [string]$name,
        [Parameter(Mandatory = $true)]
        [string]$vid
    )
    Write-Host "Suche COM-Port fuer $name (PID: $vid)"

    # PlatformIO JSON abrufen und in Objekte umwandeln
    $devices = ~/.platformio/penv/Scripts/pio.exe device list --json-output | ConvertFrom-Json

    # Filtern: hwid enthält USB + VID + gewünschte PID
    $match = $devices |
        Where-Object {
            $_.hwid -match "USB" -and
            $_.hwid -match "VID" -and
            $_.hwid -match "PID=$vid"
        }

    if (-not $match) {
        Write-Host "Keinen COM-Port für $name mit PID=$vid gefunden!"
        return $null
    }
    Write-Host "COM-Port Gefunden: $($match.port)" -ForegroundColor Green
    # COM-Port zurückgeben
    return $match.port
}

function StopPioMontor() {
    Write-Host "Beende den Monitor"
    $processList = Get-CimInstance Win32_Process | Where-Object { $_.CommandLine -like "*device monitor*" -and $_.CommandLine -like "*python*" -and $_.CommandLine -like "*--environment $env*"} 
    if ($processList.Count -eq 0) {
        Write-Host "Kein laufender Monitor-Prozess gefunden."
        return $false
    }
    $processList | ForEach-Object { Stop-Process -Id $_.ProcessId }    
    return $true
}

$currentEnv = ""
$currentProcessor = ""
$firmwarePath = ""
$launchFile = ".vscode\launch.json"

try {
    $content = Get-Content $launchFile -Raw
    $entries = [regex]::Matches($content, '"projectEnvName": "(.*)",')
    $currentEnv = $entries.Groups[1].Value
    Write-Output("Pio Env: $currentEnv")
    $entries = [regex]::Matches($content, '"toolchainBinDir": ".*(esp|rp2040).*",')
    $currentProcessor = $entries.Groups[1].Value
    Write-Output("Processor: $currentProcessor")
    $entries = [regex]::Matches($content, '"executable": "(.*)",')
    $firmwarePath = $entries.Groups[1].Value
    Write-Output("Firmware: $firmwarePath")
} catch {
    Write-Error "Fehler beim Parsen der $launchFile-Datei: $_"
    exit 1
}

if ($env -eq "useCurrent") { $env = $currentEnv }

if ($Monitor) {
    Write-Host "Starte den Monitor..."
    ~/.platformio/penv/Scripts/pio.exe device monitor --environment $env
    exit 0
}

if ($DebugBuild -or $target -eq "uploadOnly") {
    if ($target -ne "uploadOnly") {
        lib/OGM-Common/scripts/build/OpenKNX-Build.ps1 -DebugBuild -env $env
        if (!$?) { exit 1 }
    }
    if ($target -eq "upload" -or $target -eq "uploadOnly") {
        $success = $true
        StopPioMontor | Out-Null
        if ($currentProcessor -eq "rp2040") {
            $device = $(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2|RP2350" }) | Select-Object -First 1

            if (!$device) {
                Write-Host "Keinen RP2040/RP2350 im BOOTSEL-Modus gefunden."
                Write-Host
                $port = SearchComPortForDevice "RP2040" "2E8A"
                if ($port) {

                    Write-Host "Informiere die Firmware ueber bevorstehenden Upload (Speichere Zustaende)."
                    $serial = new-Object System.IO.Ports.SerialPort $port, 115200, None, 8, 1
                    try {
                        $serial.Open()
                        $serial.Write([byte[]] (7), 0, 1)
                        Start-Sleep -Seconds 1
                    }
                    catch {
                        Write-Host "Fehler beim Senden des BOOTSEL-Befehls an die Firmware, COM-Port ist bereits in Benutzung." -ForegroundColor Red
                        $success = $false
                    }
                    finally {
                        if ($serial.IsOpen) {
                            $serial.Close();
                        }
                    }
                    Write-Host
                    if ($success) {
                        Write-Host "Versuche ueber Port $port in den BOOTSEL-Modus zu versetzen..."
                        $serial = new-Object System.IO.Ports.SerialPort $port, 1200, None, 8, 1
                        try {
                            $serial.Open()
                        }
                        catch {}
                        finally {
                            if ($serial.IsOpen) {
                                $serial.Close();
                            }
                        }
                        # mode ${port}: BAUD=1200 parity=N data=8 stop=1 | Out-Null
                        Start-Sleep -s 1
                        # ./rp2040load.exe -v -D firmware
                        $device = $(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2|RP2350" }) | Select-Object -First 1
                    }
                }
            }
            Write-Host
            if ($device) {
                Write-Host "RP2040/RP2350 gefunden, installiere Firmware..." -ForegroundColor Yellow
                Write-Host
                ~/bin/picotool.exe load -v -x $firmwarePath
            } 
        } elseif ($currentProcessor -eq "esp") {
            $port = SearchComPortForDevice "ESP32" "1A86|303A"
            Write-Host "ESP32 gefunden, installiere Firmware..." -ForegroundColor Yellow
            Write-Host
            $firmwarePath = $firmwarePath -replace ".elf", ".factory.bin"
            if ($port) {
                # we found the correct port, update ist faster
                ~/bin/esptool.exe --port "$port" --baud 460800 write_flash 0x0 $firmwarePath
            } else {
                # we try to find correct port with esptool
                ~/bin/esptool.exe --baud 460800 write_flash 0x0 $firmwarePath
            }
        }
        if (-not $success) {
            Write-Host "Firmware-Upload abgebrochen, eventuell Moitor-Modus beenden!" -ForegroundColor Red
        }
        # if ($monitorStarted) {
            # Write-Host "Starte den Monitor neu..."
            # ~/.platformio/penv/Scripts/pio.exe device monitor --environment $env
        # }
    }
}
else {
    lib/OGM-Common/scripts/build/OpenKNX-Build.ps1 -env $env -target $target
}
