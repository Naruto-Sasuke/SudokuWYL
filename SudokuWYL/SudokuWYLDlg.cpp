
// SudokuWYLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SudokuWYL.h"
#include "SudokuWYLDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <time.h>
#include "sudoku.h"

#define CELLCOLOR			RGB(205, 250, 240)  //格子背景颜色
#define STROKECOLOR			RGB(220,220,220)    //格子边框颜色
#define SELECTEDNUMCOLOR	RGB(255, 51, 0)		//选中数字的颜色
#define MYNUMCOLOR			RGB(118, 77, 57)	//普通数字的颜色
#define GONGCOLOR			RGB(100, 210, 130)  //宫的颜色
#define SPSTROKECOLOR		RGB(200, 220, 120)	//选中的边框的颜色



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

	DDX_Control(pDX, IDC_COMBO1, m_cbBox);
}

BEGIN_MESSAGE_MAP(CSudokuWYLDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSudokuWYLDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSudokuWYLDlg::OnBnClickedButton2)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_BUTTON3, &CSudokuWYLDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSudokuWYLDlg::cbn_SelectedChange)
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
	canSelected = true;
	sudokuSize = 9;  //初始化为9x9


	for (int i = 0; i < 9; i++){
		for (int j = 0; j < 9; j++)
		{
			keyBoardInput[i][j] = 0;
			m_Sudoku9.setSimpleOneCheckBoard(i, j, 0);
		}
	}

#pragma region sixteen                   
	for (int i = 0; i < 16; i++){
		for (int j = 0; j < 16; j++)
		{
			p_Sudoku16->setSimpleOneCheckBoard(i, j, 0);
			keyBoardInput16[i][j] = 0;
		}
	}


#pragma endregion



	//初始化复选框
	m_cbBox.AddString(_T("9x9"));  //1
	m_cbBox.AddString(_T("16x16"));  // 0
	m_cbBox.SetCurSel(1);




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


//复选框
void CSudokuWYLDlg::cbn_SelectedChange()
{
	// TODO:  在此添加控件通知处理程序代码
	int index = m_cbBox.GetCurSel();
	sudokuSize = (index == 1) ? 9 : 16;
	if (sudokuSize == 9)
	{
		for (int i = 0; i < 9; i++){
			for (int j = 0; j < 9; j++)
			{
				keyBoardInput[i][j] = 0;
				m_Sudoku9.setSimpleOneCheckBoard(i, j, 0);
			}
		}
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);   //9x9时，交叉变换可用

	}
	else if (sudokuSize == 16)
	{

		for (int i = 0; i < 16; i++){
			for (int j = 0; j < 16; j++)
			{
				p_Sudoku16->setSimpleOneCheckBoard(i, j, 0);
				keyBoardInput16[i][j] = 0;
			}
		}
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);   //16x16时，交叉变换不可用
	}
                 
	
	canSelected = true;
	CDC *pDC = GetDC();
	DrawSudoku(pDC);
	Invalidate();


}


