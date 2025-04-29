#include "RedBlackTree.h"
#include <stdexcept>  
#include <string> 


RedBlackTree::RedBlackTree() {
    root = nullptr;
    numItems = 0;
}

RedBlackTree::RedBlackTree(int newData) {
    root = new RBTNode();
    root->data = newData;
    root->color = COLOR_BLACK; 
    root->parent = nullptr;
    numItems = 1;
}

// Inserts a node into the tree; may recolor and rotate.
void RedBlackTree::Insert(int newData){
    int numItems = 0;
    RBTNode *node = new RBTNode();
    node -> data = newData;
    node -> color = COLOR_RED;

    // first insert: if tree empty
    if(root == nullptr){
        node -> color = COLOR_BLACK; 
        root = node;
        numItems++;
        return;
    }
    else{
        BasicInsert(node);
        // fix red–red violations
        if (node->parent != nullptr && node -> parent->color == COLOR_RED) {
            InsertFixUp(node);
        }
    }
    numItems++;
}

// ToInfixString: return “(left subtree) node (right subtree)”
// in-order traversal, with color tags.
string RedBlackTree::ToInfixString(const RBTNode *n){
    if(n == nullptr){
        return "";
    }
    string result = "";

    result += ToInfixString(n->left);
    result += " " + GetColorString(n) + to_string(n->data) + " ";
    result += ToInfixString(n->right);

    return result;

}

// ToPrefixString: root first, then left, then right
// pre-order traversal with colors.
string RedBlackTree::ToPrefixString(const RBTNode *n) {
    if (n == nullptr) {
        return ""; 
    }
    string result = " ";
    result += GetColorString(n) + to_string(n->data) + " ";
    result += ToPrefixString(n->left);
    result += ToPrefixString(n->right);
    return result;
}

// ToPostfixString: left, right, root
// post-order traversal, color+value.
string RedBlackTree::ToPostfixString(const RBTNode *n) {
    if (n == nullptr){
        return "";
    }
    string result = "";
    result += ToPostfixString(n->left);
    result += ToPostfixString(n->right);
    
    result += " " + GetColorString(n) + to_string(n->data) + " ";
    return result;
}



// GetColorString: “R” if node is red, “B” otherwise (null counts as black)
string RedBlackTree::GetColorString(const RBTNode *n){
    if (n == nullptr) {
        return "B"; 
    }
    if (n -> color == COLOR_RED) {
        return "R";
    } else {
        return "B";
    }
}

