#include <iostream>		//std::cerr, std::cout
#include <random>		//std::mt19937
#include <functional>	//std::bind
#include <algorithm>	//std::find
#include <chrono>		//std::chrono
#include <ctime>		//std::time
#include <cmath>		//std::log2

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

	//creates a new node
	node* create_node(int val) noexcept;

	//performs right rotation for creation of backbone
	node* rotate_right(node* gpar, node* par, node* lchild) noexcept;

	//performs left rotation for creating perfect tree
	void rotate_left(node* gpar, node* par, node* rchild) noexcept;

	//creates a backbone for DSW tree balancing
	void create_backbone() noexcept;

	int height(node *n) const noexcept;

public:
	BST();
	BST(int val);
	~BST();

	//returns a value holded by root node
	int get_root() const noexcept;

	//inserts a value into a tree
	void insert(int val) noexcept;

	//fills the tree with random numbers(using std::mt19937 generator) of specified amount within a min and max range inclusive
	void fill_tree(int amount, int min, int max) noexcept;

	//balances a tree perfectly using DSW algorithm
	void create_perfect_tree() noexcept;

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

int BST::get_root() const noexcept 
{
	return root->val;
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

node* BST::rotate_right(node* gpar, node* par, node* lchild) noexcept
{
	if(gpar != nullptr)
		gpar->right = lchild;
	else
		root = lchild;
	par->left = lchild->right;
	lchild->right = par;
	
	return gpar;
}

void BST::rotate_left(node* gpar, node* par, node* rchild) noexcept
{
	if(gpar != nullptr)
		gpar->right = rchild;
	else
		root = rchild;
	par->right = rchild->left;
	rchild->left = par;
}

void BST::create_backbone() noexcept 
{
	node *gpar = nullptr, *par = root, *lchild = nullptr;

	while(par != nullptr) {
		lchild = par->left;
		if(lchild != nullptr) {
			gpar = rotate_right(gpar, par, lchild);
			par = lchild;
		}
		else {
			gpar = par;
			par = par->right;
		}
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

void BST::fill_tree(int amount, int min, int max) noexcept 
{
	std::vector<int> numbers;
	numbers.reserve(amount);

	std::mt19937::result_type seed = time(0);
	auto random = std::bind(std::uniform_int_distribution<int>(min, max), std::mt19937(seed));

	int number;
	
	for(int i = 0; i < amount; )
	{
		if(i > 0) {
			number = random();
			//skips adding a same number to a vector and bst
			if((std::find(std::begin(numbers), std::end(numbers), number)) != std::end(numbers)) continue;
		}
		numbers.push_back(number);
		insert(numbers.back());
		i++;
	}
}

void BST::create_perfect_tree() noexcept
{
	(*this).create_backbone();

	//number of nodes in a tree
	int nodes {};
	for(node *temp = root; temp != nullptr; temp = temp->right) ++nodes;

	int m = std::pow(2, static_cast<int>(std::log2(nodes+1))) - 1;
	
	auto make_rotation = [this, m](int r) {
		node *gpar = nullptr, *par = root, *rchild = root->right;
		for( ; r > 0; r--) {
			if(rchild != nullptr) {
				rotate_left(gpar, par, rchild);
				gpar = rchild;
				par = gpar->right;
				rchild = par->right;
			}
			else {
				break;
			}
		}
	};
	make_rotation(nodes - m);

	while(m > 1) {
		make_rotation(m /= 2);
	}
}

int BST::height() const noexcept
{
	return height(root);
}

int main() 
{	
	using clock = std::chrono::steady_clock;

	BST tree{};
	tree.fill_tree(100000, 0, 10000000);

	std::cout << "------ HEIGHT OF TREE BEFORE BALANCING: " << tree.height() << "\n\n";

	std::cout << "------ DSW ALGORITHM ------";
	clock::time_point start = clock::now();
	tree.create_perfect_tree();
	clock::time_point end = clock::now();

	std::chrono::duration<double, std::milli> execution_time = end - start;
	std::cout << "\n\n------ TOTAL TIME: " << execution_time.count() << "ms." << std::endl;

	std::cout << "\n------ HEIGHT OF TREE AFTER BALANCING: " << tree.height() << "\n\n";

	return 0;
}