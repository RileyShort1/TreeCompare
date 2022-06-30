#ifndef AVL_Tree_h
#define AVL_Tree_h

#include <iostream>

template<typename T> 
class AVL_Tree {
private:

	struct Node {
		T _data;
		Node* _left, * _right;
		int _height;
		Node(const T& d, int h, Node* l = nullptr, Node* r = nullptr) : _data(d), _height(h), _left(l), _right(r) {}
	};
	Node* _root;
	size_t _size;

	// rotation helpers
	Node* _rotate_right(Node*& node)
	{
		if (node == nullptr || node->_left == nullptr)
		{
			return nullptr;
		}
		
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

		return node->_height;
	}
	int get_balance(Node* node)
	{
		if (node == nullptr)
		{
			return 0;
		}

		return (get_height(node->_left) - get_height(node->_right));
	}

	void update(Node* node)
	{
		node->_height = (1 + (max(get_height(node->_left), get_height(node->_right))));

		return;
	}

	Node* avl_insert(Node* node, const T& data) // DONE
	{
		if (node == nullptr) 
		{
			Node* temp = new Node(data, 1);

			return temp;
		}

		if (data < node->_data)
		{
			node->_left = avl_insert(node->_left, data);
		}

		else if (data > node->_data)
		{
			node->_right = avl_insert(node->_right, data);
		}
				
		update(node);

		// Must rebalance next
		int bf = get_balance(node);

		if (bf > 1)
		{
			if (data < node->_left->_data) 
			{
				return _rotate_right(node);
			}
			else 
			{
				node->_left = _rotate_left(node->_left);
				return _rotate_right(node);
			}
		}
		else if (bf < -1) 
		{
			if (data > node->_right->_data)
			{
				return _rotate_left(node);
			}
			else 
			{
				node->_right = _rotate_right(node->_right);
				return _rotate_left(node);
			}
		}

		return node;
	}

	bool avl_remove(const T& data) // needs to be built 
	{
		
	}

	public:
		AVL_Tree() : _root(nullptr), _size(0) {}

		bool insert(const T& elem) 
		{
			if (avl_find(elem) == true) // already in tree
			{
				return true;
			}
			
			_root = avl_insert(_root, elem);
			_size++;
			return true;
		}

		bool remove(const T& elem) 
		{ 
			if (avl_find(elem) == false) // Not in tree
			{
				return true;
			}

			_size--;
			return avl_remove(elem); 
		}

	friend class AVLTests;

};



#endif /* AVL_Tree */
