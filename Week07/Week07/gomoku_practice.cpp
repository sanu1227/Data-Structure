#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

void rowcount(char board[][19]) {
	int b_count = 0;
	int w_count = 0;
	for (int i = 0; i < 19; i++) {
		for (int k = 0; k < 19; k++) {
			if (board[i][k] == '@') {
				b_count++;
			}
			if (board[i][k] == '#') {
				w_count++;
			}
		}
		printf("가로 %d | 흑: %d, 백:%d\n", i, b_count, w_count);
		w_count = 0;
		b_count = 0;
	}
}
void colcount(char board[][19]) {
	int b_count = 0;
	int w_count = 0;
	for (int i = 0; i < 19; i++) {
		for (int k = 0; k < 19; k++) {
			if (board[k][i] == '@') {
				b_count++;
			}
			if (board[k][i] == '#') {
				w_count++;
			}
		}
		printf("세로 %d | 흑: %d, 백:%d\n", i, b_count, w_count);
		w_count = 0;
		b_count = 0;
	}
}

void rowconnect(char board[][19]) {
	for (int i = 0; i < 19; i++) {
		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		int b_start = -1;
		int b_save = -1;
		int w_start = -1;
		int w_save = -1;
		for (int k = 0; k < 19; k++) {
			if (board[i][k] == '@') {
				if (b_connect == 0) {
					b_start = k;
				}
				b_connect++;
				w_connect = 0;

				if (b_connect > b_max) {
					b_max = b_connect;
					b_save = b_start;
				}
			}
			else if (board[i][k] == '#') {
				if (w_connect == 0) {
					w_start = k;
				}
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
					w_save = w_start;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
		}
		if (b_max == 0 && w_max == 0) {
			printf("가로 %d | 돌이 없습니다\n", i);
		}
		else if (b_max > w_max) {
			printf("가로 %d | 흑돌 %d 연속\n", i, b_max);
			printf("(%d, %d) ~ (%d, %d)\n", b_save, i, b_save + b_max - 1, i);
		}
		else if (b_max < w_max) {
			printf("가로 %d | 백돌 %d 연속\n", i, w_max);
			printf("(%d, %d) ~ (%d, %d)\n", w_save, i, w_save + w_max - 1, i);
		}
		else {
			printf("가로 %d | 돌의 수가 같습니다. %d 연속\n", i, b_max);
			printf("b:(%d, %d) ~ (%d, %d), w:(%d, %d) ~ (%d, %d)\n", b_save, i, b_save + b_max - 1, i, w_save, i, w_save + w_max - 1, i);
		}
	}
}
void colconnect(char board[][19]) {
	for (int i = 0; i < 19; i++) {
		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		for (int k = 0; k < 19; k++) {
			if (board[k][i] == '@') {
				b_connect++;
				w_connect = 0;

				if (b_connect > b_max) {
					b_max = b_connect;
				}
			}
			else if (board[k][i] == '#') {
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
		}
		if (b_max == 0 && w_max == 0) {
			printf("세로 %d | 돌이 없습니다\n", i);
		}
		else if (b_max > w_max) {
			printf("세로 %d | 흑돌 %d 연속\n", i, b_max);
		}
		else if (b_max < w_max) {
			printf("세로 %d | 백돌 %d 연속\n", i, w_max);
		}
		else {
			printf("세로 %d | 돌의 수가 같습니다. %d 연속\n", i, b_max);
		}
	}
}

void LTtoRB(char board[][19]) {
	for (int col = 0; col < 19; col++) {
		int i = 0;
		int k = col;

		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		while (k < 19 && i < 19) {
			if (board[i][k] == '@') {
				b_connect++;
				w_connect = 0;
				if (b_connect > b_max) {
					b_max = b_connect;
				}

			}
			else if (board[i][k] == '#') {
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
			i++;
			k++;
		}
		if (b_max == 0 && w_max == 0) {
			printf("L -> R 대각선 시작점:(%d, 0) | 돌이 없습니다\n", col);
		}
		else if (b_max > w_max) {
			printf("L -> R 대각선 시작점:(%d, 0) | 흑돌 %d 연속\n", col, b_max);
		}
		else if (b_max < w_max) {
			printf("L -> R 대각선 시작점:(%d, 0) | 백돌 %d 연속\n", col, w_max);
		}
		else {
			printf("L -> R 대각선 시작점:(%d, 0) | 돌의 수가 같습니다. %d 연속\n", col, b_max);
		}
	}

	for (int row = 1; row < 19; row++) {
		int i = row;
		int k = 0;

		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		while (i < 19 && k < 19) {
			if (board[i][k] == '@') {
				b_connect++;
				w_connect = 0;
				if (b_connect > b_max) {
					b_max = b_connect;
				}

			}
			else if (board[i][k] == '#') {
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
			i++;
			k++;
		}
		if (b_max == 0 && w_max == 0) {
			printf("L -> R 대각선 시작점:(0, %d) | 돌이 없습니다\n", row);
		}
		else if (b_max > w_max) {
			printf("L -> R 대각선 시작점:(0, %d) | 흑돌 %d 연속\n", row, b_max);
		}
		else if (b_max < w_max) {
			printf("L -> R 대각선 시작점:(0, %d) | 백돌 %d 연속\n", row, w_max);
		}
		else {
			printf("L -> R 대각선 시작점:(0, %d) | 돌의 수가 같습니다. %d 연속\n", row, b_max);
		}
	}

}

void RTtoLB(char board[][19]) {
	for (int x = 18; x >= 0; x--) {
		int i = 0;
		int k = x;

		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		while (i < 19 && k >= 0) {
			if (board[i][k] == '@') {
				b_connect++;
				w_connect = 0;
				if (b_connect > b_max) {
					b_max = b_connect;
				}
				
			}
			else if (board[i][k] == '#') {
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
			k--;
			i++;
		}
		if (b_max == 0 && w_max == 0) {
			printf("R -> L 대각선 시작점:(%d, 0) | 돌이 없습니다\n", x);
		}
		else if (b_max > w_max) {
			printf("R -> L 대각선 시작점:(%d, 0) | 흑돌 %d 연속\n", x, b_max);
		}
		else if (b_max < w_max) {
			printf("R -> L 대각선 시작점:(%d, 0) | 백돌 %d 연속\n", x, w_max);
		}
		else {
			printf("R -> L 대각선 시작점:(%d, 0) | 돌의 수가 같습니다. %d 연속\n", x, b_max);
		}
	}

	for (int y = 1; y < 19; y++) {
		int i = y;
		int k = 18;

		int b_max = 0;
		int w_max = 0;
		int b_connect = 0;
		int w_connect = 0;
		while (i < 19 && k >= 0) {
			if (board[i][k] == '@') {
				b_connect++;
				w_connect = 0;
				if (b_connect > b_max) {
					b_max = b_connect;
				}

			}
			else if (board[i][k] == '#') {
				b_connect = 0;
				w_connect++;

				if (w_connect > w_max) {
					w_max = w_connect;
				}
			}
			else {
				b_connect = 0;
				w_connect = 0;
			}
			k--;
			i++;
		}
		if (b_max == 0 && w_max == 0) {
			printf("R -> L 대각선 시작점:(18, %d) | 돌이 없습니다\n", y);
		}
		else if (b_max > w_max) {
			printf("R -> L 대각선 시작점:(18, %d) | 흑돌 %d 연속\n", y, b_max);
		}
		else if (b_max < w_max) {
			printf("R -> L 대각선 시작점:(18, %d) | 백돌 %d 연속\n", y, w_max);
		}
		else {
			printf("R -> L 대각선 시작점:(18, %d) | 돌의 수가 같습니다. %d 연속\n", y, b_max);
		}
	}
}

int main() {
	char board[19][19];
	int x, y;
	int turn = 0;
	int blackcount = 0;
	int whitecount = 0;
	char input[150];
	char extra;

	for (int i = 0; i < 19; i++) {
		for (int k = 0; k < 19; k++) {
			board[i][k] = '+';
		}
	}

	while (1) {

		printf("흑돌 수: %d , 백돌 수: %d \n", blackcount, whitecount);

		printf("돌을 놓으려는 좌표를 입력 하세요:\n(EX: 0 0, 1 3)\n");
		fgets(input, sizeof(input), stdin);
		if (sscanf(input, "%d %d %c", &x, &y, &extra) != 2) {
			printf("잘못된 입력입니다.\n");
			continue;
		}
		system("cls");



		if (x > 19 || x < 0 || y < 0 || y>19) {
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
		//rowcount(board);
		//colcount(board);
		rowconnect(board);
		//colconnect(board);
		//LTtoRB(board);
		//RTtoLB(board);
	}

	return 0;
}