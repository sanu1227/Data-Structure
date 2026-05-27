#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

typedef struct champ {
	char pos[50];
	char name[50];
	int hp;
	int ad;
	int def;
}champ;

struct snode {
	champ data;
	struct snode* next;
};

struct dnode {
	champ data;
	struct dnode* next;
	struct dnode* prev;
};

struct snode* shead = NULL;
struct snode* stail = NULL;
struct dnode* dhead = NULL;
struct dnode* dtail = NULL;


double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (double)now.QuadPart / (double)freq.QuadPart;
}

void arrytoslist(champ data) {
	struct snode* newnode;
	newnode = (struct snode*)malloc(sizeof(struct snode));

	newnode->data = data;

	if (shead == NULL) {
		shead = newnode;
		stail = newnode;
		newnode->next = newnode;
	}
	else {
		newnode->next = shead;
		stail->next = newnode;
		stail = newnode;
	}
}

void arrytodlist(champ data) {
	struct dnode* newnode;
	newnode = (struct dnode*)malloc(sizeof(struct dnode));

	newnode->data = data;

	if (dhead == NULL) {
		dhead = newnode;
		dtail = newnode;

		dhead->prev = dtail;
		dtail->next = dhead;
	}
	else {
		newnode->prev = dtail;
		newnode->next = dhead;

		dtail->next = newnode;
		dhead->prev = newnode;

		dtail = newnode;
	}
}

void printallslist() {
	struct snode* cur;

	cur = shead;

	do {
		cur = cur->next;
	} while (cur != shead);
}

void printalldlist() {
	struct dnode* cur;
	cur = dhead;
	do {
		cur = cur->next;
	} while (cur != dhead);
}

struct snode* findmaxhpslist() {
	struct snode* cur;
	struct snode* maxnode;

	cur = shead->next;
	maxnode = shead;

	while (cur != shead) {
		if (cur->data.hp > maxnode->data.hp) {
			maxnode = cur;
		}
		cur = cur->next;
	}

	return maxnode;
}

struct dnode* findmaxhpdlist() {
	struct dnode* cur;
	struct dnode* maxnode;

	cur = dhead->next;
	maxnode = dhead;

	while (cur != dhead) {
		if (cur->data.hp > maxnode->data.hp) {
			maxnode = cur;
		}
		cur = cur->next;
	}
	return maxnode;
}

struct snode* mergeslist(struct snode* s_head) {
	struct snode* fast;
	struct snode* slow;

	if (s_head == NULL || s_head->next == NULL) {
		return s_head;
	}

	slow = s_head;
	fast = s_head->next;

	while (fast != NULL && fast->next != NULL) {
		slow = slow->next;
		fast = fast->next->next;
	}

	struct snode* front;
	struct snode* back;

	front = s_head;
	back = slow->next;
	slow->next = NULL;

	front = mergeslist(front);
	back = mergeslist(back);

	struct snode* a;
	struct snode* b;
	a = front;
	b = back;

	struct snode dummy;
	struct snode* mergetail;

	dummy.next = NULL;
	mergetail = &dummy;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			mergetail->next = a;
			a = a->next;
		}
		else {
			mergetail->next = b;
			b = b->next;
		}
		mergetail = mergetail->next;
	}

	if (a != NULL) {
		mergetail->next = a;
	}
	else {
		mergetail->next = b;
	}
	return dummy.next;
}

void sortbyslist() {
	struct snode* cur;

	stail->next = NULL;
	shead = mergeslist(shead);

	cur = shead;
	while (cur->next != NULL) {
		cur = cur->next;
	}

	stail = cur;
	stail->next = shead;
}

struct dnode* mergedlist(struct dnode* d_head) {
	struct dnode* fast;
	struct dnode* slow;

	struct dnode* front;
	struct dnode* back;

	struct dnode* a;
	struct dnode* b;

	struct dnode dummy;
	struct dnode* mergetail;
	struct dnode* selected;

	if (d_head == NULL || d_head->next == NULL) {
		return d_head;
	}

