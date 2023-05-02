#include <algorithm>
#include "gtest/gtest.h"
#include "LFList.h"
#include <random>
#include <thread>


namespace lf::tests {
using IterType = typename std::vector<int>::const_iterator;

class LFListTest : public ::testing::Test {
public:
    void SetUp() override {
        ::testing::Test::SetUp();
        nProducers = 0;
        producersFinished.store(0, std::memory_order_relaxed);
    }

    std::vector<int> RandomShuffle(int n);
    static std::vector<std::pair<IterType, IterType>> Divide(const std::vector<int> &vec, int nParts);

    static void Producer(std::pair<IterType, IterType> &&rng, LFList<int> *lst, LFListTest *test);
    static void Consumer(std::pair<IterType, IterType> &&rng, LFList<int> *lst, LFListTest *test);

    template <int ProdN, int ConsN>
    void TestProdNConsM();

    void SetProducersNumber(unsigned int n) {
        nProducers = n;
    }
    auto AllProducersFinished() const {
        return nProducers == producersFinished.load(std::memory_order_acquire);
    }
    void RegisterProducerFinished() {
        producersFinished.fetch_add(1, std::memory_order_acq_rel);
    }

public:
    static constexpr int NUMBER_ELEMENTS = 10000;

private:
    std::mt19937 randGen;

    unsigned int nProducers = 0;
    std::atomic_uint producersFinished = 0;
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

    std::vector<std::pair<IterType, IterType>> res(nParts);
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
void LFListTest::Producer(std::pair<IterType, IterType> &&rng, LFList<int> *lst, LFListTest *test) {
    for (auto iter = rng.first, end = rng.second; iter != end; ++iter) {
        while (true) {
            ASSERT_FALSE(lst->Contains(*iter));
            if (lst->Insert(*iter)) {
                break;
            }
            std::this_thread::yield();
        }
    }

    test->RegisterProducerFinished();
}

/* static */
void LFListTest::Consumer(std::pair<IterType, IterType> &&rng, LFList<int> *lst, LFListTest *test) {
    for (auto iter = rng.first, end = rng.second; iter != end; ++iter) {
        while (true) {
            if (lst->Remove(*iter)) {
                break;
            }
            std::this_thread::yield();

            if (test->AllProducersFinished()) {
                ASSERT_TRUE(lst->Contains(*iter));
                ASSERT_GT(lst->Size(), 0);
            }
        }
    }
}

template <int ProdN, int ConsN>
void LFListTest::TestProdNConsM() {
    auto data = RandomShuffle(NUMBER_ELEMENTS);
    std::pmr::synchronized_pool_resource memResource;
    LFList<int> lst(&memResource);

    SetProducersNumber(ProdN);
    std::array<std::thread, ProdN> producers;
    std::array<std::thread, ConsN> consumers;

    auto prodDiv = Divide(data, ProdN);
    auto consDiv = Divide(data, ConsN);
    for (size_t i = 0; i < ProdN; ++i) {
        producers[i] = std::thread(LFListTest::Producer, prodDiv[i], &lst, this);
    }
    for (size_t i = 0; i < ConsN; ++i) {
        consumers[i] = std::thread(LFListTest::Consumer, consDiv[i], &lst, this);
    }

    for (size_t i = 0; i < ProdN; ++i) {
        producers[i].join();
    }
    for (size_t i = 0; i < ConsN; ++i) {
        consumers[i].join();
    }

    ASSERT_EQ(lst.Size(), 0);
}

#define TEST_PROD_CONS(N, M)            \
TEST_F(LFListTest, Prod##N##Cons##M) {  \
    TestProdNConsM<N, M>();             \
}

TEST_PROD_CONS(1, 1)
TEST_PROD_CONS(1, 2)
TEST_PROD_CONS(2, 1)
TEST_PROD_CONS(2, 2)
TEST_PROD_CONS(3, 1)
TEST_PROD_CONS(3, 2)
TEST_PROD_CONS(4, 4)
}   // namespace lf::tests
