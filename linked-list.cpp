#include <cstring>
#include <iostream>
#include <string>

using namespace std;

void Warning(string s);
void Info(string s);

template <typename Key, typename Info>
class Sequence {
 public:
  // The produce function
  Sequence<Key, Info> CombineSequences(Sequence& sequence1, int index1,
                                       const int length1, Sequence& sequence2,
                                       int index2, const int length2,
                                       int max_length);
  // Returns copy of part of sequence elements of index: from index to
  // index+length-1
  Sequence<Key, Info> Trim(int index, const int length) const;
  Sequence operator+(const Sequence& sequence) const;

  Sequence();
  // Adds node at the end of the list
  void AddNode(Key key, Info info);
  void RemoveNode(Key key);
  void Print();

 private:
  // Every node has a unique key
  class Node {
   public:
    Node(Key key, Info info);
    void PrintNode();
    void set_key(Key key);
    void set_info(Info info);
    void set_next(Node* next);
    Key get_key() { return key; };
    Info get_info() { return info; };
    Node* get_next() { return next; }

   private:
    Key key;
    Info info;
    Node* next;
  };

 private:
  int length_;
  Node* head;

 public:
  Node* get_head() { return head; }
};

int main() {
  Sequence<int, string> s1;
  s1.AddNode(0, "Jerzy");
  s1.AddNode(1, "Stefan");
  s1.AddNode(2, "Weronika");
  s1.AddNode(3, "Ania");
  s1.Print();  // prints s1

  Info("Testing Trim()");
  Sequence<int, string> s2;
  s2.Print();  // warning (s2 is empty)

  s2 = s1.Trim(1, 1);
  s2.Print();  // s2 (Stefan)

  s2 = s1.Trim(-1, 2);  // warning (index negative)
  s2.Print();           // s2 (Jerzy, Stefan)

  s2 = s1.Trim(1, 0);  // warning (s2 is empty)
  s2.Print();          // warning (s2 is empty)

  s2 = s1.Trim(0, -1);  // warning (length not positive)
  s2.Print();           // warning (s2 is empty)

  s2 = s1.Trim(2, 4);
  s2.Print();  // s2 (Weronika, Ania)

  Sequence<int, string> s3;
  s3.AddNode(4, "Jadwiga");
  s3.AddNode(5, "Krzysztof");

  Info("Testing operator+");
  s2 = s1 + s3;
  s2.Print();  // s2 (Jerzy, Stefan, Weronika, Ania, Jadwiga, Krzysztof)

  s2 = s1 + s2;  // warning (duplicates, each node must have a unique key)
  s2.Print();    // s2 (Jerzy, Stefan, Weronika, Ania, Jadwiga, Krzysztof)

  Info("Testing CombineSequences()");
  s2 = s2.CombineSequences(s1, 0, 1, s3, 0, 1, 2);
  s2.Print();  // s2 (Jerzy, Jadwiga)

  s2 = s2.CombineSequences(s1, 0, 2, s3, 0, 2, 3);
  s2.Print();  // s2 (Jerzy, Stefan, Jadwiga)

  s2 = s2.CombineSequences(s1, 0, 2, s3, 0, 2, -2);
  s2.Print();  // warning (s2 is empty)

  // thorough testing of Trim() ensures that CombineSequences() which makes use
  // of it works as expected

  return 0;
}

template <typename Key, typename Info>
Sequence<Key, Info> Sequence<Key, Info>::CombineSequences(
    Sequence& sequence1, int index1, const int length1, Sequence& sequence2,
    int index2, const int length2, int max_length) {
  Sequence<Key, Info> combined_sequence =
      sequence1.Trim(index1, length1) + sequence2.Trim(index2, length2);
  combined_sequence = combined_sequence.Trim(0, max_length);
  return combined_sequence;
}

