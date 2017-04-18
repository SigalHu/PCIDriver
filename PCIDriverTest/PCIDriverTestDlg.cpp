// PCIDriverTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PCIDriverTest.h"
#include "PCIDriverTestDlg.h"
#include "DialogFPGA.h"
#include "pci_driver_lib_head.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// CPCIDriverTestDlg �Ի���




CPCIDriverTestDlg::CPCIDriverTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPCIDriverTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCIDriverTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BAR_WR, Bar_wr);
}

BEGIN_MESSAGE_MAP(CPCIDriverTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CALLBACKMESSAGE_0, OnCALLBACKMESSAGE_0)    // hu �����Ϣӳ�䴦��
	ON_BN_CLICKED(IDOK, &CPCIDriverTestDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_SELECT_BIN, &CPCIDriverTestDlg::OnBnClickedSelectBin)
	ON_BN_CLICKED(IDC_SEND_DATA, &CPCIDriverTestDlg::OnBnClickedSendData)
	ON_BN_CLICKED(IDC_FPGA_INI, &CPCIDriverTestDlg::OnBnClickedFpgaIni)
	ON_NOTIFY(TCN_SELCHANGE, IDC_BAR_WR, &CPCIDriverTestDlg::OnTcnSelchangeBarWr)
	ON_EN_CHANGE(IDC_SHOW_BIN, &CPCIDriverTestDlg::OnEnChangeShowBin)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_DRAWITEM()
//	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CPCIDriverTestDlg ��Ϣ�������

BOOL CPCIDriverTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	// hu ���ڽ���
    MainDlgFlash = new CDlgFlash(this,9,10,11,12);  // hu ��ʱ��IDΪ9,10,11,12
	MainDlgFlash->SetFlashCreateSpeed(1,5);
	MainDlgFlash->SetFlashDestroySpeed(5,3);
	MainDlgFlash->SetFlashCR2NCRSpeed(12);
	MainDlgFlash->StartFlashCR2NCRDlg();

	// hu ����tab�ӽ���
	Bar_wr.InsertItem(0,_T("FPGA"));  
	Bar_wr.InsertItem(1,_T("DSP")); 
	Bar_FPGA.Create(IDD_DIALOG_FPGA,GetDlgItem(IDC_BAR_WR));
	Bar_DSP.Create(IDD_DIALOG_DSP,GetDlgItem(IDC_BAR_WR));

	// hu ��ÿͻ�����С  
	CRect rs;  
	Bar_wr.GetClientRect(&rs);

	// hu �����ӶԻ����ڸ������е�λ�ã�����ʵ���޸�  
	rs.top+=25;  
	rs.bottom-=1;  
	rs.left+=1;  
	rs.right-=1;

	// hu �����ӶԻ���ߴ粢�ƶ���ָ��λ��  
	Bar_FPGA.MoveWindow(&rs);  
	Bar_DSP.MoveWindow(&rs);

	// hu �ֱ��������غ���ʾ  
	Bar_FPGA.ShowWindow(true);  
	Bar_DSP.ShowWindow(false);  

	// hu ����Ĭ�ϵ�ѡ�  
	Bar_wr.SetCurSel(0);

	// hu FPGA��ַ������
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_FPGA_STARTADDR))->SetLimitText(7); // hu ��������Ϊ7���ֽ�
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_FPGA_ENDADDR))->SetLimitText(7);
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_REPEAT_FPGADATA))->SetLimitText(2); // hu ����ظ�д��99��

	Bar_FPGA.GetDlgItem(IDC_FPGA_STARTADDR)->SetWindowText(_T("0x")); // hu ��enchange��Ϣ�г�ʼ��
	Bar_FPGA.GetDlgItem(IDC_DATA_FPGA)->SetWindowText(_T("1"));
	Bar_FPGA.GetDlgItem(IDC_REPEAT_FPGADATA)->SetWindowText(_T("1"));
	//Bar_FPGA.GetDlgItem(IDC_FPGA_ENDADDR)->SetWindowText(_T("0x"));
	/*FileBinColor.CreateSolidBrush(RGB(255,255,0)); // hu �ı�༭�򱳾���ɫ*/

	// hu DSP��ַ������
	((CEdit*)Bar_DSP.GetDlgItem(IDC_DSP_OFFSADDR))->SetLimitText(10); // hu ��������Ϊ7���ֽ�

	Bar_DSP.GetDlgItem(IDC_DSP_OFFSADDR)->SetWindowText(_T("0x0"));
	Bar_DSP.GetDlgItem(IDC_DATAW_DSP)->SetWindowText(_T("1"));

	// hu ���������ͣ��ʾ
	EnableToolTips(TRUE);
	Bubble_Tip.Create(this);
	Bubble_Tip.Activate(TRUE);
	Bubble_Tip.SetMaxTipWidth(600);         // hu ����������ʾ����ȣ����ú��ʵ�ֶ�����ʾ
	Bar_FPGA.Bubble_Tip_FPGA.Create(this);
	Bar_FPGA.Bubble_Tip_FPGA.Activate(TRUE);
	Bar_FPGA.Bubble_Tip_FPGA.SetMaxTipWidth(600);  
	Bar_DSP.Bubble_Tip_DSP.Create(this);
	Bar_DSP.Bubble_Tip_DSP.Activate(TRUE);
	Bar_DSP.Bubble_Tip_DSP.SetMaxTipWidth(600); 
	
	// hu ��ʼ����ѡ��
	((CButton *)Bar_FPGA.GetDlgItem(IDC_FIX_SELECT))->SetCheck(TRUE);
	((CButton *)Bar_DSP.GetDlgItem(IDC_DDR_SELECT))->SetCheck(TRUE);

	// hu ��ʼ��������
	Bar_FPGA.Progress_FPGA.SetRange(0,100);  // hu ���ý������ķ�Χ
	Bar_FPGA.Progress_FPGA.SetStep(10);       // hu ���ý�������ÿһ��������
	Bar_FPGA.Progress_FPGA.SetPos(0);        // hu ���ý������ĵ�ǰλ��

	// hu ��ʼ��PCI6455
