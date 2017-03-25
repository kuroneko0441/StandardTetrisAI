#include "Board.h"

double Board::evaluate(char** board, char curPiece, int x, int rotation, char nextPiece) {
	double thisQ = 0.0;
	char**newBoard = getNewBoard(board, curPiece, x, rotation);

	/* position error: piece escapes board */
	if(newBoard == nullptr) return -1000000;

	int sum_top = 0;
	int sum_hole = 0;
	int complete = 0;	/* completed line */
	int bumpiness = 0;	/* absolute value of height difference */

	/* saving new tops */
	int* newTop = new int[boardW];
	for(int x = 0; x < boardW; x++) {
		newTop[x] = 0;
		for(int y = boardH - 1; y >= 0; y--) {
			if(newBoard[y][x] == '1') { newTop[x] = (y + 1); break; }
		}
	}

	/* counting holes */
	for(int x = 0; x < boardW; x++) {
		for(int y = 0; y < newTop[x]; y++) {
			if(newBoard[y][x] == '0') { sum_hole++; }
		}
	}

	/* counting tops */
	for(int i = 0; i < boardW; i++) { sum_top += newTop[i]; }

	/* counting complete lines */
	for(int y = 0; y < boardH; y++) {
		bool flag = true;	/* flag for line complete*/
		for(int x = 0; x < boardW; x++) {
			/* breaks if line is not complete */
			if(newBoard[y][x] == '0') { flag = false; break; }
		}
		if(flag == true) { complete++; }
	}

	/* counting bumpiness */
	for(int i = 0; i < boardW - 1; i++) { bumpiness += ABS(newTop[i] - newTop[i + 1]); }

	/* weights from <Tetris AI - The (Near) Perfect Bot> */
	thisQ = -0.510066*sum_top + 0.760666*complete + -0.35663*sum_hole + -0.184483*bumpiness;

	if(newBoard != nullptr) {
		for(int i = 0; i < boardH; i++) { SAFE_DELETE(newBoard[i]); }
		SAFE_DELETE(newBoard);
	}
	SAFE_DELETE(newTop);

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

	try {
		/* set 1 at newBoard, new position */
		/* if access overflows return nullptr */
		switch(curPiece) {
		case 'O':
			if(x < 1 ||
			   MAX2(top[x - 1], top[x]) + 1 > boardH - 1) { throw true; }
			newBoard[MAX2(top[x - 1], top[x])][x - 1] = '1';
			newBoard[MAX2(top[x - 1], top[x])][x] = '1';
			newBoard[MAX2(top[x - 1], top[x]) + 1][x - 1] = '1';
			newBoard[MAX2(top[x - 1], top[x]) + 1][x] = '1';
			break;
		case 'I':
			if(rotation % 2 == 1) {
				if(x < 2 || x > boardW - 2 ||
				   MAX4(top[x - 2], top[x - 1], top[x], top[x + 1]) > boardH - 1) { throw true; }
				newBoard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x - 2] = '1';
				newBoard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x - 1] = '1';
				newBoard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x] = '1';
				newBoard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x + 1] = '1';
			} else if(rotation % 2 == 0) {
				if(top[x] + 3 > boardH - 1) { throw true; }
				newBoard[top[x]][x] = '1';
				newBoard[top[x] + 1][x] = '1';
				newBoard[top[x] + 2][x] = '1';
				newBoard[top[x] + 3][x] = '1';
			}
			break;
		case 'S':
			if(rotation % 2 == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1], top[x], top[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1] - 1)][x] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation % 2 == 0) {
				if(x > boardW - 2 ||
				   MAX2(top[x] - 1, top[x + 1]) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(top[x] - 1, top[x + 1])][x + 1] = '1';
				newBoard[MAX2(top[x] - 1, top[x + 1]) + 1][x] = '1';
				newBoard[MAX2(top[x] - 1, top[x + 1]) + 1][x + 1] = '1';
				newBoard[MAX2(top[x] - 1, top[x + 1]) + 2][x] = '1';
			}
			break;
		case 'Z':
			if(rotation % 2 == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1])][x] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1])][x + 1] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1][x] = '1';
			} else if(rotation % 2 == 0) {
				if(x > boardW - 2 ||
				   MAX2(top[x], top[x + 1] - 1) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(top[x], top[x + 1] - 1)][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 2][x + 1] = '1';
			}
			break;
		case 'L':
			if(rotation == 1) {
				if(x<1 || x>boardW - 2 ||
				   MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1)][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(top[x], top[x + 1]) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(top[x], top[x + 1])][x] = '1';
				newBoard[MAX2(top[x], top[x + 1])][x + 1] = '1';
				newBoard[MAX2(top[x], top[x + 1]) + 1][x] = '1';
				newBoard[MAX2(top[x], top[x + 1]) + 2][x] = '1';
			} else if(rotation == 3) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x + 1] = '1';
			} else if(rotation == 4) {
				if(x < 1 ||
				   MAX2(top[x - 1] - 2, top[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(top[x - 1] - 2, top[x])][x] = '1';
				newBoard[MAX2(top[x - 1] - 2, top[x]) + 1][x] = '1';
				newBoard[MAX2(top[x - 1] - 2, top[x]) + 2][x - 1] = '1';
				newBoard[MAX2(top[x - 1] - 2, top[x]) + 2][x] = '1';
			}
			break;
		case 'J':
			if(rotation == 1) {
				if(x < 1 || x > boardW - 2 ||
				   MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1])][x + 1] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x - 1] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(top[x], top[x + 1] - 2) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(top[x], top[x + 1] - 2)][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 2) + 1][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 2) + 2][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 2) + 2][x + 1] = '1';
			} else if(rotation == 3) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x - 1] = '1';
			} else if(rotation == 4) {
				if(x <1 ||
				   MAX2(top[x - 1], top[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(top[x - 1], top[x])][x - 1] = '1';
				newBoard[MAX2(top[x - 1], top[x])][x] = '1';
				newBoard[MAX2(top[x - 1], top[x]) + 1][x] = '1';
				newBoard[MAX2(top[x - 1], top[x]) + 2][x] = '1';
			}
			break;
		case 'T':
			if(rotation == 1) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1)][x] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x - 1] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x + 1] = '1';
			} else if(rotation == 2) {
				if(x > boardW - 2 ||
				   MAX2(top[x], top[x + 1] - 1) + 2 > boardH - 1) { throw true; }
				newBoard[MAX2(top[x], top[x + 1] - 1)][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 1][x] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 1][x + 1] = '1';
				newBoard[MAX2(top[x], top[x + 1] - 1) + 2][x] = '1';
			} else if(rotation == 3) {
				if(x<1 || x > boardW - 2 ||
				   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { throw true; }
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
				newBoard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x] = '1';
			} else if(rotation == 4) {
				if(x <1 ||
				   MAX2(top[x - 1] - 1, top[x]) + 2>boardH - 1) { throw true; }
				newBoard[MAX2(top[x - 1] - 1, top[x])][x] = '1';
				newBoard[MAX2(top[x - 1] - 1, top[x]) + 1][x - 1] = '1';
				newBoard[MAX2(top[x - 1] - 1, top[x]) + 1][x] = '1';
				newBoard[MAX2(top[x - 1] - 1, top[x]) + 2][x] = '1';
			}
		}
	} catch(bool) {
		for(int i = 0; i < boardH; i++)
			SAFE_DELETE(newBoard[i]);
		SAFE_DELETE(newBoard);
		newBoard = nullptr;
	}

	return newBoard;
}

