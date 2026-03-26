#include <gtest/gtest.h>
#include "../include/Module.hpp"
#include "../include/Manager.hpp"
#include "../include/Event.hpp"

enum class TestEvents : EventTypeID {
    EVENT_A = 1,
    EVENT_B = 2,
    EVENT_C = 3
};

class TestModule : public Module {
public:
    int messageCount = 0;
    std::vector<std::string> receivedMessages;
    
    TestModule(Manager* mgr, const std::string& name) 
        : Module(mgr, name) {}
    
    void read_messages() {
        while (!inbox_message.empty()) {
            Event ev = inbox_message.front();
            inbox_message.pop();
            messageCount++;
            receivedMessages.push_back(ev.message);
        }
    }
};

TEST(BrokerTest, ModuleCreation) {
    Manager mgr;
    TestModule module(&mgr, "TestModule");
    EXPECT_EQ(module.getName(), "TestModule");
}

TEST(BrokerTest, SubscribeToEvent) {
    Manager mgr;
    TestModule module(&mgr, "TestModule");
    
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    TestModule publisher(&mgr, "Publisher");
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Test");
    
    module.read_messages();
    
    EXPECT_EQ(module.messageCount, 1);
}

TEST(BrokerTest, UnsubscribeFromEvent) {
    Manager mgr;
    TestModule module(&mgr, "TestModule");
    
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    module.unsubscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    TestModule publisher(&mgr, "Publisher");
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Test");
    
    module.read_messages();
    
    EXPECT_EQ(module.messageCount, 0);
}

TEST(BrokerTest, MessageDeliveryToSingleSubscriber) {
    Manager mgr;
    TestModule publisher(&mgr, "Publisher");
    TestModule subscriber(&mgr, "Subscriber");
    
    subscriber.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Hello!");
    
    subscriber.read_messages();
    
    EXPECT_EQ(subscriber.messageCount, 1);
    EXPECT_EQ(subscriber.receivedMessages[0], "Hello!");
}

TEST(BrokerTest, MessageDeliveryToMultipleSubscribers) {
    Manager mgr;
    TestModule publisher(&mgr, "Publisher");
    TestModule sub1(&mgr, "Subscriber1");
    TestModule sub2(&mgr, "Subscriber2");
    TestModule sub3(&mgr, "Subscriber3");
    
    sub1.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    sub2.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    sub3.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Broadcast");
    
    sub1.read_messages();
    sub2.read_messages();
    sub3.read_messages();
    
    EXPECT_EQ(sub1.messageCount, 1);
    EXPECT_EQ(sub2.messageCount, 1);
    EXPECT_EQ(sub3.messageCount, 1);
}

TEST(BrokerTest, SenderDoesNotReceiveOwnMessage) {
    Manager mgr;
    TestModule module(&mgr, "Module");
    
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    module.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Self message");
    
    module.read_messages();
    
    EXPECT_EQ(module.messageCount, 0);
}

TEST(BrokerTest, UnsubscribedModuleDoesNotReceive) {
    Manager mgr;
    TestModule publisher(&mgr, "Publisher");
    TestModule nonSubscriber(&mgr, "NonSubscriber");
    
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Message");
    
    nonSubscriber.read_messages();
    
    EXPECT_EQ(nonSubscriber.messageCount, 0);
}

TEST(BrokerTest, DifferentEventTypesAreIsolated) {
    Manager mgr;
    TestModule pub(&mgr, "Publisher");
    TestModule subA(&mgr, "SubscriberA");
    TestModule subB(&mgr, "SubscriberB");
    
    subA.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    subB.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_B));
    
    pub.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "For A");
    pub.send(static_cast<EventTypeID>(TestEvents::EVENT_B), "For B");
    
    subA.read_messages();
    subB.read_messages();
    
    EXPECT_EQ(subA.messageCount, 1);
    EXPECT_EQ(subA.receivedMessages[0], "For A");
    
    EXPECT_EQ(subB.messageCount, 1);
    EXPECT_EQ(subB.receivedMessages[0], "For B");
}

TEST(BrokerTest, MultipleMessagesInQueue) {
    Manager mgr;
    TestModule publisher(&mgr, "Publisher");
    TestModule subscriber(&mgr, "Subscriber");
    
    subscriber.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Msg 1");
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Msg 2");
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Msg 3");
    
    subscriber.read_messages();
    
    EXPECT_EQ(subscriber.messageCount, 3);
    EXPECT_EQ(subscriber.receivedMessages[0], "Msg 1");
    EXPECT_EQ(subscriber.receivedMessages[1], "Msg 2");
    EXPECT_EQ(subscriber.receivedMessages[2], "Msg 3");
}

TEST(BrokerTest, NoDuplicateSubscriptions) {
    Manager mgr;
    TestModule module(&mgr, "TestModule");
    
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    module.subscribe(static_cast<EventTypeID>(TestEvents::EVENT_A));
    
    TestModule publisher(&mgr, "Publisher");
    publisher.send(static_cast<EventTypeID>(TestEvents::EVENT_A), "Test");
    
    module.read_messages();
    
    EXPECT_EQ(module.messageCount, 1);
}
