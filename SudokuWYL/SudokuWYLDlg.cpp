
// SudokuWYLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SudokuWYL.h"
#include "SudokuWYLDlg.h"
#include "afxdialogex.h"
#include <sstream>
#include <time.h>
#include "sudoku.h"

#define CELLCOLOR			RGB(205, 250, 240)  //���ӱ�����ɫ
#define STROKECOLOR			RGB(220,220,220)    //���ӱ߿���ɫ
#define SELECTEDNUMCOLOR	RGB(255, 51, 0)		//ѡ�����ֵ���ɫ
#define MYNUMCOLOR			RGB(118, 77, 57)	//��ͨ���ֵ���ɫ
#define GONGCOLOR			RGB(100, 210, 130)  //������ɫ
#define SPSTROKECOLOR		RGB(200, 220, 120)	//ѡ�еı߿����ɫ
#define LITTLENUMCOLOR		RGB(175, 18,  88)	//С����ɫ		



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
	ON_BN_CLICKED(IDC_BUTTON4, &CSudokuWYLDlg::OnBnClickedButton4)
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
	canSelected = true;
	sudokuSize = 9;  //��ʼ��Ϊ9x9
	//BUTTON4�ǡ���ʾDLX�汾����ť�����DLX������ɵ���������ݷ����ɵ����̲�һ��ʱ�ſ��á�
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	 backTrackSameWithDlx = true;      //Ĭ�����ַ������ɵ�����һ��


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



	//��ʼ����ѡ��
	m_cbBox.AddString(_T("9x9"));  //1
	m_cbBox.AddString(_T("16x16"));  // 0
	m_cbBox.SetCurSel(1);




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


//��ѡ��
void CSudokuWYLDlg::cbn_SelectedChange()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
		GetDlgItem(IDC_CHECK3)->EnableWindow(TRUE);   //9x9ʱ������任����
		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);   

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
		GetDlgItem(IDC_CHECK3)->EnableWindow(FALSE);     //16x16ʱ������任������
		CButton* pBtn = (CButton*)GetDlgItem(IDC_CHECK3);  //ͬʱ����checkboxΪΪѡ��
		pBtn->SetCheck(0);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);		//16x16ʱ��������ɿ� 
		GetDlgItem(IDC_EDIT1)->SetWindowTextW(_T(""));
	}
	GetDlgItem(IDC_STATIC)->SetWindowTextW(_T(""));		 //�������ʾ�������
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);				//����ʾDLX�汾����ť����Ϊ�����ã�����ԭ����
	GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("��ʾDLX�汾"));
	canSelected = true;
	CDC *pDC = GetDC();
	DrawSudoku(pDC);
	Invalidate();


}


