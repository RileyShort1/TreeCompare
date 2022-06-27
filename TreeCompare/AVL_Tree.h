#ifndef AVL_Tree_h
#define AVL_Tree_h

template<typename T> 
class AVL_Tree {
private:

	struct Node {
		T _data;
		Node* _left, * _right;
		int height;
		Node(const T& d, Node* l = nullptr, Node* r = nullptr) : _data(d), _left(l), _right(r) {}
		// We need to add the new node variables in order to make it an AVL tree
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

	bool avl_insert(const T& data);

	bool avl_remove(const T& data);




};



#endif /* AVL_Tree */
