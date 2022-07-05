#ifndef SplayTree_h
#define SplayTree_h
#include <string>


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

		Node* mid = new Node(0, nullptr, nullptr);

		Node* rightTreeMin = mid;
		Node* leftTreeMax = mid;

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

				rightTreeMin->_left = p;
				rightTreeMin = rightTreeMin->_left;
				p = p->_left;
				rightTreeMin->_left = nullptr;
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

				// add root to leftTree at its maximum node - update the leftTreeMax to point to this node
				leftTreeMax->_right = p;
				leftTreeMax = leftTreeMax->_right;
				p = p->_right;
				leftTreeMax->_right = nullptr;
			}

			else // we have found the value
			{
				break;
			}
		}

		if (leftTreeMax != nullptr)
		{
			leftTreeMax->_right = p->_left;
		}
		if (rightTreeMin != nullptr)
		{
			rightTreeMin->_left = p->_right;
		}

		if (p != nullptr)
		{
			p->_left = mid->_right;
			p->_right = mid->_left;
		}

		delete mid;

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

		// else dismantle
		if (_root->_data != x)
		{
			if (x > _root->_data) // x belongs to the the right
			{
				if (_root->_right != nullptr && _root->_right->_data > x) // we need to attach on the right of new node
				{
					Node* newNode = new Node(x, nullptr, _root->_right);
					_root->_right = newNode;
				}
				else // we attach on the left of new node
				{
					Node* newNode = new Node(x, _root->_right, nullptr);
					_root->_right = newNode;
				}
				_size++;
				_splay(_root, x);
				return true;
			}

			if (x < _root->_data) // x belongs to the left
			{
				if (_root->_left != nullptr && _root->_left->_data > x) // we need to attach on the right of new node
				{
					Node* newNode = new Node(x, nullptr, _root->_left);
					_root->_left = newNode;
				}
				else // we attach on the left of new node
				{
					Node* newNode = new Node(x, _root->_left, nullptr);
					_root->_left = newNode;
				}

				_size++;
				_splay(_root, x);
				return true;
			}
		}

		return false;
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

		if (_root->_left == nullptr)
		{
			Node* temp1 = _root;
			_root = _root->_right;
			temp1->_left = nullptr;
			temp1->_right = nullptr;
			delete temp1;
			_size--;
			return true;
		}

		if (_root->_right == nullptr)
		{
			Node* temp1 = _root;
			_root = _root->_left;
			temp1->_left = nullptr;
			temp1->_right = nullptr;
			delete temp1;
			_size--;
			return true;
		}

		if (_root->_data == x) // we must remove x
		{
			// splay left child
			_splay(_root->_left, x);


			Node* newRoot = _root->_left;
			Node* xRight = _root->_right;

			if (_root->_left != nullptr)
			{
				_root->_left->_right = xRight;
			}

			Node* temp = _root;
			_root = newRoot;

			temp->_left = nullptr;
			temp->_right = nullptr;
			delete temp;
		}
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

	friend class SplayTests;
};

#endif /* SplayTree_h */
