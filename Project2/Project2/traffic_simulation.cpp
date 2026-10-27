#include <windows.h>     // 윈도우 API 사용을 위한 기본 헤더 파일
#include <tchar.h>       // TCHAR, LPCTSTR 같은 문자열 자료형 사용을 위한 헤더
#include <stdlib.h>      // rand 함수 사용을 위한 헤더
#include <time.h>        // srand(time(NULL)) 사용을 위한 헤더

HINSTANCE g_hInst;       // 현재 프로그램의 인스턴스 핸들을 저장하는 전역 변수

LPCTSTR lpszClass = L"My Window Class";              // 윈도우 클래스 이름
LPCTSTR lpszWindowName = L"Intersection Car Program"; // 윈도우 제목

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam); // 메시지 처리 함수 선언

#define WINDOW_WIDTH 900          // 윈도우 가로 크기
#define WINDOW_HEIGHT 700         // 윈도우 세로 크기

#define TIMER_MOVE 1              // 자동차와 사람을 계속 움직이게 하는 타이머 번호
#define TIMER_YELLOW 2            // 노랑 신호 1초 대기용 타이머 번호
#define TIMER_AUTO 3              // 자동 신호 변경용 타이머 번호

#define MENU_H_BLUE_V_RED 1001    // 메뉴: 가로 파랑, 세로 빨강
#define MENU_H_RED_V_BLUE 1002    // 메뉴: 가로 빨강, 세로 파랑
#define MENU_AUTO 1003            // 메뉴: 자동 신호 변경 시작
#define MENU_STOP 1004            // 메뉴: 자동 신호 변경 멈춤
#define MENU_QUIT 1005            // 메뉴: 프로그램 종료

#define SIGNAL_RED 0              // 신호등 빨강
#define SIGNAL_BLUE 1             // 신호등 파랑
#define SIGNAL_YELLOW 2           // 신호등 노랑

#define DIR_RIGHT 0               // 오른쪽으로 이동하는 자동차
#define DIR_LEFT 1                // 왼쪽으로 이동하는 자동차
#define DIR_DOWN 2                // 아래쪽으로 이동하는 자동차
#define DIR_UP 3                  // 위쪽으로 이동하는 자동차

#define PED_WAIT 0                // 사람이 대기 중인 상태
#define PED_CROSS_H 1             // 사람이 가로 방향 횡단보도를 건너는 상태
#define PED_CROSS_DIAGONAL 2      // 사람이 대각선으로 건너는 상태

struct CAR {                      // 자동차 한 대의 정보를 저장하는 구조체
	int x;                         // 자동차의 현재 x 좌표
	int y;                         // 자동차의 현재 y 좌표
	int w;                         // 자동차의 너비
	int h;                         // 자동차의 높이
	int speed;                     // 자동차의 이동 속도
	int dir;                       // 자동차의 이동 방향
	COLORREF color;                // 자동차 색상
};

CAR cars[8];                      // 화면에 있는 자동차 8대를 저장하는 배열

int hSignal = SIGNAL_BLUE;        // 가로 도로 기준 신호등 상태
int vSignal = SIGNAL_RED;         // 세로 도로 기준 신호등 상태

int beforeHSignal = SIGNAL_BLUE;  // 노랑으로 바뀌기 전 가로 신호 상태 저장
int beforeVSignal = SIGNAL_RED;   // 노랑으로 바뀌기 전 세로 신호 상태 저장
int yellowTarget = 0;             // 1이면 가로 신호 변경 중, 2이면 세로 신호 변경 중

int autoMode = 0;                 // 자동 신호 변경 여부
int globalStop = 0;               // 왼쪽 마우스 클릭으로 모든 자동차가 정지했는지 여부
int pendingResume = 0;            // 사람이 대각선 횡단 중일 때 오른쪽 클릭을 눌렀는지 저장

int speedPlus = 0;                // +/- 키로 조절되는 추가 속도값

int pedState = PED_WAIT;          // 현재 사람의 이동 상태
int pedX = 0;                     // 사람의 현재 x 좌표
int pedY = 0;                     // 사람의 현재 y 좌표
int pedStartX = 0;                // 사람의 시작 x 좌표
int pedStartY = 0;                // 사람의 시작 y 좌표
int pedEndX = 0;                  // 사람의 목표 x 좌표
int pedEndY = 0;                  // 사람의 목표 y 좌표

RECT hSignalRect;                 // 가로 도로 신호등 원의 영역
RECT vSignalRect;                 // 세로 도로 신호등 원의 영역

const int roadLeft = 330;         // 세로 도로의 왼쪽 경계
const int roadRight = 570;        // 세로 도로의 오른쪽 경계
const int roadTop = 230;          // 가로 도로의 위쪽 경계
const int roadBottom = 470;       // 가로 도로의 아래쪽 경계

const int centerX = 450;          // 사거리 중심 x 좌표
const int centerY = 350;          // 사거리 중심 y 좌표

const int stopGap = 15;           // 자동차끼리 멈출 때 유지할 간격

