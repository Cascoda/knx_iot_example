# The Applications
This application is a KNX IoT Example application, running on Cascoda's Development Board.

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

- knx_iot_example Application (CLI) 

Windows GUI using WxWidgets:

- knx_iot_example_gui Application (wxWidgets)

Embedded for Chili:

- knx_iot_example Application (embedded) 

The general structure of these programs are:

## The knx_iot_example Application

### The KNX information

- serial number : 00FA10010710
- password : 4N6AFK6T83YWDUTW23U2
- QR info : KNX:S:00FA10010710;P:4N6AFK6T83YWDUTW23U2

- manufactorer : 
- model : dev board example
- hardware_type : 000001
- hardware version : [0, 4, 0]
- firmware version : [0, 4, 0]

### Data points

| url  | channel/usage       | instance |resource type | interface type | data type |
|------| --------------------| -------- | -------------| ---------------|-----------|
| "/p/o_1_1"  | LED_1 |  1 |urn:knx:dpa.417.61 | if.a | |
| "/p/o_2_2"  | PB_1 |  1 |urn:knx:dpa.421.61 | if.s | |

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

