#include <gtest/gtest.h>
#include "Manager.hpp"
#include "Arithmetic.hpp"

TEST(BrokerTest, ProvideAndRequest) {
    Manager broker;
    broker.start();
    
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
    broker.start();
    interop::Message msg;
    EXPECT_EQ(broker.request(12345, msg), nullptr);
}

TEST(ArithmeticTest, AddPositiveNumbers) {
    Manager broker;
    broker.start();
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    auto result = client.add(10, 5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 15);
}

TEST(ArithmeticTest, SubPositiveNumbers) {
    Manager broker;
    broker.start();
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    auto result = client.sub(10, 5);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 5);
}

TEST(ArithmeticTest, NegativeNumbers) {
    Manager broker;
    broker.start();
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);
    
    EXPECT_EQ(client.add(-5, 3), -2);
    EXPECT_EQ(client.sub(0, 10), -10);
}

// Дополнительные тесты на старт и стоп брокера

TEST(BrokerTest, StartStopLifecycle) {
    Manager broker;

    EXPECT_FALSE(broker.is_running());

    EXPECT_TRUE(broker.start());
    EXPECT_TRUE(broker.is_running());

    EXPECT_FALSE(broker.start());

    EXPECT_TRUE(broker.stop());
    EXPECT_FALSE(broker.is_running());

    EXPECT_FALSE(broker.stop());
}

TEST(BrokerTest, RequestFailsWhenStopped) {
    Manager broker;

    interop::Message msg;
    EXPECT_EQ(broker.request(999, msg), nullptr);

    broker.start();
    broker.provide(999, [](const interop::Message& req) {
        return std::make_unique<interop::Message>(req.payload.data(), req.payload.size());
    });
    
    uint8_t data[] = {42};
    interop::Message req(data, sizeof(data));
    auto resp = broker.request(999, req);
    EXPECT_NE(resp, nullptr);

    broker.stop();

    EXPECT_EQ(broker.request(999, req), nullptr);
}

TEST(ArithmeticTest, FailsAfterBrokerStop) {
    Manager broker;
    broker.start();
    
    ArithmeticServer server(&broker);
    ArithmeticClient client(&broker);

    EXPECT_EQ(client.add(2, 3), 5);

    broker.stop();

    EXPECT_FALSE(client.add(2, 3).has_value());
    EXPECT_FALSE(client.sub(5, 1).has_value());
}