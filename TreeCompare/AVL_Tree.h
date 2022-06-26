#ifndef AVL_Tree_h
#define AVL_Tree_h

template<typename T> 
class AVL_Tree {
private:

	struct Node {
		T _data;
		Node* _left, * _right;
		size_t _balance;
		size_t _height;

		Node(const T& d, size_t b, size_t h Node* l = nullptr, Node* r = nullptr) : _data(d), _left(l), _right(r), _balance(b), _height(h) {}
	};
	Node* _root;
	size_t _size;


	// rotation helpers
	void _rotate_with_left_child(Node*& p)
	{
		if (p == nullptr || p->_left == nullptr)
		{
			return;
		}
		// Rotation of A and B where p is A and p->_left is B
		typename BST<T>::Node* bLeft = p->_left->_left;
		typename BST<T>::Node* bRight = p->_left->_right;
		typename BST<T>::Node* aRight = p->_right;

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
		typename BST<T>::Node* bRight = p->_right->_right;
		typename BST<T>::Node* bLeft = p->_right->_left;
		typename BST<T>::Node* aLeft = p->_left;

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


};



#endif /* AVL_Tree */