void MakeMenu(HWND hWnd);         // 메뉴를 만드는 함수 선언
void InitCars();                  // 자동차 초기 위치를 설정하는 함수 선언
void DrawRoad(HDC hDC);           // 도로와 횡단보도를 그리는 함수 선언
void DrawSignals(HDC hDC);        // 신호등을 그리는 함수 선언
void DrawCars(HDC hDC);           // 자동차를 그리는 함수 선언
void DrawOneCar(HDC hDC, CAR car);// 자동차 한 대를 그리는 함수 선언
void DrawPeople(HDC hDC);         // 사람을 그리는 함수 선언
void MoveCars();                  // 자동차를 이동시키는 함수 선언
void MoveOneCar(int index);       // 자동차 한 대를 이동시키는 함수 선언
void MovePerson(HWND hWnd);       // 사람을 이동시키는 함수 선언
void StartYellow(int target);     // 신호등을 노랑으로 바꾸는 함수 선언
void FinishYellow();              // 노랑 신호 1초 후 실제 신호를 바꾸는 함수 선언
void SetHBlueVRed();              // 가로 파랑, 세로 빨강 설정 함수 선언
void SetHRedVBlue();              // 가로 빨강, 세로 파랑 설정 함수 선언
void StartDiagonalWalk();         // 사람이 대각선으로 걷기 시작하는 함수 선언
void StartHorizontalWalk();       // 사람이 가로 횡단보도를 걷기 시작하는 함수 선언
int IsCarAllowedToMove(CAR car);  // 자동차가 현재 신호에서 움직일 수 있는지 판단하는 함수 선언
int IsInsideRect(int mx, int my, RECT rc); // 마우스 좌표가 사각 영역 안인지 검사하는 함수 선언

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;                                                // 생성될 윈도우 핸들 변수
	MSG Message;                                             // 메시지를 저장할 구조체 변수
	WNDCLASSEX WndClass;                                     // 윈도우 클래스 정보를 저장할 구조체 변수

	g_hInst = hInstance;                                     // 전역 인스턴스 변수에 현재 인스턴스 저장

	WndClass.cbSize = sizeof(WndClass);                      // 구조체 크기 설정
	WndClass.style = CS_HREDRAW | CS_VREDRAW;                // 가로/세로 크기 변경 시 다시 그리기
	WndClass.lpfnWndProc = (WNDPROC)WndProc;                 // 메시지 처리 함수 등록
	WndClass.cbClsExtra = 0;                                 // 추가 클래스 메모리 사용 안 함
	WndClass.cbWndExtra = 0;                                 // 추가 윈도우 메모리 사용 안 함
	WndClass.hInstance = hInstance;                          // 현재 프로그램 인스턴스 설정
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);        // 기본 아이콘 사용
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);          // 기본 화살표 커서 사용
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // 배경을 흰색으로 설정
	WndClass.lpszMenuName = NULL;                            // 메뉴는 코드에서 직접 만들기 때문에 NULL
	WndClass.lpszClassName = lpszClass;                      // 윈도우 클래스 이름 설정
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);      // 작은 아이콘도 기본 아이콘 사용

	RegisterClassEx(&WndClass);                              // 윈도우 클래스 등록

	hWnd = CreateWindow(                                     // 윈도우 생성
		lpszClass,                                           // 등록한 윈도우 클래스 이름
		lpszWindowName,                                      // 윈도우 제목
		WS_OVERLAPPEDWINDOW,                                 // 일반적인 윈도우 스타일
		100,                                                 // 윈도우 시작 x 위치
		50,                                                  // 윈도우 시작 y 위치
		WINDOW_WIDTH,                                        // 윈도우 가로 크기
		WINDOW_HEIGHT,                                       // 윈도우 세로 크기
		NULL,                                                // 부모 윈도우 없음
		(HMENU)NULL,                                         // 메뉴는 나중에 직접 붙임
		hInstance,                                           // 현재 인스턴스
		NULL                                                 // 추가 데이터 없음
	);

	ShowWindow(hWnd, nCmdShow);                              // 윈도우 화면에 보이기
	UpdateWindow(hWnd);                                      // 윈도우 갱신

	while (GetMessage(&Message, 0, 0, 0)) {                  // 메시지 루프 시작
		TranslateMessage(&Message);                         // 키보드 입력 메시지 변환
		DispatchMessage(&Message);                          // WndProc으로 메시지 전달
	}

	return (int)Message.wParam;                              // 프로그램 종료 코드 반환
}

void MakeMenu(HWND hWnd)
{
	HMENU hMenu = CreateMenu();                              // 최상위 메뉴 생성
	HMENU hSignalMenu = CreatePopupMenu();                   // Signal 하위 메뉴 생성
	HMENU hGameMenu = CreatePopupMenu();                     // Game 하위 메뉴 생성

	AppendMenu(hSignalMenu, MF_STRING, MENU_H_BLUE_V_RED, L"H-blue, V-red"); // 가로 파랑 세로 빨강 메뉴 추가
	AppendMenu(hSignalMenu, MF_STRING, MENU_H_RED_V_BLUE, L"H-red, V-blue"); // 가로 빨강 세로 파랑 메뉴 추가

	AppendMenu(hGameMenu, MF_STRING, MENU_AUTO, L"Auto");    // 자동 신호 변경 메뉴 추가
	AppendMenu(hGameMenu, MF_STRING, MENU_STOP, L"Stop");    // 자동 신호 변경 정지 메뉴 추가
	AppendMenu(hGameMenu, MF_STRING, MENU_QUIT, L"Quit");    // 종료 메뉴 추가

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hSignalMenu, L"Signal"); // Signal 메뉴를 메인 메뉴에 추가
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hGameMenu, L"Game");     // Game 메뉴를 메인 메뉴에 추가

	SetMenu(hWnd, hMenu);                                    // 만든 메뉴를 윈도우에 붙임
}

