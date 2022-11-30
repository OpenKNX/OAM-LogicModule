# we assume, we start this script in projects "restore" directory
$currentDir = Get-Location
Set-Location ..
$subprojects = Get-ChildItem -File lib
$projectDir = Get-Location
Set-Location ..
foreach ($subproject in $subprojects) {
    $cloned = 0
    if ($subproject.Name -eq "knx")
    {
        git clone https://github.com/thelsing/knx.git
        $cloned = 1
    }
    elseif ($subproject.Name -ne "README")
    {
        git clone https://github.com/OpenKNX/$subproject.git
        $cloned = 1
    }
    if ($cloned)
    {
        Remove-Item $projectDir/lib/$subproject
        # this has to be a cmd-shell to work in developer mode
        cmd /c "mklink /D $projectDir\lib\$subproject ..\..\$subproject"
    }
}
if ($subprojects.Count -le 1) {
    Write-Host "Everything is fine, project is already in expected state"
}
Set-Location $currentDir
timeout /T 20
