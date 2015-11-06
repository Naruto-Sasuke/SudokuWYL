
// SudokuWYLDlg.cpp : ʵ���ļ�
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


// CSudokuWYLDlg �Ի���


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


// CSudokuWYLDlg ��Ϣ�������

BOOL CSudokuWYLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

 
	mouseX = -1;
	mouseY = -1;   //��ʼ��mouseX��mouseY
	hasFiguredOut = false;  //��ʼ��Ϊ "δ���" ����
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
			keyBoardInput[i][j] = 0;
	}
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSudokuWYLDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
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
		CDialogEx::OnPaint();
	}
	
	CDC* pDc = GetDC();
	DrawSudoku(pDc);

}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSudokuWYLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//��������
void CSudokuWYLDlg::OnBnClickedButton2()           
{
	CString timeStr;
	clock_t tStart, tEnd;
	int **tmpCheckBorad = (int**)malloc(9 * sizeof(int*));
	for(int i = 0; i < 9; i++)  
	{  
		tmpCheckBorad[i] = (int*)malloc(9 * sizeof(int));
	} 
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK3 ) )   //����任,  ��ʱ�Ѷ���Ϊ3����ȥ��60����
	{
		tStart = clock();
		m_Sudoku.halfRandomGen(3,tmpCheckBorad);
		tEnd = clock();
		timeStr.Format(_T("����任���ɳ��̣���ʱ:%.7f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		m_Sudoku.setDirectCheckBoard(tmpCheckBorad);
	}
	else   //�������
	{
		 tStart = clock();
		 m_Sudoku.genCheckBoard(); 
		 tEnd = clock();
		 timeStr.Format(_T("������ɳ��̣���ʱ:%.7f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
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
	timeStr.Format(_T("���ɹ�����ʱ:%.7f ��"),(double)( tEnd - tStart)/CLOCKS_PER_SEC);
	if(ret)
	{
		hasFiguredOut = true;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
	}
	else
		MessageBox(_T("�������޽� [=_=]��"));

}

//�������
void CSudokuWYLDlg::OnBnClickedButton1()
{
	
	if ( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK1 ) )   //DLX����ѡ
	{

	}
	else  if( BST_CHECKED == IsDlgButtonChecked( IDC_CHECK2 ))     //���ݷ�����ѡ
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
					DrawRect(pDC, suRect, RGB(205, 250, 240)); // ���С��
					if (keyBoardInput[i][j])
						SpDrawStorke(pDC, suRect, RGB(200, 220, 120), 2);      //������ɫ				
					else
						SpDrawStorke(pDC,suRect,RGB(220,220,220),2);
					CString numStr;
					if (tmp)                               //ȥ��0��ʹ�䲻��ʾ
						numStr.Format(_T("%d"),tmp);
					else numStr = _T("");
					DrawNumbers(pDC, i, j, numStr, textColor, cellColor);

			}
			
		}
		//������
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

//������
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
	pDC->SetBkColor(bkColor); //bkColor��brush��ɫһ�£�
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



//ע�������i,j��point.x��point.y������
//.x�Ǿ�����ߣ���i�Ǵ����������ڼ��С�


void CSudokuWYLDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CDC *pDC = GetDC();
	GetCursorPos(&point);               
	ScreenToClient(&point);
	double docx = point.x;           
	double docy = point.y;
	if (hasFiguredOut == false)  //δ���״̬���ܽ�����������
	{
		if (docx < LEFT + SIZE * 9 && docx > LEFT && docy > TOP && docy < TOP + SIZE * 9)
		{
			mouseY = (docx - LEFT) / SIZE;     //�����mouseY�������j....
			mouseX = (docy - TOP) / SIZE;
			CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
				LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));

			DrawRect(pDC, selectedRect, RGB(205, 250, 240));				
			SpDrawStorke(pDC, selectedRect, RGB(200, 220, 120), 2);       //˫��ʱ�޸ĸ��ӱ߿�Ϊ "������ɫ"��
			keyBoardInput[mouseX][mouseY] = 1;							  //ѡ�и������á�

		}

		//Ϊ�˷�ֹ���ָ��������߸ı䣬��Ҫ�ػ湬��
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
	COLORREF textColor(RGB(248,147,29));     //��������ֵ����ɫ
	COLORREF cellColor(RGB(205, 250, 240));

	if (0 <= mouseX && mouseX <= 8 && mouseY >= 0 && mouseY <= 8)
	{
		if (pMsg->message == WM_KEYDOWN)
		{
			int keyValue = pMsg->wParam;
			if (keyValue >= 49 && keyValue <= 57) //�涨����ֵΪ1~9
			{
				int inValue = keyValue - 48;
				CString numStr;
				numStr.Format(_T("%d"), inValue);

				//�ж������Ƿ����Ҫ��
				if (!m_Sudoku.isValid(mouseX,mouseY,inValue))
				{
					MessageBox(_T("���벻�Ϸ�"));
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, RGB(220, 220, 220), 2);           //��ԭѡ�п���ɫ
				}
				else
				{
					m_Sudoku.setSimpleOneCheckBoard(mouseX, mouseY, inValue);
					DrawNumbers(pDC, mouseX, mouseY, numStr, textColor, cellColor);
				}

			}
			else  //��������Ĭ��Ϊ��
			{
				CString numDefaultStr;
				numDefaultStr.Format(_T(""));
				DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, textColor, cellColor);
				CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
					LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
				SpDrawStorke(pDC, selectedRect, RGB(220, 220, 220), 2);           //��ԭѡ�п���ɫ
			}


		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


//��������
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
//��GetDC()���������document��������Ͻǡ�
//�����GetWindowDC()�����¶��Ǵ��ڵ����Ͻ�Ϊ�滭ԭ��ġ�

