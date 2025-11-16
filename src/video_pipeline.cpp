#include "video_pipeline.h"
#include "lane_detector.h"
#include "vehicle_detector.h"
#include <opencv2/opencv.hpp>
#include <chrono>
#include <iostream>

using namespace std::chrono;

void capture_thread_func(const std::string& source, PipelineQueues* queues, PipelineControl* ctrl) {
    cv::VideoCapture cap;
    if (source == "0" || source == "camera") cap.open(0);
    else cap.open(source);

    if (!cap.isOpened()) {
        std::cerr << "[capture] Cannot open source: " << source << std::endl;
        ctrl->stop = true;
        return;
    }

    double fps = cap.get(cv::CAP_PROP_FPS);
    if (fps <= 0 || fps > 120) fps = 30;
    std::cout << "[capture] Source opened, camera fps ~ " << fps << std::endl;

    while (!ctrl->stop) {
        auto frame = std::make_shared<cv::Mat>();
        if (!cap.read(*frame)) { ctrl->stop = true; break; }
        queues->capture_queue.push(frame);
        // throttle - small sleep to avoid hogging CPU (capture handles actual rate)
        std::this_thread::sleep_for(milliseconds(1));
    }
    cap.release();
    std::cout << "[capture] stopped\n";
}

void lane_thread_func(PipelineQueues* queues, PipelineControl* ctrl) {
    LaneDetector detector;
    while (!ctrl->stop) {
        auto frame = queues->capture_queue.pop();
        if (!frame) continue;
        cv::Mat lane_out = detector.detect_and_draw(*frame);
        auto out_ptr = std::make_shared<cv::Mat>(lane_out);
        queues->lane_queue.push(out_ptr);
    }
    std::cout << "[lane] stopped\n";
}

void vehicle_thread_func(PipelineQueues* queues, PipelineControl* ctrl, const std::string& cascade_path, const std::string& onnx_path) {
    VehicleDetector vdet;
    bool ok = vdet.init(cascade_path, onnx_path);
    if (!ok) std::cout << "[vehicle] running in fallback (motion) mode\n";
    while (!ctrl->stop) {
        auto frame = queues->lane_queue.pop();
        if (!frame) continue;
        cv::Mat veh_out = vdet.detect_and_draw(*frame);
        auto out_ptr = std::make_shared<cv::Mat>(veh_out);
        queues->vehicle_queue.push(out_ptr);
    }
    std::cout << "[vehicle] stopped\n";
}

void display_thread_func(PipelineQueues* queues, PipelineControl* ctrl) {
    using clock = high_resolution_clock;
    int frame_count = 0;
    auto t0 = clock::now();
    while (!ctrl->stop) {
        auto frame_ptr = queues->vehicle_queue.pop();
        if (!frame_ptr) continue;
        frame_count++;
        auto now = clock::now();
        double sec = duration_cast<duration<double>>(now - t0).count();
        if (sec >= 1.0) {
            ctrl->fps = frame_count;
            frame_count = 0;
            t0 = now;
        }
        cv::Mat disp = *frame_ptr;
        cv::putText(disp, "FPS: " + std::to_string(ctrl->fps.load()), cv::Point(10,30),
                    cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0,255,0), 2);
        cv::imshow("AutonomousVision", disp);
        int k = cv::waitKey(1);
        if (k == 27) { ctrl->stop = true; break; } // ESC
    }
    cv::destroyAllWindows();
    std::cout << "[display] stopped\n";
}
