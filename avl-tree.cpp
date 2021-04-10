/** AVL-Tree implementation
 Key features:
 inserting, deleting, searching, printing the whole tree by level
 Based on work by D. S. Malik and E. Mahendru
**/

#include <iostream>

using namespace std;

template <typename Key, typename Info>
class Dictionary {
public:
    class Node {
    public:
        Node(Key key, Info info);

        Key getKey() const
        {
            return key_;
        }

        void setKey(Key key)
        {
            key_ = key;
        }

        Info getInfo() const
        {
            return info_;
        }

        void setInfo(Info info)
        {
            info_ = info;
        }

        Node* getLeft() const
        {
            return left_;
        }

        void setLeft(Node* left)
        {
            left_ = left;
        }

        Node* getRight() const
        {
            return right_;
        }

        void setRight(Node* right)
        {
            right_ = right;
        }

        int getHeight() const
        {
            return height_;
        }

        void setHeight(int height)
        {
            height_ = height;
        }

    private:
        Key key_;
        Info info_;
        Node* left_;
        Node* right_;
        int height_;
    };

private:
    Node* root_ = nullptr;

    Node* _insert(Node* root, Key key, Info info);

    Node* _remove(Node* root, Key key);

    Node* _rotateRight(Node* root);

    Node* _rotateLeft(Node* root);

    int _getHeight(Node* root_);

    void _printInOrder(Node* root) const;

    void _printPreOrder(Node* root) const;

    void _printPostOrder(Node* root) const;

    void _printLevels(Node* root, int height);

    void _display(Dictionary::Node* root);

public:
    Dictionary();

    ~Dictionary();

    void destroy(Node* root);

    void insert(Key key, Info info);

    void remove(Key key);

    Info find(Key key) const;

    void printInOrder() const;

    void printPreOrder() const;

    void printPostOrder() const;

    void display();
};

int main()
{
    Dictionary<string, int> dictionary;
    dictionary.insert("aa", 1);
    dictionary.insert("dd", 2);
    dictionary.insert("bb", 3);
    dictionary.insert("ee", 4);
    dictionary.insert("cc", 5);
    dictionary.remove("cc");
    dictionary.printInOrder();
    dictionary.printPostOrder();
    dictionary.printPreOrder();
    dictionary.display();

    cout << "Info of key: aa: ";
    cout << dictionary.find("aa") << endl;
    cout << "Info of key: ee: ";
    cout << dictionary.find("ee") << endl;

    Dictionary<int, int> numbers;
    numbers.insert(1, 10);
    numbers.remove(1);
    numbers.insert(2, 10);
    numbers.insert(3, 10);
    numbers.insert(4, 10);
    numbers.insert(5, 10);
    numbers.insert(6, 10);
    numbers.insert(7, 10);
    numbers.insert(8, 10);
    numbers.insert(9, 10);
    numbers.printInOrder();
    numbers.printPostOrder();
    numbers.printPreOrder();
    numbers.display();

    cout << "Info of key 6: ";
    cout << numbers.find(6) << endl;
    cout << "Info of non-existing key: ";
    cout << numbers.find(100) << endl;

    Dictionary<string, int> test;
    test.insert("word1", 0);
    test.insert("word1", 0);
    test.insert("word2", 0); // node with the same key is not inserted the second time
    test.printInOrder();
    test.remove("word1");
    test.remove("word2");
    test.printInOrder();
    test.display();
}

template <typename Key, typename Info>
Dictionary<Key, Info>::Dictionary()
{
    root_ = nullptr;
}

