#pragma once
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <iostream>


namespace cs251{
template <typename KT, typename VT>
class RBTree{
   public:
    /* The default constructor. */
    RBTree();
    /* The default destructor. You should make sure no memory leaks happens */
    ~RBTree();

    /* Insert a key-value pair to the rb tree.
     * If the pair already exists, use remove operation to remove the pair
     * and re-insert it. */
    void insert(const KT& key, const VT& value);

    /* If the key doesn’t exist, return false
     * Else, remove the key-value pair with the given key return true. */
    bool remove(const KT& key);

    /*  Find the key-value pair with the given key.
        Return the pointer to the value. 
        If the key pair doesn't exist, return nullptr */
    VT* find(const KT& key);

    /* Return the rank of the key when sorted in descending order.
      The index starts from 1.
      If the key doesn't exist, return 0 */
    size_t getRank(const KT& key);

    size_t getRankRoot();
 
    VT* getNodeByRank(size_t rank);

    struct TreeNode{
        const KT* key;
        const VT* value;
        TreeNode* left;
        TreeNode* right;
        TreeNode* parent;
        bool is_red;
        size_t size;
        TreeNode(const KT& key, const VT& value)
            : key(new KT(key)), value(new VT(value)), left(nullptr), right(nullptr), parent(nullptr), is_red(true), size(1) {}
        TreeNode()
        : key(nullptr), value(nullptr), left(nullptr), right(nullptr), parent(nullptr), is_red(false), size(1) {}
    };

    /*
        TODO: You may add public members/methods here.
    */


   private:
    TreeNode* root_;
    TreeNode* nil_;
    void leftRotate(TreeNode* x);
    void rightRotate(TreeNode* x);

    void transplant(TreeNode* u, TreeNode* v){
        if (u->parent == nil_){
            root_ = v;
        } else if (u == u->parent->left){
            u->parent->left = v;
        } else {
            u->parent->right = v;
        }
        v->parent = u->parent;
    };
    TreeNode* getMin(TreeNode* x) {
        while (x->left != nil_) {
            x = x->left;
        }
        return x;
    };
    TreeNode* getMax(TreeNode* x){
        while (x->right != nil_){
            x = x->right;
        }
        return x;
    };
    /* fix violation caused by removal. starting from x */
    void removeFixup(TreeNode* x);

    /* fix violation caused by insertion. starting from x */
    void insertFixup(TreeNode* x);
    
    void updateSize(TreeNode* x);

    TreeNode* findNode(const KT& key);

    void printHelper(TreeNode* x);
};

template <typename KT, typename VT>
RBTree<KT, VT>::RBTree(){

    nil_ = new TreeNode();
    nil_->is_red = false;
    nil_->left = nil_;
    nil_->right = nil_;
    nil_->size = 0;
    root_ = nil_;
}

template <typename KT, typename VT>
RBTree<KT, VT>::~RBTree(){
    std::stack<TreeNode*> s;
    s.push(root_);
    while (!s.empty()){
        TreeNode* x = s.top();
        s.pop();
        if (x->left != nil_){
            s.push(x->left);
        }
        if (x->right != nil_){
            s.push(x->right);
        }
        delete x->key;
        delete x->value;
        delete x;
    }
    delete nil_;
}
template<typename KT, typename VT>
VT* RBTree<KT, VT>::getNodeByRank(size_t rank) {
    if (rank == 0 || root_ == nil_ || rank > root_->size) return nullptr;

    TreeNode* x = root_;
    size_t currRank;

    while (x != nil_) {
        size_t rightSize = (x->right != nil_) ? x->right->size : 0;
        currRank = rightSize + 1;

        if (rank == currRank) {
            return const_cast<VT*>(x->value);
        } else if (rank < currRank) {
            x = x->right;  // go to larger values
        } else {
            rank -= currRank;
            x = x->left;   // go to smaller values
        }
    }
    return nullptr;  // not found (shouldn't happen if rank is valid)
}

template <typename KT, typename VT>
void RBTree<KT, VT>::leftRotate(TreeNode* x){
    TreeNode* y = x->right;
    x->right = y->left;
    if (y->left != nil_) 
        y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == nil_) root_ = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
    //y->size = x->size;
    
