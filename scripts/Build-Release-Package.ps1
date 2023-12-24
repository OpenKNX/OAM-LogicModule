# This script should be called from .vscode/tasks.json with
# {
#     "label": "Build-Release-Package",
#     "type": "shell",
#     "command": "scripts/Build-Release-Package.ps1",
#     "args": [
#     ],
#     "problemMatcher": [],
#     "group": "test"
# },

# check and cleanup working dir
if (Test-Path -Path release-package) {
    # clean working dir
    Remove-Item -Recurse release-package\*
}
else {
    New-Item -Path release-package -ItemType Directory | Out-Null
}

# list here all release variants you want to release as a package
scripts/Build-IP.ps1 IP
if (!$?) { exit 1 }
Copy-Item release/* release-package

scripts/Build-Release.ps1 Release
if (!$?) { exit 1 }
Copy-Item release/* release-package

Write-Host "Release package sucessfully created!"