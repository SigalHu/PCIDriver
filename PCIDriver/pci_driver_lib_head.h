/*
 * pci6455������غ�������ӿ�
 *
 * @author ������
 * @date 2015/12/22
 */

#ifndef _PCI_DRIVER_LIB_HEAD_H_
#define _PCI_DRIVER_LIB_HEAD_H_

#ifdef __cplusplus
extern "C" {
#endif

// hu //////////////////////////////////////////////////////
#define WM_CALLBACKMESSAGE_0 WM_USER+470
#define WM_CALLBACKMESSAGE_1 WM_USER+471
extern HWND ghWnd_0;
extern HWND ghWnd_1;
////////////////////////////////////////////////////////////

#define _EXPORTING
#ifdef _EXPORTING
#define DRIVER_DECLSPEC    __declspec(dllexport)
#else
#define DRIVER_DECLSPEC    __declspec(dllimport)
#endif

// hu ��ʼ��������������
// hu 0 -- �ɹ� 1 -- δɨ�赽PCI�豸 2 -- ��ȡ�豸�����������ʧ�� 3 -- ��ʼ���豸������ʧ��
DRIVER_DECLSPEC DWORD __stdcall Initialize_DSPDriver(int deviceX);

// hu ����������������
// hu 0 -- �ɹ� 1 -- PCI�豸δ���� 2 -- �ر�PCI�豸ʧ�� 3 -- δɨ�赽PCI�豸 4 -- ��ȡ�豸�����������ʧ��
DRIVER_DECLSPEC DWORD __stdcall Reset_DSPDriver(int deviceX);

// hu ��ָ��DSP��ַд32bit����
// hu 0 -- �ɹ� 1 -- PCI�豸д��ʧ��
DRIVER_DECLSPEC DWORD __stdcall WriteToDSP(int deviceX,int barX, unsigned long dwoffset, unsigned int write32);

// hu ��ָ��DSP��ַд1KB����
// hu 0 -- �ɹ� 1 -- PCI�豸д��ʧ��
DRIVER_DECLSPEC DWORD __stdcall WriteToDSP_256Words(int deviceX,int barX, DWORD dwoffset, UINT32 *write256Words);

// hu ��ָ��DSP��ַ��32bit����
// hu 0 -- �ɹ� 1 -- PCI�豸��ȡʧ��
DRIVER_DECLSPEC DWORD __stdcall ReadfromDSP(int deviceX,int barX, unsigned long dwoffset, unsigned int *read32);

// hu ��ָ��DSP��ַ��1KB����
// hu 0 -- �ɹ� 1 -- PCI�豸��ȡʧ��
DRIVER_DECLSPEC DWORD __stdcall ReadfromDSP_256Words(int deviceX,int barX, DWORD dwoffset, UINT32 *read256Words);

// hu ���������򴫵���Ϣ���
DRIVER_DECLSPEC void  __stdcall DllFun(int deviceX,HWND hWn);

// hu ʹ������DSP���ж�
// hu 0 -- �ɹ� 1 -- ʹ��PCI�ж�ʧ��
DRIVER_DECLSPEC DWORD __stdcall EnableInterruptFromDSP(int deviceX);

// hu ��������DSP���ж�
// hu 0 -- �ɹ� 1 -- ����PCI�ж�ʧ��
DRIVER_DECLSPEC DWORD __stdcall DisEnableInterruptFromDSP(int deviceX);

// hu ��DSP�����ж�
// hu 0 -- �ɹ�
DRIVER_DECLSPEC DWORD __stdcall SendIntToDSP(int deviceX);

// hu ���������ַ�������ڴ�ռ�
// hu 0 -- �ɹ� 1 -- �����������ռ�������������10 2 -- ��������ռ�ʧ�� 3 -- CPU����������ռ�ͬ��ʧ��
DRIVER_DECLSPEC DWORD __stdcall ApplyForPA(int deviceX, DWORD dwDMABufSize, void * * pBufAddress, DWORD * pBufPhysicalAddress, DWORD * DMAHandle);

// hu �ͷ�������Ŀռ�
// hu 0 -- �ɹ� 1 -- I/O����������ռ�ͬ��ʧ�� 2 -- �ͷ�����ռ�ʧ��
DRIVER_DECLSPEC DWORD __stdcall FreePA(int deviceX, DWORD DMAHandle);

// hu �ر�������������
// hu 0 -- �ɹ� 1 -- �ر�PCI����ʧ�� 2 -- ж���豸������ʧ�� 3 -- �ر�PCI������ж���豸������ʧ��
DRIVER_DECLSPEC DWORD __stdcall Close_DSPDriver(int deviceX);
//__declspec(dllexport) void RandGroupToDSP_to_Buffer(void * RandGroupToDSP);
//__declspec(dllexport) void Buffer_to_EDMA_Receive(void * EDMA_Receive);*/
#endif

#ifdef __cplusplus
}
#endif
