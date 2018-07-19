#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

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

std::string int_to_binary(unsigned int num, bool prepend_zeros = false)
{
    std::string result;
    if (num == 0) result.append("0");
    while (num > 0) {
        if (num & 0x01) result.append("1");
        else result.append("0");
        num >>= 1;
    }
    if (prepend_zeros) {
        size_t nr_pads = 8*sizeof(unsigned int) - result.size(); //8 bits in 1 byte
        while (nr_pads > 0) {
            result.append("0");
            nr_pads --;
        }
    }
    std::reverse(result.begin(), result.end());
    return result;
}

/* 
If we want the binary representation of say, (-10), then pass (10) to this function.
This function will then give the binary rep of (-10) as in 2's complement.
*/
std::string negative_int_to_binary(int num)
{
    /*
    2's complement of a negative number is as follows:-
    1. Take the positive value of the number and put it out in binary.
    2. Flip each of the 32 bits. This will make sure, the highest bit is set to 1.
    3. Add 1 to the result.
    */
    unsigned int answer = (unsigned int)num;
    answer = ~answer; //bitwise not 
    answer += 0x1;
    return int_to_binary(answer, true);
}

template <typename C, typename P>
class IsDerivedFromHelper {
    class No {};
    class Yes {No no[3];};

    static Yes test( P* );
    static No test( ... );
  public:
    enum { value = sizeof( test( static_cast<C*>(0) ) ) == sizeof (Yes) };
};

/**
 * Q: Implement a template function IsDerivedFrom() that takes class C and class P as template parameters. It should return true when class C is derived from class P and false otherwise.
 */
template <typename C, typename P>
bool IsDerivedFrom()
{
#if 0
    return std::is_base_of<P, C>::value; //Read as: if P is_base_of C
#else
    return IsDerivedFromHelper<C, P>::value; //Read as: if C is_derived_from P
#endif
}

/**
 * Q: Implement a template boolean IsSameClass() that takes class A and B as template parameters. It should compare class A and B and return false when they are different classes and true if they are the same class.
 */

template <typename P, typename Q>
struct is_same {
  static const bool value = false;
};

template <typename T>
struct is_same<T,T> {
  static const bool value = true;
};

template <typename A, typename B>
bool IsSameClass()
{
#if 0
    return (IsDerivedFrom<A,B>() && IsDerivedFrom<B,A>());
#else
    return is_same<A,B>::value;
#endif
}

/**
 * Q3: What is printed on stdout from the following code?
#include <iostream>

class A {
public:
    A() {}
    ~A() {
        throw 42;
    }
};

int main(int argc, const char * argv[]) {
    try {
        A a;
        throw 32;
    } catch(int a) {
        std::cout << a;
    }
}
 */
namespace Q3 {
class A {
public:
    A() {}
    ~A() {
        throw 42;
    }
};
}

int simulate_main()
{
    try {
        Q3::A a;
        //throw 32;
    } catch (int a) {
        std::cout << "inside catch: " << a << std::endl;
    }
    return 0;
}

/**
 * Q4: How can you make sure a C++ function can be called as e.g. void foo(int, int) but not as any other type like void foo(long, long)?
 */
class Q4 {
public:
    Q4() {};
    void foo(int i, int j) {std::cout << "Q4::foo(int,int) called.\n";}
private:
    void foo(long i, long j) {std::cout << "Q4::foo(long,long) called.\n";}
};

template <typename A, typename B>
void foo(A arg1, B arg2); //i.e. not defined

template <>
void foo(int arg1, int arg2)
{
    std::cout << "Q4::global::foo(int, int) called.\n";
}

/**
 * Q5: What will be the output of the following program?
struct A
{
    int data[2];

    A(int x, int y) : data{x, y} {}
    virtual void f() {}
};

int main(int argc, char **argv)
{
    A a(22, 33);

    int *arr = (int *) &a;
    std::cout << arr[2] << std::endl;

    return 0;
}
 */

namespace Q5 {
struct A
{
    int data[2];

    A(int x, int y) : data{x, y} {}
    virtual void f() {}
};
}

/**
 * Q6 (picked from a different website): What is the following code doing?
template <class IntegerType>
class smart_integer {
public: 
    typedef integer_traits<Integer> traits;
    IntegerType operator+=(IntegerType value){
        typedef typename traits::larger_integer_t larger_t;
        larger_t interm = larger_t(myValue) + larger_t(value); 
        if(interm > traits::max() || interm < traits::min())
            throw overflow();
        myValue = IntegerType(interm);
    }
}
 */
#if 0
//not ready yet for compilation
namespace Q6 {
template <class IntegerType>
class smart_integer {
public: 
    typedef integer_traits<Integer> traits;
    IntegerType operator+=(IntegerType value){
        typedef typename traits::larger_integer_t larger_t;
        larger_t interm = larger_t(myValue) + larger_t(value); 
        if(interm > traits::max() || interm < traits::min())
            throw overflow();
        myValue = IntegerType(interm);
    }
};
}
#endif

struct A {long a; virtual void foo(){} };
struct B : A {};
typedef A C;
struct D {};

int main(int argc, char** argv)
{
    std::cout << "sizeof(A): " << sizeof(A) << std::endl;
    std::cout << "sizeof(B): " << sizeof(B) << std::endl;
    std::cout << "IsDerivedFrom(B, A): " << IsDerivedFrom<B,A>() << std::endl;
    std::cout << "IsDerivedFrom(A, A): " << IsDerivedFrom<A,A>() << std::endl;
    std::cout << "IsDerivedFrom(A, B): " << IsDerivedFrom<A,B>() << std::endl;
    std::cout << "IsSameClass(A, A): " << IsSameClass<A,A>() << std::endl;
    std::cout << "IsSameClass(A, B): " << IsSameClass<A,B>() << std::endl;
    std::cout << "IsSameClass(B, A): " << IsSameClass<B,A>() << std::endl;
    std::cout << "IsSameClass(A, C): " << IsSameClass<A,C>() << std::endl;
    std::cout << "IsSameClass(C, A): " << IsSameClass<C,A>() << std::endl;
    std::cout << "IsSameClass(A, D): " << IsSameClass<A,D>() << std::endl;

#if 0
    int a = 8, b = 9;
#else
    long a = 8, b = 9;
#endif

    Q4 q4;
    //q4.foo(a,b); //This will not compile with long a,b see Q4
    //::foo(a,b); //This will not compile with long a,b see Q4

    std::cout << "Q5 now:\n";
    Q5::A a5(22, 33);

    int *arr = (int *) &a5;
    std::cout << "data[0]: " << a5.data[0] << std::endl;
    std::cout << "data[1]: " << a5.data[1] << std::endl;
    std::cout << "data[2]: " << a5.data[2] << std::endl;
    std::cout << arr[2] << std::endl;

    {
    int i = 0;
    if (false)
      // some comment
      i++;
    std::cout << "If i is 1 then //comment line is not ignored and becomes the if-clause. If i is 0, then //comment line is simply parsed out. print i = " << i <<std::endl;
    }
    std::cout << "Q3 now:" << std::endl;
    simulate_main();
    return 0;    
}


