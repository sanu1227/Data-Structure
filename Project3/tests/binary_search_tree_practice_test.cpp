// Run from a Visual Studio Native Tools prompt:
// cl /EHsc /std:c++20 binary_search_tree_practice_test.cpp && binary_search_tree_practice_test.exe
#define main practice_demo_main
#include "../Project3/binary_search_tree_practice.cpp"
#undef main
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <assert.h>

static data entry(const char* name) {
	data value = {};
	strcpy_s(value.pos, "top");
	strcpy_s(value.name, name);
	value.hp = 100;
	value.atk = 20;
	value.def = 10;
	return value;
}

static void check_order(tnode* node, const char*& previous, int& count) {
	if (node == NULL) return;
	check_order(node->left, previous, count);
	assert(previous == NULL || strcmp(previous, node->data.name) <= 0);
	previous = node->data.name;
	++count;
	check_order(node->right, previous, count);
}

static void check_tree(int expected) {
	const char* previous = NULL;
	int count = 0;
	check_order(root, previous, count);
	assert(count == expected);
}

int main() {
	char missing[] = "missing";
	assert(search_bt(missing) == NULL);
	assert(delete_bt(missing) == 0);
	printallbt(NULL);

	data center = entry("M"), left = entry("F"), right = entry("T");
	snode* first = arrytosl(center);
	assert(first == head && head == tail && tail->next == head);
	snode* second = arrytosl(left);
	assert(first->next == second && tail == second && tail->next == head);
	free(first);
	free(second);
	head = tail = NULL;

	assert(arrytobt(center) == root);
	assert(search_bt(center.name) == root);
	assert(delete_bt(center.name) == 1 && root == NULL);

	arrytobt(center);
	tnode* child = arrytobt(left);
	assert(delete_bt(center.name) == 1 && root == child);
	assert(delete_bt(left.name) == 1 && root == NULL);
	arrytobt(center);
	child = arrytobt(right);
	assert(delete_bt(center.name) == 1 && root == child);
	assert(delete_bt(right.name) == 1 && root == NULL);

	data values[] = { entry("M"), entry("F"), entry("T"), entry("A"),
		entry("H"), entry("R"), entry("Z"), entry("G") };
	for (data& value : values) {
		tnode* inserted = arrytobt(value);
		assert(inserted != NULL && search_bt(value.name) == inserted);
	}
	check_tree(8);
	assert(delete_bt(missing) == 0);
	assert(delete_bt(values[3].name) == 1); // Leaf.
	check_tree(7);
	assert(delete_bt(values[4].name) == 1); // One child.
	assert(search_bt(values[7].name) != NULL);
	check_tree(6);
	assert(delete_bt(values[0].name) == 1); // Root with two children.
	assert(strcmp(root->data.name, "R") == 0);
	assert(search_bt(values[0].name) == NULL);
	check_tree(5);
	while (root != NULL) {
		assert(delete_bt(root->data.name) == 1);
	}
	check_tree(0);

	arrytobt(center);
	arrytobt(center); // Duplicate names follow the right subtree.
	check_tree(2);
	assert(delete_bt(center.name) == 1 && search_bt(center.name) != NULL);
	assert(delete_bt(center.name) == 1 && root == NULL);

	data printed[] = { entry("B"), entry("A"), entry("C") };
	for (data& value : printed) arrytobt(value);
	printallbt(root);
	while (root != NULL) assert(delete_bt(root->data.name) == 1);
	puts("All binary search tree practice checks passed.");
	return 0;
}
