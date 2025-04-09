#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here

int checkPaths(Node *root, int depth)
{
    
    if (root->left == nullptr && root->right == nullptr)
        return depth;
    
    if (root->left != nullptr && root->right != nullptr)
    {
        int leftDepth = checkPaths(root->left, depth + 1);
        int rightDepth = checkPaths(root->right, depth + 1);
        if (leftDepth == -1 || rightDepth == -1 || leftDepth != rightDepth)
            return -1;
        return leftDepth;
    }

    if (root->left != nullptr)
        return checkPaths(root->left, depth + 1);
    return checkPaths(root->right, depth + 1);
}

bool equalPaths(Node * root)
{
    // Add your code below

    if (root == nullptr)
        return true;
    
    return checkPaths(root, 0) != -1;

}

