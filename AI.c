#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "functions.h"
#include <stdlib.h>
#define DECLSPEC_EXPORT __declspec(dllexport)
#define WINAPI __stdcall


char gName[64];

char gPieceBitmap[8][4][4][4] =
{
	// NULL
	{
		{
			{0},
		},
	},
	// O
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
	},
	// I
	{
		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
		},
		{
			{0, 0, 0, 0},
			{1, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
		},
	},
	// S
	{
		{
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 0},
			{0, 0, 1, 1},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0},
		},
	},
	// Z
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 1},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
	},
	// L
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 1, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 0, 1},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
	},
	// J
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 1},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 1, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 0, 0},
		},
	},
	// T
	{
		{
			{0, 0, 0, 0},
			{0, 1, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 0, 1, 1},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 1, 1, 1},
			{0, 0, 0, 0},
			{0, 0, 0, 0},
		},
		{
			{0, 0, 1, 0},
			{0, 1, 1, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 0},
		},
	},
};

#ifdef __cplusplus
extern "C" {
#endif

// AI�̸� ǥ�ø� ���� ����
// ������ �й����� �� ��.
DECLSPEC_EXPORT
char* WINAPI Name()
{
	strcpy(gName, "Test");
	return gName;
}

/*
 * board�� boardW*boardH�� ������ ���ڿ��� �̷���� �ִ�.
 * ���� ���, 8*3ũ���� ��죺
 * 00000000
 * 00011001
 * 01111111
 * board�� ������ ���� �־����� ��"011111110001100100000000".
 *
 * curPiece�� ������ ������� "OISZLJT" �� �ϳ��� ���ڿ��� �־�����.
 * nextPiece�� �̸����⸦ ����� ��� OISZLJT �� �ϳ��� �׷��� ���� ��� ������ �־�����.
 * curR�� ���� ������ ������ ��Ÿ���� 1�� �ʱ� ���⣬2�� 90�� ȸ����3��180�� ȸ����4�� 270�� ȸ���� ���̴�.
 * curX,curY�� 4*4 �������� 1, 2�� ��ġ�� �������� ������ ���� ��ġ�� ��Ÿ���� ��ǥ���̴�.
 *		board���� x=1,y=1�� ���� �ϴ�,
 *		board���� x=boardW,y=1�� ������ �ϴ�,
 *		board���� x=1,y=boardH�� ���� ����̴�.
 *		���丮�� pieces_orientations.jpg ���� ����.
 *		�⺻ �ʱ� ��ġ�� curY==boardH-1, curX==boardW/2+1�̴�.
 *
 * bestX,bestRotation �� �����ϸ� �ȴ�. (curX,curR�� ���� ��Ģ�� ����)
 *
 * ������ȸ���� ���� �̷������ ���� x��ǥ�� �̵��� �� ����Ʈ����.
 *		�� �Լ��� ���ϴ� 1ȸ ȣ�� �ȴ�.
 */
int AI(int boardW, int boardH,
	   char board[],
	   char curPiece,
	   int curX, int curY,
	   int curR,
	   char nextPiece,
	   int* bestX, int* bestRotation) {
	char**board2D = convert2D(board, boardW, boardH);

	select(board2D, boardW, boardH, curPiece, bestX, bestRotation, nextPiece);

	for (int i = 0; i < boardH; i++)
		free(board2D[i]);
	free(board2D);

	return 0;
}

/*
 * path�� �����ϴ� ���ɾ ���� ������
 *      'l': �� ĭ �������� �̵�
 *      'r': �� ĭ ���������� �̵�
 *      'd': �� ĭ �Ʒ��� �̵�
 *      'L': ������ ���� �������� �̵�
 *      'R': ������ ���� ���������� �̵�
 *      'D': ������ ���� �Ʒ��� �̵����� �� �߰� �̵� ���ɣ�
 *      'z': �� �ð�������� ȸ��
 *      'c': �ð�������� ȸ��
 * path ���ڿ� ����'\0'�� �ϵ� ������ ��Ÿ����.
 *
 * ���� ���ɾ �̿��Ͽ� ������ �̵��� �����ϸ� �Ʒ��� ��ü �ڵ带 �����Ͽ��� �Ѵ�.
 */
DECLSPEC_EXPORT
int WINAPI AIPath(int boardW, int boardH,
		char board[],
		char curPiece,
		int curX, int curY,
		int curR,
		char nextPiece,
		char path[] )
{
	//if (0)
	{
		int bestX = 0;
		int bestRotation = 1;
		AI(boardW, boardH,
			board,
			curPiece, 
			curX, curY,
			curR,
			nextPiece,
			&bestX, &bestRotation
			);
		while ( curR != bestRotation )
		{
			strcat(path, "z");
			curR = curR % 4 + 1;
		}
		while ( curX > bestX )
		{
			strcat(path, "l");
			curX -= 1;
		}
		while ( curX < bestX )
		{
			strcat(path, "r");
			curX += 1;
		}

	}
	//strcpy(path, "ddddddddddcccrrrrrrDL");
	return 0;
}

#ifdef __cplusplus
}
#endif