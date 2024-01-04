//
// Created by Yaraslau Sedach on 09/12/2023.
//

#ifndef EADSLAB2_BI_RING_H
#define EADSLAB2_BI_RING_H
#include <iostream>
#include <cassert>
#include<algorithm>

using namespace std;
template<typename Key, typename Info>
class bi_ring{
private:
    class Node{
    private:
        Node*next;
        Node*prev;
    public:
        Key k;
        Info i;

        Node(const Key&k, const Info&i):k(k),i(i),next(nullptr),prev(nullptr){}
        Node(const Key&k, const Info&i, Node*next,Node*prev):k(k),i(i),next(next),prev(prev){}
        friend class bi_ring;
    };

    template<typename K, typename I, typename RingT>
    class iterator{
    private:
        friend class bi_ring;
        Node*itr;
        const RingT *owner;
        iterator(Node*itr, const RingT *owner):itr(itr),owner(owner){}


    public:
        iterator next() {
            if (itr == nullptr || itr == owner->sentinel) throw runtime_error("Iterator is null or sentinel");
            itr = itr->next;
            return *this;
        }

        iterator prev(){
            if(itr == nullptr|| itr == owner->sentinel) throw runtime_error("Iterator is null or sentinel");
            itr = itr->prev;
            return *this;
        }

        iterator operator+(int n)const{
            iterator result = *this;
            for (int i = 0; i < n % owner->length; ++i) {
                result++;
            }
            return result;
        }

        iterator operator-(int n)const{
            iterator result =*this;
            for (int i = 0; i < n % owner->length; ++i) {
                result--;
            }
            return result;
        }

        iterator &operator=(const iterator &other) {
            if (this != &other) {
                if (owner == other.owner) {
                    itr = other.itr;
                } else {
                    // Handle the error or throw an exception
                    throw std::runtime_error("Iterators belong to different rings");
                }
            }
            return *this;
        }

        iterator &operator++(){
            next();
            if(itr == owner->sentinel) next();
            return *this;
        }

        iterator operator++(int){
            iterator temp = *this;
            next();
            if(itr == owner->sentinel) next();
            return temp;
        }

        iterator &operator--(){
            prev();
            if(itr == owner->sentinel) prev();
            return *this;
        }

        iterator operator--(int){
            iterator temp = *this;
            prev();
            if(itr == owner->sentinel) prev();
            return temp;
        }

        bool operator==(const iterator &other)const{
            return itr == other.itr;
        }

        bool operator!=(const iterator &other)const{
            return itr != other.itr;
        }


        K &key()const{
            return itr->k;
        }

        I &info()const{
            return itr->i;
        }
    };

    int length;
    Node* sentinel;

public:
    typedef iterator<Key, Info, bi_ring> change_iterator;
    typedef iterator<const Key, const Info, bi_ring> const_iterator;

    bi_ring():length(0){
        sentinel = new Node(Key(),Info(), nullptr, nullptr);
        sentinel ->next = sentinel;
        sentinel ->prev = sentinel;
    }

    bi_ring(const bi_ring &other):length(0){
        sentinel = new Node(Key(),Info(), nullptr, nullptr);
        sentinel ->next = sentinel;
        sentinel ->prev = sentinel;
        *this = other;
    }
    ~bi_ring(){
        clear();
        delete sentinel;
    }

    bool find(const Key& key) const {
        for (const_iterator it = const_begin(); it != const_end(); it.next()) {
            if (it.key() == key) {
                return true; // found
            }
        }
        return false; // not found
    }

    //REturn const iterator to a node right after sentinel
    const_iterator const_begin()const{
        return const_iterator(sentinel->next,this);
    }

    //Returns const iterator pointing to the last element of the ring
    const_iterator const_end()const{
        return const_iterator (sentinel,this);
    }


    //Returns changeable iterator pointing to the last element of the ring
    change_iterator end(){
        return change_iterator (sentinel,this);
    }

    //Returns changeable iterator pointing to the first element of the ring
    change_iterator begin(){
        return change_iterator (sentinel->next,this);
    }


