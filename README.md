
# modbusBoardStm8

Readme is comming...

firmware for modbus relay board on stm8s103f3p6 from aliexpress like this

<img src="https://user-images.githubusercontent.com/54972378/241314984-3c989ff9-a669-4bab-ac4f-f279179654cc.jpg" width="25%">

## Usage
on board with address 2
```
#swich relay 2 ON
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 2 -t 4 /dev/ttyUSB0 1

#swich relay 3 OFF
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 3 -t 4 /dev/ttyUSB0 0

#swich relay 1 ON on 10 seconds
modpoll -1  -m rtu -a 2 -b 9600 -p none -r 1 -t 4 /dev/ttyUSB0 21

```

## Build

```
git submodule update
git apply --directory=PetitModbus/ PetitMB.patch
```
