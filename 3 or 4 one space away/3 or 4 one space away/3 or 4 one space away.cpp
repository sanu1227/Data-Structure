#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>

#define SIZE 19

typedef struct {
	char stone;           // '@' 흑돌, '#' 백돌
	int count;            // 연결된 돌 개수
	int startX, startY;   // 시작 좌표
	int endX, endY;       // 끝 좌표
	int gapX, gapY;       // 한 칸 떨어진 빈칸 좌표
	int hasGap;           // 빈칸 포함 여부
	const char* direction;
} GAP_RESULT;


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

		for (int k = 0; k < SIZE; k++) {
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

				if (white_connect > white_max) {
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


// ----------------------------------------------------
// 기존 기능 3 : 좌상단 -> 우하단 대각선 분석
// ----------------------------------------------------
void LTtoRBcross(char board[][SIZE]) {

	for (int col = 0; col < SIZE; col++) {
		int i = 0;
		int k = col;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		while (i < SIZE && k < SIZE) {

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

	for (int row = 1; row < SIZE; row++) {
		int i = row;
		int k = 0;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		while (i < SIZE && k < SIZE) {

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


// ----------------------------------------------------
// 기존 기능 4 : 우상단 -> 좌하단 대각선 분석
// ----------------------------------------------------
void RTtoLBcross(char board[][SIZE]) {

	for (int col = SIZE - 1; col >= 0; col--) {
		int i = 0;
		int k = col;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		while (i < SIZE && k >= 0) {

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

	for (int row = 1; row < SIZE; row++) {
		int i = row;
		int k = SIZE - 1;

		int white_connect = 0;
		int black_connect = 0;
		int white_max = 0;
		int black_max = 0;

		while (i < SIZE && k >= 0) {

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
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌 없음\n", row, SIZE - 1);
		}
		else if (white_max > black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 백돌 %d연속\n", row, SIZE - 1, white_max);
		}
		else if (white_max < black_max) {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 흑돌 %d연속\n", row, SIZE - 1, black_max);
		}
		else {
			printf("RT -> LB 대각선 시작점:(%d,%d) | 연속된 돌의 수가 같습니다. (%d연속)\n", row, SIZE - 1, white_max);
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
// 상대방의 3공격 / 4공격 방어 위치 탐색
// ----------------------------------------------------
int countSameStone(
	char board[][SIZE],
	int x,
	int y,
	int dx,
	int dy,
	char stone
) {
	int count = 0;

	x += dx;
	y += dy;

	while (inBoard(x, y) && board[y][x] == stone) {
		count++;
		x += dx;
		y += dy;
	}

	return count;
}

int maxLineIfPlaced(
	char board[][SIZE],
	int x,
	int y,
	char stone
) {
	int maxLen = 1;

	int dirs[4][2] = {
		{1, 0},   // 가로
		{0, 1},   // 세로
		{1, 1},   // 좌상단 -> 우하단
		{1, -1}   // 우상단 -> 좌하단
	};

	for (int i = 0; i < 4; i++) {
		int dx = dirs[i][0];
		int dy = dirs[i][1];

		int len =
			1 +
			countSameStone(board, x, y, dx, dy, stone) +
			countSameStone(board, x, y, -dx, -dy, stone);

		if (len > maxLen) {
			maxLen = len;
		}
	}

	return maxLen;
}

/*
반환값
0 : 공격 없음
3 : 상대방 3공격 차단 위치
4 : 상대방 4공격 차단 위치
*/
int getDefenseLevel(
	char board[][SIZE],
	int x,
	int y,
	char opponentStone
) {
	if (board[y][x] != '+') {
		return 0;
	}

	board[y][x] = opponentStone;

	int lineLen = maxLineIfPlaced(board, x, y, opponentStone);

	board[y][x] = '+';

	// 상대가 이 위치에 두면 오목 완성
	// 현재는 상대의 4공격을 막아야 하는 칸
	if (lineLen >= 5) {
		return 4;
	}

	// 상대가 이 위치에 두면 4목 완성
	// 현재는 상대의 3공격을 막아야 하는 칸
	if (lineLen == 4) {
		return 3;
	}

	return 0;
}

void printBoardWithDefenseMarks(char board[][SIZE], int turn) {
	char display[SIZE][SIZE];

	int markX[SIZE * SIZE];
	int markY[SIZE * SIZE];
	int markLevel[SIZE * SIZE];
	int markCount = 0;

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

	// 방어해야 하는 위치 찾기
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			int level = getDefenseLevel(board, x, y, opponentStone);

			if (level != 0) {
				display[y][x] = '|';

				markX[markCount] = x;
				markY[markCount] = y;
				markLevel[markCount] = level;
				markCount++;
			}
		}
	}

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

	if (markCount == 0) {
		printf("현재 즉시 막아야 할 상대방의 3공격 또는 4공격 위치는 없습니다.\n");
	}
	else {
		printf("상대방 %s의 공격을 막아야 하는 위치를 '|'로 표시했습니다.\n",
			stoneName(opponentStone));

		for (int i = 0; i < markCount; i++) {
			if (markLevel[i] == 4) {
				printf("(%d, %d) : 상대방 4공격 차단 위치\n",
					markX[i], markY[i]);
			}
			else if (markLevel[i] == 3) {
				printf("(%d, %d) : 상대방 3공격 차단 위치\n",
					markX[i], markY[i]);
			}
		}
	}
}


// ----------------------------------------------------
// main
// ----------------------------------------------------
int main() {
	char board[SIZE][SIZE];

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

		fgets(input, sizeof(input), stdin);

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