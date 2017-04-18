// PCIDriverTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PCIDriverTest.h"
#include "PCIDriverTestDlg.h"
#include "DialogFPGA.h"
#include "pci_driver_lib_head.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// CPCIDriverTestDlg 对话框




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
	ON_MESSAGE(WM_CALLBACKMESSAGE_0, OnCALLBACKMESSAGE_0)    // hu 添加消息映射处理
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


// CPCIDriverTestDlg 消息处理程序

BOOL CPCIDriverTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	// hu 窗口渐变
    MainDlgFlash = new CDlgFlash(this,9,10,11,12);  // hu 定时器ID为9,10,11,12
	MainDlgFlash->SetFlashCreateSpeed(1,5);
	MainDlgFlash->SetFlashDestroySpeed(5,3);
	MainDlgFlash->SetFlashCR2NCRSpeed(12);
	MainDlgFlash->StartFlashCR2NCRDlg();

	// hu 创建tab子界面
	Bar_wr.InsertItem(0,_T("FPGA"));  
	Bar_wr.InsertItem(1,_T("DSP")); 
	Bar_FPGA.Create(IDD_DIALOG_FPGA,GetDlgItem(IDC_BAR_WR));
	Bar_DSP.Create(IDD_DIALOG_DSP,GetDlgItem(IDC_BAR_WR));

	// hu 获得客户区大小  
	CRect rs;  
	Bar_wr.GetClientRect(&rs);

	// hu 调整子对话框在父窗口中的位置，根据实际修改  
	rs.top+=25;  
	rs.bottom-=1;  
	rs.left+=1;  
	rs.right-=1;

	// hu 设置子对话框尺寸并移动到指定位置  
	Bar_FPGA.MoveWindow(&rs);  
	Bar_DSP.MoveWindow(&rs);

	// hu 分别设置隐藏和显示  
	Bar_FPGA.ShowWindow(true);  
	Bar_DSP.ShowWindow(false);  

	// hu 设置默认的选项卡  
	Bar_wr.SetCurSel(0);

	// hu FPGA地址框设置
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_FPGA_STARTADDR))->SetLimitText(7); // hu 字数限制为7个字节
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_FPGA_ENDADDR))->SetLimitText(7);
	((CEdit*)Bar_FPGA.GetDlgItem(IDC_REPEAT_FPGADATA))->SetLimitText(2); // hu 最多重复写入99次

	Bar_FPGA.GetDlgItem(IDC_FPGA_STARTADDR)->SetWindowText(_T("0x")); // hu 在enchange消息中初始化
	Bar_FPGA.GetDlgItem(IDC_DATA_FPGA)->SetWindowText(_T("1"));
	Bar_FPGA.GetDlgItem(IDC_REPEAT_FPGADATA)->SetWindowText(_T("1"));
	//Bar_FPGA.GetDlgItem(IDC_FPGA_ENDADDR)->SetWindowText(_T("0x"));
	/*FileBinColor.CreateSolidBrush(RGB(255,255,0)); // hu 改变编辑框背景颜色*/

	// hu DSP地址框设置
	((CEdit*)Bar_DSP.GetDlgItem(IDC_DSP_OFFSADDR))->SetLimitText(10); // hu 字数限制为7个字节

	Bar_DSP.GetDlgItem(IDC_DSP_OFFSADDR)->SetWindowText(_T("0x0"));
	Bar_DSP.GetDlgItem(IDC_DATAW_DSP)->SetWindowText(_T("1"));

	// hu 设置鼠标悬停提示
	EnableToolTips(TRUE);
	Bubble_Tip.Create(this);
	Bubble_Tip.Activate(TRUE);
	Bubble_Tip.SetMaxTipWidth(600);         // hu 设置气泡提示最大宽度，设置后可实现多行显示
	Bar_FPGA.Bubble_Tip_FPGA.Create(this);
	Bar_FPGA.Bubble_Tip_FPGA.Activate(TRUE);
	Bar_FPGA.Bubble_Tip_FPGA.SetMaxTipWidth(600);  
	Bar_DSP.Bubble_Tip_DSP.Create(this);
	Bar_DSP.Bubble_Tip_DSP.Activate(TRUE);
	Bar_DSP.Bubble_Tip_DSP.SetMaxTipWidth(600); 
	
	// hu 初始化单选框
	((CButton *)Bar_FPGA.GetDlgItem(IDC_FIX_SELECT))->SetCheck(TRUE);
	((CButton *)Bar_DSP.GetDlgItem(IDC_DDR_SELECT))->SetCheck(TRUE);

	// hu 初始化进度条
	Bar_FPGA.Progress_FPGA.SetRange(0,100);  // hu 设置进度条的范围
	Bar_FPGA.Progress_FPGA.SetStep(10);       // hu 设置进度条的每一步的增量
	Bar_FPGA.Progress_FPGA.SetPos(0);        // hu 设置进度条的当前位置

	// hu 初始化PCI6455
