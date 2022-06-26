#ifndef SplayTree_h
#define SplayTree_h
#include <iostream>
#include <queue>
#include <iomanip>
using namespace std;

template<typename T>
class BST {
private:
	struct Node {
		T _data;
		Node* _left, * _right;
		Node(const T& d, Node* l = nullptr, Node* r = nullptr) : _data(d), _left(l), _right(r) {}
	};
	Node* _root;
	size_t _size;

	// Helpers 
	static Node* _deep_copy(const Node* p)
	{
		if (p == nullptr)
		{
			return nullptr;
		}

		Node* return_val = new Node(p->_data, _deep_copy(p->_left), _deep_copy(p->_right));

		return return_val;
	}

	void _splay(Node*& p, const T& x)
	{
		if (p == nullptr)
		{
			return;
		}

		typename Node* mid = new typename Node(0, nullptr, nullptr);

		typename Node* rightTreeMin = mid;
		typename Node* leftTreeMax = mid;

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
					_rotate_with_left_child(p);

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
					_rotate_with_right_child(p);

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

		p->_left = mid->_right;
		p->_right = mid->_left;

		return;
	}

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



	bool splay_insert(const T& x)
	{
		if (_root == nullptr)
		{
			_root = new typename Node(x, nullptr, nullptr);
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
				if (_root->_right != nullptr && _root->_right->_data > x) // we need to attatch on the right of new node
				{
					Node* newNode = new Node(x, nullptr, _root->_right);
					_root->_right = newNode;
				}
				else // we attatch on the left of new node
				{
					Node* newNode = new Node(x, _root->_right, nullptr);
					_root->_right = newNode;
				}
				_size++;
				_splay(_root, x); // NEW
				return true;
			}

			if (x < _root->_data) // x belongs to the left
			{
				if (_root->_left != nullptr && _root->_left->_data > x) // we need to attatch on the right of new node
				{
					Node* newNode = new Node(x, nullptr, _root->_left);
					_root->_left = newNode;
				}
				else // we attatch on the left of new node
				{
					Node* newNode = new Node(x, _root->_left, nullptr);
					_root->_left = newNode;
				}

				_size++;
				_splay(_root, x); // NEW
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
				_root->_left->_right = xRight; // problem 
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

	string _to_string(const Node* p) const
	{
		/*
		string strToReturn = "";

		if (p->_left == nullptr && p->_right == nullptr)
		{
			return "";
		}
		else
		{
			strToReturn += std::to_string(p->_data);
			if (p->_right == nullptr)
			{
				strToReturn += " : " + "[NULL]" + " " + std::to_string(p->_left->_data);
			}
			else
			{
				strToReturn += " : " + std::to_string(p->_right->_data) + " " + "[NULL]";
			}
		}

		return strToReturn;
		*/
		return "";
	}

public:
	BST() : _root(nullptr), _size(0) {}
	virtual ~BST() { _recursive_delete(_root); }

	virtual size_t get_size() const { return _size; }

	virtual bool insert(const T& elem) { return _insert(_root, elem); }
	virtual bool remove(const T& elem) { return _remove(_root, elem); }
	virtual bool clear()
	{
		return _recursive_delete(_root);
	}

	virtual bool contains(const T& elem) const { return _find(_root, elem) != nullptr; }
	virtual const T& find(const T& elem) const
	{
		const Node* result = _find(_root, elem);

		if (result == nullptr)
		{
			throw Not_found_exception();
		}
	
		return result->_data;
	}
	virtual string to_string() const
	{

		string strToReturn = "";
		/*
		strToReturn += "# Tree rooted at " + std::to_string(_root->_data) + "\n";
		strToReturn += "# size = " + std::to_string(_size) + "\n";
		strToReturn += _to_string(_root->_data);
		strToReturn += "# End of tree";
		*/
		return strToReturn;
	}

	class Not_found_exception : public exception {
	public:
		string to_string() { return "Not found exception"; }
	};

	friend class Tx;
	friend class Tests;

};




#endif /* SplayTree_h */
