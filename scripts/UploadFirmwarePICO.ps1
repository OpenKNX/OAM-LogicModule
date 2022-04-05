Write-Host Suche COM-Port fuer RP2040...
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
        Write-Host Verwende $port
        mode ${port}: BAUD=1200 parity=N data=8 stop=1 | Out-Null
        ~\bin\rp2040load.exe -v -D firmware
    } else {
       Write-Host Kein Port gefunden!
    }
}
timeout /T 20 