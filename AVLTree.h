/**
 * AVLTree.h
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <string>
#include <vector>
#include <ostream>
#include <optional>

using namespace std;

class AVLTree {
public:
    using KeyType = std::string;
    using ValueType = size_t;
    bool insert(const std::string& key, size_t value);
    bool contains(const std::string& key) const;
    std::optional<size_t> get(const std::string& key) const;
    // size_t& operator[](const std::string& key);
    // vector<std::string> findRange( const std::string& lowKey, const std::string& highKey) const;
    // std::vector<std::string> keys() const;
    size_t size() const;
    size_t getHeight() const;
    // AVLTree(const AVLTree& other);
    AVLTree();
    // void operator=(const AVLTree& other);
    // ~AVLTree();



protected:
    class AVLNode {
    public:
        AVLNode(KeyType key, ValueType value, AVLNode* parent);

        KeyType key;
        ValueType value;
        size_t height;

        AVLNode* left;
        AVLNode* right;
        AVLNode* parent;

        // 0, 1 or 2
        size_t numChildren() const;
        // true or false
        bool isLeaf() const;
        // number of hops to deepest leaf node
        size_t getHeight() const;
        int getBalance() const;
    };

public:

    private:
    AVLNode* root;
    size_t treeSize;
    AVLNode* getNodePlace(const std::string& key, AVLNode* curNode) const;
    bool insertNode(const std::string& key, size_t value, AVLNode* curNode);

    friend bool printRightSide(AVLNode* node, int depth, ostream& os);
    friend bool printLeftSide(AVLNode* node, int depth, ostream& os);
    friend std::ostream& operator<<(ostream& os, const AVLTree & avlTree);
    /* Helper methods for remove */
    // this overloaded remove will do the recursion to remove the node
    bool remove(AVLNode*& current, KeyType key);
    // removeNode contains the logic for actually removing a node based on the numebr of children
    bool removeNode(AVLNode*& current);
    // You will implement this, but it is needed for removeNode()
    void balanceNode(AVLNode*& node);


};

#endif //AVLTREE_H