#ifdef DEBUG_Hu
	driver0.status = 4;
#else
	driver0.status = Initialize_DSPDriver(0);
#endif
	switch(driver0.status)
	{
	case 0:GetDlgItem(IDC_SHOW)->SetWindowText(_T("��ʼ��PCI�豸�ɹ���"));break;
	case 1:GetDlgItem(IDC_SHOW)->SetWindowText(_T("δɨ�赽PCI�豸��"));break;
	case 2:GetDlgItem(IDC_SHOW)->SetWindowText(_T("��ȡ�豸�����������ʧ�ܣ�"));break;
	case 3:GetDlgItem(IDC_SHOW)->SetWindowText(_T("��ʼ���豸������ʧ�ܣ�"));break;
	default:GetDlgItem(IDC_SHOW)->SetWindowText(_T("����������ʼ��������Ĭ�Ϸ���ֵ��"));break;
	}

	// hu ע����Ϣ����
	m_hWnd = AfxGetMainWnd()->m_hWnd;
	// hu m_hWnd = this->m_hWnd;
	DllFun(0,m_hWnd) ;//����DLL���������ھ�����ݵ�DLL��

	// hu ��������ռ�
#ifdef DEBUG_Hu
	driver0.status = 4;
#else
	driver0.status = ApplyForPA(0,PhySpaceSize,&driver0.EDMAWinDriver_Device,&driver0.EDMAPhysicalAddress_Device,&driver0.EDMAHandle_Device);