#ifdef DEBUG_Hu
	driver0.status = 4;
#else
	driver0.status = Initialize_DSPDriver(0);
#endif
	switch(driver0.status)
	{
	case 0:GetDlgItem(IDC_SHOW)->SetWindowText(_T("初始化PCI设备成功！"));break;
	case 1:GetDlgItem(IDC_SHOW)->SetWindowText(_T("未扫描到PCI设备！"));break;
	case 2:GetDlgItem(IDC_SHOW)->SetWindowText(_T("获取设备驱动操作句柄失败！"));break;
	case 3:GetDlgItem(IDC_SHOW)->SetWindowText(_T("初始化设备驱动库失败！"));break;
	default:GetDlgItem(IDC_SHOW)->SetWindowText(_T("不是驱动初始化函数的默认返回值！"));break;
	}

	// hu 注册消息函数
	m_hWnd = AfxGetMainWnd()->m_hWnd;
	// hu m_hWnd = this->m_hWnd;
	DllFun(0,m_hWnd) ;//调用DLL函数，窗口句柄传递到DLL。

	// hu 申请物理空间
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
			temp.Format(_T("%s\r\n申请%dMB连续物理空间成功！"),temp,PhySpaceSize/1024/1024);
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 1:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n所申请的物理空间数量超过上限10！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 2:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n申请物理空间失败！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 3:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\nCPU缓存与物理空间同步失败！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	default:
		{
			CString temp;
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n不是申请物理空间函数的默认返回值！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPCIDriverTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
//HCURSOR CPCIDriverTestDlg::OnQueryDragIcon()
//{
//	return static_cast<HCURSOR>(m_hIcon);
//}


void CPCIDriverTestDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CString temp;

	// hu 释放物理空间
#ifdef DEBUG_Hu
	driver0.status = 3;
#else
	driver0.status = FreePA(0,driver0.EDMAHandle_Device);
#endif
	switch(driver0.status)
	{
	case 0:GetDlgItem(IDC_SHOW)->SetWindowText(_T("释放连续物理空间成功！"));break;
	case 1:GetDlgItem(IDC_SHOW)->SetWindowText(_T("I/O缓存与物理空间同步失败！"));break;
	case 2:GetDlgItem(IDC_SHOW)->SetWindowText(_T("释放物理空间失败！"));break;
	default:GetDlgItem(IDC_SHOW)->SetWindowText(_T("不是释放物理空间函数的默认返回值！"));break;
	}

	// hu 关闭设备
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
			temp += "\r\n关闭PCI设备成功！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 1:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n关闭PCI设备失败！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 2:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n卸载设备驱动库失败！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	case 3:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n关闭PCI驱动与卸载设备驱动库失败！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	default:
		{
			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\n不是关闭驱动函数的默认返回值！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
			break;
		}
	}

	MainDlgFlash->SetFlashNCR2CRSpeed(12);
	MainDlgFlash->StartFlashNCR2CRDlg();
	MainDlgFlash->StartFlashDestroyDlg(); //开始动画关闭对话框
	//OnOK();
}

void CPCIDriverTestDlg::OnBnClickedSelectBin()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("文件(*.bin;*.bit)|*.bin;*.bit||"));

	FileDlg.m_ofn.lpstrInitialDir = _T("files");//改初始化目录

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
		AfxMessageBox(_T("请输入正确的文件路径名!"));
		return;
	}

	GetDlgItem(IDC_SHOW_TIME)->SetWindowText(_T(""));

#ifdef DEBUG_Hu
	dmaSource0.Deliver_FP.Close();
#else
	dmaSource0.Deliver_Len = dmaSource0.Deliver_FP.Read(driver0.EDMAWinDriver_Device,PhySpaceSize);

	if (dmaSource0.Deliver_Len < PhySpaceSize)
	{
		WriteToDSP(0,5,0x10,0x1);// hu 传输结束标志位
	}

	WriteToDSP(0,5,0,driver0.EDMAPhysicalAddress_Device);
	WriteToDSP(0,5,4,dmaSource0.Deliver_Len);

	EnableInterruptFromDSP(0);
	dmaSource0.Deliver_Time = GetTickCount();// hu 开始时间
	SendIntToDSP(0); // hu 向DSP发送中断