template <typename Key, typename Info>
Dictionary<Key, Info>::~Dictionary()
{
    destroy(root_);
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::destroy(Dictionary::Node* root)
{
    if (root) {
        destroy(root->getLeft());
        destroy(root->getRight());
        delete root;
        root = nullptr;
    }
}

template <typename Key, typename Info>
Dictionary<Key, Info>::Node::Node(Key key, Info info)
{
    key_ = key;
    info_ = info;
    height_ = 1;
    left_ = nullptr;
    right_ = nullptr;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::insert(Key key, Info info)
{
    root_ = _insert(root_, key, info); // root_ is overwritten only if it's null, otherwise it stays the same
}

template <typename Key, typename Info>
typename Dictionary<Key, Info>::Node* Dictionary<Key, Info>::_insert(Node* root, Key key, Info info)
{
    if (!root) {
        Node* temp = new Node(key, info);
        return temp;
    }
    if (key < root->getKey())
        root->setLeft(_insert(root->getLeft(), key, info));
    else if (key > root->getKey())
        root->setRight(_insert(root->getRight(), key, info));
    root->setHeight(1 + max(_getHeight(root->getLeft()), _getHeight(root->getRight())));
    int b_factor = _getHeight(root->getLeft()) - _getHeight(root->getRight());
    if (b_factor > 1) {
        if (key < root->getLeft()->getKey()) {
            return _rotateRight(root);
        } else {
            root->setLeft(_rotateLeft(root->getLeft()));
            return _rotateRight(root);
        }
    } else if (b_factor < -1) {
        if (key > root->getRight()->getKey()) {
            return _rotateLeft(root);
        } else {
            root->setRight(_rotateRight(root->getRight()));
            return _rotateLeft(root);
        }
    }
    return root;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::remove(Key key)
{
    root_ = _remove(root_, key);
}

template <typename Key, typename Info>
typename Dictionary<Key, Info>::Node* Dictionary<Key, Info>::_remove(Dictionary::Node* root, Key key)
{
    if (!root)
        return nullptr;
    if (key < root->getKey()) {
        root->setLeft(_remove(root->getLeft(), key));
    } else if (key > root->getKey()) {
        root->setRight(_remove(root->getRight(), key));
    } else {
        Node* r = root->getRight();
        if (!root->getRight()) {
            Node* l = root->getLeft();
            delete (root);
            root = l;
        } else if (!root->getLeft()) {
            delete (root);
            root = r;
        } else {
            while (!r->getLeft())
                r = r->getLeft();
            root->setKey(r->getKey());
            root->setRight(_remove(root->getRight(), r->getKey()));
        }
    }
    if (!root)
        return root;
    root->setHeight(1 + max(_getHeight(root->getLeft()), _getHeight(root->getRight())));
    int b_factor = _getHeight(root->getLeft()) - _getHeight(root->getRight());
    if (b_factor > 1) {
        if (key > root->getLeft()->getKey()) {
            return _rotateRight(root);
        } else {
            root->setLeft(_rotateLeft(root->getLeft()));
            return _rotateRight(root);
        }
    } else if (b_factor < -1) {
        if (key < root->getRight()->getKey()) {
            return _rotateLeft(root);
        } else {
            root->setRight(_rotateRight(root->getRight()));
            return _rotateLeft(root);
        }
    }
    return root;
}

template <typename Key, typename Info>
Info Dictionary<Key, Info>::find(const Key key) const
{
    Node* current;
    bool found = false;
    if (!root_)
        cout << "find(): the tree is empty!" << endl;
    else {
        current = root_;
        while (current && !found) {
            if (current->getKey() == key)
                found = true;
            else if (current->getKey() > key)
                current = current->getLeft();
            else
                current = current->getRight();
        }
    }
    if (found)
        return current->getInfo();
}

template <typename Key, typename Info>
typename Dictionary<Key, Info>::Node* Dictionary<Key, Info>::_rotateRight(Node* root)
{
    Node* new_root = root->getLeft();
    root->setLeft(new_root->getRight());
    new_root->setRight(root);
    root->setHeight(1 + max(_getHeight(root->getLeft()), _getHeight(root->getRight())));
    new_root->setHeight(1 + max(_getHeight(new_root->getLeft()), _getHeight(new_root->getRight())));
    return new_root;
}

template <typename Key, typename Info>
typename Dictionary<Key, Info>::Node* Dictionary<Key, Info>::_rotateLeft(Node* root)
{
    Node* new_root = root->getRight();
    root->setRight(new_root->getLeft());
    new_root->setLeft(root);
    root->setHeight(1 + max(_getHeight(root->getLeft()), _getHeight(root->getRight())));
    new_root->setHeight(1 + max(_getHeight(new_root->getLeft()), _getHeight(new_root->getRight())));
    return new_root;
}

template <typename Key, typename Info>
int Dictionary<Key, Info>::_getHeight(Node* root)
{
    if (!root)
        return 0;
    return root->getHeight();
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::_printInOrder(Dictionary::Node* root) const
{
    if (root) {
        _printInOrder(root->getLeft());
        cout << root->getKey();
        cout << root->getInfo();
        cout << ", ";
        _printInOrder(root->getRight());
    }
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::_printPreOrder(Dictionary::Node* root) const
{
    if (root) {
        cout << root->getKey();
        cout << root->getInfo();
        cout << ", ";
        _printInOrder(root->getLeft());
        _printInOrder(root->getRight());
    }
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::_printPostOrder(Dictionary::Node* root) const
{
    if (root) {
        _printInOrder(root->getLeft());
        _printInOrder(root->getRight());
        cout << root->getKey();
        cout << root->getInfo();
        cout << ", ";
    }
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::printInOrder() const
{
    cout << "Printing in order: ";
    _printInOrder(root_);
    cout << endl;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::printPreOrder() const
{
    cout << "Printing pre order: ";
    _printPreOrder(root_);
    cout << endl;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::printPostOrder() const
{
    cout << "Printing post order: ";
    _printPostOrder(root_);
    cout << endl;
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::display()
{
    _display(root_);
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::_display(Dictionary::Node* root)
{
    int height = _getHeight(root);
    for (int i = 1; i <= height; i++) {
        cout << "(Level: " << i << ") ";
        _printLevels(root, i);
        cout << endl;
    }
}

template <typename Key, typename Info>
void Dictionary<Key, Info>::_printLevels(Dictionary::Node* root, int height)
{
    if (!root)
        return;
    if (height == 1)
        cout << root->getKey() << " ";
    else {
        _printLevels(root->getLeft(), height - 1);
        _printLevels(root->getRight(), height - 1);
    }
}