template <typename Key, typename Info>
Sequence<Key, Info> Sequence<Key, Info>::Trim(int index,
                                              const int length) const {
  if (index < 0) {
    Warning("Trim(): Index must be a nonnegative number, assuming 0.");
    index = 0;
  }

  Sequence<Key, Info> trimmed_sequence;

  if (length <= 0) {
    Warning(
        "Trim(): Length must be a positive number, returning an empty list");
    return trimmed_sequence;
  }

  if (index > length_) {
    Warning("Sequence::Trim() Index out of bounds, returning an empty list");
    return trimmed_sequence;
  }

  Node* curr = head;
  int current_index = 0;
  while (current_index < index) {
    curr = curr->get_next();
    current_index++;
  }

  Node* creator = nullptr;
  Node* prev = nullptr;

  while (curr && current_index < index + length && current_index <= length_) {
    current_index++;
    creator = new Node(curr->get_key(), curr->get_info());
    if (!trimmed_sequence.head) {
      trimmed_sequence.head = creator;
    } else {
      prev->set_next(creator);
    }
    prev = creator;
    curr = curr->get_next();
  }

  if (!trimmed_sequence.head) Warning("Sequence::Trim() Trimmed list is empty");

  return trimmed_sequence;
}

template <typename Key, typename Info>
Sequence<Key, Info> Sequence<Key, Info>::operator+(
    const Sequence& sequence) const {
  if (this == &sequence) return *this;

  Sequence cList = *this;

  Node* curr = sequence.head;
  Node* prev = 0;

  if (curr != NULL) {
    bool reached_the_end = false;

    while (!reached_the_end) {
      cList.AddNode(curr->get_key(), curr->get_info());

      if (curr->get_next() == NULL) {
        reached_the_end = true;
      } else {
        prev = curr;
        curr = curr->get_next();
      }
    }
  }

  return cList;
}

template <typename Key, typename Info>
Sequence<Key, Info>::Sequence() {
  head = NULL;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::AddNode(Key key, Info info) {
  Node* curr = head;
  Node* prev = 0;

  // if adding to an empty list
  if (!curr) {
    curr = new Node(key, info);
    prev = curr;
    head = curr;
  } else {
    // checking if node with given key exists, if not adding a new node at the
    // end
    bool reached_the_end = false;

    while (!reached_the_end) {
      if (curr->get_key() == key) {
        Warning(
            "Sequence::AddNode(): Node not added. Node with the given data "
            "already exists.");
        return;
      }

      if (curr->get_next() == NULL) {
        curr->set_next(new Node(key, info));
        length_++;
        reached_the_end = true;
      } else {
        prev = curr;
        curr = curr->get_next();
      }
    }
  }
}

template <typename Key, typename Info>
void Sequence<Key, Info>::RemoveNode(Key key) {
  if (head == NULL) {
    Warning(
        "Sequence::RemoveNode() Node not removed. Cannot remove a node from an "
        "empty list.");
    return;
  }

  Node* curr = head;
  Node* prev = 0;

  bool reached_the_end = false;

  while (!reached_the_end) {
    if (curr->get_key() == key) {
      if (!prev)
        head = curr->get_next();
      else
        prev->set_next(curr->get_next());
      return;
    }
  }
}

template <typename Key, typename Info>
void Sequence<Key, Info>::Print() {
  if (head == NULL) {
    Warning(
        "Sequence::Print() Sequence not printed. Cannot print an empty list.");
    return;
  }

  Node* curr = head;

  bool reached_the_end = false;

  cout << "{";

  while (!reached_the_end) {
    curr->PrintNode();

    if (curr->get_next() == NULL) {
      reached_the_end = true;
    } else {
      curr = curr->get_next();
    }
    if (!reached_the_end) cout << ", ";
  }
  cout << "}" << endl;
}
template <typename Key, typename Info>
void Sequence<Key, Info>::Node::PrintNode() {
  cout << this->key << ": " << this->info;
}

template <typename Key, typename Info>
Sequence<Key, Info>::Node::Node(Key key, Info info) {
  this->key = key;
  this->info = info;
  this->next = NULL;
}
template <typename Key, typename Info>
void Sequence<Key, Info>::Node::set_info(Info info) {
  this->info = info;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::Node::set_key(Key key) {
  this->key = key;
}

template <typename Key, typename Info>
void Sequence<Key, Info>::Node::set_next(Node* next) {
  this->next = next;
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
