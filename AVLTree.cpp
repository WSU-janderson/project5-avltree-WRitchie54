#include "AVLTree.h"

#include <string>


AVLTree::AVLNode::AVLNode(std::string key, size_t value, AVLNode* parent) {
    this->key = key;
    this->value = value;
    this->height = 0;
    this->parent = parent;
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

int AVLTree::AVLNode::getBalance() const {
    if ((this->left == nullptr) and (this->right != nullptr)) {
        return (-1) - this->right->getHeight();
    }
    else if ((this->left != nullptr) and (this->right == nullptr)) {
        return this->left->getHeight() - (-1);
    }
    else if ((this->left != nullptr) and (this->right != nullptr)) {
        return this->left->getHeight() - this->right->getHeight();
    }
    else {
        return 0;
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
    if (this != nullptr) {
        return this->height;
    }
    else {
        return 0;
    }

}



AVLTree::AVLTree() {
    this->root = nullptr;
    this->treeSize = 0;
}


size_t AVLTree::getHeight() const {
    return this->root->getHeight();
}

size_t AVLTree::size() const {
    return this->treeSize;
}

vector<std::string> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) const {
    AVLNode* node = this->root;
    vector<std::string> returnVector;
    rangeHelper(lowKey, highKey, returnVector, this->root);

    return returnVector;
}

bool AVLTree::rangeHelper(const std::string &lowKey, const std::string &highKey, vector<std::string>& returnVector, AVLTree::AVLNode* curNode) const {
    //bottom of tree
    if (curNode == nullptr) {
      return false;
    }

    if (curNode == this->root) {
        if ((curNode->key >= lowKey) or (curNode->key <= highKey)){
            returnVector.push_back(std::to_string(curNode->value));
        }
        else {
            return false;
        }
    }

    if (curNode->key >= lowKey) {
        if (rangeHelper(lowKey, highKey, returnVector, curNode->left)) {
            returnVector.push_back(std::to_string(curNode->left->value));
        }
    }

    if (curNode->key <= highKey) {
        if (rangeHelper(lowKey, highKey, returnVector, curNode->right)) {
            returnVector.push_back(std::to_string(curNode->right->value));
        }
    }


    //leftkey less than lowkey
    if (curNode->key < lowKey) {
        return false;
    }
    //rightkey greater than right key
    if (curNode->key > highKey) {
        return false;
    }

    return true;
}

size_t& AVLTree::operator[](const std::string &key) {
    AVLNode* node = getNodePlace(key, this->root);
    if (node != nullptr) {
        return node->value;
    }
}

std::vector<std::string> AVLTree::keys() const {
    std::vector<std::string> returnVector;
    keysHelper(this->root, returnVector);
    return returnVector;
}

bool AVLTree::keysHelper(AVLNode* curNode, vector<std::string>& returnVector) const {
    //bottom of tree
    if (curNode == nullptr) {
        return false;
    }

    if (curNode == this->root) {
        returnVector.push_back(std::to_string(curNode->value));
    }

    if (curNode->right != nullptr) {
        if (keysHelper(curNode->right, returnVector)) {
            returnVector.push_back(std::to_string(curNode->right->value));
        }
    }

    if (curNode->right != nullptr) {
        if (keysHelper(curNode->left, returnVector)) {
            returnVector.push_back(std::to_string(curNode->left->value));
        }
    }
    return true;
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
            current->right->parent = current->parent;
            current = current->right;
        } else {
            current->left->parent = current->parent;
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

bool AVLTree::remove(AVLNode *&current, std::string key) {
    if (current->key == key) {
        AVLNode nodeSave = *current;
        removeNode(current);
        // balanceNode(nodeSave.parent);
        return true;
    }
    else if (current->key < key) {
        if (remove(current->right, key)) {
            balanceNode(current);
        }
    }
    else if (current->key > key) {
        if (remove(current->left, key)) {
            balanceNode(current);
        }

    }
    return false;
}

bool AVLTree::remove(const std::string &key) {
    if (remove(this->root, key)) {
        return true;
    } else {

        return false;
    }
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
    if (insertNode(key, value, this->root)) {
        treeSize++;
        return true;
    }
}

bool AVLTree::contains(const std::string &key) const {
    if (getNodePlace(key, this->root) != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

std::optional<size_t> AVLTree::get(const std::string& key) const{
    AVLNode *node = getNodePlace(key, this->root);
    if (node != nullptr) {
        return node->value;
    }
    else {
        return nullopt;
    }
}



AVLTree::AVLNode* AVLTree::getNodePlace(const std::string& key, AVLTree::AVLNode* curNode) const{
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
        curNode = new AVLNode(key, value, nullptr);

        //Node is root
        if (this->root == nullptr) {
            this->root = curNode;
        }

        return true;
    }

    if (curNode->key < key) {
        if (curNode->right == nullptr) {
            curNode->right = new AVLNode(key, value, curNode);
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
            curNode->left = new AVLNode(key, value, curNode);
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

    if ((curNode->left != nullptr && (curNode->left->getHeight() == curNode->getHeight()))
        or (curNode->right != nullptr && (curNode->right->getHeight() == curNode->getHeight()))){
            curNode->height++;
        }

    balanceNode(curNode);
    return true;
}

void AVLTree::balanceNode(AVLNode *&node) {
    int balance = node->getBalance();

    //need to balance node
    if ((balance > 1) or (balance < -1)) {
        if (balance == -2) {
            AVLNode* rightNode = node->right;
            int rightNodeBalance = rightNode->getBalance();

            //right then left rotate
            if (rightNodeBalance == 1) {
            AVLNode* origRightLeft = rightNode->left;

            //Set node to bottom right of current tree segment
            if ((node->left != nullptr) and (node->left->right != nullptr)) {
                node->right = node->left->right;
            }
            else {
                node->right = nullptr;
            }

            //Set node height to the greater of its two childrens heights + 1
            if (node->right->getHeight() > node->left->getHeight()) {
                node->height = node->right->getHeight() + 1;
            } else {
                node->height = node->left->getHeight() + 1;
            }

            // node->right = nullptr;
            // node->height = 0;
            rightNode->left = origRightLeft->right;
            if (rightNode->left != nullptr) {
                rightNode->left->parent = rightNode;
            }

            //Set orig right lefts height to the greater of its two childrens heights + 1
            if (rightNode->right->getHeight() > rightNode->left->getHeight()) {
                rightNode->height = rightNode->right->getHeight() + 1;
            } else {
                rightNode->height = rightNode->left->getHeight() + 1;
            }

            origRightLeft->right = rightNode;
            origRightLeft->left = node;
            if (node == root) {
                root = origRightLeft;
                origRightLeft->parent = nullptr;
                node->parent = origRightLeft;
            }
            else {
                origRightLeft->parent = node->parent;
                node->parent->right = origRightLeft;
                node->parent = origRightLeft;
            }

            //Set orig right lefts height to the greater of its two childrens heights + 1
            if (origRightLeft->right->getHeight() > origRightLeft->left->getHeight()) {
                origRightLeft->height = origRightLeft->right->getHeight() + 1;
            } else  if (origRightLeft->left->getHeight() > origRightLeft->right->getHeight()){
                origRightLeft->height = origRightLeft->left->getHeight() + 1;
            }else {
                node->height = 0;
            }
        }
            //left rotate
            else if (rightNodeBalance == -1) {
                node->parent = node->right;
                node->right = rightNode->left;


                //Set node height to the greater of its two childrens heights + 1
                if (node->right->getHeight() > node->left->getHeight()) {
                    node->height = node->right->getHeight() + 1;
                } else if (node->left->getHeight() > node->right->getHeight()) {
                    node->height = node->left->getHeight() + 1;
                }
                else {
                    node->height = 0;
                }

                rightNode->left = node;
                if (node == root) {
                    root = rightNode;
                    rightNode->parent = nullptr;
                    rightNode->left = node;//////
                    node->parent = rightNode;
                }
                else {
                    rightNode->left = node;////
                    rightNode->parent = node->parent;
                    node->parent->left = rightNode;
                    node->parent = rightNode;
                }
            }
        }
        else if (balance == 2) {
            AVLNode* leftNode = node->left;
            int leftNodeBalance = leftNode->getBalance();

            //left then right rotate
            if (leftNodeBalance == -1) {
                AVLNode* origLeftRight = leftNode->right;

                //Set the current node to bottom left of tree branch
                if ((node->right != nullptr) and (node->right->left != nullptr) ){
                    node->left = node->right->left;
                }
                else {
                    node->left = nullptr;
                }
                node->height = node->right->getHeight() + 1;

                //Set previous left node to be rotated into place of origLeftRight
                leftNode->right = origLeftRight->left;

                if (leftNode->right != nullptr) {
                    leftNode->right->parent = leftNode;
                }

                //Set left nodes height to be 1 greater than the greateast height between its children
                if (leftNode->right->getHeight() > leftNode->left->getHeight()) {
                    leftNode->height = leftNode->right->getHeight() + 1;
                } else {
                    leftNode->height = leftNode->left->getHeight() + 1;
                }

                //OrigLeftRight should be in place of node as head of tree segment
                origLeftRight->left = leftNode;
                origLeftRight->right = node;

                if (node == root) {
                    root = origLeftRight;
                    origLeftRight->parent = nullptr;
                    node->parent = origLeftRight;
                }
                else {
                    origLeftRight->parent = node->parent;
                    node->parent->left = origLeftRight;
                    node->parent = origLeftRight;
                }

                //Set orig left rights height to the greater of its two childrens heights + 1
                if (origLeftRight->right->getHeight() > origLeftRight->left->getHeight()) {
                    origLeftRight->height = origLeftRight->right->getHeight() + 1;
                } else {
                    origLeftRight->height = origLeftRight->left->getHeight() + 1;
                }
            }
            //right rotate
            else if (leftNodeBalance == 1) {
                node->left = nullptr;
                node->height = 0;
                leftNode->right = node;
                if (node == root) {
                    root = leftNode;
                    leftNode->parent = nullptr;
                    node->parent = leftNode;
                }
                else {
                    leftNode->parent = node->parent;
                    node->parent->left = leftNode;
                    node->parent = leftNode;
                }
            }
        }
    }
}

/*
 *  printRightSide: recursive function to print everything to the right of a node including node itself
 *
 *  params
 *      node - pointer to node that will be printed along with its children
 *      depth - how deep into the tree we are for indentation purpouses
 *      os - stream to print to
 *
 *   returns - true if it printed to the right
 *
 */
bool printRightSide(AVLTree::AVLNode *node, int depth, ostream &os) {
    //bottom of tree
    if (node->right == nullptr) {
        for (int i = 0; i < depth; i++) {
            os << "\t";
        }
        os << "{" << node->key << ": " << node->value << "}" << std::endl;
        return true;
    }
    else {
        if (printRightSide(node->right, depth+1, os)) {
            //print lower nodes left node if available
            if (node->right->left != nullptr) {
                printRightSide(node->right->left, depth+2, os);
                // for (int i = 0; i < depth+2; i++) {
                //     os << "\t";
                // }
                // os << "{" << node->right->left->key << ": " << node->right->left->value << "}" << std::endl;
            }
            //print
            for (int i = 0; i < depth; i++) {
                os << "\t";
            }
            os << "{" << node->key << ": " << node->value << "}" << std::endl;
            return true;
        }
    }
    return false;
}

/*
 *  friend << operator - prints tree to ostream. It indents the lowest height members of the tree the farthest out and then prints highest height farthest to the left
 *      each member of the tree is seperated by a new line
 *
 *      Params: os - stream to output to
 *              avlTree- tree to print
 *
 *      returns - reference to ostream that can be used to output
 */
std::ostream& operator<<(ostream& os, const AVLTree & avlTree) {
    printRightSide(avlTree.root, 0, os);
    // os << "{" << avlTree.root->key << ": " << avlTree.root->value << "}" << std::endl;
    AVLTree::AVLNode* node = avlTree.root;
    int depth = 0;

    while (node->left != nullptr) {
        node = node->left;
        depth++;
        printRightSide(node, depth, os);
        // if (node->right != nullptr) and (node->left  {
        //     for (int i = 0; i<depth; i++) {
        //         os << "\t";
        //     }
        //     os << "{" << node->key << ": " << node->value << "}" << std::endl;
        // }
    }
    return os;
}




