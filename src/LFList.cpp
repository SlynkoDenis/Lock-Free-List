#include "LFList.h"


namespace lf {
template <Comparable DataT>
bool LFList<DataT>::Insert(const DataT &data) {
    auto *node = new Node<DataT>(data, nullptr);
    NodeRef<DataT> nodeRef{node};

    while (true) {
        auto [pred, curr] = search(data);
        if (curr->data == data) {
            delete node;
            return false;
        }

        node->next.store(curr);
        if (pred->next.compare_exchange_weak(curr, nodeRef)) {
            size.fetch_add(1, std::memory_order_relaxed);
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
        if (curr->data != data) {
            return false;
        }

        auto next = curr->next.load();
        NodeRef<DataT> marked{next.GetNode(), true};
        if (!curr->next.compare_exchange_weak(next, marked)) {
            continue;
        }
        // either this or another thread will update
        if (pred->next.compare_exchange_strong(curr, next)) {
            size.fetch_sub(1, std::memory_order_relaxed);
        }
        return true;
    }
    UNREACHABLE();
    return false;
}

template <Comparable DataT>
bool LFList<DataT>::Contains(const DataT &data) {
    auto *curr = head.load().GetNode();
    while (curr && curr->data < data) {
        curr = curr->next.load().GetNode();
    }
    return curr != nullptr && curr->data == data && !curr->next.load().IsMarked();
}

template <Comparable DataT>
std::pair<NodeRef<DataT>, NodeRef<DataT>> LFList<DataT>::search(const DataT &key) {
    while (true) {
        auto pred = head.load();
        auto curr = pred->next.load();
        if (searchImpl(pred, curr, key)) {
            return {pred, curr};
        }
    }
    UNREACHABLE();
    return {NodeRef<DataT>{nullptr}, NodeRef<DataT>{nullptr}};
}

template <Comparable DataT>
bool LFList<DataT>::searchImpl(NodeRef<DataT> &pred, NodeRef<DataT> &curr, const DataT &key) {
    while (true) {
        auto next = curr->next.load();
        if (next == tail) {
            return true;
        }

        // delete marked nodes
        while (next.IsMarked()) {
            if (!pred->next.compare_exchange_weak(curr, next)) {
                return false;
            }
            curr = next;
            next = curr->next.load();
        }

        if (curr->data >= key) {
            return true;
        }
        pred = curr;
        curr = next;
    }
    UNREACHABLE();
    return false;
}

template class LFList<int>;
}   // namespace lf
