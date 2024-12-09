#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

int main() {
    // Standard creation
    shared_ptr<string> pNico(new string("nico"));
    
    // not possible to initalize using assignment operator (conversion is blocked, baceuse constructor of shared_prt is explicit)
    // shared_ptr<string> pJutta = new string("jutta");

    // prefered initialization: make_shared does only one allocation: one for object and one form stared_ptr structure
    shared_ptr<string> pJutta = make_shared<string>("jutta");

    // pointer can be assigned later, using reset()
    shared_ptr<string> pJohn;
    // pJohn = new string("John"); // ERROR
    pJohn.reset(new string("John"));
    cout<<"Name: "<< *pNico <<"\n";

    // we used it like "normal" pointers
    (*pJutta)[0] = 'J';

    // vector stores copy of pointer
    vector<shared_ptr<string>> myFriends = {pNico, pJutta, pJohn, pNico, pJutta};

    for (auto &ptr : myFriends) {
        cout<<*ptr<<" use count:"<<ptr.use_count()<<endl;
    }
    cout<<"Sizeof pJutta="<<sizeof(pJutta)<<endl;

    // Tables: shared_ptr does not support tables: while deletion "delete" is called instead "delete []"
    // to make it work, "delete function" needs to be procided
    // shared_ptr<int []> p(new int[10]);   // ERROR
    // unique_ptr<int []> p(new int[10]);   // OK

    // this compiles but is not correct
    // shared_ptr<int> p(new int[10]); //
    shared_ptr<int> p(new int[10], [](int *p) { delete[] p; cout<<"Custom delete function for tables called\n"; } );
    cout<<"Sizeof p="<<sizeof(pJutta)<<" contains function for deletion"<<endl;
    return 0;
}