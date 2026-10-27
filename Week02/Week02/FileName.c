#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

int main() {
	char board[19][19];
	int x, y;
	int turn = 0;
	int blackcount = 0;
	int whitecount = 0;
	

	for (int i = 0; i < 19; i++) {
		for (int k = 0; k < 19; k++) {
			board[i][k] = '+';
		}
	}

	while (1) {
		
		printf("흑돌 수: %d , 백돌 수: %d \n", blackcount, whitecount);

		printf("돌을 놓으려는 좌표를 입력 하세요:\n(EX: 0 0, 1 3)\n");
		scanf("%d %d", &x, &y);
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
	}

	return 0;
}