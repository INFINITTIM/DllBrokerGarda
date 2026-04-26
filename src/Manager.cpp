#include "Manager.hpp"

Manager::Manager() : running_(false) {}

bool Manager::start() {
    if (running_) return false;
    
    running_ = true;

    return true;
}

bool Manager::stop() {
    if (!running_) return false;
    
    running_ = false;
    
    handlers_.clear();
    
    return true;
}

bool Manager::is_running() const {
    return running_;
}

bool Manager::provide(uint32_t op_id, interop::TFuncOpReq handler) {
    if (!handler) return false;
    if (!running_) return false;
    
    handlers_[op_id] = std::move(handler);
    return true;
}

std::unique_ptr<interop::Message> Manager::request(uint32_t op_id, const interop::Message& msg) {
    if (!running_) return nullptr;
    
    auto it = handlers_.find(op_id);
    if (it == handlers_.end()) {
        return nullptr;
    }
    return it->second(msg);
}