void RedBlackTree::BasicInsert(RBTNode *node){
    RBTNode *current = root;
    RBTNode *parent = nullptr;

    while (current != nullptr) {
        parent = current;
        if (node -> data < current -> data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    if (parent == nullptr) {
        root = node;
    }
    else if (node->data < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }
    node->parent = parent;
}

bool RedBlackTree::IsLeftChild(RBTNode *node) const {
    return (node->parent != nullptr && node->parent->left == node);
}

bool RedBlackTree::IsRightChild(RBTNode *node) const {
    return (node->parent != nullptr && node->parent->right == node);
}

RBTNode* RedBlackTree::GetUncle(RBTNode *node)const{
    RBTNode *grand_parent = node->parent->parent;
    if (node == nullptr || node->parent == nullptr || node->parent->parent == nullptr) {
        return nullptr; 
    }
    if (IsLeftChild(node->parent)) {
        return grand_parent->right;
    } else {
        return grand_parent->left;
    }
}

void RedBlackTree::LeftRotate(RBTNode *node) {
    RBTNode *parent = node->parent;
    RBTNode *rChild = node -> right;
    RBTNode *rlGranChild = rChild -> left;
    // Find parent, rChild, rlGranChild
    //e.g., RBTNode *parent = n->parent;
    //set parent and child nodes of these three nodes
    if (node == nullptr || node->right == nullptr) {
        return; 
    }

    node->right = rlGranChild;
    if (rlGranChild != nullptr) {
        rlGranChild->parent = node;
    }

    rChild->left = node;
    rChild->parent = parent;
    node->parent = rChild;
    if (parent == nullptr) {
        root = rChild;
    } if (parent->left == node) {
        parent->left = rChild;
    } else {
        parent->right = rChild;
    }
}

void RedBlackTree::RightRotate(RBTNode *node){
    RBTNode *parent = node->parent;
    RBTNode *lChild = node -> left;
    RBTNode *lrGranChild = lChild -> right;
    if (node == nullptr || node->left == nullptr) {
        return; 
    }
    node->left = lrGranChild;
    if (lrGranChild != nullptr) {
        lrGranChild->parent = node;
    }
    lChild->right = node;
    lChild->parent = parent;
    node->parent = lChild;
    if (parent == nullptr) {
        root = lChild;
    } else if (parent->left == node) {
        parent->left = lChild;
    } else {
        parent->right = lChild;
    }
}
    
// InsertFixUp: enforce red-black rules after Insert
// 1: uncle is red, recolor parent & uncle black, grandparent red, recurse
// 2: uncle is black, rotate & recolor depending on shape
void RedBlackTree::InsertFixUp(RBTNode *node){
    RBTNode *parent = node -> parent;
    RBTNode *uncle = GetUncle(node);
    RBTNode *grand_parent = parent -> parent;
    while (node != root && node->parent->color == COLOR_RED) {
            if (uncle == nullptr || uncle->color == COLOR_BLACK){
                if(grand_parent != nullptr){
                    grand_parent -> color = COLOR_RED;
                }
                if (IsLeftChild(node) && IsLeftChild(parent)) {
                    // Left-Left case
                    RightRotate(grand_parent);
                    parent->color = COLOR_BLACK;
                } 
                else if (IsRightChild(node) && IsRightChild(parent)) {
                    // Right-Right case
                    LeftRotate(grand_parent);
                    parent->color = COLOR_BLACK;
                }
                else if (IsLeftChild(node) && IsRightChild(parent)) {
                    // Left-Right case
                    RightRotate(parent);
                    LeftRotate(grand_parent);
                    node->color = COLOR_BLACK;
                    parent->color = COLOR_RED;
                } 
                else if (IsRightChild(node) && IsLeftChild(parent)) {
                    // Right-Left case
                    LeftRotate(parent);
                    RightRotate(grand_parent);
                    node->color = COLOR_BLACK;
                    parent->color = COLOR_RED;
                } 
                else {
                    throw invalid_argument("impossible state!");
                }
            }
            else if (uncle != nullptr && uncle->color == COLOR_RED) {
                parent->color = COLOR_BLACK;
                uncle->color = COLOR_BLACK;
                if (grand_parent != nullptr) {
                    if (root != grand_parent) {
                        grand_parent->color = COLOR_RED;
                        if (grand_parent->parent != nullptr) {
                            if (grand_parent->parent->color == COLOR_RED) {
                                InsertFixUp(grand_parent);
                            }
                        }
                    }
                }
            }
        }
    root->color = COLOR_BLACK;
}

//Recursively copy the root and all vertices
RedBlackTree::RedBlackTree(const RedBlackTree &rbt) {
    root = nullptr;
    numItems = 0;
    if (rbt.root != nullptr) {
        root = CopyOf(rbt.root); 
    }
}

RBTNode* RedBlackTree::CopyOf(const RBTNode *node) {
    if (node == nullptr) {
        return nullptr;
    }

    //Create a new node and copy data and color
    RBTNode *newNode = new RBTNode();
    newNode->data = node->data;
    newNode->color = node->color;
    newNode->IsNullNode = node->IsNullNode;
    numItems++; 
    //Recursively copy left and right subtrees, and fix parent pointer.
    newNode->left = CopyOf(node->left);
    if (newNode->left != nullptr) {
        newNode->left->parent = newNode; 
    }
    newNode->right = CopyOf(node->right);
    if (newNode->right != nullptr) {
        newNode->right->parent = newNode; 
    }
    return newNode;
}

bool RedBlackTree::Contains(int data) const {
    RBTNode *current = root;
    while (current != nullptr) {
        if (data == current->data) {
            return true; 
        } else if (data < current->data) {
            current = current->left; 
        } else {
            current = current->right;
        }
    }
    return false;
}



