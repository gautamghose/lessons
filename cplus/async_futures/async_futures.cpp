#include <iostream>
#include <future>

bool is_prime(int x)
{
    std::cout << "Calculating for " << x << std::endl;
    for (int i=2; i<x; i++) {
        if (x%i == 0)
            return false;
    }
    return true;
}

int main()
{
    //calling first time is_prime asynchronously
    int val = 313222313;
    std::future<bool> fut = std::async(is_prime, 313222313);
    std::cout << "checking whether " << val << " is prime asynchronously\n" ;
    bool ret = fut.get(); //waits for fut to finish
    std::cout << val << " is " << (ret ? "" : "not ") << "prime.\n";
    return 0;
}
