#include <iostream>
#include "Board.h"

double Board::evaluate(const int x, const int rotation) {
	char**newBoard = getNewBoard(x, rotation);
	if(newBoard == nullptr) return -1000000;

	int sum_top = 0;
	int sum_hole = 0;
	int complete = 0;
	int bumpiness = 0;

	int* newTop = new int[boardW];
	memset(newTop, 0, boardW * sizeof(int));
	for(int x = 0; x < boardW; x++) {
		for(int y = boardH - 1; y >= 0; y--) {
			if(newBoard[y][x] == '1') { newTop[x] = (y + 1); break; }
		}
	}
	for(int x = 0; x < boardW; x++) {
		for(int y = 0; y < newTop[x]; y++) {
			if(newBoard[y][x] == '0') { sum_hole++; }
		}
	}


	for(int i = 0; i < boardW; i++) { sum_top += newTop[i]; }

	for(int y = 0; y < boardH; y++) {
		bool flag = true;
		for(int x = 0; x < boardW; x++) { if(newBoard[y][x] == '0') flag = false; }
		if(flag == true) { complete++; }
	}

	for(int i = 0; i < boardW - 1; i++) { bumpiness += abs(newTop[i] - newTop[i + 1]); }

	if(newBoard != nullptr) {
		for(int i = 0; i < boardH; i++) { SAFE_DELETE(newBoard[i]); }
		SAFE_DELETE(newBoard);
	}
	SAFE_DELETE(newTop);
	return -0.510066*sum_top + 0.760666*complete + -0.35663*sum_hole + -0.184483*bumpiness;
}

char ** Board::getNewBoard(int x, int rotation) {
	bool flag = true;
	char** newboard = new char*[boardH];
	for(int i = 0; i < boardH; i++) {
		newboard[i] = new char[boardW];
		for(int j = 0; j < boardW; j++)
			newboard[i][j] = board[i][j];
	}

	/* �� ������ ��ġ�� �°� newBoard�� ������ */
	//TODO top ��ġ y�� ������ ����
	switch(curPiece) {
	case 'O':
		if(x < 1 ||
		   MAX2(top[x - 1], top[x]) + 1 > boardH - 1) { flag = false; break; }
		newboard[MAX2(top[x - 1], top[x])][x - 1] = '1';
		newboard[MAX2(top[x - 1], top[x])][x] = '1';
		newboard[MAX2(top[x - 1], top[x]) + 1][x - 1] = '1';
		newboard[MAX2(top[x - 1], top[x]) + 1][x] = '1';
		break;
	case 'I':
		if(rotation % 2 == 1) {
			if(x < 2 || x > boardW - 2 ||
			   MAX4(top[x - 2], top[x - 1], top[x], top[x + 1]) > boardH - 1) { flag = false; break; }
			newboard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x - 2] = '1';
			newboard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x - 1] = '1';
			newboard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x] = '1';
			newboard[MAX4(top[x - 2], top[x - 1], top[x], top[x + 1])][x + 1] = '1';
		} else if(rotation % 2 == 0) {
			if(top[x] + 4 > boardH - 1) { flag = false; break; }
			newboard[top[x] + 1][x] = '1';
			newboard[top[x] + 2][x] = '1';
			newboard[top[x] + 3][x] = '1';
			newboard[top[x] + 4][x] = '1';
		}
		break;
	case 'S':
		if(rotation % 2 == 1) {
			if(x < 1 || x > boardW - 2 ||
			   MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1], top[x], top[x + 1] - 1)][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1] - 1)][x] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1][x] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1] - 1) + 1][x + 1] = '1';
		} else if(rotation % 2 == 0) {
			if(x > boardW - 2 ||
			   MAX2(top[x] - 1, top[x + 1]) + 2 > boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x] - 1, top[x + 1])][x + 1] = '1';
			newboard[MAX2(top[x] - 1, top[x + 1]) + 1][x] = '1';
			newboard[MAX2(top[x] - 1, top[x + 1]) + 1][x + 1] = '1';
			newboard[MAX2(top[x] - 1, top[x + 1]) + 2][x] = '1';
		}
		break;
	case 'Z':
		if(rotation % 2 == 1) {
			if(x < 1 || x > boardW - 2 ||
			   MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1])][x] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1])][x + 1] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1][x - 1] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1]) + 1][x] = '1';
		} else if(rotation % 2 == 0) {
			if(x > boardW - 2 ||
			   MAX2(top[x], top[x + 1] - 1) + 2 > boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x], top[x + 1] - 1)][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 1][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 1][x + 1] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 2][x + 1] = '1';
		}
		break;
	case 'L':
		if(rotation == 1) {
			if(x<1 || x>boardW - 2 ||
			   MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1)][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x] = '1';
			newboard[MAX3(top[x - 1], top[x] - 1, top[x + 1] - 1) + 1][x + 1] = '1';
		} else if(rotation == 2) {
			if(x > boardW - 2 ||
			   MAX2(top[x], top[x + 1]) + 2 > boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x], top[x + 1])][x] = '1';
			newboard[MAX2(top[x], top[x + 1])][x + 1] = '1';
			newboard[MAX2(top[x], top[x + 1]) + 1][x] = '1';
			newboard[MAX2(top[x], top[x + 1]) + 2][x] = '1';
		} else if(rotation == 3) {
			if(x < 1 || x > boardW - 2 ||
			   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x + 1] = '1';
		} else if(rotation == 4) {
			if(x < 1 ||
			   MAX2(top[x - 1] - 2, top[x]) + 2>boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x - 1] - 2, top[x])][x] = '1';
			newboard[MAX2(top[x - 1] - 2, top[x]) + 1][x] = '1';
			newboard[MAX2(top[x - 1] - 2, top[x]) + 2][x - 1] = '1';
			newboard[MAX2(top[x - 1] - 2, top[x]) + 2][x] = '1';
		}
		break;
	case 'J':
		if(rotation == 1) {
			if(x < 1 || x > boardW - 2 ||
			   MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1])][x + 1] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x - 1] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x] - 1, top[x + 1]) + 1][x + 1] = '1';
		} else if(rotation == 2) {
			if(x > boardW - 2 ||
			   MAX2(top[x], top[x + 1] - 2) + 2 > boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x], top[x + 1] - 2)][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 2) + 1][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 2) + 2][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 2) + 2][x + 1] = '1';
		} else if(rotation == 3) {
			if(x<1 || x > boardW - 2 ||
			   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x - 1] = '1';
		} else if(rotation == 4) {
			if(x <1 ||
			   MAX2(top[x - 1], top[x]) + 2>boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x - 1], top[x])][x - 1] = '1';
			newboard[MAX2(top[x - 1], top[x])][x] = '1';
			newboard[MAX2(top[x - 1], top[x]) + 1][x] = '1';
			newboard[MAX2(top[x - 1], top[x]) + 2][x] = '1';
		}
		break;
	case 'T':
		if(rotation == 1) {
			if(x<1 || x > boardW - 2 ||
			   MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1)][x] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x - 1] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x] = '1';
			newboard[MAX3(top[x - 1] - 1, top[x], top[x + 1] - 1) + 1][x + 1] = '1';
		} else if(rotation == 2) {
			if(x > boardW - 2 ||
			   MAX2(top[x], top[x + 1] - 1) + 2 > boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x], top[x + 1] - 1)][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 1][x] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 1][x + 1] = '1';
			newboard[MAX2(top[x], top[x + 1] - 1) + 2][x] = '1';
		} else if(rotation == 3) {
			if(x<1 || x > boardW - 2 ||
			   MAX3(top[x - 1], top[x], top[x + 1]) + 1 > boardH - 1) { flag = false; break; }
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x - 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1])][x + 1] = '1';
			newboard[MAX3(top[x - 1], top[x], top[x + 1]) + 1][x] = '1';
		} else if(rotation == 4) {
			if(x <1 ||
			   MAX2(top[x - 1] - 1, top[x]) + 2>boardH - 1) { flag = false; break; }
			newboard[MAX2(top[x - 1] - 1, top[x])][x] = '1';
			newboard[MAX2(top[x - 1] - 1, top[x]) + 1][x - 1] = '1';
			newboard[MAX2(top[x - 1] - 1, top[x]) + 1][x] = '1';
			newboard[MAX2(top[x - 1] - 1, top[x]) + 2][x] = '1';
		}
	}
	if(flag == false) {
		for(int i = 0; i < boardH; i++)
			SAFE_DELETE(newboard[i]);
		SAFE_DELETE(newboard);
		return nullptr;
	}

	return newboard;
}

