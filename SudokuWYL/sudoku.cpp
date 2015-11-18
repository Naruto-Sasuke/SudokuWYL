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
	return checkBoard[i][j] ;
}


// sudoku 消息处理程序
sudoku::sudoku(int _size_){
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
	 if (this->checkBoard != NULL)
	 {
		 for (int i = 0; i < __size__; ++i)
			 if (NULL != checkBoard[i])
			 {
			 free(checkBoard[i]);
			 }

		 free(checkBoard);
	 }

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
	outPutCheckBoard("../in.txt", false);      //output才是输出内容


	if(solve() == false)
	{
		genCheckBoard();
	}
	
}


//////////////////////////////////////////////////////////////////////////
void sudoku::setDirectCheckBoard(int **tmpCheckBorad)  //在交叉变换中使用
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

		int numZeros = 0;
		if (difficulty > 64)   //设置最小的数目为17
		{
			difficulty = 64;
		}
		                         
		while (numZeros!=difficulty)
		{
			int x = rand() % 9;
			int y = rand() % 9;
			if (table[x][y]){
				table[x][y] = 0;
				numZeros++;
			}			
		}	
}

/*=====================我是两个类的分界线=================*/




/*
*
*SudokuDlx类的使用方法：
*
*  你只需要关注三个函数（接口）：
*
*      1.setCheckBoard(): 设置棋盘
*      2.设置完成就可以求解：solve()
*      3.求解之后获取某个位置的值：getValue(x, y)
*
*      通过不断使用getValue()可以完成你需要的所有操作
*
*      其余私有函数不需要关注，当成暗盒即可。
*
*/


//===========下面是三个函数的说明=============


/*
* setCheckBoard()设置棋盘，默认的输入源
* 是从标准输入（cin），你自己可以改变输入源
* 通过输入重定向或者直接修改下面的代码也行。
*/
void SudokuDlx::setCheckBoard(){
	int tmp;
	reSet();
	fstream fs;
	fs.open("../in.txt", ios::in);
	//for (int i = 0; i < __size__; ++i){
	//	for (int j = 0; j < __size__; ++j){
	//		inData(fs, checkBoard[i][j]);
	//	}
	//}

	for (int i = 0; i < __size__; ++i){
		for (int j = 0; j < __size__; ++j){
			inData(fs, tmp);
			//inData(std::cin, tmp); //在此修改输入源
			if (tmp != 0){
				insert(i, j, tmp);
			}
			else{
				for (int k = 1; k <= __size__; ++k){
					insert(i, j, k);
				}
			}
		}
	}
	fs.close();
}

/*
*solve()去解决这个数独问题，返回一个布尔值
true表示求解成功，false表示失败，棋盘无解。
如果求解成功，那么会将答案保存在target这个二维数组中。
*/
bool SudokuDlx::solve(){
	isSolved = dance(0);
	if (false == isSolved) return false;
	for (int i = 0; i < ansd; ++i){
		target[(Ans[i] - 1) / __size__] = (Ans[i] - 1) % __size__ + 1;
	}
	return true;
}

/*
*getValue(int x, int y)对外开放的接口，
用于获取求解之后在x，y位置的值，若是目
前没有得到解，那么返回-1
*
*/
int SudokuDlx::getValue(const int x, const int y){
	if (isSolved) return target[x * __size__ + y];
	else return -1;
}

void SudokuDlx::link(const int r, const int c){
	++S[Col[++size] = c];
	Row[size] = r;
	D[size] = D[c];
	U[D[c]] = size;
	U[size] = c;
	D[c] = size;
	if (H[r] < 0) H[r] = L[size] = R[size] = size;
	else{
		R[size] = R[H[r]];
		L[R[H[r]]] = size;
		L[size] = H[r];
		R[H[r]] = size;
	}
}

void SudokuDlx::remove(int c){
	L[R[c]] = L[c];
	R[L[c]] = R[c];
	for (int i = D[c]; i != c; i = D[i]){
		for (int j = R[i]; j != i; j = R[j]){
			U[D[j]] = U[j];
			D[U[j]] = D[j];
			--S[Col[j]];
		}
	}
}

