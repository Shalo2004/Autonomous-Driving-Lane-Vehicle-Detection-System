#include "vehicle_detector.h"
#include <iostream>

VehicleDetector::VehicleDetector() : use_cascade(false), use_onnx(false) {
    bgsub = cv::createBackgroundSubtractorMOG2(500, 16, true);
}

bool VehicleDetector::init(const std::string& cascade_path, const std::string& onnx_path) {
    if (!cascade_path.empty()) {
        if (cascade.load(cascade_path)) {
            use_cascade = true;
            std::cout << "[vehicle] Cascade loaded: " << cascade_path << std::endl;
            return true;
        } else {
            std::cerr << "[vehicle] Failed to load cascade: " << cascade_path << std::endl;
        }
    }
    // ONNX pathway - left as stub (requires ONNX Runtime integration)
    if (!onnx_path.empty()) {
        // TODO: integrate ONNX Runtime; here we return false to fall back
        std::cerr << "[vehicle] ONNX path provided but ONNX Runtime not integrated in this build.\n";
    }
    // fallback to motion-based detection
    std::cout << "[vehicle] No cascade/onnx available - using background subtraction fallback\n";
    use_cascade = false;
    use_onnx = false;
    return false;
}

cv::Mat VehicleDetector::detect_and_draw(const cv::Mat& frame) {
    cv::Mat out; frame.copyTo(out);
    if (use_cascade) {
        std::vector<cv::Rect> cars;
        cv::Mat gray; cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        cascade.detectMultiScale(gray, cars, 1.1, 3, 0, cv::Size(30,30));
        for (auto &r : cars) {
            cv::rectangle(out, r, cv::Scalar(0,0,255), 2);
            cv::putText(out, "Vehicle", cv::Point(r.x, r.y-5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0,0,255),2);
        }
        return out;
    } else if (use_onnx) {
        // stub - not implemented in this template
        return out;
    } else {
        // background subtraction + contour detection
        cv::Mat fg;
        bgsub->apply(frame, fg);
        cv::erode(fg, fg, cv::Mat(), cv::Point(-1,-1), 1);
        cv::dilate(fg, fg, cv::Mat(), cv::Point(-1,-1), 2);
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(fg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        for (auto &c : contours) {
            double area = cv::contourArea(c);
            if (area > area_threshold) {
                cv::Rect r = cv::boundingRect(c);
                cv::rectangle(out, r, cv::Scalar(255,0,0), 2);
                cv::putText(out, "Vehicle-move", cv::Point(r.x, r.y-5), cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255,0,0),2);
            }
        }
        return out;
    }
}