#endif
	GetDlgItem(IDC_SHOW)->SetWindowText(_T("开始EDMA传输..."));
}

// hu 响应dll上传的PostMessage()函数
LRESULT CPCIDriverTestDlg::OnCALLBACKMESSAGE_0(WPARAM wParam, LPARAM lParam)//hu wParam为中断号
{
	CString temp;
	if(wParam == 0) 
	{
		DisEnableInterruptFromDSP(0);    // hu 屏蔽中断
		// hu ///////////添加0号中断处理程序///////////////	

		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu 开启中断   
	}
	else if(wParam == 1)
	{
		DisEnableInterruptFromDSP(0);    // hu 屏蔽中断
		// hu ///////////添加1号中断处理程序///////////////

		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu 开启中断
	}
	else if(wParam == 2)  
	{
		DisEnableInterruptFromDSP(0);    // hu 屏蔽中断
		// hu ///////////添加2号中断处理程序///////////////
		dmaSource0.Deliver_Time = GetTickCount() - dmaSource0.Deliver_Time;  // hu 所花时间

		ReadfromDSP(0,5,0x14,&dmaSource0.GPIO2FPGA_flag);
		switch(dmaSource0.GPIO2FPGA_flag)
		{
		case 0:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\n动态加载成功！";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 1:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\nFPGA初始化失败！";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 2:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\n未收到FPGA的INTB信号，动态加载失败！";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		case 3:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\n未收到FPGA的DONE信号，动态加载失败！";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		default:
			{
				GetDlgItem(IDC_SHOW)->GetWindowText(temp);
				temp += "\r\n未收到约定返回值，动态加载失败！";
				GetDlgItem(IDC_SHOW)->SetWindowText(temp);
				break;
			}
		}

		Bar_FPGA.Progress_FPGA.SetPos(100);

		temp.Format(_T("%.1f"),dmaSource0.Deliver_Time/1000.0);
		GetDlgItem(IDC_SHOW_TIME)->SetWindowText(temp);
		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu 开启中断
	}
	else if(wParam == 3)  
	{
		DisEnableInterruptFromDSP(0);    // hu 屏蔽中断
		// hu ///////////添加3号中断处理程序///////////////
		if (dmaSource0.Deliver_Len == PhySpaceSize)
		{
			dmaSource0.Deliver_Len = dmaSource0.Deliver_FP.Read(driver0.EDMAWinDriver_Device,PhySpaceSize);

			if (dmaSource0.Deliver_Len < PhySpaceSize)
			{
				WriteToDSP(0,5,0x10,0x1);// hu 传输结束标志位
			}

			WriteToDSP(0,5,0,driver0.EDMAPhysicalAddress_Device);
			WriteToDSP(0,5,4,dmaSource0.Deliver_Len);

			EnableInterruptFromDSP(0);
			SendIntToDSP(0); // hu 向DSP发送中断

			return 0;
		}
		else
		{
			dmaSource0.Deliver_Time = GetTickCount() - dmaSource0.Deliver_Time;  // hu 所花时间

			WriteToDSP(0,3,0xa0a00*4,0xf0f0);// hu 通知fpga传输完毕 0xa0a00为fpga读取数据地址

			dmaSource0.Deliver_FP.Close();

			temp.Format(_T("%.1f"),dmaSource0.Deliver_Time/1000.0);
			GetDlgItem(IDC_SHOW_TIME)->SetWindowText(temp);

			GetDlgItem(IDC_SHOW)->GetWindowText(temp);
			temp += "\r\nEDMA传输成功！";
			GetDlgItem(IDC_SHOW)->SetWindowText(temp);
		}
		// hu /////////////////////////////////////////////
		EnableInterruptFromDSP(0);       // hu 开启中断 
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
		AfxMessageBox(_T("请输入正确的文件路径名!"));
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
		AfxMessageBox(_T("加载文件不能大于11MB!"));
		return;
	}

	WriteToDSP(0,5,8,driver0.EDMAPhysicalAddress_Device);
	WriteToDSP(0,5,0xc,dmaSource0.Deliver_Len);

	EnableInterruptFromDSP(0);
	dmaSource0.Deliver_Time = GetTickCount();// hu 开始时间
	SendIntToDSP(0); // hu 向DSP发送中断
#endif
	GetDlgItem(IDC_SHOW)->SetWindowText(_T("开始动态加载..."));

	SetTimer(1,500,NULL);// hu 启动定时器1,定时时间是0.5秒
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
	CWnd* pWnd=GetDlgItem(IDC_SHOW_BIN);	// hu 获取文本控件窗口指针
	pWnd->Invalidate();          // hu 让pWnd所指向的窗口重画
}

