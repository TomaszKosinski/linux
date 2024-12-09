#include <iostream>
#include <memory>

class MyClass : public std::enable_shared_from_this<MyClass> {
public:
    void show() {
        std::cout << "MyClass object at " << this << std::endl;
    }

    // Funkcja, która zwraca shared_ptr do siebie
    std::shared_ptr<MyClass> get_shared_ptr() {
        // Zwraca shared_ptr do tego samego obiektu
        return shared_from_this();
    }
};

int main() {
    // Tworzymy obiekt za pomocą make_shared
    std::shared_ptr<MyClass> ptr1 = std::make_shared<MyClass>();

    // Uzyskujemy shared_ptr do obiektu przez metodę
    std::shared_ptr<MyClass> ptr2 = ptr1->get_shared_ptr();

    ptr1->show();
    ptr2->show();
    // Obie zmienne ptr1 i ptr2 zarządzają tym samym obiektem

    try {
        MyClass myClass;
        std::shared_ptr<MyClass> ptr3 = myClass.get_shared_ptr();
    } catch (std::bad_weak_ptr e) {
        std::cout<<"Exception is thrown because object is not managed by shared_prt\n";
    }

    return 0;
}