//
// Created by Yaraslau Sedach on 09/12/2023.
//
#include"bi_ring.h"
#ifndef EADSLAB2_BI_RING_TEST_H
#define EADSLAB2_BI_RING_TEST_H
template<typename Key, typename Info>
void print(const bi_ring<Key, Info>& ring) {
    auto it = ring.const_begin();
    if (it == ring.const_end()) {
        cout << "The ring is empty." << endl;
        return;
    }

    cout << "Ring contents: " << endl;
    do {
        cout << "Key: " << it.key() << ", Info: " << it.info() << endl;
        it.next();
    } while (it != ring.const_end());
}


void test_push_back() {
    bi_ring<int, int> ring;

    // Test adding elements
    ring.push_back(1, 100);
    ring.push_back(2, 200);
    ring.push_back(3, 300);

    // Verify the elements
    auto it = ring.const_begin();
    assert(it.key() == 1 && it.info() == 100); it.next();
    assert(it.key() == 2 && it.info() == 200); it.next();
    assert(it.key() == 3 && it.info() == 300);

    // Verify the size of the ring
    assert(ring.getLen() == 3);

    cout << "Push back test passed." << endl;
}

void test_push_front() {
    bi_ring<int, int> ring;

    // Test 1: Push one element and check
    ring.push_front(1, 100);
    assert(ring.getLen() == 1);
    assert(ring.begin().key() == 1 && ring.begin().info() == 100);

    // Test 2: Push another element and check
    ring.push_front(2, 200);
    assert(ring.getLen() == 2);
    assert(ring.begin().key() == 2 && ring.begin().info() == 200);

    // Test 3: Push a third element and check
    ring.push_front(3, 300);
    assert(ring.getLen() == 3);
    assert(ring.begin().key() == 3 && ring.begin().info() == 300);

    cout << "All push_front tests passed" << endl;
}

void test_insert() {
    bi_ring<int, int> ring;

    // Insert at the beginning
    ring.insert(ring.const_begin(), 1, 100);
    assert(ring.getLen() == 1);
    assert(ring.const_begin().key() == 1 && ring.const_begin().info() == 100);

    // Insert at the end
    ring.insert(ring.const_end(), 3, 300);
    assert(ring.getLen() == 2);

    // Check the last element
    auto last = ring.const_begin();
    last.next(); // Move to the second element
    assert(last.key() == 3 && last.info() == 300);

    // Insert in the middle
    auto middle = ring.const_begin(); // This points to the first element
    ring.insert(++middle, 2, 200); // Insert after the first element
    assert(ring.getLen() == 3);

    // Check if the middle element is correct
    middle = ring.const_begin();
    middle++; // Move to the second element, which should now be the middle element
    assert(middle.key() == 2 && middle.info() == 200);

    cout << "Insert test passed." << endl;
}


void test_pop_back() {
    bi_ring<int, int> ring;

    for (int i = 1; i <= 3; ++i) {
        ring.push_back(i, i * 100);
    }
    ring.pop_back();
    assert(ring.getLen() == 2);

    auto it = ring.const_begin();
    assert(it.key() == 2 && it.info() == 200); it.next();
    assert(it.key() == 3 && it.info() == 300);

    cout << "test_pop_back passed" << endl;
}

void test_pop_front() {
    bi_ring<int, int> ring;


    for (int i = 1; i <= 3; ++i) {
        ring.push_back(i, i * 100);
    }

    // Test pop_front
    ring.pop_front();
    assert(ring.getLen() == 2);

    auto it = ring.const_begin();
    assert(it.key() == 2 && it.info() == 200); it.next();
    assert(it.key() == 3 && it.info() == 300);

    cout << "test_pop_front passed" << endl;
}

void test_remove() {
    bi_ring<int, int> ring;

    for (int i = 1; i <= 3; ++i) {
        ring.push_back(i, i * 100);
    }

    // removing the second element
    auto it = ring.const_begin();
    it.next(); // Move to the second element
    ring.remove(it);
    assert(ring.getLen() == 2);

    it = ring.const_begin();
    assert(it.key() == 1 && it.info() == 100);
    it.next();
    assert(it.key() == 3 && it.info() == 300);

    cout << "test_remove passed" << endl;
}

