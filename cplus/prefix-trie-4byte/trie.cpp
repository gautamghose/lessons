#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <assert.h>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#ifdef ADVANCED
#include <unordered_map>
#endif

#ifndef uint8_t
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
#endif

typedef uint8_t byte;
typedef uint16_t handle_t;

typedef std::pair<uint32_t, handle_t> atom_t;

#ifdef ADVANCED
 #define MAP std::unordered_map
#else
 #define MAP std::map
#endif

static int nr_leaf_nodes_created = 0;
static inline byte get_byte(uint32_t value, int idx)
{
    typedef union {
        uint32_t u_;
        byte b_[4];
    } ref_t;

    ref_t r = {.u_ = value};
    return r.b_[idx];
}

struct leaf_node {
    byte b_;
    uint32_t atom_;
    handle_t handle_;
    leaf_node(byte b, const uint32_t a, const handle_t h) 
        : b_(b), atom_(a), handle_(h) { nr_leaf_nodes_created++; }
};

struct trie_node_level_3 {
    byte b_;
    MAP<byte, leaf_node*> children_;
    trie_node_level_3(byte b) : b_(b) {}
    void get_neighborhood_3(std::vector<atom_t>& atoms);
};

struct trie_node_level_2 {
    byte b_;
    MAP<byte, trie_node_level_3*> children_;
    trie_node_level_2(byte b) : b_(b) {}
    void get_neighborhood_2(std::vector<atom_t>& atoms);
}; 

struct trie_node_level_1 {
    byte b_;
    MAP<byte, trie_node_level_2*> children_;
    trie_node_level_1(byte b) : b_(b) {}
};

struct trie_node_level_0 {
    MAP<byte, trie_node_level_1*> children_;
};

void trie_node_level_2::get_neighborhood_2(std::vector<atom_t>& atoms)
{
    atoms.clear();
    MAP<byte, trie_node_level_3*>::const_iterator it = children_.begin();
    while (it != children_.end()) {
        trie_node_level_3* level3_node = it->second;
        std::vector<atom_t> local_atoms;
        level3_node->get_neighborhood_3(local_atoms);
        atoms.insert( atoms.begin(), local_atoms.begin(), local_atoms.end() );
        it ++;
    }
    assert(atoms.size() > 0);
}

void trie_node_level_3::get_neighborhood_3(std::vector<atom_t>& atoms)
{
    atoms.clear();
    MAP<byte, leaf_node*>::const_iterator it = children_.begin();
    while (it != children_.end()) {
        leaf_node* leaf = it->second;
        atoms.push_back( atom_t(leaf->atom_, leaf->handle_) ); 
        it ++;
    }
    assert(atoms.size() > 0);
}

struct prefix_sieve {
    /**
     * Returns 0 when successful, -1 when already such atom exists.
     */
    int insert(const uint32_t atom_value, const handle_t atom_handle);

    /**
     * Returns effectively (4 - <the-level-at-which-lookup-failed>).
     * When a full match is found, 0 is returned.
     * On the other extreme if 0 bytes matched, 4 is returned.
     * if only 1 byte (first, i.e. leftmost) matched, 3 is returned.
     * if only 2 byte (first two) matched, 2 is returned.
     * if only 3 byte (first three) matched, 1 is returned.
     * if only 4 byte (first four) matched, 0 is returned.
     *
     * if return is 0, the atom should be used.
     * if return is 1, the atom should be used as one candidate against whom the replacement byte can be coded.
     * if return is 2, the neighborhood should be used to do "arithmetic" against the neighborhood atoms.
     * if return is 3,4, atom and neighborhood are undefined.
     */
    int lookup(const uint32_t val, atom_t& atom, std::vector<atom_t>& neighborhood);

    trie_node_level_0 root_;
};

