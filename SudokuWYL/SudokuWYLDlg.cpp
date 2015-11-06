
// SudokuWYLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SudokuWYL.h"
#include "SudokuWYLDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <time.h>
#include "sudoku.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSudokuWYLDlg 对话框


CSudokuWYLDlg::CSudokuWYLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSudokuWYLDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	
}

void CSudokuWYLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK1, DLXCheckBox);
	DDX_Control(pDX, IDC_CHECK2, backtrackCheckBox);
}

BEGIN_MESSAGE_MAP(CSudokuWYLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSudokuWYLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSudokuWYLDlg::OnBnClickedButton2)
ON_WM_LBUTTONDBLCLK()
ON_BN_CLICKED(IDC_BUTTON3, &CSudokuWYLDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CSudokuWYLDlg 消息处理程序

BOOL CSudokuWYLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

 
	mouseX = -1;
	mouseY = -1;   //初始化mouseX和mouseY
	hasFiguredOut = false;  //初始化为 "未解出" 棋盘
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			keyBoardInput[i][j] = 0;
	}
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSudokuWYLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
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
		CDialogEx::OnPaint();
	}
	
	CDC* pDc = GetDC();
	DrawSudoku(pDc);

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSudokuWYLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//初盘生成
void CSudokuWYLDlg::OnBnClickedButton2()           
{
	CString timeStr;
	clock_t tStart, tEnd;
	int **tmpCheckBorad = (int**)malloc(9 * sizeof(int*));
	for(int i = 0; i < 9; i++)  
	{  
		tmpCheckBorad[i] = (int*)malloc(9 * sizeof(int));
	} 
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK3 ) )   //交叉变换,  暂时难度设为3，即去掉60个。
	{
		tStart = clock();
		m_Sudoku.halfRandomGen(3,tmpCheckBorad);
		tEnd = clock();
		timeStr.Format(_T("交叉变换生成初盘，用时:%.7f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		m_Sudoku.setDirectCheckBoard(tmpCheckBorad);
	}
	else   //随机生成
	{
		 tStart = clock();
		 m_Sudoku.genCheckBoard(); 
		 tEnd = clock();
		 timeStr.Format(_T("随机生成初盘，用时:%.7f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
		 GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		 m_Sudoku.setCheckBoard();
	}
	
	
	CDC *pDC = GetDC();
	DrawSudoku(pDC);
}





void CSudokuWYLDlg::calu()
{
	CString timeStr;
	clock_t tStart = clock();
	bool ret = m_Sudoku.solve();
	clock_t tEnd = clock();
	timeStr.Format(_T("求解成功！用时:%.7f 秒"),(double)( tEnd - tStart)/CLOCKS_PER_SEC);
	if(ret)
	{
		hasFiguredOut = true;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
	}
	else
		MessageBox(_T("此棋盘无解 [=_=]！"));

}

//求解棋盘
void CSudokuWYLDlg::OnBnClickedButton1()
{
	
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK1 ) )   //DLX被勾选
	{

	}
	else  if( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK2 ))     //回溯法被勾选
	{
		calu();
	}
}

void CSudokuWYLDlg::DrawSudoku(CDC* pDC)
{	
		COLORREF textColor(RGB(118, 77, 57));
		COLORREF cellColor(RGB(205, 250, 240));
	
		for(int i=0;i<SUDOKU_SIZE_9;i++)
		{
			for(int j=0;j<SUDOKU_SIZE_9;j++)
			{
					int tmp = m_Sudoku.getValueFromCheckBoard(i,j);
					CRect suRect(LEFT+SIZE*j,TOP+SIZE*i,LEFT+SIZE*(j+1),TOP+SIZE*(i+1));
					DrawRect(pDC, suRect, RGB(205, 250, 240)); // 填充小格。
					if (keyBoardInput[i][j])
						SpDrawStorke(pDC, suRect, RGB(200, 220, 120), 2);      //特殊颜色				
					else
						SpDrawStorke(pDC,suRect,RGB(220,220,220),2);
					CString numStr;
					if (tmp)                               //去除0，使其不显示
						numStr.Format(_T("%d"),tmp);
					else numStr = _T("");
					DrawNumbers(pDC, i, j, numStr, textColor, cellColor);

			}
			
		}
		//画宫线
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				CRect gongRect(LEFT + SIZE*i * 3, TOP + SIZE*j * 3, LEFT + SIZE * 3 * (i + 1), TOP + SIZE * 3 * (j + 1));
				SpDrawStorke(pDC, gongRect, RGB(100, 210, 130), 4);
			}
		}
		
}

void CSudokuWYLDlg::DrawRect(CDC*pDC, CRect rect, COLORREF color)
{
	CBrush newBrush, *pOldBrush;
	newBrush.CreateSolidBrush(color);
	pOldBrush = pDC->SelectObject(&newBrush);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldBrush);
}

