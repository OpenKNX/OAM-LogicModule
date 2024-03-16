<#
Open ■
┬────┴  Restore-Dependencies
■ KNX   2024 OpenKNX - Erkan Çolak

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

# Optional Input Parameters
param(
  # Set the Git checkout mode
  [ValidateSet("Branch", "Hash")]
  [string]$GitCheckoutMode= "Hash", # Branch or Hash. Default is Hash

  # Force the script to recreate symbolic links
  [switch]$ForceRecreateSymLinks= $true, # Default is $true
  
  # "dependencies.txt" file
  [string]$DependenciesFile= "dependencies.txt", # Default is "dependencies.txt"

  # Check for privileges (Windows only)
  [switch]$CheckForDeveloperMode= $true,  # Default is $true
  [switch]$CheckForSymbolicLinkPermissions= $true, # Default is $true
  [switch]$CheckForAdminOnly= $false, # Default is $false

  # Set the Write-Host message behavior
  [switch]$Verbose= $false, # Default is $false
  [switch]$DebugMsg= $false  # Default is $false
)

# Global Variables
# If the user has no permissions to create symbolic links with 'New-Item', the script will try to use mklink to create symbolic links.

#Those variables are used to check if the script is running on Windows. Only on Windows we can use mklink to create symbolic links.
# If $Auto_Use_mklink_To_Create_SymLinks is $true, the script will automatically use mklink to create symbolic links.
$Auto_Use_mklink_To_Create_SymLinks = $false # Default is $false 
# Ignore the permissions to create symbolic links with 'New-Item' and use mklink to create symbolic links.
$Force_Use_mklink_To_Create_SymLinks = $true # Default is $true. If $Auto_Use_mklink_To_Create_SymLinks is $true, this variable is ignored.