    //Remove a node at given position and return a next node
    change_iterator remove(const_iterator pos){
        if(pos == const_end()){     //if pos == const end => pos == sentinel, so we just do nothing
            return end();
        }
        Node*toDel = pos.itr;
        Node*nextNode = pos.itr->next;

        toDel->prev->next = nextNode;
        nextNode->prev = toDel->prev;
        delete toDel;
        length--;
        return change_iterator(nextNode,this);
    }

    //We delete last element, and it returns and element which is new first
    change_iterator pop_back(){
        return remove(const_begin());
    }

    //We delete first element, and it returns and element which is new last
    change_iterator pop_front(){
        return remove(const_begin());
    }


    //Add node at the end of the ring and return a changeable iterator to new Node
    change_iterator push_back(const Key& k, const Info &i){
        return insert(const_end(),k,i);
    }

    //Add node at the beginning of the ring and return a changeable iterator to new Node
    change_iterator push_front(const Key& k, const Info &i){
        return insert(const_begin(),k,i);
    }


    //Insert an element before the node pos and return iterator to new node
    change_iterator insert(const_iterator pos, const Key&k, const Info&i){
        Node*toInsert = new Node(k,i, pos.itr, pos.itr->prev);
        pos.itr->prev->next = toInsert;
        pos.itr->prev = toInsert;
        length++;
        return change_iterator (toInsert,this);
    }

    bool isEmpty()const{
        return length==0;
    };

    void clear()
    {
        while (!isEmpty()) {
            pop_back();
        }
    }


    bi_ring &operator=(const bi_ring &other){
        if(this != &other){
                clear();
                auto it = other.const_begin();
                while(it != other.const_end()){
                    push_back(it.key(), it.info());
                    it.next();
                }
        }

        return *this;
    }

    int getLen()const{
        return length;
    }

    int occurrences(const Key &k)const{
        int counter = 0;
        auto it = const_begin();
        while(it != const_end()){
            if(it.key() ==k) counter++;
            it.next();
        }
        return counter;
    }

};


//Additional tasks


template <typename Key, typename Info>
bi_ring<Key,Info>filter(const bi_ring<Key,Info>& src ,bool(pred)(const Key& )){
    bi_ring<Key,Info>result;
    for (auto it = src.const_begin(); it != src.const_end() ; it.next()) {
        if(pred(it.key())){
            result.push_back(it.key(),it.info());
        }
    }
    return result;
}

template <typename Key>
bool myPredicate(const Key& key) {
    static int count = 0; // This will count the elements
    ++count; // Increment count for each element
    return count > 3; // Only return true for elements after the third one
}


template <typename Key, typename Info>
bi_ring<Key, Info> shuffle(const bi_ring<Key,Info>& first, unsigned int fcnt,
                           const bi_ring<Key,Info>& second, unsigned int scnt, unsigned int reps) {
    bi_ring<Key, Info> result;

    for (unsigned int rep = 0; rep < reps; rep++) {
        auto first_it = first.const_begin();
        auto second_it = second.const_begin();

        for (unsigned int i = 0; i < fcnt && first_it != first.const_end(); i++, first_it.next()) {
            result.push_back(first_it.key(), first_it.info());
        }

        for (unsigned int i = 0; i < scnt && second_it != second.const_end(); i++, second_it.next()) {
            result.push_back(second_it.key(), second_it.info());
        }
    }

    return result;
}

template<typename num>
bi_ring<num, num> intersect(const bi_ring<num, num>& first, const bi_ring<num, num>& second) {
    bi_ring<num, num> result;
    //Here we compare every element of first with second
    for (auto first_it = first.const_begin(); first_it != first.const_end(); first_it.next()) {
        num start1 = first_it.key();
        num end1 = start1 + first_it.info();

        for (auto second_it = second.const_begin(); second_it != second.const_end(); second_it.next()) {
            num start2 = second_it.key();
            num end2 = start2 + second_it.info();

            // Find the max start and min end
            num maxStart = max(start1, start2);
            num minEnd = min(end1, end2);

            // Check for overlap
            if (maxStart < minEnd) {
                // There is an overlap
                result.push_back(maxStart, minEnd - maxStart);
            }
        }
    }

    return result;
}

#endif //EADSLAB2_BI_RING_H