void test_assignment_operator() {
    bi_ring<int, int> original_ring;
    for (int i = 1; i <= 3; ++i) {
        original_ring.push_back(i, i * 100);
    }

    bi_ring<int, int> assigned_ring;
    assigned_ring = original_ring;

    // Verify copy correctness
    assert(assigned_ring.getLen() == original_ring.getLen());
    auto orig_it = original_ring.const_begin();
    auto assign_it = assigned_ring.const_begin();
    for (int i = 0; i < original_ring.getLen(); ++i, orig_it.next(), assign_it.next()) {
        assert(orig_it.key() == assign_it.key() && orig_it.info() == assign_it.info());
    }

    // Test self-assignment
    original_ring = original_ring;
    assert(original_ring.getLen() == 3); // Assuming length is 3

    // Test separation of copies
    original_ring.pop_back();
    assert(original_ring.getLen() != assigned_ring.getLen());

    cout << "Assignment operator tests passed" << endl;
}
void test_occurrences_len_isEmpty() {
    bi_ring<int, int> ring;

    // Test isEmpty on an empty ring
    assert(ring.isEmpty());
    assert(ring.getLen() == 0);

    // Add some elements to the ring
    ring.push_back(1, 100);
    ring.push_back(2, 200);
    ring.push_back(1, 300);

    // Test isEmpty on a non-empty ring
    assert(!ring.isEmpty());

    // Test getLen
    assert(ring.getLen() == 3);

    // Test occurrences
    assert(ring.occurrences(1) == 2);
    assert(ring.occurrences(2) == 1);
    assert(ring.occurrences(3) == 0); // Key not present

    cout << "Occurrences, Len, and IsEmpty tests passed." << endl;
}

void test_find() {
    bi_ring<int, int> ring;

    ring.push_back(1, 100);
    ring.push_back(2, 200);
    ring.push_back(3, 300);

    // Test 1: Search for an existing key
    assert(ring.find(2) == true);

    // Test 2: Search for a non-existing key
    assert(ring.find(4) == false);

    // Test 3: Search for the first key
    assert(ring.find(1) == true);

    // Test 4: Search for the last key
    assert(ring.find(3) == true);
    cout << "Tests find passed" << endl;
}

void test_filter() {
    // Createa bi_ring with 6 elements
    bi_ring<int, int> src;
    for (int i = 1; i <= 6; ++i) {
        src.push_back(i, i * 100);
    }

    bi_ring<int, int> result = filter(src, myPredicate);

    // Check the size of the result
    assert(result.getLen() == 3); // We expect only 3 elements (4th, 5th, 6th)

    // Check that the elements are the 4th, 5th, 6th elements from the source
    auto it = result.const_begin();
    assert(it.key() == 4 && it.info() == 400); it.next();
    assert(it.key() == 5 && it.info() == 500); it.next();
    assert(it.key() == 6 && it.info() == 600);

    cout << "Filter test passed." << endl;
}

void test_shuffle() {
    bi_ring<int, int> first;
    bi_ring<int, int> second;

    for (int i = 1; i <= 5; ++i) {
        first.push_back(i, i * 100); // First ring contains elements 1, 2, 3, 4, 5
        second.push_back(i + 5, (i + 5) * 100); // Second ring contains elements 6, 7, 8, 9, 10
    }

    // Shuffle the two rings
    bi_ring<int, int> result = shuffle(first, 2, second, 3, 2);

    // Verify the shuffle result
    // Expected sequence after shuffle: 1,2,6,7,8,1,2,6,7,8
    int expected_sequence[] = {1, 2, 6, 7, 8, 1, 2, 6, 7, 8};
    int index = 0;

    for (auto it = result.const_begin(); it != result.const_end(); it.next()) {
        assert(it.key() == expected_sequence[index]);
        index++;
    }

    assert(index == 10); //Check if all elements are covered

    cout << "Shuffle test passed." << endl;
}

void test_intersect(){
    bi_ring<float,float>first;
    bi_ring<float,float>second;
    // insert elements to first as in example
    first.push_back(1.0,4.0);
    first.push_back(7.2,2.1);
    first.push_back(10.5,4.7);
    //insert elements to second as in exmaple
    second.push_back(2.0,1.0);
    second.push_back(8.0,4.5);

    auto result = intersect(first,second);
    //result should be the same as in exmaple

    print(result);
}
#endif //EADSLAB2_BI_RING_TEST_H
