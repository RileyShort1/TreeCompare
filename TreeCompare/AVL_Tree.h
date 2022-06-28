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

	int _get_balance_factor(const Node* n)
	{
		if (n == nullptr || n->_left == nullptr || n->_right == nullptr)
		{
			return 0;
		}

		return n->_right->height - n->_left->height;
	}

	void _balance(Node* node); // this may be useful

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

		return node->height;
	}

	bool avl_insert(const T& data)
	{
		if (_root == nullptr) // if no nodes, build root
		{
			_root = new Node(data, 0); // single node height 0
			_size++;
			return true;
		}

		if (avl_find(data) == true) // already in tree
		{
			return true;
		}

		// should be able to insert as we would a regular BST
		// then rebalance as necessary
		Node* temp = _root;

		while (temp != nullptr)
		{
			if (data > temp->_data) // move right
			{
				if (temp->_right == nullptr) // we insert
				{
					temp->_right = new Node(data, 0); // added Node, Done
					_size++;
					break;
				}
			
				temp = temp->_right;
			}

			else if (data < temp->_data) // move left
			{
				if (temp->_left == nullptr) // we insert
				{
					temp->_left = new Node(data, 0);
					_size++;
					break;
				}
				
				temp = temp->_left;
			}
		}

		temp->height = 1 + max(get_height(temp->_left), get_height(temp->_right));
		_root->height = 1 + max(get_height(_root->_left), get_height(_root->_right));

		// Must rebalance next

		return true;
	}

	bool avl_remove(const T& data)
	{
		if (avl_find(data) == false)
		{
			return false; // data not in tree (we could return either true or false - but I think false makes sense)
		}
	}

	public:
		AVL_Tree() : _root(nullptr), _size(0) {}

		bool insert(const T& elem) { return avl_insert(elem); }
		bool remove(const T& elem) { return avl_remove(elem); }


	friend class AVLTests;

};



#endif /* AVL_Tree */