void SudokuDlx::resume(int c){
	for (int i = U[c]; i != c; i = U[i]){
		for (int j = L[i]; j != i; j = L[j]){
			++S[Col[U[D[j]] = D[U[j]] = j]];
		}
	}
	L[R[c]] = R[L[c]] = c;
}

bool SudokuDlx::dance(int dep){
	if (R[0] == 0){
		ansd = dep;
		return true;
	}
	int c = R[0];
	for (int i = R[0]; i != 0; i = R[i]){
		if (S[i] < S[c]) c = i;
	}
	remove(c);
	for (int i = D[c]; i != c; i = D[i]){
		Ans[dep] = Row[i];
		for (int j = R[i]; j != i; j = R[j]) remove(Col[j]);
		if (dance(dep + 1)) return true;
		for (int j = L[i]; j != i; j = L[j]) resume(Col[j]);
	}
	resume(c);
	return false;
}

SudokuDlx::SudokuDlx(const int _size_){
	__size__ = _size_;
	int sqt = (int)sqrt(__size__);
	__size__ = sqt * sqt;
	MaxM = __size__ * __size__ * 4 + 5;
	MaxN = __size__ * __size__ * __size__ + 5;
	maxnode = MaxN * 4 + 10;
	L = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(L)) exit(1);
	R = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(R)) exit(1);
	U = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(U)) exit(1);
	D = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(D)) exit(1);
	Col = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(Col)) exit(1);
	Row = (int*)malloc(maxnode * sizeof(int));
	if (false == hasSpace(Row)) exit(1);
	H = (int*)malloc(MaxN * sizeof(int));
	if (false == hasSpace(H)) exit(1);
	S = (int*)malloc(MaxM * sizeof(int));
	if (false == hasSpace(S)) exit(1);
	Ans = (int*)malloc(MaxN * sizeof(int));
	if (false == hasSpace(Ans)) exit(1);
	target = (int*)malloc((__size__ * __size__ + 10) * sizeof(int*));
	if (false == hasSpace(target)) exit(1);
}

void SudokuDlx::reSet(){
	memset(L, 0, maxnode * sizeof(int));
	memset(R, 0, maxnode * sizeof(int));
	memset(U, 0, maxnode * sizeof(int));
	memset(D, 0, maxnode * sizeof(int));
	memset(Col, 0, maxnode * sizeof(int));
	memset(Row, 0, maxnode * sizeof(int));
	memset(H, 0, MaxN * sizeof(int));
	memset(S, 0, MaxM * sizeof(int));
	memset(Ans, 0, MaxN * sizeof(int));
	memset(target, 0, sizeof(int) * (__size__ * __size__ + 10));
	int m = __size__ * __size__ * 4;
	int n = __size__ * __size__ * __size__;
	for (int i = 0; i <= m; ++i){
		S[i] = 0;
		U[i] = D[i] = i;
		L[i] = i - 1;
		R[i] = i + 1;
	}
	R[m] = 0; L[0] = m;
	size = m, isSolved = false;
	for (int i = 0; i <= n; ++i) H[i] = -1;
}

SudokuDlx::~SudokuDlx(){
	free(L);
	free(R);
	free(U);
	free(D);
	free(S);
	free(H);
	free(Ans);
	free(Col);
	free(Row);
	free(target);
}

void SudokuDlx::insert(int x, int y, int val){
	int N = __size__;
	int sqt = (int)sqrt(N);
	int line = (x * N + y) * N + val;
	link(line, x * N + y + 1);
	link(line, N * N + x * N + val);
	link(line, N * N * 2 + y * N + val);
	link(line, N * N * 3 + (x / sqt * sqt + y / sqt) * N + val);
}

void SudokuDlx::outPutCheckBoard(\
	const char *filename, bool isPrintScreen){
	std::fstream fs;
	fs.open(filename, std::fstream::out);
	for (int i = 0; i < __size__; ++i){
		for (int j = 0; j < __size__; ++j){
			fs << this->getValue(i,j) << " ";
			if (isPrintScreen)
				std::cout << this->getValue(i, j) << " ";
		}
		fs << std::endl;
		if (isPrintScreen)
			std::cout << std::endl;
	}
	if (isPrintScreen)
		std::cout << std::endl;
	fs.close();
}