    TreeNode* cur1 = y->parent;
    while (cur1 != nil_) {
        updateSize(cur1);
        cur1 = cur1->parent;
    }
    TreeNode* cur2 = x->parent;
    while (cur2 != nil_) {
        updateSize(cur2);
        cur2 = cur2->parent;
    }
}

template <typename KT, typename VT>
void RBTree<KT, VT>::rightRotate(TreeNode* y){
    TreeNode* x = y->left;
    y->left = x->right; 
    if (x->right != nil_) 
        x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == nil_) root_ = x;
    else if (y == y->parent->right) y->parent->right = x;
    else y->parent->left = x;
    x->right = y;
    y->parent = x;
    //x->size = y->size;


    TreeNode* cur1 = x->parent;
    while (cur1 != nil_) {
        updateSize(cur1);
        cur1 = cur1->parent;
    }
    TreeNode* cur2 = y->parent;
    while (cur2 != nil_) {
        updateSize(cur2);
        cur2 = cur2->parent;
    }
}

template <typename KT, typename VT>
void RBTree<KT, VT>::updateSize(TreeNode* x) {
    if (x == nil_) return;
    x->size = 1;
    if (x->left != nil_) x->size += x->left->size;
    if (x->right != nil_) x->size += x->right->size;
}

template <typename KT, typename VT>
VT* RBTree<KT, VT>::find(const KT& key){
    TreeNode* node = findNode(key);
    return node ? const_cast<VT*>(node->value) : nullptr;
}


template <typename KT, typename VT>
typename RBTree<KT, VT>::TreeNode* RBTree<KT, VT>::findNode(const KT& key) {
    //printHelper(root_);
    TreeNode* x = root_;
    while (x != nil_) {
        if (key < *(x->key)) {
            x = x->left;
        } else if (key > *(x->key)) {
            x = x->right;
        } else {
            return x;
        }
    }
    return nullptr;
}

