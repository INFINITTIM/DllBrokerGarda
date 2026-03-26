#include <iostream>
#include "../include/Module.hpp"
#include "../include/Manager.hpp"

enum class MathEvents : EventTypeID {
    MATH_RESULT = 100
};

class AddModule : public Module {
public:
    AddModule(Manager* mgr) : Module(mgr, "Сложение") {}
    
    void calculate(int a, int b) {
        int result = a + b;
        std::string msg = std::to_string(a) + " + " + std::to_string(b) 
                        + " = " + std::to_string(result);
        send(static_cast<EventTypeID>(MathEvents::MATH_RESULT), msg);
    }
};

class SubtractModule : public Module {
public:
    SubtractModule(Manager* mgr) : Module(mgr, "Вычитание") {}
    
    void calculate(int a, int b) {
        int result = a - b;
        std::string msg = std::to_string(a) + " - " + std::to_string(b) 
                        + " = " + std::to_string(result);
        send(static_cast<EventTypeID>(MathEvents::MATH_RESULT), msg);
    }
};

class MultiplyModule : public Module {
public:
    MultiplyModule(Manager* mgr) : Module(mgr, "Умножение") {}
    
    void calculate(int a, int b) {
        int result = a * b;
        std::string msg = std::to_string(a) + " * " + std::to_string(b) 
                        + " = " + std::to_string(result);
        send(static_cast<EventTypeID>(MathEvents::MATH_RESULT), msg);
    }
};

class LoggerModule : public Module {
public:
    LoggerModule(Manager* mgr) : Module(mgr, "Логгер") {
        subscribe(static_cast<EventTypeID>(MathEvents::MATH_RESULT));
    }
};

int main() { 

    std::cout << "\n===================ИНИЦАЛИЗАЦИЯ=======================\n";

    Manager mgr;
    
    AddModule add(&mgr);
    SubtractModule sub(&mgr);
    MultiplyModule mul(&mgr);
    LoggerModule logger(&mgr);
    
    std::cout << "\n=======================ПОДСЧЁТ========================\n";

    std::cout << "\n--- Сложение ---\n";
    add.calculate(10, 5);
    
    std::cout << "\n--- Вычитание ---\n";
    sub.calculate(10, 5);
    
    std::cout << "\n--- Умножение ---\n";
    mul.calculate(10, 5);
    
    std::cout << "\n=================РЕЗУЛЬТАТЫ=ОТПРАВКИ===================\\n";

    std::cout << "\n--- Логгер ---\n";
    logger.read_messages();

    std::cout << "\n--- Сложение ---\n";
    add.read_messages();
    
    std::cout << "\n--- Вычитание ---\n";
    sub.read_messages();
    
    std::cout << "\n--- Умножение ---\n";
    mul.read_messages();
    
    return 0;
}