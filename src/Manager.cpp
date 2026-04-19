#include "Manager.hpp"

bool Manager::provide(uint32_t op_id, interop::TFuncOpReq handler) {
    if (!handler) return false;
    // здесь мы вохраняем обработчик в таблицу (или перезаписываем, если уже есть)
    handlers_[op_id] = std::move(handler);
    return true;
}

std::unique_ptr<interop::Message> Manager::request(uint32_t op_id, const interop::Message& msg) {
    // тут мы получается ищем обработчик по коду операции
    auto it = handlers_.find(op_id);
    if (it == handlers_.end()) {
        return nullptr; // ошибка в случае если операция не найден
    }
    // вызываем обработчик и возвращаем результат
    return it->second(msg);
}