//填数字
void CSudokuWYLDlg::DrawNumbers(CDC*pDC, int i, int j,CString numStr,COLORREF textColor,COLORREF bkColor)
{
	CFont font;
	font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("SimHei"));             // lpszFacename
	CFont*def_font = pDC->SelectObject(&font);
	pDC->SetTextColor(textColor);
	pDC->SetBkColor(bkColor); //bkColor和brush颜色一致！
	pDC->TextOut(LEFT + SIZE*j + SIZE / 2 - 5, TOP + SIZE*i + SIZE / 2 - 9, numStr);
}

void CSudokuWYLDlg::SpDrawStorke(CDC *pDC, CRect rect, COLORREF color,int stroke)
{
	CPen tmpPen(PS_SOLID,stroke,color);
	CPen *pOldPen;
	pOldPen=pDC->SelectObject(&tmpPen);
	pDC->MoveTo(rect.left,rect.top);
	pDC->LineTo(rect.left+rect.Width(),rect.top);
	pDC->LineTo(rect.left+rect.Width(),rect.top+rect.Height());
	pDC->LineTo(rect.left,rect.top+rect.Height());
	pDC->LineTo(rect.left,rect.top);
	pDC->SelectObject(pOldPen);

}



//注意数组的i,j和point.x，point.y的区别。
//.x是距离左边，而i是从上往下数第几行。


void CSudokuWYLDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	
	CDC *pDC = GetDC();
	GetCursorPos(&point);               
	ScreenToClient(&point);
	double docx = point.x;           
	double docy = point.y;
	if (hasFiguredOut == false)  //未解出状态才能进行设置棋盘
	{
		if (docx < LEFT + SIZE * 9 && docx > LEFT && docy > TOP && docy < TOP + SIZE * 9)
		{
			mouseY = (docx - LEFT) / SIZE;     //这里的mouseY是数组的j....
			mouseX = (docy - TOP) / SIZE;
			CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
				LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));

			DrawRect(pDC, selectedRect, RGB(205, 250, 240));				
			SpDrawStorke(pDC, selectedRect, RGB(200, 220, 120), 2);       //双击时修改格子边框为 "特殊颜色"。
			keyBoardInput[mouseX][mouseY] = 1;							  //选中格子设置。

		}

		//为了防止数字更改引起宫线改变，需要重绘宫线
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				CRect gongRect(LEFT + SIZE*i * 3, TOP + SIZE*j * 3, LEFT + SIZE * 3 * (i + 1), TOP + SIZE * 3 * (j + 1));
				SpDrawStorke(pDC, gongRect, RGB(100, 210, 130), 4);
			}
		}
	}
	

	CDialogEx::OnLButtonDblClk(nFlags, point);
}



BOOL CSudokuWYLDlg::PreTranslateMessage(MSG* pMsg)
{

	CDC *pDC = GetDC();
	COLORREF textColor(RGB(248,147,29));     //键盘输入值的颜色
	COLORREF cellColor(RGB(205, 250, 240));

	if (0 <= mouseX && mouseX <= 8 && mouseY >= 0 && mouseY <= 8)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			int keyValue = pMsg->wParam;
			if (keyValue >= 49 && keyValue <= 57) //规定设置值为1~9
			{
				int inValue = keyValue - 48;
				CString numStr;
				numStr.Format(_T("%d"), inValue);

				//判断输入是否符合要求
				if (!m_Sudoku.isValid(mouseX,mouseY,inValue))
				{
					MessageBox(_T("输入不合法"));
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, RGB(220, 220, 220), 2);           //还原选中框颜色
				}
				else
				{
					m_Sudoku.setSimpleOneCheckBoard(mouseX, mouseY, inValue);
					DrawNumbers(pDC, mouseX, mouseY, numStr, textColor, cellColor);
				}

			}
			else  //其他输入默认为空
			{
				CString numDefaultStr;
				numDefaultStr.Format(_T(""));
				DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, textColor, cellColor);
				CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
					LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
				SpDrawStorke(pDC, selectedRect, RGB(220, 220, 220), 2);           //还原选中框颜色
			}


		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//棋盘重置
void CSudokuWYLDlg::OnBnClickedButton3()
{
	mouseX = -1;
	mouseY = -1;
	hasFiguredOut = false;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			m_Sudoku.setSimpleOneCheckBoard(i, j, 0);
			keyBoardInput[i][j] = 0;
		}
	}
	
	
	Invalidate();
}



//////////////////////////////////////////////////////////////////////////
//用GetDC()，是相对于document区域的左上角。
//如果用GetWindowDC()，导致都是窗口的左上角为绘画原点的。