#endif
	switch(driver0.status)
	{
	case 0:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp.Format(_T("%s\r\n����%dMB��������ռ�ɹ���"),temp,PhySpaceSize/1024/1024);
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 1:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n�����������ռ�������������10��";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 2:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n��������ռ�ʧ�ܣ�";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 3:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\nCPU����������ռ�ͬ��ʧ�ܣ�";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	default:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n������������ռ亯����Ĭ�Ϸ���ֵ��";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPCIDriverTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPCIDriverTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
//HCURSOR CPCIDriverTestDlg::OnQueryDragIcon()
//{
//	return static_cast<HCURSOR>(m_hIcon);
//}


void CPCIDriverTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString temp;

	// hu �ͷ�����ռ�
#ifdef DEBUG_Hu
	driver0.status = 3;
#else
	driver0.status = FreePA(0,driver0.EDMAHandle_Device);
#endif
	switch(driver0.status)
	{
	case 0:GetDlgItem(IDC_SHOW)->SetWindowText(_T("�ͷ���������ռ�ɹ���"));break;
	case 1:GetDlgItem(IDC_SHOW)->SetWindowText(_T("I/O����������ռ�ͬ��ʧ�ܣ�"));break;
	case 2:GetDlgItem(IDC_SHOW)->SetWindowText(_T("�ͷ�����ռ�ʧ�ܣ�"));break;
	default:GetDlgItem(IDC_SHOW)->SetWindowText(_T("�����ͷ�����ռ亯����Ĭ�Ϸ���ֵ��"));break;
	}

	// hu �ر��豸
#ifdef DEBUG_Hu
	driver0.status = 4;
#else
	driver0.status = Close_DSPDriver(0);
#endif
	switch(driver0.status)
	{
	case 0:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n�ر�PCI�豸�ɹ���";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 1:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n�ر�PCI�豸ʧ�ܣ�";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 2:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\nж���豸������ʧ�ܣ�";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 3:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n�ر�PCI������ж���豸������ʧ�ܣ�";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	default:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n���ǹر�����������Ĭ�Ϸ���ֵ��";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	}

	MainDlgFlash->SetFlashNCR2CRSpeed(12);
	MainDlgFlash->StartFlashNCR2CRDlg();
	MainDlgFlash->StartFlashDestroyDlg(); //��ʼ�����رնԻ���
	//OnOK();
}

void CPCIDriverTestDlg::OnBnClickedSelectBin()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("�ļ�(*.bin;*.bit)|*.bin;*.bit||"));

	FileDlg.m_ofn.lpstrInitialDir = _T("files");//�ĳ�ʼ��Ŀ¼

	if (FileDlg.DoModal() == IDOK)
	{
		CString path;
		path = FileDlg.GetPathName();
		GetDlgItem(IDC_SHOW_BIN)->SetWindowText(path);
	}
	GetDlgItem(IDC_SHOW_BIN)->ShowWindow(TRUE);	
}

void CPCIDriverTestDlg::OnBnClickedSendData()
{
	// TODO: Add your control notification handler code here
	CString path;

	GetDlgItem(IDC_SHOW_BIN)->GetWindowText(path);
	if (!dmaSource0.Deliver_FP.Open(path,CFile::typeBinary|CFile::modeRead))
	{
		AfxMessageBox(_T("��������ȷ���ļ�·����!"));
		return;
	}

	GetDlgItem(IDC_SHOW_TIME)->SetWindowText(_T(""));

#ifdef DEBUG_Hu
	dmaSource0.Deliver_FP.Close();
#else
	dmaSource0.Deliver_Len = dmaSource0.Deliver_FP.Read(driver0.EDMAWinDriver_Device,PhySpaceSize);

	if (dmaSource0.Deliver_Len < PhySpaceSize)
	{
		WriteToDSP(0,5,0x10,0x1);// hu ���������־λ
	}

	WriteToDSP(0,5,0,driver0.EDMAPhysicalAddress_Device);
	WriteToDSP(0,5,4,dmaSource0.Deliver_Len);

	EnableInterruptFromDSP(0);
	dmaSource0.Deliver_Time = GetTickCount();// hu ��ʼʱ��
	SendIntToDSP(0); // hu ��DSP�����ж�
#endif
	GetDlgItem(IDC_SHOW)->SetWindowText(_T("��ʼEDMA����..."));
}

