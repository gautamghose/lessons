#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

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

namespace cracking {
template <typename T>
class smart_ptr {
  T* core_obj_;
  int* refcnt_;
public:
  smart_ptr(T* obj)
  {
    assert(obj);
    core_obj_ = obj;
    refcnt_ = new int(1);
  }
  ~smart_ptr()
  {
    std::cout << "destructor on " << this->get_raw()->value() << std::endl;
    release();
  }

  void release()
  {
    assert(refcnt_);
    (*refcnt_) --;
    assert(*refcnt_ >= 0);
    if (0 == *refcnt_) {
      delete core_obj_;
      delete refcnt_;
    }
    core_obj_ = NULL;
    refcnt_ = NULL;
  }

  const T* get_raw() const
  {
    return core_obj_;
  }

  T* operator->()
  {
    assert(core_obj_);
    assert(refcnt_ && (*refcnt_ >= 1));
    return core_obj_;
  }

  smart_ptr& operator=(smart_ptr& right)
  {
    std::cout << "operator= called from " << right->value() << " into " << this->get_raw()->value() << std::endl;
    if (refcnt_) {
      release();
    } 
    refcnt_ = right.refcnt_;
    core_obj_ = right.core_obj_;
    *refcnt_ ++;
    return *this;
  }

  //copy-constructor here
  smart_ptr(const smart_ptr& right)
  {
    std::cout << "copy con called with " << right.get_raw()->value() << std::endl;
    refcnt_ = right.refcnt_;
    core_obj_ = right.core_obj_;
    (*refcnt_) ++;
  }

  //this is needed to be able to use this in the unordered_map
  bool operator==(const smart_ptr& other) const
  {
    return (core_obj_ == other.core_obj_);
  }

};
} //end namespace

namespace std {
  template <typename S>
  struct hash<cracking::smart_ptr<S>>
  {
    std::size_t operator()(const cracking::smart_ptr<S>& key) const
    {
      return hash<uint64_t>()((uint64_t)(key.get_raw()));
    }
  };
}

static int N;

struct my_int {
  int n_;
  my_int(int n): n_(n) {}
  ~my_int()
  {
    std::cout << "my_int destructor called.\n";
  }
  void method1()
  {
    std::cout << "method1 called on " << n_ << std::endl;
  }
  void method2() const
  {
    std::cout << "method2 called on " << n_ << std::endl;
  }
  int value() const {return n_;}
};

int main(int argc, char** argv)
{
    cracking::smart_ptr<my_int> pInt(new my_int(12));
    if (argc > 1) {
        assert(sscanf(argv[1],"%d",&N) == 1);
        if (N > 16) {
            std::cout << "N greater than 16 not supported\n";
            return 1;
        }
    }
    pInt->method1();
    pInt.get_raw()->method2();

    std::vector<cracking::smart_ptr<my_int> > myints;
    std::cout << "Just before myints.push_back\n";
    myints.push_back(pInt);
    cracking::smart_ptr<my_int> pInt2(new my_int(13));
    std::cout << "Just before myints.push_back2\n";
    myints.push_back(pInt2);
    std::cout << "Just after myints.push_back2\n";
#if 0
    myints.push_back(cracking::smart_ptr<my_int>(new my_int(13)));
#endif
    std::vector<cracking::smart_ptr<my_int> >::const_iterator it = myints.begin();
    int obj_nr = 0;
    for (; it != myints.end(); it++) {
        std::cout << "Object number " << obj_nr ++ << ": " << (*it).get_raw()->value() << std::endl;
    }

    my_int* tmp_int = new my_int(14);
    cracking::smart_ptr<my_int> pMyInt2(tmp_int);
    std::unordered_map<cracking::smart_ptr<my_int>, uint64_t> myints_map;
    myints_map.insert(std::make_pair<cracking::smart_ptr<my_int>, uint64_t>(
                                        cracking::smart_ptr<my_int>(tmp_int), 0xdeadbeef));
    assert(myints_map.find(pMyInt2) != myints_map.end());
    assert(myints_map.find(cracking::smart_ptr<my_int>(new my_int(15))) == myints_map.end());
    
    
    std::cout << "Just before return\n";
    return 0;    
}


