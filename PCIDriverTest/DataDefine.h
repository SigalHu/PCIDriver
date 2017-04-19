/*
 * 定义全局变量
 *
 * @author SigalHu
 * @date 2015/12/21
 */

#pragma once

#define PhySpaceSize 11534336  // hu 申请的连续物理空间为11MB
#define DEBUG_Hu

class driver
{
public:
	int status;                                   // hu 驱动状态
	void * EDMAWinDriver_Device;                  // hu 指向底层驱动申请的内存空间首地址(操作系统可用的虚拟地址)
	unsigned long EDMAPhysicalAddress_Device;     // hu 物理地址
	unsigned long EDMAHandle_Device;              // hu 物理空间句柄
};

class dmaSource
{
public:
	UINT  Deliver_Time;
	CFile Deliver_FP;
	UINT  Deliver_Len;
	UINT  GPIO2FPGA_flag;
};

//extern driver driver0;
//extern dmaSource dmaSource0;