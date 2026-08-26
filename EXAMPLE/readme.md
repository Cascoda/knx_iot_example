# The Generic Info


## The knx_iot_example Application



### The KNX information

- serial number : 029B10010710
- password : 4N6AFK6T83YWDUTW23U2
- QR info : KNX:S:029B10010710;P:4N6AFK6T83YWDUTW23U2

- manufacturer    : cascoda
- manufacturer id : 0x029B
- model : dev board example
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
| "/p/o_1_1"  | LED_1 |  1 |urn:knx:dpa.417.61 | if.a | |
| "/p/o_2_2"  | PB_1 |  1 |urn:knx:dpa.421.61 | if.s | |
| "/p/o_3_3"  | InfoOnOff_1 |  1 |urn:knx:dpa.417.51 | if.s | |

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
| "/p/o_3_3"  | InfoOnOff_1 |  desc |Feedback 1 |


