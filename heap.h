#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
#include <string>
using namespace std;

template <
         typename T,
         typename KComparator = std::equal_to<T>,
         typename PComparator = std::less<T>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const KComparator& kcomp = KComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item
    void push(const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey - Finds key matching old object &
    /// updates its location in the heap based on the new value
    void decreaseKey(const T& old, const T& newVal);

 private:
    /// Add whatever helper functions and data members you need below

    // trickle value of item up if its not there
    void trickleUp(int loc);

    // Function to trickle down nodes and maintain heap property 
    void heapify(int idx);

    // Swap the values at 2 different nodes (exists for heapify only)
    void swap(T& val1, T& val2);

 private:
   /// Data members - These should be sufficient
    std::vector< T > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, KComparator> keyToLocation_;
};

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const KComparator& kcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, kcomp)
{

}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
Heap<T,KComparator,PComparator,Hasher>::~Heap()
{

}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::push( const T& item)
{
    if (store_.size() == 0)
    {
        store_.push_back(item);
    }
    store_.push_back(item);
    keyToLocation_[item] = store_.size() - 1;
    trickleUp(store_.size() - 1);
}

// TRICKLE UP
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::trickleUp(int loc)
{
    int parent = (loc+m_-2)/m_;
    while (parent >= 1 && c_(store_[loc], store_[parent]))
    {
        keyToLocation_[store_[loc]] = parent;
        keyToLocation_[store_[parent]] = loc;
        swap(store_[parent], store_[loc]);
        loc = parent;
        parent = (loc+m_-2)/m_;
    }
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::decreaseKey(const T& old, const T& newVal)
{

    if (c_(newVal, old))
    {
        int loc = keyToLocation_[old];
        store_[loc] = newVal;
        keyToLocation_.erase(old);
        keyToLocation_[newVal] = loc;
        trickleUp(loc);
    }
}

// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
T const & Heap<T,KComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }
    // If we get here we know the heap has at least 1 item
    // Add code to return the top element
    return store_[1];
}

// Incomplete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    keyToLocation_.erase(store_[1]);
    store_[1] = store_.back();
    keyToLocation_[store_[1]] = 1;
    store_.pop_back();
    heapify(1);
    if (store_.size() == 1)
    {
        store_.pop_back();
    }
}

// HEAPIFY 
// used to maintain heap property by bringing down the value 
// that was at the bottom of heap until it is in the right place
// based off comparator used
template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::heapify(int idx)
{
    unsigned smallestChild = idx*m_ + 2 - m_;
    if (smallestChild > store_.size())
    {
        return;
    }
    else
    {
        unsigned nextChild = smallestChild + 1;
        while (nextChild < store_.size() && nextChild < (unsigned)(idx*m_ + 2) )
        {
            if (c_(store_[nextChild], store_[smallestChild]))
            {
                smallestChild = nextChild;
            }
            nextChild++;
        }
        if (smallestChild < store_.size() && c_(store_[smallestChild],store_[idx]))
        {
            keyToLocation_[store_[smallestChild]] = idx;
            keyToLocation_[store_[idx]] = smallestChild;
            swap(store_[smallestChild],store_[idx]);
            heapify(smallestChild);
        }
    }
}

/// Complete!
template <typename T, typename KComparator, typename PComparator, typename Hasher >
bool Heap<T,KComparator,PComparator,Hasher>::empty() const
{
    if (store_.size() <= 1)
    {
        return true;
    }
    return false;
}

template <typename T, typename KComparator, typename PComparator, typename Hasher >
void Heap<T,KComparator,PComparator,Hasher>::swap(T& val1, T& val2)
{
    T temp = val1;
    val1 = val2;
    val2 = temp;
}

#endif