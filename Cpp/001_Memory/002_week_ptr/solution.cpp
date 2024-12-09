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

    weak_ptr<B> relation;
};

class B {
public:
    void setRelation(shared_ptr<A> a) {
        relation = a;
    }
    ~B() {
        cout<<"Descructor of ~B called"<<endl;
    }

    weak_ptr<A> relation;
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
    
    // usage of lock
    {
        shared_ptr<int> sp1{new int{10}};
        {
            shared_ptr<int> sp2;
            weak_ptr<int> wp = sp1;
            cout<<"Use count before lock:"<<sp1.use_count()<<endl;
            // get weak_prt of existing sp
            sp2 = wp.lock();
            cout<<"pointed value:"<<*sp2<<endl;
            cout<<"Use count after lock:"<<sp2.use_count()<<endl;
            sp1.reset();
            cout<<"Use count after reset:"<<sp2.use_count()<<endl;
        }
        cout<<"Use count after week_ptr is ut of scope:"<<sp1.use_count()<<endl;

        {
            sp1.reset(new int{12});
            cout<<"Sp1 initialized again holds:"<<*sp1<<endl;
            weak_ptr<int> wp = sp1;
            sp1.reset();

            shared_ptr<int> sp2;
            // try to dereference using lock()
            sp2 = wp.lock();
            if (sp2) {
                cout<<"pointer is not empty (not expected)\n";
            } else {
                cout<<"pointer is empty (as expected)\n";
            }
            try {
                shared_ptr<int> invalidShared{wp};
            } catch (std::bad_weak_ptr e) {
                cout<<"Expected bad_weak_ptr:"<<e.what()<<endl;
            }
        }
    }
    
    return 0;
}