#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void array2slinkedlist();
void printall();
struct node* search(char name[]);
void Delete(char name[]);
void deleteall(char pos[50]);
void findmaxhp();
void insert();

struct LOL_DIC {
	char name[50];
	int hp;
	int mp;
	int speed;
	int range;
	char pos[50];
};

struct node {
	struct LOL_DIC data;
	struct node* next;
};

struct node* head;
struct node* tail;
struct LOL_DIC loldic[100];
int count = 0;

void array2slinkedlist() {
	for (int i = 0; i < count; i++) {
		struct node* newnode;
		newnode = (struct node*)malloc(sizeof(struct node));
		
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

void printall() {
	if (head == NULL) {
		printf("챔피언이 없습니다\n");
		return;
	}
	struct node* cur = head;
	for (int i = 0; i < count; i++) {
		printf("name: %s\n hp: %d\n mp: %d\n speed: %d\n range: %d\n pos: %s\n\n",
			cur->data.name,
			cur->data.hp,
			cur->data.mp,
			cur->data.speed,
			cur->data.range,
			cur->data.pos);
		cur = cur->next;
	}
}

struct node* search(char name[]) {
	struct node* cur;
	struct node* prev;
	int found = 0;

	if (head == NULL) {
		return NULL;
	}

	cur = head;
	prev = tail;

	for (int i = 0; i < count; i++) {
		if (strcmp(name, cur->data.name) == 0) {
			found = 1;
			return prev;
		}
		prev = cur;
		cur = cur->next;
	}

	if (found != 1) {
		return NULL;
	}
}

void Delete(char name[]) {

	if (head == NULL) {
		printf("챔피언이 없습니다\n");
		return;
	}
	struct node* prev = search(name);
	
	if (prev == NULL) {
		printf("챔피언이 없습니다\n");
		return;
	}
	struct node* cur = prev->next;

	if (count == 1) {
		free(cur);
		head = NULL;
		tail = NULL;
		count--;
		printf("%s 챔피언을 삭제했습니다\n", name);
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
	
	printf("%s 챔피언을 삭제했습니다\n", name);

	return;
}

void deleteall(char pos[]) {
	int deleted = 0;
	
	while (1) {
		int found = 0;
		struct node* cur = head;
		for (int i = 0; i < count; i++) {
			if (strcmp(pos, cur->data.pos) == 0) {
				char name[50];
				strcpy(name, cur->data.name);
				Delete(name);
				found = 1;
				deleted++;
				break;
			}
			cur = cur->next;
		}
		if (found == 0) {
			break;
		}
	}
	printf("%s 포지션 챔피언 %d명을 삭제 했습니다.\n", pos, deleted);

}

void findmaxhp() {
	if (head == NULL) {
		printf("챔피언이 없습니다\n");
		return;
	}

	struct node* cur = head;
	struct node* maxhp = head;

	for (int i = 0; i < count; i++) {
		if (maxhp->data.hp < cur->data.hp) {
			maxhp = cur;
		}
		cur = cur->next;
	}
	cur = head;
	for (int i = 0; i < count; i++) {
		if (maxhp->data.hp == cur->data.hp) {
			printf("name: %s\n hp: %d\n mp: %d\n speed: %d\n range: %d\n pos: %s\n\n",
				cur->data.name,
				cur->data.hp,
				cur->data.mp,
				cur->data.speed,
				cur->data.range,
				cur->data.pos);	
		}
		cur = cur->next;
	}
}

void insert() {
	struct node* newnode;
	struct LOL_DIC temp;
	struct node* cur = head;
	struct node* prev = tail;

	newnode = (struct node*)malloc(sizeof(struct node));

	if (newnode == NULL) {
		printf("메모리 할당 실패\n");
		return;
	}

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
	fgets(temp.pos, sizeof(temp.pos), stdin);
	temp.pos[strcspn(temp.pos, "\n")] = '\0';

	newnode->data = temp;
	newnode->next = NULL;

	if (head == NULL) {
		head = newnode;
		tail = newnode;
		tail->next = head;
		count++;
		return;
	}

	if (newnode->data.hp > head->data.hp) {
		newnode->next = head;
		head = newnode;
		tail->next = head;
		count++;
		return;
	}

	prev = head;
	cur = head->next;

	while (cur != head) {
		if (temp.hp > cur->data.hp) {
			break;
		}
		prev = cur;
		cur = cur->next;
	}

	prev->next = newnode;
	newnode->next = cur;
	
	if (cur == head) {
		tail = newnode;
	}

	tail->next = head;
	count++;
	return;
}

int main() {
	FILE* fp;
	char order[50];
	char line[200];

	fp = fopen("testdata.txt", "r");
	if (fp == NULL) {
		printf("파일을 불러올수 없습니다\n");
		return -1;
	}

	while (fgets(line, sizeof(line), fp)) {
		line[strcspn(line, "\n")] = '\0';

		sscanf(line, "%[^0-9] %d %d %d %d %s", loldic[count].name, &loldic[count].hp, &loldic[count].mp, &loldic[count].speed, &loldic[count].range, loldic[count].pos);
		loldic[count].name[strcspn(loldic[count].name, "\t")] = '\0';
		count++;
	}

	fclose(fp);

	array2slinkedlist();

	while (1) {
		printf("명령어를 입력하세요\n(search [name], printAll, delete [name], deleteAll [position], insert, findmaxhp, sortbyhp)\n");
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';

		if (strncmp(order, "search ", 7) == 0) {
			char name[50];
			strcpy(name, order + 7);

			struct node* prev = search(name);
			if (prev == NULL) {
				printf("챔피언을 찾을수 없습니다\n");
			}
			else {
				struct node* cur = prev->next;
				printf("name: %s\n hp: %d\n mp: %d\n speed: %d\n range: %d\n pos: %s\n\n",
					cur->data.name,
					cur->data.hp,
					cur->data.mp,
					cur->data.speed,
					cur->data.range,
					cur->data.pos);
			}

		}
		else if (strcmp(order, "insert") == 0) {
			insert();
		}
		else if (strncmp(order, "delete ", 7) == 0) {
			char name[50];
			strcpy(name, order + 7);
			Delete(name);
		}
		else if (strncmp(order, "deleteall ", 10) == 0) {
			char pos[50];
			strcpy(pos, order + 10);
			deleteall(pos);
		}
		else if (strcmp(order, "printall") == 0) {
			printall();
		}
		else if (strcmp(order, "findmaxhp") == 0) {
			findmaxhp();
		}
		else if (strcmp(order, "sortbyhp") == 0) {

		}
		else {
			printf("잘못된 명령어 입니다\n");
		}
	}
}