void Board::select(char curPiece, int*bestX, int* bestRotation, char nextPiece) {
	/* unusing codes -  rule based */
	{
		//int min = boardH;
		//int min_index = boardW;

		//switch(curPiece) {
		//case 'O':
		//	//À§Ä¡°¡ ¾øÀ» ¶§
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(MAX2(top[i], top[i + 1]) <= min) {
		//			min = MAX2(top[i], top[i + 1]);
		//			min_index = i - 1;
		//		}
		//	min = boardH;
		//	//±í¼÷ÇÑ ¸ðÇü 1
		//	for(int i = 0; i < boardW - 1; i++)
		//		if((i == 0 || top[i - 1] > (top[i] > top[i + 1] ? top[i] : top[i + 1])) &&
		//			(i == boardW - 2 || top[i + 2] > (top[i] > top[i + 1] ? top[i] : top[i + 1])) &&
		//		   (top[i] > top[i + 1] ? top[i] : top[i + 1]) + 1 <= min) {
		//			min = (top[i] > top[i + 1] ? top[i] : top[i + 1]) + 1;
		//			min_index = i - 1;
		//		}
		//	//2Ä­ °ø¹é (¸ðÇü 1)
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//		}
		//	break;
		//case 'I':
		//	//1Ä­ ±¸¸Û
		//	for(int i = 0; i < boardW; i++) {
		//		if((top[i] + 1 == top[i + 1] || top[i] + 1 == top[i - 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	}
		//	//2Ä­ ±¸¸Û
		//	for(int i = 0; i < boardW; i++) {
		//		if((top[i] + 2 == top[i + 1] || top[i] + 2 == top[i - 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	}
		//	//Æò¸é (¸ðÇü 1)
		//	for(int i = 0; i < boardW - 3; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] == top[i + 2] &&
		//		   top[i] == top[i + 3] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i;
		//			best[1] = 1;
		//		}
		//	//3Ä­ ÀÌ»ó ±¸¸Û
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
		//	//ÆòÆòÇÒ¶§ ¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] > top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//±í¼÷ÇÑ ¸ðÇü
		//	//for(int i = 0; i<boardW - 1; i++)
		//	//	if((i == 0 || top[i - 1] > top[i] + 2) &&
		//	//		(i == boardW - 2 || top[i + 2] > top[i + 1] + 2) &&
		//	//	   (top[i] > top[i + 1] ? top[i] : top[i + 1]) <= min) {
		//	//		min = (top[i] > top[i + 1] ? top[i] : top[i + 1]);
		//	//		min_index = i - 1;
		//	//	}
		//	//¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] - 1 &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//¸ðÇü 1
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
		//	//ÆòÆòÇÒ¶§ ¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] < top[i + 1] &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//°ø¹é ÀÖ´Â ¸ðÇü 1
		//	//for(int i = 0; i < boardW - 2; i++)
		//	//	if(top[i] == top[i + 2] + 1 &&
		//	//	   top[i + 1] > top[i + 2] &&
		//	//	   top[i + 2] <= min) {
		//	//		min = top[i + 2];
		//	//		min_index = i - 1;
		//	//		best[1] = 1;
		//	//	}
		//	//±í¼÷ÇÑ ¸ðÇü 2
		//	//for(int i=0;i<boardW-1;i++)
		//	//	if((i == 0 || top[i - 1] - 1 > (top[i] > top[i + 1]) ? top[i] : top[i + 1]) &&
		//	//		(i == board - 2 || top[i + 1] - 1 > (top[i] > top[i + 1]) ? top[i] : top[i + 1]) &&
		//	//	   top[i] < min) {
		//	//		min = top[i];
		//	//		min_index = i - 2;
		//	//		best[1] = 2;
		//	//	}
		//	//¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//¸ðÇü 1
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
		//	//±í¼÷ÇÑ ¸ðÇü 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] > top[i + 1] + 2 &&
		//			(i == boardW - 2 || top[i + 2] > top[i]) &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	min += 1;
		//	//2Ä­ Æò¸é (¸ðÇü 2)
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//3Ä­ Æò¸é (¸ðÇü 3)
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//¸ðÇü 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] + 2 &&
		//		   top[i + 1] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//¸ðÇü 1
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
		//	//±í¼÷ÇÑ ¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] > top[i] + 2 &&
		//			(i == 0 || top[i - 1] > top[i + 1]) &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	min += 1;
		//	//¸ðÇü 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//¸ðÇü 3
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//¸ðÇü 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] - 1 &&
		//		   top[i + 2] <= min) {
		//			min = top[i + 2];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	//¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 2 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	break;
		//case 'T':
		//	//¸ðÇü 2
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i + 1] == top[i] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 2;
		//			best[1] = 2;
		//		}
		//	//±í¼÷ÇÑ ¸ðÇü 2
		//	//for(int i = 0; i < boardW - 1; i++)
		//	//	if(top[i + 1] > top[i] + 4 &&
		//	//	   (i==0||top[i-1]>top[i+1]) &&
		//	//	   (i==boardW-2||top[i+2>top[i+1]]) &&
		//	//	   top[i]+1 <= min) {
		//	//		min = top[i]+1;
		//	//		min_index = i - 2;
		//	//		best[1] = 2;
		//	//	}
		//	//¸ðÇü 4
		//	for(int i = 0; i < boardW - 1; i++)
		//		if(top[i] == top[i + 1] + 1 &&
		//		   top[i] <= min) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 4;
		//		}
		//	//¸ðÇü 3
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] == top[i + 1] &&
		//		   top[i + 2] == top[i + 1] &&
		//		   top[i] <= min) {
		//			min = top[i];
		//			min_index = i - 1;
		//			best[1] = 3;
		//		}
		//	//±í¼÷ÇÑ ¸ðÇü 1
		//	for(int i = 0; i < boardW - 2; i++)
		//		if(top[i] >= top[i + 1] + 2 &&
		//		   top[i + 2] == top[i] &&
		//		   top[i + 1] <= min - 4) {
		//			min = top[i + 1];
		//			min_index = i - 1;
		//			best[1] = 1;
		//		}
		//	//¸ðÇü 1
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

	/* saving tops */
	this->top = new int[boardW];
	for(int x = 0; x < boardW; x++) {
		top[x] = 0;
		for(int y = boardH - 1; y >= 0; y--) {
			if(this->board[y][x] == '1') { top[x] = (y + 1); break; }
		}
	}
}
Board::~Board() {
	for(int i = 0; i < boardW; i++)
		SAFE_DELETE(board[i]);
	SAFE_DELETE(board);

	SAFE_DELETE(top);
}
