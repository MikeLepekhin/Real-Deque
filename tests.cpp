#include "deque.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <deque>
#include <vector>

TEST(DequeTest, checkPushBackAndIterator) {
    Deque<int> d;
    for (int i = -65; i < 128; ++i)
        d.push_back(i);
    ASSERT_EQ(d.size(), size_t(128 + 65));
    Deque<int>::const_iterator it = d.cbegin();
    for (int i = -65; i < 128; ++i, ++it) 
        ASSERT_EQ(*it, i);
}

TEST(DequeTest, checkPushBackAndPushFrontAndIterators) {
    const int DSIZE = 100;
    Deque<int> d;
    for (size_t i = 1; i <= DSIZE; ++i)
        if (i % 2 == 1)
            d.push_back(i);
        else
            d.push_front(i);
    std::vector<int> need;
    for (int i = DSIZE; i >= 1; --i)
        if (i % 2 == 0)
            need.push_back(i);
    for (int i = 1; i <= DSIZE; ++i)
        if (i % 2 == 1)
            need.push_back(i);
    Deque<int>::iterator it1 = d.begin();
    Deque<int>::iterator it2 = d.end() -1;
    for (int i = 0; i < DSIZE; ++i) { 
        ASSERT_EQ(*(it1 + i), need[i]);
        ASSERT_EQ(*(it2 - (DSIZE - i - 1)), need[i]);
    }
    Deque<int>::reverse_iterator it3 = d.rbegin();
    Deque<int>::reverse_iterator it4 = d.rend() - 1;
    for (int i = 0; i < DSIZE; ++i) {
        ASSERT_EQ(*(it3 + i), need[DSIZE - i - 1]);
        ASSERT_EQ(*(it4 - (DSIZE - i - 1)), need[DSIZE - i - 1]);
    }
}

TEST(DequeTest, checkIterators) {
    const int DSIZE = 123;
    Deque<int> d;
    for (int i = 0; i < DSIZE; ++i)
        d.push_back(0);
    Deque<int>::iterator a = d.begin();
    for (int i = 0; i < DSIZE; ++i) 
        a[i] = i;
    Deque<int>::reverse_iterator b = d.rbegin();
    for (int i = 0; i < DSIZE; ++i) 
        b[i] *= i;
    ASSERT_EQ(b.base() - a, DSIZE);
    for (int i = 0; i < DSIZE; ++i) 
        ASSERT_EQ(d[i], i * (DSIZE - 1 - i));
}

TEST(DequeTest, checkIntSort) {
    Deque<int> d;
    for (size_t i = 0; i < 1000000; ++i) 
        d.push_back(rand());
    std::sort(d.begin(), d.end());
    ASSERT_TRUE(std::is_sorted(d.begin(), d.end()));
    std::sort(d.begin(), d.end(), std::greater<int>());
    ASSERT_TRUE(std::is_sorted(d.rbegin(), d.rend()));
}

TEST(DequeTest, checkStringSort) {
    using std::string;
    Deque<string> d;
    for (size_t i = 0; i < 2; ++i) {
        int x = rand();
        string add = "";
        while (x > 0) {
            add.push_back(char(x % 10 + '0'));
            x /= 10;
        }   
        //std::cout << add << '\n';
        if (add.empty())
            add.push_back('0');
        reverse(add.begin(), add.end());
        d.push_back(add);
    }
    /*std::sort(d.begin(), d.end());
    ASSERT_TRUE(std::is_sorted(d.begin(), d.end()));
    std::sort(d.begin(), d.end(), std::greater<string>());
    ASSERT_TRUE(std::is_sorted(d.rbegin(), d.rend()));*/
}

TEST(DequeTest, pairSort) {
    using std::pair;
    Deque<pair<int, int>> d;
    for (size_t i = 0; i < 1000000; ++i) 
        d.push_back({rand(), rand()});
    std::sort(d.begin(), d.end());
    ASSERT_TRUE(std::is_sorted(d.begin(), d.end()));
    std::sort(d.begin(), d.end(), std::greater<pair<int, int>>());
    ASSERT_TRUE(std::is_sorted(d.rbegin(), d.rend()));
}

TEST(DequeTest, checkResize) {
    
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}