//初盘生成
void CSudokuWYLDlg::OnBnClickedButton2()
{
	CString timeStr;
	clock_t tStart, tEnd;
	if (sudokuSize == 9)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				m_Sudoku9.setSimpleOneCheckBoard(i, j, 0);
				keyBoardInput[i][j] = 0;
			}
		}
	}

	//16x16
	else{
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				p_Sudoku16->setSimpleOneCheckBoard(i, j, 0);
				keyBoardInput16[i][j] = 0;
			}
		}
	}

	canSelected = false;
	int **tmpCheckBorad = (int**)malloc(9 * sizeof(int*));
	for (int i = 0; i < 9; i++)
	{
		tmpCheckBorad[i] = (int*)malloc(9 * sizeof(int));
	}
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3))   //交叉变换,  暂时难度设为3，即去掉60个。
	{															//交叉变换不支持16x16
		tStart = clock();
		CString numStr;
	    GetDlgItem(IDC_EDIT1)->GetWindowTextW(numStr);
		m_Sudoku9.halfRandomGen(_ttoi(numStr), tmpCheckBorad);
		tEnd = clock();
		timeStr.Format(_T("交叉变换生成初盘，用时:%.6f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		m_Sudoku9.setDirectCheckBoard(tmpCheckBorad);
		m_Sudoku9.outPutCheckBoard("../in.txt",false);   //将交叉变换生成的棋盘输出到in.txt中
		p_SudokuDlx9->setCheckBoard();        //dlx对象读取棋盘
	}
	else   //随机生成
	{
		if (sudokuSize == 9)
		{
			tStart = clock();
			m_Sudoku9.genCheckBoard();      //随机生成时，当生成正确的棋盘时，in.txt已经得到了该棋盘。
			
			tEnd = clock();
			timeStr.Format(_T("随机生成初盘，用时:%.6f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
			GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
			m_Sudoku9.setCheckBoard();            //将in.txt读入到checkboard中
			p_SudokuDlx9->setCheckBoard();        //dlx对象读取棋盘
		}
		else
		{
			tStart = clock();
			p_Sudoku16->genCheckBoard();
			tEnd = clock();
			timeStr.Format(_T("随机生成初盘，用时:%.6f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
			GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
			p_Sudoku16->setCheckBoard();			 //将in.txt读入到checkboard中
			p_SudokuDlx16->setCheckBoard();     //dlx对象读取棋盘
		}

	}
	CDC *pDC = GetDC();
	DrawSudoku(pDC);             //只由针对回溯的棋盘画法即可。
}



void CSudokuWYLDlg::calu()
{
	CString timeStr;
	clock_t tStart = clock();
	bool ret;
	/*生成的方法有3种，第一种随机生成，这种当genCheckBoard时，就已经生成了对应的in.txt,
	*第二种交叉变换，需要单独outputCheckBoard，将checkboard数独输出到in.txt
	*第三种人工设置，由于每次设置均是直接对内部的checkboard数独进行更改，因此在求解棋盘时，加上下面的outPutCheckBoard
	*就是为了应付这种情况
	*/
	  
	if (sudokuSize == 9)
	{
		m_Sudoku9.outPutCheckBoard("../in.txt", false);   //应付第三种生成方式
		for (int i = 0; i < SUDOKU_SIZE_9; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_9;j++)
			{
				if (m_Sudoku9.getValueFromCheckBoard(i, j) != 0)           
				{
					keyBoardInput[i][j] = 1;
				}		
			}
		}
		ret = m_Sudoku9.solve();
	}
	else
	{
		p_Sudoku16->outPutCheckBoard("../in.txt", false);   //应付第三种生成方式
		for (int i = 0; i < SUDOKU_SIZE_16; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_16; j++)
			{
				if (p_Sudoku16->getValueFromCheckBoard(i, j) != 0)
				{
					keyBoardInput16[i][j] = 1;
				}
			}
		}
		ret = p_Sudoku16->solve();	
	}

	clock_t tEnd = clock();
	timeStr.Format(_T("回溯法求解成功！用时:%.6f 秒"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
	if (ret)
	{
		canSelected = false;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
		if (sudokuSize == 9)
			m_Sudoku9.outPutCheckBoard();   //调用回溯法类的solve是为了输出到out.txt
		else
			p_Sudoku16->outPutCheckBoard();  //调用回溯法类的solve是为了输出到out.txt
		
	}
	else
		MessageBox(_T("此棋盘无解 [=_=]！"));

}
void CSudokuWYLDlg::caluDlx()
{
	CString timeStr;
	clock_t tStart1,tEnd1,tEnd2;
	bool ret;
	/*生成的方法有3种，第一种随机生成，这种当genCheckBoard时，就已经生成了对应的in.txt,
	*第二种交叉变换，需要单独outputCheckBoard，将checkboard数独输出到in.txt
	*第三种人工设置，由于每次设置均是直接对内部的checkboard数独进行更改，因此在求解棋盘时，加上下面的outPutCheckBoard
	*就是为了应付这种情况
	*/

	if (sudokuSize == 9)
	{
		m_Sudoku9.outPutCheckBoard("../in.txt", false);   //应付第三种生成方式
		for (int i = 0; i < SUDOKU_SIZE_9; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_9; j++)
			{
				if (m_Sudoku9.getValueFromCheckBoard(i, j) != 0)
				{
					keyBoardInput[i][j] = 1;
				}
			}
		}
		tStart1 = clock();
		m_Sudoku9.solve();   //调用回溯法类的solve是为了画图
	    tEnd1 = clock();
		p_SudokuDlx9->setCheckBoard();        //dlx对象读取棋盘
		ret = p_SudokuDlx9->solve();       //用dlx类进行solve
		p_SudokuDlx9->outPutCheckBoard("../outDlx9.txt", false);   //测试dlx生成的是否相同
	}
	else
	{
		p_Sudoku16->outPutCheckBoard("../in.txt", false);   //应付第三种生成方式
		for (int i = 0; i < SUDOKU_SIZE_16; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_16; j++)
			{
				if (p_Sudoku16->getValueFromCheckBoard(i, j) != 0)
				{
					keyBoardInput16[i][j] = 1;
				}
			}
		}
		tStart1 = clock();
		p_Sudoku16->solve();   //调用回溯法类的solve是为了画图
		tEnd1 = clock();
		p_SudokuDlx16->setCheckBoard();        //dlx对象读取棋盘
		ret = p_SudokuDlx16->solve();    //用dlx类进行solve
		p_SudokuDlx16->outPutCheckBoard("../outDlx16.txt", false);   //测试dlx生成的是否相同
	}

    tEnd2 = clock();         //tEnd2-tEnd1是dlx方法
	timeStr.Format(_T("DLX求解成功！用时:%.6f 秒\n若用回溯法求解！用时:%.6f 秒"), (double)(tEnd2 - tEnd1) / CLOCKS_PER_SEC,(double)(tEnd1 - tStart1) / CLOCKS_PER_SEC);
	if (ret)
	{
		canSelected = false;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
		if (sudokuSize == 9)
			m_Sudoku9.outPutCheckBoard();    //调用回溯法类的solve是为了输出到out.txt
		else
			p_Sudoku16->outPutCheckBoard();     //调用回溯法类的solve是为了输出到out.txt

	}
	else
		MessageBox(_T("此棋盘无解 [=_=]！"));
}
//求解棋盘
void CSudokuWYLDlg::OnBnClickedButton1()
{

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))   //DLX被勾选
	{
		caluDlx();
	}
	else  if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))     //回溯法被勾选
	{
		calu();
	}
}

