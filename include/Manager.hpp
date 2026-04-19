#pragma once
#include <unordered_map>
#include "Message.hpp"

class Manager {
private:
    std::unordered_map<uint32_t, interop::TFuncOpReq> handlers_;

public:
    bool provide(uint32_t op_id, interop::TFuncOpReq handler);

    std::unique_ptr<interop::Message> request(uint32_t op_id, const interop::Message& msg);
};
