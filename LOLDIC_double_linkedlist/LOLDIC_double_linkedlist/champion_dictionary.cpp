#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct LOL_DIC {
	char name[50];
	int hp;
	int mp;
	int speed;
	int range;
	char pos[50];
};

struct LOL_DIC loldic[100];
int count = 0;

struct node {
	struct LOL_DIC data;
	struct node* prev;
	struct node* next;
};

struct node* head = NULL;
struct node* tail = NULL;

void Array2DLinkedlist() {
	for (int i = 0; i < count; i++) {
		struct node* newnode;
		newnode = (struct node*)malloc(sizeof(struct node));

		newnode->data = loldic[i];
		newnode->prev = NULL;
		newnode->next = NULL;

		if (head == NULL) {
			head = newnode;
			tail = newnode;

			head->next = head;
			head->prev = head;
		}
		else {
			newnode->prev = tail;
			newnode->next = head;

			tail->next = newnode;
			head->prev = newnode;

			tail = newnode;
		}
	}
		
}

void printall() {
	struct node* cur;
	if (head == NULL) {
		printf("출력할 챔피언이 없습니다.\n");
		return;
	}
	cur = head;
	do {
		printf("name: %s, hp: %d, mp: %d, speed: %d, range: %d, pos: %s\n\n",
			cur->data.name,
			cur->data.hp,
			cur->data.mp,
			cur->data.speed,
			cur->data.range,
			cur->data.pos);

		cur = cur->next;
	} while (cur != head);
}

struct node* search(char name[]) {
	if (head == NULL) {
		return NULL;
	}
	struct node* cur  = head;

	do {
		if (strcmp(cur->data.name, name) == 0) {
			return cur;
		}
		cur = cur->next;

	} while (cur != head);

	return NULL;
}

void Delete(char name[]) {
	if (head == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}
	struct node* cur = search(name);


	if (cur == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}

	if (head == tail) {
		free(cur);
		head = NULL;
		tail = NULL;

		printf("챔피언 %s가 삭제되었습니다.\n", name);
		return;
	}

	struct node* prev = cur->prev;
	struct node* next = cur->next;
	
	if (cur == head) {
		next->prev = tail;
		prev->next = next;
		head = next;
		free(cur);

		printf("챔피언 %s가 삭제되었습니다.\n", name);
	}
	else if (cur == tail) {
		prev->next = next;
		next->prev = prev;
		tail = prev;
		free(cur);
		count--;
		printf("챔피언 %s가 삭제되었습니다.\n", name);
	}
	else {
		prev->next = next;
		next->prev = prev;
		free(cur);
		count--;
		printf("챔피언 %s가 삭제되었습니다.\n", name);
	}

}

void deleteall(char pos[]) {
	if (head == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}
	int num = 0;
	
	while (head != NULL) {
		struct node* cur = head;
		struct node* target = NULL;
		
		do {
			if (strcmp(pos, cur->data.pos) == 0) {
				target = cur;
				break;
			}
			cur = cur->next;
		} while (cur != head);
		
		if (target == NULL) {
			break;
		}

		char name[50];
		strcpy(name, target->data.name);
		Delete(name);
		num++;
	}
	printf("%s 포지션 챔피언 %d개가 삭제되었습니다.\n", pos, num);
}

void findmaxhp() {
	if (head == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}
	struct node* cur = head;
	struct node* maxhp = head;
	do {
		if (cur->data.hp > maxhp->data.hp) {
			maxhp = cur;
		}
		cur = cur->next;
	} while (cur != head);

	cur = head;
	do {
		if (cur->data.hp == maxhp->data.hp) {
			printf("name: %s, hp: %d, mp: %d, speed: %d, range: %d, pos: %s\n\n",
				cur->data.name,
				cur->data.hp,
				cur->data.mp,
				cur->data.speed,
				cur->data.range,
				cur->data.pos);
		}
		cur = cur->next;
	} while (cur != head);
}