void InitCars()
{
	cars[0].x = 50;                                          // 오른쪽 이동 자동차 1의 x 위치
	cars[0].y = 280;                                         // 오른쪽 이동 자동차 1의 y 위치
	cars[0].w = 55;                                          // 오른쪽 이동 자동차 1의 너비
	cars[0].h = 28;                                          // 오른쪽 이동 자동차 1의 높이
	cars[0].speed = 4;                                       // 오른쪽 이동 자동차 1의 속도
	cars[0].dir = DIR_RIGHT;                                 // 오른쪽 이동 방향
	cars[0].color = RGB(255, 80, 80);                        // 자동차 색상

	cars[1].x = -170;                                        // 오른쪽 이동 자동차 2의 x 위치
	cars[1].y = 320;                                         // 오른쪽 이동 자동차 2의 y 위치
	cars[1].w = 55;                                          // 오른쪽 이동 자동차 2의 너비
	cars[1].h = 28;                                          // 오른쪽 이동 자동차 2의 높이
	cars[1].speed = 3;                                       // 오른쪽 이동 자동차 2의 속도
	cars[1].dir = DIR_RIGHT;                                 // 오른쪽 이동 방향
	cars[1].color = RGB(255, 140, 80);                       // 자동차 색상

	cars[2].x = 780;                                         // 왼쪽 이동 자동차 1의 x 위치
	cars[2].y = 390;                                         // 왼쪽 이동 자동차 1의 y 위치
	cars[2].w = 55;                                          // 왼쪽 이동 자동차 1의 너비
	cars[2].h = 28;                                          // 왼쪽 이동 자동차 1의 높이
	cars[2].speed = 5;                                       // 왼쪽 이동 자동차 1의 속도
	cars[2].dir = DIR_LEFT;                                  // 왼쪽 이동 방향
	cars[2].color = RGB(80, 160, 255);                       // 자동차 색상

	cars[3].x = 1020;                                        // 왼쪽 이동 자동차 2의 x 위치
	cars[3].y = 430;                                         // 왼쪽 이동 자동차 2의 y 위치
	cars[3].w = 55;                                          // 왼쪽 이동 자동차 2의 너비
	cars[3].h = 28;                                          // 왼쪽 이동 자동차 2의 높이
	cars[3].speed = 3;                                       // 왼쪽 이동 자동차 2의 속도
	cars[3].dir = DIR_LEFT;                                  // 왼쪽 이동 방향
	cars[3].color = RGB(80, 220, 255);                       // 자동차 색상

	cars[4].x = 385;                                         // 아래 이동 자동차 1의 x 위치
	cars[4].y = 30;                                          // 아래 이동 자동차 1의 y 위치
	cars[4].w = 28;                                          // 아래 이동 자동차 1의 너비
	cars[4].h = 55;                                          // 아래 이동 자동차 1의 높이
	cars[4].speed = 4;                                       // 아래 이동 자동차 1의 속도
	cars[4].dir = DIR_DOWN;                                  // 아래 이동 방향
	cars[4].color = RGB(120, 255, 120);                      // 자동차 색상

	cars[5].x = 425;                                         // 아래 이동 자동차 2의 x 위치
	cars[5].y = -210;                                        // 아래 이동 자동차 2의 y 위치
	cars[5].w = 28;                                          // 아래 이동 자동차 2의 너비
	cars[5].h = 55;                                          // 아래 이동 자동차 2의 높이
	cars[5].speed = 2;                                       // 아래 이동 자동차 2의 속도
	cars[5].dir = DIR_DOWN;                                  // 아래 이동 방향
	cars[5].color = RGB(180, 255, 120);                      // 자동차 색상

	cars[6].x = 500;                                         // 위 이동 자동차 1의 x 위치
	cars[6].y = 620;                                         // 위 이동 자동차 1의 y 위치
	cars[6].w = 28;                                          // 위 이동 자동차 1의 너비
	cars[6].h = 55;                                          // 위 이동 자동차 1의 높이
	cars[6].speed = 5;                                       // 위 이동 자동차 1의 속도
	cars[6].dir = DIR_UP;                                    // 위 이동 방향
	cars[6].color = RGB(180, 120, 255);                      // 자동차 색상

	cars[7].x = 535;                                         // 위 이동 자동차 2의 x 위치
	cars[7].y = 860;                                         // 위 이동 자동차 2의 y 위치
	cars[7].w = 28;                                          // 위 이동 자동차 2의 너비
	cars[7].h = 55;                                          // 위 이동 자동차 2의 높이
	cars[7].speed = 3;                                       // 위 이동 자동차 2의 속도
	cars[7].dir = DIR_UP;                                    // 위 이동 방향
	cars[7].color = RGB(220, 120, 255);                      // 자동차 색상
}