// hu ��Ӧdll�ϴ���PostMessage()����
LRESULT CPCIDriverTestDlg::OnCALLBACKMESSAGE_0(WPARAM wParam, LPARAM lParam)//hu wParamΪ�жϺ�
{
	CString temp;
	if(wParam == 0) 
	{
		DisEnableInterruptFromDSP(0);    // hu �����ж�
		// hu ///////////���0���жϴ������///////////////	

		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu �����ж�   
	}
	else if(wParam == 1)
	{
		DisEnableInterruptFromDSP(0);    // hu �����ж�
		// hu ///////////���1���жϴ������///////////////

		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu �����ж�
	}
	else if(wParam == 2)  
	{
		DisEnableInterruptFromDSP(0);    // hu �����ж�
		// hu ///////////���2���жϴ������///////////////
		dmaSource0.Deliver_Time = GetTickCount() - dmaSource0.Deliver_Time;  // hu ����ʱ��

		ReadfromDSP(0,5,0x14,&dmaSource0.GPIO2FPGA_flag);
		switch(dmaSource0.GPIO2FPGA_flag)
		{
		case 0:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\n��̬���سɹ���";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 1:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\nFPGA��ʼ��ʧ�ܣ�";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 2:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\nδ�յ�FPGA��INTB�źţ���̬����ʧ�ܣ�";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 3:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\nδ�յ�FPGA��DONE�źţ���̬����ʧ�ܣ�";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		default:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\nδ�յ�Լ������ֵ����̬����ʧ�ܣ�";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		}

		Bar_FPGA.Progress_FPGA.SetPos(100);

		temp.Format(_T("%.1f"),dmaSource0.Deliver_Time/1000.0);
		GetDlgItem(IDC_SHOW_TIME)->SetWindowText(temp);
		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu �����ж�
	}
	else if(wParam == 3)  
	{
		DisEnableInterruptFromDSP(0);    // hu �����ж�
		// hu ///////////���3���жϴ������///////////////
		if (dmaSource0.Deliver_Len == PhySpaceSize)
		{
			dmaSource0.Deliver_Len = dmaSource0.Deliver_FP.Read(driver0.EDMAWinDriver_Device,PhySpaceSize);

			if (dmaSource0.Deliver_Len < PhySpaceSize)
			{
				WriteToDSP(0,5,0x10,0x1);// hu ���������־λ
			}

			WriteToDSP(0,5,0,driver0.EDMAPhysicalAddress_Device);
			WriteToDSP(0,5,4,dmaSource0.Deliver_Len);

			EnableInterruptFromDSP(0);
			SendIntToDSP(0); // hu ��DSP�����ж�

			return 0;
		}
		else
		{
			dmaSource0.Deliver_Time = GetTickCount() - dmaSource0.Deliver_Time;  // hu ����ʱ��

			WriteToDSP(0,3,0xa0a00*4,0xf0f0);// hu ֪ͨfpga������� 0xa0a00Ϊfpga��ȡ���ݵ�ַ

			dmaSource0.Deliver_FP.Close();

			temp.Format(_T("%.1f"),dmaSource0.Deliver_Time/1000.0);
			GetDlgItem(IDC_SHOW_TIME)->SetWindowText(temp);

			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\nEDMA����ɹ���";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
		}
		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu �����ж� 
	}		

	return 0; 
}
void CPCIDriverTestDlg::OnBnClickedFpgaIni()
{
	// TODO: Add your control notification handler code here
	CString path;

	GetDlgItem(IDC_SHOW_BIN)->GetWindowText(path);
	if (!dmaSource0.Deliver_FP.Open(path,CFile::typeBinary|CFile::modeRead))
	{
		AfxMessageBox(_T("��������ȷ���ļ�·����!"));
		return;
	}

	GetDlgItem(IDC_SHOW_TIME)->SetWindowText(_T(""));
#ifdef DEBUG_Hu
	dmaSource0.Deliver_FP.Close();
#else
	dmaSource0.Deliver_Len = dmaSource0.Deliver_FP.Read(driver0.EDMAWinDriver_Device,PhySpaceSize);
	dmaSource0.Deliver_FP.Close();

	if (dmaSource0.Deliver_Len == PhySpaceSize)
	{
		AfxMessageBox(_T("�����ļ����ܴ���11MB!"));
		return;
	}

	WriteToDSP(0,5,8,driver0.EDMAPhysicalAddress_Device);
	WriteToDSP(0,5,0xc,dmaSource0.Deliver_Len);

	EnableInterruptFromDSP(0);
	dmaSource0.Deliver_Time = GetTickCount();// hu ��ʼʱ��
	SendIntToDSP(0); // hu ��DSP�����ж�
#endif
	GetDlgItem(IDC_SHOW)->SetWindowText(_T("��ʼ��̬����..."));

	SetTimer(1,500,NULL);// hu ������ʱ��1,��ʱʱ����0.5��
}

void CPCIDriverTestDlg::OnTcnSelchangeBarWr(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int CurSel = Bar_wr.GetCurSel();  
	switch(CurSel)  
	{  
	case 0:  
		{
			Bar_FPGA.ShowWindow(true);
			Bar_DSP.ShowWindow(false);
			break;  
		}
	case 1: 
		{
			Bar_FPGA.ShowWindow(false);  
			Bar_DSP.ShowWindow(true);  
			break;
		}
	}

	*pResult = 0;
}


void CPCIDriverTestDlg::OnEnChangeShowBin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CWnd* pWnd=GetDlgItem(IDC_SHOW_BIN);	// hu ��ȡ�ı��ؼ�����ָ��
	pWnd->Invalidate();          // hu ��pWnd��ָ��Ĵ����ػ�
}

