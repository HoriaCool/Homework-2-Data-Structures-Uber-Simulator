// Copyright 2019 Nedelcu Horia (nedelcu.horia.alexandru@gmail.com)

#ifndef SORTED_LIST_H_
#define SORTED_LIST_H_

#include <list>

template <typename T>
class SortedList {
 private:
    std::list<T> list_;
    bool (*compare_)(const T&, const T&);

 public:
    // Constructor
    explicit SortedList(bool (*c)(const T&, const T&));

    // Destructor
    ~SortedList();

    // Insert an element in list
    void insertInOrder(const T&);

    // Remove an element from list
    void remove(const T&);

    // Return number of elements from list
    int getSize();

    // Return a copy list with elements in order
    std::list<T> getList();
};

template <typename T>
SortedList<T>::SortedList(bool (*c)(const T&, const T&)):
    list_(), compare_(c) {}

template <typename T>
SortedList<T>::~SortedList() {}

template <typename T>
void SortedList<T>::insertInOrder(const T& element) {
    auto begin = list_.begin();
    auto end = list_.end();

    while (begin != end && compare_(element, *begin)) {
        ++begin;
    }

    list_.insert(begin, element);
}

template <typename T>
void SortedList<T>::remove(const T& element) {
    auto begin = list_.begin();
    auto end = list_.end();

    for (auto it = begin;  it !=  end; ++it) {
        if (*it == element) {
            list_.erase(it);
            break;
        }
    }
}

template <typename T>
int SortedList<T>::getSize() {
    return list_.size();
}

template <typename T>
std::list<T> SortedList<T>::getList() {
    return list_;
}

#endif  // SORTED_LIST_H_
