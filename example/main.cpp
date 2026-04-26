#include <iostream>
#include "Manager.hpp"
#include "Arithmetic.hpp"

int main() {
    Manager broker;
    
    broker.start();
    
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    std::cout << "=== RPC Test ===" << std::endl;

    auto result_add = client.add(10, 4);
    if (result_add) {
        std::cout << "10 + 4 = " << *result_add << std::endl;
    }
    
    auto result_sub = client.sub(10, 4);
    if (result_sub) {
        std::cout << "10 - 4 = " << *result_sub << std::endl;
    }
    
    broker.stop();

    auto result_after_stop = client.add(1, 1);
    if (!result_after_stop) {
        std::cout << "Всё работает корректно!\n";
    }

    return 0;
}
