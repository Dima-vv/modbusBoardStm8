
# modbusBoardStm8

Based on https://github.com/FxDev/PetitModbus  

firmware for modbus relay board on stm8s103f3p6 from aliexpress like this

<img src="https://user-images.githubusercontent.com/54972378/241314984-3c989ff9-a669-4bab-ac4f-f279179654cc.jpg" width="25%">

## Usage
This firmware doesn't use write/read coils (commands 0x01, 0x02, 0x05). Instead it uses reading/writing to holding registers (comands 0x03, 0x10).  
One feature is swich relay ON temporary (1-32767 seconds. about 9 hours). То use this you have to write to corresponding register (1-4) value, equal time * 2 + 1. So value 21 means 10 seconds, 11 - 5 sec.  
Changing port speed is not suported yet

registers:  
 - 1,2,3,4 - control relay outputs
 - 5,6,7,8 - view timer values
 - 9,10,11,12 - inputs
 - 13 - modbus address

on board with address 2
```
# swich relay 2 ON
# write 1 to holding register 2
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 2 -t 4 /dev/ttyUSB0 1
```
```
# swich relay 3 OFF
# write 0 to holding register 3
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 3 -t 4 /dev/ttyUSB0 0
```
```
# swich relay 1 ON on 10 seconds
# write 21 to holding register 1 (10 seconds * 2 + 1)
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 1 -t 4 /dev/ttyUSB0 21
```
```
# Read digital input 1 status - read register 9
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 9 -t 4 /dev/ttyUSB0 

modpoll 3.6 - FieldTalk(tm) Modbus(R) Master Simulator
Copyright (c) 2002-2018 proconX Pty Ltd
Visit https://www.modbusdriver.com for Modbus libraries and tools.

Protocol configuration: Modbus RTU
Slave configuration...: address = 2, start reference = 9, count = 1
Communication.........: /dev/ttyUSB0, 9600, 8, 1, none, t/o 1.00 s, poll rate 1000 ms
Data type.............: 16-bit register, output (holding) register table

-- Polling slave...
[9]: 1
```
```
# change mb address
# write new ddress value (33) to 13 register
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 13 -t 4  /dev/ttyUSB0 33
```

## Build

[SDCC](https://sdcc.sourceforge.net/) required. Version 4.2 works fine


```
git submodule update --init
git apply --directory=PetitModbus/ PetitMB.patch
cmake .
make
```

## Flash
for example  
`stm8flash -c stlinkv2 -p stm8s003?3 -w main.ihx`  
or  
`make flash`  
or by any burning tool use `main.ihx` file

