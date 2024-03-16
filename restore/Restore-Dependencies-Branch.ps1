<#
Open ■
┬────┴  Restore-Dependencies
■ KNX   2024 OpenKNX - Erkan Çolak

FILEPATH: restore/Restore-Dependencies-Branch.ps1
#>

param(
  # Set the Git checkout mode
  [ValidateSet("Branch", "Hash")]
  [string]$GitCheckoutMode = "Branch", # Default is Branch

  # Force the script to recreate symbolic links
  [switch]$ForceRecreateSymLinks = $true, # Default is $true
  
  # "dependencies.txt" file
  [string]$DependenciesFile = "dependencies.txt", # Default is "dependencies.txt"

  # Check for privileges (Windows only)
  [switch]$CheckForDeveloperMode = $true, # Default is $true
  [switch]$CheckForSymbolicLinkPermissions = $true, # Default is $true
  [switch]$CheckForAdminOnly = $false, # Default is $false

  # Set the Write-Host message behavior
  [switch]$Verbose = $false, # Default is $false
  [switch]$DebugMsg = $false  # Default is $false
)

# Construct the command to invoke Restore-Dependencies.ps1
$command = ".\Restore-Dependencies.ps1" +
" -GitCheckoutMode $GitCheckoutMode" +
" -ForceRecreateSymLinks $ForceRecreateSymLinks" +
" -DependenciesFile $DependenciesFile" +
" -CheckForDeveloperMode $CheckForDeveloperMode" +
" -CheckForSymbolicLinkPermissions $CheckForSymbolicLinkPermissions" +
" -CheckForAdminOnly $CheckForAdminOnly" +
" -Verbose:$Verbose" +
" -DebugMsg:$DebugMsg"

# Execute the command
Invoke-Expression $command
