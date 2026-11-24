#include "map_audit.h"
#include "disjoint_set.h"

#include <cassert>
#include <iostream>
#include <vector>

void test_regions() {
    std::vector<Edge> bridges = {
        {0, 1, 5},
        {1, 2, 3},
        {3, 4, 2}
    };

    std::vector<int> result = region_labels(6, bridges);

    assert(result.size() == 6);
    assert(result[0] == result[1]);
    assert(result[1] == result[2]);
    assert(result[3] == result[4]);
    assert(result[0] != result [3]);
    assert(result[5] != result[0]);
    assert(result[5] != result[3]);

    std::cout << "Test 1: Connected regions passed\n";
}

void test_isolated_islands() {
    std::vector<Edge> bridges;

    std::vector<int> result = region_labels(4, bridges);

    assert(result.size() == 4);

    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            assert(result[i] != result[j]);
        }
    }

    std::cout << "Test 2: Isolated islands passed\n";
}

void test_fewest_hops() {
    std::vector<Edge> bridges = {
        {0, 1, 5},
        {1, 2, 3},
        {0, 3, 2},
        {3, 2, 4}
    };

    assert(fewest_hops(4, bridges, 0, 2) == 2);
    assert(fewest_hops(4, bridges, 0, 1) == 1);

    std::cout << "Test 3: Fewest hops passed\n";
}

void test_unreachable() {
    std::vector<Edge> bridges = {
        {0, 1, 5},
        {2, 3, 4}
    };

    assert(fewest_hops(4, bridges, 0, 3) == -1);

    std::cout << "Test 4: Unreachable island passed\n";
}

void test_same_island() {
    std::vector<Edge> bridges = {
        {0, 1, 5}
    };

    assert(fewest_hops(2, bridges, 0, 0) == 0);

    std::cout << "Test 5: Same island passed\n";
}

void test_disjoint_set() {
    DisjointSet ds(5);

    assert(ds.count() == 5);

    assert(ds.unite(0, 1));
    assert(ds.unite(1, 2));
    assert(ds.count() == 3);

    assert(ds.find(0) == ds.find(2));
    assert(ds.find(0) != ds.find(3));

    assert(!ds.unite(0, 2));
    assert(ds.count() == 3);

    assert(ds.unite(3, 4));
    assert(ds.count() == 2);

    assert(ds.unite(2, 4));
    assert(ds.count() == 1);

    std::cout << "Test 6: DisjointSet passed\n";
}

int main() {
    test_regions();
    test_isolated_islands();
    test_fewest_hops();
    test_unreachable();
    test_same_island();
    test_disjoint_set();

    std::cout << "\nAll map audit tests passed!\n";

    return 0;
}