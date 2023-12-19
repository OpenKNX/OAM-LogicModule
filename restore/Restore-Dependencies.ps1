<#
FILEPATH: restore/Restore-Dependencies.ps1

This script is designed to automate the process of managing dependencies in a software project, making it easier to 
set up the project environment for development or deployment. It is structured in a way that allows for easy
modification and extension, and it can be used in a variety of different environments due to its platform-independent design.

Here's a high-level description of what it does:
  1. The script starts by checking the operating system it's running on. It sets up several variables 
    (IsWindows, IsLinux, IsMacOs) based on the operating system type. This information is used later in 
    the script to handle OS-specific operations. The script is designed to run platform-independently.

  2. It assumes that the script is started in the project's 'restore' directory. It suggests going one 
    directory back to get to the project directory.

  3. The script checks if a specific dependencies file (dependencies.txt) exists. If the file exists, 
    it reads the content of the file into a variable. This file contains information about the dependencies 
    that need to be cloned from git.

  4. For each dependency listed in the dependencies.txt file, the script performs a git clone operation 
    to download the dependency into a directory outside of the current project. It also checks out the 
    correct branch for each dependency as specified in the dependencies.txt file.

  5. The script checks if a 'lib' directory exists. If the directory does not exist, it creates the directory 
    and outputs a message to the user. It then gets the project files from the 'lib' directory.

  6. The script creates symbolic links for the cloned dependencies in the 'lib' subfolder. This allows the 
    project to reference the dependencies as if they were part of the project's own source code.

  7. The script checks if each project file exists. If a project file does not exist, it outputs a message to the user.

  8. Many core functions of the script are modularized and can be used outside of this script, providing flexibility and reusability.
#>
$Verbose=$false
$DebugMsg=$false

