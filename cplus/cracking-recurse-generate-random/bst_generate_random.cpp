#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>

struct bst_node {
    struct bst_node* left_;
    struct bst_node* right_;
    int data_;
    size_t population_;
    bst_node(int val) : left_(NULL), right_(NULL), data_(val), population_(1) {}
    ~bst_node() {}
};

struct bst_tree {
    size_t nr_entries_;
    bst_node* root_;
    bst_tree() : nr_entries_(0), root_(NULL) {}
    void insert(int val);
    int get_random();
    bool remove(int val);
    void print();
private:
    void insert_(bst_node* root, int val);
    int get_random_(bst_node* root, size_t rank);
    bool remove_(bst_node* parent, bst_node* root, int val);
    bst_node* remove_root_node(bst_node* root);
    int find_max(bst_node* root);
    void print_(bst_node* root);
};

void bst_tree::insert(int val)
{
    if (nr_entries_ == 0) {
        assert(root_ == NULL);
        root_ = new bst_node(val);
        ++nr_entries_;
        return;
    }
    insert_(root_, val);
    ++nr_entries_;
}

bool bst_tree::remove(int val)
{
    if (nr_entries_ == 0) return false;
    assert(root_);
    bool rc = false;
    if (root_->data_ == val) {
        bst_node tmp(val);
std::cout << "Corner case here\n";
        tmp.left_ = root_;
        assert(remove_(&tmp, root_, val));
        rc = true;
    } else if (val < root_->data_) {
        rc = remove_(root_, root_->left_, val);
    } else {
        rc = remove_(root_, root_->right_, val);
    }
    if (rc) {
        --root_->population_;
        --nr_entries_;
    }
    return rc;
}

void bst_tree::insert_(bst_node* root, int val)
{
    assert(root->data_ != val);
    ++root->population_;
    if (val < root->data_) {
        if (root->left_ == NULL) {
            root->left_ = new bst_node(val);
            return;
        }
        insert_(root->left_, val);
    } else {
        if (root->right_ == NULL) {
            root->right_ = new bst_node(val);
            return;
        }
        insert_(root->right_, val);
    }
    return;
}

bool bst_tree::remove_(bst_node* parent, bst_node* root, int val)
{
    if (!root) return false;
    if (val < root->data_) {
        bool rc = remove_(root, root->left_, val);
        if (rc) --root->population_;
        return rc;
    } else if (val > root->data_) {
        bool rc = remove_(root, root->right_, val);
        if (rc) --root->population_;
        return rc;
    }
    assert(root->data_ == val);
    bst_node*& to_be_patched = (parent->right_ == root ? parent->right_ : parent->left_);
    
    if (root->left_ == NULL && root->right_ == NULL) {
        //easiest case
std::cout << "Remove corner case 0\n";
        to_be_patched = NULL;
        delete root;
        return true;
    }
    if (root->left_ == NULL) {
std::cout << "Remove corner case 1\n";
        to_be_patched = root->right_;
        delete root;
        return true;
    } else if (root->right_ == NULL) {
std::cout << "Remove corner case 2\n";
        to_be_patched = root->left_;
        delete root;
        return true;
    }

std::cout << "Remove corner case\n";
    assert(root->left_ && root->right_);
    int maxm = find_max(root->left_);
    bool rc = remove_(root, root->left_, maxm);
    assert(rc);
    root->data_ = maxm;
    root->population_ = 1;
    root->population_ += (root->left_ ? root->left_->population_ : 0);
    root->population_ += (root->right_ ? root->right_->population_ : 0);
    return true;
}

int bst_tree::find_max(bst_node* root)
{
    assert(root);
    if (root->right_) 
        return find_max(root->right_);
    return (root->data_);
}

int bst_tree::get_random()
{
    size_t rank = (size_t)random() % nr_entries_; 
    std::cout << "debug: rank " << rank << std::endl;
    return get_random_(root_, rank);
}

int bst_tree::get_random_(bst_node* root, size_t rank)
{
    assert(root);
    assert(rank < root->population_);
    size_t left_pop = (root->left_ ? root->left_->population_ : 0);
    size_t right_pop = (root->right_ ? root->right_->population_ : 0);
    assert(left_pop + right_pop + 1 == root->population_);

    if (rank < left_pop) {
        return get_random_(root->left_, rank);
    } else if (rank == left_pop) {
        return root->data_;
    } else {
        return get_random_(root->right_, rank-left_pop-1);
    }
}

void bst_tree::print_(bst_node* root)
{
    if (!root) return;
    print_(root->left_);
    std::cout << "root: " << root;
    std::cout << " left: " << root->left_;
    std::cout << " right: " << root->right_;
    std::cout << " data: " << root->data_;
    std::cout << " pop: " << root->population_;
    std::cout << std::endl;
    print_(root->right_);
}

void bst_tree::print()
{
    std::cout << "ROOT: " << root_ << std::endl;
    print_(root_);
}

int main(int argc, char** argv)
{
    bst_tree root;
    root.insert(3);
    root.insert(4);
    root.insert(2);
    root.insert(1);
    root.insert(5);
    root.insert(8);
    root.insert(7);
    root.insert(9);
    root.insert(6);

    root.print();

    root.insert(12);
std::cout << "After 12\n";
    root.print();
    assert(root.remove(12));
    assert(root.remove(12) == false);
std::cout << "After 12 removal\n";
    root.print();

    for (int i=0; i<20; i++) {
        std::cout << "Random " << i << ": " << root.get_random() << std::endl;
    }
    return 0;    
}


