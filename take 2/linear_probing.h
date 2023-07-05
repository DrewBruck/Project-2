/*
Name: Drew Bruck
Class: CSCI 335
Professor: Jaime Canizales
Assignment: Project 2
Description: Modified version of double_hashing.h for Project 2.
*/
#ifndef LINEAR_PROBING_H
#define LINEAR_PROBING_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

namespace {

// Internal method to test if a positive number is prime.
bool IsPrimeLinear(size_t n) {
  if( n == 2 || n == 3 )
    return true;
  
  if( n == 1 || n % 2 == 0 )
    return false;
  
  for( size_t i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;
  
  return true;
}

// Internal method to return a prime number at least as large as n.
int NextPrimeLinear(size_t n) {
  if (n % 2 == 0)
    ++n;  
  while (!IsPrimeLinear(n)) n += 2;  
  return n;
}
}  // namespace

// Linear probing implementation
template <typename HashedObj>
class HashTableLinear {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableLinear(size_t size = 101) : array_(NextPrimeLinear(size))
    { MakeEmpty(); }
  
  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    number_of_collisions_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos)) {
      return false;
    }
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

 // returns the size of the hash table
  double getTotalNumberOfElements( ) { return static_cast<double>( current_size_ ); }

  // getTotalSizeOfTable() assigns n to the size of the array
  double getTotalSizeOfTable( ) { return static_cast<double>( array_.size() ); }

    //returns the number of collitions that occurred during insert operation
  void getTotalNumberOfCollisions (size_t &n) { n = number_of_collisions_; }

  // resetCollisions() resets number of collisions to 0
  void resetCollisions (  ) { number_of_collisions_ = 0; }

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
  
  //initialize counter for collisions, mutable to allow changes even with const
  mutable size_t number_of_collisions_; 
  
  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    //size_t offset = 1;
    size_t current_pos = InternalHash(x);

    //linear procedure
    while (  array_[current_pos].info_ != EMPTY &&
             array_[current_pos].element_ != x  ) {
      current_pos += 1;  // Compute ith probe.
      //offset += 2;

      // array wrap around
      if (current_pos >= array_.size()) {
        current_pos -= array_.size();
      }
      number_of_collisions_++;
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrimeLinear(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
        Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
};

#endif  // LINEAR_PROBING_H
