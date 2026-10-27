#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <string.h>

struct LOL_Dic
{
	char name[50];
	int hp;
	int mp;
	int speed;
	int range;
	char position[20];
	int used;
};
struct LOL_Dic loldic[100] =
{
	{"Jayce",3466,8307,452,73,"mid"},
	{ "Vayne",3318,2790,390,60,"support" },
	{ "Lee Sin",9864,6257,747,19,"support" },
	{ "Teemo",1915,1313,700,68,"mid" },
	{ "Nami",2968,2153,803,74,"support" },
	{ "Jinx",2583,3042,123,53,"jungle" },
	{ "Cassiopeia",6067,7330,683,24,"top" },
	{ "Ahri",4573,7079,177,79,"top" },
	{ "Fiora",2591,9329,337,81,"jungle" },
	{ "Zac",9864,7272,405,49,"bottom" },
	{ "Janna",4922,2275,690,66,"jungle" },
	{ "Ashe",7421,5549,428,31,"top" },
	{ "Twitch",2980,7703,204,91,"mid" },
	{ "Ezreal",3660,9835,274,44,"mid" },
	{ "Sona",5655,8442,252,26,"bottom" },
	{ "Lulu",3231,2014,503,32,"support" },
	{ "Amumu",6763,3994,427,19,"jungle" },
	{ "Master Yi",5737,7168,434,61,"top" },
	{ "Jarvan",1895,3070,323,80,"mid" },
	{ "Shen",4794,9220,783,42,"mid" },
	{ "Fizz",9864,1913,433,38,"bottom" },
	{ "Taric",6158,7451,160,36,"bottom" }
};

void printfch(struct LOL_Dic champ){
	printf("이름: %s\n", champ.name);
	printf("hp: %d\n", champ.hp);
	printf("mp: %d\n", champ.mp);
	printf("speed: %d\n", champ.speed);
	printf("range: %d\n", champ.range);
	printf("position: %s\n\n", champ.position);
}

void Search(int i, char searchname[]) {
	if (i >= 100) {
		printf("찾으려는 챔피언이 없습니다.\n");
		return;
	}

	if (strcmp(loldic[i].name, searchname) == 0) {
		printfch(loldic[i]);
		return;
	}
	Search(i + 1, searchname);
}

void PrintAll(int i) {
	if (i >= 100) {
		return;
	}
	if (loldic[i].used == 1) {
		printfch(loldic[i]);
	}
	PrintAll(i + 1);
}
void Insert(int i, char name[], int hp, int mp, int speed, int range, char pos[]) {
	if (i >= 100) {
		return;
	}
	if (loldic[i].used == 0) {
		strcpy(loldic[i].name, name);
		loldic[i].hp = hp;
		loldic[i].mp = mp;
		loldic[i].speed = speed;
		loldic[i].range = range;
		strcpy(loldic[i].position, pos);
		loldic[i].used = 1;
		printf("챔피언이 추가 되었습니다.\n");
		return;
	}
	Insert(i+1, name, hp, mp, speed, range, pos);
}
void Delete(int i, char delname[]) {
	if (i >= 100) {
		return;
	}

	if (loldic[i].used == 1 && strcmp(loldic[i].name , delname)==0) {
		loldic[i] = (struct LOL_Dic){ 0 };
	}
	Delete(i + 1, delname);
}

void DeleteAll(int i, char delpos[]) {
	if (i >= 100) {
		printf("삭제할 포지션의 챔피언이 없습니다\n");
		return;
	}
	int posnum = 0;
	if (loldic[i].used == 1 && strcmp(loldic[i].position, delpos) == 0) {
		loldic[i] = (struct LOL_Dic){ 0 };
		posnum = 1;
		return;
	}
	DeleteAll(i + 1, delpos);
}
int FindMaxHp(int i) {
	if (i >= 100) {
		return -1;
	}
	int next = FindMaxHp(i + 1);
	if (next == -1) {
		return i;
	}
	if (loldic[i].used == 0) {
		return next;
	}
	if(loldic[i].hp > loldic[next].hp){
		return i;
	}
	else {
		return next;
	}
}
void swap(int a, int b) {
	struct LOL_Dic temp = loldic[a];
	loldic[a] = loldic[b];
	loldic[b] = temp;
}
void sortbyhp() {
	int i, maxhpindex;
	for (i = 0; i < 99; i++) {
		if (loldic[i].used == 0) {
			continue;
		}
		maxhpindex = FindMaxHp(i);
		if (maxhpindex != -1 && maxhpindex != i) {
			swap(i, maxhpindex);
		}
	}
	FindMaxHp(0);
}

int main() {
	for (int i = 0; i < 22; i++) {
		loldic[i].used = 1;
	 }
	for (int i = 22; i < 100; i++) {
		loldic[i].used = 0;
	}

	printf("명령어를 입력하세요\n");
	printf("Search [챔피언이름] - 주어진 챔피언의 정보를 출력한다\n");
	printf("Insert - 새로운 챔피언의 정보를 입력받아 삽입한다\n");
	printf("Delete [챔피언이름] - 주어진 챔피언에 대한 정보를 삭제한다\n");
	printf("DeleteAll [position] - 주어진 포지션의 모든 챔피언 정보를 삭제한다\n");
	printf("PrintAll - 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다\n");
	printf("FindMaxHp - 가장 체력이 큰 챔피언의 정보를 출력한다\n");
	printf("SortByHp - 체력이 큰 챔피언부터 순서대로 저장한다\n");
	printf("End - 프로그램을 종료합니다\n");

	char order[50];
	while (1) {
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';

		if (strcmp(order, "End") == 0) {
			break;
		}
		else if (strcmp(order, "PrintAll") == 0) {
			PrintAll(0);
		}
		else if (strncmp(order, "Search ", 7)==0) {
			char searchname[50];
			strcpy(searchname, order + 7);
			Search(0, searchname);
		}
		else if (strcmp(order, "Insert") == 0) {
			char name[50];
			int hp, mp, speed, range;
			char pos[20];
			printf("정보를 입력 하세요\n");
			printf("이름;");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';
			
			printf("hp:");
			scanf_s("%d", &hp);
			printf("mp:");
			scanf_s("%d", &mp);
			printf("speed:");
			scanf_s("%d", &speed);
			printf("range:");
			scanf_s("%d", &range);
			printf("position:");
			getchar();
			fgets(pos, sizeof(pos), stdin);
			pos[strcspn(pos, "\n")] = '\0';

			Insert(0, name, hp, mp, speed, range, pos);
		}
		else if (strncmp(order, "Delete ", 7) == 0) {
			char delname[50];
			strcpy(delname, order + 7);
			Delete(0, delname);
		}
		else if (strncmp(order, "DeleteAll ", 10) == 0) {
			char delpos[50];
			strcpy(delpos, order + 10);
			DeleteAll(0, delpos);
			
		}
		else if (strcmp(order, "FindMaxHp") == 0) {
			int maxhp;
			maxhp = FindMaxHp(0);
			for (int i = 0; i < 100; i++) {
				if (loldic[i].hp == loldic[maxhp].hp) {
					printfch(loldic[i]);
				}
			}

		}
		else if (strcmp(order, "SortByHp") == 0) {
			sortbyhp();
		}
		else {
			printf("잘못된 명령어 입니다\n");
		}
	}
}