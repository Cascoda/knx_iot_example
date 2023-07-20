# The Applications
This application is a KNX IoT Example application.

IMPORTANT: Please note that by default, every KNX device running this application will have the same KNX serial number.
This makes it impossible to differentiate the devices from each other, and thus impossible to link two (or more) of these togehter, and have a working demo.
It is therefore necessary to assign a different KNX serial number to each device. Please [click here](https://github.com/Cascoda/cascoda-sdk/blob/master/posix/app/chilictl/README.md#flashing-manufacturer-data) for instructions on how to do that.

The application acts as both an LSSB (Light Switching Sensor Basic) and an LSAB (Light Switching Actuator Basic)

Controls:
- Press SW1 for the LSSB side of the application. This will cause the LSAB side on another device to turn its LED ON.
  Note that the LSSB is acting as a toggle.
LED:
- LED 2 is the LSAB that can be controlled by an LSSB.

Programming Mode:
- Press SW4 to put the device into and out of programming mode.
 The LED next to SW3 will keep blinking
while the device is in programming mode.

Reset:
- Hold down SW4 to reset the device.
  There are two levels of reset, depending on how long you hold down the button.
  - Level 1: KNX reset. Press and hold down the button until the LED next to SW3 flickers.
    At this point, you can release the button, unless you want to reach level 2.
  - Level 2: Thread reset. You reach this level by not releasing SW4 after level 1.
    Instead, keep holding down the button until the LED next to SW3 flickers again (at a slower rate this time). You may now let go of the button.


The following applications are in this folder:

CLI:

- knx_iot_example Application (CLI) using the following files:
  - knx_iot_example.c
  - knx_iot_example.h

Windows GUI using WxWidgets:

- knx_iot_example_gui Application (wxWidgets) using the following files:
  - knx_iot_example.c
  - knx_iot_example.h
  - knx_iot_example.cpp

Embedded for Chili:

- knx_iot_example Application (embedded) using the following files:
  - knx_iot_example.c
  - knx_iot_example_dev.c
  - knx_iot_example.h
which needs the Cascoda SDK to build.

The general structure of these programs are:

```
   __________________
  | Application CODE |   <---- WxWidget, Printf, hardware connection
  |__________________|
  |  POINT API CODE  |   <---- Generic code handling all Point API CoAP code
  |__________________|
  |      STACK       |   <---- Generic The KNX IoT Point API Stack (other repo)
  |__________________|

  General structure
```

The Point API Code is shared code between the different applications
it implementes the datapoint resources.

The point code has an API so that one can:

- Set/retrieve data from an URL
- Callback on PUT data changes
- Functions to figure out what type of data the url conveys

These functions then can be used to add the functionallity for
handling the GUI (wxWidgets) and embedded (chili) to connect to the hardware.

## The knx_iot_example Application

### The KNX information

- serial number : 00FA10010710
- password : 4N6AFK6T83YWDUTW23U2
- QR info : KNX:S:00FA10010710;P:4N6AFK6T83YWDUTW23U2

### Data points

| url  | channel/usage       | instance |resource type | interface type | data type |
|------| --------------------| -------- | -------------| ---------------|-----------|
| "/p/o_1_1"  | LED_1 |  1 |urn:knx:dpa.417.52 | if.a |bool |
| "/p/o_2_2"  | PB_1 |  1 |urn:knx:dpa.421.61 | if.s |bool |

### Parameters

| url  | channel/usage   | instance | data type |
|------| ----------------| ---------| --------- |

### MetaData

The mandatory metadata parameters per data points implemented:

- id (9) - the unique url
- rt - the resource type
- if - the interface type
- dpt - the data point type
- ga - the array of group addresses (if initialized)

Next to the mandatory metadata fields the following datapoint specific metadata fields are implemented:

| url  | name   | metadata tag | metadata value |
|------| ----------------| ---------| --------- |
| "/p/o_1_1"  | LED_1 |  desc |On/Off switch 1 |
| "/p/o_2_2"  | PB_1 |  desc |On/Off push button 1 |

For querying the metadata items implemented one can use the following commands:

```
  GET /p/o_1_1?m=*
  GET /p/o_2_2?m=*
```
## Development Board Jumper settings
|Jumper | Setting | Usage |
|-------|---------| ------|
| U19 (SW1)   | 1-2     | ON/OFF button (LSSB) |
| U20 (SW2)   | 1-2     | LED (LSAB)  |
| U25 (SW3)   | 1-2     | Programming mode and reset LED indicator |
| U26 (SW4)   | 1-2     | Programming mode/reset button |

## Building on Windows

From the top level folder of the project execute:

- mkdir build
- cd build
- cmake ..
  - this command retrieves the dependencies from github
- open solution (sln) created in the build folder with visual studio
- build the applications in visual studio

To use knx gitlab as source of the KNX-IOT-STACK use the following command:

- cmake .. -DUSE_GITLAB=true

scripts available:

- build.sh , building unsecured version in folder build
- build_secured.sh , building secured version in folder build_secured

Note that one has to have access to the knx gitlab repo.

## Building on Linux

Note: the GUI variant is not available on Linux, only CLI will be build

From the top level folder of the project execute:

- mkdir build_Linux
- cd build_Linux
- cmake ..
  - this command retrieves the dependencies from github
- make

To use knx gitlab as source of the KNX-IOT-STACK use the following command:

- cmake .. -DUSE_GITLAB=true

Note that one has to have access to the knx gitlab repo.

## Building Embedded for the Chili

### For Linux

```bash
mkdir build
cd build/
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm_gcc_m2351.cmake || make
```

To save time on subsequent incremental builds, you may simply run `make` inside your build directory.

### On Windows

#### Installation of Tool chain

Run the following within an ADMIN Powershell window in order to set up the Chocolatey package manager:

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
```

Then, also in an ADMIN Powershell window, allow the execution of local scripts & install the toolchain using Choco.
```powershell
Set-ExecutionPolicy RemoteSigned
.\install-toolchain.ps1
```

#### Configure and build

Finally, set up the build directory & compile your binaries.

```powershell
# using ninja on windows (secured)
.\build_win_bin_ninja
# using ninja on windows (unsecured)
.\build_win_bin_ninja_unsecured
# using mingw on windows (secured)
.\build_win_bin_mingw
```

On subsequent builds, you only need to run `Ninja` (or make) inside your build directory.

## Example Binaries

Once the build completes, you will find `knx_iot_example.bin` inside your `build_win_bin/` directory. 
You must then flash the binary. 
This can be done via USB, with a programmer (J-Link / NuLink) or via Over-the-Air Software Update. 
See the [Flashing Guide](https://github.com/Cascoda/cascoda-sdk/blob/master/docs/guides/flashing.md) for a detailed how-to.


***


