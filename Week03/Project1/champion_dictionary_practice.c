#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>

struct LOL_Dic
{
	char name[50];
	int hp;
	int mp;
	int speed;
	int range;
	char position[20];
};

int main() {
	struct LOL_Dic loldic[100] = {
	{ "Jayce",3466,8307,452,73,"mid" },
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

	int count = 22;
	char order[50];

	printf("명령어를 입력하세요\n");
	printf("Search [챔피언이름] - 주어진 챔피언의 정보를 출력한다\n");
	printf("Insert - 새로운 챔피언의 정보를 입력받아 삽입한다\n");
	printf("Delete [챔피언이름] - 주어진 챔피언에 대한 정보를 삭제한다\n");
	printf("DeleteAll [position] - 주어진 포지션의 모든 챔피언 정보를 삭제한다\n");
	printf("PrintAll - 모든 챔피언의 정보를 배열에 저장된 순서대로 출력한다\n");
	printf("FindMaxHp - 가장 체력이 큰 챔피언의 정보를 출력한다\n");
	printf("SortByHp - 체력이 큰 챔피언부터 순서대로 저장한다\n");
	printf("End - 프로그램을 종료합니다\n");

	while (1) {
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';		//공부

		if (strncmp(order, "Search ", 7) == 0) {
			char searchname[50];
			strcpy(searchname, order + 7);
			int searchcount = 0;

			for (int i = 0; i < count; i++) {
				if (strcmp(searchname, loldic[i].name) == 0) {
					searchcount = 1;
					printf("이름: %s\n", loldic[i].name);
					printf("hp: %d\n", loldic[i].hp);
					printf("mp: %d\n", loldic[i].mp);
					printf("speed: %d\n", loldic[i].speed);
					printf("range: %d\n", loldic[i].range);
					printf("position: %s\n", loldic[i].position);
				}
				
			}

			if (searchcount == 0)
				printf("입력한 챔피언 정보가 없습니다\n");

		}
		else if (strcmp(order, "Insert") == 0) {
		

			if (count >= 100) {
				printf("더 이상 챔피언을 추가할수 없습니다\n");
			}
			else {
				printf("추가할 챔피언의 정보를 입력 하세요\n");

				printf("이름: ");
				fgets(loldic[count].name, sizeof(loldic[count].name), stdin);
				loldic[count].name[strcspn(loldic[count].name, "\n")] = '\0';

				printf("hp: ");
				scanf("%d", &loldic[count].hp);

				printf("mp: ");
				scanf("%d", &loldic[count].mp);

				printf("speed: ");
				scanf("%d", &loldic[count].speed);

				printf("range: ");
				scanf("%d", &loldic[count].range);

				getchar();

				printf("position: ");
				fgets(loldic[count].position, sizeof(loldic[count].position), stdin);
				loldic[count].position[strcspn(loldic[count].position, "\n")] = '\0';

				count++;
				printf("챔피언 %s가 저장되었습니다\n", loldic[count - 1].name);
			}
		}
		else if (strncmp(order, "Delete ", 7) == 0) {
			char delname[30];
			int delt = 0;
			strcpy(delname, order + 7);

			for (int i = 0; i < count; i++) {
				if (strcmp(delname, loldic[i].name)==0) {
					delt = 1;
					for (int k = i; k < count - 1; k++) {
						loldic[k] = loldic[k + 1];
					}
					count--;
					printf("챔피언 정보가 삭제되었습니다\n");
					break;
				}
				
			}
			if (!delt) {
				printf("삭제하려는 챔피언이 없습니다\n");
			}
				

		}

		else if (strncmp(order, "DeleteAll ", 10) == 0) {
			char delpos[20];
			int delcount = 0;
			int i = 0;

			strcpy(delpos, order + 10);
			while (i < count) {
				if (strcmp(delpos, loldic[i].position) == 0) {

					for (int k = i; k < count - 1; k++) {
						loldic[k] = loldic[k + 1];
					}
					count--;
					delcount++;
				}
				else {
					i++;
				}
			}
			if (delcount > 0) {
				printf("%s 포지션을 가진 챔피언 %d개를 삭제 했습니다\n", delpos, delcount);
			}
			else {
				printf("%s 포지션을 가진 챔피언이 없습니다\n", delpos);
			}
		}

		else if (strcmp(order, "PrintAll") == 0) {
			for (int i = 0; i < count; i++) {
				printf("이름: %s\n", loldic[i].name);
				printf("hp: %d\n", loldic[i].hp);
				printf("mp: %d\n", loldic[i].mp);
				printf("speed: %d\n", loldic[i].speed);
				printf("range: %d\n", loldic[i].range);
				printf("position: %s\n\n", loldic[i].position);
			}
		}
		else if (strcmp(order, "FindMaxHp") == 0) {
			int maxhp = 0;

			for (int i = 0; i < count; i++) {
				if (loldic[i].hp > loldic[maxhp].hp) {
					maxhp = i;
				}
			}
			for (int i = 0; i < count; i++) {
				if (loldic[i].hp == loldic[maxhp].hp) {
					printf("이름: %s\n", loldic[i].name);
					printf("hp: %d\n", loldic[i].hp);
					printf("mp: %d\n", loldic[i].mp);
					printf("speed: %d\n", loldic[i].speed);
					printf("range: %d\n", loldic[i].range);
					printf("position: %s\n\n", loldic[i].position);
				}
			}
		}

		else if (strcmp(order, "SortByHp") == 0) {
			
			for (int i = 0; i < count - 1; i++) {
				for (int k = i + 1; k < count; k++) {
					if (loldic[i].hp < loldic[k].hp) {
						struct LOL_Dic temp;
						temp = loldic[i];
						loldic[i] = loldic[k];
						loldic[k] = temp;
					}
				}
			}
			printf("hp가 큰 순서대로 저장되었습니다\n");
		}
		else if (strcmp(order, "End") == 0) {
			printf("프로그램을 종료합니다");
			break;
		}
		else {
			printf("잘못된 명령어 입니다\n");
		}
	}
}

