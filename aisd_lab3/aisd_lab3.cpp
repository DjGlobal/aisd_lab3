#include <iostream>
#include <string>

template <typename T>
struct Node {
	int key;
	T data;
	Node<T> *left, *right;
	Node() {
		key = 0;
		left = nullptr;
		right = nullptr;
	}
	Node(int k, T d) {
		key = k;
		data = d;
		left = nullptr;
		right = nullptr;
	}
	Node(int k, T d, Node* l, Node* r) {
		key = k;
		data = d;
		left = l;
		right = r;
	}
};

template <typename T>
class Tree {
public:
	Node<T> root;
	int size;
	Tree() {
		Node<T> * root = new Node<T>;
		size = 0;
	}
	/*~Tree() {

	}*/
	void add(int key, T data) {
		Node<T> * n = new Node<T>;
		*n = { key, data };
		if (!size) {
			root = n;
		}
		else {
			bool useRight = false;
			Node<T> *next = root;
			while (next != nullptr)
			{
				curr = next;
				if (key >= curr->key) {
					next = curr->right;
					useRight = false;
				}
					
				else {
					next = curr->left;
					useRight = true;
				}
					
			}
			if (useRight) curr->right = n;
			else curr->left = n;

			
		}
	}
};

int main()
{
	Tree<std::string> t;
	t.add(1, "hui");
    return 0;
}

