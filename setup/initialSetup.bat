REM Under construction, do NOT use

mkdir %1
cd %1
git clone https://github.com/thelsing/knx
git clone https://github.com/OpenKNX/OGM-Common
git clone https://github.com/OpenKNX/OGM-SensorDevices
git clone https://github.com/OpenKNX/OAM-LogicModule
code OAM-LogicModule\LogicModule.code-workspace
