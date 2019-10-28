#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <sstream>

template <typename T_key, typename T_value>
class SplayTree 
{
public:
	struct Node 
	{
		Node* parent;
		Node* left;
		Node* right;
		T_value value;
		T_key key;
	};

	Node* root;

	SplayTree() 
	{
		root = nullptr;
	}

	~SplayTree()
	{
		destroy(root);
	}

	void destroy(Node* node);
	void add(T_key K, T_value V);
	void set(T_key K, T_value V);
	std::pair<bool, T_value> search(T_key K);
	void splay(Node* node);
	void zig(Node* node);
	void zigzigL(Node* node);
	void zigzigR(Node* node);
	void zigzagL(Node* node);
	void zigzagR(Node* node);
	void left_rotation(Node* node);
	void right_rotation(Node* node);
	std::pair<T_key, T_value> min();
	std::pair<T_key, T_value> max();
	void delete_node(T_key K);
	std::string print();
	size_t find_h(Node* temp, size_t temp_index, size_t temp_h, std::unordered_map< size_t, Node*>& map);

	Node* find_node(T_key K) 
	{
		Node* pre_iterator = root;
		Node* iterator = root;
		while (iterator != nullptr)
		{
			pre_iterator = iterator;
			if (iterator->key == K)
			{
				return iterator;
			}
			else if (iterator->key > K) iterator = iterator->left;
			else iterator = iterator->right;
		}
		return pre_iterator;
	}

	Node* find_max(Node* root_)
	{
		if (root_ == nullptr) throw std::logic_error("operation with empty tree");
		Node * iterator = root_;
		while (iterator->right != nullptr)
		{
			iterator = iterator->right;
		}
		return iterator;
	}

	Node* find_min(Node* root_)
	{
		if (root_ == nullptr) throw std::logic_error("operation with empty tree");
		Node * iterator = root_;
		while (iterator->left != nullptr)
		{
			iterator = iterator->left;
		}
		return iterator;
	}
};

template <typename T_key, typename T_value>
void SplayTree<T_key, T_value>::destroy(Node* node)
{
	if (node != nullptr)
	{
		destroy(node->left);
		destroy(node->right);
		delete node;
	}
}

template <typename T_key, typename T_value>
void SplayTree<T_key, T_value>::add(T_key K, T_value V) 
{
	Node* new_node = new Node;
	new_node->key = K;
	new_node->value = V;
	new_node->left = nullptr;
	new_node->right = nullptr;

	if (root == nullptr) 
	{
		new_node->parent = nullptr;
		root = new_node;
	}
	else 
	{
		Node* iterator = root;
		Node* pre_iterator = nullptr;
		while (iterator != nullptr) 
		{
			pre_iterator = iterator;
			if (new_node->key < iterator->key) iterator = iterator->left;
			else iterator = iterator->right;
		}
		new_node->parent = pre_iterator;
		if (pre_iterator == nullptr) root = new_node;
		else if (new_node->key < pre_iterator->key) pre_iterator->left = new_node;
		else if (new_node->key == pre_iterator->key) 
		{
			delete new_node;
			throw std::logic_error("double add");
		}
		else pre_iterator->right = new_node;
	}
	splay(new_node);
}

template <typename T_key, typename T_value>
void SplayTree<T_key, T_value>::set(T_key K, T_value V)
{
	Node* iterator = root;
	Node* pre_iterator = nullptr;
	while (iterator != nullptr) 
	{
		if (iterator->key == K)
		{
			iterator->value = V;
			splay(iterator);
			return;
		}
		else if (iterator->key > K) iterator = iterator->left;
		else iterator = iterator->right;
	}
	throw std::logic_error("key not find");
}

