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

- knx_eink_battleships Application (CLI) 

Windows GUI using WxWidgets:

- knx_eink_battleships_gui Application (wxWidgets)

Embedded for Chili:

- knx_eink_battleships Application (embedded) 

The general structure of these programs are:

## The knx_eink_battleships Application

### The KNX information

- serial number : 00fa10010713
- password : 0MK4U5LV950ST3VRXL8G
- QR info : KNX:S:00fa10010713;P:0MK4U5LV950ST3VRXL8G

- manufactorer : 
- model : KNX Battleships Demo eink
- hardware_type : 000000000000
- hardware version : [0, 4, 0]
- firmware version : [0, 4, 0]

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

