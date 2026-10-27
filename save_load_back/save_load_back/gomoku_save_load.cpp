#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define SIZE 19
#define max_stone (SIZE*SIZE)

typedef struct {
	char stone;           // '@' 흑돌, '#' 백돌
	int count;            // 연결된 돌 개수
	int startX, startY;   // 시작 좌표
	int endX, endY;       // 끝 좌표
	int gapX, gapY;       // 한 칸 떨어진 빈칸 좌표
	int hasGap;           // 빈칸 포함 여부
	const char* direction;
} GAP_RESULT;

typedef struct {
	int x;
	int y;
	char stone;
}put;

// ----------------------------------------------------
// 공통 함수
// ----------------------------------------------------
int inBoard(int x, int y) {
	return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

const char* stoneName(char stone) {
	if (stone == '@') {
		return "흑돌";
	}
	else if (stone == '#') {
		return "백돌";
	}
	return "없음";
}


// ----------------------------------------------------
// 기존 기능 1 : 가로 연속 돌 분석
// ----------------------------------------------------
void rowstreak(char board[][SIZE]) {

	for (int i = 0; i < SIZE; i++) {
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start = -1;
		int white_start = -1;
		int b_start_save = -1;
		int w_start_save = -1;

		for (int k = 0; k < SIZE; k++) {
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

				if (white_connect > white_max) {
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
			printf("(%d, %d) ~ (%d, %d)\n",
				w_start_save, i,
				w_start_save + white_max - 1, i);
		}
		else if (white_max < black_max) {
			printf("가로 %d | 흑돌 %d연속\n", i, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				b_start_save, i,
				b_start_save + black_max - 1, i);
		}
		else {
			printf("가로 %d | 연속된 돌의 수가 같습니다. (%d연속)\n", i, white_max);
			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				b_start_save, i,
				b_start_save + black_max - 1, i,
				w_start_save, i,
				w_start_save + white_max - 1, i);
		}
	}
}


// ----------------------------------------------------
// 기존 기능 2 : 세로 연속 돌 분석
// ----------------------------------------------------
void colstreak(char board[][SIZE]) {

	for (int i = 0; i < SIZE; i++) {
		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start = -1;
		int white_start = -1;
		int b_start_save = -1;
		int w_start_save = -1;

		for (int k = 0; k < SIZE; k++) {
			if (board[k][i] == '@') {
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
			else if (board[k][i] == '#') {
				if (white_connect == 0) {
					white_start = k;
				}

				white_connect++;
				black_connect = 0;

				if (white_connect > white_max) {
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
			printf("세로 %d | 연속된 돌 없음\n", i);
		}
		else if (white_max > black_max) {
			printf("세로 %d | 백돌 %d연속\n", i, white_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				i, w_start_save,
				i, w_start_save + white_max - 1);
		}
		else if (white_max < black_max) {
			printf("세로 %d | 흑돌 %d연속\n", i, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				i, b_start_save,
				i, b_start_save + black_max - 1);
		}
		else {
			printf("세로 %d | 연속된 돌의 수가 같습니다. (%d연속)\n", i, white_max);
			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				i, b_start_save,
				i, b_start_save + black_max - 1,
				i, w_start_save,
				i, w_start_save + white_max - 1);
		}
	}
}
// ----------------------------------------------------
// 기존 기능 3 : 좌상단 -> 우하단 대각선 분석
// ----------------------------------------------------
void LTtoRBcross(char board[][SIZE]) {

	// 윗변에서 시작하는 대각선
	for (int col = 0; col < SIZE; col++) {
		int y = 0;
		int x = col;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start_x = -1;
		int black_start_y = -1;
		int white_start_x = -1;
		int white_start_y = -1;

		int b_start_save_x = -1;
		int b_start_save_y = -1;
		int w_start_save_x = -1;
		int w_start_save_y = -1;

		while (y < SIZE && x < SIZE) {

			if (board[y][x] == '@') {
				if (black_connect == 0) {
					black_start_x = x;
					black_start_y = y;
				}

				black_connect++;
				white_connect = 0;

				if (black_connect > black_max) {
					black_max = black_connect;
					b_start_save_x = black_start_x;
					b_start_save_y = black_start_y;
				}
			}
			else if (board[y][x] == '#') {
				if (white_connect == 0) {
					white_start_x = x;
					white_start_y = y;
				}

				white_connect++;
				black_connect = 0;

				if (white_connect > white_max) {
					white_max = white_connect;
					w_start_save_x = white_start_x;
					w_start_save_y = white_start_y;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}

			y++;
			x++;
		}

		if (white_max == 0 && black_max == 0) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 연속된 돌 없음\n", col, 0);
		}
		else if (white_max > black_max) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 백돌 %d연속\n", col, 0, white_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				w_start_save_x, w_start_save_y,
				w_start_save_x + white_max - 1,
				w_start_save_y + white_max - 1);
		}
		else if (white_max < black_max) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 흑돌 %d연속\n", col, 0, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x + black_max - 1,
				b_start_save_y + black_max - 1);
		}
		else {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n",
				col, 0, white_max);

			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x + black_max - 1,
				b_start_save_y + black_max - 1,
				w_start_save_x, w_start_save_y,
				w_start_save_x + white_max - 1,
				w_start_save_y + white_max - 1);
		}
	}

	// 왼쪽변에서 시작하는 대각선
	for (int row = 1; row < SIZE; row++) {
		int y = row;
		int x = 0;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start_x = -1;
		int black_start_y = -1;
		int white_start_x = -1;
		int white_start_y = -1;

		int b_start_save_x = -1;
		int b_start_save_y = -1;
		int w_start_save_x = -1;
		int w_start_save_y = -1;

		while (y < SIZE && x < SIZE) {

			if (board[y][x] == '@') {
				if (black_connect == 0) {
					black_start_x = x;
					black_start_y = y;
				}

				black_connect++;
				white_connect = 0;

				if (black_connect > black_max) {
					black_max = black_connect;
					b_start_save_x = black_start_x;
					b_start_save_y = black_start_y;
				}
			}
			else if (board[y][x] == '#') {
				if (white_connect == 0) {
					white_start_x = x;
					white_start_y = y;
				}

				white_connect++;
				black_connect = 0;

				if (white_connect > white_max) {
					white_max = white_connect;
					w_start_save_x = white_start_x;
					w_start_save_y = white_start_y;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}

			y++;
			x++;
		}

		if (white_max == 0 && black_max == 0) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 연속된 돌 없음\n", 0, row);
		}
		else if (white_max > black_max) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 백돌 %d연속\n", 0, row, white_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				w_start_save_x, w_start_save_y,
				w_start_save_x + white_max - 1,
				w_start_save_y + white_max - 1);
		}
		else if (white_max < black_max) {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 흑돌 %d연속\n", 0, row, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x + black_max - 1,
				b_start_save_y + black_max - 1);
		}
		else {
			printf("LT -> RB 대각선 시작점:(%d,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n",
				0, row, white_max);

			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x + black_max - 1,
				b_start_save_y + black_max - 1,
				w_start_save_x, w_start_save_y,
				w_start_save_x + white_max - 1,
				w_start_save_y + white_max - 1);
		}
	}
}
// ----------------------------------------------------
// 기존 기능 4 : 우상단 -> 좌하단 대각선 분석
// ----------------------------------------------------
void RTtoLBcross(char board[][SIZE]) {

	// 윗변에서 시작하는 대각선
	for (int col = SIZE - 1; col >= 0; col--) {
		int y = 0;
		int x = col;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start_x = -1;
		int black_start_y = -1;
		int white_start_x = -1;
		int white_start_y = -1;

		int b_start_save_x = -1;
		int b_start_save_y = -1;
		int w_start_save_x = -1;
		int w_start_save_y = -1;

		while (y < SIZE && x >= 0) {

			if (board[y][x] == '@') {
				if (black_connect == 0) {
					black_start_x = x;
					black_start_y = y;
				}

				black_connect++;
				white_connect = 0;

				if (black_connect > black_max) {
					black_max = black_connect;
					b_start_save_x = black_start_x;
					b_start_save_y = black_start_y;
				}
			}
			else if (board[y][x] == '#') {
				if (white_connect == 0) {
					white_start_x = x;
					white_start_y = y;
				}

				white_connect++;
				black_connect = 0;

				if (white_connect > white_max) {
					white_max = white_connect;
					w_start_save_x = white_start_x;
					w_start_save_y = white_start_y;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}

			y++;
			x--;
		}

		if (white_max == 0 && black_max == 0) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌 없음\n", col, 0);
		}
		else if (white_max > black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 백돌 %d연속\n", col, 0, white_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				w_start_save_x, w_start_save_y,
				w_start_save_x - white_max + 1,
				w_start_save_y + white_max - 1);
		}
		else if (white_max < black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 흑돌 %d연속\n", col, 0, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x - black_max + 1,
				b_start_save_y + black_max - 1);
		}
		else {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n",
				col, 0, white_max);

			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x - black_max + 1,
				b_start_save_y + black_max - 1,
				w_start_save_x, w_start_save_y,
				w_start_save_x - white_max + 1,
				w_start_save_y + white_max - 1);
		}
	}

	// 오른쪽변에서 시작하는 대각선
	for (int row = 1; row < SIZE; row++) {
		int y = row;
		int x = SIZE - 1;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		int black_start_x = -1;
		int black_start_y = -1;
		int white_start_x = -1;
		int white_start_y = -1;

		int b_start_save_x = -1;
		int b_start_save_y = -1;
		int w_start_save_x = -1;
		int w_start_save_y = -1;

		while (y < SIZE && x >= 0) {

			if (board[y][x] == '@') {
				if (black_connect == 0) {
					black_start_x = x;
					black_start_y = y;
				}

				black_connect++;
				white_connect = 0;

				if (black_connect > black_max) {
					black_max = black_connect;
					b_start_save_x = black_start_x;
					b_start_save_y = black_start_y;
				}
			}
			else if (board[y][x] == '#') {
				if (white_connect == 0) {
					white_start_x = x;
					white_start_y = y;
				}

				white_connect++;
				black_connect = 0;

				if (white_connect > white_max) {
					white_max = white_connect;
					w_start_save_x = white_start_x;
					w_start_save_y = white_start_y;
				}
			}
			else {
				black_connect = 0;
				white_connect = 0;
			}

			y++;
			x--;
		}

		if (white_max == 0 && black_max == 0) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌 없음\n", SIZE - 1, row);
		}
		else if (white_max > black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 백돌 %d연속\n", SIZE - 1, row, white_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				w_start_save_x, w_start_save_y,
				w_start_save_x - white_max + 1,
				w_start_save_y + white_max - 1);
		}
		else if (white_max < black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 흑돌 %d연속\n", SIZE - 1, row, black_max);
			printf("(%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x - black_max + 1,
				b_start_save_y + black_max - 1);
		}
		else {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n",
				SIZE - 1, row, white_max);

			printf("흑돌: (%d, %d) ~ (%d, %d), 백돌: (%d, %d) ~ (%d, %d)\n",
				b_start_save_x, b_start_save_y,
				b_start_save_x - black_max + 1,
				b_start_save_y + black_max - 1,
				w_start_save_x, w_start_save_y,
				w_start_save_x - white_max + 1,
				w_start_save_y + white_max - 1);
		}
	}
}