function CheckOS {
  # check on which os we are running
  # After check, the Os-Informations are availibe in the PS-Env.
  if ($PSVersionTable.PSVersion.Major -lt 6.0) {
    switch ($([System.Environment]::OSVersion.Platform)) {
      'Win32NT' {
        New-Variable -Option Constant -Name IsWindows -Value $True -ErrorAction SilentlyContinue
        New-Variable -Option Constant -Name IsLinux -Value $false -ErrorAction SilentlyContinue
        New-Variable -Option Constant -Name IsMacOs -Value $false -ErrorAction SilentlyContinue
      }
    }
  }
  $script:IsLinuxEnv = (Get-Variable -Name "IsLinux" -ErrorAction Ignore) -and $IsLinux
  $script:IsMacOSEnv = (Get-Variable -Name "IsMacOS" -ErrorAction Ignore) -and $IsMacOS
  $script:IsWinEnv = !$IsLinuxEnv -and !$IsMacOSEnv

  if ($IsLinuxEnv) { Write-Host -ForegroundColor Green "We are on Linux Build Enviroment" }
  if ($IsMacOSEnv ) { Write-Host -ForegroundColor Green "We are on MacOS Build Enviroment" }
  if ($IsWinEnv ) { Write-Host -ForegroundColor Green "We are on Windows Build Enviroment" }
}
function ProcessDependencies($DependenciesFile) {
  # Check if the file exists
  if (-not (Test-Path $DependenciesFile)) {
    if($Verbose) { Write-Host -ForegroundColor DarkYellow "The file $DependenciesFile does not exist." }
    return
  }

  # Get the content of the file
  $dependencies = Get-Content $DependenciesFile

  # Process each line and filter out invalid ones
  $lineNumber = 0
  $dependedProjects = foreach ($line in $dependencies) {
    $lineNumber++
    if ($line -notmatch '^-------' -and -not $line.StartsWith('#')) {
      $parts = $line -split ' ', 4
      if ($parts.Count -lt 4) {
        if($Verbose) { Write-Host -ForegroundColor DarkYellow "Dependencies: Error on line ${lineNumber}: Line does not have the expected format and will be skipped: $line" }
      } else {
        $hash, $branch, $folders, $urlParts = $parts
        $folders = $folders -split '/'
        $urlParts = $urlParts -split '#'
        $url = $urlParts[0]
        $branch = if($urlParts.Count -gt 1) { $urlParts[1] } else { $branch }
        # Extract the project name from the URL
        $urlParts = $url -split '/'
        $projectNameWithExtension = $urlParts[-1]
        $projectName = $projectNameWithExtension -split '\.' | Select-Object -First 1
        # Create a custom object for the project
        [PSCustomObject]@{
          "Hash" = $hash
          "Branch" = $branch
          "Folder" = $folders
          "FolderCount" = $folders.Count
          "URL" = $url
          "ProjectName" = $projectName
        }
      }
    }
  }

  return $dependedProjects
}
function Get-ProjectFiles($subprojects) {
  # Initialize an empty array for the project files
  $projectFiles = @()
  
  # Loop through each subproject
  foreach ($subproject in $subprojects) {
    # Find and Get the project file
      $projectFile = $subproject | Select-Object BaseName
      if($Verbose) { Write-Host "Get-ProjectFiles - Found existing project file: $($projectFile.BaseName)" -ForegroundColor Yellow }

    # Check if the file name is not "README" and is not empty
    if ($projectFile.BaseName -ne "README" -and ![string]::IsNullOrWhiteSpace($projectFile.BaseName)) {
      # Add the project file to the list
      $projectFiles += $projectFile
    }
  }

  # Return the list of project files
  return $projectFiles
}
function CloneRepository($projectFilesGitInfo, $dependedProjects, $CloneDir) {
  # Loop through each depended project
  foreach ($dependedProject in $dependedProjects) {
    # Initialize a flag to track if the project is found
    $found = $false
    $hashMatch = $false
    
    if($Verbose) { Write-Host "CloneRepository - Check: "$dependedProject.ProjectName -ForegroundColor Green }
    # Loop through each project file's git information
    foreach ($projectFile in $projectFilesGitInfo) {
      # Check if projectFile contains valid data
      if (($null -ne $projectFile.BaseName        -and $projectFile.BaseName        -ne "") -and 
          ($null -ne $dependedProject.ProjectName -and $dependedProject.ProjectName -ne "") -and
          (          $projectFile.BaseName      -match $dependedProject.ProjectName) )
      {
        if ($null -ne $projectFile.TargetBranch -and $projectFile.TargetBranch -ne "") {
          $found = $true
          if($Verbose) { Write-Host "CloneRepository - Found: "$dependedProject.ProjectName" - "$projectFile.Path -ForegroundColor Green }
          # Compare the TargetShortHash with depend hash
          if ($null -ne $projectFile.TargetShortHash -and $null -ne $dependedProject.Hash -and $projectFile.TargetShortHash -match $dependedProject.Hash) {
            # If the project is found, set the flag to true and break the loop
            $hashMatch = $true
            if($Verbose) { Write-Host "CloneRepository - Hashes mathing: "$dependedProject.ProjectName -ForegroundColor Green }
          }
          break
        }
      }
    }

    # If the project is not found, clone the repository
    if (-not $found) {
      # Create the Git clone URL with the URL and the Hash from the dependency
      $GitClone = ($dependedProject.URL).ToString()
      if($Verbose) { Write-Host "CloneRepository - $($dependedProject.ProjectName) not Found. Target does not exist." -ForegroundColor DarkYellow }
      # Try to clone the repository
      try {
        if($IsWinEnv){
          $CloneTarget = Join-Path $CloneDir $dependedProject.ProjectName
        } else {
          $CloneTarget = Join-Path -Path $CloneDir -ChildPath $dependedProject.ProjectName
        }
        if($Verbose) { Write-Host "CloneRepository - CloneTarget: " $CloneTarget -ForegroundColor Green }
        if($Verbose) { Write-Host "- CloneRepository - Cloning "$dependedProject.ProjectName": '$GitClone' to '$CloneTarget'" -ForegroundColor Yellow }
        Invoke-RestMethod -Uri $GitClone -Method Head -ErrorAction Stop; 
        git clone -q $GitClone $CloneTarget.ToString()
        if($true) { Write-Host "- CloneRepository - Cloning "$dependedProject.ProjectName": '$GitClone' to '$CloneTarget' Done"([Char]0x221A) -ForegroundColor Green }
      }
      # If the repository does not exist, catch the error
      catch {
        if($Verbose) { Write-Host "CloneRepository - Cloning Error: "$dependedProject.ProjectName"- Repository does not exist: "$GitClone -ForegroundColor DarkYellow }
      }
    } 
    # If the project is found, check out to the specific branch
    if(-not $hashMatch) 
    {
      if($Verbose) { Write-Host "CloneRepository - Found: "$dependedProject.ProjectName" - "$dependedProject.URL -ForegroundColor Green }
      
      # If the repository already exists, switch to the corresponding branch
      if($Verbose) { Write-Host "CloneRepository -"$dependedProject.ProjectName"- Repository already exists. Checking out to the branch." -ForegroundColor Yellow }
      
      try {
        # Check the installed Git version
        if($IsWinEnv){
          $CloneTarget = Join-Path $CloneDir $dependedProject.ProjectName
          $GitDir = Join-Path $CloneTarget ".git"
        } else {
          $CloneTarget = Join-Path -Path $CloneDir -ChildPath $dependedProject.ProjectName
          $GitDir = Join-Path -Path $CloneTarget.TargetPath -ChildPath ".git"
        }
        
        if($Verbose) { Write-Host "CloneRepository - Branch - GitDir: "$GitDir -ForegroundColor Yellow }
        $GitCmd = "git --git-dir=""$($GitDir)"" --work-tree=""$($CloneTarget.ToString())"""
        if($Verbose) { Write-Host "CloneRepository - Branch - GitCmd: "$GitCmd -ForegroundColor Yellow }

        if ((& git --version) -ge 'git version 2.23') {
          # If the Git version is 2.23 or higher, use the 'switch' command
          Invoke-Expression "$GitCmd switch $($dependedProject.Branch)"
        } else {
          # If the Git version is lower than 2.23, use the 'checkout' command
          Invoke-Expression "$GitCmd checkout $($dependedProject.Branch)"
        }
        if($true) { Write-Host "- CloneRepository -$($dependedProject.ProjectName) Branch ""$($dependedProject.Branch)"" Checked out."([Char]0x221A) -ForegroundColor Green }
      }
      # If cannot check out to the branch, catch the error
      catch {
        if($Verbose) { Write-Host "CloneRepository -"$dependedProject.ProjectName"- Checkout Error! Cannot checkout to branch: "$dependedProject.Hash -ForegroundColor DarkYellow }
      }
    }
  }
}
function Get-GitInfo($path) {
  if ($IsWinEnv) {
    $TargetDir= (Join-Path $path ".git").ToString()
  } else {
    $TargetDir= (Join-Path -Path $path -ChildPath ".git").ToString()
  }
  if($Verbose) { Write-Host "GitInfo - TargetDir: "$TargetDir -ForegroundColor Yellow }
  if (Test-Path $TargetDir) {
    $ShortCommitHash = "git --git-dir ""$($TargetDir)"" log -1 --pretty=format:'%h'"
    $LongCommitHash = "git --git-dir ""$($TargetDir)"" log -1 --pretty=format:'%H'"
    $RemoteOriginURL = "git --git-dir ""$($TargetDir)"" config --get remote.origin.url"
    
    $ShortCommitHash= Invoke-Expression $ShortCommitHash
    $LongCommitHash= Invoke-Expression $LongCommitHash
    $RemoteOriginURL = Invoke-Expression $RemoteOriginURL
    
    if($Verbose) { Write-Host "GitInfo - Found git repo in directory: $path" -ForegroundColor Green }
    
    return @{
      ShortHash = $ShortCommitHash
      LongHash  = $LongCommitHash
      RemoteURL = $RemoteOriginURL
    }
  }
  else {
    if($Verbose) { Write-Host "GitInfo - Not a git repo. Ignoring directory: $path" -ForegroundColor DarkYellow }
    return $null
  }
}
function CreateGitDependencyInfo($projectDir, $dependedProjects) {
  # Check if project files or project directory is null or empty
  if ( -not $projectDir -or -not $dependedProjects) {
    if($Verbose) { Write-Host "CreateGitDependencyInfo - Project files or project directory is null or empty." -ForegroundColor DarkYellow }
      return @()
  }
   # Iterate over each project file
   $projectDependedList = $dependedProjects | ForEach-Object {
    if( $IsWinEnv ){
      $fullPath = Join-Path $projectDir $_.Folder[0]
      $fullPath = Join-Path $fullPath $_.Folder[1]
      $TargetPath = (Resolve-Path (Join-Path $projectDir '..') ).Path
      $TargetPath = (Join-Path $TargetPath $_.ProjectName).ToString()
    } 
    else { 
      $fullPath = Join-Path -Path $projectDir -ChildPath $_.Folder[0] -AdditionalChildPath $_.Folder[1]
      $TargetPath = (Join-Path -Path ((Resolve-Path (Join-Path $projectDir '..')).Path) -ChildPath $_.ProjectName).ToString()
    } 

    # Get Git information for the current and target paths
    $TargetBranch = ""
    if (Test-Path (Join-Path $TargetPath ".git")) {
      if ($Verbose) { Write-Host "CreateGitDependencyInfo - GitInfo for target project: $($_.ProjectName)" -ForegroundColor Yellow }
      $gitInfo = Get-GitInfo $fullPath
      $targetGitInfo = Get-GitInfo $TargetPath
      if ($Verbose) { Write-Host "CreateGitDependencyInfo - getting the current branch of project: $($_.ProjectName)" -ForegroundColor Yellow }
      try {
        $gitDir = Join-Path -Path $TargetPath -ChildPath ".git"
        $TargetBranch = & git --git-dir $gitDir branch --show-current
      } catch {
        Write-Host "Failed to get the current branch of project: $($_.ProjectName)" -ForegroundColor Red
      }
    } else {
      if ($Verbose) { Write-Host "CreateGitDependencyInfo - Target project: $($_.ProjectName) is not a git repo." -ForegroundColor DarkYellow }
    }
    
    # Create a new PSObject with the git information and return it
    New-Object PSObject -Property @{
        BaseName         = $_.ProjectName
        Path             = $fullPath
        Branch           = $_.Branch
        ShortHash        = $_.Hash
        LongHash         = $gitInfo.LongHash
        RemoteURL        = $_.URL
        TargetPath       = $TargetPath
        TargetBranch     = $TargetBranch
        TargetShortHash  = $targetGitInfo.ShortHash
        TargetLongHash   = $targetGitInfo.LongHash
        TargetRemoteURL  = $targetGitInfo.RemoteURL
    }
  }
  return $projectDependedList
}
function CreateSymbolicLink ($projectDir, $projectFiles) {
  
  # Check if projectDir and projectFiles are not empty
  if (-not $projectDir -or -not $projectFiles) {
    if($Verbose) { Write-Host "CreateSymbolicLink - Project directory or project files are empty." -ForegroundColor DarkYellow }
    return
  }
  foreach ($projectFile in $projectFiles) {
    $CreateSymLink = $true 

    # Test if if a symbolic link exists
    if($Verbose) { Write-Host "CreateSymbolicLink - Symbolic link test: $($projectFile.Path)" -ForegroundColor Yellow }
    if ($null -ne $projectFile.Path -and $projectFile.Path -ne '' -and (Test-Path $projectFile.Path)) {
        if ((Get-Item $projectFile.Path).Attributes.ToString().Contains("ReparsePoint")) {
        # Seems that there is a valid link. Now lets get the linked Target of it
        $targetPath = (Get-Item $projectFile.Path)
        if($DebugMsg) { write-output $targetPath }
        $symlink = $targetPath.target[0]
        if($Verbose) { Write-Host "CreateSymbolicLink - Found Symbolic Link: $($symlink)"-ForegroundColor DarkYellow }
      
        # Now, lets create target link path
        if ($IsMacOS -or $IsLinux) { $linkTarget = "../../" + $ProjectFile.BaseName.ToString()  }
        else { 
          #$linkTarget = Get-Item ($projectFile.Path).ToString()
          $TargetLinkDir = Split-Path -Path $projectDir -Parent
          $linkTarget = Get-Item ($TargetLinkDir.ToString() + "/" + $ProjectFile.BaseName.ToString()).ToString()
          if($Verbose) { Write-Host "CreateSymbolicLink - Should be Symbolic Link Target: $($linkTarget.ToString())" -ForegroundColor DarkYellow }
        }
      
        if ($symlink.ToString() -eq $linkTarget.ToString()) { 
           if($true) { Write-Host "- CreateSymbolicLink -"$ProjectFile.BaseName"- A existing and valid symbolic link detected. Skip linking."([Char]0x221A) -ForegroundColor Green }
          $CreateSymLink = $false
        } 
      }
    } else { if (-not ($null -ne $projectFile.Path -and $projectFile.Path -ne '')) { $CreateSymLink = $false } }

    if($CreateSymLink) {
      # Create a symlink
      if($Verbose) { Write-Host "CreateSymbolicLink -"$ProjectFile.BaseName"- No valid symbolic link detected. Creating new symbolic link." -ForegroundColor Yellow }
      # Remove a existing symbolic link first
      if($null -ne $projectFile.Path -and $projectFile.Path -ne '' -and (Test-Path $projectFile.Path)) {
        Remove-Item -Path $projectFile.Path -Force
        if (!$?) { exit 1 }
      }
      if($DebugMsg) { write-output $ProjectFile }
      $linkTarget = $ProjectFile.BaseName
      if ($IsMacOS -or $IsLinux) {
        $linkValue = Join-Path -Path ".." -ChildPath ".." -AdditionalChildPath $linkTarget
        New-Item -ItemType SymbolicLink -Path $projectFile.Path -Value $linkValue | Out-Null
        if($true) { Write-Host "- CreateSymbolicLink - Symbolic link created at $($projectFile.Path) with target $linkValue"([Char]0x221A) -ForegroundColor Green }
      } else { 
        $TargetLinkDir = Split-Path -Path $projectDir -Parent
        $LinkPath = Join-Path $projectDir "lib"
        $LinkName = $ProjectFile.BaseName
        $LinkTarget = Join-Path $TargetLinkDir $ProjectFile.BaseName
        New-Item -ItemType Junction -Path $LinkPath -Name $LinkName -Value $LinkTarget -OutVariable output | Out-Null
        if($true) { Write-Host "- CreateSymbolicLink - Symbolic (Junction) link created at $($LinkPath) with Name $($LinkName) and target $($LinkTarget)"([Char]0x221A)  -ForegroundColor Green }
        
      }
    }
  }
}
Write-Host -ForegroundColor Green "Starting Restore-Dependencies.ps1"
CheckOS # check on which os we are running
Write-Host -ForegroundColor Yellow "- We are on $($PSVersionTable.PSVersion.Major).$($PSVersionTable.PSVersion.Minor) Build Enviroment"
Write-Host -ForegroundColor Yellow "- We assume, we start this script in the project's 'restore' directory."
Set-Location .. # Go one directory back, to get the project dir.
# Now we are in the project directory

