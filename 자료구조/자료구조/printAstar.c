#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

int star(int n) {
	if (n == 0) {
		return 0;
	}

	star(n - 1);

	for (int i = 0; i < n; i++) {
		printf("%d",i+1);
		Sleep(200);
	}
	printf("\n");

	return 0;
}

int main() {
	printf("줄 수 입력 : ");
	int a;
	scanf("%d", &a);

	star(a);

	return 0;
}