void DrawRoad(HDC hDC)
{
	HBRUSH roadBrush = CreateSolidBrush(RGB(70, 70, 70));     // 도로를 칠할 진한 회색 브러시 생성
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, roadBrush);   // 새 브러시 선택하고 기존 브러시 저장

	Rectangle(hDC, 0, roadTop, WINDOW_WIDTH, roadBottom);     // 가로 도로 그리기
	Rectangle(hDC, roadLeft, 0, roadRight, WINDOW_HEIGHT);    // 세로 도로 그리기

	SelectObject(hDC, oldBrush);                              // 기존 브러시 복구
	DeleteObject(roadBrush);                                  // 만든 브러시 삭제

	HPEN whitePen = CreatePen(PS_SOLID, 2, RGB(255, 255, 255)); // 차선용 흰색 펜 생성
	HPEN oldPen = (HPEN)SelectObject(hDC, whitePen);          // 흰색 펜 선택

	MoveToEx(hDC, 0, centerY, NULL);                          // 가로 도로 중앙선 시작점 이동
	LineTo(hDC, roadLeft, centerY);                           // 왼쪽 구간 중앙선 그리기
	MoveToEx(hDC, roadRight, centerY, NULL);                  // 가로 도로 오른쪽 구간 시작점 이동
	LineTo(hDC, WINDOW_WIDTH, centerY);                       // 오른쪽 구간 중앙선 그리기

	MoveToEx(hDC, centerX, 0, NULL);                          // 세로 도로 중앙선 시작점 이동
	LineTo(hDC, centerX, roadTop);                            // 위쪽 구간 중앙선 그리기
	MoveToEx(hDC, centerX, roadBottom, NULL);                 // 세로 도로 아래쪽 구간 시작점 이동
	LineTo(hDC, centerX, WINDOW_HEIGHT);                      // 아래쪽 구간 중앙선 그리기

	for (int i = 0; i < 6; i++) {                             // 횡단보도 줄 여러 개를 반복해서 그림
		int offset = i * 18;                                  // 횡단보도 줄 사이 간격 계산

		Rectangle(hDC, roadLeft - 70 + offset, roadTop - 25, roadLeft - 60 + offset, roadTop);       // 위쪽 왼쪽 횡단보도 일부
		Rectangle(hDC, roadRight + 20 + offset, roadBottom, roadRight + 30 + offset, roadBottom + 25); // 아래쪽 오른쪽 횡단보도 일부
		Rectangle(hDC, roadLeft - 25, roadTop - 70 + offset, roadLeft, roadTop - 60 + offset);       // 왼쪽 위 횡단보도 일부
		Rectangle(hDC, roadRight, roadBottom + 20 + offset, roadRight + 25, roadBottom + 30 + offset); // 오른쪽 아래 횡단보도 일부
	}

	SelectObject(hDC, oldPen);                                // 기존 펜 복구
	DeleteObject(whitePen);                                    // 만든 펜 삭제
}

void DrawSignals(HDC hDC)
{
	hSignalRect.left = 130;                                    // 가로 신호등 원의 왼쪽 좌표
	hSignalRect.top = 90;                                      // 가로 신호등 원의 위쪽 좌표
	hSignalRect.right = 190;                                   // 가로 신호등 원의 오른쪽 좌표
	hSignalRect.bottom = 150;                                  // 가로 신호등 원의 아래쪽 좌표

	vSignalRect.left = 220;                                    // 세로 신호등 원의 왼쪽 좌표
	vSignalRect.top = 90;                                      // 세로 신호등 원의 위쪽 좌표
	vSignalRect.right = 280;                                   // 세로 신호등 원의 오른쪽 좌표
	vSignalRect.bottom = 150;                                  // 세로 신호등 원의 아래쪽 좌표

	COLORREF hColor;                                           // 가로 신호등 색을 저장할 변수
	COLORREF vColor;                                           // 세로 신호등 색을 저장할 변수

	if (hSignal == SIGNAL_RED) hColor = RGB(255, 0, 0);        // 가로 신호가 빨강이면 빨강색 설정
	else if (hSignal == SIGNAL_BLUE) hColor = RGB(0, 120, 255);// 가로 신호가 파랑이면 파랑색 설정
	else hColor = RGB(255, 220, 0);                            // 가로 신호가 노랑이면 노랑색 설정

	if (vSignal == SIGNAL_RED) vColor = RGB(255, 0, 0);        // 세로 신호가 빨강이면 빨강색 설정
	else if (vSignal == SIGNAL_BLUE) vColor = RGB(0, 120, 255);// 세로 신호가 파랑이면 파랑색 설정
	else vColor = RGB(255, 220, 0);                            // 세로 신호가 노랑이면 노랑색 설정

	HBRUSH hBrush = CreateSolidBrush(hColor);                  // 가로 신호등 색 브러시 생성
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, hBrush);       // 가로 신호등 브러시 선택
	Ellipse(hDC, hSignalRect.left, hSignalRect.top, hSignalRect.right, hSignalRect.bottom); // 가로 신호등 원 그리기
	SelectObject(hDC, oldBrush);                              // 기존 브러시 복구
	DeleteObject(hBrush);                                     // 가로 신호등 브러시 삭제

	HBRUSH vBrush = CreateSolidBrush(vColor);                  // 세로 신호등 색 브러시 생성
	oldBrush = (HBRUSH)SelectObject(hDC, vBrush);              // 세로 신호등 브러시 선택
	Ellipse(hDC, vSignalRect.left, vSignalRect.top, vSignalRect.right, vSignalRect.bottom); // 세로 신호등 원 그리기
	SelectObject(hDC, oldBrush);                              // 기존 브러시 복구
	DeleteObject(vBrush);                                     // 세로 신호등 브러시 삭제

	TextOut(hDC, 125, 160, L"H Signal", 8);                    // 가로 신호등 설명 출력
	TextOut(hDC, 215, 160, L"V Signal", 8);                    // 세로 신호등 설명 출력

	TextOut(hDC, 600, 40, L"+ / - : speed up / down", 22);     // 키보드 설명 출력
	TextOut(hDC, 600, 65, L"A : auto signal on/off", 22);      // 키보드 설명 출력
	TextOut(hDC, 600, 90, L"Q : quit", 8);                     // 키보드 설명 출력
	TextOut(hDC, 600, 115, L"LButton : all stop + diagonal walk", 34); // 마우스 설명 출력
	TextOut(hDC, 600, 140, L"RButton : resume cars", 21);      // 마우스 설명 출력
}