HBRUSH CPCIDriverTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_EDIT && pWnd->GetDlgCtrlID()==IDC_SHOW_BIN)//ע��˴��ģ�pWnd->��������ûЧ��
	{
		CString path;
		CFileStatus temp;
		GetDlgItem(IDC_SHOW_BIN)->GetWindowText(path);
		if (dmaSource0.Deliver_FP.GetStatus(path,temp))
		{
			pDC->SetTextColor(RGB(0,0,0));   // hu ���ļ����ڣ�������Ϊ��ɫ
		}
		else
		{
			pDC->SetTextColor(RGB(255,0,0)); // hu ���ļ������ڣ�������Ϊ��ɫ
		}
		//pDC->SetBkColor(RGB(255,255,255));// hu �����ı�����ɫ
		//pDC->SetBkMode(TRANSPARENT);// hu ���ñ���͸��
		//hbr = (HBRUSH)FileBinColor;
	}
    
	//if(nCtlColor==CTLCOLOR_BTN)          // hu ���İ�ť��ɫ����������岻��ʾ������
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(0,0,0));
	//	//pDC->SetBkColor(RGB(121,121,255));   
	//	HBRUSH b=CreateSolidBrush(RGB(118,238,198));
	//	return b;
	//}

	if(nCtlColor==CTLCOLOR_EDIT)   // hu ���ı༭��
	{
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(152,251,152));
		HBRUSH b=CreateSolidBrush(RGB(152,251,152));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_STATIC)  // hu ���ľ�̬�ı�
	{
		//pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(84,255,159));
		HBRUSH b=CreateSolidBrush(RGB(84,255,159));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_DLG)   // hu ���ĶԻ��򱳾�ɫ
	{
		//pDC->SetTextColor(RGB(0,0,0));
		//pDC->SetBkColor(RGB(166,254,1));
		HBRUSH b=CreateSolidBrush(RGB(84,255,159));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_SCROLLBAR)  // hu ������
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(233,233,220));
		HBRUSH b=CreateSolidBrush(RGB(233,233,220));
		return b;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CPCIDriverTestDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_MOUSEMOVE)  // hu ��ʾ������ʾ
	{
		if (pMsg->hwnd == GetDlgItem(IDC_SHOW_BIN)->m_hWnd)
		{
			CString tmp;
			GetDlgItem(IDC_SHOW_BIN)->GetWindowText(tmp);
			if (!dmaSource0.Deliver_FP.Open(tmp,CFile::typeBinary|CFile::modeRead))
			{
				Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_BIN),_T("д��������ļ�·��"));//Ϊ�˿ؼ����tip
				Bubble_Tip.RelayEvent(pMsg);
			}
			else
			{
				int dataLen = dmaSource0.Deliver_FP.GetLength();
				dmaSource0.Deliver_FP.Close();
				tmp.Format(L"�ļ���СΪ%dBytes",dataLen);
				Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_BIN),tmp);//Ϊ�˿ؼ����tip
				Bubble_Tip.RelayEvent(pMsg);
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SHOW_TIME)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_TIME),_T("��ʾ�ļ�д��ײ�FPGA����ʱ��"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SELECT_BIN)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SELECT_BIN),_T("ѡ�������(.bin)�ļ���FPGA(.bit)�ļ�"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SEND_DATA)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SEND_DATA),_T("��ײ�FPGAд�������(.bin)�ļ�"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_FPGA_INI)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_FPGA_INI),_T("��FPGA(.bit)�ļ���ʼ���ײ�FPGA"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SHOW)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW),_T("��ʾϵͳ״̬"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDOK)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDOK),_T("�رյײ�Ӳ���������˳����"));//Ϊ�˿ؼ����tip
			Bubble_Tip.RelayEvent(pMsg);
		}
	}

	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)   // hu ����ENTER  
		return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)   // hu ����ESC
		return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CPCIDriverTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	static int timer_count = 0;

	switch(nIDEvent)
	{
	case 1:
		{
			Bar_FPGA.Progress_FPGA.SetPos((timer_count+1)*0.5*100/15);
			if ((timer_count+1)*0.5 == 13)
			{
				timer_count = 0;
				KillTimer(nIDEvent);
				break;
			}
			timer_count++;
			break;
		}
	default:
		{
			MainDlgFlash->OnTimer(nIDEvent);
			break;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CPCIDriverTestDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your message handler code here and/or call default
	// hu �Ի水ť
	//if(nIDCtl==IDC_SELECT_BIN) //hu ֻ�ı�һ����ť
	
	CDC dc;
	dc.Attach(lpDrawItemStruct ->hDC);
	RECT rect;
	rect= lpDrawItemStruct ->rcItem;

	dc.Draw3dRect(&rect,RGB(255,255,255),RGB(205,155,29));
	dc.FillSolidRect(&rect,RGB(255,236,139));
	UINT state=lpDrawItemStruct->itemState;

	if((state & ODS_SELECTED))
	{
		dc.DrawEdge(&rect,EDGE_SUNKEN,BF_RECT);

	}
	else
	{
		dc.DrawEdge(&rect,EDGE_RAISED,BF_RECT);
	}

	dc.SetBkColor(RGB(255,236,139));
	//dc.SetBkColor(GetSysColor(COLOR_BTNFACE));
	dc.SetTextColor(RGB(0,0,0));


	TCHAR buffer[MAX_PATH];
	ZeroMemory(buffer,MAX_PATH );
	::GetWindowText(lpDrawItemStruct->hwndItem,buffer,MAX_PATH);
	dc.DrawText(buffer,&rect,DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	dc.Detach();

	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

//void CPCIDriverTestDlg::OnClose()
//{
//	// TODO: Add your message handler code here and/or call default
//	CDialog::OnClose();
//}
