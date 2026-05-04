#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <memory>

namespace interop {

struct Message {
    std::vector<uint8_t> payload;

    Message() = default;

    Message(const void* data, size_t size) 
        : payload(static_cast<const uint8_t*>(data), 
                  static_cast<const uint8_t*>(data) + size) {}
};

using TFuncOpReq = std::function<std::unique_ptr<Message>(const Message&)>;

} // namespace interop
