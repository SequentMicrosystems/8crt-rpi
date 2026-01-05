[![8crt-rpi](sequent.jpg)](https://sequentmicrosystems.com)

# Modbus

The Sequent Microsystems [HALL Current Sensor DAQ 8-Layer Stackable HAT for Raspberry Pi](https://sequentmicrosystems.com/products/hall-current-sensor-daq-stackable-hat-for-raspberry-pi) can be accessed through the Modbus RTU protocol over the RS-485 port.
You can set up the RS-485 port with the **8crt** command.

Example:
```bash
~$ 8crt 0 cfg485wr 1 9600 1 0 1
```
Sets Modbus RTU, Baudrate: 9600bps, 1 Stop Bit,  parity: None, slave address base: 1


```bash
~$ 8crt -h cfg485wr
```
Display the complete set of options:


## Slave Address
The three address jumpers have a dual purpose. 
When the card is accessed from a Raspberry Pi, the jumpers set the I2C address offset, allowing stacking of up to 8 cards.
When the card is accessed via the MODBUS protocol over the RS-485 port, the jumpers set the offset of the RS-485 base address configured by the software.

## Modbus object types
All Modbus RTU object types with standard addresses are implemented: Coils, Discrete Inputs, Input Registers, and Holding Registers.

### Coils

Access level Read/Write, Size 1 bit

| Device function | Register Address | Modbus Address | Description |
| --- | --- | --- | --- |
| COIL_LED1 | 0001 | 0x00 | Led #1 control |
|	COIL_LED2 | 0002 | 0x01 | Led #2 control |
|	COIL_LED3 | 0003 | 0x02 | Led #3 control |
|	COIL_LED4 | 0004 | 0x03 | Led #4 control |
| COIL_LED5 | 0005 | 0x04 | Led #5 control |
|	COIL_LED6 | 0006 | 0x05 | Led #6 control |
|	COIL_LED7 | 0007 | 0x06 | Led #7 control |
|	COIL_LED8 | 0008 | 0x07 | Led #8 control |
|COIL_SO_TYPE1| 0009| 0x08| Sensor #1 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE2| 0010| 0x09| Sensor #2 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE3| 0011| 0x0a| Sensor #3 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE4| 0012| 0x0b| Sensor #4 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE5| 0013| 0x0c| Sensor #5 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE6| 0014| 0x0d| Sensor #6 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE7| 0015| 0x0e| Sensor #7 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |
|COIL_SO_TYPE8| 0016| 0x0f| Sensor #8 out type (0 = 2.5V +/- 0.625V ; 1 = 2.5V +/- 1V) |

### Discrete Inputs

Access level Read Only, Size 1 bit

| Device function | Register Address | Modbus Address | Description |
| --- | --- | --- | --- |



### Input registers

Access level Read Only, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Description |
| --- | --- | --- | --- | --- |
| IR_CRT_IN1| 30001 | 0x00 | A/100 | Instantaneus current for sensor #1| 
| IR_CRT_IN2| 30002 | 0x01 | A/100 | Instantaneus current for sensor #2| 
| IR_CRT_IN3| 30003 | 0x02 | A/100 | Instantaneus current for sensor #3| 
| IR_CRT_IN4| 30004 | 0x03 | A/100 | Instantaneus current for sensor #4| 
| IR_CRT_IN5| 30005 | 0x04 | A/100 | Instantaneus current for sensor #5| 
| IR_CRT_IN6| 30006 | 0x05 | A/100 | Instantaneus current for sensor #6| 
| IR_CRT_IN7| 30007 | 0x06 | A/100 | Instantaneus current for sensor #7| 
| IR_CRT_IN8| 30008 | 0x07 | A/100 | Instantaneus current for sensor #8| 
| IR_CRT_RMS_IN1| 30009 | 0x00 | A/100 | RMS current for sensor #1| 
| IR_CRT_RMS_IN2| 30010 | 0x01 | A/100 | RMS current for sensor #2| 
| IR_CRT_RMS_IN3| 30011 | 0x02 | A/100 | RMS current for sensor #3| 
| IR_CRT_RMS_IN4| 30012 | 0x03 | A/100 | RMS current for sensor #4| 
| IR_CRT_RMS_IN5| 30013 | 0x04 | A/100 | RMS current for sensor #5| 
| IR_CRT_RMS_IN6| 30014 | 0x05 | A/100 | RMS current for sensor #6| 
| IR_CRT_RMS_IN7| 30015 | 0x06 | A/100 | RMS current for sensor #7| 
| IR_CRT_RMS_IN8| 30016 | 0x07 | A/100 | RMS current for sensor #8| 


### Holding registers

Access level Read/Write, Size 16 bits

| Device function | Register Address | Modbus Address | Measurement Unit | Range | Description |
| --- | --- | --- | --- | --- | --- |
| HR_S_RANGE1 | 40001 | 0x00 | A | 2..300 | Sensor #1 full scale  |
| HR_S_RANGE2 | 40002 | 0x01 | A | 2..300 | Sensor #2 full scale  |
| HR_S_RANGE3 | 40003 | 0x02 | A | 2..300 | Sensor #3 full scale  |
| HR_S_RANGE4 | 40004 | 0x03 | A | 2..300 | Sensor #4 full scale  |
| HR_S_RANGE5 | 40005 | 0x04 | A | 2..300 | Sensor #5 full scale  |
| HR_S_RANGE6 | 40006 | 0x05 | A | 2..300 | Sensor #6 full scale  |
| HR_S_RANGE7 | 40007 | 0x06 | A | 2..300 | Sensor #7 full scale  |
| HR_S_RANGE8 | 40008 | 0x07 | A | 2..300 | Sensor #8 full scale  |


## Function codes implemented

* Read Coils (0x01)
* Read Discrete Inputs (0x02)
* Read Holding Registers (0x03)
* Read Input Registers (0x04)
* Write Single Coil (0x05)
* Write Single Register (0x06)
* Write Multiple Coils (0x0f)
* Write Multiple registers (0x10)
