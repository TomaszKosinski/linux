#include <iostream>
#include <memory>

using namespace std;

struct CustomDeleterWithData {
    int data;
    void operator()(int* ptr) { delete ptr; }
};
struct CustomDeleterWithoutData {
    void operator()(int* ptr) { delete ptr; }
};

int main() {
    std::default_delete<int> d;
    std::unique_ptr<int> u1;
    std::unique_ptr<int> u2 (nullptr);
    std::unique_ptr<int> u3 (new int);
    std::unique_ptr<int> u4 (new int, d);
    std::unique_ptr<int> u5 (new int, std::default_delete<int>());
    std::unique_ptr<int> u6 (std::move(u5));
    std::unique_ptr<int> u7 (std::move(u6));
    std::unique_ptr<int> u8 (std::auto_ptr<int>(new int));

    std::cout << "u1: " << (u1?"not null":"null") << " sizeof="<<sizeof(u1)<<'\n';
    std::cout << "u2: " << (u2?"not null":"null") << " sizeof="<<sizeof(u2)<<'\n';
    std::cout << "u3: " << (u3?"not null":"null") << " sizeof="<<sizeof(u3)<<'\n';
    std::cout << "u4: " << (u4?"not null":"null") << " sizeof="<<sizeof(u4)<<'\n';
    std::cout << "u5: " << (u5?"not null":"null") << " sizeof="<<sizeof(u5)<<'\n';
    std::cout << "u6: " << (u6?"not null":"null") << " sizeof="<<sizeof(u6)<<'\n';
    std::cout << "u7: " << (u7?"not null":"null") << " sizeof="<<sizeof(u7)<<'\n';
    std::cout << "u8: " << (u8?"not null":"null") << " sizeof="<<sizeof(u8)<<'\n';

    cout << "Sizeof(*int):"<<sizeof(int*)<<'\n';
    
    // size of this one is not increaset because CustomDeleterWithoutData doesn't have any data
    std::unique_ptr<int, CustomDeleterWithoutData> u9(new int);
    // size of this one is increaset because CustomDeleterWithData have data
    std::unique_ptr<int, CustomDeleterWithData> u10(new int);
    std::cout << "u9: " << (u9?"not null":"null") << " sizeof="<<sizeof(u9)<<'\n';
    std::cout << "u10: " << (u10?"not null":"null") << " sizeof="<<sizeof(u10)<<'\n';
};
