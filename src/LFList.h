#ifndef LF_LIST_H_
#define LF_LIST_H_

#include <atomic>
#include "macros.h"
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
    explicit NodeRef(Node<DataT> *node) : ref(node) {}
    NodeRef(Node<DataT> *node, bool m) : ref(node), marked(m) {}
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
        return marked;
    }
    void SetMarked() {
        marked = true;
    }

    constexpr bool operator==(const Node<DataT> *node) const {
        return ref == node;
    }
    constexpr bool operator==(const NodeRef<DataT> other) const {
        return ref == other.ref && marked == other.marked;
    }

private:
    Node<DataT> *ref = nullptr;
    bool marked = false;
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

    LFList()
        : tail(NodeRef<DataT>{new NodeT{DataT(), nullptr}}),
          head(NodeRef<DataT>{new NodeT{DataT(), tail.load().GetNode()}})
    {}
    DEFAULT_COPY_SEMANTIC(LFList);
    DEFAULT_MOVE_SEMANTIC(LFList);
    DEFAULT_DTOR(LFList);

    bool Insert(const DataT &data);
    bool Remove(const DataT &data);
    bool Contains(const DataT &data);
    unsigned int Size() const {
        return size.load(std::memory_order_relaxed);
    }

private:
    std::pair<NodeRef<DataT>, NodeRef<DataT>> search(const DataT &key);
    bool searchImpl(NodeRef<DataT> &pred, NodeRef<DataT> &curr, const DataT &key);

private:
    std::atomic<NodeRef<DataT>> tail;
    std::atomic<NodeRef<DataT>> head;
    std::atomic_uint32_t size;
};
}   // namespace lf

#endif  // LF_LIST_H_
