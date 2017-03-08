//#############################################################################
//File Name   : IMU.h
//Programmer  : Yan Naing Aye
//Date        : 28 Sep 2015
//WebSite: http://cool-emerald.blogspot.com/
//License: Creative Commons Attribution-ShareAlike 3.0
//         http://creativecommons.org/licenses/by-sa/3.0/
//#############################################################################
#ifndef	IMU_H
	#define	IMU_H
//-----------------------------------------------------------------------------
// Acc CK 0 - P0_04
// Acc CK 1 - P0_05
// Acc CK 2 - P0_06
// Gyro CK  - P0_07
// |31-28|27-24|23-20|19-16|15-12|11-08|07-04|03-00|
// |0000 |0000 |0000 |0000 |0000 |0000 |0111 |0000 | CK_ACC = 0x00000070
// |0000 |0000 |0000 |0000 |0000 |0000 |1000 |0000 | CK_GYR = 0x00000080
#define CK_ACC 0x00000070
#define CK_GYR 0x00000080

// Acc MOSI 0 - P0_18
// Acc MOSI 1 - P0_19
// Acc MOSI 2 - P0_20
// Gyro MOSI  - P0_21
// |31-28|27-24|23-20|19-16|15-12|11-08|07-04|03-00|
// |0000 |0000 |0001 |1100 |0000 |0000 |0000 |0000 | MO_ACC = 0x001C0000
// |0000 |0000 |0010 |0000 |0000 |0000 |0000 |0000 | MO_GYR = 0x00200000
#define MO_ACC 0x001C0000
#define MO_GYR 0x00200000


// Acc CS 0   - P0_25
// Acc CS 1   - P0_26
// Acc CS 2   - P0_27
// Gyro CS    - P0_28
// |31-28|27-24|23-20|19-16|15-12|11-08|07-04|03-00|
// |0000 |1110 |0000 |0000 |0000 |0000 |0000 |0000 | CS_ACC = 0x0E000000
// |0001 |0000 |0000 |0000 |0000 |0000 |0000 |0000 | CS_GYR = 0x10000000
#define CS_ACC 0x0E400000
#define CS_GYR 0x10000000

// Acc MISO 0  - P1_0
// Acc MISO 1  - P1_1
// Acc MISO 2  - P1_2
// Acc MISO 3  - P1_3
// Acc MISO 4  - P1_4
// Acc MISO 5  - P1_5
// Gyro MISO   - P1_6
// |31-28|27-24|23-20|19-16|15-12|11-08|07-04|03-00|
// |0000 |0000 |0000 |0000 |0000 |0000 |0100 |0000 | MI_GYR_MASK 0x00000040
// |0000 |0000 |0000 |0000 |0000 |0000 |0011 |1111 | MI_ACC_MASK 0x0000002F
#define MI_GYR_MASK 0x00000040
#define MI_ACC_MASK 0x0000002F
#define MI_A0 0
#define MI_A1 1
#define MI_A2 2
#define MI_A3 3
#define MI_A4 4
#define MI_A5 5
#define MI_G0 6
#define K 0.596994 //milli meter per second square
#define K4G 8.75 //milli degree per second
// (4000/65535) milli-g per digit for +/-2g full scale using 16 bit digital output
//g at Singapore is 9781mm/s2
typedef struct {
	int a0;
	int a1;
	int a2;
	int a3;
	int a4;
	int a5;
	int g0;
} IMU_Data;

typedef struct {
	float x;
	float y;
	float z;
} IMU_Acc;
//-----------------------------------------------------------------------------
void IMU_Init();
void IMU_CS_High();
void IMU_CS_Low();
void IMU_CS_High_Acc();
void IMU_CS_Low_Acc();
void IMU_CS_High_Gyro();
void IMU_CS_Low_Gyro();
IMU_Data IMU_Transfer(unsigned long aw,unsigned long gw);
void IMU_Rd();
IMU_Data IMU_x();
IMU_Data IMU_y();
IMU_Data IMU_z();
IMU_Acc IMU_a();
//-----------------------------------------------------------------------------
#endif