void CSudokuWYLDlg::DrawSudoku(CDC* pDC)
{

	//9x9
	if (sudokuSize == 9){
		
		for (int i = 0; i < SUDOKU_SIZE_9; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_9; j++)
			{
				int tmp = m_Sudoku9.getValueFromCheckBoard(i, j);
				CRect suRect(LEFT + SIZE*j, TOP + SIZE*i, LEFT + SIZE*(j + 1), TOP + SIZE*(i + 1));
				DrawRect(pDC, suRect, CELLCOLOR); // 填充小格。			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numStr;
				if (tmp)                               //去除0，使其不显示
					numStr.Format(_T("%d"), tmp);
				else numStr = _T("");
				DrawNumbers(pDC, i, j, numStr, MYNUMCOLOR, CELLCOLOR);

			}

		}
		for (int i = 0; i < SUDOKU_SIZE_9; i++)   //将初盘的有数字的位置以特殊的颜色显示
		{
			for (int j = 0; j < SUDOKU_SIZE_9; j++)
			{
				if (keyBoardInput[i][j] == 1){
					CString selectedNumStr;
					selectedNumStr.Format(_T("%d"), m_Sudoku9.getValueFromCheckBoard(i, j));
					DrawNumbers(pDC, i, j, selectedNumStr, SELECTEDNUMCOLOR, CELLCOLOR);
				}
			}
		}
		//画宫线
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				CRect gongRect(LEFT + SIZE*i * 3, TOP + SIZE*j * 3, LEFT + SIZE * 3 * (i + 1), TOP + SIZE * 3 * (j + 1));
				SpDrawStorke(pDC, gongRect, GONGCOLOR, 4);
			}
		}

		
	}
	//16x16
#pragma region sixteen
	else if (sudokuSize == 16){
		for (int i = 0; i < SUDOKU_SIZE_16; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_16; j++)
			{
				int tmp = p_Sudoku16->getValueFromCheckBoard(i, j);
				CRect suRect(LEFT + SIZE*j, TOP + SIZE*i, LEFT + SIZE*(j + 1), TOP + SIZE*(i + 1));
				DrawRect(pDC, suRect, CELLCOLOR); // 填充小格。			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numStr;
				if (tmp)                               //去除0，使其不显示
					numStr.Format(_T("%d"), tmp);
				else numStr = _T("");
				DrawNumbers(pDC, i, j, numStr, MYNUMCOLOR, CELLCOLOR);

			}

		}
		for (int i = 0; i < SUDOKU_SIZE_16; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_16; j++)
			{
				if (keyBoardInput16[i][j] == 1){
					CString selectedNumStr;
					selectedNumStr.Format(_T("%d"), p_Sudoku16->getValueFromCheckBoard(i, j));
					DrawNumbers(pDC, i, j, selectedNumStr, SELECTEDNUMCOLOR, CELLCOLOR);
				}
			}
		}

		//画宫线
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				CRect gongRect(LEFT + SIZE*i * 4, TOP + SIZE*j * 4, LEFT + SIZE * 4 * (i + 1), TOP + SIZE * 4 * (j + 1));
				SpDrawStorke(pDC, gongRect, GONGCOLOR, 4);
			}
		}
	}
