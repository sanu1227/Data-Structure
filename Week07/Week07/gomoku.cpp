#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

void rowstreak(char board[][19]) {

	for (int i = 0;  i < 19; i++) {
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		int black_start = -1;
		int white_start = -1;
		int b_start_save = -1;
		int w_start_save = -1;
		for (int k = 0; k < 19; k++) {
			if (board[i][k] == '@') {
				if (black_connect == 0) {
					black_start = k;
				}
				black_connect++;
				white_connect = 0;

				if (black_connect > black_max) {
					black_max = black_connect;
					b_start_save = black_start;
				}
			}
			else if (board[i][k] == '#') {
				if (white_connect == 0) {
					white_start = k;
				}
				white_connect++;
				black_connect = 0;
				if (white_max < white_connect) {
					white_max = white_connect;
					w_start_save = white_start;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
		}
		if (white_max == 0 && black_max == 0) {
			printf("가로 %d | 연속된 돌 없음\n", i);
		}
		else if (white_max > black_max) {
			printf("가로 %d | 백돌 %d연속\n", i, white_max);
			printf("(%d,%d) ~ (%d,%d)\n", w_start_save, i, w_start_save + white_max-1, i);
		}
		else if (white_max < black_max) {
			printf("가로 %d | 흑돌 %d연속\n", i, black_max);
			printf("(%d, %d) ~ (%d, %d)\n", b_start_save, i, b_start_save + black_max-1, i);
		}
		else {
			printf("가로 %d | 연속된 돌의 수가 같습니다. (%d연속)\n", i, white_max);
			printf("b:(%d, %d) ~ (%d, %d), w:(%d, %d) ~ (%d, %d)\n", b_start_save, i, b_start_save + black_max-1, i, w_start_save, i, w_start_save + white_max-1, i);
		}

	}
}
void colstreak(char board[][19]) {

	for (int i = 0; i < 19; i++) {
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		for (int k = 0; k < 19; k++) {
			if (board[k][i] == '@') {
				black_connect++;
				white_connect = 0;
				if (black_connect > black_max) {
					black_max = black_connect;
				}
			}
			else if (board[k][i] == '#') {
				white_connect++;
				black_connect = 0;
				if (white_max < white_connect) {
					white_max = white_connect;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
		}
		if (white_max == 0 && black_max == 0) {
			printf("세로 %d | 연속된 돌 없음\n", i);
		}
		else if (white_max > black_max) {
			printf("세로 %d | 백돌 %d연속\n", i, white_max);
		}
		else if (white_max < black_max) {
			printf("세로 %d | 흑돌 %d연속\n", i, black_max);
		}
		else {
			printf("세로 %d | 연속된 돌의 수가 같습니다. (%d연속)\n", i, white_max);
		}

	}
}

void LTtoRBcross(char board[][19]) {
	for (int col = 0; col < 19; col++) {
		int i = 0;
		int k = col;
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		while (i < 19 && k < 19) {

			if (board[i][k] == '@') {
				black_connect++;
				white_connect = 0;
				if (black_connect > black_max) {
					black_max = black_connect;
				}
			}
			else if (board[i][k] == '#') {
				black_connect = 0;
				white_connect++;
				if (white_connect > white_max) {
					white_max = white_connect;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
			i++;
			k++;
		}
		if (white_max == 0 && black_max == 0) {
			printf("LT -> RB 대각선 시작점:(0,%d) | 연속된 돌 없음\n", col);
		}
		else if (white_max > black_max) {
			printf("LT -> RB 대각선 시작점:(0,%d) | 백돌 %d연속\n", col, white_max);
		}
		else if (white_max < black_max) {
			printf("LT -> RB 대각선 시작점:(0,%d) | 흑돌 %d연속\n", col, black_max);
		}
		else {
			printf("LT -> RB 대각선 시작점:(0,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n", col, white_max);
		}
	}

	for (int row = 1; row < 19; row++) {
		int i = row;
		int k = 0;
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		while (i < 19 && k < 19) {

			if (board[i][k] == '@') {
				black_connect++;
				white_connect = 0;
				if (black_connect > black_max) {
					black_max = black_connect;
				}
			}
			else if (board[i][k] == '#') {
				black_connect = 0;
				white_connect++;
				if (white_connect > white_max) {
					white_max = white_connect;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
			i++;
			k++;
		}
		if (white_max == 0 && black_max == 0) {
			printf("LT -> RB 대각선 시작점:(%d,0) | 연속된 돌 없음\n", row);
		}
		else if (white_max > black_max) {
			printf("LT -> RB 대각선 시작점:(%d,0) | 백돌 %d연속\n", row, white_max);
		}
		else if (white_max < black_max) {
			printf("LT -> RB 대각선 시작점:(%d,0) | 흑돌 %d연속\n", row, black_max);
		}
		else {
			printf("LT -> RB 대각선 시작점:(%d,0) | 연속된 돌의 수가 같습니다. (%d연속)\n", row, white_max);
		}
	}
}

void RTtoLBcross(char board[][19]) {
	for (int col = 18; col >= 0; col--) {
		int i = 0;
		int k = col;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		while (i < 19 && k >= 0) {

			if (board[i][k] == '@') {
				black_connect++;
				white_connect = 0;
				if (black_connect > black_max) {
					black_max = black_connect;
				}
			}
			else if (board[i][k] == '#') {
				black_connect = 0;
				white_connect++;
				if (white_connect > white_max) {
					white_max = white_connect;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
			i++;
			k--;
		}
		if (white_max == 0 && black_max == 0) {
			printf("RT -> LB 대각선 시작점:(0,%d) | 연속된 돌 없음\n", col);
		}
		else if (white_max > black_max) {
			printf("RT -> LB 대각선 시작점:(0,%d) | 백돌 %d연속\n", col, white_max);
		}
		else if (white_max < black_max) {
			printf("RT -> LB 대각선 시작점:(0,%d) | 흑돌 %d연속\n", col, black_max);
		}
		else {
			printf("RT -> LB 대각선 시작점:(0,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n", col, white_max);
		}
	}

	for (int row = 1; row < 19; row++) {
		int i = row;
		int k = 18;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;
		while (i < 19 && k >= 0) {

			if (board[i][k] == '@') {
				black_connect++;
				white_connect = 0;
				if (black_connect > black_max) {
					black_max = black_connect;
				}
			}
			else if (board[i][k] == '#') {
				black_connect = 0;
				white_connect++;
				if (white_connect > white_max) {
					white_max = white_connect;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}
			i++;
			k--;
		}
		if (white_max == 0 && black_max == 0) {
			printf("RT -> LB 대각선 시작점:(%d,0) | 연속된 돌 없음\n", row);
		}
		else if (white_max > black_max) {
			printf("RT -> LB 대각선 시작점:(%d,0) | 백돌 %d연속\n", row, white_max);
		}
		else if (white_max < black_max) {
			printf("RT -> LB 대각선 시작점:(%d,0) | 흑돌 %d연속\n", row, black_max);
		}
		else {
			printf("RT -> LB 대각선 시작점:(%d,0) | 연속된 돌의 수가 같습니다. (%d연속)\n", row, white_max);
		}
	}
}

int main() {
	char board[19][19];
	int x, y;
	char input[150];
	int turn = 0;
	int blackcount = 0;
	int whitecount = 0;
	int rowboard[2][19];
	int rowblack = 0;
	int rowwhite = 0;
	int colboard[2][19];
	int colblack = 0;
	int colwhite = 0;
	char extra;


	for (int i = 0; i < 19; i++) {
		for (int k = 0; k < 19; k++) {
			board[i][k] = '+';
		}
	}

	while (1) {

		printf("흑돌 수: %d , 백돌 수: %d \n", blackcount, whitecount);

		printf("돌을 놓으려는 좌표를 입력 하세요:\n(EX: x y, 0 0, 1 3)\n");
		fgets(input, sizeof(input), stdin);
		if (sscanf(input, "%d %d %c", &x, &y, &extra) != 2) {
			printf("잘못된 입력 입니다\n");
			continue;
		}
		system("cls");



		if (x >= 19 || x < 0 || y < 0 || y>=19) {
			printf("잘못된 위치 입니다");
		}
		else if (board[y][x] != '+') {
			printf("<<이미 돌이 놓여 있습니다>> \n \n");
		}
		else {
			if (turn == 0) {
				board[y][x] = '@';
				blackcount++;
			}
			else {
				board[y][x] = '#';
				whitecount++;
			}
			turn = 1 - turn;
		}


		for (int i = 0; i < 19; i++) {
			for (int k = 0; k < 19; k++) {
				printf("%c ", board[i][k]);
			}
			printf("\n");
		}

		for (int i = 0; i < 19; i++) {
			for (int k = 0; k < 19; k++) {
				if (board[i][k] == '@') {
					rowblack++;
				}
				else if (board[i][k] == '#') {
					rowwhite++;
				}

			}
			rowboard[0][i] = rowblack;
			rowboard[1][i] = rowwhite;
			rowblack = 0;
			rowwhite = 0;
		}

		for (int k = 0; k < 19; k++) {
			printf("가로 %d | 흑돌 수: %d, 백돌 수: %d\n", k, rowboard[0][k], rowboard[1][k]);
		}


		for (int i = 0; i < 19; i++) {
			for (int k = 0; k < 19; k++) {
				if (board[k][i] == '@') {
					colblack++;
				}
				else if (board[k][i] == '#') {
					colwhite++;
				}

			}
			colboard[0][i] = colblack;
			colboard[1][i] = colwhite;
			colblack = 0;
			colwhite = 0;
		}

		for (int k = 0; k < 19; k++) {
			printf("세로 %d | 흑돌 수: %d, 백돌 수: %d\n", k, colboard[0][k], colboard[1][k]);
		}
		rowstreak(board);
		colstreak(board);
		LTtoRBcross(board);
		RTtoLBcross(board);
	}

	return 0;
}