// PCIDriverTestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "DialogFPGA.h"
#include "DialogDSP.h"
#include "DataDefine.h"
#include "MyTabCtrl.h"
#include "CDlgFlash.h"


// CPCIDriverTestDlg �Ի���
class CPCIDriverTestDlg : public CDialog
{
// ����
public:
	CPCIDriverTestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PCIDriverTest_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnCALLBACKMESSAGE_0(WPARAM wParam, LPARAM lParam); // hu ���ɵ���Ϣӳ�亯��
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelectBin();
	afx_msg void OnBnClickedSendData();
	afx_msg void OnBnClickedFpgaIni();
	afx_msg void OnTcnSelchangeBarWr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeShowBin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);    // hu ��Ӷ�ʱ����Ϣӳ�亯��
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//    afx_msg void OnClose();           // hu ��Ӵ��ڹر���Ϣӳ�亯��,����ԭ�Ի���Դ��ڹرյ�Ĭ�ϴ���
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyTabCtrl Bar_wr;
	CDialogFPGA Bar_FPGA;
	CDialogDSP Bar_DSP;
	CToolTipCtrl Bubble_Tip;
	driver driver0;
	dmaSource dmaSource0;
	CDlgFlash *MainDlgFlash;
};
