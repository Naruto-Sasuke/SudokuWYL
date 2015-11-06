
// SudokuWYLDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "sudoku.h"


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
	void SpDrawStorke(CDC *pDC, CRect rect, COLORREF color,int stroke);  //�Զ����ˢ
	afx_msg void OnBnClickedButton1();

	void DrawSudoku(CDC* pDC);	       //������
	void DrawNumbers(CDC*pDC, int i, int j, CString numStr,COLORREF textColor, COLORREF bkColor);     //vs2013����
	void DrawRect(CDC*pDC, CRect rect, COLORREF color);  //vs2013����
	//int  GetKeyBoardInput(int i, int j);                    //�õ�keyBoardInput��ĳ��λ�õ�ֵ

private:
	sudoku m_Sudoku; 
	int mouseX;        //������˫�����ĸ��ӱ��
	int mouseY;
	bool hasFiguredOut;    //��¼�Ƿ��Ѿ������
	int keyBoardInput[SUDOKU_SIZE_9][SUDOKU_SIZE_9];  //��¼��Щλ���Ǽ��������ֵ���Ա������ɫ��ǡ�

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton3();
};