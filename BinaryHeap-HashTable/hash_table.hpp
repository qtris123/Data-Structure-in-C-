#pragma once
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <list>

namespace cs251
{
template <typename KT, typename VT> // KeyType, ValueType
class HashTable{
   public:
    /* The default constructor. size: bucket size */
    HashTable(size_t size);

    /* The default destructor. You should make sure no memory leaks happens */
    ~HashTable();

    /* Insert a key-value pair to the hash table 
     * If the pair already exists, update the value. */
    void insert(const KT& key, const VT& value);

    /* If the pair doesn't exist, return false. 
     * Else, Remove the key-value pair with the given key and return true. */
    bool remove(const KT& key);

    /* Find the key-value pair with the given key. 
     * Return the pointer to the value.
     * If the key pair doesn't exist, return nullptr */
    VT* find(const KT& key);

    size_t get_num_buckets_() {
        return num_buckets_;
    }

    std::vector<std::list<std::pair<KT,VT>>> get_buckets_() {
        return buckets_;
    }

    std::list<std::pair<KT, VT>> &operator[](size_t index); 

   private:
    std::vector<std::list<std::pair<KT, VT>>> buckets_;
    size_t num_buckets_;

    /* Hash algorithm. For string, use djb2; 
     * For size_t, use Knuth’s multiplicative hash. */
    size_t hash(const std::string& key) const;
    size_t hash(size_t key) const;

};

template <typename KT, typename VT>
std::list<std::pair<KT, VT>> &HashTable<KT, VT>::operator[](size_t index) {
    return buckets_[index];  
}

template <typename KT, typename VT>
HashTable<KT, VT>::HashTable(size_t size){
    num_buckets_ = size;
    buckets_.resize(num_buckets_);
}

template <typename KT, typename VT>
HashTable<KT, VT>::~HashTable(){
    buckets_.clear();
}

template <typename KT, typename VT>
void HashTable<KT, VT>::insert(const KT& key, const VT& value){
    size_t index = hash(key);
    auto& bucket = buckets_[index];

    for (auto& pair : bucket) {
        if (pair.first == key) {
            pair.second = value; 
            return;
        }
    }

    bucket.emplace_back(key, value);
}

template <typename KT, typename VT>
bool HashTable<KT, VT>::remove(const KT& key){
    size_t index = hash(key);
    auto& bucket = buckets_[index];

    for (auto i = bucket.begin(); i != bucket.end(); ++i) {
        if (i->first == key) {
            bucket.erase(i);
            return true;
        }
    }

    return false;
}


template <typename KT, typename VT>
VT* HashTable<KT, VT>::find(const KT& key){
    size_t index = hash(key);
    auto& bucket = buckets_[index]; // access directly to the buckets_[index], not through copying
    
    for (auto& pair : bucket) {
        if (pair.first == key) {
            return &pair.second;
        }
    }
    return nullptr;
}

template <typename KT, typename VT>
size_t HashTable<KT, VT>::hash(const std::string& key) const{
    size_t hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash % num_buckets_;
}

template <typename KT, typename VT>
size_t HashTable<KT, VT>::hash(size_t key) const{
    const size_t prime = 2654435761u; // Knuth’s multiplicative constant
    return (key * prime) % num_buckets_;
}
}  // namespace cs251