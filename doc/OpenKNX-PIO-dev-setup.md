# Installation of dev-Environment for LogicModule

Follow all steps described in OpenKNX-Wiki: [Installation of OpenKNX development environment for PlatformIO (PIO)](https://github.com/OpenKNX/OpenKNX/wiki/Installation-of-OpenKNX-development-environment-for-PlatformIO-(PIO))

Do also all verification steps described in our Wiki, these steps download the LogicModule.

## Test if everything works

The next steps test only if the installed environment works correctly. 

Go to the directory OAM-LogicModule. Open File LogicModule.code-workspace with VSCode. 

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
* is the LED powered by an 1 or a 0 signal,
* the PIN for the Prog-Button,
* which are the UART-PINS for KNX communication
* which is the PIN for the Buzzer (optional)
* which is the PIN for power failure (SAVE-PIN, optional)

There might be additional things necessary for firmware, which is using this LogicModule, this should be described in their dev setup document.

Most occasional users will just use Hardware where our firmware already works. There is a File LogikmodulHardware.h where this hardware is listed. 

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
* **-D BOARD_xxxxxxxxx** should be a board from LogikmodulHardware.h, this depends indirectly also on the processor, the example here is BOARD_MASIFI_V2 for a SensorModule-Hardware v2.0 (SAMD-based) or BOARD_MASIFI_HFPM_DEVEL (RP2040-based), both from [https://www.smart-mf.de](https://www.smart-mf.de), or BOARD_SIRSYDOM_PIPICO_BCU_CONNECTOR (RP2040-based) described in more detail here [OpenKNX-Wiki](https://github.com/OpenKNX/OpenKNX/wiki/PiPico-BCU-Connector)

If you have the right combination, which fits to your hardware, see "How to upload the firmware to your hardware".

### Define Board to run on other hardware

In cases you want to run on arbitary hardware, you have to create a BOARD-define in "LogikmodulHardware.h" and use this board in the above mentioned 

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
