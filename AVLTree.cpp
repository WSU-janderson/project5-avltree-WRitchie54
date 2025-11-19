#include "AVLTree.h"

#include <string>

/*
 * AvlNode constructor - sets parent, key and value to paramater values
 *
 *      params
 *          key - the key for the node
 *          value - the value stored in the node
 *          parent - the node appearing above this one in the tree
 *
 */
AVLTree::AVLNode::AVLNode(std::string key, size_t value, AVLNode* parent) {
    this->key = key;
    this->value = value;
    this->height = 0;
    this->parent = parent;
    this->left = nullptr;
    this->right = nullptr;
}

/*
 * AvlNode constructor - sets parent, key and value to paramater values
 *
 *      params
 *          other - node to get key, value and height information from
 *          parent - the node appearing above this one in the tree
 *
 */
AVLTree::AVLNode::AVLNode(AVLNode &other, AVLNode *parent) {
    this->key = other.key;
    this->value = other.value;
    this->height = other.height;
    this->parent = parent;
    this->left = nullptr;
    this->right = nullptr;
}

/*
 *  numChildren: returns the number of children the node has
 *
 *  returns 0, 1 or 2 depending on the number of children present
 *
 */
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

/*
 *  getBalance - returns the balance of the node
 *
 *      returns the balance if it is outside of the range -1-1 then balancing needs to be done
 *
 */
int AVLTree::AVLNode::getBalance() const {
    if ((this->left == nullptr) and (this->right != nullptr)) {
        //assuming -1 if no left node
        return (-1) - this->right->getHeight();
    }
    else if ((this->left != nullptr) and (this->right == nullptr)) {
        //assuming -1 in place of having no right node
        return this->left->getHeight() - (-1);
    }
    else if ((this->left != nullptr) and (this->right != nullptr)) {
        return this->left->getHeight() - this->right->getHeight();
    }
    else {
        return 0;
    }
}

/*
 * isLeaf - returns if the node if at the extents of the tree
 *
 *  return true if node is an edge of the tree
 */
bool AVLTree::AVLNode::isLeaf() const {
    if (this->numChildren() == 0) {
        return true;
    }
    else {
        return false;
    }
}

/*
 * getHeight - returns height of the node which is the number of traversals necessarry to get to bottom of furthest leaf
 *
 *    returns
 *      height of current node within tree
 */
size_t AVLTree::AVLNode::getHeight() const {
    if (this != nullptr) {
        return this->height;
    }
    else {
        return 0;
    }

}

/*
 *  default AVLTree constructor - sets treeSize to zero and root to nullptr
 */
AVLTree::AVLTree() {
    this->root = nullptr;
    this->treeSize = 0;
}

/*
 * getHeight - returns the height of the root object to know the overall height of the tree
 *
 *  returns root node height
 *
 */
size_t AVLTree::getHeight() const {
    return this->root->getHeight();
}

/*
 *  size - returns the number of key-value pairs within the tree
 *
 *      returns treeSize a variable used to know how many keyValue pairs are in the tree
 */
size_t AVLTree::size() const {
    return this->treeSize;
}

/*
 *  findRange - returns a vector containing all values whose key falls within the range lowKey < key < highKey
 *
 *  params
 *      lowKey - reference to low key value
 *      highKey = reference to high key value
 *
 *   returns
 *      vector list of all values in key range
 */
vector<std::string> AVLTree::findRange(const std::string &lowKey, const std::string &highKey) const {
    AVLNode* node = this->root;
    vector<std::string> returnVector;
    //Recursive function used to add all keys to return vector
    rangeHelper(lowKey, highKey, returnVector, this->root);

    return returnVector;
}

/*
 * rangeHelper - recursive function that adds all nodes with key values in range into a vector
 *
 *  params
 *      lowKey - low range
 *      highKey - high range
 *      returnVector - vector values are being added to
 *      curNode - the current node being looked at in the list
 *
 *   returns false if node is not in range
 *
 */