void DrawOneCar(HDC hDC, CAR car)
{
	HBRUSH carBrush = CreateSolidBrush(car.color);             // 자동차 색상의 브러시 생성
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, carBrush);     // 자동차 브러시 선택

	Rectangle(hDC, car.x, car.y, car.x + car.w, car.y + car.h); // 자동차 본체 사각형 그리기

	if (car.dir == DIR_RIGHT && car.x + car.w > WINDOW_WIDTH) { // 오른쪽 끝을 넘어가는 경우
		Rectangle(hDC, car.x - WINDOW_WIDTH, car.y, car.x - WINDOW_WIDTH + car.w, car.y + car.h); // 반대편에 이어서 그리기
	}

	if (car.dir == DIR_LEFT && car.x < 0) {                     // 왼쪽 끝을 넘어가는 경우
		Rectangle(hDC, car.x + WINDOW_WIDTH, car.y, car.x + WINDOW_WIDTH + car.w, car.y + car.h); // 반대편에 이어서 그리기
	}

	if (car.dir == DIR_DOWN && car.y + car.h > WINDOW_HEIGHT) { // 아래쪽 끝을 넘어가는 경우
		Rectangle(hDC, car.x, car.y - WINDOW_HEIGHT, car.x + car.w, car.y - WINDOW_HEIGHT + car.h); // 반대편에 이어서 그리기
	}

	if (car.dir == DIR_UP && car.y < 0) {                       // 위쪽 끝을 넘어가는 경우
		Rectangle(hDC, car.x, car.y + WINDOW_HEIGHT, car.x + car.w, car.y + WINDOW_HEIGHT + car.h); // 반대편에 이어서 그리기
	}

	SelectObject(hDC, oldBrush);                               // 기존 브러시 복구
	DeleteObject(carBrush);                                    // 자동차 브러시 삭제
}

void DrawCars(HDC hDC)
{
	for (int i = 0; i < 8; i++) {                               // 자동차 8대를 반복
		DrawOneCar(hDC, cars[i]);                              // i번째 자동차 그리기
	}
}

void DrawPeople(HDC hDC)
{
	HBRUSH personBrush = CreateSolidBrush(RGB(20, 20, 20));     // 사람을 검은색으로 그릴 브러시 생성
	HBRUSH oldBrush = (HBRUSH)SelectObject(hDC, personBrush);   // 사람 브러시 선택

	Ellipse(hDC, pedX - 8, pedY - 22, pedX + 8, pedY - 6);      // 사람 머리 그리기
	Rectangle(hDC, pedX - 6, pedY - 5, pedX + 6, pedY + 18);    // 사람 몸통 그리기

	MoveToEx(hDC, pedX - 6, pedY + 18, NULL);                  // 왼쪽 다리 시작점
	LineTo(hDC, pedX - 14, pedY + 32);                         // 왼쪽 다리 그리기
	MoveToEx(hDC, pedX + 6, pedY + 18, NULL);                  // 오른쪽 다리 시작점
	LineTo(hDC, pedX + 14, pedY + 32);                         // 오른쪽 다리 그리기

	SelectObject(hDC, oldBrush);                               // 기존 브러시 복구
	DeleteObject(personBrush);                                 // 사람 브러시 삭제
}

int IsCarAllowedToMove(CAR car)
{
	if (globalStop == 1) {                                      // 전체 정지 상태라면
		return 0;                                               // 어떤 자동차도 움직일 수 없음
	}

	if (pedState == PED_CROSS_DIAGONAL) {                       // 사람이 대각선 횡단 중이라면
		return 0;                                               // 안전을 위해 자동차 이동 금지
	}

	if (car.dir == DIR_RIGHT || car.dir == DIR_LEFT) {           // 자동차가 가로 도로를 달린다면
		if (hSignal == SIGNAL_BLUE) return 1;                    // 가로 신호가 파랑이면 움직임
		return 0;                                               // 빨강 또는 노랑이면 멈춤
	}

	if (car.dir == DIR_UP || car.dir == DIR_DOWN) {              // 자동차가 세로 도로를 달린다면
		if (vSignal == SIGNAL_BLUE) return 1;                    // 세로 신호가 파랑이면 움직임
		return 0;                                               // 빨강 또는 노랑이면 멈춤
	}

	return 0;                                                   // 혹시 모를 예외 상황에서는 멈춤
}