HBRUSH CPCIDriverTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(nCtlColor==CTLCOLOR_EDIT && pWnd->GetDlgCtrlID()==IDC_SHOW_BIN)//注意此处的（pWnd->），否则没效果
	{
		CString path;
		CFileStatus temp;
		GetDlgItem(IDC_SHOW_BIN)->GetWindowText(path);
		if (dmaSource0.Deliver_FP.GetStatus(path,temp))
		{
			pDC->SetTextColor(RGB(0,0,0));   // hu 若文件存在，将字设为黑色
		}
		else
		{
			pDC->SetTextColor(RGB(255,0,0)); // hu 若文件不存在，将字设为红色
		}
		//pDC->SetBkColor(RGB(255,255,255));// hu 设置文本背景色
		//pDC->SetBkMode(TRANSPARENT);// hu 设置背景透明
		//hbr = (HBRUSH)FileBinColor;
	}
    
	//if(nCtlColor==CTLCOLOR_BTN)          // hu 更改按钮颜色，会出现字体不显示的问题
	//{
	//	//pDC->SetBkMode(TRANSPARENT);
	//	//pDC->SetTextColor(RGB(0,0,0));
	//	//pDC->SetBkColor(RGB(121,121,255));   
	//	HBRUSH b=CreateSolidBrush(RGB(118,238,198));
	//	return b;
	//}

	if(nCtlColor==CTLCOLOR_EDIT)   // hu 更改编辑框
	{
		//pDC->SetBkMode(TRANSPARENT);
		//pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(152,251,152));
		HBRUSH b=CreateSolidBrush(RGB(152,251,152));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_STATIC)  // hu 更改静态文本
	{
		//pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(84,255,159));
		HBRUSH b=CreateSolidBrush(RGB(84,255,159));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_DLG)   // hu 更改对话框背景色
	{
		//pDC->SetTextColor(RGB(0,0,0));
		//pDC->SetBkColor(RGB(166,254,1));
		HBRUSH b=CreateSolidBrush(RGB(84,255,159));
		return b;
	}
	else if(nCtlColor==CTLCOLOR_SCROLLBAR)  // hu 滚动条
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
	if(pMsg->message == WM_MOUSEMOVE)  // hu 显示气泡提示
	{
		if (pMsg->hwnd == GetDlgItem(IDC_SHOW_BIN)->m_hWnd)
		{
			CString tmp;
			GetDlgItem(IDC_SHOW_BIN)->GetWindowText(tmp);
			if (!dmaSource0.Deliver_FP.Open(tmp,CFile::typeBinary|CFile::modeRead))
			{
				Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_BIN),_T("写入二进制文件路径"));//为此控件添加tip
				Bubble_Tip.RelayEvent(pMsg);
			}
			else
			{
				int dataLen = dmaSource0.Deliver_FP.GetLength();
				dmaSource0.Deliver_FP.Close();
				tmp.Format(L"文件大小为%dBytes",dataLen);
				Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_BIN),tmp);//为此控件添加tip
				Bubble_Tip.RelayEvent(pMsg);
			}
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SHOW_TIME)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW_TIME),_T("显示文件写入底层FPGA所花时间"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SELECT_BIN)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SELECT_BIN),_T("选择二进制(.bin)文件或FPGA(.bit)文件"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SEND_DATA)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SEND_DATA),_T("向底层FPGA写入二进制(.bin)文件"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_FPGA_INI)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_FPGA_INI),_T("用FPGA(.bit)文件初始化底层FPGA"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_SHOW)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDC_SHOW),_T("显示系统状态"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
		else if (pMsg->hwnd == GetDlgItem(IDOK)->m_hWnd)
		{
			Bubble_Tip.AddTool(GetDlgItem(IDOK),_T("关闭底层硬件驱动并退出软件"));//为此控件添加tip
			Bubble_Tip.RelayEvent(pMsg);
		}
	}

	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)   // hu 屏蔽ENTER  
		return TRUE; 
	if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_ESCAPE)   // hu 屏蔽ESC
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
	// hu 自绘按钮
	//if(nIDCtl==IDC_SELECT_BIN) //hu 只改变一个按钮
	
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