// ----------------------------------------------------
// 추가 기능 1
// 한 칸 띄어진 돌까지 포함한 최장 연결 분석
// ----------------------------------------------------
void updateGapResult(
	GAP_RESULT* best,
	char stone,
	int count,
	int startX,
	int startY,
	int endX,
	int endY,
	int gapX,
	int gapY,
	int hasGap,
	const char* direction
) {
	if (count > best->count) {
		best->stone = stone;
		best->count = count;
		best->startX = startX;
		best->startY = startY;
		best->endX = endX;
		best->endY = endY;
		best->gapX = gapX;
		best->gapY = gapY;
		best->hasGap = hasGap;
		best->direction = direction;
	}
}

void analyzeLineWithOneGap(
	char board[][SIZE],
	int startX,
	int startY,
	int dx,
	int dy,
	const char* direction,
	GAP_RESULT* best
) {
	int xs[SIZE];
	int ys[SIZE];
	char values[SIZE];
	int n = 0;

	int x = startX;
	int y = startY;

	while (inBoard(x, y)) {
		xs[n] = x;
		ys[n] = y;
		values[n] = board[y][x];
		n++;

		x += dx;
		y += dy;
	}

	for (int i = 0; i < n;) {

		if (values[i] != '@' && values[i] != '#') {
			i++;
			continue;
		}

		char stone = values[i];

		int firstStart = i;
		int firstEnd = i;

		while (firstEnd + 1 < n && values[firstEnd + 1] == stone) {
			firstEnd++;
		}

		int firstCount = firstEnd - firstStart + 1;

		// 빈칸 없이 연속된 경우도 후보로 저장
		updateGapResult(
			best,
			stone,
			firstCount,
			xs[firstStart],
			ys[firstStart],
			xs[firstEnd],
			ys[firstEnd],
			-1,
			-1,
			0,
			direction
		);

		// 한 칸 비어 있고 다시 같은 돌이 이어지는 경우
		if (
			firstEnd + 2 < n &&
			values[firstEnd + 1] == '+' &&
			values[firstEnd + 2] == stone
			) {
			int secondStart = firstEnd + 2;
			int secondEnd = secondStart;

			while (secondEnd + 1 < n && values[secondEnd + 1] == stone) {
				secondEnd++;
			}

			int secondCount = secondEnd - secondStart + 1;
			int totalCount = firstCount + secondCount;

			updateGapResult(
				best,
				stone,
				totalCount,
				xs[firstStart],
				ys[firstStart],
				xs[secondEnd],
				ys[secondEnd],
				xs[firstEnd + 1],
				ys[firstEnd + 1],
				1,
				direction
			);
		}

		i = firstEnd + 1;
	}
}