# Call the ProcessDependencies function and store the result in the $dependedProjects variable
Write-Host -ForegroundColor Yellow "- Reading the dependencies.txt file and processing each line."
$dependedProjects = ProcessDependencies ("dependencies.txt")
if($DebugMsg) { $dependedProjects | ForEach-Object { Write-Output $_ } } # Output each depended project

# Call the Get-ProjectFiles function with the content of the 'lib' directory and store the result in the $projectFiles variable
# Check if the 'lib' directory exists
Write-Host -ForegroundColor Yellow "- Checking if the 'lib' directory exists."
if (Test-Path 'lib') {
  # Call the Get-ProjectFiles function with the content of the 'lib' directory and store the result in the $projectFiles variable
  $projectFiles = Get-ProjectFiles (Get-ChildItem 'lib')
} else {
  Write-Host -ForegroundColor DarkYellow "- The 'lib' directory was not found and will be created in 3 seconds..."
  Start-Sleep -Seconds 5
  New-Item -ItemType Directory -Path 'lib' | Out-Null
  $projectFiles = Get-ProjectFiles (Get-ChildItem 'lib')
}

#Write-Host "Project files:"
if($DebugMsg) { $projectFiles | ForEach-Object { Write-Output $_ } } # Output each project file
# Get the current location and store it in the $projectDir variable
$projectDir = Get-Location

# Call the CreateGitDependencyInfo function with the current location and the project files and store the result in the $projectFilesGitInfo variable
Write-Host -ForegroundColor Yellow "- Creating git dependency information for each project file."
$projectFilesGitInfo = CreateGitDependencyInfo $projectDir $dependedProjects
if($DebugMsg) { $projectFilesGitInfo | ForEach-Object { Write-Output $_ } }
 # Output each project file's git information

# Call the CloneRepository function with the project files' git information and the depended projects
Write-Host -ForegroundColor Yellow "- Checking, cloning and rebranching the git repositories for each dependency."
$CloneDir = (Resolve-Path (Join-Path $projectDir '..')).Path
if($Verbose) { Write-Host $CloneDir }
CloneRepository $projectFilesGitInfo $dependedProjects $CloneDir

Write-Host -ForegroundColor Yellow "- Checking and creating symbolic links for each project file."
CreateSymbolicLink $projectDir $projectFilesGitInfo
Write-Host -ForegroundColor Green "- Done -"