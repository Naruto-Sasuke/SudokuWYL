// sudoku.cpp : 实现文件
//

#include "stdafx.h"
#include "SudokuWYL.h"
#include "afxdialogex.h"
#include "sudoku.h"
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <ctime>
#include <cstring>
#include <algorithm>

// sudoku

int sudoku::getValueFromCheckBoard(int i,int j)
{
	if(i >= SUDOKU_SIZE_9 || j >= SUDOKU_SIZE_9)  //针对9*9
	{
		return -1;
	}
	else
		return checkBoard[i][j] ;
}


// sudoku 消息处理程序
sudoku::sudoku(int _size_){
	_size_ = 9;
	canBeSolved = false;
	__size__ = _size_;
	__sqt__ = int(sqrt((double)__size__));                     
	__size__ = __sqt__ * __sqt__;
	checkBoard = (int**)malloc(__size__ * sizeof(int*));
	if(hasSpace(this->checkBoard) == false) exit(1);
	for(int i = 0; i < __size__; ++i){
		checkBoard[i] = (int*)malloc(__size__ * sizeof(int));
		if(hasSpace(this->checkBoard[i]) == false) exit(1);
		for(int j = 0; j < __size__; ++j) checkBoard[i][j] = 0;
	}
	seed = (int*)malloc(__size__ * sizeof(int));
	if(hasSpace(seed) == false) exit(1);
	nextZero = (int*)malloc(__size__ * __size__ * sizeof(int));
	if(hasSpace(nextZero ) == false) exit(1);
	for(int i = 0; i < __size__; ++i) seed[i] = i + 1;
	randSeed(); 
}

 sudoku::~sudoku(){
	 //if(this->checkBoard != NULL)
	 //{
		// for(int i = 0; i < __size__; ++i)
		//	 if (NULL!=checkBoard[i])
		//	 {
		//		  free(checkBoard[i]);
		//	 }
		//	
		// free(checkBoard);
	 //}

}

void sudoku::setValue(const int x, \
	const int y, const int value){
		checkBoard[x][y] = value;
}

void sudoku::outPutCheckBoard(\
	const char *filename, bool isPrintScreen){
		std::fstream fs;
		fs.open(filename, std::fstream::out);
		for(int i = 0; i < __size__; ++i){
			for(int j = 0; j < __size__; ++j){
				fs << checkBoard[i][j] << " ";
				if(isPrintScreen)
					std::cout << checkBoard[i][j] << " ";
			}
			fs << std::endl;
			if(isPrintScreen)
				std::cout << std::endl;
		}
		if(isPrintScreen)
			std::cout << std::endl;
		fs.close();
}

void sudoku::setCheckBoard(){
	fstream fs;
	fs.open("../in.txt", ios::in);
	for(int i = 0; i < __size__; ++i){
		for(int j = 0; j < __size__; ++j){
			inData(fs, checkBoard[i][j]);
		}
	}
	fs.close();
}

void sudoku::dfs(int x, int y){
	if(canBeSolved) return;
	if(y == __size__){     //越界，y置0
		++x;
		y = 0;
	}
	while(x < __size__ && checkBoard[x][y] != 0){
		++y;
		if(y == __size__){
			y = 0;
			++x;
		}
	}
	if(x == __size__){
		canBeSolved = true;
		return;
	}
	for(int i = 1;i <= __size__; ++i){
		if(isValid(x, y, i)){
			setValue(x, y, i);
			dfs(x, y + 1);
			if(canBeSolved) return;
			setValue(x, y, 0);
		}
	}
}

bool sudoku::solve(){
	canBeSolved = false;
	dfs(0, 0);
	return canBeSolved;
}

bool sudoku::isValid(const int x, \
	const int y, const int value){
		for(int i = 0;i < __size__;i ++){
			if(checkBoard[x][i] == value\
				|| checkBoard[i][y] == value)
				return false;
		}
		int row = (x / __sqt__) * __sqt__;
		int col = (y / __sqt__) * __sqt__;
		for(int i = row;i < row + __sqt__;i ++){
			for(int j = col;j < col + __sqt__;j ++)
				if(checkBoard[i][j] == value) return false;
		}
		return true;
}

void sudoku::randSeed(){
	srand((unsigned)time(NULL));
	for(int i = 0;i < __size__; ++i){
		int rand_pos = i + rand() % (__size__ - i);
		int tmp = seed[i];
		seed[i] = seed[rand_pos];
		seed[rand_pos] = tmp;
	}
}

