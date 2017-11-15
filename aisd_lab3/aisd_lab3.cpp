#include <iostream>
#include <string>
#include <iostream>
#include <time.h>

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
	/*Node(int k, T d, Node* l, Node* r) {
		key = k;
		data = d;
		left = l;
		right = r;
	}*/
};
template <typename T>
using callback = void(*) (Node<T>*);

template <typename T>
struct ret {
	Node<T> *curr, *next;
	bool side;
};
template <typename T>
struct ret2 {
	Node<T> *parent;
	bool side;
};
template <typename T>
struct Tret {
	bool ok;
	T data;
};
template <typename T>
class Tree {
public:
	Node<T> * root;
	int size;
	Tree() {
		root = new Node<T>;
		size = 0;
	}
	~Tree() {
		clear();
	}
	bool add(int key, T data) {
		Node<T> * n = new Node<T>(key,data);
		if (!size) {
			root = n;
		}
		else {
			bool useRight = false;
			Node<T> *next = root;
			Node<T> * curr = nullptr;
			while (next != nullptr)
			{
				curr = next;
				if (key > curr->key) {
					next = curr->right;
					useRight = true;
				}
				if (key < curr->key)
				{
					next = curr->left;
					useRight = false;
				}
				if (key == curr->key) return false;

			}
			if (useRight) curr->right = n;
			else curr->left = n;


		}
		++size;
		return true;
	}
	Node<T> * find(int key) {
		Node<T> * curr = root;
		if (curr == nullptr) return 0;
		while (key != curr->key) {
			if (key < curr->key) {
				if (curr->left == nullptr) return nullptr;
				curr = curr->left;
			}
			if (key > curr->key) {
				if (curr->right == nullptr) return nullptr;
				curr = curr->right;
			}
		}
		return curr;
	}
	ret<T> findPrev(int key) {
		Node<T> * curr = 0;
		Node<T> * next = root;
		bool side = false;
		if (next == nullptr) return{ 0,0,0 };
		while (key != next->key) {
			curr = next;
			if (key < next->key) {
				if (next->left == nullptr) return{ 0,0,0 };
				next = next->left;
				side = 0;
			}
			if (key > curr->key) {
				if (next->right == nullptr) return{ 0,0,0 };
				next = next->right;
				side = 1;
			}
		}
		return{ curr, next, side };
	}
	ret2<T> find2(int key) {
		Node<T> * curr = 0;
		Node<T> * next = root;
		bool side = false;
		if (next == nullptr) return{ 0,0 };
		while (key != next->key) {
			curr = next;
			if (key < next->key) {
				if (next->left == nullptr) return{ 0,0 };
				next = next->left;
				side = 0;
			}
			if (key > curr->key) {
				if (next->right == nullptr) return{ 0,0 };
				next = next->right;
				side = 1;
			}
		}
		return{ curr, side };
	}
	ret<T> findSmallest(Node<T> * prev, Node<T> * curr)
	{
		bool side = 1;
		while (curr->left != nullptr) {
			prev = curr;
			curr = curr->left;
			side = 0;
		}
		return{ prev, curr, side };
	}
	ret<T> findLargest(Node<T> * prev, Node<T> * curr) {
		bool side = 0;
		while (curr->right != nullptr) {
			prev = curr;
			curr = curr->right;
			side = 1;
		}
		return{ prev, curr, side };
	}
	