//��������
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
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3))   //����任,  ��ʱ�Ѷ���Ϊ3����ȥ��60����
	{															//����任��֧��16x16
		tStart = clock();
		CString numStr;
	    GetDlgItem(IDC_EDIT1)->GetWindowTextW(numStr);
		m_Sudoku9.halfRandomGen(_ttoi(numStr), tmpCheckBorad);
		tEnd = clock();
		timeStr.Format(_T("����任���ɳ��̣���ʱ:%.6f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		m_Sudoku9.setDirectCheckBoard(tmpCheckBorad);
		m_Sudoku9.outPutCheckBoard("../in.txt",false);   //������任���ɵ����������in.txt��
		p_SudokuDlx9->setCheckBoard();        //dlx�����ȡ����
	}
	else   //�������
	{
		if (sudokuSize == 9)
		{
			tStart = clock();
			m_Sudoku9.genCheckBoard();      //�������ʱ����������ȷ������ʱ��in.txt�Ѿ��õ��˸����̡�
			
			tEnd = clock();
			timeStr.Format(_T("������ɳ��̣���ʱ:%.6f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
			GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
			m_Sudoku9.setCheckBoard();            //��in.txt���뵽checkboard��
			p_SudokuDlx9->setCheckBoard();        //dlx�����ȡ����
		}
		else
		{
			tStart = clock();
			p_Sudoku16->genCheckBoard();
			tEnd = clock();
			timeStr.Format(_T("������ɳ��̣���ʱ:%.6f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
			GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
			p_Sudoku16->setCheckBoard();			 //��in.txt���뵽checkboard��
			p_SudokuDlx16->setCheckBoard();     //dlx�����ȡ����
		}

	}
	CDC *pDC = GetDC();
	DrawSudoku(pDC);             //ֻ����Ի��ݵ����̻������ɡ�
}



void CSudokuWYLDlg::calu()
{
	CString timeStr;
	clock_t tStart = clock();
	bool ret;
	/*���ɵķ�����3�֣���һ��������ɣ����ֵ�genCheckBoardʱ�����Ѿ������˶�Ӧ��in.txt,
	*�ڶ��ֽ���任����Ҫ����outputCheckBoard����checkboard���������in.txt
	*�������˹����ã�����ÿ�����þ���ֱ�Ӷ��ڲ���checkboard�������и��ģ�������������ʱ�����������outPutCheckBoard
	*����Ϊ��Ӧ���������
	*/
	  
	if (sudokuSize == 9)
	{
		m_Sudoku9.outPutCheckBoard("../in.txt", false);   //Ӧ�����������ɷ�ʽ
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
		p_Sudoku16->outPutCheckBoard("../in.txt", false);   //Ӧ�����������ɷ�ʽ
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
	timeStr.Format(_T("���ݷ����ɹ�����ʱ:%.6f ��"), (double)(tEnd - tStart) / CLOCKS_PER_SEC);
	if (ret)
	{
		canSelected = false;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
		if (sudokuSize == 9)
			m_Sudoku9.outPutCheckBoard();   //���û��ݷ����solve��Ϊ�������out.txt
		else
			p_Sudoku16->outPutCheckBoard();  //���û��ݷ����solve��Ϊ�������out.txt
		
	}
	else
		MessageBox(_T("�������޽� [=_=]��"));

}
void CSudokuWYLDlg::caluDlx()
{
	CString timeStr;
	clock_t tStart1,tEnd1,tEnd2;
	bool ret;
	/*���ɵķ�����3�֣���һ��������ɣ����ֵ�genCheckBoardʱ�����Ѿ������˶�Ӧ��in.txt,
	*�ڶ��ֽ���任����Ҫ����outputCheckBoard����checkboard���������in.txt
	*�������˹����ã�����ÿ�����þ���ֱ�Ӷ��ڲ���checkboard�������и��ģ�������������ʱ�����������outPutCheckBoard
	*����Ϊ��Ӧ���������
	*/

	if (sudokuSize == 9)
	{
		m_Sudoku9.outPutCheckBoard("../in.txt", false);   //Ӧ�����������ɷ�ʽ
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
		m_Sudoku9.solve();   //���û��ݷ����solve��Ϊ�˻�ͼ
	    tEnd1 = clock();
		p_SudokuDlx9->setCheckBoard();        //dlx�����ȡ����
		ret = p_SudokuDlx9->solve();       //��dlx�����solve
		p_SudokuDlx9->outPutCheckBoard("../outDlx9.txt", false);   //���dlx���ɵ�����
		//������ְ汾�Ƿ�һ��
		for (int i = 0; i < SUDOKU_SIZE_9;i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_9; j++)
			{
				if (m_Sudoku9.getValueFromCheckBoard(i,j) != p_SudokuDlx9->getValue(i,j))
				{
					backTrackSameWithDlx = false;
				}
			}
		}
	}
	else
	{
		p_Sudoku16->outPutCheckBoard("../in.txt", false);   //Ӧ�����������ɷ�ʽ
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
		p_Sudoku16->solve();   //���û��ݷ����solve��Ϊ�˻�ͼ
		tEnd1 = clock();
		p_SudokuDlx16->setCheckBoard();        //dlx�����ȡ����
		ret = p_SudokuDlx16->solve();    //��dlx�����solve
		p_SudokuDlx16->outPutCheckBoard("../outDlx16.txt", false);   //����dlx���ɵ��Ƿ���ͬ
		//������ְ汾�Ƿ�һ��
		for (int i = 0; i < SUDOKU_SIZE_16; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_16; j++)
			{
				if (p_Sudoku16->getValueFromCheckBoard(i, j) != p_SudokuDlx16->getValue(i, j))
				{
					backTrackSameWithDlx = false;
				}
			}
		}
	}

    tEnd2 = clock();         //tEnd2-tEnd1��dlx����
	if (backTrackSameWithDlx)
	{
		timeStr.Format(_T("DLX���ɹ�����ʱ:%.6f ��\n���û��ݷ���⣡��ʱ:%.6f ��"), (double)(tEnd2 - tEnd1) / CLOCKS_PER_SEC, (double)(tEnd1 - tStart1) / CLOCKS_PER_SEC);
	}
	else
	{
		timeStr.Format(_T("DLX���ɹ�����ʱ:%.6f ��\n���û��ݷ���⣡��ʱ:%.6f ��,���ַ������ɵ����̲�ͬ��"), \
			(double)(tEnd2 - tEnd1) / CLOCKS_PER_SEC, (double)(tEnd1 - tStart1) / CLOCKS_PER_SEC);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(TRUE);   
	}
	if (ret)
	{
		canSelected = false;
		GetDlgItem(IDC_STATIC)->SetWindowTextW(timeStr);
		CDC *pDC = GetDC();
		DrawSudoku(pDC);
		if (sudokuSize == 9)
			m_Sudoku9.outPutCheckBoard();    //���û��ݷ����solve��Ϊ�������out.txt
		else
			p_Sudoku16->outPutCheckBoard();     //���û��ݷ����solve��Ϊ�������out.txt

	}
	else
		MessageBox(_T("�������޽� [=_=]��"));
}
//�������
void CSudokuWYLDlg::OnBnClickedButton1()
{

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))   //DLX����ѡ
	{
		caluDlx();
	}
	else  if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))     //���ݷ�����ѡ
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
				DrawRect(pDC, suRect, CELLCOLOR); // ���С��			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numStr;
				if (tmp)                               //ȥ��0��ʹ�䲻��ʾ
					numStr.Format(_T("%d"), tmp);
				else numStr = _T("");
				DrawNumbers(pDC, i, j, numStr, MYNUMCOLOR, CELLCOLOR);

			}

		}
		for (int i = 0; i < SUDOKU_SIZE_9; i++)   //�����̵������ֵ�λ�����������ɫ��ʾ
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
		//������
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
				DrawRect(pDC, suRect, CELLCOLOR); // ���С��			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numStr;
				if (tmp)                               //ȥ��0��ʹ�䲻��ʾ
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

		//������
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

