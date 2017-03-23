#define _CRT_SECURE_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include "Board.h"
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

// AI이름 표시를 위해 지정
// 본인의 학번으로 할 것.
DECLSPEC_EXPORT
char* WINAPI Name()
{
	strcpy(gName, "Test");
	return gName;
}

/*
 * board는 boardW*boardH의 길이의 문자열로 이루어져 있다.
 * 예를 들어, 8*3크기의 경우：
 * 00000000
 * 00011001
 * 01111111
 * board에 다음과 같이 주어진다 ："011111110001100100000000".
 *
 * curPiece는 블럭의 모양으로 "OISZLJT" 중 하나의 문자열이 주어진다.
 * nextPiece는 미리보기를 사용할 경우 OISZLJT 중 하나가 그렇지 않을 경우 공백이 주어진다.
 * curR은 현재 블럭의 방향을 나타내며 1이 초기 방향，2는 90도 회전，3는180도 회전，4는 270도 회전된 것이다.
 * curX,curY는 4*4 블럭에서 1, 2의 위치를 기준으로 블럭의 현재 위치를 나타내는 좌표값이다.
 *		board에서 x=1,y=1은 왼쪽 하단,
 *		board에서 x=boardW,y=1은 오른쪽 하단,
 *		board에서 x=1,y=boardH은 왼쪽 상단이다.
 *		디렉토리에 pieces_orientations.jpg 파일 참고.
 *		기본 초기 위치는 curY==boardH-1, curX==boardW/2+1이다.
 *
 * bestX,bestRotation 을 지정하면 된다. (curX,curR과 같은 규칙을 따름)
 *
 * 참고：회전이 먼저 이루어지며 이후 x좌표를 이동한 후 떨어트린다.
 *		이 함수는 블록당 1회 호출 된다.
 */
int AI(int boardW, int boardH,
	   char board[],
	   char curPiece,
	   int curX, int curY,
	   int curR,
	   char nextPiece,
	   int* bestX, int* bestRotation) {
	Board b(board, boardW, boardH, curPiece, nextPiece);

	b.select(bestX,bestRotation);

	return 0;
}

/*
 * path를 지정하는 명령어에 대한 설명：
 *      'l': 한 칸 왼쪽으로 이동
 *      'r': 한 칸 오른쪽으로 이동
 *      'd': 한 칸 아래로 이동
 *      'L': 가능한 가장 왼쪽으로 이동
 *      'R': 가능한 가장 오른쪽으로 이동
 *      'D': 가능한 가장 아래로 이동（이 후 추가 이동 가능）
 *      'z': 반 시계방향으로 회전
 *      'c': 시계방향으로 회전
 * path 문자열 끝에'\0'이 하드 랜딩을 나타낸다.
 *
 * 위의 명령어를 이용하여 임의의 이동이 가능하며 아래의 전체 코드를 수정하여야 한다.
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
