#pragma once
#define ABS(x) ((x)>0?(x):-(x))
#define MAX2(x,y) ((x)>(y)?(x):(y))
#define MAX3(x,y,z) MAX2(MAX2((x),(y)),(z))
#define MAX4(w,x,y,z) MAX3(MAX2((w),(x)),(y),(z))

char** convert2D(char*board, int boardW, int boardH);
int* getHeight(char ** board, int boardW, int boardH);
double evaluate(char** board, int boardW, int boardH, char curPiece, int x, int rotation, char nextPiece);
char ** getNewBoard(char** board, int boardW, int boardH, char curPiece, int x, int rotation);
void select(char**board, int boardW, int boardH,char curPiece, int*bestX, int* bestRotation, char nextPiece);