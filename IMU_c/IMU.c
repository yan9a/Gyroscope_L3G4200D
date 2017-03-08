//#############################################################################
//File Name   : IMU.c
//Programmer  : Yan Naing Aye
//Date        : 28 Sep 2015
//WebSite: http://cool-emerald.blogspot.com/
//License: Creative Commons Attribution-ShareAlike 3.0
//         http://creativecommons.org/licenses/by-sa/3.0/
//#############################################################################
#include "chip.h"
#include "IMU.h"
IMU_Data l,h,x,y,z;
IMU_Acc a;
//-----------------------------------------------------------------------------
void IMU_Init() {
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A0, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A1, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A2, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A3, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A4, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_A5, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, MI_G0, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 5, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 25, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 26, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 27, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 28, IOCON_MODE_INACT | IOCON_FUNC0 | IOCON_DIGITAL_EN | IOCON_GPIO_MODE);
	Chip_GPIO_SetPortDIROutput(LPC_GPIO,0,(CK_ACC|CK_GYR|MO_ACC|MO_GYR|CS_ACC|CS_GYR));
	Chip_GPIO_SetPortDIRInput(LPC_GPIO,1,(MI_GYR_MASK|MI_ACC_MASK));
	Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,(CK_ACC|CK_GYR|MO_ACC|MO_GYR|CS_ACC|CS_GYR));

	//Accelerometer settings
	IMU_CS_Low_Acc();
	IMU_Transfer(0x20,0);
	IMU_Transfer(0x7F,0);
	IMU_CS_High_Acc();

	//Gyro settings
	//CTRL_REG2 = |0 |0 |HPM1|HPM0|HPCF3|HPCF2|HPCF1|HPCF0|
	//Default =   |0 |0 |0   |0   |0    |0    |0    |0    |
	//HPM = 00 => Normal (High Pass filter Mode selection)
	//HPC = 1001 => 0.1 High Pass Filter Cut-off freq configuration
	IMU_CS_Low_Gyro();
	IMU_Transfer(0,0x21);
	IMU_Transfer(0,0x09);
	IMU_CS_High_Gyro();

	//CTRL_REG3 = |I1_Int1|I1_Boot|H_Lactive|PP_OD|I2DRDY|I2_WTM|I2_ORun|I2_Empty|
	//Default =   |0      |0      |0        |0    |0     |0     |0      |0       |
	//Use Default

	//CTRL_REG4 = |BDU|BLE|FS1|FS0| - |ST1|ST0|SIM|
	//Default =   |0  |0  |0  |0  |0  |0  |0  |0  |
	//BDU = 1 => Block Data Update
	//BLE = 0 => Little endian
	//FS = 00 => 250 dps (Full scale selection)
	//ST = 000 => Disable Self test
	IMU_CS_Low_Gyro();
	IMU_Transfer(0,0x23);
	IMU_Transfer(0,0x80);
	IMU_CS_High_Gyro();

	//CTRL_REG5 = |BOOT|FIFO_EN| - |HPen|INT1_Sel1|INT1_Sel0|Out_Sel1|Out_Sel0|
	//Default =   |0   |0      |0  |0   |0        |0        |0       |0       |
	//BOOT = 0 => Normal mode (Reboot Memory Content)
	//FIFO_EN = 0 => disable FIFO
	//HPen = 0 => disable (High Pass Filter)
	//INT1_Sel = 00 => Non high pass filtered data are used for interrupt generation
	//Out_Sel = 00 => no filtering
	//Use Default

	//CTRL_REG1 = |DR1|DR0|BW1|BW0|PD|Zen|Yen|Xen|
	//Default =   |0  |0  |0  |0  |0 |1  |1  |1  |
	//DR = 11 => ODR 800 Hz (output data rate)
	//BW = 10 => Cut-off 50 (Bandwidth 50 Hz)
	//PD = 1 => Normal
	//Zen = Yen = Xen = 1 => Enable
	IMU_CS_Low_Gyro();
	IMU_Transfer(0,0x20);
	IMU_Transfer(0,0xEF);
	IMU_CS_High_Gyro();
}
//-----------------------------------------------------------------------------
IMU_Data IMU_Transfer(unsigned long aw,unsigned long gw) {
	int i=0,a0=0,a1=0,a2=0,a3=0,a4=0,a5=0,gy=0;
	IMU_Data d;
	for(i=0;i<8;i++)
	{
		Chip_GPIO_SetPortOutLow(LPC_GPIO,0,(CK_ACC|CK_GYR)); // GSCK=0;
			if(aw & 0x80) Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,MO_ACC);
			else Chip_GPIO_SetPortOutLow(LPC_GPIO,0,MO_ACC);
			if(gw & 0x80) Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,MO_GYR);
			else Chip_GPIO_SetPortOutLow(LPC_GPIO,0,MO_GYR);
			aw<<=1;	gw<<=1;	a0<<=1; a1<<=1; a2<<=1; a3<<=1; a4<<=1; a5<<=1; gy<<=1;
		Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,(CK_ACC|CK_GYR)); // GSCK=1;
			a0|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A0);
			a1|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A1);
			a2|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A2);
			a3|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A3);
			a4|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A4);
			a5|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_A5);
			gy|=Chip_GPIO_GetPinState(LPC_GPIO, 1, MI_G0);
	}
	d.a0=a0; d.a1=a1; d.a2=a2; d.a3=a3; d.a4=a4; d.a5=a5; d.g0=gy;
	return d;
}
void IMU_CS_High() {
	Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,(CS_ACC|CS_GYR));
}
void IMU_CS_Low() {
	Chip_GPIO_SetPortOutLow(LPC_GPIO,0,(CS_ACC|CS_GYR));
}
void IMU_CS_High_Acc() {
	Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,CS_ACC);
}
void IMU_CS_Low_Acc() {
	Chip_GPIO_SetPortOutLow(LPC_GPIO,0,CS_ACC);
}
void IMU_CS_High_Gyro() {
	Chip_GPIO_SetPortOutHigh(LPC_GPIO,0,CS_GYR);
}
void IMU_CS_Low_Gyro() {
	Chip_GPIO_SetPortOutLow(LPC_GPIO,0,CS_GYR);
}
void IMU_Rd() {
	//Read IMU
	IMU_CS_Low();
	IMU_Transfer(0xA8,0xE8);
	l=IMU_Transfer(0,0);
	h=IMU_Transfer(0,0);
	l.a0=l.a0|(h.a0<<8); if(l.a0 & 0x8000) l.a0|=0xFFFF0000;
	l.a1=l.a1|(h.a1<<8); if(l.a1 & 0x8000) l.a1|=0xFFFF0000;
	l.a2=l.a2|(h.a2<<8); if(l.a2 & 0x8000) l.a2|=0xFFFF0000;
	l.a3=l.a3|(h.a3<<8); if(l.a3 & 0x8000) l.a3|=0xFFFF0000;
	l.a4=l.a4|(h.a4<<8); if(l.a4 & 0x8000) l.a4|=0xFFFF0000;
	l.a5=l.a5|(h.a5<<8); if(l.a5 & 0x8000) l.a5|=0xFFFF0000;
	l.g0=l.g0|(h.g0<<8); if(l.g0 & 0x8000) l.g0|=0xFFFF0000;
	x=l;
	l=IMU_Transfer(0,0);
	h=IMU_Transfer(0,0);
	l.a0=l.a0|(h.a0<<8); if(l.a0 & 0x8000) l.a0|=0xFFFF0000;
	l.a1=l.a1|(h.a1<<8); if(l.a1 & 0x8000) l.a1|=0xFFFF0000;
	l.a2=l.a2|(h.a2<<8); if(l.a2 & 0x8000) l.a2|=0xFFFF0000;
	l.a3=l.a3|(h.a3<<8); if(l.a3 & 0x8000) l.a3|=0xFFFF0000;
	l.a4=l.a4|(h.a4<<8); if(l.a4 & 0x8000) l.a4|=0xFFFF0000;
	l.a5=l.a5|(h.a5<<8); if(l.a5 & 0x8000) l.a5|=0xFFFF0000;
	l.g0=l.g0|(h.g0<<8); if(l.g0 & 0x8000) l.g0|=0xFFFF0000;
	y=l;
	l=IMU_Transfer(0,0);
	h=IMU_Transfer(0,0);
	l.a0=l.a0|(h.a0<<8); if(l.a0 & 0x8000) l.a0|=0xFFFF0000;
	l.a1=l.a1|(h.a1<<8); if(l.a1 & 0x8000) l.a1|=0xFFFF0000;
	l.a2=l.a2|(h.a2<<8); if(l.a2 & 0x8000) l.a2|=0xFFFF0000;
	l.a3=l.a3|(h.a3<<8); if(l.a3 & 0x8000) l.a3|=0xFFFF0000;
	l.a4=l.a4|(h.a4<<8); if(l.a4 & 0x8000) l.a4|=0xFFFF0000;
	l.a5=l.a5|(h.a5<<8); if(l.a5 & 0x8000) l.a5|=0xFFFF0000;
	l.g0=l.g0|(h.g0<<8); if(l.g0 & 0x8000) l.g0|=0xFFFF0000;
	z=l;
	IMU_CS_High();
	IMU_a(); //convert to acc
}
IMU_Data IMU_x() {
	return x;
}
IMU_Data IMU_y() {
	return y;
}
IMU_Data IMU_z() {
	return z;
}
IMU_Acc IMU_a() {
	float ax,ay,az;
	IMU_Acc a;
	//ax=K*x.g0;
	//ay=K*y.g0;
	//az=K*z.g0;
	ax=x.a0;
	ay=y.a0;
	az=z.a0;
	a.x=ax;
	a.y=ay;
	a.z=az;
	return a;
}
//-----------------------------------------------------------------------------

