#include <iostream>
#include "Manager.hpp"
#include "Arithmetic.hpp"

int main() {
    Manager broker;

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

    return 0;
}