//主要是随机位置产生随机的vector的中的数。通过2个随机达到较好的均匀分布。
//pos表示随机的__size__个数的vector的pos位置。
void sudoku::initDfs(std::vector<std::pair<int,\
	int> >& vec, bool& ok, int pos){
		if(pos == vec.size()) ok = true;
		if(ok) return;
		for(int i = pos; i < vec.size(); ++i){
			int start = rand() % __size__;
			int x = vec[pos].first;
			int y = vec[pos].second;
			for(int j = start; j < __size__ + start; ++j){
				if(isValid(x, y,  seed[j % __size__])){
					setValue(x, y, seed[j % __size__]);
					initDfs(vec, ok, pos + 1);
					if(ok) return;
					setValue(x, y, 0);
				}
			}
		}
}

void sudoku::genCheckBoard(){
	for(int i = 0; i < __size__; ++i){
		for(int j = 0; j < __size__; ++j)
			checkBoard[i][j] = 0;
	}
	srand((unsigned)time(NULL));
	std::vector<std::pair<int, int> > vec;
	std::map<std::pair<int, int>, bool> mp;   //对随机的位置进行
	int randNum = rand() % (maxSeedNum - \
		minSeedNum) + minSeedNum;
	while(mp.size() < randNum){
		int x = rand() % __size__;
		int y = rand() % __size__;
		mp[std::make_pair(x, y)] = true;
	}
	for(std::map<std::pair<int, int>, bool>::iterator \
		it = mp.begin(); it != mp.end(); ++it){
			vec.push_back(it->first);
	}
	bool ok = false;
	initDfs(vec, ok, 0);
	//	outPutCheckBoard("data/in.txt", false);       //修改了     
	outPutCheckBoard("../in.txt", false);


	if(solve() == false)
	{
		genCheckBoard();
	}
	
}


//////////////////////////////////////////////////////////////////////////
void sudoku::setDirectCheckBoard(int **tmpCheckBorad)
{
	for(int i = 0; i < SUDOKU_SIZE_9;i++)
	{
		for (int j = 0; j < SUDOKU_SIZE_9; j++)
		{

			 checkBoard[i][j] = tmpCheckBorad[i][j];     //这个应该是在checkborad初始化之后使用
		}
	}
}

void sudoku::setSimpleOneCheckBoard(int x, int y, int value){
	checkBoard[x][y] = value;
}

