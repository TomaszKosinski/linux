#include <iostream>
#include <memory>

using namespace std;

class B;

class A {
public:
    void setRelation(shared_ptr<B> b) {
        relation = b;
    }
    ~A() {
        cout<<"Descructor of ~A called"<<endl;
    }

    shared_ptr<B> relation;
};

class B {
public:
    void setRelation(shared_ptr<A> a) {
        relation = a;
    }
    ~B() {
        cout<<"Descructor of ~B called"<<endl;
    }

    shared_ptr<A> relation;
};

int main() {
    // Create shared object
    shared_ptr<A> a(new A());
    shared_ptr<B> b(new B());

    cout<<"Number of use count before circular dependency A:" << a.use_count() << " B:"<<b.use_count()<<endl;

    // Create circular depencency:
    a->setRelation(b);
    b->setRelation(a);

    cout<<"Number of use count after circular dependency A:" << a.use_count() << " B:"<<b.use_count()<<endl;

    a.reset();
    cout<<"Number of use count after a is reset A:" << a.use_count() << " B:"<<b.use_count()<<endl;

    b.reset();
    cout<<"Number of use count after b is reset A:" << a.use_count() << " B:"<<b.use_count()<<endl;

    cout<< "descructors not called\n";
    return 0;
}