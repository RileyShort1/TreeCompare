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
	Node* _rotate_right(Node*& node)
	{
		
		if (node == nullptr || node->_left == nullptr)
		{
			return nullptr;
		}
		/*

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
		*/
		Node* newParent = node->_left;
		node->_left = newParent->_right;
		newParent->_right = node;
		update(node);
		update(newParent);

		return newParent;
	}

	Node* _rotate_left(Node*& node)
	{
		
		if (node == nullptr || node->_right == nullptr)
		{
			return nullptr;
		}
		/*
		
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
		*/
		Node* newParent = node->_right;
		node->_right = newParent->_left;
		newParent->_left = node;
		update(node);
		update(newParent);

		return newParent;
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

		return node->height;
	}
	int balance(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}

		return (get_height(node->_left) - get_height(node->_right));
	}

	Node* leftLeftCase(Node* node) {
		return _rotate_right(node);
	}

	Node* leftRightCase(Node* node) {
		node->_left = _rotate_left(node->_left);
		return leftLeftCase(node);
	}

	Node* rightRightCase(Node* node) {
		return _rotate_left(node);
	}

	Node* rightLeftCase(Node* node) {
		node->_right = _rotate_right(node->_right);
		return rightRightCase(node);
	}

	Node* _balance(Node* node) // Does not work / untested
	{
		/*
		int bf = (get_height(node->_right) - get_height(node->_left)); // balance factor
		std::cout << "balance factor: " << bf << std::endl;

		// four cases

		// Right Right (is right child) // WORKS
		if (bf < -1 && isRightChild == false)
		{
			std::cout << "1" << std::endl;
			_rotate_right(node);
		}

		// Left Left (is left child) // WORKS
		else if (bf > 1 && isRightChild == false)
		{
			std::cout << "2" << std::endl;
			//std::cout << "Rotating right on node " << node->_data << std::endl;
			_rotate_left(node);
		}

		// Right Left (is right child) // NEEDS FIXING
		else if (bf < -1 && isRightChild == true)
		{
			std::cout << "3" << std::endl;
			_rotate_right(node);
			_rotate_left(node);
		}

		// Left Right (is left child)
		else if (bf > 1 && isRightChild == false) // NEEDS FIXING
		{
			std::cout << "4" << std::endl;
			_rotate_left(node);
			_rotate_right(node);
		}

		return;
		*/
		// Left heavy subtree.
		if (balance(node) == -2) {

			// Left-Left case.
			if (balance(node->_left) <= 0) {
				return leftLeftCase(node);

				// Left-Right case.
			}
			else {
				return leftRightCase(node);
			}

			// Right heavy subtree needs balancing.
		}
		else if (balance(node) == +2) {

			// Right-Right case.
			if (balance(node->_right) >= 0) {
				return rightRightCase(node);

				// Right-Left case.
			}
			else {
				return rightLeftCase(node);
			}
		}

		return node;

	}

	void update(Node* node)
	{
		node->height = (1 + (max(get_height(node->_left), get_height(node->_right))));
		return;
	}

	Node* avl_insert(Node* node, const T& data) // does not yet rebalance
	{
		if (node == nullptr) {
			Node* temp = new Node(data, 1);
			return temp;
		}
		if (data < node->_data) node->_left = avl_insert(node->_left, data);
		else if (data > node->_data) node->_right = avl_insert(node->_right, data);
		//head->height = 1 + max(height(head->left), height(head->right));
			
		update(node);

		// Must rebalance next
		int bal = balance(node);

		//_balance(node);
		if (bal > 1) {
			if (data < node->_left->_data) {
				return _rotate_right(node);
			}
			else {
				node->_left = _rotate_left(node->_left);
				return _rotate_right(node);
			}
		}
		else if (bal < -1) {
			if (data > node->_right->_data) {
				return _rotate_left(node);
			}
			else {
				node->_right = _rotate_right(node->_right);
				return _rotate_left(node);
			}
		}


		return node;
	
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

		/*
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
		*/
		void insert(T x) {
			_root = avl_insert(_root, x);
		}
		bool remove(const T& elem) { return avl_remove(elem); }


	friend class AVLTests;

};



#endif /* AVL_Tree */
