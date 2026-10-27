#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// 전역변수
int count = 0;

//함수 선언
void Array2SLinkedList();
void printAll();
struct NODE* search(char name[]);
void Delete(char name[]);
void deleteAll(char position[]);
void findmaxhp();
void insert();
void sortbyhp();

// LOL_DIC 구조체
struct LOL_DIC {
	char name[50];
	int hp;
	int mp;
	int speed;
	int range;
	char position[50];
};

// NODE 구조체
struct NODE {
	struct LOL_DIC data;
	struct NODE* next;
};


struct NODE* head = NULL;
struct NODE* tail = NULL;
struct LOL_DIC loldic[100];

//배열을 연결리스트로 복사하는 함수
void Array2SLinkedList() {
	for (int i = 0; i < count; i++) {
		struct NODE* newnode = (struct NODE*)malloc(sizeof (struct NODE));

		if (newnode == NULL) {
			printf("메모리 할당 실패\n");
			return;
		}

		newnode->data = loldic[i];
		newnode->next = NULL;

		if (head == NULL) {
			head = newnode;
			tail = newnode;
			tail->next = head;
		}
		else {
			tail->next = newnode;
			tail = newnode;
			tail->next = head;
		}
	}
}

void printAll() {
	struct NODE* cur = head;

	if (head == NULL) {
		printf("챔피언이 없습니다\n");
	}
	else {
		for (int i = 0; i < count; i++) {
			printf("name: %s\nhp: %d\nmp: %d\nspeed: %d\nrange: %d\nposition: %s\n\n",
				cur->data.name,
				cur->data.hp,
				cur->data.mp,
				cur->data.speed,
				cur->data.range,
				cur->data.position);

			cur = cur->next;
		}
	}
}

struct NODE* search(char name[]) {
	struct NODE* cur = head;
	struct NODE* prev = tail;

	if (head == NULL) {
		return NULL;
	}
	for (int i = 0; i < count; i++) {
		if (strcmp(name, cur->data.name) == 0) {
			return prev;
		}
		prev = cur;
		cur = cur->next;
	}
	return NULL;
}
void Delete(char name[]) {
	
	if (head == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}
	
	struct NODE* prev = search(name);
	
	if (prev == NULL) {
		printf("삭제할 챔피언이 없습니다\n");
		return;
	}
	
	struct NODE* cur = prev->next;

	if (count == 1) {
		free(cur);
		head = NULL;
		tail = NULL;
		count--;
		printf("%s 챔피언을 삭제했습니다.\n", name);
		return;
	}

	prev->next = cur->next;

	if (cur == head) {
		head = cur->next;
	}

	if (cur == tail) {
		tail = prev;
	}

	tail->next = head;
	free(cur);
	count--;

	printf("%s 챔피언을 삭제했습니다.\n", name);
}

void deleteAll(char position[]) {
	int deleted = 0;

	while (1) {
		struct NODE* cur = head;
		int found_index = 0;

		if (head == NULL) {
			break;
		}

		for (int i = 0; i < count; i++) {
			if (strcmp(position, cur->data.position)==0) {
				char posname[50];

				strcpy(posname, cur->data.name);
				Delete(posname);

				deleted++;
				found_index = 1;
				break;
			}
			cur = cur->next;
		}
		if (found_index == 0) {
			break;
		}
	}
	printf("%s 포지션 챔피언 %d명을 삭제 했습니다.\n", position, deleted);
}

void findmaxhp() {
	struct NODE* cur = head;
	

	if (head == NULL) {
		printf("챔피언이 없습니다\n");
		return;
	}

	int maxhp = cur->data.hp;
	
	for (int i = 0; i < count; i++) {
		if (maxhp < cur->data.hp) {
			maxhp = cur->data.hp;
		}
		cur = cur->next;
	}
	
	cur = head;
	
	for (int i = 0; i < count; i++) {
		if (cur->data.hp == maxhp) {
			printf("name: %s\nhp: %d\nmp: %d\nspeed: %d\nrange: %d\nposition: %s\n\n",
				cur->data.name,
				cur->data.hp,
				cur->data.mp,
				cur->data.speed,
				cur->data.range,
				cur->data.position);
		}
		cur = cur->next;
	}
}

