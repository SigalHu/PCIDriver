/*
 * ����ȫ�ֱ���
 *
 * @author SigalHu
 * @date 2015/12/21
 */

#pragma once

#define PhySpaceSize 11534336  // hu �������������ռ�Ϊ11MB
#define DEBUG_Hu

class driver
{
public:
	int status;                                   // hu ����״̬
	void * EDMAWinDriver_Device;                  // hu ָ��ײ�����������ڴ�ռ��׵�ַ(����ϵͳ���õ������ַ)
	unsigned long EDMAPhysicalAddress_Device;     // hu �����ַ
	unsigned long EDMAHandle_Device;              // hu ����ռ���
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