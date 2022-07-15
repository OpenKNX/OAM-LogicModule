$pioEnv = $args[0]
$firmwareName = $args[1]
$binaryFormat = $args[2]

~/.platformio/penv/Scripts/pio.exe run -e $pioEnv
if (!$?) {
    Write-Host "$pioEnv build failed, Release was not built!"
    exit 1
}
Copy-Item ".pio/build/$pioEnv/firmware.$binaryFormat" "release/data/$firmwareName.$binaryFormat"
exit 0
