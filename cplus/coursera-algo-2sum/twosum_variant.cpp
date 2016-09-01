#include <iostream>
#include <map>
#include <unordered_map>
#include <fstream>

int main(int argc, char *argv[])
{
    // Create an unordered_map of three strings (that map to strings)
    std::unordered_map<std::string, std::string> u = {
        {"RED","#FF0000"},
        {"GREEN","#00FF00"},
        {"BLUE","#0000FF"}
    };
    std::unordered_map<long, int> numbers;
    numbers.reserve(2000000); // There will be 1 million numbers, but reserving for 2 million improves load factor and reduces collision.
    //std::cout << u["RED"] << std::endl;
    std::ifstream infile(argv[1]);
    long a_num;
    while (infile >> a_num) {
        //std::cout << a_num << std::endl;
        auto search = numbers.find(a_num);
        if (search != numbers.end()) {
            search->second ++;
        } else {
            numbers.insert(std::make_pair(a_num, (int)1));
        }
    }

    std::cout << "Number of numbers = " << numbers.size() << std::endl;
    std::cout << "Load factor = " << numbers.load_factor() << std::endl;
    std::cout << "Max Load factor = " << numbers.max_load_factor() << std::endl;

    long range = 1000;
    long nr_targets_reached = 0;
    long nr_lookups = 0;
    for (long target=-range; target<=range; target++)
    {
        printf ("Target: %ld\n", target);
        std::unordered_map<long, int>::const_iterator iter = numbers.begin();
        while (iter != numbers.end()) {

            long first = (*iter).first;
        //printf ("  First: %d\n", first);
            //See if (target - first) is present in the hashmap
            long look_for = target - first;
            if (look_for == first) {
                if ((*iter).second > 1) {
                //Found pair for first, for target 
                    //printf("Target %d: Found one pair (%d,%d)\n", target, first, look_for);
                }
            } else {
                nr_lookups ++;
                if (numbers.find(look_for) != numbers.end()) {
                    //printf("TargeT %d: Found one pair (%d,%d)\n", target, first, look_for);
                    nr_targets_reached ++;
                    printf ("\t reached.\n");
                    break; // break our of the while loop; i.e. search for the next target
                }
            }
            iter++;
        }
    }

    printf ("Number of targets reached: %ld\n", nr_targets_reached);
    std::cout << "nr_lookups = " << nr_lookups << std::endl;
    return 1;
}