int * Board::select() {
	int*best = new int[2];
	best[0] = 0;
	best[1] = 1;
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

	double bestQ = -1000000;
	for(int i = 0; i < boardW; i++) {
		for(int j = 1; j < 5; j++) {
			double Q = evaluate(i, j);
			if(Q > bestQ) {
				bestQ = Q;
				best[0] = i;
				best[1] = j;
			}
		}
	}
	return best;
}

Board::Board(const char board[], const int boardW, const int boardH, const int curPiece, const int nextPiece)
	:boardW(boardW), boardH(boardH), curPiece(curPiece), nextPiece(nextPiece) {
	this->board = new char*[boardH];
	for(int y = 0; y < boardH; y++) {
		this->board[y] = new char[boardW];
		for(int x = 0; x < boardW; x++)
			this->board[y][x] = board[y*boardW + x];
	}

	this->top = new int[boardW];
	memset(top, 0, boardW * sizeof(int));
	for(int i = 0; i < boardH; i++)
		for(int j = 0; j < boardW; j++)
			if(board[i*boardW + j] == '1')
				top[j] = (i + 1);

	this->hole = new int[boardW];
	memset(hole, 0, boardW * sizeof(int));
	for(int x = 0; x < boardW; x++)
		for(int y = 0; y < top[x]; y++)
			if(this->board[y][x] == '0')
				hole[x]++;
}

Board::~Board() {
	for(int i = 0; i < boardW; i++)
		SAFE_DELETE(board[i]);
	SAFE_DELETE(board);

	SAFE_DELETE(top);
	SAFE_DELETE(hole);
}