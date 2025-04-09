#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void rotateLeft(AVLNode<Key, Value>* node);
    void rotateRight(AVLNode<Key, Value>* node);
    void rotateLeftRight(AVLNode<Key, Value>* node);
    void rotateRightLeft(AVLNode<Key, Value>* node);
    void rebalance(AVLNode<Key, Value>* node);
    int height(AVLNode<Key, Value>* node) const;


};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        return;
    }
    
    AVLNode<Key, Value>* current = static_cast<AVLNode<Key, Value>*>(this->root_);
    while(true) {
        if(new_item.first < current->getKey()) {
            if(current->getLeft() == NULL) {
                current->setLeft(new AVLNode<Key, Value>(new_item.first, new_item.second, current));
                rebalance(current);
                return;
            }
            current = current->getLeft();
        } else if(new_item.first > current->getKey()) {
            if(current->getRight() == NULL) {
                current->setRight(new AVLNode<Key, Value>(new_item.first, new_item.second, current));
                rebalance(current);
                return;
            }
            current = current->getRight();
        } else {
            current->setValue(new_item.second);
            return;
        }
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* nodeToRemove = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if (nodeToRemove == NULL)
        return;

    if (nodeToRemove->getLeft() != NULL && nodeToRemove->getRight() != NULL) {
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(nodeToRemove));
        nodeSwap(nodeToRemove, pred); 
        nodeToRemove = pred;  
    }

    AVLNode<Key, Value>* child = (nodeToRemove->getLeft() != NULL) ? nodeToRemove->getLeft() : nodeToRemove->getRight();
    AVLNode<Key, Value>* parent = nodeToRemove->getParent();

    if (child != NULL)
        child->setParent(parent);

    
    if (parent == NULL) {
        this->root_ = child;  
    } else if (parent->getLeft() == nodeToRemove) {
        parent->setLeft(child);
    } else {
        parent->setRight(child);
    }

    delete nodeToRemove; 

    rebalance(parent);


}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}
template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* newParent = node->getRight();
    node->setRight(newParent->getLeft());
    if (newParent->getLeft() != NULL) {
        newParent->getLeft()->setParent(node);
    }
    newParent->setParent(node->getParent());
    if (node->getParent() == NULL) {
        this->root_ = newParent;
    } else if (node == node->getParent()->getLeft()) {
        node->getParent()->setLeft(newParent);
    } else {
        node->getParent()->setRight(newParent);
    }
    newParent->setLeft(node);
    node->setParent(newParent);

    node->updateBalance(1);
    newParent->updateBalance(-1);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* node)
{
    AVLNode<Key, Value>* newParent = node->getLeft();
    node->setLeft(newParent->getRight());
    if (newParent->getRight() != NULL) {
        newParent->getRight()->setParent(node);
    }
    newParent->setParent(node->getParent());
    if (node->getParent() == NULL) {
        this->root_ = newParent;
    } else if (node == node->getParent()->getRight()) {
        node->getParent()->setRight(newParent);
    } else {
        node->getParent()->setLeft(newParent);
    }
    newParent->setRight(node);
    node->setParent(newParent);

    node->updateBalance(-1);
    newParent->updateBalance(1);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeftRight(AVLNode<Key, Value>* node)
{
    rotateLeft(node->getLeft());
    rotateRight(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRightLeft(AVLNode<Key, Value>* node)
{
    rotateRight(node->getRight());
    rotateLeft(node);
}

template<class Key, class Value>
void AVLTree<Key, Value>::rebalance(AVLNode<Key, Value>* node)
{
    while (node != NULL) {
        int balance = height(node->getLeft()) - height(node->getRight());
        node->setBalance(balance);

        if (node->getBalance() == 2) {
            if (height(node->getLeft()->getLeft()) >= height(node->getLeft()->getRight())) {
                rotateRight(node);
            } else {
                rotateLeftRight(node);
            }
        } else if (node->getBalance() == -2) {
            if (height(node->getRight()->getRight()) >= height(node->getRight()->getLeft())) {
                rotateLeft(node);
            } else {
                rotateRightLeft(node);
            }
        }

        node = node->getParent();
    }
}

template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* node) const
{
    if (node == NULL) {
        return 0;
    }
    return 1 + std::max(height(node->getLeft()), height(node->getRight()));
}


#endif