#pragma endregion


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
void CSudokuWYLDlg::DrawNumbers(CDC*pDC, int i, int j, CString numStr, COLORREF textColor, COLORREF bkColor)
{
	CFont font;
	font.CreateFont(
		20,                        // nHeight
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
	int num = _ttoi(numStr);      //CString转int
	if (num > 0){				  
		if (sudokuSize == 9)
		{
			pDC->TextOut(LEFT + SIZE*j + SIZE / 3, TOP + SIZE*i + SIZE / 5, numStr);
		}
		else //16x16的棋盘的数字位置
		{

			if (num >= 10) //2位数
				pDC->TextOut(LEFT + SIZE*j + SIZE / 4, TOP + SIZE*i + SIZE / 5, numStr);
			else
				pDC->TextOut(LEFT + SIZE*j + SIZE / 3, TOP + SIZE*i + SIZE / 5, numStr);
		}
	}
	
}

void CSudokuWYLDlg::SpDrawStorke(CDC *pDC, CRect rect, COLORREF color, int stroke)
{
	CPen tmpPen(PS_SOLID, stroke, color);
	CPen *pOldPen;
	pOldPen = pDC->SelectObject(&tmpPen);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left + rect.Width(), rect.top);
	pDC->LineTo(rect.left + rect.Width(), rect.top + rect.Height());
	pDC->LineTo(rect.left, rect.top + rect.Height());
	pDC->LineTo(rect.left, rect.top);
	pDC->SelectObject(pOldPen);

}



//注意数组的i,j和point.x，point.y的区别。
//.x是距离左边，而i是从上往下数第几行。


void CSudokuWYLDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDC *pDC = GetDC();
	GetCursorPos(&point);
	ScreenToClient(&point);
	double docx = point.x;
	double docy = point.y;
	mouseY = (docx - LEFT) / SIZE;     //这里的mouseY是数组的j....
	mouseX = (docy - TOP) / SIZE;
	//9x9
	if (sudokuSize == 9){
		if (canSelected == true)
		{
			if (docx < LEFT + SIZE * 9 && docx > LEFT && docy > TOP && docy < TOP + SIZE * 9)
			{
				CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
					LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
				DrawRect(pDC, selectedRect, CELLCOLOR);				//将选中的各自以原有颜色填充
				SpDrawStorke(pDC, selectedRect, SPSTROKECOLOR, 2);       //双击时修改格子边框为 "特殊颜色"。
				m_Sudoku9.setSimpleOneCheckBoard(mouseX, mouseY, 0);
				keyBoardInput[mouseX][mouseY] = 1;						//同时设置选中

			}

			//为了防止数字更改引起宫线改变，需要重绘宫线
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					CRect gongRect(LEFT + SIZE*i * 3, TOP + SIZE*j * 3, LEFT + SIZE * 3 * (i + 1), TOP + SIZE * 3 * (j + 1));
					SpDrawStorke(pDC, gongRect, GONGCOLOR, 4);
				}
			}
		}
	}
#pragma region sixteen
	//16x16
	if (sudokuSize == 16){
		if (canSelected == true)
		{
			if (docx < LEFT + SIZE * 16 && docx > LEFT && docy > TOP && docy < TOP + SIZE * 16)
			{
				CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
					LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));

				DrawRect(pDC, selectedRect, CELLCOLOR);				//将选中的各自以原有颜色填充
				SpDrawStorke(pDC, selectedRect, SPSTROKECOLOR, 2);       //双击时修改格子边框为 "特殊颜色"。
				p_Sudoku16->setSimpleOneCheckBoard(mouseX, mouseY, 0);	
				keyBoardInput16[mouseX][mouseY] = 1;

			}

			//为了防止数字更改引起宫线改变，需要重绘宫线
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					CRect gongRect(LEFT + SIZE*i * 4, TOP + SIZE*j * 4, LEFT + SIZE * 4 * (i + 1), TOP + SIZE * 4 * (j + 1));
					SpDrawStorke(pDC, gongRect, GONGCOLOR, 4);
				}
			}
		}
	}
#pragma endregion


	CDialogEx::OnLButtonDblClk(nFlags, point);
}



