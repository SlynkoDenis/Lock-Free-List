#include <algorithm>
#include "gtest/gtest.h"
#include "LFList.h"
#include <random>
#include <thread>


namespace lf::tests {
using IterType = typename std::vector<int>::const_iterator;

class LFListTest : public ::testing::Test {
public:
    std::vector<int> RandomShuffle(int n);
    static std::vector<std::pair<IterType, IterType>> Divide(const std::vector<int> &vec, int nParts);

    static void Producer(std::pair<IterType, IterType> &&rng, LFList<int> *lst);
    static void Consumer(std::pair<IterType, IterType> &&rng, LFList<int> *lst);

public:
    static constexpr int NUMBER_ELEMENTS = 1000000;

private:
    std::mt19937 randGen;
};

std::vector<int> LFListTest::RandomShuffle(int n) {
    ASSERT(n > 0);
    std::vector<int> vec(n);
    std::iota(vec.begin(), vec.end(), 0);
    std::shuffle(vec.begin(), vec.end(), randGen);
    return vec;
}

/* static */
std::vector<std::pair<IterType, IterType>> LFListTest::Divide(const std::vector<int> &vec, int nParts) {
    ASSERT(nParts > 0);
    if (nParts == 1) {
        return {{vec.begin(), vec.end()}};
    }

    std::vector<std::pair<decltype(vec.begin()), decltype(vec.begin())>> res(nParts);
    int step = vec.size() / nParts;
    auto iter = vec.begin();
    for (int i = 0, end = nParts - 1; i < end; ++i) {
        res[i] = {iter, iter + step};
        iter += step;
    }
    res[nParts - 1] = {iter, vec.end()};

    return res;
}

/* static */
void LFListTest::Producer(std::pair<IterType, IterType> &&rng, LFList<int> *lst) {
    for (auto iter = rng.first, end = rng.second; iter != end; ++iter) {
        while (true) {
            ASSERT_FALSE(lst->Contains(*iter));
            if (lst->Insert(*iter)) {
                break;
            }
        }
    }
}

/* static */
void LFListTest::Consumer(std::pair<IterType, IterType> &&rng, LFList<int> *lst) {
    for (auto iter = rng.first, end = rng.second; iter != end; ++iter) {
        while (true) {
            // numbers are unique
            // ASSERT_TRUE(lst->Contains(*iter));
            // ASSERT_GT(lst->Size(), 0);
            if (lst->Remove(*iter)) {
                break;
            }
        }
    }
}

TEST_F(LFListTest, Prod1Cons1) {
    auto data = RandomShuffle(NUMBER_ELEMENTS);
    LFList<int> lst;

    std::thread prod(LFListTest::Producer, std::pair<IterType, IterType>{data.begin(), data.end()}, &lst);
    std::thread cons(LFListTest::Consumer, std::pair<IterType, IterType>{data.begin(), data.end()}, &lst);

    prod.join();
    cons.join();

    ASSERT_EQ(lst.Size(), 0);
}
}   // namespace lf::tests
