#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>
#include "Message.hpp"

// интерфейс для различных видов транспорта 
class InterfaceTransport {
public:
    virtual ~InterfaceTransport() = default;
    //регистрирует (задаёт) обработчики для какой-то конкретной операции
    virtual void set_handler(uint32_t op_id, interop::TFuncOpReq handler) = 0;
    // выполняет запрос через транспортный уровень
    virtual std::unique_ptr<interop::Message> request(uint32_t op_id, const interop::Message& msg) = 0;
};

// представленный как пример базовый транспорт - базовая доставка 
class Transport : public InterfaceTransport {
private:
    std::unordered_map<uint32_t, interop::TFuncOpReq> handlers_;
public:
    void set_handler(uint32_t op_id, interop::TFuncOpReq handler) override {
        handlers_[op_id] = std::move(handler);
    }
    std::unique_ptr<interop::Message> request(uint32_t op_id, const interop::Message& msg) override {
        auto it = handlers_.find(op_id);
        if (it == handlers_.end()) return nullptr;
        return it->second(msg);
    }
};

class Manager {
private:
    // указатель на вид транспорта (транспортного уровня)
    std::unique_ptr<InterfaceTransport> transport_;
    // флаг состояния
    bool running_;
public:
    explicit Manager(std::unique_ptr<InterfaceTransport> transport = std::make_unique<Transport>());
    
    bool start();
    bool stop();
    bool is_running() const;
    bool provide(uint32_t op_id, interop::TFuncOpReq handler);
    std::unique_ptr<interop::Message> request(uint32_t op_id, const interop::Message& msg);
};