BOOL CSudokuWYLDlg::PreTranslateMessage(MSG* pMsg)
{
//为了防止通过键盘使得combox获得焦点，从而造成9x9和16x16的界面转换
//推荐每次进行界面转换后，重置一下棋盘，使得combox失去焦点
	CDC *pDC = GetDC();
	//9x9
	if (sudokuSize == 9)
	{
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
					if (!m_Sudoku9.isValid(mouseX, mouseY, inValue))
					{
						MessageBox(_T("输入不合法"));
						CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
							LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
						SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //还原选中框颜色
						keyBoardInput[mouseX][mouseY] = 0;							//不符合要求时将相应位置keyBoardInput置为0
					}
					else
					{
						m_Sudoku9.setSimpleOneCheckBoard(mouseX, mouseY, inValue);
						DrawNumbers(pDC, mouseX, mouseY, numStr, SELECTEDNUMCOLOR, CELLCOLOR);
						keyBoardInput[mouseX][mouseY] = 1;							
					}

				}
				else  //其他输入默认为空
				{
					CString numDefaultStr;
					numDefaultStr.Format(_T(""));
					DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, SELECTEDNUMCOLOR, CELLCOLOR);
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //还原选中框颜色
					keyBoardInput[mouseX][mouseY] = 0;					//不符合要求时将相应位置keyBoardInput置为0
				}


			}
		}
	}
	//16x16
#pragma region sixteen
	else
	{
		if (0 <= mouseX && mouseX <= 15 && mouseY >= 0 && mouseY <= 15)
		{
			if (pMsg->message == WM_KEYDOWN)
			{
				int keyValue = pMsg->wParam;
				if ((keyValue >= 65 && keyValue <= 71) ||( keyValue>=49 && keyValue <=57)) //从A到F，为10到16
				{
					int inValue = 0;
					if (keyValue >= 48 && keyValue <= 57)
						inValue = keyValue - 48;
					else
						inValue = keyValue - 55;
					CString numStr;
					numStr.Format(_T("%d"), inValue);

					//判断输入是否符合要求
					if (!p_Sudoku16->isValid(mouseX, mouseY, inValue))
					{
						MessageBox(_T("输入不合法"));
						CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
							LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
						SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //还原选中框颜色
						keyBoardInput[mouseX][mouseY] = 0;							//不符合要求时将相应位置keyBoardInput置为0
					}
					else
					{
						p_Sudoku16->setSimpleOneCheckBoard(mouseX, mouseY, inValue);
						DrawNumbers(pDC, mouseX, mouseY, numStr, SELECTEDNUMCOLOR, CELLCOLOR);
						keyBoardInput16[mouseX][mouseY] = 1;			//当输入数字后，才能设置为1				
					}

				}
				else  //其他输入默认为空
				{
					CString numDefaultStr;
					numDefaultStr.Format(_T(""));
					DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, SELECTEDNUMCOLOR, CELLCOLOR);
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //还原选中框颜色
					keyBoardInput[mouseX][mouseY] = 0;							//不符合要求时将相应位置keyBoardInput置为0
				}
			}
		}
	}
#pragma endregion

	return CDialogEx::PreTranslateMessage(pMsg);
}

//棋盘重置
void CSudokuWYLDlg::OnBnClickedButton3()
{
	mouseX = -1;
	mouseY = -1;
	canSelected = true;
	if (sudokuSize == 9)
	{
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				m_Sudoku9.setSimpleOneCheckBoard(i, j, 0);
				keyBoardInput[i][j] = 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				p_Sudoku16->setSimpleOneCheckBoard(i, j, 0);
				keyBoardInput16[i][j] = 0;
			}
		}
	}


	Invalidate();

}



//////////////////////////////////////////////////////////////////////////
//用GetDC()，是相对于document区域的左上角。
//如果用GetWindowDC()，导致都是窗口的左上角为绘画原点的。
/************************************************************************/
/* 在有DLX后，时间相减得到一个数组，通过判断bool数组的true多还是false多，从而
判断回溯法和DLX方法的好坏。*/
/************************************************************************/
/************************************************************************/
/*								使用说明                                 */
/*1.在通过复选框更换棋盘大小时，最好要按一下“棋盘重置”，使得复选框失去焦点，否则
输入数字“1,9”时会使得棋盘界面更改。
  2.只能通过棋盘上方的数字键“1~9”和“a~g（仅在16x16棋盘有效，分别代表10~16）”
/************************************************************************/
