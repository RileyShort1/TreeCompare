#ifndef SplayTree_h
#define SplayTree_h

template<typename T>
class SplayTree {
private:
	struct Node {
		T _data;
		Node* _left, * _right;
		Node(const T& d, Node* l = nullptr, Node* r = nullptr) : _data(d), _left(l), _right(r) {}
	};
	Node* _root;
	size_t _size;

	void _splay(Node*& p, const T& x)
	{
		if (p == nullptr)
		{
			return;
		}

		Node* left_t = nullptr;
		Node* right_t = nullptr; 
		Node* leftMax = nullptr;
		Node* rightMin = nullptr;

		while (p != nullptr)
		{
			if (x < p->_data)
			{
				if (p->_left == nullptr) // Item is not in tree
				{
					break;
				}

				if (x < p->_left->_data) // We zig zig left
				{
					p = _rotate_right(p);

					if (p->_left == nullptr) // item is not in tree
					{
						break;
					}
				}

				// add root to rightTree at its minimum node - 
				// update the rightTreeMin to point to this node

				if (right_t == nullptr)
				{
					right_t = p;
				}
				else
				{
					rightMin->_left = p;
				}

				rightMin = p;
				p = p->_left;
			}

			else if (x > p->_data)
			{
				if (p->_right == nullptr) // value is not in tree
				{
					break;
				}

				if (x > p->_right->_data) // zig zig right
				{
					p = _rotate_left(p);

					if (p->_right == nullptr) // item is not in tree
					{
						break;
					}
				}

				if (left_t == nullptr)
				{
					left_t = p;
				}
				else
				{
					leftMax->_right = p;
				}

				leftMax = p;
				p = p->_right;
			}

			else // we have found the value
			{
				break;
			}
		}

		if (left_t != nullptr)
		{
			leftMax->_right = p->_left;
			p->_left = left_t;
		}
		if (right_t != nullptr)
		{
			rightMin->_left = p->_right;
			p->_right = right_t;
		}

		return;	
	}

	Node* _rotate_right(Node*& node)
	{
		if (node == nullptr || node->_left == nullptr)
		{
			return nullptr;
		}

		Node* newParent = node->_left;
		node->_left = newParent->_right;
		newParent->_right = node;

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

		return newParent;
	}

	bool splay_insert(const T& x)
	{
		if (_root == nullptr)
		{
			_root = new Node(x, nullptr, nullptr);
			_size++;
			return true;
		}

		_splay(_root, x);

		if (_root->_data == x) // item was already in tree
		{
			return false;
		}

		if (x < _root->_data) // root shifts right
		{
			Node* new_root = new Node(x, _root->_left, _root);
			_root->_left = nullptr;
			_root = new_root;
		}
		else if (x > _root->_data) // root shifts left
		{
			Node* new_root = new Node(x, _root, _root->_right);
			_root->_right = nullptr;
			_root = new_root;
		}

		_size++;
		return true;	
	}

	bool splay_remove(const T& x)
	{
		if (_root == nullptr)
		{
			return false;
		}

		_splay(_root, x);

		if (_root->_data != x) // item was not in tree
		{
			return false;
		}

		Node* new_root = nullptr;

		if (_root->_left == nullptr)
		{
			new_root = _root->_right;
		}
		else
		{
			new_root = _root->_left;
			_splay(new_root, x);
			new_root->_right = _root->_right;
		}

		delete _root;
		_root = new_root;
		_size--;

		return true;
	}

	const Node* splay_get(const T& x)
	{
		if (_root == nullptr)
		{
			return nullptr;
		}

		_splay(_root, x);

		if (_root->_data != x) 
		{
			return nullptr;
		}

		return _root;
	}

	bool splay_contains(const T& x) 
	{
		if (splay_get(x) == nullptr)
		{
			return false;
		}

		return true;
	}

public:
	SplayTree() : _root(nullptr), _size(0) {}

	size_t get_size() const { return _size; }

	bool contains(const T& elem) { return splay_contains(elem); }
	const Node* get(const T& elem) { return splay_get(elem); }
	
	bool insert(const T& elem) { return splay_insert(elem); }
	bool remove(const T& elem) { return splay_remove(elem); }

	//friend class SplayTests;
};

#endif /* SplayTree_h */