void MoveOneCar(int index)
{
	CAR car = cars[index];                                      // 현재 자동차 정보를 임시 변수에 복사
	int moveSpeed = car.speed + speedPlus;                      // 기본 속도에 추가 속도값 적용

	if (moveSpeed < 1) moveSpeed = 1;                           // 속도가 1보다 작아지지 않도록 보정

	if (IsCarAllowedToMove(car) == 1) {                         // 현재 신호에서 자동차가 움직일 수 있다면
		if (car.dir == DIR_RIGHT) cars[index].x += moveSpeed;    // 오른쪽 이동
		if (car.dir == DIR_LEFT) cars[index].x -= moveSpeed;     // 왼쪽 이동
		if (car.dir == DIR_DOWN) cars[index].y += moveSpeed;     // 아래 이동
		if (car.dir == DIR_UP) cars[index].y -= moveSpeed;       // 위 이동
	}
	else {                                                      // 현재 신호에서 자동차가 멈춰야 한다면
		if (car.dir == DIR_RIGHT) {                             // 오른쪽 방향 자동차 정지 처리
			int stopX = roadLeft - car.w - 20;                   // 사거리 앞 정지선 위치
			if (cars[index].x < stopX && cars[index].x + moveSpeed > stopX) cars[index].x = stopX; // 정지선에 맞춰 정지
			if (index == 1 && cars[1].x + cars[1].w + stopGap > cars[0].x) cars[1].x = cars[0].x - cars[1].w - stopGap; // 뒤차가 앞차 뒤에 멈춤
		}

		if (car.dir == DIR_LEFT) {                              // 왼쪽 방향 자동차 정지 처리
			int stopX = roadRight + 20;                          // 사거리 앞 정지선 위치
			if (cars[index].x > stopX && cars[index].x - moveSpeed < stopX) cars[index].x = stopX; // 정지선에 맞춰 정지
			if (index == 3 && cars[3].x < cars[2].x + cars[2].w + stopGap) cars[3].x = cars[2].x + cars[2].w + stopGap; // 뒤차가 앞차 뒤에 멈춤
		}

		if (car.dir == DIR_DOWN) {                              // 아래 방향 자동차 정지 처리
			int stopY = roadTop - car.h - 20;                    // 사거리 앞 정지선 위치
			if (cars[index].y < stopY && cars[index].y + moveSpeed > stopY) cars[index].y = stopY; // 정지선에 맞춰 정지
			if (index == 5 && cars[5].y + cars[5].h + stopGap > cars[4].y) cars[5].y = cars[4].y - cars[5].h - stopGap; // 뒤차가 앞차 뒤에 멈춤
		}

		if (car.dir == DIR_UP) {                                // 위 방향 자동차 정지 처리
			int stopY = roadBottom + 20;                         // 사거리 앞 정지선 위치
			if (cars[index].y > stopY && cars[index].y - moveSpeed < stopY) cars[index].y = stopY; // 정지선에 맞춰 정지
			if (index == 7 && cars[7].y < cars[6].y + cars[6].h + stopGap) cars[7].y = cars[6].y + cars[6].h + stopGap; // 뒤차가 앞차 뒤에 멈춤
		}
	}

	if (cars[index].dir == DIR_RIGHT && cars[index].x > WINDOW_WIDTH) cars[index].x = -cars[index].w; // 오른쪽 끝을 완전히 지나면 왼쪽에서 다시 등장
	if (cars[index].dir == DIR_LEFT && cars[index].x + cars[index].w < 0) cars[index].x = WINDOW_WIDTH; // 왼쪽 끝을 완전히 지나면 오른쪽에서 다시 등장
	if (cars[index].dir == DIR_DOWN && cars[index].y > WINDOW_HEIGHT) cars[index].y = -cars[index].h; // 아래쪽 끝을 완전히 지나면 위에서 다시 등장
	if (cars[index].dir == DIR_UP && cars[index].y + cars[index].h < 0) cars[index].y = WINDOW_HEIGHT; // 위쪽 끝을 완전히 지나면 아래에서 다시 등장
}

void MoveCars()
{
	for (int i = 0; i < 8; i++) {                               // 자동차 8대를 반복
		MoveOneCar(i);                                         // i번째 자동차 이동 처리
	}
}

void StartHorizontalWalk()
{
	pedState = PED_CROSS_H;                                    // 사람 상태를 가로 횡단으로 변경
	pedStartX = roadLeft - 80;                                 // 시작 x 좌표
	pedStartY = centerY + 15;                                  // 시작 y 좌표
	pedEndX = roadRight + 80;                                  // 도착 x 좌표
	pedEndY = centerY + 15;                                    // 도착 y 좌표
	pedX = pedStartX;                                          // 현재 x 좌표를 시작 위치로 설정
	pedY = pedStartY;                                          // 현재 y 좌표를 시작 위치로 설정
}

void StartDiagonalWalk()
{
	pedState = PED_CROSS_DIAGONAL;                             // 사람 상태를 대각선 횡단으로 변경
	pedStartX = roadLeft - 70;                                 // 대각선 시작 x 좌표
	pedStartY = roadTop - 70;                                  // 대각선 시작 y 좌표
	pedEndX = roadRight + 70;                                  // 대각선 도착 x 좌표
	pedEndY = roadBottom + 70;                                 // 대각선 도착 y 좌표
	pedX = pedStartX;                                          // 현재 x 좌표를 시작 위치로 설정
	pedY = pedStartY;                                          // 현재 y 좌표를 시작 위치로 설정
}

void MovePerson(HWND hWnd)
{
	if (pedState == PED_CROSS_H) {                              // 사람이 가로 횡단 중이라면
		if (pedX < pedEndX) pedX += 4;                          // 오른쪽으로 조금씩 이동
		else {                                                  // 도착했다면
			pedState = PED_WAIT;                                // 대기 상태로 변경
			pedX = roadLeft - 80;                               // 다시 대기 위치 x로 이동
			pedY = centerY + 15;                                // 다시 대기 위치 y로 이동
		}
	}

	if (pedState == PED_CROSS_DIAGONAL) {                       // 사람이 대각선 횡단 중이라면
		if (pedX < pedEndX) pedX += 4;                          // 오른쪽으로 이동
		if (pedY < pedEndY) pedY += 4;                          // 아래쪽으로 이동

		if (pedX >= pedEndX && pedY >= pedEndY) {                // 대각선 횡단이 끝났다면
			pedState = PED_WAIT;                                // 사람을 대기 상태로 변경
			pedX = roadLeft - 70;                               // 대기 위치 x 좌표
			pedY = roadTop - 70;                                // 대기 위치 y 좌표
			globalStop = 0;                                     // 전체 정지 해제

			if (pendingResume == 1) {                           // 대각선 횡단 중 오른쪽 클릭을 했었다면
				pendingResume = 0;                              // 예약 해제
				SetHBlueVRed();                                 // 자동차를 다시 움직일 수 있도록 기본 신호 설정
			}
		}
	}
}

void SetHBlueVRed()
{
	hSignal = SIGNAL_BLUE;                                      // 가로 도로 신호를 파랑으로 설정
	vSignal = SIGNAL_RED;                                       // 세로 도로 신호를 빨강으로 설정
	yellowTarget = 0;                                           // 노랑 전환 상태 해제
}

