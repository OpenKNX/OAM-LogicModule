$checkVersion = "1.5.8"
$toolsExist = Test-Path -PathType Leaf ~/bin/OpenKNXproducer.exe
if ($toolsExist) {
    $toolsExist = [System.Version]((~/bin/OpenKNXproducer version) -split ' ')[1] -ge [System.Version]$checkVersion
}
if ($toolsExist) {
    $toolsExist = Test-Path -PathType Leaf ~/bin/bossac.exe
}
if (!$toolsExist) {
    Write-Host "
        Fuer das Setup fehlen die notwendigen OpenKNX-Tools oder sie sind veraltet..
        Bitte das neuste Paket herunterladen

            https://github.com/OpenKNX/OpenKNXproducer/releases
        
        entpacken und das Readme befolgen. Weitere Informationen hierzu gibt es im OpenKNX-Wiki

            https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-tools

        Danach bitte dieses Script erneut starten.

        ACHTUNG: Heutige Browser warnen vor dem Inhalt des OpenKNX-Tools Pakets, 
                 weil es ausfuehrbare Programme und ein PowerShell-Skript enthaellt.
                 Falls jemand dem Paketinhalt nicht traut, kann er sich OpenKNXproducer
                 und bossac selber kompilieren und entsprechend installieren.
    "
    timeout /T -1
}

if ($toolsExist) {
    ~/bin/OpenKNXproducer.exe knxprod --NoXsd --Output=./LogicModule.knxprod data/LogicModule.xml
    timeout /T 20 
}
