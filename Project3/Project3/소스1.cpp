#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>

typedef struct champ {
	char pos[100];
	char name[100];
	int hp;
	int atk;
	int def;
}champ;

typedef struct snode {
	champ data;
	struct snode* next;
}snode;

snode* head = NULL;
snode* tail = NULL;

typedef struct node {
	champ data;
	struct node* left;
	struct node* right;
}node;

node* root = NULL;

double gettime() {
	LARGE_INTEGER freq;
	LARGE_INTEGER now;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&now);

	return (double)now.QuadPart / (double)freq.QuadPart;
}

void arrytoslink(champ data){
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

snode* mergesl(snode* start) {
	snode* fast;
	snode* slow;
	snode* front;
	snode* back;
	snode* a;
	snode* b;
	snode dummy;
	snode* cur;

	if (start == NULL || start->next == NULL) {
		return start;
	}

	fast = start->next;
	slow = start;

	while (fast != NULL && start->next != NULL) {
		fast = fast->next->next;
		slow = slow->next;
	}

	front = start;
	back = slow->next;
	slow->next = NULL;

	front = mergesl(front);
	back = mergesl(back);

	a = front;
	b = back;

	dummy.next = NULL;
	cur = &dummy;

	int cmp;

	while (a != NULL && b != NULL) {
		if (strcmp(a->data.name, b->data.name) <= 0) {
			cur->next = a;
			a = a->next;
		}
		else {
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

void sortbynamesl() {
	snode* cur;
	
	tail->next = NULL;
	head = mergesl(head);
	cur = head;
	
	while (cur->next != NULL) {
		cur = cur->next;
	}
	tail = cur;
	tail->next = head;
}

void arrytobt(champ data) {
	node* newnode = (node*)malloc(sizeof(node));
	node* cur;
	node* parent;
	int cmp;

	newnode->data = data;
	newnode->right = NULL;
	newnode->left = NULL;

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

node* searchbt(char name[]) {
	node* cur;
	cur = root;
	int cmp;

	while (cur != NULL) {
		cmp = strcmp(cur->data.name, name);
		if (cmp == 0) {
			return cur;
		}
		else if (cmp < 0) {
			cur = cur->right;
		}
		else {
			cur = cur->left;
		}
	}

	return NULL;
}

void printall_bt(node* cur) {
	if (cur == NULL) {
		return;
	}
	printall_bt(cur->left);
	printf("%s %s %d %d %d\n",
		cur->data.pos,
		cur->data.name,
		cur->data.hp,
		cur->data.atk,
		cur->data.def);
	printall_bt(cur->right);
}

int delete_bt(char name[]) {
	node* cur;
	node* child;
	node* parent;
	node* succ;
	node* succ_parent;
	int cmp;

	cur = root;
	parent = NULL;

	while (cur != NULL) {
		cmp = strcmp(cur->data.name, name);
		if (cmp == 0) {
			break;
		}
		else if (cmp < 0) {
			parent = cur;
			cur = parent->right;
		}
		else {
			parent = cur;
			cur = parent->left;
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
	if (succ_parent->right == succ) {
		succ_parent->right = child;
	}
	else {
		succ_parent->left = child;
	}

	free(succ);
	return 1;
}

int main() {
	FILE* fp;
	champ temp;
	double start, end, total;
	total = 0;

	fp = fopen("test.txt", "r");
	while (fscanf(fp, "%s %s %d %d %d", temp.pos, temp.name, &temp.hp, &temp.atk, &temp.def) == 5) {
		arrytoslink(temp);
		start = gettime();
		arrytobt(temp);
		end = gettime();
		total += end - start;
	}
	fclose(fp);


}