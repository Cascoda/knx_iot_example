# The Applications
This application is a Battleships game.

The game supports two players playing against each other.

The game follows the standard rules - 10x10 grid, 5 ships, single shot per turn.


Controls:
- Buttons 2 through 4 are used to interact with the UI displayed on the e-paper display.
- Button 2 is generally move left/right
- Button 3 is generally move up/down
- Button 4 is generally select/place/rotate

Programming Mode:
- Programming mode isn't easily accessible in the application's current state.

Reset:
- Reset is also not easily accessible.


The following applications are in this folder:

CLI:

- knx_eink_battleships Application (CLI) using the following files:
  - knx_eink_battleships.c
  - knx_eink_battleships_virtual.c
  - knx_eink_battleships.h

Windows GUI using WxWidgets:

- knx_eink_battleships_gui Application (wxWidgets) using the following files:
  - knx_eink_battleships.c
  - knx_eink_battleships_virtual.c
  - knx_eink_battleships.h
  - knx_eink_battleships.cpp

Embedded for Chili:

- knx_eink_battleships Application (embedded) using the following files:
  - knx_eink_battleships.c
  - knx_eink_battleships_dev.c
  - knx_eink_battleships.h
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

## The knx_eink_battleships Application

### The KNX information

- serial number : 00fa10010713
- password : 0MK4U5LV950ST3VRXL8G
- QR info : KNX:S:00fa10010713;P:0MK4U5LV950ST3VRXL8G

### Data points

| url  | channel/usage       | instance |resource type | interface type | data type |
|------| --------------------| -------- | -------------| ---------------|-----------|
| "/p/o_1_1"  | SendShot |  1 |urn:knx:dpa.65500.101 | if.o | |
| "/p/o_1_2"  | ReceiveShot |  1 |urn:knx:dpa.65501.111 | if.i | |
| "/p/o_1_3"  | SendShotStatus |  1 |urn:knx:dpa.65501.102 | if.o | |
| "/p/o_1_4"  | ReceiveShotStatus |  1 |urn:knx:dpa.65500.112 | if.i | |
| "/p/o_1_5"  | SendReady |  1 |urn:knx:dpa.65500.103 | if.o | |
| "/p/o_1_6"  | ReceiveReady |  1 |urn:knx:dpa.65501.113 | if.i | |

### Parameters

| url  | channel/usage   | instance | data type |
|------| ----------------| ---------| --------- |
| "/p/p_1_1"  | Starting_Player |  1 | |

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

For querying the metadata items implemented one can use the following commands:

```
  GET /p/o_1_1?m=*
  GET /p/o_1_2?m=*
  GET /p/o_1_3?m=*
  GET /p/o_1_4?m=*
  GET /p/o_1_5?m=*
  GET /p/o_1_6?m=*
```
## Development Board Jumper settings
|Jumper | Setting | Usage |
|-------|---------| ------|
| U19 (SW1)   | Not Connected | e-paper|
| U20 (SW2)   | 1-2     | button|
| U25 (SW3)   | 1-2     | button|
| U26 (SW4)   | 1-2     | button|

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

Once the build completes, you will find `knx_eink_battleships.bin` inside your `build_win_bin/` directory. 
You must then flash the binary. 
This can be done via USB, with a programmer (J-Link / NuLink) or via Over-the-Air Software Update. 
See the [Flashing Guide](https://github.com/Cascoda/cascoda-sdk/blob/master/docs/guides/flashing.md) for a detailed how-to.


***