void insert() {
	struct NODE* newnode;
	struct NODE* cur;
	struct NODE* prev;
	struct LOL_DIC temp;

	printf("name: ");
	fgets(temp.name, sizeof(temp.name), stdin);
	temp.name[strcspn(temp.name, "\n")] = '\0';

	printf("hp: ");
	scanf("%d", &temp.hp);

	printf("mp: ");
	scanf("%d", &temp.mp);

	printf("speed: ");
	scanf("%d", &temp.speed);

	printf("range: ");
	scanf("%d", &temp.range);
	
	getchar();

	printf("position: ");
	fgets(temp.position, sizeof(temp.position), stdin);
	temp.position[strcspn(temp.position, "\n")] = '\0';

	newnode = (struct NODE*)malloc(sizeof(struct NODE));
	newnode->data = temp;
	newnode->next = NULL;

	if (head == NULL) {
		head = newnode;
		tail = newnode;
		tail->next = head;
		count++;
		return;
	}

	if (temp.hp > head->data.hp) {
		newnode->next = head;
		head = newnode;
		tail->next = head;
		count++;
		return;
	}

	prev = head;
	cur = head->next;

	while (cur != head) {
		if (newnode->data.hp > cur->data.hp) {
			break;
		}
		prev = cur;
		cur = cur->next;
	}

	prev->next = newnode;
	newnode->next = cur;

	if (newnode->next == head) {
		tail = newnode;
	}
	tail->next = head;
	count++;
}

void sortbyhp() {
	struct NODE* s_head = NULL;
	struct NODE* s_tail = NULL;
	
	if (head == NULL || count <= 1) {
		return;
	}

	int s_count = count;
	
	for (int i = 0; i < s_count; i++) {
		struct NODE* maxcur = head;
		struct NODE* maxprev = tail;
		struct NODE* cur = head;
		struct NODE* prev = tail;

		for (int k = 0; k < count; k++) {
			if (maxcur->data.hp < cur->data.hp) {
				maxcur = cur;
				maxprev = prev;
			}
			prev = cur;
			cur = cur->next;
		}

		if (count == 1) {
			head = NULL;
			tail = NULL;
		}
		else {
			maxprev->next = maxcur->next;
			
			if (maxcur == head) {
				head = maxcur->next;
			}
			if (maxcur == tail) {
				tail = maxprev;
			}
			tail->next = head;
		}
		count--;

		maxcur->next = NULL;

		if (s_head == NULL) {
			s_head = maxcur;
			s_tail = maxcur;
			s_tail->next = s_head;
		}
		else {
			s_tail->next = maxcur;
			s_tail = maxcur;
			s_tail->next = s_head;
		}
	}

	head = s_head;
	tail = s_tail;
	count = s_count;

}

int main() {
	FILE* fp;
	char line[200];
	char order[50];


	fp = fopen("testdata.txt", "r");

	if (fp == NULL) {
		printf("파일을 열수 없습니다.");
		return -1;
	}

	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0';

		sscanf(line, "%[^0-9] %d %d %d %d %s", loldic[count].name, &loldic[count].hp, &loldic[count].mp, &loldic[count].speed, &loldic[count].range, loldic[count].position);
		loldic[count].name[strcspn(loldic[count].name, "\t")] = '\0';

		count++;
	}

	fclose(fp);

	Array2SLinkedList();

	while (1) {
		printf("명령어를 입력하세요\n(search [name], printAll, delete [name], deleteAll [position], insert, findmaxhp, sortbyhp)\n");
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';

		if (strncmp(order, "search ", 7) == 0) {
			char name[50];
			strcpy(name, order + 7);

			struct NODE* prev = search(name);
			
			if (prev == NULL) {
				printf("챔피언을 찾을수 없습니다\n");
			}
			else {
				struct NODE* cur = prev->next;
				printf("name: %s\nhp: %d\nmp: %d\nspeed: %d\nrange: %d\nposition: %s\n\n",
					cur->data.name,
					cur->data.hp,
					cur->data.mp,
					cur->data.speed,
					cur->data.range,
					cur->data.position);
			}
		}
		else if (strncmp(order, "delete ", 7) == 0) {
			char name[50];
			strcpy(name, order + 7);
			Delete(name);
		}
		else if (strcmp(order, "insert") == 0) {
			insert();
		}
		else if (strncmp(order, "deleteAll ", 10) == 0) {
			char position[50];
			strcpy(position, order + 10);
			deleteAll(position);
		}
		else if (strcmp(order, "printAll") == 0) {
			printAll();
		}
		else if (strcmp(order, "findmaxhp") == 0) {
			findmaxhp();
		}
		else if (strcmp(order, "sortbyhp") == 0) {
			sortbyhp();
		}
		else {
			printf("잘못된 명령어 입니다\n");
		}
		
	}

}