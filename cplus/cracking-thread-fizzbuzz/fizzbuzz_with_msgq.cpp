#include <assert.h>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
 
#if 0
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
#endif
 
namespace cracking {
template <class MsgT>
class MessageQ {
    std::deque<MsgT> messages_;
    size_t max_;
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
public:
    MessageQ(size_t length) : max_(length) 
    {
        mutex_ = PTHREAD_MUTEX_INITIALIZER;
        cond_ = PTHREAD_COND_INITIALIZER; 
    }
    //send can block.
    bool send(const MsgT& msg) 
    {
        pthread_mutex_lock(&mutex_);
        while (messages_.size() >= max_) {
            pthread_cond_wait(&cond_, &mutex_);
        }
        assert(messages_.size() < max_);
        messages_.push_back(msg);
        pthread_cond_signal(&cond_);
        pthread_mutex_unlock(&mutex_);
        return true;
    }
    //recv can block.
    bool recv(MsgT& msg) 
    {
        pthread_mutex_lock(&mutex_);
        while (messages_.size() == 0) {
            pthread_cond_wait(&cond_, &mutex_);
        }
        assert(messages_.size() < max_ && messages_.size() > 0);
        msg = messages_.front();
        messages_.pop_front();
        pthread_cond_signal(&cond_);
        pthread_mutex_unlock(&mutex_);
        return true;
    }
    ~MessageQ() {};
};
}

struct Input {
    int val_;
    Input(): val_(-2) {}
    Input(int val): val_(val) {}
    Input(const Input& in) {val_ = in.val_;}
    Input& operator=(const Input& in) {val_ = in.val_; return *this;}
};
struct Output {
    int val_;
    bool result_;
    Output(): val_(-3), result_(false) {}
    Output(int val, bool result): val_(val), result_(result) {}
    Output(const Output& out) {val_ = out.val_; result_ = out.result_;}
    Output& operator=(const Output& out) {val_ = out.val_; result_ = out.result_; return *this;}
};

static cracking::MessageQ<Input> mq_fizzer_input(10);
static cracking::MessageQ<Input> mq_buzzer_input(10);
static cracking::MessageQ<Input> mq_fizzbuzzer_input(10);
static cracking::MessageQ<Output> mq_fizzer_output(10);
static cracking::MessageQ<Output> mq_buzzer_output(10);
static cracking::MessageQ<Output> mq_fizzbuzzer_output(10);

void check_fizz()
{
    Input input;
    Output output;
    while (true) {
        assert(mq_fizzer_input.recv(input));
        if (input.val_ == -1) break;
        output.val_ = input.val_;
        output.result_ = (input.val_ % 3 == 0);
        assert(mq_fizzer_output.send(output)); 
    }
}
void check_buzz()
{
    Input input;
    Output output;
    while (true) {
        assert(mq_buzzer_input.recv(input));
        if (input.val_ == -1) break;
        output.val_ = input.val_;
        output.result_ = (input.val_ % 5 == 0);
        assert(mq_buzzer_output.send(output)); 
    }
}
void check_fizzbuzz()
{
    Input input;
    Output output;
    while (true) {
        assert(mq_fizzbuzzer_input.recv(input));
        if (input.val_ == -1) break;
        output.val_ = input.val_;
        output.result_ = ((input.val_ % 5 == 0) && (input.val_ % 3 == 0));
        assert(mq_fizzbuzzer_output.send(output)); 
    }
}

int main(int argc, char** argv)
{
    if (argc < 1) {
        std::cout << "Usage: " << argv[0] << " <positive-number-upto-which-todo-fizzbuzzing>\n";
        return 1;
    }
    int count_upto = 0;
    assert(1 == sscanf(argv[1], "%d", &count_upto));
    std::cout << "starting first helper...\n";
    std::thread fizzer(check_fizz);
    std::cout << "starting second helper...\n";
    std::thread buzzer(check_buzz);
    std::cout << "starting third helper...\n";
    std::thread fizzbuzzer(check_fizzbuzz);
 
    //main thread doing the numbers
    for (int i=1; i<=count_upto; i++) {
        Input input(i);
        assert(mq_fizzer_input.send(input)); 
        assert(mq_buzzer_input.send(input)); 
        assert(mq_fizzbuzzer_input.send(input)); 

        Output fizz_out, buzz_out, fizzbuzz_out;
        assert(mq_fizzer_output.recv(fizz_out));
        assert(mq_buzzer_output.recv(buzz_out));
        assert(mq_fizzbuzzer_output.recv(fizzbuzz_out));

        assert(fizz_out.val_ == i);
        assert(buzz_out.val_ == i);
        assert(fizzbuzz_out.val_ == i);

        std::cout << i << " : ";
        if (fizzbuzz_out.result_) {
            std::cout << "fizzbuzz" << std::endl;
        } else if (buzz_out.result_) {
            std::cout << "buzz" << std::endl;
        } else if (fizz_out.result_) {
            std::cout << "fizz" << std::endl;
        } else {
            std::cout << "noluck" << std::endl;
        }
    }

    assert(mq_fizzer_input.send(Input(-1)));
    assert(mq_buzzer_input.send(Input(-1)));
    assert(mq_fizzbuzzer_input.send(Input(-1)));

    std::cout << "waiting for helpers to finish..." << std::endl;
    fizzer.join();
    buzzer.join();
    fizzbuzzer.join();
 
    std::cout << "done!\n";
}

