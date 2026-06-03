#define _CRT_SECURE_NO_WARNING
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <Windows.h>

typedef struct data {
	char pos[100];
	char name[100];
	int hp;
	int atk;
	int def;
}data;

typedef struct snode {
	data data;
	struct snode* next;
}snode;

typedef struct tnode {
	data data;
	struct tnode* left;
	struct tnode* right;
}tnode;

snode* head = NULL;
snode* tail = NULL;
tnode* root = NULL;

double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (double)now.QuadPart / (double)freq.QuadPart;
}

snode* arrytosl(data data) {
	snode* newnode = (snode*)malloc(sizeof(snode));

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

tnode* arrytobt(data data) {
	tnode* newnode = (tnode*)malloc(sizeof(tnode));
	tnode* parent;
	tnode* cur;
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

tnode* search_bt(char name[]) {
	tnode* cur = root;
	int cmp;

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

void printallbt(tnode* cur) {
	tnode* cur;
	if (cur == NULL) {
		return;
	}
	
	printallbt(cur->left);

	printf("");
	printallbt(cur->right);

}

int delete_bt(char name[]) {
	tnode* cur;
	tnode* parent;
	tnode* child;
	int cmp;
	tnode* succ;
	tnode* succ_parent;

	cur = root;
	parent = NULL;

	while (cur != NULL) {
		parent = cur;
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
			child = root;
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

}