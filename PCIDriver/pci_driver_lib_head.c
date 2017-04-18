#include <stdio.h>
#include "pci_driver_lib.h"
#include "pci_driver_lib_head.h"

HWND ghWnd_0;
HWND ghWnd_1;

DWORD __stdcall Initialize_DSPDriver(int deviceX)
{
	return initialize_PCI(deviceX);
}

DWORD __stdcall Reset_DSPDriver(int deviceX)
{
	return reset_PCI(deviceX);
}

DWORD __stdcall WriteToDSP(int deviceX,int barX, DWORD dwoffset, UINT32 write32)
{
	return  Write(deviceX,barX, dwoffset,write32);
}
DWORD __stdcall WriteToDSP_256Words(int deviceX,int barX, DWORD dwoffset, UINT32 *write256Words)
{
	return Write_256Words(deviceX, barX,  dwoffset, write256Words);
}

DWORD __stdcall ReadfromDSP(int deviceX,int barX, DWORD dwoffset, UINT32 *read32)
{
	return  Read(deviceX,barX, dwoffset,read32);
}


DWORD __stdcall ReadfromDSP_256Words(int deviceX,int barX, DWORD dwoffset, UINT32 *read256Words)
{
	return Read_256Words(deviceX, barX,  dwoffset,  read256Words);
}


void __stdcall DllFun(int deviceX,HWND hWn)  
{
	switch(deviceX)
	{
	case 0:
		ghWnd_0=hWn;
		break;
	case 1:
		ghWnd_1=hWn;
		break;
	}
	
}

DWORD __stdcall EnableInterruptFromDSP(int deviceX)
{
	return Interrupt_1(deviceX);
}


DWORD __stdcall DisEnableInterruptFromDSP(int deviceX)
{
	return DisInterrupt_1(deviceX);
}


DWORD __stdcall SendIntToDSP(int deviceX)
{
	return SendInterrupt(deviceX);
}

DWORD __stdcall ApplyForPA(int deviceX, DWORD dwDMABufSize, void * * pBufAddress, DWORD * pBufPhysicalAddress, DWORD * DMAHandle)
{
	return ApplyForPhysicalAddress(deviceX, dwDMABufSize, pBufAddress, pBufPhysicalAddress, DMAHandle);
}

DWORD __stdcall FreePA(int deviceX, DWORD DMAHandle)
{
	return FreePhysicalAddress(deviceX, DMAHandle);
}


DWORD __stdcall Close_DSPDriver(int deviceX)
{
	return Close_PCI(deviceX);
}


/*
void RandGroupToDSP_to_Buffer(void * RandGroupToDSP) 
{
	memcpy_to_pBuffSend(RandGroupToDSP);
}

void Buffer_to_EDMA_Receive(void * EDMA_Receive)
{
	memcpy_to_EDMA_Receive(EDMA_Receive);
}
*/
