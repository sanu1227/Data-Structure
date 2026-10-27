#define _CRT_SECURE_NO_WARNINGS
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
	if (newnode == NULL) {
		return NULL;
	}

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
	return newnode;
}

tnode* arrytobt(data data) {
	tnode* newnode = (tnode*)malloc(sizeof(tnode));
	if (newnode == NULL) {
		return NULL;
	}
	tnode* parent;
	tnode* cur;
	int cmp;

	newnode->data = data;
	newnode->left = NULL;
	newnode->right = NULL;

	if (root == NULL) {
		root = newnode;
		return newnode;
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
	return newnode;
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
	if (cur == NULL) {
		return;
	}
	
	printallbt(cur->left);

	printf("%s %s %d %d %d\n",
		cur->data.pos, cur->data.name, cur->data.hp, cur->data.atk, cur->data.def);
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
	FILE* fp = fopen("test.txt", "r");
	if (fp == NULL) {
		perror("test.txt");
		return 1;
	}

	data entry;
	while (fscanf(fp, "%99s %99s %d %d %d", entry.pos, entry.name,
		&entry.hp, &entry.atk, &entry.def) == 5) {
		if (arrytosl(entry) == NULL || arrytobt(entry) == NULL) {
			fclose(fp);
			fprintf(stderr, "Unable to allocate a node.\n");
			return 1;
		}
	}
	fclose(fp);
	printallbt(root);
	return 0;
}