int prefix_sieve::lookup(const uint32_t val, atom_t& atom, std::vector<atom_t>& neighborhood)
{
    trie_node_level_0* level0 = &root_;

    byte b0 = get_byte(val,0);
    MAP<byte, trie_node_level_1*>::iterator it0 = level0->children_.find(b0);
    trie_node_level_1* level1 = NULL;
    if (it0 == level0->children_.end()) {
        return 4;
    } else {
        level1 = it0->second;
    }
    assert(level1);

    trie_node_level_2* level2 = NULL;
    byte b1 = get_byte(val,1);
    MAP<byte, trie_node_level_2*>::iterator it1 = level1->children_.find(b1);
    if (it1 == level1->children_.end()) {
        return 3;
    } else {
        level2 = it1->second;
    }
    assert(level2);

    trie_node_level_3* level3 = NULL;
    byte b2 = get_byte(val,2);
    MAP<byte, trie_node_level_3*>::iterator it2 = level2->children_.find(b2);
    if (it2 == level2->children_.end()) {
        level2->get_neighborhood_2(neighborhood);
        atom = neighborhood.front();
        return 2;
    } else {
        level3 = it2->second;
    }
    assert(level3);

    leaf_node* level4 = NULL;
    byte b3 = get_byte(val,3);
    MAP<byte, leaf_node*>::iterator it3 = level3->children_.find(b3);
    if (it3 == level3->children_.end()) {
        level3->get_neighborhood_3(neighborhood);
        atom = neighborhood.front();
        return 1;
    } else {
        level4 = it3->second;
    }
    assert(level4);
    assert(level4->atom_ == val);
    atom.first = val;
    atom.second = level4->handle_;
    return 0;
}

int prefix_sieve::insert(const uint32_t atom_value, const handle_t atom_handle)
{
    const uint32_t& val = atom_value;
    trie_node_level_0* level0 = &root_;

    byte b0 = get_byte(val,0);
    MAP<byte, trie_node_level_1*>::iterator it0 = level0->children_.find(b0);
    trie_node_level_1* level1 = NULL;
    if (it0 == level0->children_.end()) {
        level1 = new trie_node_level_1(b0);
        level0->children_.insert(std::pair<byte, trie_node_level_1*>(b0, level1));
    } else {
        level1 = it0->second;
    }
    assert(level1);

    trie_node_level_2* level2 = NULL;
    byte b1 = get_byte(val,1);
    MAP<byte, trie_node_level_2*>::iterator it1 = level1->children_.find(b1);
    if (it1 == level1->children_.end()) {
        level2 = new trie_node_level_2(b1);
        level1->children_.insert(std::pair<byte, trie_node_level_2*>(b1, level2));
    } else {
        level2 = it1->second;
    }
    assert(level2);

    trie_node_level_3* level3 = NULL;
    byte b2 = get_byte(val,2);
    MAP<byte, trie_node_level_3*>::iterator it2 = level2->children_.find(b2);
    if (it2 == level2->children_.end()) {
        level3 = new trie_node_level_3(b2);
        level2->children_.insert(std::pair<byte, trie_node_level_3*>(b2, level3));
    } else {
        level3 = it2->second;
    }
    assert(level3);

    leaf_node* level4 = NULL;
    byte b3 = get_byte(val,3);
    MAP<byte, leaf_node*>::iterator it3 = level3->children_.find(b3);
    if (it3 == level3->children_.end()) {
        level4 = new leaf_node(b3, atom_value, atom_handle);
        level3->children_.insert(std::pair<byte, leaf_node*>(b3, level4));
    } else {
        level4 = it3->second;
        bool bAtomExists = true;
        assert(!bAtomExists);
        return -1;
    }
    assert(level4);
    return 0;
}