bool AVLTree::rangeHelper(const std::string &lowKey, const std::string &highKey, vector<std::string>& returnVector, AVLTree::AVLNode* curNode) const {
    //bottom of tree
    if (curNode == nullptr) {
      return false;
    }

    //root case
    if (curNode == this->root) {
        //if root is not in range then can end early
        if ((curNode->key >= lowKey) or (curNode->key <= highKey)){
            returnVector.push_back(std::to_string(curNode->value));
        }
        else {
            return false;
        }
    }

    //Go through left side of node
    if (curNode->key >= lowKey) {
        if (rangeHelper(lowKey, highKey, returnVector, curNode->left)) {
            returnVector.push_back(std::to_string(curNode->left->value));
        }
    }

    //Go through right side of node
    if (curNode->key <= highKey) {
        if (rangeHelper(lowKey, highKey, returnVector, curNode->right)) {
            returnVector.push_back(std::to_string(curNode->right->value));
        }
    }


    //leftkey less than lowkey end
    if (curNode->key < lowKey) {
        return false;
    }
    //rightkey greater than right key end
    if (curNode->key > highKey) {
        return false;
    }

    //continue
    return true;
}

/*
 * operator[] - allows access to value given key value
 *
 * returns - size_t reference so that the value can be modified
 */
size_t& AVLTree::operator[](const std::string &key) {
    AVLNode* node = getNodePlace(key, this->root);
    if (node != nullptr) {
        return node->value;
    }
}

/*
 * keys - returns all values from tree into a vector
 *
 *  returns - vector containing all values
 *
 */
std::vector<std::string> AVLTree::keys() const {
    std::vector<std::string> returnVector;
    //recursive function that navigates tree
    keysHelper(this->root, returnVector);
    return returnVector;
}

/*
 *  keysHelper - recursive helper function used to recursivley go through list and add all node values to vector
 *
 *  params
 *      curNode - the current node being looked at in the tree
 *      returnVector - vector that is getting added to
 *
 *  returns - boolean false if done true if needing to continue
 *
 */
bool AVLTree::keysHelper(AVLNode* curNode, vector<std::string>& returnVector) const {
    //bottom of tree
    if (curNode == nullptr) {
        return false;
    }

    //root case
    if (curNode == this->root) {
        returnVector.push_back(std::to_string(curNode->value));
    }

    //add all right members of node
    if (curNode->right != nullptr) {
        if (keysHelper(curNode->right, returnVector)) {
            returnVector.push_back(std::to_string(curNode->right->value));
        }
    }

    //add all left members of node
    if (curNode->right != nullptr) {
        if (keysHelper(curNode->left, returnVector)) {
            returnVector.push_back(std::to_string(curNode->left->value));
        }
    }

    //continue
    return true;
}

/*
 *  removeNode - function containing logic needed when deleting nodes from tree. Need to check children amount and decide
 *      on delete logic from there.
 *
 *  params
 *      current - the current node being looked at in the tree
 *
 *  returns - boolean false if not deleted true if it is deleted
 *
 */