template<typename T_key, typename T_value>
std::pair<bool, T_value> SplayTree<T_key, T_value>::search(T_key K)
{
	std::pair<T_key, T_value> find_K_pair;
	Node* iterator = find_node(K);
	if (root == nullptr || iterator->key != K)
	{
		std::string empty = "";
		find_K_pair.first = false;
		find_K_pair.second = empty; 
	}
	else 
	{
		find_K_pair.first = true;
		find_K_pair.second = iterator->value;
	}
	splay(iterator);
	return find_K_pair;
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::splay(Node* x)
{
	while (x != root)
	{
		if (x->parent == root) zig(x);
		else 
		{
			if ((x == x->parent->left) && (x->parent == x->parent->parent->left)) zigzigL(x);
			else if ((x == x->parent->right) && (x->parent == x->parent->parent->right)) zigzigR(x);		
			else if ((x == x->parent->left) && (x->parent == x->parent->parent->right)) zigzagL(x);
			else if ((x == x->parent->right) && (x->parent == x->parent->parent->left)) zigzagR(x);
		}
	}
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::zig(Node* node)
{
	if (node == node->parent->left) right_rotation(node->parent);
	else left_rotation(node->parent);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::zigzigL(Node* node)
{
	right_rotation(node->parent->parent);
	right_rotation(node->parent);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::zigzigR(Node* node)
{
	left_rotation(node->parent->parent);
	left_rotation(node->parent);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::zigzagL(Node* node)
{
	right_rotation(node->parent);
	left_rotation(node->parent);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::zigzagR(Node* node)
{
	left_rotation(node->parent);
	right_rotation(node->parent);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::left_rotation(Node* x)
{
	Node* y = x->right;
	x->right = y->left;
	if (y->left != nullptr) y->left->parent = x;
	y->parent = x->parent;
	if (x->parent == nullptr) root = y;
	else if (x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	y->left = x;
	x->parent = y;
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::right_rotation(Node* x)
{
	Node* y = x->left;
	x->left = y->right;
	if (y->right != nullptr) y->right->parent = x;
	y->parent = x->parent;
	if (x->parent == nullptr) root = y;
	else if (x == x->parent->left) x->parent->left = y;
	else x->parent->right = y;
	y->right = x;
	x->parent = y;
}

template<typename T_key, typename T_value>
std::pair<T_key, T_value> SplayTree<T_key, T_value>::min()
{
	Node* min = find_min(root);
	splay(min);
	return std::pair<T_key, T_value>(min->key, min->value);
}

template<typename T_key, typename T_value>
std::pair<T_key, T_value> SplayTree<T_key, T_value>::max()
{
	Node* max = find_max(root);
	splay(max);
	return std::pair<T_key, T_value>(max->key, max->value);
}

template<typename T_key, typename T_value>
void SplayTree<T_key, T_value>::delete_node(T_key K)
{
	Node* delete_node = find_node(K);
	if (delete_node==nullptr || delete_node->key != K ) throw std::logic_error("vertex not find");
	splay(delete_node);
	Node* left_tree = root->left;
	Node* right_tree = root->right;
	
	
	if (left_tree == nullptr && right_tree == nullptr) 
	{
		delete root;
		root = nullptr;
	}
	else if (left_tree == nullptr) 
	{
		right_tree->parent = nullptr;
		delete root;
		root = right_tree;
	}
	else if (right_tree == nullptr) 
	{
		left_tree->parent = nullptr;
		delete root;
		root = left_tree;
	}
	else 
	{
		left_tree->parent = nullptr;
		right_tree->parent = nullptr;
		delete root;
		root = left_tree;
		Node* max_in_left = find_max(left_tree);
		splay(max_in_left);
		root = max_in_left;
		root->right = right_tree;
		right_tree->parent = root;
	}
}

template<typename T_key, typename T_value>
std::string SplayTree<T_key, T_value>::print()
{
	if (root == nullptr) return "_";
		//throw std::logic_error("print empty tree");
	std::queue<Node*> queue;
	std::string out = "";
	queue.push(root->left);
	queue.push(root->right);
	std::unordered_map< size_t, Node*> map;
	size_t h = find_h(root, 0, 0, map);
	size_t counter_el = 2, counter_h = 1;
	out += '[' + std::to_string(root->key) + ' ' + root->value + ']';
	if (root->left != nullptr || root->right != nullptr) out += '\n';
	else if (root->left == nullptr && root->right == nullptr) return out;
	while (!queue.empty())
	{
		Node* time = queue.front();
		queue.pop();
		if (time == nullptr)
		{
			out += "_";
			++counter_el;
			if (h-1!=counter_h) 
			{
				queue.push(nullptr);
				queue.push(nullptr);
			}	
		}
		else 
		{
			out += '[' + std::to_string(time->key) + ' ' + time->value + ' ' + std::to_string(time->parent->key) +']';
			++counter_el;
			if (counter_h != h - 1) 
			{
				queue.push(time->left);
				queue.push(time->right);
			}
		}

		if ((counter_el & (counter_el - 1)) == 0 )
		{
			if (counter_h < h-1) out += '\n';
			counter_h++;
		}		
		else out += ' ';
	}
	return out;
}

template<typename T_key, typename T_value>
size_t SplayTree<T_key, T_value>::find_h(Node * temp, size_t temp_index, size_t temp_h, std::unordered_map< size_t, Node*>& map)
{
	if (temp != nullptr) 
	{
		size_t new_temp_h = temp_h + 1;
		size_t left_index = 2 * temp_index + 1;
		size_t right_index = 2 * temp_index + 2;
		map[temp_index] = temp;
		return std::max(find_h(temp->left, left_index, new_temp_h, map),
			find_h(temp->right, right_index, new_temp_h, map));
	}
	return temp_h;
}


	
int main()
{
	
	std::string temp_1, temp_2, temp_3, output = "";
	SplayTree<long long, std::string> My_tree;
	std::getline(std::cin, temp_1, '\0');
	std::istringstream line_stream(temp_1);
	while (line_stream >> temp_1)
	{
		try
		{
			temp_2.clear(), temp_3.clear();
			if (temp_1 == "print" && line_stream.peek() == '\n')
			{
				output += My_tree.print() + '\n';
			}
			else if (temp_1 == "min" && line_stream.peek() == '\n')
			{
				std::pair<long long, std::string> pair = My_tree.min();
				output += std::to_string(pair.first) + ' ' + pair.second + '\n';
			}
			else if (temp_1 == "max" && line_stream.peek() == '\n')
			{
				std::pair<long long, std::string> pair = My_tree.max();
				output += std::to_string(pair.first) + ' ' + pair.second + '\n';
			}
			else if (temp_1 == "search" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() == '\n')
			{
				std::pair<bool, std::string> pair = My_tree.search(std::stoull(temp_2));
				if (!pair.first) output += "0\n";
				else output += std::to_string(pair.first) + ' ' + pair.second + '\n';
			}
			else if (temp_1 == "delete" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() == '\n')
			{
				My_tree.delete_node(std::stoll(temp_2));
			}
			else if (temp_1 == "set" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() != '\n' && 
				line_stream >> temp_3 && line_stream.peek() == '\n')
			{
				My_tree.set(std::stoll(temp_2), temp_3);
			}
			else if (temp_1 == "add" && line_stream.peek() != '\n' && line_stream >> temp_2 && line_stream.peek() != '\n' && 
				line_stream >> temp_3 && line_stream.peek() == '\n')
			{
				My_tree.add(std::stoll(temp_2), temp_3);
			}
			else
			{
				line_stream.ignore(256, '\n');
				output += "error\n";
			}
		}
		catch (...)
		{
			output += "error\n";
			line_stream.ignore(256, '\n');
		}
	}

	if (output != "") output.pop_back();
	std::cout << output;
	return 0;
}