int main(int argc, char *argv[])
{
#define MAX_NUM_ATOMS (64 << 2)
    int fd = ::open(argv[1], O_RDONLY);
    assert(fd != -1);
    uint8_t buffer[MAX_NUM_ATOMS*sizeof(uint32_t)]; //64K elements, 4 bytes each
    ssize_t rc = 0;
    prefix_sieve s;
    do
    {
        rc = ::read(fd, buffer, sizeof(buffer));
    } while ((rc == -1) && (errno == EINTR));
    assert((int)rc == (int)sizeof(buffer));

    uint8_t* buf = &buffer[0];
    std::vector<atom_t> neighborhood;
    atom_t atom;

    std::vector<uint32_t> all_values;
    int cnt = 0;
    while (cnt < (MAX_NUM_ATOMS)) {
        uint32_t val = *(uint32_t*)buf;
        handle_t handle = (handle_t)cnt;
        rc = s.insert(val, handle);
        assert(0 == rc);
        buf += sizeof(uint32_t);
        cnt ++;
        //verify
        rc = s.lookup(val, atom, neighborhood);
        assert(0 == rc);
        all_values.push_back(val);
    }
    assert(cnt == (MAX_NUM_ATOMS));
    assert(cnt == nr_leaf_nodes_created);

    //now generate a million elements
    //look up each element; based on the return code, assert that the atom and neighborhood
    //are correct.
    cnt = 0;
    std::vector<atom_t> neighbors;
    int nr_tests[5] = {0, 0, 0, 0, 0};
    int nr_neighbors_bucket[5] = {0, 0, 0, 0, 0};

    //while (cnt < (MAX_NUM_ATOMS*1000)) {
    while (false) {
        uint32_t val = (uint32_t)random();
        rc = s.lookup(val, atom, neighbors);
        nr_tests[rc] ++;
        switch(rc) {
        case 0:
            //full match
            assert(atom.first == val);
            break;
        case 1:
            //3 bytes matched
            assert(atom.first != val);
            assert(get_byte(atom.first,0) == get_byte(val,0));
            assert(get_byte(atom.first,1) == get_byte(val,1));
            assert(get_byte(atom.first,2) == get_byte(val,2));
            assert(get_byte(atom.first,3) != get_byte(val,3));
            for (std::vector<atom_t>::const_iterator it=neighbors.begin();
                    it != neighbors.end(); it++) {
                atom_t a = (*it);
                assert(a.first != val);
                assert(get_byte(a.first,0) == get_byte(val,0));
                assert(get_byte(a.first,1) == get_byte(val,1));
                assert(get_byte(a.first,2) == get_byte(val,2));
                assert(get_byte(a.first,3) != get_byte(val,3));
            }
            break;
        case 2:
            //2 bytes matched
            assert(atom.first != val);
            assert(get_byte(atom.first,0) == get_byte(val,0));
            assert(get_byte(atom.first,1) == get_byte(val,1));
            assert(get_byte(atom.first,2) != get_byte(val,2));
            nr_neighbors_bucket[0] += neighbors.size(); 

            for (std::vector<atom_t>::const_iterator it=neighbors.begin();
                    it != neighbors.end(); it++) {
                atom_t a = (*it);
                assert(a.first != val);
                assert(get_byte(a.first,0) == get_byte(val,0));
                assert(get_byte(a.first,1) == get_byte(val,1));
                assert(get_byte(a.first,2) != get_byte(val,2));
            }
            break;
        case 3:
            //1 bytes matched
            break;
        case 4:
            //0 bytes matched
            break;
        default:
            assert(false); // should never be here
        } 
        cnt ++;
    }

    for (int i=0; i<=4; i++) {
        std::cout << "Number of tests for rc " << i << " is " << nr_tests[i] << std::endl;
        std::cout << "Number of neighbors for bucket " << i << " is " << nr_neighbors_bucket[i] << std::endl;
    }

    std::cout << "Will execute " << all_values.size() << " successful queries " << 1000000 << " times. " <<std::endl;
    cnt = 0;
    while (cnt < 1000000) {
        for (std::vector<uint32_t>::const_iterator it = all_values.begin(); it!=all_values.end(); it++) {
            rc = s.lookup((*it), atom, neighbors);
            assert(rc == 0 && atom.first == (*it));
        } 
        cnt++;
    }
    std::cout << "Exceuted " << all_values.size() << " successful queries " << cnt << " times. " <<std::endl;
    //std::ifstream infile(argv[1]);
    //uint32_t a_num;
    //uint16_t nr_numbers = 0;
    //while (infile >> a_num) {
        //int rc = s.insert(a_num, nr_numbers);
        //assert(rc == 0);
        //nr_numbers++;
    //}
    return 0;
}

