#include "Board.h"

double w_height;
double w_hole;
double w_blockade;
double w_clear;
double w_edge;
double w_wall;
double w_floor;

int * Board::getHeight(char ** board) {
	int* height = new int[boardW];
	for(int x = 0; x < boardW; x++) {
		height[x] = 0;
		for(int y = boardH - 1; y >= 0; y--) {
			if(board[y][x] == '1') { height[x] = (y + 1); break; }
		}
	}
	return height;
}
int* Board::getHole(char** board,int* height) {
	int* hole = new int[boardW];
	for(int x = 0; x < boardW; x++) {
		hole[x] = 0;
		for(int y = 0; y < height[x]; y++) {
			if(board[y][x] == '0') { hole[x]++; }
		}
	}
	return hole;
}

int Board::getBlockade(char** board,int*height, int* hole) {
	int sum=0;
	for (int x = 0; x < boardW; x++) {
		if (hole[x] == 0) { continue; }

		for (int y = height[x] - 1, thisHole = 0; y >= 0 && thisHole < hole[x]; y--) {
			if (board[y][x] == '1') sum++;
			else thisHole++;
		}
	}

	return sum;
}
int Board::getEdge(char** board) {
	int sum = 0;
	for (int y = 0; y < boardH; y++) {
		for (int x = 0; x < boardW; x++) {
			if (board[y][x] == '1') {
				if (x != boardW - 1 && board[y][x + 1] == '1') { sum++; }
				if (y != boardH - 1 && board[y + 1][x] == '1') { sum++; }
			}
		}
	}
	return sum;
}

int Board::getWall(char ** board)
{
	int sum = 0;
	for (int y = 0; y < boardH; y++) {
		if (board[y][0] == '1') { sum++; }
		if (board[y][boardW - 1] == '1') { sum++; }
	}
	return sum;
}

int Board::getFloor(char ** board)
{
	int sum = 0;
	for (int x = 0; x < boardW; x++) {
		if (board[0][x] == '1') { sum++; }
	}
	return sum;
}

double Board::evaluate(char** board, char curPiece, int x, int rotation, char nextPiece) {
	double thisQ = 0.0;
	char**newBoard = getNewBoard(board, curPiece, x, rotation);

	/* position error: piece escapes board */
	if(newBoard == nullptr) return -1000000;

	int* newHeight = getHeight(newBoard);
	int* newHole = getHole(newBoard, newHeight);

	int sum_height = 0;
	int sum_hole = 0;
	int sum_blockade =getBlockade(newBoard,newHeight,newHole);
	int sum_clear = 0;
	int sum_edge = getEdge(newBoard);
	int sum_wall = getWall(newBoard);		/* blocks touching the wall */
	int sum_floor = getFloor(newBoard);		/* blocks touching the floor */

	/* counting heights */
	for(int i = 0; i < boardW; i++) { sum_height += newHeight[i]; }

	/* counting holes */
	for(int i = 0; i < boardW; i++) { sum_hole += newHole[i]; }

	/* counting sum_clear lines */
	for(int y = 0; y < boardH; y++) {
		bool flag = true;	/* flag for line sum_clear*/
		for(int x = 0; x < boardW; x++) {
			if(newBoard[y][x] == '0') { flag = false; break; }
		}
		if(flag == true) { sum_clear++; }
	}

	thisQ = sum_height*w_height + sum_hole*w_hole + sum_blockade*w_blockade + sum_clear*w_clear +
		sum_edge*w_edge + sum_wall*w_wall + sum_floor*w_floor;

	if(newBoard != nullptr) {
		for(int i = 0; i < boardH; i++) { SAFE_DELETE(newBoard[i]); }
		SAFE_DELETE(newBoard);
	}

	SAFE_DELETE(newHeight);
	SAFE_DELETE(newHole);

	return thisQ;
}

