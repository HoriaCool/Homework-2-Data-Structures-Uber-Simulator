// Copyright 2019 Nedelcu Horia (nedelcu.horia.alexandru@gmail.com)

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <vector>
#define ULL unsigned int
#define PRIME_CAPACITY_FOR_HASH 666013

enum SlotType {Empty, Occupied, Lazy_Delete};

template <typename Tkey, typename Tvalue>
struct info {
    Tkey key;
    Tvalue value;
};

template <typename Tkey, typename Tvalue>
class Hashtable {
 private:
    std::vector<struct info<Tkey, Tvalue>> hash_table_;
    std::vector<SlotType> slot_;
    int size_;
    int capacity_;
    ULL (*hash_)(Tkey);

 public:
    // Constructor
    Hashtable(int, ULL (*h)(Tkey));

    // Destructor
    ~Hashtable();

    // Find an empty slot if it exists for insertion
    int findSlotInsert(const Tkey&);

    // Return the index's slot of key to be searched
    int findSlotSearch(const Tkey&);

    // Return true if the key is in hashtable, false otherwise
    bool lookup(const Tkey&);

    // Puts value associated with key in hashtable
    void set(const Tkey&, const Tvalue&);

    // Remove key from hashtable
    void remove(const Tkey&);

    // Return value associated with key, if it exists
    Tvalue operator[](const Tkey&);

    // Return numbers of keys from hashtable
    int getSize();

    // Return the maximum numbers of keys that hashtable can hold
    int getCapacity();
};

template <typename Tkey, typename Tvalue>
Hashtable<Tkey, Tvalue>::Hashtable(int capacity, ULL (*h)(Tkey)):
    hash_table_(capacity), slot_(capacity, SlotType::Empty),
    size_(0), capacity_(capacity), hash_(h) {}

template <typename Tkey, typename Tvalue>
Hashtable<Tkey, Tvalue>::~Hashtable() {}

template <typename Tkey, typename Tvalue>
int Hashtable<Tkey, Tvalue>::findSlotInsert(const Tkey& key) {
    int i = hash_(key) % capacity_;

    // search until we either find the key, or find an empty slot.
    while (slot_[i] == SlotType::Occupied && hash_table_[i].key != key) {
        i = (i + 1) % capacity_;
    }

    // Cases found: Empty slot, Lazy_Delete slot, slot[i].key equal with key
    return i;
}

template <typename Tkey, typename Tvalue>
int Hashtable<Tkey, Tvalue>::findSlotSearch(const Tkey& key) {
    int i = hash_(key) % capacity_;

    // search until we either find the key, or find an empty slot.
    while (slot_[i] != SlotType::Empty && hash_table_[i].key != key) {
        i = (i + 1) % capacity_;
    }

    // Cases found: Empty slot (passed all Occupied and Lazy_Delet slots),
    // slot[i].key equal with key
    return i;
}

template <typename Tkey, typename Tvalue>
bool Hashtable<Tkey, Tvalue>::lookup(const Tkey& key) {
    int i = findSlotSearch(key);

    if (slot_[i] == SlotType::Occupied) {  // key is in table
        return true;
    } else {                                 // key is not in table
        return false;
    }
}

template <typename Tkey, typename Tvalue>
void Hashtable<Tkey, Tvalue>::set(const Tkey& key, const Tvalue& value) {
    int i = findSlotInsert(key);

    hash_table_[i].key = key;
    hash_table_[i].value = value;
    slot_[i] = SlotType::Occupied;
}

template <typename Tkey, typename Tvalue>
void Hashtable<Tkey, Tvalue>::remove(const Tkey& key) {
    int i = findSlotInsert(key);

    if (slot_[i] == SlotType::Occupied) {  // key is in the table
        slot_[i] = SlotType::Lazy_Delete;
    }
}

template <typename Tkey, typename Tvalue>
Tvalue Hashtable<Tkey, Tvalue>::operator[](const Tkey& key) {
    int i = findSlotSearch(key);

    if (slot_[i] == SlotType::Occupied) {  // key is in table
        return hash_table_[i].value;
    } else {                                 // key is not in table
        return Tvalue();
    }
}

template <typename Tkey, typename Tvalue>
int Hashtable<Tkey, Tvalue>::getSize() {
    return size_;
}

template <typename Tkey, typename Tvalue>
int Hashtable<Tkey, Tvalue>::getCapacity() {
    return capacity_;
}

#endif  // HASHTABLE_H_
