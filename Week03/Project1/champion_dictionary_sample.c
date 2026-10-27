#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <string.h>

struct LOL_Dic {
	char name[20];
	int hp;
	int mp;
	int speed;
	int range;
	char position[20];
};

int main() {

	struct LOL_Dic loldic[50] = {
		{"Jayce",3466, 8307, 452, 73, "mid"},
		{"Vayne",3318,2790,390,60,"support"},
		{"Lee Sin",9864,6257,747,19,"support"},
		{ "Teemo",1915,1313,700,68,"mid" },
		{ "Nami",2968,2153,803,74,"support" },
		{ "Jinx",2583,	3042,123,53,"jungle" },
		{ "Cassiopeia",6067,7330,683,24,"top" },
		{ "Ahri",4573,	7079,177,79,"top" },
		{ "Fiora",2591,9329,337,81,"jungle" },
		{ "Zac"	,9864,7272,405,	49,"bottom" },
		{ "Janna",4922,2275,690,66,"jungle" },
		{ "Ashe",7421,5549,428,	31,"top" },
		{ "Twitch",2980,7703,204,91,"mid" },
		{ "Ezreal",3660,9835,274,44,"mid" },
		{ "Sona",5655,8442,252,	26,"bottom" },
		{ "Lulu",3231,2014,503,	32,	"support" },
		{ "Amumu",6763	,3994,427,19,"jungle" },
		{ "Master Yi",5737,7168,434,61,	"top" },
		{ "Jarvan",1895,3070,323,80,"mid" },
		{ "Shen",4794,9220,783,42,"mid" },
		{ "Fizz",9864,1913,433,38,"bottom" },
		{ "Taric",6158,7451,160,36,	"bottom" }
	};

	printf("명령어를 입력 하세요.\n");
	printf("챔피언 이름 - 주어진 챔피언의 정보를 출력한다\n");
	printf("Insert [챔피언이름] - 새로운 챔피언의 정보를 입력받아 삽입한다\n");
	printf("Delete [챔피언이름] - 주어진 챔피언에 대한 정보를 삭제한다\n");
	printf("DeleteALL [position] - 주어진 위치의 모든 챔피언 정보를 삭제한다\n");
	printf("PrintALL - 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다\n");
	printf("FindMaxHp - 가장 체력이 큰 챔피언의 정보를 출력한다\n");
	printf("SortByHp - 체력이 큰 챔피언부터 순서대로 저장한다\n");
	printf("End - 프로그램을 종료 합니다\n");

}