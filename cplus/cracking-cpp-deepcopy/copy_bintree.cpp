#include <stdio.h>
#include <assert.h>
#include <string>
#include <iostream>
#include <unordered_map>

typedef struct btnode_s {
  struct btnode_s *left_, *right_;
  int val_;
} btnode_t;

btnode_t* create_btree()
{
    btnode_t* root = (btnode_t*)malloc(sizeof(btnode_t));
    root->left_ = (btnode_t*)malloc(sizeof(btnode_t));
    root->left_->left_ = root->left_->right_ = NULL; root->left_->val_ = 3;
    root->right_ = (btnode_t*)malloc(sizeof(btnode_t));
    root->right_->left_ = root->right_->right_ = NULL; root->right_->val_ = 5;
    root->val_ = 4;
    return root;
}

static std::unordered_map<uint64_t, uint64_t> already_visited;

btnode_t* clone_btree(btnode_t* root)
{
    if (!root) return NULL;

    std::unordered_map<uint64_t, uint64_t>::const_iterator got = already_visited.find((uint64_t)root);
    if (got != already_visited.end()) {
        return (btnode_t*)got->second;
    }

    btnode_t* new_root = (btnode_t*)malloc(sizeof(btnode_t));
    assert(new_root);

    already_visited.insert(std::make_pair<uint64_t, uint64_t>(
                                (uint64_t)root, (uint64_t)new_root));

    new_root->left_ = clone_btree(root->left_);
    new_root->right_ = clone_btree(root->right_);
    new_root->val_ = root->val_;
    return new_root;
}

void print_btree_inorder(const btnode_t* root)
{
    if (!root) return;
    print_btree_inorder(root->left_);
    std::cout << root << ":" << root->val_ << std::endl;
    print_btree_inorder(root->right_);
}

void print_btree_postorder(const btnode_t* root)
{
    if (!root) return;
    print_btree_postorder(root->left_);
    print_btree_postorder(root->right_);
    std::cout << root << ":" << root->val_ << std::endl;
}

int main() 
{
    btnode_t* orig = create_btree();
    btnode_t* clone = clone_btree(orig);
    std::cout << "Orig tree: inorder\n";
    print_btree_inorder(orig);
    std::cout << "Orig tree: postorder\n";
    print_btree_postorder(orig);
    std::cout << "Clon tree: inorder\n";
    print_btree_inorder(clone);
    std::cout << "Clon tree: postorder\n";
    print_btree_postorder(clone);
}

