// #include <iostream>
// #include <stdexcept>

// template<typename T, size_t N>
// class Vector {
// private:
//     T array[N];
//     size_t size;

// public:
//     Vector() : size(0) {}

//     size_t getSize() const {
//         return size;
//     }

//     size_t getMaxSize() const {
//         return N;
//     }

//     bool empty() const {
//         return size == 0;
//     }

//     void push_back(const T& value) {
//         if (size >= N) {
//             throw std::out_of_range("Vector is full. Cannot push_back.");
//         }
//         array[size++] = value;
//     }

//     void pop_back() {
//         if (empty()) {
//             throw std::out_of_range("Vector is empty. Cannot pop_back.");
//         }
//         --size;
//     }

//     T& operator[](size_t index) {
//         if (index >= size) {
//             throw std::out_of_range("Index out of range.");
//         }
//         return array[index];
//     }

// // list=[1,2,3,4,5]
// // list[i]

// //vector my_list[i]
// //vector a== vector b

//     T& at(size_t index) {
//         if (index >= size) {
//             throw std::out_of_range("Index out of range.");
//         }
//         return array[index];
//     }


// };

// int main() {
//     Vector<int, 5> vec;
//     vec.push_back(1);
//     vec.push_back(2);
//     vec.push_back(3);

//     std::cout << "Vector size: " << vec.getSize() << std::endl;
//     std::cout << "Vector capacity: " << vec.getMaxSize() << std::endl;

//     std::cout << "Elements in vector:";
//     for (size_t i = 0; i < vec.getSize(); ++i) {
//         std::cout << " " << vec[i];
//     }
//     std::cout << std::endl;

//     return 0;
// }

// ----

// #include <iostream>
// // #include <stdexcept>

// class ArrayVector {
// private:
//     int* A;
//     int size; // current number of elements
//     int capacity;

//     void reserve(int newCapacity) {
//         int* newArray = new int[newCapacity];
//         for (int i = 0; i < size; ++i) {
//             newArray[i] = A[i];
//         }
//         capacity = newCapacity;
//         delete[] A;
//         A = newArray;
//     }

// public:
//     ArrayVector() : A(nullptr), size(0), capacity(0) {}

//     ~ArrayVector() {
//         delete[] A;
//     }

//     void insert(int i, const int& e) {
//         // Do we need more space? If so, double array size
//         if (size >= capacity) {
//             reserve(std::max(1, 2 * capacity));
//         }

//         // Shift elements up
//         for (int j = size - 1; j >= i; j--) {
//             A[j + 1] = A[j];
//         }

//         A[i] = e; // Put in empty slot
//         size++; // One more element
//     }

//     void print() {
//         for (int i = 0; i < size; ++i) {
//             std::cout << A[i] << " ";
//         }
//         std::cout << std::endl;
//     }
// };

// int main() {
//     ArrayVector vec;
//     vec.insert(0, 1);
//     vec.insert(1, 2);
//     vec.insert(0, 0);

//     vec.print(); // Output: 0 1 2

//     return 0;
// }

// ----

#include <iostream>

// Define the element type of the list
using Elem = int;

// Forward declaration of Node class
class Node;

// Iterator class
class Iterator {
public:
    Elem& operator*(); // reference to the element
    bool operator==(const Iterator& p) const; // compare positions
    bool operator!=(const Iterator& p) const;
    Iterator& operator++(); // move to next position
    Iterator& operator--(); // move to previous position
    Iterator(Node* u); // constructor create from node


    Node* v; // pointer to the node
};

// Node class
class Node {
public:
    Elem elem; // element stored in the node
    Node* prev; // pointer to the previous node
    Node* next; // pointer to the next node
    Node(const Elem& e, Node* p = nullptr, Node* n = nullptr) : elem(e), prev(p), next(n) {}
};

// NodeList class
class NodeList {
public:
    NodeList(); // default constructor
    Iterator begin() const; // beginning position
    Iterator end() const; // (just beyond) last position
    void insertFront(const Elem& e); // insert at front
    void insertBack(const Elem& e); // insert at rear
    void insert(const Iterator& p, const Elem& e); // insert e before p
    void eraseFront(); // remove first
    void eraseBack(); // remove last
    void erase(const Iterator& p); // remove p
    void erase(const Elem& e); // remove all occurrences of e

private:
    int n; // number of items
    Node* header; // head-of-list sentinel
    Node* trailer; // tail-of-list sentinel
};

// Iterator methods implementation
Iterator::Iterator(Node* u) : v(u) {}

Elem& Iterator::operator*() {
    return v->elem;
}

bool Iterator::operator==(const Iterator& p) const {
    return v == p.v;
}

bool Iterator::operator!=(const Iterator& p) const {
    return v != p.v;
}

Iterator& Iterator::operator++() {
    v = v->next;
    return *this;
}

Iterator& Iterator::operator--() {
    v = v->prev;
    return *this;
}

// NodeList methods implementation
NodeList::NodeList() : n(0), header(new Node(Elem())), trailer(new Node(Elem())) {
    header->next = trailer;
    trailer->prev = header;
}

Iterator NodeList::begin() const {
    return Iterator(header->next);
}

Iterator NodeList::end() const {
    return Iterator(trailer);
}

void NodeList::insertFront(const Elem& e) {
    insert(begin(), e);
}

void NodeList::insertBack(const Elem& e) {
    insert(end(), e);
}

void NodeList::insert(const Iterator& p, const Elem& e) {
    Node* w = p.v;
    Node* u = w->prev;
    Node* newNode = new Node(e, u, w);
    u->next = newNode;
    w->prev = newNode;
    n++;
}

void NodeList::eraseFront() {
    erase(begin());
}

void NodeList::eraseBack() {
    erase(--end());
}
void NodeList::erase(const Elem& e) {
    Iterator it = begin();
    while (it != end()) {
        if (*it == e) {
            Iterator to_remove = it;
            ++it; // Advance iterator before erasing to avoid invalidating it
            erase(to_remove); // Remove the element
        } else {
            ++it; // Move to the next element
        }
    }
}
void NodeList::erase(const Iterator& p) {
    if (p == end()) // nothing to erase
        return;
    Node* v = p.v;
    Node* w = v->next;
    Node* u = v->prev;
    u->next = w;
    w->prev = u;
    delete v;
    n--;
}


// Example usage
int main() {
    NodeList list;
    list.insertFront(1);
    list.insertBack(3);
    list.insertBack(4);
    list.insertFront(0);

    for (Iterator it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    list.eraseBack();
    list.eraseFront();

    for (Iterator it = list.begin(); it != list.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
