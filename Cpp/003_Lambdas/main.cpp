#include <iostream>

using namespace std;

int main() {
    
    int i = 10;
    [] () {cout<<"Hello World\n";} () ;

    // lambda which gets one argument
    [] (int a) {cout<<"Value:"<<a<<endl;} (100) ;
    
    // lambda which gets one argument by value from main
    [] (int &a) {cout<<"Value:"<<a<<endl;} (i) ;

    // lambda which gets one argument by value from main, try to inctement
    [] (int &a) {a++;cout<<"Value:"<<a<<endl;} (i) ;

    // capture local scope variable by value
    [i] () {cout<<"Value:"<<i<<endl;} ();

    // capture local scope variable by value (mutable is needed)
    [i] () mutable {++i;cout<<"Value:"<<i<<endl;} ();
    cout<<"But outside lambda the value is still:"<<i<<endl;

    // capture local scope variable by value (mutable is needed)
    [&i] () mutable {++i;cout<<"Value:"<<i<<endl;} ();
    cout<<"Outside lambda the value is incremented:"<<i<<endl;

    // lambda which returns value which type is deducted by compiler
    auto sum = [] (int a, int b) { return a+b; } (10, 20);
    cout<<"calculated sum:"<<sum<<endl;

    // lambda which returns value which type is defined
    float div = [] (int a, int b) -> double { return static_cast<double>(a)/b; } (10, 3);
    cout<<"calculated div:"<<div<<endl;

    return 0;
}