void printMaxOneGapStreak(char board[][SIZE]) {
	GAP_RESULT best = { 0 };

	// 가로 검사
	for (int y = 0; y < SIZE; y++) {
		analyzeLineWithOneGap(board, 0, y, 1, 0, "가로", &best);
	}

	// 세로 검사
	for (int x = 0; x < SIZE; x++) {
		analyzeLineWithOneGap(board, x, 0, 0, 1, "세로", &best);
	}

	// 좌상단 -> 우하단 대각선
	for (int x = 0; x < SIZE; x++) {
		analyzeLineWithOneGap(board, x, 0, 1, 1, "좌상단 -> 우하단 대각선", &best);
	}

	for (int y = 1; y < SIZE; y++) {
		analyzeLineWithOneGap(board, 0, y, 1, 1, "좌상단 -> 우하단 대각선", &best);
	}

	// 우상단 -> 좌하단 대각선
	for (int x = 0; x < SIZE; x++) {
		analyzeLineWithOneGap(board, x, 0, -1, 1, "우상단 -> 좌하단 대각선", &best);
	}

	for (int y = 1; y < SIZE; y++) {
		analyzeLineWithOneGap(board, SIZE - 1, y, -1, 1, "우상단 -> 좌하단 대각선", &best);
	}

	printf("\n");
	printf("============================================================\n");
	printf("[한 칸 띄움까지 포함한 최장 연결]\n");

	if (best.count == 0) {
		printf("아직 놓인 돌이 없습니다.\n");
	}
	else {
		printf("돌 종류 : %s\n", stoneName(best.stone));
		printf("방향    : %s\n", best.direction);
		printf("돌 개수 : %d개\n", best.count);
		printf("좌표    : (%d, %d) ~ (%d, %d)\n",
			best.startX, best.startY,
			best.endX, best.endY);

		if (best.hasGap == 1) {
			printf("중간 빈칸: (%d, %d)\n", best.gapX, best.gapY);
		}
		else {
			printf("중간 빈칸 없이 완전히 연속된 구간입니다.\n");
		}
	}

	printf("============================================================\n");
}


