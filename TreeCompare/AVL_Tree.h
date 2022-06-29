#ifndef AVL_Tree_h
#define AVL_Tree_h

#include <iostream>

template<typename T> 
class AVL_Tree {
private:

	struct Node {
		T _data;
		Node* _left, * _right;
		int height;
		Node(const T& d, int h, Node* l = nullptr, Node* r = nullptr) : _data(d), height(h), _left(l), _right(r) {}
	};
	Node* _root;
	size_t _size;

	// rotation helpers (same ones from the splay tree)
	void _rotate_with_left_child(Node*& p)
	{
		if (p == nullptr || p->_left == nullptr)
		{
			return;
		}

		// Rotation of A and B where p is A and p->_left is B
		Node* bLeft = p->_left->_left;
		Node* bRight = p->_left->_right;
		Node* aRight = p->_right;

		// now we can cut off the nodes
		p->_right = p->_left;
		p->_left = nullptr;
		T data = p->_data;
		p->_data = p->_right->_data;
		p->_right->_data = data;

		// correct new roots node ptr's
		p->_left = bLeft;
		p->_right->_left = bRight;
		p->_right->_right = aRight;

		return;
	}

	void _rotate_with_right_child(Node*& p)
	{
		if (p == nullptr || p->_right == nullptr)
		{
			return;
		}

		// Rotation of A and B where p is A and p->_left is B
		Node* bRight = p->_right->_right;
		Node* bLeft = p->_right->_left;
		Node* aLeft = p->_left;

		// now we can cut off the nodes
		p->_left = p->_right;
		p->_right = nullptr;
		T data = p->_data;
		p->_data = p->_left->_data;
		p->_left->_data = data;

		// correct new roots node ptr's
		p->_right = bRight;
		p->_left->_right = bLeft;
		p->_left->_left = aLeft;

		return;
	}

	const Node* avl_get(Node* p, const T& elem) const
	{
		const Node* temp = p;

		while (temp != nullptr)
		{
			if (temp->_data == elem)
			{
				return temp;
			}

			if (elem > temp->_data) // move right
			{
				temp = temp->_right;
			}
			else if (elem < temp->_data) // move left
			{
				temp = temp->_left;
			}
		}

		return nullptr;
	}

	bool avl_find(const T& data)
	{
		if (avl_get(_root, data) == nullptr)
		{
			return false;
		}

		return true;
	}

	// NOTES 
	// Balance factor (height ot right subtree - height of left subtree)
	// balance factor shuld always be -1, 0, or 1;

	// height of a node can be calculated as the number of edges/connections between 
	// the node and the deepest node on it's subtree

	// this means single nodes with no children have a height 0 

	int max(int val, int altval) // helpers
	{
		if (val > altval)
		{
			return val;
		}

		return altval;
	}

	int get_height(Node* node) // helpers
	{
		if (node == nullptr)
		{
			return 0;
		}
		//std::cout << "here: " << node->_data << std::endl;

		return node->height;
	}

	void _balance(Node* node, bool isRightChild) // Does not work / untested
	{
		int bf = (get_height(node->_right) - get_height(node->_left)); // balance factor
		std::cout << "balance factor: " << bf << std::endl;

		// four cases

		// Right Right (is right child)
		if (bf < -1 && isRightChild == true)
		{
			std::cout << "1" << std::endl;
			_rotate_with_left_child(node);
		}

		// Left Left (is left child)
		else if (bf > 1 && isRightChild == false)
		{
			std::cout << "2" << std::endl;
			_rotate_with_right_child(node);
		}

		// Right Left (is right child)
		else if (bf < -1 && isRightChild == true)
		{
			std::cout << "3" << std::endl;
			_rotate_with_right_child(node);
			_rotate_with_left_child(node);
		}

		// Left Right (is left child)
		else if (bf > 1 && isRightChild == false)
		{
			std::cout << "4" << std::endl;
			_rotate_with_left_child(node);
			_rotate_with_right_child(node);
		}

		return;

	}

	void update(Node* node)
	{
		node->height = (1 + (max(get_height(node->_left), get_height(node->_right))));
		return;
	}

	bool avl_insert(Node* node, const T& data) // does not work (it can insert like a BST, but can't keep track of height)
	{
		if (data < node->_data) // go left
		{
			if (node->_left == nullptr)
			{
				node->_left = new Node(data, 0);
			}
			else {
				avl_insert(node->_left, data);
			}
		}
		
		else if (data > node->_data)
		{
			if (node->_right == nullptr)
			{
				node->_right = new Node(data, 0);
			}
			else {
				avl_insert(node->_right, data);
			}
		}
			
		node->height = 1 + max(get_height(node->_left),
			get_height(node->_right));

		// Must rebalance next


		return true;
	
	}

	bool avl_remove(const T& data) // incomplete
	{
		if (avl_find(data) == false)
		{
			return false; // data not in tree (we could return either true or false - but I think false makes sense)
		}
	}

	public:
		AVL_Tree() : _root(nullptr), _size(0) {}

		bool insert(const T& elem) {

			if (_root == nullptr) // if no nodes, build root
			{
				_root = new Node(elem, 0); // single node height 0
				_size++;
				return true;
			}

			if (avl_find(elem) == true) // already in tree
			{
				return true;
			}

			return avl_insert(_root, elem); 
			
		}
		bool remove(const T& elem) { return avl_remove(elem); }


	friend class AVLTests;

};



#endif /* AVL_Tree */
