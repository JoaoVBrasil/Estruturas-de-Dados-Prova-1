#include <iostream>

#include "../include/doubly_linked_list.hpp"

template <class T>
DoublyLinkedList<T>::Node::Node(const T& value)
    : value{value}, next{nullptr}, prev{nullptr} {}

template <class T>
DoublyLinkedList<T>::Node::~Node() {
  if (next != nullptr) {
    delete next;
  }
}

template <class T>
DoublyLinkedList<T>::DoublyLinkedList()
    : head{nullptr}, tail{nullptr}, _size(0) {}

template <class T>
DoublyLinkedList<T>::~DoublyLinkedList() {
  if (head != nullptr) {
    delete head;
  }
}

template <class T>
size_t DoublyLinkedList<T>::size() const {
  return _size;
}

template <class T>
bool DoublyLinkedList<T>::empty() const {
  return size() == 0;
}

template <class T>
void DoublyLinkedList<T>::push_front(const T& value) {
  auto new_node = new Node(value);

  if (empty()) {
    tail = new_node;
  } else {
    new_node->next = head;
    head->prev = new_node;
  }

  head = new_node;

  _size++;
}

template <class T>
void DoublyLinkedList<T>::print() const {
  for (auto& v : *this) {
    std::cout << v << " <-> ";
  }

  std::cout << "NULL (" << size() << ")\n";
}

template <class T>
void DoublyLinkedList<T>::push_back(const T& value) {
  auto new_node = new Node(value);

  if (empty()) {
    head = new_node;
  } else {
    new_node->prev = tail;
    tail->next = new_node;
  }

  tail = new_node;

  _size++;
}

// O(1)  constante
template <class T>
void DoublyLinkedList<T>::pop_front() {
    if (empty()) {  
        throw std::out_of_range("Lista vazia");  
    }

    Node* old_head = head;
    head = head->next;  

    if (head != nullptr) {  
        head->prev = nullptr;  
    } else {  
        tail = nullptr;  
    }

    delete old_head;  
    _size--;  
  
}

// contante O( 1 )
template <class T>
void DoublyLinkedList<T>::pop_back() {
  if (empty()) {
    throw std::out_of_range("lista está vazia");
  } else if (size() == 1) {
    delete tail;
    head = nullptr;
    tail = nullptr;
  } else {
    auto temp = tail;
    tail = tail->prev;
    tail->next = nullptr;

    delete temp;
  }

  _size--;
}

template <class T>
template <class U>
DoublyLinkedList<T>::Iterator<U>::Iterator(U* ptr, bool end)
    : node{ptr}, end{end} {}

template <class T>
template <class U>
auto& DoublyLinkedList<T>::Iterator<U>::operator*() const {
  return node->value;
}

template <class T>
template <class U>
DoublyLinkedList<T>::Iterator<U>&
DoublyLinkedList<T>::Iterator<U>::operator++() {
  if (node->next == nullptr) {
    end = true;
  } else {
    node = node->next;
  }

  return *this;
}

template <class T>
template <class U>
DoublyLinkedList<T>::Iterator<U>&
DoublyLinkedList<T>::Iterator<U>::operator--() {
  if (end) {
    end = false;
  } else {
    node = node->prev;
  }

  return *this;
}

template <class T>
template <class U>
bool DoublyLinkedList<T>::Iterator<U>::operator==(
    const Iterator<U>& other) const {
  return node == other.node and end == other.end;
}

template <class T>
template <class U>
bool DoublyLinkedList<T>::Iterator<U>::operator!=(
    const Iterator<U>& other) const {
  return not(*this == other);
}

template <class T>
auto DoublyLinkedList<T>::begin() const -> const_iterator {
  return const_iterator(head, empty());
}

template <class T>
auto DoublyLinkedList<T>::begin() -> iterator {
  return iterator(head, empty());
}

template <class T>
auto DoublyLinkedList<T>::end() const -> const_iterator {
  return const_iterator(tail, true);
}

template <class T>
auto DoublyLinkedList<T>::end() -> iterator {
  return iterator(tail, true);
}

template <class T>
template <class U>
DoublyLinkedList<T>::Iterator<U> DoublyLinkedList<T>::Iterator<U>::operator+(
    size_t offset) const {
  auto it = *this;

  for (int i = 0; i < offset; i++) {
    ++it;
  }

  return it;
}

template <class T>
template <class U>
auto DoublyLinkedList<T>::Iterator<U>::operator-(size_t offset) const
    -> Iterator<U> {
  auto it = *this;

  for (int i = 0; i < offset; i++) {
    --it;
  }

  return it;
}

template <class T>
void DoublyLinkedList<T>::insert(iterator pos, const T& value) {
  if (pos == begin()) {
    return push_front(value);
  } else if (pos == end()) {
    return push_back(value);
  }

  auto node_pos = pos.node;
  auto node_prev = (--pos).node;

  auto new_node = new Node(value);

  node_prev->next = new_node;
  new_node->next = node_pos;

  new_node->prev = node_prev;
  node_pos->prev = new_node;

  _size++;
}

template <class T>
void DoublyLinkedList<T>::erase(iterator first,iterator last) {
    if (first == last) return;

    Node* start = first.node;
    Node* end = last.node;

    if (!start) return; 

    Node* before = start->prev;
    Node* current = start;

    while (current != end) {
        Node* temp = current;
        current = current->next;
        delete temp;
        _size--;
    }

    if (before) {
        before->next = end;
    } else {
        head = end;
    }

    if (end) {
        end->prev = before;
    } else {
        tail = before;
    }
}

template <class T>
template <class U>
size_t DoublyLinkedList<T>::Iterator<U>::operator-(
    const Iterator<U> other) const {
  auto pos = other;
  size_t count = 0;
  while (pos != *this) {
    count++;
    ++pos;
  }

  return count;
}

template <class T>
auto DoublyLinkedList<T>::find(const T& item) -> iterator {
  auto it = begin();
  while (it != end()) {
    if (*it == item) break;
    ++it;
  }

  return it;
}

template <class T>
auto DoublyLinkedList<T>::find(const T& item) const -> const_iterator {
  auto it = begin();
  while (it != end()) {
    if (*it == item) break;
    ++it;
  }

  return it;
}

template <class T>
bool DoublyLinkedList<T>::contains(const T& item) const {
  return find(item) != end();
}

template <class T>
T& DoublyLinkedList<T>::operator[](size_t index) {
  if (index >= size()) {
    throw std::out_of_range("índice invalido");
  }
  return *(begin() + index);
}

template <class T>
const T& DoublyLinkedList<T>::operator[](size_t index) const {
  if (index >= size()) {
    throw std::out_of_range("índice invalido");
  }
  return *(begin() + index);
}

template <class T>
void DoublyLinkedList<T>::clear() {
  erase(begin(), end());
}

template <class T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& list)
    : head{nullptr}, tail{nullptr}, _size(0) {
  for (auto& i : list) {
    push_back(i);
  }
}

template <class T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(
    const DoublyLinkedList<T>& list) {
  clear();
  for (auto& i : list) {
    push_back(i);
  }
}