bool AVLTree::removeNode(AVLNode*& current){
    if (!current) {
        return false;
    }

    AVLNode* toDelete = current;
    auto nChildren = current->numChildren();

    //If bottom node just delete it
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

/*
 *  remove - recursive helper function used to recursivley go through list and find the node to remove
 *
 *  params
 *      current - the current node being looked at in the tree
 *      key  - key being searched for to remove
 *
 *  returns - boolean true if done false if needing to continue
 */
bool AVLTree::remove(AVLNode *&current, std::string key) {
    if (current->key == key) {
        //Remove node from list and return out of recursion
        removeNode(current);
        return true;
    }
    else if (current->key < key) {
        //Balancing needs checked if a node was removed
        if (remove(current->right, key)) {
            balanceNode(current);
        }
    }
    else if (current->key > key) {
        //Balancing needs checked if a node was removed
        if (remove(current->left, key)) {
            balanceNode(current);
        }

    }
    return false;
}

/*
 *  remove - remove base function takes a key and uses recursive helper function to find and remove node
 *
 *  params
 *      key  - key being searched for to remove
 *
 *  returns - boolean true if done false if failed
 */
bool AVLTree::remove(const std::string &key) {
    if (remove(this->root, key)) {
        return true;
    } else {
        return false;
    }
}

/*
 *  insert - Insert node based off key and value data. If already in list fail to add
 *
 *  params
 *      key  - key being used to insert
 *      value - the size_t value to store in the node
 *
 *  returns - boolean true if done false if failed
 */
bool AVLTree::insert(const std::string& key, size_t value){
    //Key is in tree already
    if (getNodePlace(key, this->root) != nullptr) {
        return false;
    }

    //Find where the node should be inserted. Recusivley looks for lowest level
    if (insertNode(key, value, this->root)) {
        //If succesful be sure to increase treeSize
        treeSize++;
        return true;
    }
}

/*
 *  contains - look to see if node is in tree already
 *
 *  params
 *      key  - key being searched for
 *
 *  returns - boolean true if done false if failed
 */
bool AVLTree::contains(const std::string &key) const {
    if (getNodePlace(key, this->root) != nullptr) {
        return true;
    }
    else {
        return false;
    }
}

/*
 *  get - look to see if node is in tree and if it is return value otherwise return nullopt
 *
 *  params
 *      key  - key being searched for
 *
 *  returns - optional<size_t> value of node if present otherwise null opt
 */
std::optional<size_t> AVLTree::get(const std::string& key) const{
    AVLNode *node = getNodePlace(key, this->root);

    //if node is nullptr then it is not in tree
    if (node != nullptr) {
        return node->value;
    }
    else {
        return nullopt;
    }
}

/*
 *  getNodePlace - recursive helper function used to recursivley go through list and find a node
 *
 *  params
 *      key  - key being searched for
 *      curNode - the node currently being used
 *
 *  returns - the pointer to the node found
 */
AVLTree::AVLNode* AVLTree::getNodePlace(const std::string& key, AVLTree::AVLNode* curNode) const{
    //Found bottom of list node is not present
    if (curNode == nullptr) {
        return curNode;
    }
    //Found node return out
    else if (curNode->key == key) {
        return curNode;
    }
    //Can still continue search through right node
    else if (curNode->key < key) {
        return getNodePlace(key, curNode->right);
    }
    //Can still continue search through left node
    else {
        return getNodePlace(key, curNode->left);
    }
}

/*
 *  insertNode - helper function used to find proper spot to insert node. Balance is checked for and done if needed after
 *
 *  params
 *      key  - key to insert
 *      value  - value to insert
 *      curNode  - current node being accessed
 *
 *  returns - boolean true if done false if not done
 */
bool AVLTree::insertNode(const std::string& key, size_t value, AVLTree::AVLNode* curNode) {
    //Bottom of tree insert
    if (curNode == nullptr) {
        curNode = new AVLNode(key, value, nullptr);

        //Node is root
        if (this->root == nullptr) {
            this->root = curNode;
        }

        return true;
    }

    //Go down right side of node to find where to insert
    if (curNode->key < key) {
        if (curNode->right == nullptr) {
            curNode->right = new AVLNode(key, value, curNode);
            if (curNode->getHeight() == 0) {
                curNode->height = 1;
            }
            return true;
        }
        else {
            //insert was not completed return failure
            if (!insertNode(key, value, curNode->right)) {
                return false;
            }
        }
    }
    //go down left side of node for insertion place
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
    //Key found return false
    else if (curNode->key == key) {
        return false;
    }

    //Set curNode height based off children height while exiting recursion
    if ((curNode->left != nullptr && (curNode->left->getHeight() == curNode->getHeight()))
        or (curNode->right != nullptr && (curNode->right->getHeight() == curNode->getHeight()))){
            curNode->height++;
        }

    //attempt to balance tree
    balanceNode(curNode);
    return true;
}

/*
 *  balanceNode - look at balance of node if 2 or -2 need to execute rotations around node
 *
 *  params
 *      node  - current node being accessed
 */
void AVLTree::balanceNode(AVLNode *&node) {
    int balance = node->getBalance();

    //need to balance node
    if (balance == -2) {
        //Look at right nodes balance
        AVLNode* rightNode = node->right;
        int rightNodeBalance = rightNode->getBalance();

        //right then left rotate
        if (rightNodeBalance == 1) {
            AVLNode* origRightLeft = rightNode->left;

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
            }   else {
                node->height = 0;
            }
        }
        //left rotate
        else if (rightNodeBalance == -1) {
            //Set parent of node to the right node and rotate the rightNodes left node around pivot node
            node->parent = rightNode;
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

            //set root nood properly if it has changed
            if (node == root) {
                root = rightNode;
                rightNode->parent = nullptr;
                rightNode->left = node;
                node->parent = rightNode;
            }
            else {
                rightNode->left = node;
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
            RightRotate(node);
        }
    }
}

void AVLTree::RightRotate(AVLNode* pivotNode) {
    AVLNode* leftNode = pivotNode->left;

    if (leftNode->right != nullptr) {
        pivotNode->left = leftNode->right;
    }
    else {
        pivotNode->left = nullptr;
    }

    //Set pivot nodes height to the greater of its two childrens heights + 1
    if (pivotNode->right->getHeight() > pivotNode->left->getHeight()) {
        pivotNode->height = pivotNode->right->getHeight() + 1;
    } else if (pivotNode->left->getHeight() > pivotNode->right->getHeight()){
        pivotNode->height = pivotNode->left->getHeight() + 1;
    } else {
        pivotNode->height = 0;
    }

    //adjust pivot and left nodes for new positions
    leftNode->right = pivotNode;
    if (pivotNode == this->root) {
        this->root = leftNode;
        leftNode->parent = nullptr;
    }
    else {
        leftNode->parent = pivotNode->parent;
        //set pivots previous parents left to left node
        pivotNode->parent->left = leftNode;
    }
    //Set pivot parent to left node
    pivotNode->parent = leftNode;
}

AVLTree::AVLTree(const AVLTree &other) {
    if (other.root != nullptr) {
        this->root = new AVLNode(*other.root, nullptr);
        copyHelper(other.root, this->root);
        treeSize = other.treeSize;
    }
}

bool AVLTree::copyHelper(AVLNode* curNodeOld, AVLNode* curNode) const{
    if (curNodeOld->getHeight() == 0) {
        return false;
    }

    if (curNodeOld->left != nullptr) {
        curNode->left = new AVLNode(*curNodeOld->left);
        copyHelper(curNodeOld->left, curNode->left);
    }

    if (curNodeOld->right != nullptr) {
        curNode->right = new AVLNode(*curNodeOld->right);
        copyHelper(curNodeOld->right, curNode->right);
    }

    return true;
}

void AVLTree::operator=(const AVLTree &other) {
    deleteHelper(this->root);
    if (other.root != nullptr) {
        this->root = new AVLNode(*other.root, nullptr);
        copyHelper(other.root, this->root);
        this->treeSize = other.treeSize;
    }
}

AVLTree::~AVLTree() {
    deleteHelper(this->root);
}

bool AVLTree::deleteHelper(AVLNode* curNode) const{
    if (curNode == nullptr) {
        return false;
    }

    if (curNode->left != nullptr) {
        if (deleteHelper(curNode->left)) {
            curNode->left = nullptr;
        }
    }

    if (curNode->right != nullptr) {
        if (deleteHelper(curNode->right)) {
            curNode->right = nullptr;
        }
    }

    delete curNode;
    curNode = nullptr;

    return true;
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
    }
    return os;
}




