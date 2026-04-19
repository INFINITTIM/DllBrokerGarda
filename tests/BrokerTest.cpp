#include <gtest/gtest.h>
#include "Manager.hpp"
#include "Arithmetic.hpp"

TEST(BrokerTest, ProvideAndRequest) {
    Manager broker;
    
    broker.provide(999, [](const interop::Message& req) {
        return std::make_unique<interop::Message>(req.payload.data(), req.payload.size());
    });
    
    uint8_t data[] = {1, 2, 3};
    interop::Message msg(data, sizeof(data));
    auto resp = broker.request(999, msg);
    
    ASSERT_NE(resp, nullptr);
    ASSERT_EQ(resp->payload.size(), 3);
    EXPECT_EQ(resp->payload[0], 1);
    EXPECT_EQ(resp->payload[1], 2);
    EXPECT_EQ(resp->payload[2], 3);
}

TEST(BrokerTest, UnknownOpReturnsNull) {
    Manager broker;
    interop::Message msg;
    EXPECT_EQ(broker.request(12345, msg), nullptr);
}

TEST(ArithmeticTest, AddPositiveNumbers) {
    Manager broker;
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    auto result = client.add(10, 5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 15);
}

TEST(ArithmeticTest, SubPositiveNumbers) {
    Manager broker;
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    auto result = client.sub(10, 5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 5);
}

TEST(ArithmeticTest, NegativeNumbers) {
    Manager broker;
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    EXPECT_EQ(client.add(-5, 3), -2);
    EXPECT_EQ(client.sub(0, 10), -10);
}
