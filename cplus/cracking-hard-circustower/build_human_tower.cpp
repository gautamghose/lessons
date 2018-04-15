#include <assert.h>
#include <vector>
#include <iostream>
#include <algorithm>


/// One permutation is captured in a vector
/// height, weight
typedef std::pair<int,int> building_block_t;

struct XGreater {
    bool operator() (const building_block_t& left, const building_block_t& right) const
    {
        std::cout << "XGreater compare()\n";
        return (left.second < right.second);
    }
};

bool compare_operator (const building_block_t& left, const building_block_t& right)
{
    std::cout << "operator< compare()\n";
    return (left.first < right.first);
}

void sort_all_by_height(std::vector<building_block_t>& all_blocks)
{
#if 1
    std::sort(all_blocks.begin(), all_blocks.end(), [] (const building_block_t& left, const building_block_t& right) 
                                                    {
                                                        std::cout << "lambda compare()\n";
                                                        return (left.first > right.first);
                                                    });
#else
    std::sort(all_blocks.begin(), all_blocks.end(), XGreater());
    std::sort(all_blocks.begin(), all_blocks.end(), compare_operator);
#endif

    
}

void generate_longest_sequence_ending_at(size_t idx, const std::vector<building_block_t>& all_blocks, std::vector<building_block_t> longest_sequence_ending_at_index[])
{
    assert(idx < all_blocks.size());
    if (idx == 0) {
        longest_sequence_ending_at_index[0].push_back(all_blocks[0]);
        assert(longest_sequence_ending_at_index[0].size() == 1);
        return;
    }
    std::vector<building_block_t> current_best;
    ssize_t current_best_idx = -1;
    size_t current_best_length = 0;
    for (size_t i=0; i<idx; i++) {
        if (longest_sequence_ending_at_index[i].size() >= current_best_length) {
            if (longest_sequence_ending_at_index[i].back().second >= all_blocks[idx].second) {
                current_best_length = longest_sequence_ending_at_index[i].size();
                current_best_idx = i;
            }
        }
    }
    assert(longest_sequence_ending_at_index[idx].size() == 0);
    if (current_best_idx == -1) {
        longest_sequence_ending_at_index[idx].push_back(all_blocks[idx]);
    } else {
        assert(current_best_idx >= 0);
        assert(current_best_length > 0);
        longest_sequence_ending_at_index[idx] = longest_sequence_ending_at_index[current_best_idx];
        assert(longest_sequence_ending_at_index[idx].back().second >= all_blocks[idx].second);
        longest_sequence_ending_at_index[idx].push_back(all_blocks[idx]);
    }
}

void find_longest_decreasing_sequence_by_weight(const std::vector<building_block_t>& all_blocks, std::vector<building_block_t>& answer)
{
    //I need an array of std::vector<building_block_t>
    std::vector<building_block_t>* longest_sequence_ending_at_index = new std::vector<building_block_t> [all_blocks.size()];
        
    for (size_t i=0; i<all_blocks.size(); i++) {
        generate_longest_sequence_ending_at(i, all_blocks, longest_sequence_ending_at_index);
        assert(longest_sequence_ending_at_index[i].size() > 0);
    }
   
    size_t longest_idx = 0;
    size_t longest_length = longest_sequence_ending_at_index[0].size();
    for (size_t i=0; i<all_blocks.size(); i++) {
        if (longest_sequence_ending_at_index[i].size() > longest_length) {
            longest_idx = i;
            longest_length = longest_sequence_ending_at_index[i].size();
        }
    }

    answer.clear();
    answer = longest_sequence_ending_at_index[longest_idx];
     
    delete [] longest_sequence_ending_at_index;
}

int main(int argc, char** argv)
{
    std::vector<building_block_t> all_blocks;

    all_blocks.push_back(building_block_t(62, 140));
    all_blocks.push_back(building_block_t(65, 155));
    all_blocks.push_back(building_block_t(66, 160));
    all_blocks.push_back(building_block_t(64, 145));
    all_blocks.push_back(building_block_t(67, 150));
    all_blocks.push_back(building_block_t(63, 150));

    sort_all_by_height(all_blocks);

    auto it = all_blocks.begin();
    while (it != all_blocks.end()) {
        [] (const building_block_t& one) {
            std::cout << one.first << "," << one.second;
            std::cout << std::endl;
            return;
        } (*it);
        it ++;
    }

    std::vector<building_block_t> answer;
    find_longest_decreasing_sequence_by_weight(all_blocks, answer);
    std::cout << "Answer: " << std::endl;
    it = answer.begin();
    while (it != answer.end()) {
        [] (const building_block_t& one) {
            std::cout << one.first << "," << one.second;
            std::cout << std::endl;
            return;
        } (*it);
        it ++;
    }

    return 0;
}


