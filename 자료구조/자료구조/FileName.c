#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>


int main() {
	int a;
	printf("출력할 줄 수를 입력 하세요 : ");

	scanf("%d", &a);


	for (int i = 0; i <= a; i++) {
		for (int k = 1; k <= i; k++) {
		printf("%d",k);
		Sleep(100);
	}
		printf("\n");
	}
	return 0;
}