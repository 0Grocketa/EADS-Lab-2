//
// Created by Yaraslau Sedach on 09/12/2023.
//
#include"bi_ring_test.h"



void tests(){
    test_push_back();
    test_push_front();
    test_insert();
    test_pop_back();
    test_pop_front();
    test_remove();
    test_assignment_operator();
    test_occurrences_len_isEmpty();
    test_find();
    test_filter();
    test_shuffle();
}

int main() {
    tests();
    cout<<"\n Test intersect"<<endl;
    test_intersect();
    return 0;
}