//#############################################################################
//Interface to gyroscope
//File Name   : L3G4200D.c
//Programmer  : Yan Naing Aye
//Date        : 18 April 2011
//Last Updated: 18 April 2011
//#############################################################################
#include "Headers.h"
//*****************************************************************************
ui08 idata gxL,gxH,gyL,gyH,gzL,gzH;
//-----------------------------------------------------------------------------
void GyrInit()
{
	//Init pins
	G_CS=1; 	 
	GSCK=1; 
	GSDI=1; 
	GSDO=1; 
	//-------------------------------------------------------------------------	
	//ComPrintHex(GyrRd(WHO_AM_I));
	//-------------------------------------------------------------------------
	//CTRL_REG1 = |DR1|DR0|BW1|BW0|PD|Zen|Yen|Xen|
	//Default =   |0  |0  |0  |0  |0 |1  |1  |1  |
	//DR = 10 => ODR 400 Hz
	//BW = 00 => Cut-off 20 
	//PD = 1 => Normal
	//Zen = Yen = Xen = 1 => Enable
	GyrWr(CTRL_REG1,0x8F);
	//-------------------------------------------------------------------------
	//CTRL_REG2 = |0 |0 |HPM1|HPM0|HPCF3|HPCF2|HPCF1|HPCF0|
	//Default =   |0 |0 |0   |0   |0    |0    |0    |0    |
	//HPM = 00 => Normal (High Pass filter Mode selection)
	//HPC = 1001 => 0.05 High Pass Filter Cut-off freq configuration
	GyrWr(CTRL_REG2,0x09);
	//-------------------------------------------------------------------------
	//CTRL_REG3 = |I1_Int1|I1_Boot|H_Lactive|PP_OD|I2DRDY|I2_WTM|I2_ORun|I2_Empty|
	//Default =   |0      |0      |0        |0    |0     |0     |0      |0       |
	//Use Default
	//GyrWr(CTRL_REG3,0x00);
	//-------------------------------------------------------------------------
	//CTRL_REG4 = |BDU|BLE|FS1|FS0| - |ST1|ST0|SIM|
	//Default =   |0  |0  |0  |0  |0  |0  |0  |0  |
	//BDU = 0 => Continuous update (Block Data Update)
	//BLE = 0 => Little endian 
	//FS = 00 => 250 dps (Full scale selection)
	//ST = 000 => Disable Self test
	//Use default
	//GyrWr(CTRL_REG4,0x00);
	//-------------------------------------------------------------------------
	//CTRL_REG5 = |BOOT|FIFO_EN| - |HPen|INT1_Sel1|INT1_Sel0|Out_Sel1|Out_Sel0|
	//Default =   |0   |0      |0  |0   |0        |0        |0       |0       |
	//BOOT = 0 => Normal mode (Reboot Memory Content)
	//FIFO_EN = 0 => disable FIFO 
	//HPen = 0 => disable (High Pass Filter)
	//INT1_Sel = 00 => Non high pass filtered data are used for interrupt generation
	//Out_Sel = 10 => use low pass filter 2	
	GyrWr(CTRL_REG5,0x02);
	//-------------------------------------------------------------------------
	//FIFO_CTRL_REG = |FM2|FM1|FM0|WTM4|WTM3|WTM2|WTM1|WTM0|
	//Default =       |0  |0  |0  |0   |0   |0   |0   |0   |
	//FM = 000 => Bypass mode
	//WTM = 00000 => FIFO threshold is not used. (Water Mark level setting)
	//Use default
	//GyrWr(FIFO_CTRL_REG,0x00);
	//-------------------------------------------------------------------------
	//INT are not used
}
//-----------------------------------------------------------------------------
ui08 GyrRd(ui08 ad)
{
	ui08 i,v;

	ad=(ad&0xBF)|0x80;//set bit 7 to read and clear bit 6 not to increase address
	
	//SysDI();//Enter critical section
	GSCK=1;
	G_CS=0; //enable
	for(i=0;i<16;i++)
	{	
	    GSCK=0;
		GSDI=(ad & 0x80)?1:0;
		ad<<=1;				
		
		GSCK=1;
		v<<=1;
		v|=GSDO;				
	}
	G_CS=1;	
	//SysEI();//Exit critical section
	return v;
}
//-----------------------------------------------------------------------------
void GyrWr(ui08 ad,ui08 v)
{
	ui08 i;

	ad=ad&0x3F;//clear bit 7 to write and bit 6 not to increase address
		
	//SysDI();//Enter critical section
	GSCK=1;
	G_CS=0; //enable
	for(i=0;i<16;i++)
	{	
		if(i<8)
		{	
			GSCK=0;
			GSDI=(ad & 0x80)?1:0;							
			
			GSCK=1;	
			ad<<=1;		
		}
		else
		{	
			GSCK=0;
			GSDI=(v & 0x80)?1:0;							
			
			GSCK=1;	
			v<<=1;		
		}		
	}
	G_CS=1;	
	//SysEI();//Exit critical section
}
//-----------------------------------------------------------------------------
void GyrUpdate()
{
	gxL=GyrRd(OUT_X_L);
	gxH=GyrRd(OUT_X_H);
	gyL=GyrRd(OUT_Y_L);
	gyH=GyrRd(OUT_Y_H);
	gzL=GyrRd(OUT_Z_L);
	gzH=GyrRd(OUT_Z_H);	
}
//-----------------------------------------------------------------------------
void GyrGetVal(ui08 gv[])
{
	gv[0]=gxH;
	gv[1]=gxL;
	gv[2]=gyH;
	gv[3]=gyL;
	gv[4]=gzH;
	gv[5]=gzL;
}
//-----------------------------------------------------------------------------