char ** Board::getNewBoard(char** board, char curPiece, int x, int rotation) {
	/* new board after this piece falls */
	char** newBoard = new char*[boardH];
	for(int i = 0; i < boardH; i++) {
		newBoard[i] = new char[boardW];
		for(int j = 0; j < boardW; j++)
			newBoard[i][j] = board[i][j];
	}
	int*height = getHeight(newBoard);

	try {
		/* set 1 at newBoard, new position */
		/* if access overflows return nullptr */
		switch(curPiece) {
		case 'O':
			if(x < 1 ||
			   MAX2(height[x - 1], height[x]) + 1 > boardH - 1) { throw true; }
			newBoard[MAX2(height[x - 1], height[x])][x - 1] = '1';
			newBoard[MAX2(height[x - 1], height[x])][x] = '1';
			newBoard[MAX2(height[x - 1], height[x]) + 1][x - 1] = '1';
			newBoard[MAX2(height[x - 1], height[x]) + 1][x] = '1';
			break;
		case 'I':
			if(rotation % 2 == 1) {
				if(x < 2 || x > boardW - 2 ||
				   MAX4(height[x - 2], height[x - 1], height[x], height[x + 1]) > boardH - 1) { throw true; }
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x - 2] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX4(height[x - 2], height[x - 1], height[x], height[x + 1])][x + 1] = '1';
			} else if(rotation % 2 == 0) {
				if(height[x] + 3 > boardH - 1) { throw true; }
				newBoard[height[x]][x] = '1';
				newBoard[height[x] + 1][x] = '1';
				newBoard[height[x] + 2][x] = '1';
				newBoard[height[x] + 3][x] = '1';
			}
			break;
		case 'S':
			if(rotation % 2 == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation % 2 == 0) {
				if(x > boardW - 2 ||
				   MAX2(height[x] - 1, height[x + 1]) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(height[x] - 1, height[x + 1])][x + 1] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 1][x] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 1][x + 1] = '1';
				newBoard[MAX2(height[x] - 1, height[x + 1]) + 2][x] = '1';
			}
			break;
		case 'Z':
			if(rotation % 2 == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1]) + 1][x] = '1';
			} else if(rotation % 2 == 0) {
				if(x > boardW - 2 ||
				   MAX2(height[x], height[x + 1] - 1) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 2][x + 1] = '1';
			}
			break;
		case 'L':
			if(rotation == 1) {
				if(x<1 || x>boardW - 2 ||
				   MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1], height[x] - 1, height[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(height[x], height[x + 1]) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(height[x], height[x + 1])][x] = '1';
				newBoard[MAX2(height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1]) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1]) + 2][x] = '1';
			} else if(rotation == 3) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x + 1] = '1';
			} else if(rotation == 4) {
				if(x < 1 ||
				   MAX2(height[x - 1] - 2, height[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(height[x - 1] - 2, height[x])][x] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 2][x - 1] = '1';
				newBoard[MAX2(height[x - 1] - 2, height[x]) + 2][x] = '1';
			}
			break;
		case 'J':
			if(rotation == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x] - 1, height[x + 1]) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(height[x], height[x + 1] - 2) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(height[x], height[x + 1] - 2)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 2][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 2) + 2][x + 1] = '1';
			} else if(rotation == 3) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x - 1] = '1';
			} else if(rotation == 4) {
				if(x <1 ||
				   MAX2(height[x - 1], height[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(height[x - 1], height[x])][x - 1] = '1';
				newBoard[MAX2(height[x - 1], height[x])][x] = '1';
				newBoard[MAX2(height[x - 1], height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1], height[x]) + 2][x] = '1';
			}
			break;
		case 'T':
			if(rotation == 1) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(height[x - 1] - 1, height[x], height[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(height[x], height[x + 1] - 1) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(height[x], height[x + 1] - 1)][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(height[x], height[x + 1] - 1) + 2][x] = '1';
			} else if(rotation == 3) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(height[x - 1], height[x], height[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x - 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1])][x + 1] = '1';
				newBoard[MAX3(height[x - 1], height[x], height[x + 1]) + 1][x] = '1';
			} else if(rotation == 4) {
				if(x <1 ||
				   MAX2(height[x - 1] - 1, height[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(height[x - 1] - 1, height[x])][x] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 1][x - 1] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 1][x] = '1';
				newBoard[MAX2(height[x - 1] - 1, height[x]) + 2][x] = '1';
			}
		}
	} catch(bool) {
		for(int i = 0; i < boardH; i++)
			SAFE_DELETE(newBoard[i]);
		SAFE_DELETE(newBoard);
		newBoard = nullptr;
	}

	SAFE_DELETE(height);

	return newBoard;
}

