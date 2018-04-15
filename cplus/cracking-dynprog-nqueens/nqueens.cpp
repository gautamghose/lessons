#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>

#if 0
//Just testing logical versus arithmetic bit shifting
int main()
{
    int val = -643;
    int val_logical_shift = (unsigned)val >> 1;
    int val_arith_shift = val >> 1;
    printf ("Orig %d, val_logical_shift %d, val_arith_shift %d\n", val, val_logical_shift, val_arith_shift);
    return 0;
}
#endif

static int N = 4;

typedef struct location_s {
    int x, y;
} location_t;

int absolut(int a)
{
    if (a>0) return a;
    return (0-a);
}

bool is_on_diagonal(location_t pt1, location_t pt2)
{
    return (absolut(pt1.x-pt2.x) == absolut(pt1.y-pt2.y));
}

bool is_location_allowed(const std::vector<location_t>& curr_queen_locations, location_t test)
{
    std::vector<location_t>::const_iterator it = curr_queen_locations.begin();
    for (; it != curr_queen_locations.end(); it++) {
        location_t loc = (*it);
        if (loc.x == test.x || loc.y == test.y) return false;
        if (is_on_diagonal(loc, test)) return false;
    }
    return true;
}

void get_allowable_locations(const std::vector<location_t>& curr_queen_locations, std::vector<location_t>& allowed, int queen_nr)
{
    location_t trial {.x=-1, .y=-1};
    allowed.clear();
    assert(queen_nr >= 1 && queen_nr <= N);
    for (int j=0; j<N; j++) {
        trial.x = (queen_nr-1); trial.y = j;
        if (is_location_allowed(curr_queen_locations, trial)) {
            allowed.push_back(trial);
        }    
    }
}

bool place_queen_number_x_and_rest(std::vector<location_t>& curr_queen_locations, int x)
{
    if (x>N) return true;

    std::vector<location_t> trial_locations;
    get_allowable_locations(curr_queen_locations, trial_locations, x);
    std::vector<location_t>::const_iterator it = trial_locations.begin();
    for (; it != trial_locations.end(); it++) {
        location_t trial = *it;
        //1. place queen 1 in that location.
        //2. update curr_queen_locations with that location
        curr_queen_locations.push_back(trial);
        //3. Try and see if the rest of the queens can be placed.
        bool rc = place_queen_number_x_and_rest(curr_queen_locations, x+1); 
        if (rc == true) {
            //Found a setting.
            return true;
        }
        curr_queen_locations.pop_back();
    }
    return false;
}


int main(int argc, char** argv)
{
    if (argc > 1) {
        assert(sscanf(argv[1],"%d",&N) == 1);
        if (N > 16) {
            std::cout << "N greater than 16 not supported\n";
            return 1;
        }
    }
    std::vector<location_t> curr_queen_locations;
    if (place_queen_number_x_and_rest(curr_queen_locations, 1) == true) {
        std::vector<location_t>::const_iterator it = curr_queen_locations.begin();
        for (int q=1; it != curr_queen_locations.end(); it++,q++) {
            std::cout << "Position of Queen " << q << ": " << "(" << (*it).x << "," << (*it).y << ")" << std::endl;
        }
    } else {
        std::cout << N << " queens problem cannot be solved." << std::endl;
    }
    return 0;    
}


