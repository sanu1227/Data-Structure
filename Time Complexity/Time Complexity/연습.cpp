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
	snode* next;
}snode;

snode* shead = NULL;
snode* stail = NULL;

typedef struct dnode {
	champ data;
	dnode* next;
	dnode* prev;
}dnode;

dnode* dhead = NULL;
dnode* dtail = NULL;

double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceFrequency(&now);

	return (double)now.QuadPart / (double)freq.QuadPart;
}

void arrytoslink(champ data) {
	snode* newnode = (snode*)malloc(sizeof(snode));
	newnode->data = data;

	if (shead == NULL) {
		shead = newnode;
		stail = newnode;
		stail->next = newnode;
	}
	else {
		stail->next = newnode;
		newnode->next = shead;
		stail = newnode;
	}
}

void arrytodlink(champ data) {
	dnode* newnode = (dnode*)malloc(sizeof(dnode));
	newnode->data = data;

	if (dhead == NULL) {
		dhead = newnode;
		dtail = newnode;
		dtail->next = dhead;
		dhead->prev = dtail;
	}
	else {
		dtail->next = newnode;
		dhead->prev = newnode;
		newnode->next = dhead;
		newnode->prev = dtail;
		dtail = newnode;
	}
}

void printallslink() {
	snode* cur;
	cur = shead;
	do {
		cur = cur->next;
	} while (cur != shead);
}

void printalldlink() {
	dnode* cur;
	cur = dhead;
	do {
		cur = cur->next;
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
		cur = cur->next;
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
		cur = cur->next;
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
	snode newlist;
	snode* n_tail;

	if (s_head == NULL || s_head->next == NULL) {
		return s_head;
	}

	fast = s_head->next;
	slow = s_head;

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

	newlist.next = NULL;
	n_tail = &newlist;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			n_tail->next = a;
			a = a->next;
		}
		else {
			n_tail->next = b;
			b = b->next;
		}
		n_tail = n_tail->next;
	}

	if (a != NULL) {
		n_tail->next = a;
	}
	else {
		n_tail->next = b;
	}
	return newlist.next;
}

void sortbyslist() {
	snode* cur;

	stail->next = NULL;
	shead = mergeslist(shead);

	cur = shead;
	while (cur->next != NULL) {
		cur = cur->next;
	}
	stail = cur;
	stail->next = shead;
}

dnode* mergedlist(dnode* dhead) {
	struct dnode* fast;
	struct dnode* slow;

	struct dnode* back;
	struct dnode* front;

	struct dnode* a;
	struct dnode* b;

	struct dnode newlist;
	struct dnode* n_tail;
	struct dnode* select;

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

	newlist.next = NULL;
	n_tail = &newlist;

	while (a != NULL && b != NULL) {
		if (a->data.hp >= b->data.hp) {
			select = a;
			a = a->next;
		}
		else {
			select = b;
			b = b->next;
		}
		select->next = NULL;

		if (n_tail == &newlist) {
			select->prev = NULL;
		}
		else {
			select->prev = n_tail;
		}
		n_tail->next = select;
		n_tail = select;
	}
	while (a != NULL) {
		select = a;
		a = a->next;

		select->next = NULL;

		if (n_tail == &newlist) {
			select->prev = NULL;
		}
		else {
			select->prev = n_tail;
		}
		n_tail->next = select;
		n_tail = select;
	}

	while (b != NULL) {
		select = b;
		b = b->next;

		select->next = NULL;

		if (n_tail == &newlist) {
			select->prev = NULL;
		}
		else {
			select->prev = n_tail;
		}
		n_tail->next = select;
		n_tail = select;
	}

	return newlist.next;
}

void sortbyhpdlist() {
	dnode* cur;

	dhead->prev = NULL;
	dtail->next = NULL;
	dhead = mergedlist(dhead);

	cur = dhead;

	while (cur->next != NULL) {
		cur = cur->next;
	}
	dtail = cur;

	dtail->next = dhead;
	dhead->prev = dtail;
}

int main() {
	FILE* fp;
	fp = fopen("test.txt", "r");
	champ temp;

	while (fscanf(fp, "%s %s %d %d %d", temp.pos, temp.name, &temp.hp, &temp.ad, &temp.def) == 5) {
		arrytodlink(temp);
		arrytoslink(temp);
	}

	fclose(fp);
}