	fast = d_head->next;
	slow = d_head;

	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;
	}

	front = d_head;
	back = slow->next;

	slow->next = NULL;

	if (back != NULL) {
		back->prev = NULL;
	}

	front = mergedlist(front);
	back = mergedlist(back);

	a = front;
	b = back;

	dummy.next = NULL;
	mergetail = &dummy;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			selected = a;
			a = a->next;
		}
		else {
			selected = b;
			b = b->next;
		}

		selected->next = NULL;

		if (mergetail == &dummy) {
			selected->prev = NULL;
		}
		else {
			selected->prev = mergetail;
		}

		mergetail->next = selected;
		mergetail = selected;
	}

	while (a != NULL) {
		selected = a;
		a = a->next;

		selected->next = NULL;
		if (mergetail == &dummy) {
			selected->prev = NULL;
		}
		else {
			selected->prev = mergetail;
		}

		mergetail->next = selected;
		mergetail = selected;
	}

	while (b != NULL) {
		selected = b;
		b = b->next;

		selected->next = NULL;
		if (mergetail == &dummy) {
			selected->prev = NULL;
		}
		else {
			selected->prev = mergetail;
		}

		mergetail->next = selected;
		mergetail = selected;
	}

	return dummy.next;
}

void sortbydlist() {
	struct dnode* cur;

	dtail->next = NULL;
	dhead->prev = NULL;

	dhead = mergedlist(dhead);

	cur = dhead;

	while(cur->next != NULL){
		cur = cur->next;
	}
	dtail = cur;
	dtail->next = dhead;

	dhead->prev = dtail;

}

int main() {
	FILE* fp;
	champ temp;

	//fp = fopen("LOLDic.txt", "r");
	fp = fopen("test.txt", "r");

	if (fp == NULL) {
		printf("파일을 열 수 없습니다.\n");
		return 1;
	}

	while (fscanf(fp, "%s %s %d %d %d", temp.pos, temp.name, &temp.hp, &temp.ad, &temp.def) == 5) {
		arrytoslist(temp);
		arrytodlist(temp);
	}

	fclose(fp);

	while (1) {
		printf("order: printall, sortbyhp, findmaxhp, end\n");
		char order[20];
		printf("order: ");
		scanf("%s", order);

		if (strcmp(order, "printall") == 0) {
			double start;
			double end;

			start = gettime();
			printallslist();
			end = gettime();
			printf("단일 환형 연결리스트 printall 수행 시간: %.6f초\n", end - start);

			start = gettime();
			printalldlist();
			end = gettime();
			printf("이중 환형 연결리스트 printall 수행 시간: %.6f초\n", end - start);
		}
		else if (strcmp(order, "sortbyhp") == 0) {
			double start;
			double end;

			start = gettime();
			sortbyslist();
			end = gettime();
			printf("slist time: %.6f\n", end - start);

			start = gettime();
			sortbydlist();
			end = gettime();
			printf("dlist time: %.6f\n", end - start);
		}
		else if (strcmp(order, "findmaxhp") == 0) {
			struct snode* smaxnode;
			struct dnode* dmaxnode;
			double start;
			double end;

			start = gettime();
			smaxnode = findmaxhpslist();
			end = gettime();
			printf("maxhp champ: %s %s %d %d %d\n", smaxnode->data.pos, smaxnode->data.name, smaxnode->data.hp, smaxnode->data.ad, smaxnode->data.def);
			printf("findmaxhp slist time: %.6f\n", end - start);
		
			start = gettime();
			dmaxnode = findmaxhpdlist();
			end = gettime();
			printf("maxhp champ: %s %s %d %d %d\n", dmaxnode->data.pos, dmaxnode->data.name, dmaxnode->data.hp, dmaxnode->data.ad, dmaxnode->data.def);
			printf("findmaxhp dlist time: %.6f\n", end - start);
		}
		else if (strcmp(order, "end") == 0) {
			break;
		}
		else {
			printf("wrong order\n");
		}
	}
}