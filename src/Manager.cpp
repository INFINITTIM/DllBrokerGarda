#include "Manager.hpp"

Manager::Manager(std::unique_ptr<InterfaceTransport> transport)
    : transport_(std::move(transport)), running_(false) {}

bool Manager::start() {
    if (running_) return false;
    running_ = true;
    return true;
}

bool Manager::stop() {
    if (!running_) return false;
    running_ = false;
    return true;
}

bool Manager::is_running() const { return running_; }

bool Manager::provide(uint32_t op_id, interop::TFuncOpReq handler) {
    if (!handler) return false;
    transport_->set_handler(op_id, std::move(handler));
    return true;
}

std::unique_ptr<interop::Message> Manager::request(uint32_t op_id, const interop::Message& msg) {
    if (!running_) return nullptr;
    return transport_->request(op_id, msg);
}