void SetHRedVBlue()
{
	hSignal = SIGNAL_RED;                                       // 가로 도로 신호를 빨강으로 설정
	vSignal = SIGNAL_BLUE;                                      // 세로 도로 신호를 파랑으로 설정
	yellowTarget = 0;                                           // 노랑 전환 상태 해제
}

void StartYellow(int target)
{
	if (hSignal == SIGNAL_YELLOW || vSignal == SIGNAL_YELLOW) { // 이미 노랑 신호라면
		return;                                                 // 중복 실행하지 않음
	}

	beforeHSignal = hSignal;                                    // 노랑으로 바꾸기 전 가로 신호 저장
	beforeVSignal = vSignal;                                    // 노랑으로 바꾸기 전 세로 신호 저장
	yellowTarget = target;                                      // 어떤 신호를 기준으로 변경하는지 저장

	hSignal = SIGNAL_YELLOW;                                    // 가로 신호를 노랑으로 설정
	vSignal = SIGNAL_YELLOW;                                    // 세로 신호를 노랑으로 설정
}

void FinishYellow()
{
	if (yellowTarget == 1) {                                    // 가로 신호를 기준으로 변경 중이었다면
		if (beforeHSignal == SIGNAL_RED) SetHBlueVRed();        // 가로가 원래 빨강이면 가로 파랑으로 변경
		else SetHRedVBlue();                                    // 가로가 원래 파랑이면 가로 빨강으로 변경
	}

	if (yellowTarget == 2) {                                    // 세로 신호를 기준으로 변경 중이었다면
		if (beforeVSignal == SIGNAL_RED) SetHRedVBlue();        // 세로가 원래 빨강이면 세로 파랑으로 변경
		else SetHBlueVRed();                                    // 세로가 원래 파랑이면 세로 빨강으로 변경
	}

	if (yellowTarget == 3) {                                    // 자동 모드에서 변경 중이었다면
		if (beforeHSignal == SIGNAL_BLUE) SetHRedVBlue();       // 가로가 원래 파랑이면 세로 파랑으로 변경
		else SetHBlueVRed();                                    // 세로가 원래 파랑이면 가로 파랑으로 변경
	}

	yellowTarget = 0;                                           // 노랑 전환 상태 해제
}

