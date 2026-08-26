# The Generic Info


## The knx_eink_battleships Application



### The KNX information

- serial number : 029B10010713
- password : 0MK4U5LV950ST3VRXL8G
- QR info : KNX:S:029B10010713;P:0MK4U5LV950ST3VRXL8G

- manufacturer    : cascoda
- manufacturer id : 0x029B
- model : KNX Battleships Demo eink
- hardware_type : 000000000000
- hardware version : [0, 4, 0]
- firmware version : [0, 4, 0]


#### MT info

- mt application number : 
- mt application version : 
- mt hardware version : 


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


