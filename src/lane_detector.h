#ifndef LANE_DETECTOR_H
#define LANE_DETECTOR_H

#include <opencv2/opencv.hpp>

class LaneDetector {
public:
    LaneDetector() = default;
    // process frame and return annotated BGR image
    cv::Mat detect_and_draw(const cv::Mat& frame);
private:
    cv::Mat roi_mask(const cv::Mat& img);
    std::vector<cv::Vec4i> detect_lines(const cv::Mat& edges);
    cv::Mat draw_lines(const cv::Mat& img, const std::vector<cv::Vec4i>& lines);
};

#endif // LANE_DETECTOR_H
