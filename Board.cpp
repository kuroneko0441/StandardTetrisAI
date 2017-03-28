#include "Board.h"

int* Board::getHeight(char ** board)
{
	int*height = new int[boardW];
	for (int x = 0; x < boardW; x++) {
		height[x] = 0;
		for (int y = boardH - 1; y >= 0; y--) {
			if (board[y][x] == '1') { height[x] = (y + 1); break; }
		}
	}
	return height;
}

double Board::evaluate(char** board, char curPiece, int x, int rotation, char nextPiece) {
	double thisQ = 0.0;
	char**newBoard = getNewBoard(board, curPiece, x, rotation);

	/* position error: piece escapes board */
	if (newBoard == nullptr) return -1000000;

	int sum_top = 0;
	int sum_hole = 0;
	int complete = 0;	/* completed line */
	int bumpiness = 0;	/* absolute value of height difference */

	/* saving new tops */
	int* newHeight = new int[boardW];
	for (int x = 0; x < boardW; x++) {
		newHeight[x] = 0;
		for (int y = boardH - 1; y >= 0; y--) {
			if (newBoard[y][x] == '1') { newHeight[x] = (y + 1); break; }
		}
	}

	/* counting holes */
	for (int x = 0; x < boardW; x++) {
		for (int y = 0; y < newHeight[x]; y++) {
			if (newBoard[y][x] == '0') { sum_hole++; }
		}
	}

	/* counting tops */
	for (int i = 0; i < boardW; i++) { sum_top += newHeight[i]; }

	/* counting complete lines */
	for (int y = 0; y < boardH; y++) {
		bool flag = true;	/* flag for line complete*/
		for (int x = 0; x < boardW; x++) {
			/* breaks if line is not complete */
			if (newBoard[y][x] == '0') { flag = false; break; }
		}
		if (flag == true) { complete++; }
	}

	/* counting bumpiness */
	for (int i = 0; i < boardW - 1; i++) { bumpiness += ABS(newHeight[i] - newHeight[i + 1]); }

	/* weights from <Tetris AI - The (Near) Perfect Bot> */
	thisQ = -0.510066*sum_top + 0.760666*complete + -0.35663*sum_hole + -0.184483*bumpiness;

	if (newBoard != nullptr) {
		for (int i = 0; i < boardH; i++) { SAFE_DELETE(newBoard[i]); }
		SAFE_DELETE(newBoard);
	}
	SAFE_DELETE(newHeight);

	return thisQ;
}