// ----------------------------------------------------
// 추가 기능 2
// 상대방의 일반 / 뛰어있는 3공격, 4공격 방어 위치 탐색
// ----------------------------------------------------


// 방어 위치 저장
// level = 3 : 3공격 차단 위치
// level = 4 : 4공격 차단 위치
void markDefensePosition(
	char board[][SIZE],
	int defenseLevel[][SIZE],
	int x,
	int y,
	int level
) {
	if (!inBoard(x, y)) {
		return;
	}

	if (board[y][x] != '+') {
		return;
	}

	// 같은 칸이 여러 공격에 해당하면
	// 4공격이 3공격보다 우선
	if (level > defenseLevel[y][x]) {
		defenseLevel[y][x] = level;
	}
}


// 한 줄을 배열로 읽은 뒤
// 3공격 / 4공격 패턴을 검사
void scanLineForDefensePatterns(
	char board[][SIZE],
	int startX,
	int startY,
	int dx,
	int dy,
	char opponentStone,
	int defenseLevel[][SIZE]
) {
	int xs[SIZE];
	int ys[SIZE];
	char values[SIZE];
	int n = 0;

	int x = startX;
	int y = startY;

	// 한 줄의 좌표와 값을 배열에 저장
	while (inBoard(x, y)) {
		xs[n] = x;
		ys[n] = y;
		values[n] = board[y][x];
		n++;

		x += dx;
		y += dy;
	}


	// ----------------------------------------------------
	// 1. 4공격 검사
	// 길이 5칸 안에 상대 돌 4개 + 빈칸 1개
	// ----------------------------------------------------
	for (int i = 0; i <= n - 5; i++) {
		int stoneCount = 0;
		int emptyCount = 0;
		int emptyIndex = -1;
		int invalid = 0;

		for (int j = 0; j < 5; j++) {
			char cur = values[i + j];

			if (cur == opponentStone) {
				stoneCount++;
			}
			else if (cur == '+') {
				emptyCount++;
				emptyIndex = i + j;
			}
			else {
				// 내 돌이 섞이면 공격 패턴 아님
				invalid = 1;
				break;
			}
		}

		if (
			invalid == 0 &&
			stoneCount == 4 &&
			emptyCount == 1
			) {
			markDefensePosition(
				board,
				defenseLevel,
				xs[emptyIndex],
				ys[emptyIndex],
				4
			);
		}
	}


	// ----------------------------------------------------
	// 2. 3공격 검사
	// 길이 4칸 안에 상대 돌 3개 + 빈칸 1개
	// ----------------------------------------------------
	for (int i = 0; i <= n - 4; i++) {
		int stoneCount = 0;
		int emptyCount = 0;
		int emptyIndex = -1;
		int invalid = 0;

		for (int j = 0; j < 4; j++) {
			char cur = values[i + j];

			if (cur == opponentStone) {
				stoneCount++;
			}
			else if (cur == '+') {
				emptyCount++;
				emptyIndex = i + j;
			}
			else {
				// 내 돌이 섞이면 공격 패턴 아님
				invalid = 1;
				break;
			}
		}

		if (
			invalid == 0 &&
			stoneCount == 3 &&
			emptyCount == 1
			) {
			markDefensePosition(
				board,
				defenseLevel,
				xs[emptyIndex],
				ys[emptyIndex],
				3
			);
		}
	}
}


