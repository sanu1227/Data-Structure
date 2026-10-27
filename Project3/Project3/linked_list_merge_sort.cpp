#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

typedef struct champ {
	char pos[50];
	char name[50];
	int hp;
	int atk;
	int def;
}champ;

typedef struct node {
	champ data;
	struct node* next;
}node;

node* head = NULL;
node* tail = NULL;

double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (double)now.QuadPart / (double)freq.QuadPart;
}

void arrytoslink(champ data) {
	node* newnode = (node*)malloc(sizeof(node));

	newnode->data = data;

	if (head == NULL) {
		head = newnode;
		tail = newnode;
		tail->next = head;
	}
	else {
		tail->next = newnode;
		newnode->next = head;
		tail = newnode;
	}
}

node* merge_sort_sl(node* start) {
	node* fast;
	node* slow;
	node* front;
	node* back;
	node dummy;
	node* cur;
	node* a;
	node* b;


	if (start == NULL || start->next == NULL) {
		return start;
	}

	fast = start->next;
	slow = start;

	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;
	}

	front = start;
	back = slow->next;
	slow->next = NULL;

	front = merge_sort_sl(front);
	back = merge_sort_sl(back);

	a = front;
	b = back;

	dummy.next = NULL;
	cur = &dummy;

	while (a != NULL && b != NULL) {
		if (strcmp(a->data.name, b->data.name) <= 0) {
			cur->next = a;
			a = a->next;
		}
		else{
			cur->next = b;
			b = b->next;
		}
		cur = cur->next;
	}

	if (a != NULL) {
		cur->next = a;
	}
	else {
		cur->next = b;
	}

	return dummy.next;
}

void sortbyname_sl() {
	node* cur;

	tail->next = NULL;
	head = merge_sort_sl(head);
	cur = head;

	while (cur->next != NULL) {
		cur = cur->next;
	}

	tail = cur;
	tail->next = head;
}

typedef struct tnode {
	champ data;
	struct tnode* left;
	struct tnode* right;
}tnode;

tnode* root = NULL;

void arrytobst(champ data) {
	tnode* newnode = (tnode*)malloc(sizeof(tnode));
	tnode* cur;
	tnode* parent;
	int cmp;

	newnode->data = data;
	newnode->left = NULL;
	newnode->right = NULL;

	if (root == NULL) {
		root = newnode;
		return;
	}

	cur = root;
	parent = NULL;

	while (cur != NULL) {
		parent = cur;

		cmp = strcmp(data.name, cur->data.name);
		if (cmp < 0) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}

	cmp = strcmp(data.name, parent->data.name);

	if (cmp < 0) {
		parent->left = newnode;
	}
	else {
		parent->right = newnode;
	}
}

tnode* searchbyname(char name[]) {
	tnode* cur;
	int cmp;

	cur = root;

	while (cur != NULL) {
		cmp = strcmp(name, cur->data.name);

		if (cmp == 0) {
			return cur;
		}
		else if (cmp < 0) {
			cur = cur->left;
		}
		else {
			cur = cur->right;
		}
	}

	return NULL;
}

void printall(tnode* cur) {
	if (cur == NULL) {
		return;
	}

	printall(cur->left);
	printf("%s %s %d %d %d\n",
		cur->data.pos,
		cur->data.name,
		cur->data.hp,
		cur->data.atk,
		cur->data.def);
	printall(cur->right);
}

int delete_bt(char name[]) {
	tnode* cur;
	tnode* parent;
	tnode* child;
	tnode* succ;
	tnode* succ_parent;
	int cmp;

	cur = root;
	parent = NULL;

	while (cur != NULL) {
		cmp = strcmp(name, cur->data.name);
		if (cmp == 0) {
			break;
		}
		else if (cmp < 0) {
			parent = cur;
			cur = cur->left;
		}
		else {
			parent = cur;
			cur = cur->right;
		}
	}

	if (cur == NULL) {
		return 0;
	}

	if (cur->left == NULL || cur->right == NULL) {
		if (cur->left == NULL) {
			child = cur->right;
		}
		else {
			child = cur->left;
		}
		if (parent == NULL) {
			root = child;
		}
		else if (parent->left == cur) {
			parent->left = child;
		}
		else {
			parent->right = child;
		}

		free(cur);
		return 1;
	}

	succ_parent = cur;
	succ = cur->right;

	while (succ->left != NULL) {
		succ_parent = succ;
		succ = succ->left;
	}

	cur->data = succ->data;

	child = succ->right;

	if (succ_parent->left == succ) {
		succ_parent->left = child;
	}
	else {
		succ_parent->right = child;
		
	}
	free(succ);
	return 1;
}

int main() {
	FILE* fp;
	champ temp;
	double start, end;

	fp = fopen("test.txt", "r");
	while (fscanf(fp, "%s %s %d %d %d", temp.pos, temp.name, &temp.hp, &temp.atk, &temp.def) == 5) {
		arrytoslink(temp);
		arrytobst(temp);
	}
	fclose(fp);

	while (1) {
		char order[50];
		printf("order: \n");
		printf("sortbyname_sl, searchbyname_bt, insert_bt, delete_bt, printall_bt, sortbyname_bt\n");
		fgets(order, sizeof(order), stdin);
		order[strcspn(order, "\n")] = '\0';

		if (strcmp(order, "sortbyname_sl") == 0) {
			start = gettime();
			sortbyname_sl();
			end = gettime();
			printf("sortbyname_SL 수행시간: %.6f\n", end - start);
		}
		else if (strcmp(order, "searchbyname_bt") == 0) {
			char name[50];
			printf("찾으려는 챔피언의 이름을 입력하세요\n");
			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';
			tnode* cur;

			start = gettime();
			cur = searchbyname(name);
			end = gettime();

			if (cur != NULL) {
				printf("검색 성공\n");
				printf("%s %s %d %d %d\n",
					cur->data.pos,
					cur->data.name,
					cur->data.hp,
					cur->data.atk,
					cur->data.def);
			}
			else {
				printf("검색 실패: 해당 이름의 챔피언이 없습니다.\n");
			}

			printf("searchbyname_BT 수행시간: %.10f\n", end - start);
		}
		else if (strcmp(order, "printall_bt") == 0) {
			start = gettime();

			printall(root);

			end = gettime();

			printf("printall_BT 수행시간: %.6f\n", end - start);
		}
		else if (strcmp(order, "insert_bt") == 0) {
			champ temp;
			printf("추가할 챔피언 정보를 입력하세요\n");
			printf("입력 형식: 포지션 이름 체력 공격력 방어력\n");

			scanf("%s %s %d %d %d",
				temp.pos,
				temp.name,
				&temp.hp,
				&temp.atk,
				&temp.def);

			getchar();

			start = gettime();

			arrytobst(temp);

			end = gettime();

			printf("insert_BT 수행시간: %.10f\n", end - start);
		}
		else if (strcmp(order, "delete_bt") == 0) {
			char name[50];
			int result;

			printf("삭제하려는 챔피언의 이름을 입력하세요\n");

			fgets(name, sizeof(name), stdin);
			name[strcspn(name, "\n")] = '\0';

			start = gettime();

			result = delete_bt(name);

			end = gettime();

			if (result == 1) {
				printf("삭제 성공\n");
			}
			else {
				printf("삭제 실패: 해당 이름의 챔피언이 없습니다.\n");
			}

			printf("delete_BT 수행시간: %.10f\n", end - start);
		}
	}
}