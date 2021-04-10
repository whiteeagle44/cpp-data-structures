#include <iostream>

using namespace std;

void Warning(string s);
void Info(string s);

// Doubly linked ring with a sentinel node
template <typename Key, typename Info>
class Ring {
 private:
  class Node {
   private:
    Key key_;
    Info info_;
    Node* next_;
    Node* prev_;

   public:
    Node(Key key, Info info, Node* next, Node* prev)
        : key_(key), info_(info), next_(next), prev_(prev) {}
    Node(Node* next, Node* prev) : next_(next), prev_(prev) {}
    Info get_info() const { return this->info_; }
    Key get_key() const { return this->key_; }
    void set_key(const Key& key);
    void set_info(const Info& info);
    Node* get_next() { return this->next_; }
    void set_next(Node* next_) { this->next_ = next_; }
    Node* get_prev() { return this->prev_; }
    void set_prev(Node* prev_) { this->prev_ = prev_; }
  };
  Node* head_;
  int length_ = 0;

 public:
  template <typename K, typename I>
  class iterator {
   private:
    friend class Ring<Key, Info>;
    Node* ptr_;

   public:
    iterator() : ptr_(nullptr){};
    iterator(Node* ptr) : ptr_(ptr){};
    iterator& operator++()  // pre incrementation
    {
      ptr_ = ptr_->get_next();
      return *this;
    }

    iterator operator++(int)  // post incrementation
    {
      iterator before = *this;
      ptr_ = ptr_->get_next();
      return before;
    }

    iterator& operator--()  // pre decrementation
    {
      ptr_ = ptr_->get_prev();
      return *this;
    }

    iterator operator--(int)  // post decrementation
    {
      iterator before = *this;
      ptr_ = ptr_->get_prev();
      return before;
    }

    bool operator==(const iterator& it) { return ptr_ == it.ptr_; }
    bool operator!=(const iterator& it) { return ptr_ != it.ptr_; }

    K operator*() { return ptr_->get_key(); }
    I get_info() { return ptr_->get_info(); }
  };
  typedef iterator<Key, Info> Iterator;
  typedef iterator<const Key, const Info> Const_Iterator;

  // element after sentinel node is first
  Iterator begin() { return Iterator(head_->get_next()); }
  // sentinel is last
  Iterator end() { return Iterator(head_); }

  Const_Iterator const_begin() const {
    return Const_Iterator(head_->get_next());
  }
  Const_Iterator const_end() const { return Const_Iterator(head_); }

  Ring();
  void InsertAtEnd(const Key& k, const Info& i);
  void InsertAfter(Node* new_node, Node* old_node);
  void Print() const;
  void PrintReverse() const;
};

int main() {
  Ring<int, int> s1;
  s1.InsertAtEnd(1, 10);
  s1.InsertAtEnd(2, 20);
  s1.InsertAtEnd(3, 30);
  s1.InsertAtEnd(4, 40);
  s1.InsertAtEnd(5, 50);
  s1.InsertAtEnd(6, 60);
  s1.InsertAtEnd(7, 70);
  s1.InsertAtEnd(8, 80);
  s1.InsertAtEnd(9, 90);
  s1.InsertAtEnd(10, 100);
  Info("Printing s1 in order:");
  s1.Print();
  Info("Printing s1 in reverse:");
  s1.PrintReverse();

  Ring<int, string> s2;
  s2.InsertAtEnd(777, "3xSeven");
  s2.InsertAtEnd(888, "3xEight");
  s2.InsertAtEnd(999, "3xNine");
  Info("Printing s2 in order:");
  s2.Print();
  Info("Printing s2 in reverse:");
  s2.PrintReverse();

  Ring<string, string> s3;
  Info("Attempting to print empty ring");
  s3.Print();
}

template <typename Key, typename Info>
Ring<Key, Info>::Ring() {
  // creating sentinel node
  head_ = new Node(nullptr, nullptr);
  head_->set_next(head_);
  head_->set_prev(head_);
}

template <typename Key, typename Info>
void Ring<Key, Info>::InsertAtEnd(const Key& k, const Info& i) {
  InsertAfter(new Node(k, i, head_, head_->get_prev()), head_->get_prev());
}

template <typename Key, typename Info>
void Ring<Key, Info>::InsertAfter(Node* new_node, Node* old_node) {
  new_node->set_next(old_node->get_next());
  new_node->set_prev(old_node);
  old_node->get_next()->set_prev(new_node);
  old_node->set_next(new_node);
  length_++;
}

template <typename Key, typename Info>
void Ring<Key, Info>::Print() const {
  if (!this->length_) Warning("Ring is empty");

  Const_Iterator it;
  for (it = this->const_begin(); it != this->const_end(); it++) {
    cout << "key: " << *it << ", info:" << it.get_info() << endl;
  };
}

template <typename Key, typename Info>
void Ring<Key, Info>::PrintReverse() const {
  if (!this->length_) Warning("Ring is empty");

  Const_Iterator it;
  for (it = --this->const_end(); it != this->const_end(); it--) {
    cout << "key: " << *it << ", info:" << it.get_info() << endl;
  };
}

void Warning(string s) {
  cout << "\033[94m"
       << "Warning: "
       << "\033[0m" << s << endl;
}

void Info(string s) {
  cout << "\033[92m"
       << "Info: "
       << "\033[0m" << s << endl;
}
