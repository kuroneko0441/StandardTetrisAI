#pragma once
#define ABS(x) ((x)>0?(x):-(x))
#define MAX2(x,y) ((x)>(y)?(x):(y))
#define MAX3(x,y,z) MAX2(MAX2((x),(y)),(z))
#define MAX4(w,x,y,z) MAX3(MAX2((w),(x)),(y),(z))
#define SAFE_DELETE(ptr) if((ptr)!=nullptr) delete[]ptr

extern double w_height;
extern double w_hole;
extern double w_blockade;
extern double w_clear;
extern double w_edge;
extern double w_wall;
extern double w_floor;

class Board
{
private:
	char**board;
	const int boardW;
	const int boardH;
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
void initialize(double w[]);