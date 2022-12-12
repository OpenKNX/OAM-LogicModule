# we assume, we start this script in projects "restore" directory
$currentDir = Get-Location
Set-Location ..
$subprojects = Get-ChildItem -File lib
$projectDir = Get-Location
# we first check, if we are in Admin- or Developer mode and allowed to create symlinks:
cmd /c "mklink /D $projectDir\lib\linktest ..\restore"
if (!$?) { 
    Write-Host "You need Developer-Mode or Administrator privileges to execute this script!"
    timeout /T 20
    exit 1 
}
# cleanup after test
cmd /c "rmdir $projectDir\lib\linktest"
if (!$?) { exit 1 }

Set-Location ..
foreach ($subproject in $subprojects) {
    $cloned = 0
    if ($subproject.Name -eq "knx")
    {
        # the following command may fail, because the project might already have been cloned
        git clone https://github.com/thelsing/knx.git
        if (!$?) { 
            Write-Host -ForegroundColor Yellow "If the above message was 'fatal: ... already exists' then you can ignore this message, the project is already cloned"
        }
        $cloned = 1
    }
    elseif ($subproject.Name -ne "README")
    {
        # the following command may fail, because the project might already have been cloned
        git clone https://github.com/OpenKNX/$subproject.git
        if (!$?) { 
            Write-Host -ForegroundColor Yellow "If the above message was 'fatal: ... already exists' then you can ignore this message, the project is already cloned"
        }
        $cloned = 1
    }
    if ($cloned)
    {
        # here we have a potential problem case: 
        #  - to create a link with mklink, we have to remove the existing file first
        #  - if link creation fails, the file is missed, if we restart the script
        # for robustness, we rename first and delete only if link creation was successful
        Rename-Item $projectDir/lib/$subproject $projectDir/lib/tmp-openknx-restore
        if (!$?) { exit 1 }
        # this has to be a cmd-shell to work in developer mode
        cmd /c "mklink /D $projectDir\lib\$subproject ..\..\$subproject"
        if ($?) { 
            # link creation successful, now we can safely delete old renamed file
            Remove-Item $projectDir/lib/tmp-openknx-restore
            if (!$?) { exit 1 }
        } else {
            # link creation failed, we restore previous state and leave script
            Rename-Item $projectDir/lib/tmp-openknx-restore $projectDir/lib/$subproject 
            exit 1 
        }
    }
}
if ($subprojects.Count -le 1) {
    Write-Host "Everything is fine, project is already in expected state"
}
Set-Location $currentDir
timeout /T 20
