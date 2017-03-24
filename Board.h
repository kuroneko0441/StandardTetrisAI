#pragma once
#define MAX2(x,y) ((x)>(y)?(x):(y))
#define MAX3(x,y,z) MAX2(MAX2((x),(y)),(z))
#define MAX4(w,x,y,z) MAX3(MAX2((w),(x)),(y),(z))
#define SAFE_DELETE(ptr) if((ptr)!=nullptr) delete[]ptr

class Board
{
private:
	char**board;
	const int boardW;
	const int boardH;
	const char curPiece;
	const char nextPiece;
	int*top;
	int*hole;
private:
	/* evaluate current status with tops, holes, completed lines, bumpiness */
	double evaluate(int x, int rotation);
	/* get next board status after current (piece, x, rotation) falls */
	char** getNewBoard(int x, int rotation);
public:
	void select(int*bestX, int* bestRotation);
public:
	Board(const char board[], const int boardW, const int boardH, const int curPiece, const int nextPiece);
	virtual ~Board();
};