char ** Board::getNewBoard(char** board, char curPiece, int x, int rotation) {
	/* new board after this piece falls */
	char** newBoard = new char*[boardH];
	for (int i = 0; i < boardH; i++) {
		newBoard[i] = new char[boardW];
		for (int j = 0; j < boardW; j++)
			newBoard[i][j] = board[i][j];
	}
	int* height = getHeight(board);
	try {
		/* set 1 at newBoard, new position */
		/* if access overflows return nullptr */
		switch (curPiece) {
		case 'O':
			if (x < 1 ||
				MAX2(height[x - 1], height[x]) + 1 > boardH - 1) {
				throw true;
			}
			newBoard[MAX2(height[x - 1], height[x])][x - 1] = '1';
			newBoard[MAX2(height[x - 1], height[x])][x] = '1';
			newBoard[MAX2(height[x - 1], height[x]) + 1][x - 1] = '1';
			newBoard[MAX2(height[x - 1], height[x]) + 1][x] = '1';
			break;
		case 'I':
			if (rotation % 2 == 1) {
				if (x < 2 || x > boardW - 2 ||
					MAX4(height[x - 2], height[x - 1], height[x], height[x + 1]) > boardH - 1) {
					throw true;
				}
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x - 2] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x + 1] = '1';
			}
			else if (rotation % 2 == 0) {
				if (height[x] + 3 > boardH - 1) { throw true; }
				newBoard[height[x]][x] = '1';
				newBoard[height[x] + 1][x] = '1';
				newBoard[height[x] + 2][x] = '1';
				newBoard[height[x] + 3][x] = '1';
			}
			break;
		case 'S':
			if (rotation % 2 == 1) {
				if (x < 1 || x > boardW - 2 ||
					MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1][x + 1] = '1';
			}
			else if (rotation % 2 == 0) {
				if (x > boardW - 2 ||
					MAX2(height[x] - 1, height[x + 1]) + 2 > boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x] - 1, height[x + 1])][x + 1] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 1][x] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 1][x + 1] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 2][x] = '1';
			}
			break;
		case 'Z':
			if (rotation % 2 == 1) {
				if (x < 1 || x > boardW - 2 ||
					MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1][x] = '1';
			}
			else if (rotation % 2 == 0) {
				if (x > boardW - 2 ||
					MAX2(height[x], height[x + 1] - 1) + 2 > boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 2][x + 1] = '1';
			}
			break;
		case 'L':
			if (rotation == 1) {
				if (x<1 || x>boardW - 2 ||
					MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x + 1] = '1';
			}
			else if (rotation == 2) {
				if (x > boardW - 2 ||
					MAX2(height[x], height[x + 1]) + 2 > boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x], height[x + 1])][x] = '1';
				newBoard[MAX2(height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1]) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1]) + 2][x] = '1';
			}
			else if (rotation == 3) {
				if (x < 1 || x > boardW - 2 ||
					MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x + 1] = '1';
			}
			else if (rotation == 4) {
				if (x < 1 ||
					MAX2(height[x - 1] - 2, height[x]) + 2>boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x - 1] - 2, height[x])][x] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 2][x - 1] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 2][x] = '1';
			}
			break;
		case 'J':
			if (rotation == 1) {
				if (x < 1 || x > boardW - 2 ||
					MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x + 1] = '1';
			}
			else if (rotation == 2) {
				if (x > boardW - 2 ||
					MAX2(height[x], height[x + 1] - 2) + 2 > boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x], height[x + 1] - 2)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 2][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 2][x + 1] = '1';
			}
			else if (rotation == 3) {
				if (x<1 || x > boardW - 2 ||
					MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x - 1] = '1';
			}
			else if (rotation == 4) {
				if (x <1 ||
					MAX2(height[x - 1], height[x]) + 2>boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x - 1], height[x])][x - 1] = '1';
				newBoard[MAX2(height[x - 1], height[x])][x] = '1';
				newBoard[MAX2(height[x - 1], height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1], height[x]) + 2][x] = '1';
			}
			break;
		case 'T':
			if (rotation == 1) {
				if (x<1 || x > boardW - 2 ||
					MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x + 1] = '1';
			}
			else if (rotation == 2) {
				if (x > boardW - 2 ||
					MAX2(height[x], height[x + 1] - 1) + 2 > boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 2][x] = '1';
			}
			else if (rotation == 3) {
				if (x<1 || x > boardW - 2 ||
					MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) {
					throw true;
				}
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x] = '1';
			}
			else if (rotation == 4) {
				if (x <1 ||
					MAX2(height[x - 1] - 1, height[x]) + 2>boardH - 1) {
					throw true;
				}
				newBoard[MAX2(height[x - 1] - 1, height[x])][x] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 1][x - 1] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 2][x] = '1';
			}
		}
	}
	catch (bool) {
		for (int i = 0; i < boardH; i++)
			SAFE_DELETE(newBoard[i]);
		SAFE_DELETE(newBoard);
		newBoard = nullptr;
	}

	return newBoard;
}

void Board::select(char curPiece, int*bestX, int* bestRotation, char nextPiece) {
	double bestQ = -1000000.0;
	for (int i = 0; i < boardW; i++) {	/* for every Xs */
		for (int j = 1; j < 5; j++) {	/* for every rotations */
			double Q = evaluate(board, curPiece, i, j, nextPiece);	/* find Max Q */
			if (Q > bestQ) {
				bestQ = Q;
				*bestX = i + 1;
				*bestRotation = j;
			}
		}
	}
}

Board::Board(const char board[], const int boardW, const int boardH)
	:boardW(boardW), boardH(boardH) {
	/* convert 1d board to 2d board */
	this->board = new char*[boardH];
	for (int y = 0; y < boardH; y++) {
		this->board[y] = new char[boardW];
		for (int x = 0; x < boardW; x++)
			this->board[y][x] = board[y*boardW + x];
	}

}
Board::~Board() {
	for (int i = 0; i < boardW; i++)
		SAFE_DELETE(board[i]);
	SAFE_DELETE(board);
}
