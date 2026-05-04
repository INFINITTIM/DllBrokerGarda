#pragma once
#include "Message.hpp"
#include "Manager.hpp"
#include <cstring>
#include <optional>

constexpr uint32_t kArithmStart = 500;
enum class ArithmOp : uint32_t {
    Add = kArithmStart,
    Sub
};

struct ArithmeticAddOpReq  { int64_t lh; int64_t rh; };
struct ArithmeticAddOpResp { int64_t sum; };

struct ArithmeticSubOpReq  { int64_t lh; int64_t rh; };
struct ArithmeticSubOpResp { int64_t diff; };

class ArithmeticServer {
public:
    explicit ArithmeticServer(Manager* broker) : broker_(broker) {

        broker_->provide(static_cast<uint32_t>(ArithmOp::Add),
            [](const interop::Message& req) -> std::unique_ptr<interop::Message> {

                ArithmeticAddOpReq args;
                std::memcpy(&args, req.payload.data(), sizeof(args));
                
                ArithmeticAddOpResp resp{args.lh + args.rh};
                
                return std::make_unique<interop::Message>(&resp, sizeof(resp));
            });

        broker_->provide(static_cast<uint32_t>(ArithmOp::Sub),
            [](const interop::Message& req) -> std::unique_ptr<interop::Message> {
                ArithmeticSubOpReq args;
                std::memcpy(&args, req.payload.data(), sizeof(args));
                
                ArithmeticSubOpResp resp{args.lh - args.rh};
                return std::make_unique<interop::Message>(&resp, sizeof(resp));
            });
    }
private:
    Manager* broker_;
};

class ArithmeticClient {
public:
    explicit ArithmeticClient(Manager* broker) : broker_(broker) {}

    std::optional<int64_t> add(int64_t lh, int64_t rh) {

        ArithmeticAddOpReq req{lh, rh};
        interop::Message msg(&req, sizeof(req));
        
        auto resp = broker_->request(static_cast<uint32_t>(ArithmOp::Add), msg);
        if (!resp) return std::nullopt;
        
        ArithmeticAddOpResp res;
        std::memcpy(&res, resp->payload.data(), sizeof(res));
        return res.sum;
    }

    std::optional<int64_t> sub(int64_t lh, int64_t rh) {
        ArithmeticSubOpReq req{lh, rh};
        interop::Message msg(&req, sizeof(req));
        
        auto resp = broker_->request(static_cast<uint32_t>(ArithmOp::Sub), msg);
        if (!resp) return std::nullopt;
        
        ArithmeticSubOpResp res;
        std::memcpy(&res, resp->payload.data(), sizeof(res));
        return res.diff;
    }

private:
    Manager* broker_;
};
