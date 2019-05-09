#include <iostream>		//std::cerr, std::cout
#include <random>		//std::mt19937
#include <functional>	//std::bind
#include <stack>		//std::stack for inorder_iterative
#include <algorithm>	//std::find
#include <chrono>		//std::chrono
#include <ctime>		//std::time

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

	//needed for recursion
	void inorder_recursive(node *r);

	//algorithm to implement rotation of nodes
	node* rotate_child(node* gpar, node* par, node* lchild) noexcept;

public:
	BST();
	BST(int val);
	~BST();

	//returns a value holded by root node
	int get_root() const noexcept;

	//performs depth-first inorder recursive traversal
	void inorder_recursive();

	//performs depth-first inorder iterative traversal
	void inorder_iterative();

	//inserts a value into a tree
	void insert(int val) noexcept;

	//fills the tree with random numbers(using std::mt19937 generator) of specified amount within a min and max range inclusive
	void fill_tree(int amount, int min, int max) noexcept;

	//traverses a bst using morris inorder algorithm
	void morris_inorder() const noexcept;

	void create_backbone() noexcept;
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

void BST::inorder_recursive(node *r)
{
	if(r)
	{
		inorder_recursive(r->left);
		std::cout << r->val << " ";
		inorder_recursive(r->right);
	}
}

node* BST::rotate_child(node* gpar, node* par, node* lchild) noexcept
{
	if(gpar != nullptr)
		gpar->right = lchild;
	else
		root = lchild;
	par->left = lchild->right;
	lchild->right = par;
	
	return gpar;
}

void BST::inorder_recursive()
{
	inorder_recursive(root);
}

void BST::inorder_iterative()
{
	std::stack<node*> s;
	node *p = root;

	while(p)
	{
		while(p)
		{
			if(p->right)
				s.push(p->right);
			s.push(p);
			p = p->left;
		}
		p = s.top();
		s.pop();
		while(!s.empty() && p->right == nullptr)
		{
			std::cout << p->val << " ";
			p = s.top();
			s.pop();
		}
		std::cout << p->val << " ";
		if(!s.empty())
		{
			p = s.top();
			s.pop();
		}
		else p = nullptr;
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

void BST::morris_inorder() const noexcept
{
	node *p = root, *tmp = nullptr;
	while(p != nullptr)
		if(p->left == nullptr) {
			std::cout << p->val << " ";
			p = p->right;
		}
		else {
			tmp = p->left;
			while(tmp->right != nullptr && tmp->right != p)
				tmp = tmp->right;
			if(tmp->right == nullptr) {
				tmp->right = p;
				p = p->left;
			}
			else {
				std::cout << p->val << " ";
				tmp->right = nullptr;

				p = p->right;
			}
		}
}

void BST::create_backbone() noexcept 
{
	node *gpar = nullptr, *par = root, *lchild = nullptr;

	while(par != nullptr) {
		lchild = par->left;
		if(lchild != nullptr) {
			gpar = rotate_child(gpar, par, lchild);
			par = lchild;
		}
		else {
			gpar = par;
			par = par->right;
		}
	}
	//traverse in a straightforward way
	while(root != nullptr) {
		std::cout << root->val << " ";
		root = root->right;
	}
}

int main() 
{	
	using clock = std::chrono::steady_clock;

	BST tree{};
	tree.fill_tree(100000, 0, 10000000);

	std::cout << "------ MORRIS ALGORITHM ------\n\n";
	clock::time_point start = clock::now();
	tree.morris_inorder();
	clock::time_point end = clock::now();

	std::chrono::duration<double, std::milli> execution_time = end - start;
	std::cout << "\n\n------ TOTAL TIME: " << execution_time.count() << "ms." << std::endl;

	std::cout << "------ DSW ALGORITHM ------\n\n";
	start = clock::now();
	tree.create_backbone();
	end = clock::now();

	execution_time = end - start;
	std::cout << "\n\n------ TOTAL TIME: " << execution_time.count() << "ms." << std::endl;

	return 0;
}