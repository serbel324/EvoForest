#include "message_queue.h"

void LocalMessageQueue::Push(MessageWrapper::SPtr msg) {
    _messages.push_back(std::move(msg));
}

MessageWrapper::SPtr LocalMessageQueue::PullOne() {
    if (IsEmpty()) {
        return nullptr;
    }

    MessageWrapper::SPtr front = std::move(_messages.front());
    _messages.pop_front();
    return front;
}

std::vector<MessageWrapper::SPtr> LocalMessageQueue::PullAll() {
    std::vector<MessageWrapper::SPtr> res;

    while (!IsEmpty()) {
        res.push_back(std::move(_messages.front()));
        _messages.pop_front();
    }
    return std::move(res);
}

bool LocalMessageQueue::IsEmpty() {
    return _messages.empty();
}


void ProtectedMessageQueue::Push(MessageWrapper::SPtr msg) {
    std::lock_guard g(_mtx);
    _messages.push_back(std::move(msg));
}

MessageWrapper::SPtr ProtectedMessageQueue::PullOne() {
    std::lock_guard g(_mtx);
    if (_messages.empty()) {
        return nullptr;
    }

    MessageWrapper::SPtr front = std::move(_messages.front());
    _messages.pop_front();
    return front;
}

std::vector<MessageWrapper::SPtr> ProtectedMessageQueue::PullAll() {
    std::lock_guard g(_mtx);
    std::vector<MessageWrapper::SPtr> res;

    while (!_messages.empty()) {
        res.push_back(std::move(_messages.front()));
        _messages.pop_front();
    }
    return std::move(res);
}

bool ProtectedMessageQueue::IsEmpty() {
    std::lock_guard g(_mtx);
    return _messages.empty();
}
