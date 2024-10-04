#pragma once

#ifndef _MAP_
#define _MAP_

#include "libs/string/string.h"

template<typename _KTy, typename _VTy>
class map {
private:
    struct Node {
        _KTy key;
        _VTy value;
        Node* left;
        Node* right;

        Node() = default;

        Node(const _KTy& key, const _VTy& value)
            : key(key), value(value), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, const _KTy& key, const _VTy& value);
    Node* find(Node* node, const _KTy& key) const;
    Node* erase(Node* node, const _KTy& key);
    Node* findMin(Node* node) const;
    void clear(Node* node);

    Node* copy(Node* node) const;

    size_t size(Node* node) const;

    template<typename Func>
    void inOrderTraversal(Node* node, Func func) const;

public:
    map() : root(nullptr) {}

    map(const map& other) : root(copy(other.root)) {}

    map& operator=(const map& other) {
        if (this != &other) {
            clear(root);
            root = copy(other.root);
        }
        return *this;
    }

    ~map() {
        clear(root);
    }

    void insert(const _KTy& key, const _VTy& value);
    _VTy& operator[](const _KTy& key);
    _VTy* find(const _KTy& key) const;
    void erase(const _KTy& key);
    bool has(const _KTy& key) const;

    size_t size() const;

    template<typename Func>
    void forEach(Func func) const;
};

template<typename _KTy, typename _VTy>
inline typename map<_KTy, _VTy>::Node* map<_KTy, _VTy>::copy(Node* node) const {
    if (node == nullptr) {
        return nullptr;
    }

    Node* newNode = (Node*)(malloc(sizeof(Node)));

    memcpy(&(newNode->key), &(node->key), sizeof(_KTy));
    memcpy(&(newNode->value), &(node->value), sizeof(_VTy));

    newNode->left = copy(node->left);
    newNode->right = copy(node->right);
    return newNode;
}

template<typename _KTy, typename _VTy>
inline typename map<_KTy, _VTy>::Node* map<_KTy, _VTy>::insert(Node* node, const _KTy& key, const _VTy& value) {
    if (node == nullptr) {
        Node* new_node = (Node*)malloc(sizeof(Node));
        new_node->key = key;
        new_node->value = value;
        new_node->left = new_node->right = nullptr;
        return new_node;
    }

    if (key < node->key) {
        node->left = insert(node->left, key, value);
    }
    else if (key > node->key) {
        node->right = insert(node->right, key, value);
    }
    else {
        node->value = value;
    }

    return node;
}

template<typename _KTy, typename _VTy>
inline typename map<_KTy, _VTy>::Node* map<_KTy, _VTy>::find(Node* node, const _KTy& key) const {
    if (node == nullptr || node->key == key) {
        return node;
    }

    if (key < node->key) {
        return find(node->left, key);
    }
    else {
        return find(node->right, key);
    }
}

template<typename _KTy, typename _VTy>
inline typename map<_KTy, _VTy>::Node* map<_KTy, _VTy>::erase(Node* node, const _KTy& key) {
    if (node == nullptr) {
        return nullptr;
    }

    if (key < node->key) {
        node->left = erase(node->left, key);
    }
    else if (key > node->key) {
        node->right = erase(node->right, key);
    }
    else {
        if (node->left == nullptr) {
            Node* rightNode = node->right;
            free(node);
            return rightNode;
        }
        else if (node->right == nullptr) {
            Node* leftNode = node->left;
            free(node);
            return leftNode;
        }
        else {
            Node* minLargerNode = findMin(node->right);
            node->key = minLargerNode->key;
            node->value = minLargerNode->value;
            node->right = erase(node->right, minLargerNode->key);
        }
    }

    return node;
}

template<typename _KTy, typename _VTy>
inline typename map<_KTy, _VTy>::Node* map<_KTy, _VTy>::findMin(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template<typename _KTy, typename _VTy>
inline void map<_KTy, _VTy>::clear(Node* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        free(node);
    }
}

template<typename _KTy, typename _VTy>
template<typename Func>
inline void map<_KTy, _VTy>::inOrderTraversal(Node* node, Func func) const {
    if (node != nullptr) {
        inOrderTraversal(node->left, func);
        func(node->key, node->value);
        inOrderTraversal(node->right, func);
    }
}

template<typename _KTy, typename _VTy>
inline void map<_KTy, _VTy>::insert(const _KTy& key, const _VTy& value) {
    root = insert(root, key, value);
}

template<typename _KTy, typename _VTy>
inline _VTy& map<_KTy, _VTy>::operator[](const _KTy& key) {
    Node* node = find(root, key);
    if (node == nullptr) {
        root = insert(root, key, _VTy());
        node = find(root, key);
    }
    return node->value;
}

template<typename _KTy, typename _VTy>
inline _VTy* map<_KTy, _VTy>::find(const _KTy& key) const {
    Node* node = find(root, key);
    if (node != nullptr) {
        return &(node->value);
    }
    return nullptr;
}

template<typename _KTy, typename _VTy>
inline void map<_KTy, _VTy>::erase(const _KTy& key) {
    root = erase(root, key);
}

template<typename _KTy, typename _VTy>
inline bool map<_KTy, _VTy>::has(const _KTy& key) const {
    return find(root, key) != nullptr;
}

template<typename _KTy, typename _VTy>
template<typename Func>
inline void map<_KTy, _VTy>::forEach(Func func) const {
    inOrderTraversal(root, func);
}

template<typename _KTy, typename _VTy>
inline size_t map<_KTy, _VTy>::size(Node* node) const {
    if (node == nullptr) {
        return 0;
    }
    return 1 + size(node->left) + size(node->right);
}

template<typename _KTy, typename _VTy>
inline size_t map<_KTy, _VTy>::size() const {
    return size(root);
}

#endif // _MAP_