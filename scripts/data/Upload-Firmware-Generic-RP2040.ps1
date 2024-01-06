# Import-Module BitsTransfer
function OpenKNX_ShowLogo($AddCustomText = $null) {
    Write-Host ""
    Write-Host "Open " -NoNewline
    #Write-Host "■" -ForegroundColor Green
    Write-Host "$( [char]::ConvertFromUtf32(0x25A0) )" -ForegroundColor Green
    $unicodeString = "$( [char]::ConvertFromUtf32(0x252C) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2534) ) "
  
    if ($AddCustomText) { 
        #Write-Host "┳━━━━┻  $AddCustomText" -ForegroundColor Green
        Write-Host "$($unicodeString) $($AddCustomText)"  -ForegroundColor Green
    }
    else {
        #Write-Host "┳━━━━┻" -ForegroundColor Green
        Write-Host "$($unicodeString)"  -ForegroundColor Green
    }

    #Write-Host "■" -NoNewline -ForegroundColor Green
    Write-Host "$( [char]::ConvertFromUtf32(0x25A0) )" -NoNewline -ForegroundColor Green
    Write-Host " KNX"
    Write-Host ""
}

$firmwareName = $args[0]
if (!($isRecursive -eq "1")) {
    OpenKNX_ShowLogo "Upload Firmware RP2040"
} 

Write-Host "Suche RP2040 im BOOTSEL-Modus (das kann einige Zeit dauern)..."
$device = $(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2" })
if (!$device) {
    Write-Host "Keinen RP2040 im BOOTSEL-Modus gefunden."
    Write-Host "Alternative: Suche COM-Port fuer RP2040 (auch das kann etwas dauern)..."
    $portList = get-pnpdevice -class Ports
    if ($portList) {
        foreach ($usbDevice in $portList) {
            if ($usbDevice.Present) {
                $isPico = $usbDevice.InstanceId.StartsWith("USB\VID_2E8A")
                # $isCom = $usbDevice.Name -match "USB.*\((COM\d{1,3})\)"
                $isCom = $usbDevice.Name -match "COM\d{1,3}"
                if ($isPico) {
                    # $port = $Matches[1]
                    $port = $Matches[0]
                    Write-Host "COM-Port Gefunden: $port"
                    break
                }
            }
        }
        if ($port) {
            Write-Host "Versuche den RP2040 ueber Port $port in den BOOTSEL-Modus zu versetzen..."
            $serial = new-Object System.IO.Ports.SerialPort $port, 1200, None, 8, 1
            try { $serial.Open() } catch {}
            $serial.Close()
            # mode ${port}: BAUD=1200 parity=N data=8 stop=1 | Out-Null
            Start-Sleep -s 1
            # ./rp2040load.exe -v -D firmware
            $device = $(Get-WmiObject Win32_LogicalDisk | Where-Object { $_.VolumeName -match "RPI-RP2" })
        }
    }
}
if ($device) {
    Write-Host "RP2040 gefunden, installiere Firmware..."
    # There are different options how to copy a large file, but most of them have side effects
    
    # the following one prints very often errors AFTER the file was copied 
    # Start-BitsTransfer -Source data/$firmwareName -Destination $device.DeviceID.ToString() -Description "Installiere" -DisplayName "Installiere Firmware..."

    # the following has no progress bar
    # Copy-Item data/$firmwareName $device.DeviceID.ToString()

    # the following is just Windows based, not pure PowerShell
    # cmd /c copy /z data\$firmwareName $device.DeviceID.ToString()

    # the following is just windows but looks best...
    $FOF_CREATEPROGRESSDLG = "&H0&"
    $currentDir = (Get-Item .).FullName
    $objShell = New-Object -ComObject "Shell.Application"
    $objFolder = $objShell.NameSpace($device.DeviceID.ToString()) 
    $objFolder.CopyHere("$currentDir\data\$firmwareName", $FOF_CREATEPROGRESSDLG)
    Write-Host Fertig!
    timeout /T 20 
}
else {
    Write-Host 
    Write-Host "Kein RP2040 gefunden!"
    Write-Host 
    Write-Host "Versuche bitte die manuelle Setup-Methode: Den RP2040 selber im BOOTSEL-Modus zu starten"
    Write-Host "Falls die Hardware eine Reset-Taste hat, dann erst die BOOTSEL-Taste druecken und halten,"
    Write-Host "und dann zusaetzlich die Reset-Taste druecken. Dann beide Tasten loslassen."
    Write-Host "Ohne Reset-Taste das Geraet stromlos machen (USB-Stecker ziehen und vom KNX trennen),"
    Write-Host "Danach die BOOTSEL-Taste druecken und gleichzeitig USB mit dem Recner verbinden."
    Write-Host "Jetzt befindet sich das Geraet im BOOTSEL-Modus, die BOOTSEL-Taste kann jetzt losgelassen werden."
    Write-Host "Anschliessend das Skript erneut starten."
    timeout /T 60     
}
