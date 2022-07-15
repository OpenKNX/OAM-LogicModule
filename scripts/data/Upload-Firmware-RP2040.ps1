Import-Module BitsTransfer

$firmwareName = $args[0]

Write-Host Suche RP2040 als Disk-Laufwerk...
$device=$(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2" })
if (!$device)
{
    Write-Host Nicht gefunden, Suche COM-Port fuer RP2040...
    $portList = get-pnpdevice -class Ports
    if ($portList) {
        foreach($device in $portList) {
            if ($device.Present) {
                $isCom = $device.Name -match "COM(\d{1,3})"
                if($isCom)
                {
                    Write-Host Gefunden $port
                    $port = $Matches[0]
                    break
                }
            }
        }
        if($port)
        {
            Write-Host Verwende $port zum neustart vom RP2040
            $serial = new-Object System.IO.Ports.SerialPort $port,1200,None,8,1
            try { $serial.Open()} catch {}
            $serial.Close()
            # mode ${port}: BAUD=1200 parity=N data=8 stop=1 | Out-Null
            Start-Sleep -s 1
            # ./rp2040load.exe -v -D firmware
            $device=$(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2" })
        }
    }
}
if ($device)
{
    Write-Host Installiere firmware...
    Start-BitsTransfer -Source data/$firmwareName -Destination $device.DeviceID.ToString() -Description "Installiere" -DisplayName "Installiere Firmware..."
    # Copy-Item data/firmware.uf2 $device.DeviceID.ToString()
     Write-Host Fertig!
}
else 
{
    Write-Host Kein RP2040 gefunden!
}
timeout /T 20 