#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>

typedef struct champ {
	char pos[50];
	char name[50];
	int hp;
	int ad;
	int def;
}champ;

typedef struct snode {
	champ data;
	struct snode* next;
}snode;

typedef struct dnode {
	champ data;
	struct dnode* prev;
	struct dnode* next;
}dnode;

snode* shead = NULL;
snode* stail = NULL;
dnode* dhead = NULL;
dnode* dtail = NULL;

double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (double)freq.QuadPart / (double)now.QuadPart;
}

void arrytoslist(champ data) {
	snode* newnode = (snode*)malloc(sizeof(snode));

	newnode->data = data;
	if (shead == NULL) {
		shead = newnode;
		stail = newnode;
		stail->next = shead;
	}
	else {
		stail->next = newnode;
		newnode->next = shead;
		stail = newnode;
	}

}

void arrytodlist(champ data) {
	dnode* newnode = (dnode*)malloc(sizeof(dnode));

	newnode->data = data;
	if (dhead == NULL) {
		dhead = newnode;
		dtail = newnode;
		dhead->prev = dtail;
		dtail->next = dhead;
	}
	else {
		dtail->next = newnode;
		newnode->prev = dtail;

		dhead->prev = newnode;
		newnode->next = dhead;
		dtail = newnode;
	}
}

void printallslist() {
	snode* cur;
	cur = shead;
	int count = 0;
	do {
		cur = cur->next;
		count++;
		if (count < 10) {
			printf("%s %s %d %d %d", cur->data.pos, cur->data.name, cur->data.hp, cur->data.ad, cur->data.def);
		}
	} while (cur != shead);

}
void printalldlist() {
	dnode* cur;
	cur = dhead;
	int count = 0;
	do {
		cur = cur->next;
		count++;
		if (count < 10) {
			printf("%s %s %d %d %d", cur->data.pos, cur->data.name, cur->data.hp, cur->data.ad, cur->data.def);
		}
	} while (cur != dhead);

}

snode* findmaxhpslist() {
	snode* cur;
	snode* maxhp;

	cur = shead->next;
	maxhp = shead;

	while (cur != shead) {
		if (cur->data.hp > maxhp->data.hp) {
			maxhp = cur;
		}
	}

	return maxhp;
}

dnode* findmaxhpdlist() {
	dnode* cur;
	dnode* maxhp;

	cur = dhead->next;
	maxhp = dhead;

	while (cur != dhead) {
		if (cur->data.hp > maxhp->data.hp) {
			maxhp = cur;
		}
	}

	return maxhp;
}

snode* mergeslist(snode* s_head) {
	snode* fast;
	snode* slow;

	snode* front;
	snode* back;
	snode* a;
	snode* b;

	snode space;
	snode* cur_tail;

	if (s_head == NULL || s_head->next == NULL) {
		return s_head;
	}

	fast = s_head->next;
	slow = shead;

	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;
	}

	front = s_head;
	back = slow->next;
	slow->next = NULL;

	front = mergeslist(front);
	back = mergeslist(back);

	a = front;
	b = back;

	space.next = NULL;
	cur_tail = &space;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			cur_tail->next = a;
			a = a->next;
		}
		else {
			cur_tail->next = b;
			b = b->next;
		}
		cur_tail = cur_tail->next;
	}

	if (a != NULL) {
		cur_tail->next = a;
	}
	else {
		cur_tail->next = b;
	}
	return space.next;
}

void sortbyslist() {
	snode* cur;
	shead->next = NULL;
	shead = mergeslist(shead);
	cur = shead;

	while (cur->next != NULL) {
		cur = cur->next;
	}
	stail = cur;
	stail->next = shead;
}

dnode* mergedlist(dnode* dhead) {
	dnode* fast;
	dnode* slow;

	dnode* front;
	dnode* back;

	dnode* a;
	dnode* b;

	dnode space;
	dnode* cur_tail;
	dnode* select;

	if (dhead == NULL || dhead->next == NULL) {
		return dhead;
	}

	fast = dhead->next;
	slow = dhead;

	while (fast != NULL && fast->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;
	}
	front = dhead;
	back = slow->next;
	slow->next = NULL;

	if (back != NULL) {
		back->prev = NULL;
	}

	front = mergedlist(front);
	back = mergedlist(back);

	a = front;
	b = back;

	space.next = NULL;
	cur_tail = &space;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			select= a;
			a = a->next;
		}
		else {
			select = b;
			b = b->next;
		}
		select->next = NULL;

		if (cur_tail == &space) {
			select->prev = NULL;
		}
		else {
			select->prev = cur_tail;
		}

		cur_tail->next = select;
		cur_tail = select;
	}

	while (a != NULL) {
		select = a;
		a = a->next;
		select->next = NULL;

		if (cur_tail == &space) {
			select->prev = NULL;
		}
		else {
			select->prev = cur_tail;
		}

		cur_tail->next = select;
		cur_tail = select;
	}

	while (b != NULL) {
		select = b;
		b = b->next;
		select->next = NULL;

		if (cur_tail == &space) {
			select->prev = NULL;
		}
		else {
			select->prev = cur_tail;
		}

		cur_tail->next = select;
		cur_tail = select;
	}
	return space.next;
}

void sortbydlist() {
	dnode* cur;
	
	dhead->prev = NULL;
	dtail->next = NULL;

	dhead = mergedlist(dhead);

	cur = dhead;

	while (cur->next != NULL) {
		cur = cur->next;
	}

	dtail = cur;
	dhead->prev = dtail;
	dtail->next = dhead;
}