	bool del(int key) {
		ret<T> toDelete = findPrev(key);
		if ((toDelete.curr == 0) && toDelete.next != root) return false;
		if (toDelete.next == nullptr) return false;
		if (toDelete.next->left == nullptr && toDelete.next->right == nullptr) {
			if (toDelete.next == root) {
				root = nullptr;
				return true;
			}
			if (!toDelete.side) 
				toDelete.curr->left = nullptr;
			else 
				toDelete.curr->right = nullptr;
		}
		else {
			ret<T> n;

			if (!toDelete.side) { //delete left
				if (toDelete.next->left != nullptr) {
					n = findLargest(toDelete.next, toDelete.next->left);
					n.curr->right = nullptr;
				}
				else {
					n = findSmallest(toDelete.next, toDelete.next->right);
					n.curr->left = nullptr;
				}
				if (toDelete.next == root) {
					std::swap(root->key, n.next->key);
					std::swap(root->data, n.next->data);
					return del(key);
				}
					
				else 
					toDelete.curr->left = n.next;
			}
			else {//delete right

				if (toDelete.next->right != nullptr) {
					n = findSmallest(toDelete.next, toDelete.next->right);
				}
				else {
					n = findLargest(toDelete.next, toDelete.next->left);
				}
				if (!n.side) 
					n.curr->left = nullptr;
				else 
					n.curr->right = nullptr;
				toDelete.curr->right = n.next;
			}
			n.next->left = toDelete.next->left;
			n.next->right = toDelete.next->right;
		}
		--size;
		return true;
	}
	bool del2(int key) {
		Node<T> * parent = nullptr, * del = root;
		while (key != del->key) {
			parent = del;
			if (key > del->key) del = del->right;
			else del = del->left;
		}
		// easiest
		if (!del->left && !del->right) {
			if (del == parent->left) parent->left = nullptr;
			else parent->right = nullptr;
			return true;
		}

		Node<T> * replace  = nullptr;
		// as hard as fuck
		if (del->left && del->right) {
			Node<T> * replaceParent;
			replaceParent = del; //2
			replace = del->left; //0
			while (replace->right) {
				replaceParent = replace;
				replace = replace->right;
			}
			if (replaceParent != del)
				replaceParent->right = nullptr;
			else
				replaceParent->left = replace->left;
			del->key = replace->key;
			del->data = replace->data;
			return true;
		}
		// with one branch	
		if (!del->left && del->right) {
			replace = del->right;
		}
		if (del->left && !del->right) {
			replace = del->left;
		}
		del->key = replace->key;
		del->data = replace->data;
		del->left = replace->left;
		del->right = replace->right;
		return true;
	}
	Tret<T> get(int key) {
		Node<T>* t = find(key);
		if (t == nullptr) return{ false, "" };
		return{ true, t->data };
	}
	bool isKeyInTree(int key) {
		Node<T>* t = find(key);
		if (!t) return false;
		else return true;
	}
	bool isDataInSubTree(Node<T>* el, T data) {
		if (el == nullptr) return false;
		if (el->data == data) return true;
		if ((el->left != nullptr) && (isDataInSubTree(el->left, data))) return true;
		if ((el->right != nullptr) && (isDataInSubTree(el->right, data))) return true;
		return false;
	}
	bool isDataInTree(T data) {
		return isDataInSubTree(root, data);
	}
	void clearNode(Node<T>*el) {
		if (el->left != nullptr) clearNode(el->left);
		if (el->right != nullptr) clearNode(el->right);
		delete el;
	}
	void clear() {
		if (size > 0)
			clearNode(root);
		size = 0;
	}
	void goSub(Node<T>*el, callback<T> f) {
		if (el->left != nullptr) goSub(el->left, f);
		if (el->right != nullptr) goSub(el->right, f);
		f(el);
		//std::cout << el->data << " ";
	}
	void go(callback<T> f) {
		goSub(root, f);
	}
};
template <typename T>
void print(Node<T>*el) {
	if (el->left != nullptr && el->right != nullptr) {
		std::cout << el->key << ": left " << el->left->key << " right " << el->right->key << " \n";
		return;
	}
		
	if (el->left != nullptr) {
		std::cout << el->key << ": left " << el->left->key << " \n";
		return;
	}
	if (el->right != nullptr) {
		std::cout << el->key << ": right " << el->right->key << " \n";
		return;
	}
	std::cout << el->key << ": ---- "<< "\n";
}

std::string generateString(size_t size) {
	char * st = new char[size+1];
	size_t i;
	for (i = 0; i < size; i++)
	{
		st[i] = (char)(rand() % 25 + 97);
	}
	st[i] = 0;
	return std::string(st);
}

int main()
{
	Tree<std::string> t;
	//srand(time(NULL));
	//for (size_t i = 0; i < 10; i++)
	//{
	//	int q = rand() % 100;
	//	t.add(q, generateString(20)); //1
	//}
	t.add(54,"54");
	t.add(22, "33");
	t.add(10, "a");
	t.add(2, "b");
	t.add(6, "qw");
	t.add(8, "qwe");
	t.add(4, "qwer");
	t.add(0, "qwert");
	t.add(99, "qwerty");
	t.add(89, "q");

	std::cout << "size: " << t.size << "\n"; //3

	t.go(print<std::string>); //8
	int d;
	std::cout << "\nindex to delete: ";
	std::cin >> d;
	while (!t.del2(d)) { //2
		std::cout << "wrong index";
		std::cout << "\nindex to delete: ";
		std::cin >> d;
	}
	std::cout << "\n";
	t.go(print<std::string>); //8
	std::cout << "\nindex to delete: ";
	std::cin >> d;
	while (!t.del2(d)) { //2
		std::cout << "wrong index";
		std::cout << "\nindex to delete: ";
		std::cin >> d;
	}
	std::cout << "\n";
	t.go(print<std::string>); //8
	std::cout << "\nindex to delete: ";
	std::cin >> d;
	while (!t.del2(d)) { //2
		std::cout << "wrong index";
		std::cout << "\nindex to delete: ";
		std::cin >> d;
	}
	std::cout << "\n";
	t.go(print<std::string>); //8
	std::cout << "size: " << t.size << "\n"; //3
	t.go(print<std::string>); //8

	std::cout << "\nindex to show: ";
	std::cin >> d;
	Tret<std::string> s;
	s = t.get(d);
	while (!s.ok) { //5
		std::cout << "wrong index";
		std::cout << "\nindex to show: ";
		std::cin >> d;
		s = t.get(d);
	}
	std::cout << s.data;

	std::cout << "\nindex to find: ";
	std::cin >> d;
	std::cout << "is in tree: " << t.isKeyInTree(d) << "\n"; //6

	std::cout << "\nstring to find: ";
	std::string st;
	std::cin >> st;
	std::cout << "is in tree: " << t.isDataInTree(st) << "\n"; //7

	t.clear(); //4	
	system("pause");
	return 0;
}