void Board::select(char curPiece, int*bestX, int* bestRotation, char nextPiece) {
	/* unusing codes -  rule based */
	{
		//int min = boardH;
		//int min_index = boardW;

		//switch(curPiece) {
		//case 'O':
		//	//��ġ�� ���� ��
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(MAX2(top[i], top[i + 1]) <= min) {
		//			min = MAX2(top[i], top[i + 1]);
		//			min_index = i - 1;
		//		}
		//	min = boardH;
		//	//������ ���� 1
		//	for(int i = 0; i < boardW - 1; i++)
		//		if((i == 0 || top[i - 1] > (top[i] > top[i + 1] ? top[i] : top[i + 1])) &&
		//			(i == boardW - 2 || top[i + 2] > (top[i] > top[i + 1] ? top[i] : top[i + 1])) &&
		//		   (top[i] > top[i + 1] ? top[i] : top[i + 1]) + 1 <= min) {
		//			min = (top[i] > top[i + 1] ? top[i] : top[i + 1]) + 1;
		//			min_index = i - 1;
		//		}
		//	//2ĭ ���� (���� 1)
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//		}
		//	break;
		//case 'I':
		//	//1ĭ ����
		//	for(int i = 0; i < boardW; i++) {
		//		if((top[i] + 1 == top[i + 1] || top[i] + 1 == top[i - 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	}
		//	//2ĭ ����
		//	for(int i = 0; i < boardW; i++) {
		//		if((top[i] + 2 == top[i + 1] || top[i] + 2 == top[i - 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	}
		//	//��� (���� 1)
		//	for(int i = 0; i < boardW - 3; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] == top[i + 2] &&
		//		   top[i] == top[i + 3] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i;
		//			best[1] = 1;
		//		}
		//	//3ĭ �̻� ����
		//	for(int i = 0; i < boardW; i++) {
		//		if((top[i] + 3 <= top[i + 1] || top[i] + 3 <= top[i - 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	}
		//	break;
		//case 'S':
		//	//�����Ҷ� ���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] > top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//������ ����
		//	//for(int i = 0; i<boardW - 1; i++)
		//	//	if((i == 0 || top[i - 1] > top[i] + 2) &&
		//	//		(i == boardW - 2 || top[i + 2] > top[i + 1] + 2) &&
		//	//	   (top[i] > top[i + 1] ? top[i] : top[i + 1]) <= min) {
		//	//		min = (top[i] > top[i + 1] ? top[i] : top[i + 1]);
		//	//		min_index = i - 1;
		//	//	}
		//	//���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] - 1 &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	break;
		//case 'Z':
		//	//�����Ҷ� ���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] < top[i + 1] &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//���� �ִ� ���� 1
		//	//for(int i = 0; i < boardW - 2; i++)
		//	//	if(top[i] == top[i + 2] + 1 &&
		//	//	   top[i + 1] > top[i + 2] &&
		//	//	   top[i + 2] <= min) {
		//	//		min = top[i + 2];
		//	//		min_index = i - 1;
		//	//		best[1] = 1;
		//	//	}
		//	//������ ���� 2
		//	//for(int i=0;i<boardW-1;i++)
		//	//	if((i == 0 || top[i - 1] - 1 > (top[i] > top[i + 1]) ? top[i] : top[i + 1]) &&
		//	//		(i == board - 2 || top[i + 1] - 1 > (top[i] > top[i + 1]) ? top[i] : top[i + 1]) &&
		//	//	   top[i] < min) {
		//	//		min = top[i];
		//	//		min_index = i - 2;
		//	//		best[1] = 2;
		//	//	}
		//	//���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] + 1 &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	break;
		//case 'L':
		//	//������ ���� 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] > top[i + 1] + 2 &&
		//			(i == boardW - 2 || top[i + 2] > top[i]) &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	min += 1;
		//	//2ĭ ��� (���� 2)
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//3ĭ ��� (���� 3)
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//���� 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] + 2 &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] - 1 &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	break;
		//case 'J':
		//	//������ ���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] > top[i] + 2 &&
		//			(i == 0 || top[i - 1] > top[i + 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//���� 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//���� 3
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] - 1 &&
		//		   top[i + 2] <= min) {
		//			min = top[i + 2];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	//���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 2 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	break;
		//case 'T':
		//	//���� 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//������ ���� 2
		//	//for(int i = 0; i < boardW - 1; i++)
		//	//	if(top[i + 1] > top[i] + 4 &&
		//	//	   (i==0||top[i-1]>top[i+1]) &&
		//	//	   (i==boardW-2||top[i+2>top[i+1]]) &&
		//	//	   top[i]+1 <= min) {
		//	//		min = top[i]+1;
		//	//		min_index = i - 2;
		//	//		best[1] = 2;
		//	//	}
		//	//���� 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//���� 3
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//������ ���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] >= top[i + 1] + 2 &&
		//		   top[i + 2] == top[i] &&
		//		   top[i + 1] <= min - 4) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	//���� 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] + 1 &&
		//		   top[i + 2] == top[i] &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	break;
		//default:
		//	min_index = 7;
		//	break;
		//}

		//best[0] = min_index;
	}

	double bestQ = -1000000.0;
	for(int i = 0; i < boardW; i++) {	/* for every Xs */
		for(int j = 1; j < 5; j++) {	/* for every rotations */
			double Q = evaluate(board, curPiece, i, j, nextPiece);	/* find Max Q */
			if(Q > bestQ) {
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
	for(int y = 0; y < boardH; y++) {
		this->board[y] = new char[boardW];
		for(int x = 0; x < boardW; x++)
			this->board[y][x] = board[y*boardW + x];
	}
}
Board::~Board() {
	for(int i = 0; i < boardW; i++)
		SAFE_DELETE(board[i]);
	SAFE_DELETE(board);

}

void initialize(double w[])
{
	w_height = w[0];
	w_hole = w[1];
	w_blockade=w[2];
	w_clear = w[3];
	w_edge = w[4];
	w_wall = w[5];
	w_floor = w[6];
}