// 현재 보드에 상대방의 3 / 4 공격 방어 위치 표시
void printBoardWithDefenseMarks(char board[][SIZE], int turn) {
	char display[SIZE][SIZE];
	int defenseLevel[SIZE][SIZE] = { 0 };

	char opponentStone;

	/*
	turn == 0 : 다음 차례 흑돌
				상대방은 백돌
	turn == 1 : 다음 차례 백돌
				상대방은 흑돌
	*/
	if (turn == 0) {
		opponentStone = '#';
	}
	else {
		opponentStone = '@';
	}


	// 원본 보드를 출력용 배열에 복사
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			display[y][x] = board[y][x];
		}
	}


	// ----------------------------------------------------
	// 1. 가로 검사
	// ----------------------------------------------------
	for (int y = 0; y < SIZE; y++) {
		scanLineForDefensePatterns(
			board,
			0,
			y,
			1,
			0,
			opponentStone,
			defenseLevel
		);
	}


	// ----------------------------------------------------
	// 2. 세로 검사
	// ----------------------------------------------------
	for (int x = 0; x < SIZE; x++) {
		scanLineForDefensePatterns(
			board,
			x,
			0,
			0,
			1,
			opponentStone,
			defenseLevel
		);
	}


	// ----------------------------------------------------
	// 3. 좌상단 -> 우하단 대각선 검사
	// ----------------------------------------------------

	// 윗변에서 시작
	for (int x = 0; x < SIZE; x++) {
		scanLineForDefensePatterns(
			board,
			x,
			0,
			1,
			1,
			opponentStone,
			defenseLevel
		);
	}

	// 왼쪽변에서 시작
	for (int y = 1; y < SIZE; y++) {
		scanLineForDefensePatterns(
			board,
			0,
			y,
			1,
			1,
			opponentStone,
			defenseLevel
		);
	}


	// ----------------------------------------------------
	// 4. 우상단 -> 좌하단 대각선 검사
	// ----------------------------------------------------

	// 윗변에서 시작
	for (int x = 0; x < SIZE; x++) {
		scanLineForDefensePatterns(
			board,
			x,
			0,
			-1,
			1,
			opponentStone,
			defenseLevel
		);
	}

	// 오른쪽변에서 시작
	for (int y = 1; y < SIZE; y++) {
		scanLineForDefensePatterns(
			board,
			SIZE - 1,
			y,
			-1,
			1,
			opponentStone,
			defenseLevel
		);
	}


	// ----------------------------------------------------
	// 방어 위치를 출력용 보드에 반영
	// ----------------------------------------------------
	int markCount = 0;

	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			if (defenseLevel[y][x] != 0) {
				display[y][x] = '|';
				markCount++;
			}
		}
	}


	// ----------------------------------------------------
	// 바둑판 출력
	// ----------------------------------------------------
	printf("\n");
	printf("[현재 바둑판]\n");
	printf("기호 안내: @ = 흑돌, # = 백돌, + = 빈칸, | = 방어 필요 위치\n\n");

	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			printf("%c ", display[y][x]);
		}
		printf("\n");
	}

	printf("\n");


	// ----------------------------------------------------
	// 좌표 출력
	// ----------------------------------------------------
	if (markCount == 0) {
		printf("현재 막아야 할 상대방의 3공격 또는 4공격 위치는 없습니다.\n");
	}
	else {
		printf("상대방 %s의 3공격 / 4공격 방어 위치를 '|'로 표시했습니다.\n",
			stoneName(opponentStone));

		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {

				if (defenseLevel[y][x] == 4) {
					printf("(%d, %d) : 상대방 4공격 차단 위치\n",
						x, y);
				}
				else if (defenseLevel[y][x] == 3) {
					printf("(%d, %d) : 상대방 3공격 차단 위치\n",
						x, y);
				}
			}
		}
	}
}

