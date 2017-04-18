// PCIDriverTestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "DialogFPGA.h"
#include "DialogDSP.h"
#include "DataDefine.h"
#include "MyTabCtrl.h"
#include "CDlgFlash.h"


// CPCIDriverTestDlg 对话框
class CPCIDriverTestDlg : public CDialog
{
// 构造
public:
	CPCIDriverTestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PCIDriverTest_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
//	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnCALLBACKMESSAGE_0(WPARAM wParam, LPARAM lParam); // hu 生成的消息映射函数
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedSelectBin();
	afx_msg void OnBnClickedSendData();
	afx_msg void OnBnClickedFpgaIni();
	afx_msg void OnTcnSelchangeBarWr(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeShowBin();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);    // hu 添加定时器消息映射函数
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
//    afx_msg void OnClose();           // hu 添加窗口关闭消息映射函数,覆盖原对话框对窗口关闭的默认处理
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyTabCtrl Bar_wr;
	CDialogFPGA Bar_FPGA;
	CDialogDSP Bar_DSP;
	CToolTipCtrl Bubble_Tip;
	driver driver0;
	dmaSource dmaSource0;
	CDlgFlash *MainDlgFlash;
};
