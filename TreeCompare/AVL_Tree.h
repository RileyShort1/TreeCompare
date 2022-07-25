#ifndef AVL_Tree_h
#define AVL_Tree_h

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
		if (node == nullptr) // base case adding new node
		{
			Node* temp = new Node(data, 1);
			_size++;

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
    
    //remove helper

	Node* maxNode(Node* node)
	{
		Node* max = node;

		while (max->_right != nullptr)
		{
			max = max->_right;
		}

		return max;
	}
    
	Node* avl_remove(Node* root, const T& data)
	{
        if (root == nullptr) { //basic null condition
            return nullptr;
        }
        
        if (data < root->_data) {     //search left tree
            
            root->_left = avl_remove(root->_left, data);
        }
        
        else if (data > root->_data) {     //search right tree
            
            root->_right = avl_remove(root->_right, data);
        }

        else {

            _size--;
            
            if ((root->_left == nullptr) || (root->_right == nullptr)) { // one or fewer child nodes
                
                Node *temp = nullptr;
                
                if (root->_left != nullptr) { //if left is only child
                    temp = root->_left;
                }

                else if (root->_right != nullptr) { //if right is only child
                    temp = root->_right;
                }
                
                if (temp == nullptr) {        // no child
                    
                    temp = root;
                    root = nullptr;
                }

                else {                       //one child
                    *root = *temp;   
                }

                if (temp != nullptr)
                {
                    delete temp;
                }   
            }

            else {
                
                Node* temp = maxNode(root->_left);
                
                root->_data = temp->_data;
                
                root->_left = avl_remove(root->_left, temp->_data);
            }

        }
        
        if (root == nullptr) { //basic null condition

            return nullptr;
        }
         
        // get new height
        update(root);
     
        //balance of root
        int bal = get_balance(root);
     
        // double left
        if (bal > 1 && get_balance(root->_left) >= 0){
            
            return _rotate_right(root);
        }
     
        // left right
        if (bal > 1 && get_balance(root->_left) < 0){
            
            root->_left = _rotate_left(root->_left);
            return _rotate_right(root);
        }
     
        // double right
        if (bal < -1 && get_balance(root->_right) <= 0){
            
            return _rotate_left(root);
        }
     
        // right left
        if (bal < -1 && get_balance(root->_right) > 0){
            
            root->_right = _rotate_right(root->_right);
            return _rotate_left(root);
        }
     
        return root;  
	}

	public:
		AVL_Tree() : _root(nullptr), _size(0) {}
		size_t get_size() { return _size; }
		bool contains(const T& data) { return avl_find(data); }

		bool insert(const T& elem)
		{
			int size = _size;
			_root = avl_insert(_root, elem);

			if (size == _size)
			{
				return false;
			}

			return true;
		}

		bool remove(const T& elem) 
		{ 
			int size = _size;

			_root = avl_remove(_root, elem);

			if (size != _size)
			{
				_size = size - 1;
				return true;
			}
			
			return false;
		}

	//friend class AVLTests;
};

#endif /* AVL_Tree */
