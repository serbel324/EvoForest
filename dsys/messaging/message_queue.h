#pragma once

#include <deque>
#include <memory>
#include <vector>

#include <dsys/messaging/message.h>

template<class T>
class Queue {
public:
    using SPtr = std::shared_ptr<Queue>;

public:
    ~Queue() = default;

    virtual void Push(T elem) = 0;
    virtual T PullOne() = 0;
    virtual std::vector<T> PullAll() = 0;

    virtual bool IsEmpty() = 0;
};

using MessageQueue = Queue<MessageWrapper::SPtr>;

class LocalMessageQueue : public MessageQueue {
public:
    void Push(MessageWrapper::SPtr msg) override;
    MessageWrapper::SPtr PullOne() override;
    std::vector<MessageWrapper::SPtr> PullAll() override;
    bool IsEmpty() override;

private:
    std::deque<MessageWrapper::SPtr> _messages;
};

class ProtectedMessageQueue : public MessageQueue {
public:
    void Push(MessageWrapper::SPtr msg) override;
    MessageWrapper::SPtr PullOne() override;
    std::vector<MessageWrapper::SPtr> PullAll() override;
    bool IsEmpty() override;

private:
    std::deque<MessageWrapper::SPtr> _messages;
    std::mutex _mtx;
};
