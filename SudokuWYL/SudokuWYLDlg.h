
// SudokuWYLDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "sudoku.h"
#include "afxcmn.h"


// CSudokuWYLDlg �Ի���
class CSudokuWYLDlg : public CDialogEx
{
// ����
public:
	CSudokuWYLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SUDOKUWYL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton DLXCheckBox;
	CButton backtrackCheckBox;
	void calu();      //��ʼ����
	void caluDlx();
	void SpDrawStorke(CDC *pDC, CRect rect, COLORREF color,int stroke);  //�Զ����ˢ
	afx_msg void OnBnClickedButton1();

	void DrawSudoku(CDC* pDC);	       //������
	void DrawNumbers(CDC*pDC, int i, int j, CString numStr,COLORREF textColor, COLORREF bkColor);     //vs2013����
	void DrawRect(CDC*pDC, CRect rect, COLORREF color);  //vs2013����
	//int  GetKeyBoardInput(int i, int j);                    //�õ�keyBoardInput��ĳ��λ�õ�ֵ

private:
	sudoku m_Sudoku9; 
	sudoku *p_Sudoku16 = new sudoku(16);    //�ѵ�һ��Ҫ������ʼ������
	SudokuDlx *p_SudokuDlx9 = new SudokuDlx(9);
	SudokuDlx *p_SudokuDlx16 = new SudokuDlx(16);  //̫TM�����ˣ���
	int sudokuSize;		//9����16
	int mouseX;        //������˫�����ĸ��ӱ��
	int mouseY;
	bool canSelected;    //��ʾ��ǰ����״̬�ܷ�ѡ�����
	bool keyBoardInput[SUDOKU_SIZE_9][SUDOKU_SIZE_9];  //��¼��Щλ���Ǽ��������ֵ���Ա������ɫ��ǡ�
	bool keyBoardInput16[SUDOKU_SIZE_16][SUDOKU_SIZE_16];  //  

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton3();
	// ������С��ѡ��
	CComboBox m_cbBox;
	afx_msg void cbn_SelectedChange();
};