void savegame(char board[][SIZE], int turn, int blackcount, int whitecount, int top, put stack[], int top2, put stack2[]) {
	FILE* fp;
	fp = fopen("save.txt", "w");
	if (fp == NULL) {
		printf("FILE cant save");
		return;
	}

	fprintf(fp, "%d\n", turn);
	fprintf(fp, "%d %d\n", blackcount, whitecount);
	for (int y = 0; y < SIZE; ++y) {
		for (int x = 0; x < SIZE; ++x) {
			fprintf(fp, "%c", board[y][x]);
		}
		fprintf(fp, "\n");
	}

	fprintf(fp, "%d\n", top+1);
	for (int i = 0; i < top + 1; ++i) {
		fprintf(fp, "%d %d %c\n", stack[i].x, stack[i].y, stack[i].stone);
	}

	fprintf(fp, "%d\n", top2 + 1);
	for (int i = 0; i < top2 + 1; ++i) {
		fprintf(fp, "%d %d %c", stack2[i].x, stack2[i].y, stack2[i].stone);
	}

	fclose(fp);
	printf("game saved");
	return;
}

void loadgame(char board[SIZE][SIZE], int* turn, int* blackcount, int* whitecount, int* top, put stack[], int* top2, put stack2[]) {
	FILE* fp;
	fp = fopen("save.txt", "r");
	if (fp == NULL) {
		printf("FILE load false");
		return;
	}

	char loadboard[SIZE][SIZE];
	int loadturn, loadblackcount, loadwhitecount;

	fscanf(fp, "%d", &loadturn);
	fscanf(fp, "%d %d", &loadblackcount, &loadwhitecount);
	for (int y = 0; y < SIZE; ++y) {
		for (int x = 0; x < SIZE; ++x) {
			fscanf(fp, " %c", &loadboard[y][x]);
			board[y][x] = loadboard[y][x];
		}
	}

	int loadtop, loadtop2;
	int loadx, loady;
	char loadstone;

	fscanf(fp, "%d", &loadtop);
	for (int i = 0; i < loadtop; ++i) {
		fscanf(fp, "%d %d %c", &loadx, &loady, &loadstone);
		stack[i].x = loadx;
		stack[i].y = loady;
		stack[i].stone = loadstone;
	}

	fscanf(fp, "%d", &loadtop2);
	for (int i = 0; i < loadtop2; ++i) {
		fscanf(fp, "%d %d %c", &loadx, &loady, &loadstone);
		stack2[i].x = loadx;
		stack2[i].y = loady;
		stack2[i].stone = loadstone;
	}

	*turn = loadturn;
	*blackcount = loadblackcount;
	*whitecount = loadwhitecount;
	*top = loadtop -1;
	*top2 = loadtop2 -1;

	fclose(fp);
	printf("game loaded");
	return;
}

