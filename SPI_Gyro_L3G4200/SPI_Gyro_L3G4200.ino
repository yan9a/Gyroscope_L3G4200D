#include <SPI.h>
const int CS_Pin = 10;// set pin 10 as the chip select
//SPI pin configuration: pin 11 as MOSI (SDI), pin 12 as MISO (SDO) , pin 13 as clock (SPC)

int x,y,z;
float K=8.75; // 8.75 mdps per digit for +/-250dps full scale using 16 bit digital output

void setup() {
  Serial.begin(115200);
  pinMode (CS_Pin, OUTPUT);  //Chip Select pin to control SPI
  digitalWrite(CS_Pin, HIGH);//Disable SPI
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE3);

  //b7 = set (1) for reading 
  //b6 = cleared (0) not to auto increase address 
  //b5-b0 = register address
  
  //digitalWrite(CS_Pin, LOW);//Enable SPI
  //-------------------------------------------------------------------------
  //SPI.transfer(0x8F);//Read 'Who am I?' register
  //x=SPI.transfer(0x00);//Get return value of Who am I
  //-------------------------------------------------------------------------
  //digitalWrite(CS_Pin, HIGH);//Disable SPI
  
  digitalWrite(CS_Pin, LOW);//Enable SPI
  //-------------------------------------------------------------------------
  //CTRL_REG2 = |0 |0 |HPM1|HPM0|HPCF3|HPCF2|HPCF1|HPCF0|
  //Default =   |0 |0 |0   |0   |0    |0    |0    |0    |
  //HPM = 00 => Normal (High Pass filter Mode selection)
  //HPC = 1001 => 0.1 High Pass Filter Cut-off freq configuration
  SPI.transfer(0x21);//address 
  SPI.transfer(0x09);//register value to write
  //-------------------------------------------------------------------------
  digitalWrite(CS_Pin, HIGH);//Disable SPI
  
  //-------------------------------------------------------------------------
  //CTRL_REG3 = |I1_Int1|I1_Boot|H_Lactive|PP_OD|I2DRDY|I2_WTM|I2_ORun|I2_Empty|
  //Default =   |0      |0      |0        |0    |0     |0     |0      |0       |
  //Use Default
  //-------------------------------------------------------------------------
  
  digitalWrite(CS_Pin, LOW);//Enable SPI
  //-------------------------------------------------------------------------
  //CTRL_REG4 = |BDU|BLE|FS1|FS0| - |ST1|ST0|SIM|
  //Default =   |0  |0  |0  |0  |0  |0  |0  |0  |
  //BDU = 1 => Block Data Update
  //BLE = 0 => Little endian 
  //FS = 00 => 250 dps (Full scale selection)
  //ST = 000 => Disable Self test
  SPI.transfer(0x23);//address 
  SPI.transfer(0x80);//register value to write
  //-------------------------------------------------------------------------
  digitalWrite(CS_Pin, HIGH);//Disable SPI

  //-------------------------------------------------------------------------
  //CTRL_REG5 = |BOOT|FIFO_EN| - |HPen|INT1_Sel1|INT1_Sel0|Out_Sel1|Out_Sel0|
  //Default =   |0   |0      |0  |0   |0        |0        |0       |0       |
  //BOOT = 0 => Normal mode (Reboot Memory Content)
  //FIFO_EN = 0 => disable FIFO 
  //HPen = 0 => disable (High Pass Filter)
  //INT1_Sel = 00 => Non high pass filtered data are used for interrupt generation
  //Out_Sel = 00 => no filtering
  //Use Default
  //-------------------------------------------------------------------------
  
  digitalWrite(CS_Pin, LOW);//Enable SPI
  //-------------------------------------------------------------------------
  //CTRL_REG1 = |DR1|DR0|BW1|BW0|PD|Zen|Yen|Xen|
  //Default =   |0  |0  |0  |0  |0 |1  |1  |1  |
  //DR = 11 => ODR 800 Hz (output data rate)
  //BW = 10 => Cut-off 50 (Bandwidth 50 Hz)
  //PD = 1 => Normal
  //Zen = Yen = Xen = 1 => Enable
  SPI.transfer(0x20);//address
  SPI.transfer(0xEF);// data rate 800, BW 50Hz, enable all axes
  //-------------------------------------------------------------------------
  digitalWrite(CS_Pin, HIGH);//Disable SPI
}

void loop() {
  delay(10);
  digitalWrite(CS_Pin, LOW);//Enable SPI
  SPI.transfer(0xE8);//Send address of LSB of x. Address is auto-increased after each reading.
  x = SPI.transfer(0) | SPI.transfer(0)<<8; //x axis 
  y = SPI.transfer(0) | SPI.transfer(0)<<8; //y axis
  z = SPI.transfer(0) | SPI.transfer(0)<<8; //z axis
  digitalWrite(CS_Pin, HIGH);//Disable SPI
  Serial.println(String(K*x)+" "+String(K*y)+" "+String(K*z));
}

