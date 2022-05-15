# Installation of dev-Environment for LogicModule

Tested on Windows 10 and Windows 11!

Download and install git from [https://git-scm.com/downloads](https://git-scm.com/downloads) with default options

Download and install the newest release of OpenKNX-Tools from [https://github.com/OpenKNX/OpenKNXproducer/releases](https://github.com/OpenKNX/OpenKNXproducer/releases) according to installation instructions described in OpenKNX-Wiki [https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-tools](https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-tools)

Download and install visual studio code from [https://code.visualstudio.com/download](https://code.visualstudio.com/download) (User installer, 64 bit)

Start visual studio code

Go to extensions (Ctrl-Shift-X)

Enter "platformio" in search field

Install "PlatformIO IDE" extension

Wait until installation is finished, do the necessary reload window afterwards (may take some time)

Click on the new PlatformIO-Icon on the left ![PIO-Icon](pics/PIO2.png)

In "Quick Access", choose open

In the new "PIO Home" tab, click on "New Project..."

In the upcoming dialog, provide the name "Test", Board "Sparkfun SAMD21 Dev Breakout", Framework "Arduino" and Location "Use default location"

Click "Finish" and wait until finished. Visuals Studio Code will open the newly created project afterwards. The new project is just used to create default envoronment and can be deleted afterwards.

Click again the PlatformIO Icon ![PIO-Icon](pics/PIO2.png)

Again "Quick Access" appears, click "Miscellaneous->PlatformIO Core CLI"

A new terminal (within Visual Studio Code) appears, the path is home of the new test project. We don't need the test project, it was just used to create all necessary path for development.
From now on we work in this terminal window:

    cd .. 

You should be now in a directory ending with ...\Documents\PlatformIO\Projects

    mkdir OpenKNX
    cd OpenKNX
    git clone https://github.com/thelsing/knx.git
    git clone https://github.com/OpenKNX/OGM-Common.git
    git clone https://github.com/OpenKNX/OGM-SensorDevices.git
    git clone https://github.com/OpenKNX/OAM-LogicModule.git
    cd OAM-LogicModule
    code LogicModule.code-workspace

Now a new instance of Visual Studio Code is started. You can close the other (previous) instance.

## Test if everything works

The next steps test only if the installed environment works correctly.

### Building of a knxprod

Press Ctrl-Shift-P, enter "run test task" and click the appearing "Tasks: Run Test Task"

In the following dropdown select "**OpenKNXproducer** OAM-LogicModule"

Wait for the success message in the terminal window

For a knxprod creation a valid ETS installation is necessary, see Remarks our OpenKNX-Wiki [https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-tools](https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-tools).

### Building a firmware

Press Ctrl-Shift-B, select the "**Build SAMD** OAM-LogicModule" or the "**Build RP2040** OAM-LogicModule" build task and press enter.

Now the compiler starts, this may take a while, there will be many yellow warnings, they can be ignored.

First time build of RP2040-Firmware might take a very long time (up to 1 hour). The whole compile toolchain and RP2040 SDK has to be downloaded and compiled first. But this happens just once.

At the end, there should be a message like

    Linking .pio\build\build\firmware.elf
    Building .pio\build\build\firmware.bin
    Checking size .pio\build\build\firmware.elf
    Advanced Memory Usage is available via "PlatformIO Home > Project Inspect"
    RAM:   [==        ]  22.0% (used 7216 bytes from 32768 bytes)
    Flash: [======    ]  55.7% (used 145892 bytes from 262144 bytes)
    ============================ [SUCCESS] Took 34.60 seconds ======

Now you successfully build the Firmware for the LogicModule, which proves, that everything works fine.

## Development tasks

You are now free to modify Firmware or ETS-Application to your needs. For an experienced developer this might be enough documentation. The following chapters just sketch some tasks for occasional users. There is additional documentation for some tasks in [https://github.com/OpenKNX/OAM-LogicModule/tree/main/doc](https://github.com/OpenKNX/OAM-LogicModule/tree/main/doc). If not, you can request support in KNXUF, the "price" is a good documentation for the task, which will the be integrated here and helps other users.

### Hardware information

For all tasks you have to prepare PIO to built for your hardware. For our LogicModule, the necessary information is

* the PIN for Prog-LED,
* is the LED on with a 1 or a 0 signal,
* the PIN for the Prog-Button,
* does the Prog-Button send a 0 or a 1 signal on press,
* which are the UART-Pins for KNX communication
* which is the Pin for the Buzzer (optional)

There might be additional things necessary for firmware, which is using this LogicModule, this should be described in their dev setup document.

Most occasional users will just use Hardware where our firmware already works. For this hardware there are existing board definitions in a file "HardwareDevices.h" contained in our OGM-SensorDevices project.

Open platformio.ini-file located in OAM-LogicModule project. At the end of this file you will find a section

    ### This is an example for enduser compiled environment
    [env:enduser]
    extends = SAMD
    ;extends = RP2040
    upload_protocol = sam-ba
    ;upload_protocol = picotool
    build_flags =
    ${SAMD.build_flags}
    -D DEBUG_DELAY=100
    -D BOARD_MASIFI_V2
    ;  -D BOARD_MASIFI_HFPM_DEVEL

You have to pay attention on the following parts:

* **extends** should be SAMD or RP2040, depending on the processor you use
* **upload_protocol** should be sam-ba for SAMD and picotool for RP2040
* **-D BOARD_xxxxxxxxx** should be a board from HardwareDevices.h, this depends indirectly also on the processor, the example here is BOARD_MASIFI_V2 for a SensorModule-Hardware v2.0 (SAMD-based) or BOARD_MASIFI_HFPM_DEVEL (RP2040-based), both from MASIFI.

If you have the right combination, which fits to your hardware, see "How to upload the firmware to your hardware".

### Define Board to run on other hardware

In cases you want to run on arbitary hardware, you have to create a BOARD-define in "HardwareDevices.h" (in Project OGM-SensorDevices) and use this board in the above mentioned 

    [env:enduser]
    ...

section. Doing so you should be an experienced user, because we have limited capacity to provide support here.

### Implement an own user function

In most cases (hopefully) you are going to implement a specific user function for your logic. Your original setup was done by a release package delivered through github. In such cases, in platformio.ini file there is a release-section for your existing hardware, i.e.:

    [env:release_SAMD_v30]
    extends = SAMD
    build_flags =
    ${SAMD.build_flags}
    -D DEBUG_DELAY=100
    -D BOARD_MASIFI_V3

Just copy all lines below [env:release_xxxx] to the last section [env:enduser]. Now you can do your user formula changes and finally build and upload you firmware as described in "How to upload the firmware to your hardware"

## How to upload the firmware to your hardware

Connect your device via USB to your PC. If the device is powered by the KNX-Bus, connect it also to KNX.

Press Ctrl-Shift-B, select "**Upload ENDUSER** OAM-LogicModule" build task and press enter.

Wait until file is built and uploaded.
