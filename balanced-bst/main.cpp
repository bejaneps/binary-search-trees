#include <iostream>		//std::cerr, std::cout
#include <random>		//std::mt19937
#include <functional>	//std::bind
#include <queue>		//std::stack for breadth-first top-down left-right traversal
#include <vector>		//std::vector
#include <ctime>		//std::time
#include <algorithm>	//std::find

class BST;

class node {
private:
	int val;
	node *left, *right;

	friend class BST;
};

class BST {
private:
	node *root;

	node* create_node(int val) noexcept;

	void delete_by_merging(node* &n) noexcept;

	int height(node *n) const noexcept;

public:
	BST();
	BST(int val);
	~BST();

	//performs breadth first, top-to-down, left-to-right tree traversal and stores tree in a provided vector
	void breadth_first(std::vector<int> &container) noexcept;

	//inserts a value into a tree
	void insert(int val) noexcept;

	//finds elem and deletes it if it's in a tree, otherwise prints error message
	void find_delete_by_merging(int elem) noexcept;

	//balances a tree
	void balance_tree(std::vector<int> data, int first, int last) noexcept;

	//returns a height of a bst
	int height() const noexcept;
};

BST::BST(): 
root(nullptr) 
{
	
}

BST::BST(int val)
{
	node *temp = create_node(val);

	root = temp;
}

BST::~BST() 
{
	delete root;
}

node* BST::create_node(int val) noexcept
{
	//to allocate a memory even if memory refuses
	node *temp;
	while(true)
	{
		temp = new (std::nothrow) node;
		if(!temp) continue;
		break;
	}
	temp->val = val;
	temp->left = temp->right = nullptr;

	return temp;
}

void BST::delete_by_merging(node* &n) noexcept
{
	node *temp = n;
	if(n != 0) {
		if(!n->right)
			n = n->left;
		else if(n->left == nullptr)
			n = n->right;
		else {
			temp = n->left;
			while(temp->right != nullptr)
				temp = temp->right;

			temp->right = n->right;
			temp = n;
			n = n->left;
		}
		delete temp;
	}
}

int BST::height(node *n) const noexcept 
{
	if(n == nullptr)
		return 0;
	else {
		int l = height(n->left);
		int r = height(n->right);

		if(l > r)
			return l + 1;
		else return r + 1;
	}
}

void BST::breadth_first(std::vector<int> &container) noexcept
{
	std::queue<node*> q;
	node *r = root;
	if(r != nullptr) {
		q.push(r);
		while(!q.empty()) {
			node *temp = q.front();
			q.pop();
			r = temp;

			container.push_back(r->val);
			if(r->left != 0)
				q.push(r->left);
			if(r->right != 0)
				q.push(r->right);
		}
	}
}

void BST::insert(int val) noexcept 
{
	if(!root) 
	{
		node *temp = create_node(val);
		root = temp;
	}
	else
	{
		node *c = root, *prev = root;

		while(c && val != c->val)
		{
			prev = c;
			if(val < c->val)
				c = c->left;
			else
				c = c->right;
		}
		if (val == prev->val) return;
		else if(val < prev->val)
		{
			prev->left = create_node(val);
		}
		else {
			prev->right = create_node(val);
		}
	}
}

void BST::find_delete_by_merging(int elem) noexcept
{
	node *temp = root, *prev = nullptr;
	while(temp != nullptr) {
		if(temp->val == elem)
			break;
		prev = temp;
		if(elem < temp->val)
			temp = temp->left;
		else temp = temp->right;
	}
	if(temp != nullptr && temp->val == elem)
		if(temp == root)
			delete_by_merging(root);
		else if(prev->left == temp)
			delete_by_merging(prev->left);
		else delete_by_merging(prev->right);
	else if(root != nullptr)
		std::cerr << elem << " not in the tree." << std::endl;
	else
		std::cerr << "empty tree." << std::endl;
}

void BST::balance_tree(std::vector<int> data, int first, int last) noexcept
{
	if(first <= last) {
		int middle = (first + last) / 2;
		insert(data[middle]);
		balance_tree(data, first, middle-1);
		balance_tree(data, middle+1, last);
	}
}

int BST::height() const noexcept
{
	return height(root);
}

int main() 
{	
	BST tree{}, btree{};
	std::vector<int> numbers;
	numbers.reserve(1000);

	std::mt19937::result_type seed = std::time(NULL);
	auto random = std::bind(std::uniform_int_distribution<int>(0, 10000), std::mt19937(seed));

	//filling tree with 10 random numbers and deleting 2 of them (100 times)
	int num;
	for(int i = 0; i < 100; i++) {
		for(int j = 0; j < 10; ) {
			if(j > 0) {
				num = random();
				//skips adding a same number to a vector and bst
				if((std::find(numbers.begin(), numbers.end(), num)) != numbers.end()) continue;
			}
			numbers.push_back(num);
			tree.insert(numbers.back());
			j++;	//for 1000 pushes
		}
		if(numbers.front() == 0 || numbers.back() == 0) continue;
		tree.find_delete_by_merging(numbers.front());
		tree.find_delete_by_merging(numbers.back());
	}

	numbers.clear();

	tree.breadth_first(numbers);
	std::cout << "Height of unbalanced bst: " << tree.height() << std::endl;

	//better than self-defined sorting algorithms
	std::sort(numbers.begin(), numbers.end());

	btree.balance_tree(numbers, 0, numbers.size());
	std::cout << "Heiht of balanced bst: " << btree.height() << std::endl;

	return 0;
}

/*
 * So results are better when we balance binary search tree.
 * Mathematically, height of a tree improves approximately in 2.5 times
 * 
 * ---------------------------------------------------------
 * Test: 1
 * Height of a tree without balancing: 25
 * Heigh of a balanced tree: 11
 * ---------------------------------------------------------
 * Test: 2
 * Height of a tree without balancing: 23
 * Heigh of a balanced tree: 11
 * ---------------------------------------------------------
 * Test: 3
 * Height of a tree without balancing: 25
 * Heigh of a balanced tree: 11
 * ---------------------------------------------------------
*/