int IsInsideRect(int mx, int my, RECT rc)
{
	if (mx >= rc.left && mx <= rc.right && my >= rc.top && my <= rc.bottom) { // 좌표가 영역 안이면
		return 1;                                               // 참 반환
	}
	return 0;                                                   // 아니면 거짓 반환
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;                                             // WM_PAINT에서 사용하는 그리기 구조체
	HDC hDC;                                                     // 그림을 그릴 DC 핸들
	int mx;                                                      // 마우스 x 좌표
	int my;                                                      // 마우스 y 좌표

	switch (uMsg) {                                              // 들어온 메시지 종류에 따라 처리
	case WM_CREATE:                                             // 윈도우가 처음 만들어질 때 실행
		srand((unsigned int)time(NULL));                         // 랜덤 초기화
		MakeMenu(hWnd);                                         // 메뉴 생성 후 윈도우에 붙이기
		InitCars();                                             // 자동차 초기 위치 설정
		pedX = roadLeft - 70;                                   // 사람 초기 x 좌표 설정
		pedY = roadTop - 70;                                    // 사람 초기 y 좌표 설정
		SetTimer(hWnd, TIMER_MOVE, 30, NULL);                   // 0.03초마다 자동차와 사람 이동
		break;                                                  // WM_CREATE 종료

	case WM_COMMAND:                                            // 메뉴 명령이 들어왔을 때 실행
		switch (LOWORD(wParam)) {                               // 어떤 메뉴가 눌렸는지 확인
		case MENU_H_BLUE_V_RED:                                 // 가로 파랑, 세로 빨강 메뉴
			globalStop = 0;                                     // 전체 정지 해제
			SetHBlueVRed();                                     // 가로 파랑, 세로 빨강 설정
			InvalidateRect(hWnd, NULL, TRUE);                   // 화면 다시 그리기
			break;                                              // 메뉴 처리 종료

		case MENU_H_RED_V_BLUE:                                 // 가로 빨강, 세로 파랑 메뉴
			globalStop = 0;                                     // 전체 정지 해제
			SetHRedVBlue();                                     // 가로 빨강, 세로 파랑 설정
			InvalidateRect(hWnd, NULL, TRUE);                   // 화면 다시 그리기
			break;                                              // 메뉴 처리 종료

		case MENU_AUTO:                                         // 자동 신호 변경 메뉴
			autoMode = 1;                                       // 자동 모드 켜기
			SetTimer(hWnd, TIMER_AUTO, 3000, NULL);             // 3초마다 자동 신호 변경
			break;                                              // 메뉴 처리 종료

		case MENU_STOP:                                         // 자동 멈춤 메뉴
			autoMode = 0;                                       // 자동 모드 끄기
			KillTimer(hWnd, TIMER_AUTO);                        // 자동 신호 타이머 제거
			break;                                              // 메뉴 처리 종료

		case MENU_QUIT:                                         // 종료 메뉴
			DestroyWindow(hWnd);                                // 윈도우 종료
			break;                                              // 메뉴 처리 종료
		}
		break;                                                  // WM_COMMAND 종료

	case WM_TIMER:                                              // 타이머 메시지가 들어왔을 때 실행
		if (wParam == TIMER_MOVE) {                             // 이동 타이머라면
			MoveCars();                                         // 자동차 이동 처리
			MovePerson(hWnd);                                   // 사람 이동 처리
			InvalidateRect(hWnd, NULL, TRUE);                   // 화면 다시 그리기
		}

		if (wParam == TIMER_YELLOW) {                           // 노랑 신호 1초 타이머라면
			KillTimer(hWnd, TIMER_YELLOW);                      // 노랑 타이머 종료
			FinishYellow();                                     // 실제 빨강/파랑 신호로 변경
			if (hSignal == SIGNAL_RED && pedState == PED_WAIT && globalStop == 0) { // 가로가 빨강이고 사람이 대기 중이면
				StartHorizontalWalk();                          // 사람이 가로 횡단보도 건너기 시작
			}
			InvalidateRect(hWnd, NULL, TRUE);                   // 화면 다시 그리기
		}

		if (wParam == TIMER_AUTO) {                             // 자동 신호 변경 타이머라면
			if (autoMode == 1 && hSignal != SIGNAL_YELLOW && vSignal != SIGNAL_YELLOW) { // 자동 모드이고 노랑 상태가 아니라면
				beforeHSignal = hSignal;                        // 현재 가로 신호 저장
				beforeVSignal = vSignal;                        // 현재 세로 신호 저장
				yellowTarget = 3;                               // 자동 변경 상태 저장
				hSignal = SIGNAL_YELLOW;                        // 가로 신호 노랑
				vSignal = SIGNAL_YELLOW;                        // 세로 신호 노랑
				SetTimer(hWnd, TIMER_YELLOW, 1000, NULL);       // 1초 뒤 신호 변경
			}
		}
		break;                                                  // WM_TIMER 종료

	case WM_LBUTTONDOWN:                                        // 왼쪽 마우스 버튼을 눌렀을 때 실행
		globalStop = 1;                                         // 모든 자동차 정지 상태로 설정
		StartDiagonalWalk();                                    // 사람이 대각선 횡단 시작
		InvalidateRect(hWnd, NULL, TRUE);                       // 화면 다시 그리기
		break;                                                  // WM_LBUTTONDOWN 종료

	case WM_RBUTTONDOWN:                                        // 오른쪽 마우스 버튼을 눌렀을 때 실행
		if (pedState == PED_CROSS_DIAGONAL) {                   // 사람이 아직 대각선 횡단 중이라면
			pendingResume = 1;                                  // 횡단이 끝난 뒤 다시 움직이도록 예약
		}
		else {                                                  // 사람이 대각선 횡단 중이 아니라면
			globalStop = 0;                                     // 전체 정지 해제
			SetHBlueVRed();                                     // 가로 파랑, 세로 빨강으로 기본 재시작
		}
		InvalidateRect(hWnd, NULL, TRUE);                       // 화면 다시 그리기
		break;                                                  // WM_RBUTTONDOWN 종료

	case WM_LBUTTONDBLCLK:                                      // 왼쪽 더블클릭은 따로 사용하지 않음
		break;                                                  // WM_LBUTTONDBLCLK 종료

	case WM_KEYDOWN:                                            // 키보드를 눌렀을 때 실행
		if (wParam == VK_OEM_PLUS || wParam == L'+') {          // + 키를 눌렀다면
			speedPlus++;                                        // 자동차 추가 속도 증가
		}

		if (wParam == VK_OEM_MINUS || wParam == L'-') {         // - 키를 눌렀다면
			speedPlus--;                                        // 자동차 추가 속도 감소
			if (speedPlus < -2) speedPlus = -2;                 // 너무 느려지지 않도록 제한
		}

		if (wParam == L'a' || wParam == L'A') {                 // A 키를 눌렀다면
			if (autoMode == 0) {                                // 자동 모드가 꺼져 있으면
				autoMode = 1;                                   // 자동 모드 켜기
				SetTimer(hWnd, TIMER_AUTO, 3000, NULL);         // 자동 신호 변경 타이머 시작
			}
			else {                                              // 자동 모드가 켜져 있으면
				autoMode = 0;                                   // 자동 모드 끄기
				KillTimer(hWnd, TIMER_AUTO);                    // 자동 신호 변경 타이머 제거
			}
		}

		if (wParam == L'q' || wParam == L'Q') {                 // Q 키를 눌렀다면
			DestroyWindow(hWnd);                                // 프로그램 종료
		}

		InvalidateRect(hWnd, NULL, TRUE);                       // 화면 다시 그리기
		break;                                                  // WM_KEYDOWN 종료

	case WM_LBUTTONUP:                                          // 왼쪽 마우스 버튼을 뗐을 때
		break;                                                  // 현재는 사용하지 않음

	case WM_MOUSEMOVE:                                          // 마우스 이동 메시지
		break;                                                  // 현재는 사용하지 않음

	case WM_PAINT:                                              // 화면을 다시 그려야 할 때 실행
		hDC = BeginPaint(hWnd, &ps);                            // 그리기 시작

		DrawRoad(hDC);                                          // 사거리 도로 그리기
		DrawSignals(hDC);                                       // 신호등 그리기
		DrawCars(hDC);                                          // 자동차 8대 그리기
		DrawPeople(hDC);                                        // 사람 그리기

		EndPaint(hWnd, &ps);                                    // 그리기 종료
		break;                                                  // WM_PAINT 종료

	case WM_DESTROY:                                            // 윈도우가 종료될 때 실행
		KillTimer(hWnd, TIMER_MOVE);                            // 이동 타이머 제거
		KillTimer(hWnd, TIMER_YELLOW);                          // 노랑 신호 타이머 제거
		KillTimer(hWnd, TIMER_AUTO);                            // 자동 신호 타이머 제거
		PostQuitMessage(0);                                     // 프로그램 종료 메시지 발생
		break;                                                  // WM_DESTROY 종료
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);           // 처리하지 않은 메시지는 기본 처리 함수로 넘김
}