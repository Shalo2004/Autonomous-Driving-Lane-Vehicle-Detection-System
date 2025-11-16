#ifndef UTILS_H
#define UTILS_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include <memory>
#include <opencv2/opencv.hpp>

// Thread-safe queue for shared_ptr<cv::Mat>
template<typename T>
class ThreadSafeQueue {
public:
    ThreadSafeQueue(size_t maxSize = 8) : max_size(maxSize) {}
    void push(const std::shared_ptr<T>& item);
    std::shared_ptr<T> pop();
    bool try_pop(std::shared_ptr<T>& out);
    void clear();
    size_t size();
private:
    std::queue<std::shared_ptr<T>> q;
    std::mutex m;
    std::condition_variable cv;
    size_t max_size;
};

#endif // UTILS_H
