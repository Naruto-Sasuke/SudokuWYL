
// SudokuWYLDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "sudoku.h"
#include "afxcmn.h"


// CSudokuWYLDlg 对话框
class CSudokuWYLDlg : public CDialogEx
{
// 构造
public:
	CSudokuWYLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SUDOKUWYL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton DLXCheckBox;
	CButton backtrackCheckBox;
	void calu();      //开始计算
	void caluDlx();
	void SpDrawStorke(CDC *pDC, CRect rect, COLORREF color,int stroke);  //自定义笔刷
	afx_msg void OnBnClickedButton1();

	void DrawSudoku(CDC* pDC);	       //画界面
	void DrawNumbers(CDC*pDC, int i, int j, CString numStr,COLORREF textColor, COLORREF bkColor);     //vs2013新增
	void DrawRect(CDC*pDC, CRect rect, COLORREF color);  //vs2013新增
	//int  GetKeyBoardInput(int i, int j);                    //得到keyBoardInput的某个位置的值

private:
	sudoku m_Sudoku9; 
	sudoku *p_Sudoku16 = new sudoku(16);    //难道一定要这样初始化？？
	SudokuDlx *p_SudokuDlx9 = new SudokuDlx(9);
	SudokuDlx *p_SudokuDlx16 = new SudokuDlx(16);  //太TM冗余了！！
	int sudokuSize;		//9或者16
	int mouseX;        //数独内双击鼠标的格子编号
	int mouseY;
	bool canSelected;    //表示当前数独状态能否选择格子
	bool keyBoardInput[SUDOKU_SIZE_9][SUDOKU_SIZE_9];  //记录哪些位置是键盘输入的值，以便进行颜色标记。
	bool keyBoardInput16[SUDOKU_SIZE_16][SUDOKU_SIZE_16];  //  

public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButton3();
	// 数独大小复选框
	CComboBox m_cbBox;
	afx_msg void cbn_SelectedChange();
};
