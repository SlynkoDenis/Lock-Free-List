#ifndef LF_LIST_H_
#define LF_LIST_H_

#include <atomic>
#include "macros.h"
#include <memory_resource>
#include <type_traits>


namespace lf {
template <typename T>
concept Comparable = std::is_default_constructible_v<T>
    && std::is_copy_constructible_v<T>
    && std::is_copy_assignable_v<T>
    && requires(const T &t1, const T &t2) {
        { t1 < t2 } -> std::convertible_to<bool>;   // comparable
    };

template <Comparable DataT>
struct Node;

template <Comparable DataT>
class NodeRef final {
public:
    explicit NodeRef(Node<DataT> *node, bool m = false) : ref(node), marked{static_cast<char>(m)} {}

    NodeRef() = delete;
    DEFAULT_COPY_SEMANTIC(NodeRef);
    DEFAULT_MOVE_SEMANTIC(NodeRef);
    DEFAULT_DTOR(NodeRef);

    Node<DataT> *GetNode() {
        return ref;
    }
    const Node<DataT> *GetNode() const {
        return ref;
    }

    Node<DataT> *operator->() {
        return GetNode();
    }
    const Node<DataT> *operator->() const {
        return GetNode();
    }

    bool IsMarked() const {
        return marked[0] > 0;
    }

    constexpr bool operator==(const Node<DataT> *node) const {
        return ref == node;
    }
    constexpr bool operator==(const NodeRef<DataT> &other) const {
        return ref == other.ref && marked[0] == other.marked[0];
    }

private:
    static constexpr size_t WORD_SIZE = sizeof(int *);

    Node<DataT> *ref;
    char marked[WORD_SIZE];
};

// based on
// Timothy L. Harris, "A Pragmatic Implementation of Non-Blocking Linked-List"
template <Comparable DataT>
struct Node final {
    Node(const DataT &d, Node *n) : data(d), next(NodeRef<DataT>{n}) {}
    Node(DataT &&d, Node *n) : data(std::move(d)), next(NodeRef<DataT>{n}) {}

    Node() = delete;
    DEFAULT_COPY_SEMANTIC(Node);
    DEFAULT_MOVE_SEMANTIC(Node);
    DEFAULT_DTOR(Node);

    DataT data;
    std::atomic<NodeRef<DataT>> next;
};

template <Comparable DataT>
class LFList {
public:
    using NodeT = Node<DataT>;

    explicit LFList(std::pmr::memory_resource *mem)
        : memResource(mem),
          tail(NodeRef<DataT>{allocate(mem, DataT(), nullptr)}),
          head(NodeRef<DataT>{allocate(mem, DataT(), tail.load(std::memory_order_relaxed).GetNode())}),
          size(0)
    {}
    DEFAULT_COPY_SEMANTIC(LFList);
    DEFAULT_MOVE_SEMANTIC(LFList);
    ~LFList() noexcept {
        memResource->deallocate(tail.load(std::memory_order_relaxed).GetNode(), sizeof(NodeT));
        memResource->deallocate(head.load(std::memory_order_relaxed).GetNode(), sizeof(NodeT));
    }

    bool Insert(const DataT &data);
    bool Remove(const DataT &data);
    bool Contains(const DataT &data);
    unsigned int Size() const {
        return size.load(std::memory_order_acquire);
    }

private:
    std::pair<NodeRef<DataT>, NodeRef<DataT>> search(const DataT &key);

    const NodeT *getSentinel() const {
        return tail.load(std::memory_order_relaxed).GetNode();
    }

    static NodeT *allocate(std::pmr::memory_resource *memResource, const DataT &data, NodeT *next) {
        ASSERT(memResource);
        auto allocator = std::pmr::polymorphic_allocator(memResource);
        auto *mem = allocator.template allocate_object<NodeT>();
        ASSERT(mem);
        allocator.construct(mem, data, next);
        return mem;
    }
    NodeT *allocate(const DataT &data, NodeT *next) {
        return allocate(memResource, data, next);
    }

private:
    std::pmr::memory_resource *memResource;

    std::atomic<NodeRef<DataT>> tail;
    std::atomic<NodeRef<DataT>> head;
    std::atomic_uint32_t size;
};
}   // namespace lf

#endif  // LF_LIST_H_