template <typename KT, typename VT>
bool RBTree<KT, VT>::remove(const KT& key){
    TreeNode* z = findNode(key);
    if (!z) return false;

    TreeNode* y = z;
    TreeNode* x;
    bool y_original_color = y->is_red;

    if (z->left == nil_) {
        x = z->right;
        transplant(z, z->right);
    } else if (z->right == nil_) {
        x = z->left;
        transplant(z, z->left);
    } else {
        y = getMin(z->right);
        y_original_color = y->is_red;
        x = y->right;
        if (y->parent == z) {
            //if (x != nil_) x->parent = y; => reason why i got 4 wrong !!!!!
            x->parent = y;
        } else {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->is_red = z->is_red;
        //for this case, update size of the z.right bc it lost one children node, which is the z's inorder succesor.
        // since y and z will switch place, then delete z, that's in line 234, change from x->parent to x.
        // => solved using code from 234-238 with cur = x->parent
    }

    //update size of the above nodes after insertion
    //delete z;
    TreeNode* cur = x->parent; //line 235 leads to this line not exist, meaning x->parent doesn't exist if y->right is null or x->parent != y
    while (cur != nil_) {
        updateSize(cur);
        cur = cur->parent;
    }
    delete z;
    if (!y_original_color) removeFixup(x);
    return true;
}

template <typename KT, typename VT>
void RBTree<KT, VT>::removeFixup(TreeNode* x){
    while (x != root_ && !x->is_red) {
        if (x == x->parent->left) { // node == parent.left
            TreeNode* w = x->parent->right; // sibling == parent.right
            if (w->is_red) {
                w->is_red = false;
                x->parent->is_red = true;
                leftRotate(x->parent);                
                w = x->parent->right; // update sibling
            }
            if (!w->left->is_red && !w->right->is_red) { // && vs ||
                w->is_red = true;
                x = x->parent;
            } else {
                if (!w->right->is_red) {
                    if (w->left != nil_) w->left->is_red = false;
                    w->is_red = true;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->is_red = x->parent->is_red;
                x->parent->is_red = false;
                if (w->right != nil_) w->right->is_red = false;
                leftRotate(x->parent);
                x = root_;
            }
        } else {                     // node == parent.right
            TreeNode* w = x->parent->left;  // sibling == parent.left
            if (w->is_red) {
                w->is_red = false;
                x->parent->is_red = true;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (!w->left->is_red && !w->right->is_red) {
                w->is_red = true;
                x = x->parent;
            } else {
                if (!w->left->is_red) {
                    if (w->right != nil_) w->right->is_red = false;
                    w->is_red = true;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->is_red = x->parent->is_red;
                x->parent->is_red = false;
                if (w->left != nil_) w->left->is_red = false;
                rightRotate(x->parent);
                x = root_;
            }
        }
    }
    x->is_red = false;

    //TreeNode* cur = x;
    //while (cur != nil_) {
    //    updateSize(cur);
    //    cur = cur->parent;
    //}
}

template<typename KT, typename VT>
void RBTree<KT, VT>::insert(const KT& key, const VT& value){
    TreeNode* z = new TreeNode(key, value);
    z->left = z->right = z->parent = nil_;

    TreeNode* y = nil_;
    TreeNode* x = root_;
    while (x != nil_) {
        //x->size++;
        y = x;
        if (*(z->key) < *(x->key)) x = x->left;
        else if (*(z->key) > *(x->key)) x = x->right;
        else {
            remove(*(x->key));
            insert(key, value);
            return;
        }
    }
    z->parent = y;
    if (y == nil_) root_ = z;
    else if (*(z->key) < *(y->key)) y->left = z;
    else y->right = z;

    z->left = z->right = nil_;
    z->is_red = true;

    //update size of the above nodes after insertion
    TreeNode* curr = z->parent;
    while (curr != nil_) {
        updateSize(curr);
        curr = curr->parent;
    }

    //insertFixup(z);

    //printHelper(root_);
}


template<typename KT, typename VT>
void RBTree<KT, VT>::insertFixup(TreeNode* z){
    while (z->parent->is_red) {
        if (z->parent == z->parent->parent->left) { // parent == grandp.left
            TreeNode* y = z->parent->parent->right; // y == uncle == grandp.right
            if (y->is_red) { // case 2
                z->parent->is_red = false;
                y->is_red = false;
                z->parent->parent->is_red = true;
                z = z->parent->parent;
            } else {  // case 3, 4
                if (z == z->parent->right) { // case 3: triangle
                    z = z->parent;
                    leftRotate(z);
                } // case 4: line
                z->parent->is_red = false;
                z->parent->parent->is_red = true;
                rightRotate(z->parent->parent);
            }
        } else {                                   // parent == grandp.right
            TreeNode* y = z->parent->parent->left; // y == uncle == grandp.left
            if (y->is_red) { // case 2
                z->parent->is_red = false;
                y->is_red = false;
                z->parent->parent->is_red = true;
                z = z->parent->parent;
            } else { // case 3, 4
                if (z == z->parent->left) { // case 3: triangle
                    z = z->parent;
                    rightRotate(z);
                } // case 4: line
                z->parent->is_red = false;
                z->parent->parent->is_red = true;
                leftRotate(z->parent->parent);
            }
        }
    }
    root_->is_red = false;

    TreeNode* curr = z;
    while (curr != nil_) {
     updateSize(curr);
        curr = curr->parent;
    }
}

template<typename KT, typename VT>
size_t RBTree<KT, VT>::getRank(const KT& key){
    //printHelper(root_);
    size_t ascRank = 1;
    TreeNode* x = root_;
    while (x != nil_) {
        if (key < *(x->key)) {
            x = x->left;
        } else if (key > *(x->key)) {
            ascRank += (x->left != nil_ ? x->left->size : 0) + 1;
            x = x->right;
        } else {
            ascRank += (x->left != nil_ ? x->left->size : 0);
            return (root_->size - ascRank + 1);
        }
    }
    return 0;
}

template<typename KT, typename VT>
size_t RBTree<KT, VT>::getRankRoot(){
    return root_->size;
}

template <typename KT, typename VT>
void RBTree<KT,VT>::printHelper(TreeNode* node) {
    std::cout << "|";
    if (node == nil_) return;

 
    std::string scolor = (node->is_red) ? "RED" : "BLACK";
    std::cout << "(" << *(node->key) << "," << scolor << ":";
    std::cout << " Left - ";
    if (node->left != nil_) {
        std::cout << *(node->left->key);
    } else {
        std::cout << "Null";
    }
    std::cout << ", Right - ";
    if (node->right != nil_) {
        std::cout << *(node->right->key);
    } else {
        std::cout << "Null";
    }
    std::cout << ")" ;
    printHelper(node->left);
    printHelper(node->right);
}

}
// namespace cs251