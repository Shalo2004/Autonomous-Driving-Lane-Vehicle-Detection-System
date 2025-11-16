#ifndef VEHICLE_DETECTOR_H
#define VEHICLE_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <string>

class VehicleDetector {
public:
    VehicleDetector();
    // init returns true if cascade or onnx loaded, false if fallback (motion)
    bool init(const std::string& cascade_path="", const std::string& onnx_path="");
    cv::Mat detect_and_draw(const cv::Mat& frame);
private:
    bool use_cascade;
    bool use_onnx;
    cv::CascadeClassifier cascade;
    // fallback motion detector
    cv::Ptr<cv::BackgroundSubtractor> bgsub;
    double area_threshold = 500.0;

    // placeholder for ONNX variables if user links ONNX Runtime
    // (not used by default)
};

#endif // VEHICLE_DETECTOR_H
