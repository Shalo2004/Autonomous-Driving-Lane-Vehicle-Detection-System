#include "utils.h"

template<typename T>
void ThreadSafeQueue<T>::push(const std::shared_ptr<T>& item) {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&]() { return q.size() < max_size; });
    q.push(item);
    lk.unlock();
    cv.notify_all();
}

template<typename T>
std::shared_ptr<T> ThreadSafeQueue<T>::pop() {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk, [&]() { return !q.empty(); });
    auto v = q.front(); q.pop();
    lk.unlock();
    cv.notify_all();
    return v;
}

template<typename T>
bool ThreadSafeQueue<T>::try_pop(std::shared_ptr<T>& out) {
    std::unique_lock<std::mutex> lk(m);
    if (q.empty()) return false;
    out = q.front(); q.pop();
    lk.unlock();
    cv.notify_all();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::clear() {
    std::unique_lock<std::mutex> lk(m);
    while (!q.empty()) q.pop();
    lk.unlock();
    cv.notify_all();
}

template<typename T>
size_t ThreadSafeQueue<T>::size() {
    std::unique_lock<std::mutex> lk(m);
    return q.size();
}

// Explicit instantiation for cv::Mat so we can compile utils.cpp
template class ThreadSafeQueue<cv::Mat>;
