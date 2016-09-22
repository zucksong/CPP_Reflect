#include <iostream>
#include <string>
#include <map>

#define BASE_CLASS Test
#define GLOBAL_FUN_MAP FunMap<BASE_CLASS>::newInstance_fun_map()
#define DEFINE_CLASS(class_name, fun_name) \
    class_name() { \
        GLOBAL_FUN_MAP.Register(#fun_name, class_name::fun_name); \
    } \
    static class_name class_name##_; \
    static BASE_CLASS* fun_name() { \
        return new class_name; \
    }
#define REGISTER_CLASS(class_name) \
    class_name class_name::class_name##_;

using namespace std;

template <class T>
class FunMap {
  typedef T* (*FUN)(void); //定义的函数指针，等价于typedef FUN T*(*)();

  private:
   map<string, FUN> fun_map_;
  public:
   void Register(const string& fun_name, FUN fun) {
       fun_map_[fun_name] = fun;
   }

   T* newInstance(const string& fun_name) {
       if (fun_map_.end() != fun_map_.find(fun_name)) {
           return fun_map_[fun_name]();
       }
      return NULL;
   }

   static FunMap<T>& newInstance_fun_map() {
       static FunMap fun_map;
       return fun_map;
   }
};

class Test {
  public:
    virtual void print() = 0;
};


class Test1 : public Test {
  public:
    void print() {cout << "test1" << endl;}
    DEFINE_CLASS(Test1, test1)
};
class Test2 : public Test {
  public:
    void print() {cout << "test2" << endl;}
    DEFINE_CLASS(Test2, test2)
};

REGISTER_CLASS(Test1);
REGISTER_CLASS(Test2);

int main() {
    Test *t1 = GLOBAL_FUN_MAP.newInstance("test1");
    Test *t2 = GLOBAL_FUN_MAP.newInstance("test2");
    t1->print();
    t2->print();
    GLOBAL_FUN_MAP.newInstance("111");
}


