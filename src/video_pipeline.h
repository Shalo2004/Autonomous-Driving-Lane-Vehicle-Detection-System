#ifndef VIDEO_PIPELINE_H
#define VIDEO_PIPELINE_H

#include "utils.h"
#include <atomic>
#include <string>

// Shared queues using cv::Mat pointers (zero-copy)
using FramePtr = std::shared_ptr<cv::Mat>;

struct PipelineQueues {
    ThreadSafeQueue<cv::Mat> capture_queue{8};
    ThreadSafeQueue<cv::Mat> lane_queue{8};
    ThreadSafeQueue<cv::Mat> vehicle_queue{8};
};

struct PipelineControl {
    std::atomic<bool> stop{false};
    std::atomic<int> fps{0};
};

void capture_thread_func(const std::string& source, PipelineQueues* queues, PipelineControl* ctrl);
void lane_thread_func(PipelineQueues* queues, PipelineControl* ctrl);
void vehicle_thread_func(PipelineQueues* queues, PipelineControl* ctrl, const std::string& cascade_path="", const std::string& onnx_path="");
void display_thread_func(PipelineQueues* queues, PipelineControl* ctrl);

#endif // VIDEO_PIPELINE_H
