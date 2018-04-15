#include <assert.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
 
static pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condvar = PTHREAD_COND_INITIALIZER;
static int counter = 0;
static bool flipflop = false;
#define COUNT_MAX 1000000000

void foo()
{
    // simulate expensive operation
    //std::this_thread::sleep_for(std::chrono::seconds(1));
  bool exitnow = false;
  while(!exitnow) {
    pthread_mutex_lock(&mutex1);
    while (flipflop == false)
        pthread_cond_wait(&condvar, &mutex1);
    assert(flipflop == true);
    assert(counter % 2 == 1);
    counter ++;
    //std::cout << "foo: count " << counter << std::endl;
    if (counter >= COUNT_MAX) exitnow = true;
    flipflop = false;
    pthread_cond_signal(&condvar);
    pthread_mutex_unlock(&mutex1);
  }
}
 
void bar()
{
    // simulate expensive operation
    //std::this_thread::sleep_for(std::chrono::seconds(1));
#if 0
    pthread_mutex_lock(&mutex1);
    counter ++;
    pthread_mutex_unlock(&mutex1);
#endif
  bool exitnow = false;
  while(!exitnow) {
    pthread_mutex_lock(&mutex1);
    while (flipflop == true)
        pthread_cond_wait(&condvar, &mutex1);
    assert(flipflop == false);
    assert(counter % 2 == 0);
    counter ++;
    //std::cout << "bar: count " << counter << std::endl;
    if (counter >= COUNT_MAX) exitnow = true;
    flipflop = true;
    pthread_cond_signal(&condvar);
    pthread_mutex_unlock(&mutex1);
  }
}
 
int main()
{
    std::cout << "starting first helper...\n";
    std::thread helper1(foo);
 
    std::cout << "starting second helper...\n";
    std::thread helper2(bar);
 
    std::cout << "waiting for helpers to finish..." << std::endl;
    helper1.join();
    helper2.join();
 
    assert(counter >= COUNT_MAX);
    std::cout << "done!\n";
}

