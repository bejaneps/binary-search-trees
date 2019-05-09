#include <iostream>		//std::cerr, std::cout
#include <random>		//std::mt19937
#include <functional>	//std::bind
#include <stack>		//std::stack for inorder_iterative
#include <chrono>		//std::chrono for time calculation

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

public:
	BST();
	BST(int val);
	~BST();

	//performs depth-first inorder recursive traversal
	void inorder_recursive();

	//performs depth-first inorder iterative traversal
	void inorder_iterative();

	//inserts a value into a tree
	void insert(int val) noexcept;

	//fills the tree with random numbers(using std::mt19937 generator) of specified amount within a min and max range inclusive
	void fill_tree(int amount, int min, int max) noexcept;
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

void BST::inorder_recursive(node *r)
{
	if(r)
	{
		inorder_recursive(r->left);
		std::cout << r->val << " ";
		inorder_recursive(r->right);
	}
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
	std::mt19937::result_type seed = time(0);
	auto random = std::bind(std::uniform_int_distribution<int>(min, max), std::mt19937(seed));

	for(int i = 0; i < amount; i++)
	{
		insert(random());
	}
}

int main() 
{	
	using clock = std::chrono::steady_clock;

	BST tree{};
	tree.fill_tree(100, 1, 1000000);

	clock::time_point start = clock::now();
	std::cout << "------ INORDER ITERATIVE ------\n\n";
	tree.inorder_iterative();
	clock::time_point end = clock::now();

	std::chrono::duration<double, std::milli> execution_time = end - start;
	std::cout << "\n\n------ TOTAL TIME: " << execution_time.count() << "ms." << std::endl;

	start = clock::now();
	std::cout << "------ INORDER RECURSIVE ------\n\n";
	tree.inorder_recursive();
	end = clock::now();

	execution_time = end - start;
	std::cout << "\n\n------ TOTAL TIME: " << execution_time.count() << "ms." << std::endl;

	return 0;
}