//������
void CSudokuWYLDlg::DrawNumbers(CDC*pDC, int i, int j, CString numStr, COLORREF textColor, COLORREF bkColor)
{
	CFont font;
	font.CreateFont(
		17,                        // nHeight
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
	int num = _ttoi(numStr);      //CStringתint
	if (num > 0){				  
		if (sudokuSize == 9)
		{
			pDC->TextOut(LEFT + SIZE*j + SIZE / 3, TOP + SIZE*i + SIZE / 5, numStr);
		}
		else //16x16�����̵�����λ��
		{

			if (num >= 10) //2λ��
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



//ע�������i,j��point.x��point.y������
//.x�Ǿ�����ߣ���i�Ǵ����������ڼ��С�


void CSudokuWYLDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CDC *pDC = GetDC();
	GetCursorPos(&point);
	ScreenToClient(&point);
	double docx = point.x;
	double docy = point.y;
	mouseY = (docx - LEFT) / SIZE;     //�����mouseY�������j....
	mouseX = (docy - TOP) / SIZE;
	//9x9
	if (sudokuSize == 9){
		if (canSelected == true)
		{
			if (docx < LEFT + SIZE * 9 && docx > LEFT && docy > TOP && docy < TOP + SIZE * 9)
			{
				CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
					LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
				DrawRect(pDC, selectedRect, CELLCOLOR);				//��ѡ�еĸ�����ԭ����ɫ���
				SpDrawStorke(pDC, selectedRect, SPSTROKECOLOR, 2);       //˫��ʱ�޸ĸ��ӱ߿�Ϊ "������ɫ"��
				m_Sudoku9.setSimpleOneCheckBoard(mouseX, mouseY, 0);
				keyBoardInput[mouseX][mouseY] = 1;						//ͬʱ����ѡ��

			}

			//Ϊ�˷�ֹ���ָ��������߸ı䣬��Ҫ�ػ湬��
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

				DrawRect(pDC, selectedRect, CELLCOLOR);				//��ѡ�еĸ�����ԭ����ɫ���
				SpDrawStorke(pDC, selectedRect, SPSTROKECOLOR, 2);       //˫��ʱ�޸ĸ��ӱ߿�Ϊ "������ɫ"��
				p_Sudoku16->setSimpleOneCheckBoard(mouseX, mouseY, 0);	
				keyBoardInput16[mouseX][mouseY] = 1;

			}

			//Ϊ�˷�ֹ���ָ��������߸ı䣬��Ҫ�ػ湬��
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
//Ϊ�˷�ֹͨ������ʹ��combox��ý��㣬�Ӷ����9x9��16x16�Ľ���ת��
//�Ƽ�ÿ�ν��н���ת��������һ�����̣�ʹ��comboxʧȥ����
	CDC *pDC = GetDC();
	//9x9
	if (sudokuSize == 9)
	{
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
					if (!m_Sudoku9.isValid(mouseX, mouseY, inValue))
					{
						MessageBox(_T("���벻�Ϸ�"));
						CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
							LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
						SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //��ԭѡ�п���ɫ
						keyBoardInput[mouseX][mouseY] = 0;							//������Ҫ��ʱ����Ӧλ��keyBoardInput��Ϊ0
					}
					else
					{
						m_Sudoku9.setSimpleOneCheckBoard(mouseX, mouseY, inValue);
						DrawNumbers(pDC, mouseX, mouseY, numStr, SELECTEDNUMCOLOR, CELLCOLOR);
						keyBoardInput[mouseX][mouseY] = 1;							
					}

				}
				else  //��������Ĭ��Ϊ��
				{
					CString numDefaultStr;
					numDefaultStr.Format(_T(""));
					DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, SELECTEDNUMCOLOR, CELLCOLOR);
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //��ԭѡ�п���ɫ
					keyBoardInput[mouseX][mouseY] = 0;					//������Ҫ��ʱ����Ӧλ��keyBoardInput��Ϊ0
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
				if ((keyValue >= 65 && keyValue <= 71) ||( keyValue>=49 && keyValue <=57)) //��A��F��Ϊ10��16
				{
					int inValue = 0;
					if (keyValue >= 48 && keyValue <= 57)
						inValue = keyValue - 48;
					else
						inValue = keyValue - 55;
					CString numStr;
					numStr.Format(_T("%d"), inValue);

					//�ж������Ƿ����Ҫ��
					if (!p_Sudoku16->isValid(mouseX, mouseY, inValue))
					{
						MessageBox(_T("���벻�Ϸ�"));
						CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
							LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
						SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //��ԭѡ�п���ɫ
						keyBoardInput[mouseX][mouseY] = 0;							//������Ҫ��ʱ����Ӧλ��keyBoardInput��Ϊ0
					}
					else
					{
						p_Sudoku16->setSimpleOneCheckBoard(mouseX, mouseY, inValue);
						DrawNumbers(pDC, mouseX, mouseY, numStr, SELECTEDNUMCOLOR, CELLCOLOR);
						keyBoardInput16[mouseX][mouseY] = 1;			//���������ֺ󣬲�������Ϊ1				
					}

				}
				else  //��������Ĭ��Ϊ��
				{
					CString numDefaultStr;
					numDefaultStr.Format(_T(""));
					DrawNumbers(pDC, mouseX, mouseY, numDefaultStr, SELECTEDNUMCOLOR, CELLCOLOR);
					CRect selectedRect(LEFT + SIZE*mouseY, TOP + SIZE*mouseX, \
						LEFT + SIZE*(mouseY + 1), TOP + SIZE*(mouseX + 1));
					SpDrawStorke(pDC, selectedRect, STROKECOLOR, 2);           //��ԭѡ�п���ɫ
					keyBoardInput[mouseX][mouseY] = 0;							//������Ҫ��ʱ����Ӧλ��keyBoardInput��Ϊ0
				}
			}
		}
	}
#pragma endregion

	return CDialogEx::PreTranslateMessage(pMsg);
}

//��������
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
	backTrackSameWithDlx = true;
	GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("��ʾDLX�汾"));
	GetDlgItem(IDC_BUTTON4)->EnableWindow(FALSE);
	GetDlgItem(IDC_STATIC)->SetWindowTextW(_T(""));

	Invalidate();

}



