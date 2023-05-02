#include "LFList.h"


namespace lf {
namespace {
template <Comparable DataT>
inline auto toMarkedRef(NodeRef<DataT> ref) {
    return NodeRef<DataT>{ref.GetNode(), true};
}

template <Comparable DataT>
inline auto toMarkedRef(Node<DataT> *node) {
    return NodeRef<DataT>{node, true};
}

template <Comparable DataT>
inline auto toUnmarkedRef(NodeRef<DataT> ref) {
    return NodeRef<DataT>{ref.GetNode(), false};
}

template <Comparable DataT>
inline auto toUnmarkedRef(Node<DataT> *node) {
    return NodeRef<DataT>{node, false};
}
}   // anonymous namespace


template <Comparable DataT>
bool LFList<DataT>::Insert(const DataT &data) {
    auto *node = allocate(data, nullptr);

    while (true) {
        auto [pred, curr] = search(data);
        if (curr != getSentinel() && curr->data == data) {
            // key already present
            memResource->deallocate(node, sizeof(NodeT));
            return false;
        }

        ASSERT(curr.GetNode() && !curr.IsMarked());
        node->next.store(curr, std::memory_order_release);
        if (pred->next.compare_exchange_strong(curr, toUnmarkedRef(node))) {
            size.fetch_add(1, std::memory_order_acq_rel);
            return true;
        }
    }
    UNREACHABLE();
    return false;
}

template <Comparable DataT>
bool LFList<DataT>::Remove(const DataT &data) {
    while (true) {
        auto [pred, curr] = search(data);
        if (curr == getSentinel() || curr->data != data) {
            return false;
        }

        auto next = curr->next.load(std::memory_order_acquire);
        // try to mark curr for removal
        if (next.IsMarked() || !curr->next.compare_exchange_strong(next, toMarkedRef(next))) {
            continue;
        }
        // either this or another thread will actually remove
        if (!pred->next.compare_exchange_strong(curr, NodeRef<DataT>{next.GetNode(), curr.IsMarked()})) {
            search(curr->data);
        }
        size.fetch_sub(1, std::memory_order_acq_rel);
        return true;
    }
    UNREACHABLE();
    return false;
}

template <Comparable DataT>
bool LFList<DataT>::Contains(const DataT &data) {
    auto [pred, curr] = search(data);
    return (curr != getSentinel()) && (curr->data == data);
}

template <Comparable DataT>
std::pair<NodeRef<DataT>, NodeRef<DataT>> LFList<DataT>::search(const DataT &key) {
    const auto *sentinel = getSentinel();
    NodeRef<DataT> left{nullptr};
    NodeRef<DataT> leftNext{nullptr};
    NodeRef<DataT> right{nullptr};

    while (true) {
        auto pred = head.load(std::memory_order_acquire);
        auto curr = pred->next.load(std::memory_order_acquire);
        ASSERT(!curr.IsMarked());

        do {
            if (!curr.IsMarked()) {
                left = pred;
                leftNext = curr;
            }

            pred = toUnmarkedRef(curr);
            if (pred == sentinel) {
                break;
            }
            curr = pred->next.load(std::memory_order_acquire);
        } while (curr.IsMarked() || pred->data < key);
        right = pred;

        if (leftNext == right) {
            if (right != sentinel && right->next.load(std::memory_order_acquire).IsMarked()) {
                continue;
            }
            return {left, right};
        }

        ASSERT(left.GetNode());
        // remove marked nodes
        if (left->next.compare_exchange_strong(leftNext, toUnmarkedRef(right))) {
            if (right != sentinel && right->next.load(std::memory_order_acquire).IsMarked()) {
                continue;
            }
            return {left, right};
        }
    }
    UNREACHABLE();
    return {left, right};
}

template class LFList<int>;
}   // namespace lf