void pushstack(int* top, put stack[], int x, int y, char stone) {
	if (*top >= max_stone - 1) {
		return;
	}
	(*top)++;
	stack[*top].x = x;
	stack[*top].y = y;
	stack[*top].stone = stone;
	return;
}

int popstack(put stack[], int* top, put* outstack) {
	if (*top < 0) {
		return 0;
	}
	*outstack = stack[*top];
	(*top)--;
	return 1;
}

// ----------------------------------------------------
// main
// ----------------------------------------------------
int main() {
	char board[SIZE][SIZE];

	put stack[max_stone];
	put stack2[max_stone];
	int top, top2;

	int x, y;
	char input[150];
	char extra;

	int turn = 0;          // 0: 흑돌 차례, 1: 백돌 차례
	int blackcount = 0;
	int whitecount = 0;

	int rowboard[2][SIZE];
	int rowblack = 0;
	int rowwhite = 0;

	int colboard[2][SIZE];
	int colblack = 0;
	int colwhite = 0;

	// 보드 초기화
	for (int i = 0; i < SIZE; i++) {
		for (int k = 0; k < SIZE; k++) {
			board[i][k] = '+';
		}
	}

	while (1) {

		printf("\n");
		printf("흑돌 수: %d , 백돌 수: %d\n", blackcount, whitecount);

		if (turn == 0) {
			printf("현재 차례: 흑돌(@)\n");
		}
		else {
			printf("현재 차례: 백돌(#)\n");
		}

		printf("돌을 놓으려는 좌표를 입력하세요.\n");
		printf("(예: 0 0, 1 3)\n");
		printf("S - save, L - load, U - back, R - back return");

		fgets(input, sizeof(input), stdin);

		if (input[0] == 'S') {
			system("cls");
			savegame(board, turn, blackcount, whitecount, top, stack, top2, stack2);
			continue;
		}
		if (input[0] == 'L') {
			system("cls");
			loadgame(board, &turn, &blackcount, &whitecount, &top, stack, &top2, stack2);
			printBoardWithDefenseMarks(board, turn);
			continue;
		}
		if (input[0] == 'U') {
			system("cls");
			put lastput;
			if (popstack(stack, &top, &lastput) == 0) {
				printf("back false");
				continue;
			}
			pushstack(&top2, stack2, lastput.x, lastput.y, lastput.stone);

			board[lastput.y][lastput.x] = '+';
			if (lastput.stone == '@') {
				blackcount--;
				turn = 0;
			}
			else if (lastput.stone == '#') {
				whitecount--;
				turn = 1;
			}

			printBoardWithDefenseMarks(board, turn);
			continue;
		}
		if (input[0] == 'R') {
			system("cls");
			put lastback;
			if (popstack(stack2, &top2, &lastback) == 0) {
				printf("back return false");
				continue;
			}

			if (lastback.stone == '@') {
				blackcount++;
				turn = 1;
			}
			else if (lastback.stone == '#') {
				whitecount++;
				turn = 0;
			}
			board[lastback.y][lastback.x] = lastback.stone;

			pushstack(&top, stack, lastback.x, lastback.y, lastback.stone);
			printBoardWithDefenseMarks(board, turn);
			continue;
		}

		if (sscanf(input, "%d %d %c", &x, &y, &extra) != 2) {
			system("cls");
			printf("잘못된 입력입니다.\n");
			continue;
		}

		system("cls");

		if (x >= SIZE || x < 0 || y >= SIZE || y < 0) {
			printf("잘못된 위치입니다.\n");
		}
		else if (board[y][x] != '+') {
			printf("<<이미 돌이 놓여 있습니다>>\n");
		}
		else {
			char laststone;
			if (turn == 0) {
				board[y][x] = '@';
				blackcount++;
				laststone = '@';
			}
			else {
				board[y][x] = '#';
				whitecount++;
				laststone = '#';
			}

			pushstack(&top, stack, x, y, laststone);
			top2 = -1;
			turn = 1 - turn;
		}

		// 방어 위치가 표시된 보드 출력
		printBoardWithDefenseMarks(board, turn);

		// 가로 방향 돌 개수 세기
		for (int i = 0; i < SIZE; i++) {
			for (int k = 0; k < SIZE; k++) {
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

		printf("\n");
		printf("============================================================\n");
		printf("[가로별 돌 개수]\n");

		for (int k = 0; k < SIZE; k++) {
			printf("가로 %d | 흑돌 수: %d, 백돌 수: %d\n",
				k, rowboard[0][k], rowboard[1][k]);
		}

		// 세로 방향 돌 개수 세기
		for (int i = 0; i < SIZE; i++) {
			for (int k = 0; k < SIZE; k++) {
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

		printf("\n");
		printf("[세로별 돌 개수]\n");

		for (int k = 0; k < SIZE; k++) {
			printf("세로 %d | 흑돌 수: %d, 백돌 수: %d\n",
				k, colboard[0][k], colboard[1][k]);
		}

		printf("============================================================\n");

		// 기존 연속 분석 출력
		printf("\n");
		printf("============================================================\n");
		printf("[기존 연속 돌 분석]\n");

		rowstreak(board);
		colstreak(board);
		LTtoRBcross(board);
		RTtoLBcross(board);

		printf("============================================================\n");

		// 새 기능 1 : 한 칸 띄움 포함 최장 연결 분석
		printMaxOneGapStreak(board);
	}

	return 0;
}