//////////////////////////////////////////////////////////////////////////
//��GetDC()���������document��������Ͻǡ�
//�����GetWindowDC()�����¶��Ǵ��ڵ����Ͻ�Ϊ�滭ԭ��ġ�
/************************************************************************/
/*								ʹ��˵��                                 */
/*1.��ͨ����ѡ��������̴�Сʱ�����Ҫ��һ�¡��������á���ʹ�ø�ѡ��ʧȥ���㣬����
�������֡�1,9��ʱ��ʹ�����̽�����ġ�
  2.ֻ��ͨ�������Ϸ������ּ���1~9���͡�a~g������16x16������Ч���ֱ����10~16����
/************************************************************************/


#pragma region OnlyUsedInDlxDiff
//�����ֻ��"��ʾDLX"��ť���ã������ʱ�ŵ���

//����ʾDLX�汾����ť
void CSudokuWYLDlg::OnBnClickedButton4()
{
	CDC *pDC = GetDC();
	CString btnStr;
	GetDlgItem(IDC_BUTTON4)->GetWindowTextW(btnStr);
	if (btnStr == _T("��ʾDLX�汾"))
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("���ػ��ݷ�����"));
		DrawSudokuDLX(pDC,true);
	}
	else
	{
		GetDlgItem(IDC_BUTTON4)->SetWindowTextW(_T("��ʾDLX�汾"));
		DrawSudokuDLX(pDC, false);
	}
	
}
//dlx�汾������ռ���壨���֣������ݷ��������½���С����ʾ
void CSudokuWYLDlg::DrawSudokuDLX(CDC* pDC,bool showLittleNums)
{
	//9x9
	if (sudokuSize == 9){
		for (int i = 0; i < SUDOKU_SIZE_9; i++)
		{
			for (int j = 0; j < SUDOKU_SIZE_9; j++)
			{
				int tmp = p_SudokuDlx9->getValue(i, j);
				CRect suRect(LEFT + SIZE*j, TOP + SIZE*i, LEFT + SIZE*(j + 1), TOP + SIZE*(i + 1));
				DrawRect(pDC, suRect, CELLCOLOR); // ���С��			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numDLXStr;
				numDLXStr.Format(_T("%d"), tmp);
				DrawNumbers(pDC, i, j, numDLXStr, MYNUMCOLOR, CELLCOLOR);
				int tmpBackTrack = m_Sudoku9.getValueFromCheckBoard(i, j);
				if (tmp!=tmpBackTrack)		//���ַ������ɵ����ֲ���ͬ
				{
					CString numTrackStr;
					numTrackStr.Format(_T("%d"), tmpBackTrack);
					if (showLittleNums)			//��ʾ���ݷ�����
						DrawBackTrackLittleNums(pDC, i, j, numTrackStr, LITTLENUMCOLOR, CELLCOLOR);
				}

			}

		}
		for (int i = 0; i < SUDOKU_SIZE_9; i++)   //�����̵������ֵ�λ�����������ɫ��ʾ
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
		//������
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
				int tmp = p_SudokuDlx16->getValue(i, j);
				CRect suRect(LEFT + SIZE*j, TOP + SIZE*i, LEFT + SIZE*(j + 1), TOP + SIZE*(i + 1));
				DrawRect(pDC, suRect, CELLCOLOR); // ���С��			
				SpDrawStorke(pDC, suRect, STROKECOLOR, 2);
				CString numDLXStr;
				numDLXStr.Format(_T("%d"), tmp);
				DrawNumbers(pDC, i, j, numDLXStr, MYNUMCOLOR, CELLCOLOR);
				int tmpBackTrack = p_Sudoku16->getValueFromCheckBoard(i, j);
				if (tmp != tmpBackTrack)		//���ַ������ɵ����ֲ���ͬ
				{
					CString numTrackStr;
					numTrackStr.Format(_T("%d"), tmpBackTrack);
					if (showLittleNums)
						DrawBackTrackLittleNums(pDC, i, j, numTrackStr, LITTLENUMCOLOR, CELLCOLOR);
				}

			}

		}
		for (int i = 0; i < SUDOKU_SIZE_16; i++)        //�����̵������ֵ�λ�����������ɫ��ʾ
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

		//������
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
//��ʾ���ݷ����̣���С����ʾ��
void CSudokuWYLDlg::DrawBackTrackLittleNums(CDC*pDC, int i, int j, CString numStr, COLORREF textColor, COLORREF bkColor)
{
	CFont font;
	font.CreateFont(
		10,                        // nHeight
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
	pDC->SetBkColor(bkColor);	   //bkColor��brush��ɫһ�£�
	int num = _ttoi(numStr);      //CStringתint
	if (num > 0){
		if (sudokuSize == 9)
		{
			pDC->TextOut(LEFT + SIZE*j + SIZE*2.0 / 3, TOP + SIZE*i + SIZE*3.0 / 5, numStr);
		}
		else //16x16�����̵�����λ��
		{

			if (num >= 10) //2λ��
				pDC->TextOut(LEFT + SIZE*j + SIZE*3.0 / 5, TOP + SIZE*i + SIZE*3.0 / 5+SIZE*1.0/10, numStr);
			else
				pDC->TextOut(LEFT + SIZE*j + SIZE *2.0/ 3, TOP + SIZE*i + SIZE*3.0 / 5+SIZE*1.0/10, numStr);
		}
	}
}



#pragma endregion


