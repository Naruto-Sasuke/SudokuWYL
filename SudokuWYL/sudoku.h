#pragma once


// sudoku

#define SUDOKU_SIZE_9 9
#define SUDOKU_SIZE_16 16
#define minSeedNum 17
#define maxSeedNum 35
#define		SIZE		35		
#define		TOP			20		
#define		LEFT		20		

#include <vector>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

template <class myOUT, class DATA>
inline void printLogInfo(myOUT& myOUT,\
	DATA& data, bool isPrintTime=true);

template <class myIN, class DATA>
inline bool inData(myIN& in, DATA& data){
	return bool(in >> data);
}


inline char* getTime(){
	time_t tmptime;
	time(&tmptime);
	return asctime(gmtime(&tmptime));
}

template <class myOUT, class DATA>
inline void printLogInfo(myOUT& out, \
	DATA& data, bool isPrintTime){
		if(isPrintTime)
			out << getTime();
		out << data << std::endl;
}



inline bool hasSpace(const void* ptr){
	std::fstream fs;
	//	fs.open("log/init.log", std::fstream::out);           //修改了
	fs.open("../init.log", std::fstream::out);   
	if(ptr == NULL){
		printLogInfo(fs, \
			"[FATAL]: Failed, no enough space, exited.");
		printLogInfo(std::cout, \
			"[FATAL]: Failed, no enough space, exited.");
		fs.close();
		return false;
	}
	fs.close();
	return true;
}

class sudoku {
public:
	sudoku(int _size_ =9);      //....
	 virtual ~sudoku();

	int getValueFromCheckBoard(int i,int j);   //新增的，得到CheckBoard的[i][j]值
	void setCheckBoard();
	void genCheckBoard();
	void halfRandomGen(int difficulty,int**table);      //半随机生成
							


	void setDirectCheckBoard(int **tmpCheckBorad);           //直接设置checkboard
	void setSimpleOneCheckBoard(int x, int y, int value);    


	void outPutCheckBoard(const char* \
		filename="../out.txt", bool isPrintScreen=false);       

	bool solve();

	bool isValid(const int x, \
		const int y, const int value);
	

private:

	int __sqt__;                     
	int __size__;

	bool canBeSolved;

	int* seed;
	int* nextZero;
	int** checkBoard;
	 vector<int>src;

	void setValue(const int x, \
		 const int y, const int value);
	void randSeed();
	void dfs(const int x, const int y);
	//bool isValid(const int x,\
	//	const int y, const int value);
	//void setValue(const int x, \
	//	const int y, const int value);
	void initDfs(std::vector<std::pair<int, int> \
	>& vec, bool& ok, int pos);
};


class SudokuDlx{

public:

	SudokuDlx(int _size_ = SUDOKU_SIZE_9);
	~SudokuDlx();

	void setCheckBoard();

	bool solve();

	int getValue(const int x, const int y);

	void SudokuDlx::outPutCheckBoard(\
		const char *filename, bool isPrintScreen);

private:

	bool isSolved;

	int MaxM, MaxN, maxnode;
	int __size__, size, ansd;

	int* L;
	int* R;
	int* U;
	int* D;
	int* S;
	int* H;
	int* Ans;
	int* Col;
	int* Row;
	int* target;

	void reSet();
	void remove(const int c);
	void resume(const int c);
	void link(const int r, const int c);
	void link_process(int x, int y, int tmp);
	void insert(int x, int y, int val);

	bool dance(int dep);
};