#include "AVLTree.h"

#include <string>


AVLTree::AVLNode::AVLNode(KeyType key, ValueType value) {
    this->key = key;
    this->value = value;
    this->height = 0;
    this->left = nullptr;
    this->right = nullptr;
}

size_t AVLTree::AVLNode::numChildren() const {
    if ((this->left == nullptr) and (this->right == nullptr)) {
        return 0;
    }
    else if(((this->left == nullptr) and (this->right != nullptr)) or
            ((this->left != nullptr) and (this->right == nullptr))){
        return 1;
    }
    else {
        return 2;
    }
}

bool AVLTree::AVLNode::isLeaf() const {
    if (this->numChildren() == 0) {
        return true;
    }
    else {
        return false;
    }
}

size_t AVLTree::AVLNode::getHeight() const {
    return this->height;
}



AVLTree::AVLTree() {
    root = nullptr;
}


bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();
    if (current->isLeaf()) {
        // case 1 we can delete the node
        current = nullptr;
    } else if (nChildren == 1) {
        // case 2 - replace current with its only child
        if (current->right) {
            current = current->right;
        } else {
            current = current->left;
        }
    } else {
        // case 3 - we have two children,
        // get smallest key in right subtree by
        // getting right child and go left until left is null
        AVLNode* smallestInRight = current->right;
        // I could check if smallestInRight is null,
        // but it shouldn't be since the node has two children
        while (smallestInRight->left) {
            smallestInRight = smallestInRight->left;
        }
        std::string newKey = smallestInRight->key;
        int newValue = smallestInRight->value;
        remove(root, smallestInRight->key); // delete this one

        current->key = newKey;
        current->value = newValue;

        current->height = current->getHeight();
        balanceNode(current);

        return true; // we already deleted the one we needed to so return
    }
    delete toDelete;

    return true;
}

bool AVLTree::remove(AVLNode *&current, KeyType key) {
    return false;
}

// Insert a new key-value pair into the tree. After a sucessful insert, the tree is rebalanced if necessary.
// Duplicate keys are disallowed. The insert() method should return true if the insertion was
// successful, false otherwise. If the insertion was unsuccessful, such as when a duplicate is
// attempted to be inserted, the method should return false.
// The time complexity for insert must be 𝒪︀ (log2 𝑛).
bool AVLTree::insert(const std::string& key, size_t value){
    //Key is in tree already
    if (getNodePlace(key, this->root) != nullptr) {
        return false;
    }

    //Find where the node should be inserted. Recusivley looks for lowest level
    insertNode(key, value, this->root);
    return true;
}

AVLTree::AVLNode* AVLTree::getNodePlace(const std::string& key, AVLTree::AVLNode* curNode) {
    if (curNode == nullptr) {
        return curNode;
    }
    else if (curNode->key == key) {
        return curNode;
    }
    else if (curNode->key < key) {
        return getNodePlace(key, curNode->right);
    }
    else {
        return getNodePlace(key, curNode->left);
    }
}

bool AVLTree::insertNode(const std::string& key, size_t value, AVLTree::AVLNode* curNode) {
    if (curNode == nullptr) {
        curNode = new AVLNode(key, value);

        //Node is root
        if (this->root == nullptr) {
            this->root = curNode;
        }

        return true;
    }

    if (curNode->key < key) {
        if (curNode->right == nullptr) {
            curNode->right = new AVLNode(key, value);
            if (curNode->getHeight() == 0) {
                curNode->height = 1;
            }
            return true;
        }
        else {
            if (!insertNode(key, value, curNode->right)) {
                return false;
            }
        }
    }
    else if (curNode->key > key) {
        if (curNode->left == nullptr) {
            curNode->left = new AVLNode(key, value);
            if (curNode->getHeight() == 0) {
                curNode->height = 1;
            }
            return true;
        }
        else {
            if (!insertNode(key, value, curNode->left)) {
                return false;
            }
        }
    }
    else if (curNode->key == key) {
        return false;
    }

    curNode->height++;
    balanceNode(curNode);
    return true;
}

void AVLTree::balanceNode(AVLNode *&node) {
    int balance = node->left->getHeight() - node->right->getHeight();
    if ((balance > 1) or (balance < -1)) {
        
    }
}
