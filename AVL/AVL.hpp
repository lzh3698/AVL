#include <iostream>
#include <algorithm>
#include <queue>
#include <stdexcept>

template<class T>
class AVL_tree {
public:

	class Node;

	AVL_tree() : root(nullptr)
	{
		
	}

	~AVL_tree() {
		level_order([](Node* node) {
			delete node;
			});
	}

	// 插入
	void insert(const T& val) {
		root = insert(root, val);
	}

	// 删除
	void remove(const T& val) {
		root = remove(root, val);
	}

	// 前序遍历
	void pre_order() const {
		preorder(root);
		std::cout << std::endl;
	}

	// 中序遍历
	void in_order() const {
		inorder(root);
		std::cout << std::endl;
	}

	// 后序遍历
	void post_order() const {
		postorder(root);
		std::cout << std::endl;
	}

	// 层序遍历
	void level_order() const {
		level_order([](Node *node) {
			cout << node->val << " ";
			});
		std::cout << std::endl;
	}

	// 查找元素
	bool find(const T& val) const {
		return find(root, val);
	}

	// 获取最大元素
	const T& max() const {
		if (root) {
			Node* pre = root;
			Node* cur = root->right;
			while (cur) {
				pre = cur;
				cur = cur->right;
			}
			return pre->val;
		}
		throw runtime_error("No element!");
	}

	// 获取最小元素
	const T& min() const {
		if (root) {
			Node* pre = root;
			Node* cur = root->left;
			while (cur) {
				pre = cur;
				cur = cur->left;
			}
			return pre->val;
		}
		throw runtime_error("No element!");
	}

private:

	struct Node {
		Node(const T& v, Node* l = nullptr, Node* r = nullptr, int h = 1)
			: val(v), left(l), right(r), height(h)
		{

		}

		T val;
		Node* left;
		Node* right;
		int height;
	};

	Node* root;

	// 递归查找
	bool find(Node* node, const T& val) const {
		if (!node) {
			return false;
		}
		if (node->val < val) {
			return find(node->right, val);
		}
		else if (node->val > val) {
			return find(node->left, val);
		}
		else {
			return true;
		}
	}

	void preorder(Node* node) const {
		if (!node) {
			return;
		}
		std::cout << node->val << " ";
		preorder(node->left);
		preorder(node->right);
	}

	void inorder(Node* node) const {
		if (!node) {
			return;
		}
		inorder(node->left);
		std::cout << node->val << " ";
		inorder(node->right);
	}

	void postorder(Node* node) const {
		if (!node) {
			return;
		}
		postorder(node->left);
		postorder(node->right);
		std::cout << node->val << " ";
	}

	template<typename U>
	void level_order(const U& deal) const {
		std::queue<Node*> que;
		que.push(root);
		while (!que.empty()) {
			Node* cur = que.front();
			que.pop();
			if (cur->left) {
				que.push(cur->left);
			}
			if (cur->right) {
				que.push(cur->right);
			}
			deal(cur);
		}
	}

	Node* insert(Node* node, const T& val) {
		if (!node) {
			return new Node(val);
		}

		if (node->val > val) {
			node->left = insert(node->left, val);
		}
		else if (node->val < val) {
			node->right = insert(node->right, val);
		}
		else {
			return node;
		}

		// 更新节点信息
		node->height = std::max(height(node->left), height(node->right)) + 1;

		// 插入之后不平衡，调整
		if (std::abs(height(node->left) - height(node->right)) > 1) {
			return adjust(node);
		}
		return node;
	}

	Node* remove(Node* node, const T& val) {
		if (!node) {
			return nullptr;
		}
		
		if (node->val < val) {
			node->right = remove(node->right, val);
		}
		else if (node->val > val) {
			node->left = remove(node->left, val);
		}
		else {
			//找前驱结点
			if (node->left && node->right) {
				Node* cur = node->left;
				Node* parent = node;
				while (cur) {
					parent = cur;
					cur = cur->right;
				}
				node->val = parent->val;
				node->left = remove(node->left, node->val);
			}
			else
			{
				// 只有一个孩子节点，直接替换
				if (node->left) {
					Node* tmp = node->left;
					delete node;
					return tmp;
				}
				else {
					Node* tmp = node->right;
					delete node;
					return tmp;
				}
			}
		}

		// 更新节点信息
		node->height = std::max(height(node->left), height(node->right)) + 1;
		if (std::abs(height(node->left) - height(node->right)) > 1) {
			return adjust(node);
		}
		return node;
	}

	// 左旋
	Node* left_rotate(Node* node)
	{
		Node* child = node->right;
		node->right = child->left;
		child->left = node;
		node->height = std::max(height(node->left), height(node->right)) + 1;
		child->height = std::max(height(child->left), height(child->right)) + 1;
		return child;
	}

	// 右旋
	Node* right_rotate(Node* node)
	{
		Node* child = node->left;
		node->left = child->right;
		child->right = node;
		node->height = std::max(height(node->left), height(node->right)) + 1;
		child->height = std::max(height(child->left), height(child->right)) + 1;
		return child;
	}

	// 右平衡
	Node* right_balance(Node* node)
	{
		node->right = right_rotate(node->right);
		return left_rotate(node);
	}

	// 左平衡
	Node* left_balance(Node* node)
	{
		node->left = left_rotate(node->left);
		return right_rotate(node);
	}

	// 不平衡的调整函数
	Node* adjust(Node* node) {
		if (height(node->left) > height(node->right)) {
			Node* child = node->left;
			if (height(child->left) >= height(child->right)) {
				return right_rotate(node);
			}
			else {
				return left_balance(node);
			}
		}
		else{
			Node* child = node->right;
			if (height(child->right) >= height(child->left)) {
				return left_rotate(node);
			}
			else {
				return right_balance(node);
			}
		}
	}

	// 返回高度
	int height(Node* node) const {
		if (!node) {
			return 0;
		}
		return node->height;
	}
	
};