void sudoku::halfRandomGen(int difficulty, int **table)
{
		srand(time(NULL));
		/*
		table = new int*[9];  
		for(int i = 0; i < 9; i++)  
		{  
			table[i] = new int[9];  
			memset(table[i],0,sizeof(int)*9);  
		} 
		*/
		src.clear();
		for (int i = 0; i < 9; ++i)
			src.push_back(i + 1);
		random_shuffle(src.begin(), src.end()); // 随机化序列

		// 初始化中间的九宫格
		int k = 0;
		for (int i = 3; i < 6; ++i)
			for (int j = 3; j < 6; ++j)
				table[i][j] = src[k++];
		
	
		// 由中间的九宫格交叉变换，初始化上下左右四个九宫格
		for (int i = 3; i < 6; ++i)
		{
			int l = 0;
			for (int j = 3; j < 6; ++j)
			{
				if (i == 3)
				{
					table[i + 1][l] = table[i][j];
					table[i + 2][l + 6] = table[i][j];
					++l;
				}
				else if (i == 4)
				{
					table[i + 1][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
				else if (i == 5)
				{
					table[i - 2][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
			}
		}
		for (int j = 3; j < 6; ++j)
		{
			int l = 0;
			for (int i = 3; i < 6; ++i)
			{
				if (j == 3)
				{
					table[l][j + 1] = table[i][j];
					table[l + 6][j + 2] = table[i][j];
					++l;
				}
				else if (j == 4)
				{
					table[l][j + 1] = table[i][j];
					table[l + 6][j - 1] = table[i][j];
					++l;
				}
				else if (j == 5)
				{
					table[l][j - 2] = table[i][j];
					table[l + 6][j - 1] = table[i][j];
					++l;
				}
			}
		}

		// 初始化四个角上的四个九宫格
		for (int i = 0; i < 3; ++i)
		{
			int l = 0;
			for (int j = 3; j < 6; ++j)
			{
				if (i == 0)
				{
					table[i + 1][l] = table[i][j];
					table[i + 2][l + 6] = table[i][j];
					++l;
				}
				else if (i == 1)
				{
					table[i + 1][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
				else if (i == 2)
				{
					table[i - 2][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
			}
		}
		for (int i = 6; i < 9; ++i)
		{
			int l = 0;
			for (int j = 3; j < 6; ++j)
			{
				if (i == 6)
				{
					table[i + 1][l] = table[i][j];
					table[i + 2][l + 6] = table[i][j];
					++l;
				}
				else if (i == 7)
				{
					table[i + 1][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
				else if (i == 8)
				{
					table[i - 2][l] = table[i][j];
					table[i - 1][l + 6] = table[i][j];
					++l;
				}
			}
		}

		difficulty *= 20;
		while (difficulty--)
		{
			table[rand() % 9][rand() % 9] = 0;
		}	
}

//SudokuDlx::SudokuDlx(const int _size_){
//	__size__ = _size_;
//	int sqt = (int)sqrt(__size__);
//	__size__ = sqt * sqt;
//	int len = 4 * __size__ * __size__ * __size__\
//		+ __size__ + 1;
//	L = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(L)) exit(1);
//	R = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(R)) exit(1);
//	U = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(U)) exit(1);
//	D = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(D)) exit(1);
//	Col = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(Col)) exit(1);
//	Vis = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(Vis)) exit(1);
//	Row = (int*)malloc(len * sizeof(int));
//	if(false == hasSpace(Row)) exit(1);
//	H = (int*)malloc(__size__ + 1);
//	if(false == hasSpace(H)) exit(1);
//	S = (int*)malloc(__size__ + 1);
//	if(false == hasSpace(S)) exit(1);
//	Ans = (int*)malloc(__size__ * __size__ * __size__);
//	if(false == hasSpace(Ans)) exit(1);
//	for(int i = 0; i <= __size__; ++i){
//		S[i] = 0;
//		L[i] = i - 1;
//		R[i] = i + 1;
//		U[i] = D[i] = i;
//	}
//	L[0] = __size__, R[__size__] = 0;
//	num = __size__;
//	for(int i = 1; i <= __size__; ++i) H[i] = -1;
//}
//
//SudokuDlx::~SudokuDlx(){
//	free(L);
//	free(R);
//	free(U);
//	free(D);
//	free(S);
//	free(H);
//	free(Col);
//	free(Row);
//}
//
//void SudokuDlx::link(const int r, const int c){
//	++S[Col[++num] = c];
//	Row[num] = r;
//	D[num] = D[c];
//	U[D[c]] = num;
//	U[num] = c;
//	D[c] = num;
//	if(H[r] < 0) H[r] = L[num] = R[num] = num;
//	else{
//		R[num] = R[H[r]];
//		L[R[H[r]]] = num;
//		L[num] = H[r];
//		R[H[r]] = num;
//	}
//}
//
//void SudokuDlx::remove(int c){
//	L[R[c]] = L[c];
//	R[L[c]] = R[c];
//	for(int i = D[c]; i != c; i = D[i]){
//		for(int j = R[i]; j != i; j = R[j]){
//			U[D[j]] = U[j];
//			D[U[j]] = D[j];
//			--S[Col[j]];
//		}
//	}
//}
//
//void SudokuDlx::resume(int c){
//	for(int i = U[c]; i != c; i = U[i]){
//		for(int j = L[i]; j != i; j = L[j]){
//			++S[Col[U[D[j]]=D[U[j]]=j]];
//		}
//	}
//	L[R[c]] = R[L[c]] = c;
//}
//
//bool SudokuDlx::dance(int dep){
//	if(R[0] == 0){
//		return true;
//	}
//	int c = R[0];
//	for(int i = R[0]; i != 0; i = R[i])
//		if(S[i] < S[c]) c = i;
//	remove(c);
//	for(int i = D[c]; i != c; i = D[i]){
//		Ans[dep] = Row[i];
//		for(int j = R[i]; j != i; j = R[j]) remove(Col[j]);
//		if(dance(dep + 1)) return true;
//		for(int j = L[i]; j != i; j = L[j]) resume(Col[j]);
//	}
//	resume(c);
//	return false;
//}
//
//bool SudokuDlx::solve(){
//	if(false == dance(0)) return false;
//	return true;
//}
//
//void SudokuDlx::setCheckBoard(){
//	int tmp;
//	for(int i = 0; i < __size__; ++i){
//		for(int j = 0; j < __size__; ++j){
//			inData(std::cin, tmp);
//			if(tmp == 0){
//
//			}else{
//
//			}
//		}
//	}
//}