void sortbyhp() {
	if (head == NULL) {
		printf("챔피언이 없습니다.\n");
		return;
	}

	struct node* s_head = NULL;
	struct node* s_tail = NULL;
	
	while (head != NULL) {
		struct node* cur = head;

		if (head == tail) {
			head = NULL;
			tail = NULL;
		}
		else {
			head = head->next;
			tail->next = head;
			head->prev = tail;
		}

		if (s_head == NULL) {
			s_head = cur;
			s_tail = cur;
			
			s_head->next = s_head;
			s_tail->prev = s_head;
			continue;
		}
		struct node* s_cur = s_head;
		int inserted = 0;

		do {
			if (cur->data.hp >= s_cur->data.hp) {
				cur->prev = s_cur->prev;
				cur->next = s_cur;

				s_cur->prev->next = cur;
				s_cur->prev = cur;

				if (s_cur == s_head) {
					s_head = cur;
				}
				inserted = 1;
				break;
			}

			s_cur = s_cur->next;
		} while (s_cur != s_head);
		if (inserted == 0) {
			cur->next = s_head;
			cur->prev = s_tail;

			s_tail->next = cur;
			s_head->prev = cur;

			s_tail = cur;
		}
	}

	head = s_head;
	tail = s_tail;
}

void insert() {
	struct node* newnode = (struct node*)malloc(sizeof(struct node));

	printf("추가할 챔피언 정보를 입력하세요.\n");

	printf("name: ");
	fgets(newnode->data.name, sizeof(newnode->data.name), stdin);
	newnode->data.name[strcspn(newnode->data.name, "\n")] = '\0';

	printf("hp: ");
	scanf("%d", &newnode->data.hp);

	printf("mp: ");
	scanf("%d", &newnode->data.mp);

	printf("speed: ");
	scanf("%d", &newnode->data.speed);

	printf("range: ");
	scanf("%d", &newnode->data.range);

	getchar();

	printf("pos: ");
	fgets(newnode->data.pos, sizeof(newnode->data.pos), stdin);
	newnode->data.pos[strcspn(newnode->data.pos, "\n")] = '\0';

	

	newnode->prev = NULL;
	newnode->next = NULL;

	if (head == NULL) {
		head = newnode;
		tail = newnode;

		head->next = head;
		head->prev = head;

		count++;
		return;
	}

	struct node* cur = head;


	do {
		struct node* prev = cur->prev;
		if (newnode->data.hp > cur->data.hp) {
			newnode->next = cur;
			newnode->prev = prev;

			prev->next = newnode;
			cur->prev = newnode;

			if (cur == head) {
				head = newnode;
			}
			count++;
			return;
		}
		cur = cur->next;
	} while (cur != head);

	newnode->prev = tail;
	newnode->next = head;

	tail->next = newnode;
	head->prev = newnode;

	tail = newnode;

	count++;
	return;
}

int main() {
	FILE* fp;
	char line[200];

	fp = fopen("testdata.txt", "r");
	if (fp == NULL) {
		printf("파일을 불러올수 없습니다.");
		return -1;
	}

	while (fgets(line, sizeof(line), fp) != NULL) {
		line[strcspn(line, "\n")] = '\0';

		sscanf(line, "%49[^\t]%d%d%d%d%49s", loldic[count].name, &loldic[count].hp, &loldic[count].mp, &loldic[count].speed, &loldic[count].range, loldic[count].pos);
		count++;
	}

	fclose(fp);

	Array2DLinkedlist();

	while (1) {
		char order[50];
		printf("명령어를 입력하세요.\n(search [name], insert, delete [name], deleteall [pos], printall, findmaxhp, sortbyhp)\n");
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';

		if (strcmp(order, "insert") == 0) {
			insert();
		}
		else if (strcmp(order, "printall") == 0) {
			printall();
		}
		else if (strcmp(order, "findmaxhp") == 0) {
			findmaxhp();
		}
		else if (strcmp(order, "sortbyhp") == 0) {
			sortbyhp();
		}
		else if (strncmp(order, "search ", 7) == 0) {
			char name[50];
			strcpy(name, order + 7);
			struct node* cur = search(name);

			if (cur == NULL) {
				printf("챔피언이 없습니다.\n");
			}
			else {
				printf("name: %s, hp: %d, mp: %d, speed: %d, range: %d, pos: %s\n\n",
					cur->data.name,
					cur->data.hp,
					cur->data.mp,
					cur->data.speed,
					cur->data.range,
					cur->data.pos);
			}

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



	}
}
