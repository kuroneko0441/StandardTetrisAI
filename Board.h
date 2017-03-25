#pragma once
#define ABS(x) ((x)>0?(x):-(x))
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
	/* weights from <Coding a Tetris AI using a Genetic Algorithm> */
	const double w_height = -3.78;
	const double w_hole = -8.8;
	const double w_blockade = -0.59;
	const double w_clear = 8.2;
	const double w_edge = 3.7;
	const double w_wall = 2.5;
	const double w_floor = 4.0;
private:
	int* getHeight(char** board);
	/* count holes */
	int* getHole(char** board,int* height);
	/* count blockades */
	int getBlockade(char** board,int*height, int* hole);
	/* count edge between blocks */
	int getEdge(char** board);
	/* count blocks touching the wall */
	int getWall(char** board);
	/* count blocks touching the floor */
	int getFloor(char** board);
	/* evaluate current status with heights, holes, completed lines, bumpiness */
	double evaluate(char** board, char piece, int x, int rotation, char nextPiece = ' ');
	/* get next board status after current (piece, x, rotation) falls */
	char** getNewBoard(char** board, char piece, int x, int rotation);
public:
	void select(char curPiece, int*bestX, int* bestRotation, char nextPiece);
public:
	Board(const char board[], const int boardW, const int boardH);
	virtual ~Board();
};