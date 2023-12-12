# we assume, we start this script in projects "restore" directory
$currentDir = Get-Location
Set-Location ..
$subprojects = Get-ChildItem -File lib
$subprojects_dependencies = Get-Content dependencies.txt
$projectDir = Get-Location
# we first check, if we are in Admin- or Developer mode and allowed to create symlinks:
cmd /c "mklink /D `"$projectDir\lib\linktest`" ..\restore"
if (!$?) { 
    Write-Host "You need Developer-Mode or Administrator privileges to execute this script!"
    Set-Location $currentDir
    timeout /T 20
    exit 1 
}
# cleanup after test
cmd /c "rmdir `"$projectDir\lib\linktest`""
if (!$?) {
    Set-Location $currentDir
    exit 1
}

Set-Location ..
foreach ($line in $subprojects_dependencies) {
    $attr = $line.Split()
    $hash = $attr[0]
    $branch = $attr[1]
    $subproject = $attr[2].Substring(4, $attr[2].Length-4);
    $repo_url = $attr[3]
    if ($attr[0] -ne "-------") {
        $cloned = 0
        if ($subproject.Name -ne "README")
        {
            # the following command may fail, because the project might already have been cloned
            git clone $repo_url
            if (!$?) { 
                Write-Host -ForegroundColor Yellow "If the above message was 'fatal: ... already exists' then you can ignore this message, the project is already cloned"
            }
            $cloned = 1
        }

        # check if $projectDir\lib\$subproject exists
        $renamed = $false
        if(Test-Path -Path $projectDir/lib/$subproject) {
            Rename-Item $projectDir/lib/$subproject $projectDir/lib/tmp-openknx-restore
            if (!$?) { 
                Set-Location $currentDir
                exit 1 
            }
            $renamed = $true
        }
        cmd /c "mklink /D `"$projectDir\lib\$subproject`" ..\..\$subproject"
        if($renamed)
        {
            if ($?) { 
                # link creation successful, now we can safely delete old renamed file
                Remove-Item -Recurse $projectDir/lib/tmp-openknx-restore
                if (!$?) { 
                    Set-Location $currentDir
                    exit 1 
                }
            } else {
                # link creation failed, we restore previous state and leave script
                Rename-Item $projectDir/lib/tmp-openknx-restore $projectDir/lib/$subproject 
                Set-Location $currentDir
                exit 1 
            }
        }
    }
}
if ($subprojects.Count -le 1) {
    Write-Host "Everything is fine, project is already in expected state"
}
Set-Location $currentDir
timeout /T 20
