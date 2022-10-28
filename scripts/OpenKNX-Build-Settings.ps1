
# set product names, allows mapping of (devel) name in Project to a more consistent name in release
$settings = @{}

$settings.sourceName="Logikmodul"  
$settings.targetName="LogicModule" 
$settings.knxprod="src/{0}.h" -f $settings.sourceName
$settings.hardware="src/{0}Hardware.h" -f $settings.sourceName

Return $settings