function Test-Administrator {
  return (([Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole('Administrators')).If($true, $false)
}

function Test-DeveloperMode {
  try {
    # Check if the registry key exists
    $isDeveloperMode = (Get-ItemPropertyValue -Path "HKLM:\SOFTWARE\Microsoft\Windows\CurrentVersion\AppModelUnlock" -Name "AllowDevelopmentWithoutDevLicense" -ErrorAction Stop) -eq 1
    return $isDeveloperMode
  } catch {
    Write-Host -ForegroundColor Red "Error: $_"
    return $false
  }
}

function Test-SymbolicLinkPermission {
  # Clear the error variable
  $Error.Clear()
  $bRet = $true
  # Create a test symbolic link and target
  $testLinkPath = Join-Path ([System.IO.Path]::GetTempPath()) "test_symlink"
  $testTargetPath = Join-Path ([System.IO.Path]::GetTempPath()) "test_target"
  # Create the test target file
  $null = New-Item -ItemType File -Path $testTargetPath -Force
  $null = New-Item -ItemType SymbolicLink -Path $testLinkPath -Target $testTargetPath -Force -ErrorAction SilentlyContinue
  # Check last error message to see if the symbolic link was created successfully
  if ($Error) {
    # There was an error, so check the error message. If the error message is "UnauthorizedAccess", the user does not have permissions to create symbolic links
    $bRet = $false
    # Any other error message is unexpected and should be investigated. The script will return $false also in this case.
    # If the symbolic link was not created successfully, check the error message
    foreach ($err in $Error) {
      # Check the FullyQualifiedErrorId property if it contains the "UnauthorizedAccess" error
      if ($DebugMsg) {
        Write-Host -ForegroundColor DarkRed "- Error Message: $($err.Exception.Message)"
        Write-Host -ForegroundColor DarkRed "- Category : $($err.CategoryInfo.Category)"
        Write-Host -ForegroundColor DarkRed "- TargetName: $($err.CategoryInfo.TargetName)"
        Write-Host -ForegroundColor DarkRed "- ScriptName: $($err.InvocationInfo.ScriptName)"
        Write-Host -ForegroundColor DarkRed "- ErrorId: $($err.FullyQualifiedErrorId)"
      } else {
        if ($Verbose) { 
          Write-Host -ForegroundColor DarkRed "- Error Message: $($err.Exception.Message)"
          Write-Host -ForegroundColor DarkRed "- ErrorId: $($err.FullyQualifiedErrorId)" 
        }
      }
      if ($err.FullyQualifiedErrorId -eq "UnauthorizedAccess" -or 
        $err.FullyQualifiedErrorId -eq "NewItemSymbolicLinkElevationRequired,Microsoft.PowerShell.Commands.NewItemCommand") {
          if ($Verbose) { Write-Host -ForegroundColor Red "- Symbolic link creation requires elevated privileges." ([Char]0x2717) }
          Write-Host -ForegroundColor Red "- User $($env:USERNAME) has not permissions to create symbolic links with 'New-Item'." ([Char]0x2717) 
        break
      }
    }
  } else {
    # There was no error, so the symbolic link was created successfully
    if ($Verbose) { Write-Host -ForegroundColor Green "- TestSymbolic link created successfully." }
    Write-Host -ForegroundColor Green "- User $($env:USERNAME) has permissions to create symbolic links with New-Item." ([Char]0x221A)
  }
  # Clean up the test symbolic link and target
  Remove-Item -Path $testLinkPath -ErrorAction SilentlyContinue
  Remove-Item -Path $testTargetPath -ErrorAction SilentlyContinue
  return $bRet
}

function OpenKNX_ShowLogo($AddCustomText = $null) {
  Write-Host ""
  Write-Host "Open " -NoNewline
  #Write-Host "■" -ForegroundColor Green
  Write-Host "$( [char]::ConvertFromUtf32(0x25A0) )" -ForegroundColor Green
  $unicodeString = "$( [char]::ConvertFromUtf32(0x252C) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2500) )$( [char]::ConvertFromUtf32(0x2534) ) "

  if ($AddCustomText) { 
    #Write-Host "┬────┴  $AddCustomText" -ForegroundColor Green
    Write-Host "$($unicodeString) $($AddCustomText)"  -ForegroundColor Green
  }
  else {
    #Write-Host "┬────┴" -ForegroundColor Green
    Write-Host "$($unicodeString)"  -ForegroundColor Green
  }

  #Write-Host "■" -NoNewline -ForegroundColor Green
  Write-Host "$( [char]::ConvertFromUtf32(0x25A0) )" -NoNewline -ForegroundColor Green
  Write-Host " KNX"
  Write-Host ""
}

function CheckForPrivileges {
  if ($IsWinEnv ) {
    if($CheckForAdminOnly) {
      if($Verbose) { Write-Host -ForegroundColor Yellow "- Checking if we are in Administrator privileges" }
      if( -not (Test-Administrator) ) {
        Write-Host -ForegroundColor Red "ERROR: Restore-Dependencies requires Administrator privileges to run!"
        Write-Host -ForegroundColor Red "- Please run the script again with Administrator privileges."
        exit 1
      } else { Write-Host -ForegroundColor Green "- The script is running with Administrator privileges." ([Char]0x221A) }
    }
    if($CheckForDeveloperMode) {
      if($Verbose) { Write-Host -ForegroundColor Yellow "- Checking if we are in Developer Mode" }
      if( -not (Test-DeveloperMode) ) {
        Write-Host -ForegroundColor Red "ERROR: Restore-Dependencies requires Developer Mode to run!"
        Write-Host -ForegroundColor Red "- Please run the script again with Developer Mode."
        Write-Host -ForegroundColor Red "- If you are using Windows `>10, you can enable Developer Mode by going to Settings `> Update `& Security `> For developers and selecting Developer mode."
        exit 1
      } else { Write-Host -ForegroundColor Green "- The script is running with Developer Mode." ([Char]0x221A) }
    }
    if(-not $Force_Use_mklink_To_Create_SymLinks -and $CheckForSymbolicLinkPermissions) {
      if($Verbose) { Write-Host -ForegroundColor Yellow "- Checking if we have permissions to create symbolic links" }
      if( -not (Test-SymbolicLinkPermission) ) {
        if($Auto_Use_mklink_To_Create_SymLinks) {
          $script:Force_Use_mklink_To_Create_SymLinks = $true
          Write-Host -ForegroundColor Yellow "- We have no permissions to create symbolic links with 'New-Item'. We will try to use mklink to create symbolic links."
        } else {
          Write-Host -ForegroundColor Red "ERROR: Restore-Dependencies requires permissions to create symbolic links to run!"
          Write-Host -ForegroundColor Red "- Please run the script again with Administrator privileges."
          exit 1
        }
      } else { Write-Host -ForegroundColor Green "- The script has permissions to create symbolic links." ([Char]0x221A) }
    } else { Write-Host -ForegroundColor Green "- We will use mklink to create symbolic links." ([Char]0x221A) }
  }
}

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

  $CurrentOS = switch($true) {
    $IsLinuxEnv { "Linux" }
    $IsMacOSEnv { "MacOS" }
    $IsWinEnv { "Windows" }
    default { "Unknown" }
  }
  $PSVersion = "$($PSVersionTable.PSVersion.Major).$($PSVersionTable.PSVersion.Minor).$($PSVersionTable.PSVersion.Patch)"
  if($true) { Write-Host -ForegroundColor Green "- We are on $CurrentOS Build Environment with PowerShell $PSVersion"  ([Char]0x221A) }
}
function ProcessDependencies($DependenciesFile) {
  # Check if the file exists
  if (-not (Test-Path $DependenciesFile)) {
    Write-Host -ForegroundColor Red "The file $DependenciesFile does not exist."
    return $null
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
        # Assuming the project name is the same as the repository name, and considering the possibility of a .git extension to fix a 'dot' in the project name.
        # Get the index of the last dot in the string (.git)
        $lastDotPosition = $projectNameWithExtension.LastIndexOf('.')
        # Check if a dot was found
        if ($lastDotPosition -ge 0) {
            # Extract the substring without the last dot
            $projectName = $projectNameWithExtension.Substring(0, $lastDotPosition)
        } else {
            # No dot found, use the entire string as the project name!
            $projectName = $projectNameWithExtension
        }
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
      if($Verbose) { Write-Host "- Get-ProjectFiles - Found existing project file: $($projectFile.BaseName)" -ForegroundColor Yellow }

    # Check if the file name is not "README" and is not empty
    if ($projectFile.BaseName -ne "README" -and ![string]::IsNullOrWhiteSpace($projectFile.BaseName)) {
      # Add the project file to the list
      $projectFiles += $projectFile
    }
  }

  # Return the list of project files
  return $projectFiles
}
function CloneRepository($projectFilesGitInfo, $dependedProjects, $CloneDir, $CloneModeHash= $false) {
  # Loop through each depended project
  if($CloneModeHash -and $Verbose) { Write-Host "- CloneRepository - Using hash" -ForegroundColor Green }
  foreach ($dependedProject in $dependedProjects) {
    # Initialize a flag to track if the project is found
    $hashMatch = $false
    $branchMatch = $false
    
    if($Verbose) { Write-Host "- CloneRepository - Check: '$($dependedProject.ProjectName)'" -ForegroundColor Green }
    # Loop through each project file's git information
    foreach ($projectFile in $projectFilesGitInfo) {
      # Check if projectFile contains valid data
      if (($null -ne $projectFile.BaseName        -and $projectFile.BaseName        -ne "") -and 
          ($null -ne $dependedProject.ProjectName -and $dependedProject.ProjectName -ne "") -and
          (          $projectFile.BaseName      -match $dependedProject.ProjectName) )
      {
        if($Verbose) { Write-Host "- CloneRepository - Found: '$($dependedProject.ProjectName)' - '$($projectFile.Path)'" -ForegroundColor Green }
        #Compare the branches and hashes
        # Check if the branches match
        if (($null -ne $projectFile.TargetBranch -and $projectFile.TargetBranch -ne "") -and 
                       $projectFile.TargetBranch -eq $dependedProject.Branch) 
        {
          # If the branches match, set the flag to true
          $branchMatch = $true
          if($Verbose) { Write-Host "- CloneRepository - Branch: '$($dependedProject.ProjectName)' - Both matches to ($($projectFile.TargetBranch))" -ForegroundColor Green }
        }
        # Check if the hashes match
        if (($null -ne $projectFile.TargetShortHash -and $null -ne $dependedProject.Hash) -and 
                       $projectFile.TargetShortHash -match $dependedProject.Hash) 
        {
          # If the project is found, set the flag to true and break the loop
          $hashMatch = $true
          if($Verbose) { Write-Host "- CloneRepository - Hash: '$($dependedProject.ProjectName)' - Both matches to ($($projectFile.TargetShortHash))" -ForegroundColor Green }
        }
        if($branchMatch -and $hashMatch) {
          if($Verbose) { Write-Host "- CloneRepository - Found: "$dependedProject.ProjectName" - "$projectFile.Path -ForegroundColor Green }
          if($Verbose) { Write-Host "- CloneRepository - Branches and Hashes matching: '$($dependedProject.ProjectName)'" -ForegroundColor Green }
          break
        }
        break
      }
    }

    # If the project is not found, clone the repository
    if (-not $branchMatch -and -not $hashMatch) {
      # Create the Git clone URL with the URL and the Hash from the dependency
      $GitClone = ($dependedProject.URL).ToString()
      if($Verbose) { Write-Host "- CloneRepository - '$($dependedProject.ProjectName)' not Found. Target does not exist." -ForegroundColor DarkYellow }
      # Try to clone the repository
      try {
        if($IsWinEnv){
          $CloneTarget = Join-Path $CloneDir $dependedProject.ProjectName
        } else {
          $CloneTarget = Join-Path -Path $CloneDir -ChildPath $dependedProject.ProjectName
        }
        if($Verbose) { Write-Host "- CloneRepository - CloneTarget: " $CloneTarget -ForegroundColor Green }
        if($Verbose) { Write-Host "- CloneRepository - Cloning '$($dependedProject.ProjectName)': '$GitClone' to '$CloneTarget'" -ForegroundColor Yellow }
        
        $DoClone= $true
        # Check if the folder already exists
        if (Test-Path $CloneTarget -PathType Container) {
          # Check if it's a Git repository (contains .git directory)
          if (Test-Path (Join-Path $CloneTarget ".git") -PathType Container) {
            if($Verbose) { Write-Host "- CloneRepository - Cloning: The target directory is already a Git repository." -ForegroundColor Yellow }
            $DoClone= $false
          } else {
            # The folder exists, but it's not a Git repository, so rename it
            if($true) { Write-Host "- CloneRepository - The directory already exists, but it's not a Git repository. Renaming..." -ForegroundColor Yellow }
            Rename-Item -Path $CloneTarget -NewName "${CloneTarget}_backup" -Force
          }
        }
        if($DoClone) {
          Invoke-RestMethod -Uri $GitClone -Method Head -ErrorAction Stop;
          if($Verbose) { $GitCmd= "git clone '$($GitClone)' '$($CloneTarget.ToString())'" 
          } else { $GitCmd= "git clone -q '$($GitClone)' '$($CloneTarget.ToString())'" }
          Invoke-Expression $($GitCmd)
          #git clone -q '$GitClone' '$CloneTarget.ToString()'
        }
        
        if($true) { Write-Host "- CloneRepository - Cloning "$dependedProject.ProjectName": '$GitClone' to '$CloneTarget' Done"([Char]0x221A) -ForegroundColor Green }
      }
      # If the repository does not exist, catch the error
      catch {
        if($Verbose) { Write-Host "CloneRepository - Cloning Error: "$dependedProject.ProjectName"- Repository does not exist: "$GitClone -ForegroundColor DarkYellow }
      }
    } 
    # If the project is found, check out to the specific branch
    if(-not $hashMatch -or -not $branchMatch) 
    {
      if($Verbose) { Write-Host "- CloneRepository - Found: "$dependedProject.ProjectName" - "$dependedProject.URL -ForegroundColor Green }
      
      # If the repository already exists, switch to the corresponding branch
      $checkoutTarget = if ($CloneModeHash) { "Hash" } else { "Branch" }
      if($Verbose) { Write-Host "- CloneRepository -"$dependedProject.ProjectName"- Repository already exists. Checking out to the $($checkoutTarget)." -ForegroundColor Yellow }
      
      try {
        # Check the installed Git version
        if($IsWinEnv){
          $CloneTarget = Join-Path $CloneDir $dependedProject.ProjectName
          $GitDir = Join-Path $CloneTarget ".git"
        } else {
          $CloneTarget = Join-Path -Path $CloneDir -ChildPath $dependedProject.ProjectName
          $GitDir = Join-Path -Path $CloneTarget -ChildPath ".git"
        }
        
        if($Verbose) { Write-Host "- CloneRepository - $($checkoutTarget) - GitDir: "$GitDir -ForegroundColor Yellow }
        $GitCmd = "git --git-dir=""$($GitDir)"" --work-tree=""$($CloneTarget.ToString())"""
        if($Verbose) { Write-Host "- CloneRepository - $($checkoutTarget) - GitCmd: "$GitCmd -ForegroundColor Yellow }

        if($CloneModeHash) {
          $CheckOutTarget = $($dependedProject.Hash)  # Optional: If the CloneModeHash is true, use the Hash
        } else {
          $CheckOutTarget = $($dependedProject.Branch) # If the CloneModeHash is false (default), use the Branch
        }

        if ((& git --version) -ge 'git version 2.23' -and $CloneModeHash -eq $false ) {
          $CheckOutMethod = "switch"    # If the Git version is 2.23 or higher, use the 'switch' command
        } else {
          $CheckOutMethod = "checkout"  # If the Git version is 2.23 or higher, use the 'switch' command
        }

        # Let's do the git checkout
        if($Verbose) { 
          Invoke-Expression "$GitCmd $CheckOutMethod $($CheckOutTarget)"
        } else { 
          Invoke-Expression "$GitCmd $CheckOutMethod $($CheckOutTarget) -q" | Out-Null
        }

        if($true) { 
          $checkoutTarget = if ($CloneModeHash) {  "Hash '$($dependedProject.Hash)'" } else { "Branch '$($dependedProject.Branch)'" }
          Write-Host "- CloneRepository - '$($dependedProject.ProjectName)' $($checkoutTarget) Checked out."([Char]0x221A) -ForegroundColor Green 
        }
      }
      # If cannot check out to the branch, catch the error
      catch {
        if($Verbose) {
          $checkoutTarget = if ($CloneModeHash) {  "Hash '$($dependedProject.Hash)'" } else { "Branch '$($dependedProject.Branch)'" }
          Write-Host "- CloneRepository - $($dependedProject.ProjectName) - Checkout Error! Cannot checkout $($checkoutTarget) Checked out."([Char]0x2717) -ForegroundColor Red }
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
    
    if($Verbose) { $ShortCommitHash= Invoke-Expression $ShortCommitHash } else { $ShortCommitHash= Invoke-Expression $ShortCommitHash | Out-Null }
    if($Verbose) { $LongCommitHash= Invoke-Expression $LongCommitHash } else { $LongCommitHash= Invoke-Expression $LongCommitHash | Out-Null }
    if($Verbose) { $RemoteOriginURL = Invoke-Expression $RemoteOriginURL } else { $RemoteOriginURL = Invoke-Expression $RemoteOriginURL  | Out-Null }
    
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
    if($Verbose) { Write-Host "- CreateSymbolicLink - Project directory or project files are empty." -ForegroundColor DarkYellow }
    return
  }
  foreach ($projectFile in $projectFiles) {
    $CreateSymLink = $true 

    # Test if if a symbolic link exists
    if($Verbose) { Write-Host "- CreateSymbolicLink - Symbolic link test: $($projectFile.Path)" -ForegroundColor Yellow }
    if ($null -ne $projectFile.Path -and $projectFile.Path -ne '' -and (Test-Path $projectFile.Path)) {
        if ((Get-Item $projectFile.Path).Attributes.ToString().Contains("ReparsePoint")) {
        # Seems that there is a valid link. Now lets get the linked Target of it
        $targetPath = (Get-Item $projectFile.Path)
        if($DebugMsg) { write-output $targetPath }
        
        # Get the target of the symbolic link
        if($IsMacOS -or $IsLinux) { $symlink = $targetPath.target
        } else { $symlink = $targetPath.target[0] }

        if($Verbose) { Write-Host "- CreateSymbolicLink - Found Symbolic Link: $($symlink)"-ForegroundColor DarkYellow }
        
        # Now, lets create target link path
        $linkTarget = Join-Path $(Join-Path ".." "..") $projectFile.BaseName.ToString()

        # Check if we are on Windows and if we should use mklink to create the symbolic link
        if($IsWinEnv -and -not $Force_Use_mklink_To_Create_SymLinks) {
          $LinkTarget = Join-Path $(Split-Path -Path $projectDir -Parent) $ProjectFile.BaseName
        }
        
        # If the link target is the same as the project file name, set the CreateSymLink flag to false
        if ($symlink.ToString() -eq $linkTarget.ToString()) { 
          if(!$ForceRecreateSymLinks) { Write-Host "- CreateSymbolicLink - '$($ProjectFile.BaseName)'- A existing and valid symbolic link detected. Skip linking."([Char]0x221A) -ForegroundColor Green }
         $CreateSymLink = $false
        }
      }
    } else { if (-not ($null -ne $projectFile.Path -and $projectFile.Path -ne '')) { $CreateSymLink = $false } }

    if($CreateSymLink -or $ForceRecreateSymLinks ) {
      # Create a symlink
      if($Verbose) { 
        $Message= "- CreateSymbolicLink - '$($ProjectFile.BaseName)' - No valid symbolic link detected. Creating new symbolic link."
        if( $ForceRecreateSymLinks ) {
          $Message= "- CreateSymbolicLink - '$($ProjectFile.BaseName)' - Forcing to create new symbolic links."
        }
        Write-Host $Message -ForegroundColor Yellow
      }
      
      # Remove a existing symbolic link first
      if(  ($ForceRecreateSymLinks -or ($null -ne $projectFile.Path -and $projectFile.Path -ne '')) -and 
           (Test-Path $projectFile.Path) )  {
        
        if($Verbose -and $ForceRecreateSymLinks ) { 
          $Message= "- CreateSymbolicLink - '$($ProjectFile.BaseName)' - Forcing to remove existing symbolic link: '$($projectFile.Path)'"
          Write-Host $Message -ForegroundColor Yellow
        }
        
        Remove-Item -Path $projectFile.Path -Force -Recurse
        if (!$?) { exit 1 }
      }
      if($DebugMsg) { write-output $ProjectFile }
      
      # Create the symbolic link to link the project file to the target
      $linkTarget = $ProjectFile.BaseName
      $linkValue = Join-Path $(Join-Path ".." "..") $linkTarget
      
      #The create smbolic link command
      $CreateSymLinkCommand = "New-Item -ItemType SymbolicLink -Path '$($projectFile.Path)' -Target '$($linkValue)'"
      
      # Check if we are on Windows and if we should use mklink command to create the symbolic link
      if($IsWinEnv) { 
        if ( $Force_Use_mklink_To_Create_SymLinks) {
         $CreateSymLinkCommand = "cmd /C mklink /D ""$($projectFile.Path)"" ""$($linkValue)"""
        } else {
          $TargetLinkDir = Split-Path -Path $projectDir -Parent
          $linkValue = Join-Path $TargetLinkDir $ProjectFile.BaseName
          $CreateSymLinkCommand = "New-Item -ItemType SymbolicLink -Path '$($projectFile.Path)' -Target '$($linkValue)'" 
        }
      }
      if($Verbose) { Write-Host "- CreateSymbolicLink - '$($ProjectFile.BaseName)' - CreateSymLinkCommand: $($CreateSymLinkCommand)" -ForegroundColor Yellow }
      
      # Try to create the symbolic link
      try {
        if($Verbose) { 
            Invoke-Expression $CreateSymLinkCommand 
        } else { 
            Invoke-Expression $CreateSymLinkCommand | Out-Null
        }
        Write-Host "- CreateSymbolicLink - Symbolic link created at $($projectFile.Path) with target $linkValue"([Char]0x221A) -ForegroundColor Green
      } catch {
        Write-Host "Error creating symbolic link: $_" -ForegroundColor Red
        exit 1
      }
    }
  }
}

# Beispielaufruf der Funktion
Clear-Host
OpenKNX_ShowLogo -AddCustomText "Restore Dependencies"
Write-Host "Starting to Restore depended projects..." -ForegroundColor Green

CheckOS # check on which os we are running
CheckForPrivileges # check for privileges, which are needed to run the script
if($Verbose) { Write-Host -ForegroundColor Yellow "- We assume, we start this script in the project's 'restore' directory." }
#Start-Sleep 30
Set-Location .. # Go one directory back, to get the project dir.
# Now we are in the project directory

# Call the ProcessDependencies function and store the result in the $dependedProjects variable
Write-Host -ForegroundColor Yellow "- Reading the dependencies.txt file and processing each line."
$dependedProjects = ProcessDependencies $DependenciesFile
if( $dependedProjects.Count -eq 0) { 
  Write-Host -ForegroundColor Red "- No dependencies found in dependencies.txt file. Please check the file." 
  exit 1  # exit with error
}
if($DebugMsg) { $dependedProjects | ForEach-Object { Write-Output $_ } } # Output each depended project

# Call the Get-ProjectFiles function with the content of the 'lib' directory and store the result in the $projectFiles variable
# Check if the 'lib' directory exists
Write-Host -ForegroundColor Yellow "- Checking if the 'lib' directory exists."
if (Test-Path 'lib') {
  # Call the Get-ProjectFiles function with the content of the 'lib' directory and store the result in the $projectFiles variable
  $projectFiles = Get-ProjectFiles (Get-ChildItem 'lib')
} else {
  Write-Host -ForegroundColor DarkYellow "- The 'lib' directory was not found and will be created in 3 seconds..."
  Start-Sleep -Seconds 3
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
CloneRepository $projectFilesGitInfo $dependedProjects $CloneDir ($GitCheckoutMode -eq "Hash")


if($Verbose) { Write-Host -ForegroundColor Yellow "- Checking and creating symbolic links for each project file." }
CreateSymbolicLink $projectDir $projectFilesGitInfo
OpenKNX_ShowLogo -AddCustomText "Restore Dependencies: Done $([Char]0x221A)"