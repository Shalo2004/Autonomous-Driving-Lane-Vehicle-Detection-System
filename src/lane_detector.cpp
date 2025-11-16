#include "lane_detector.h"
#include <algorithm>

cv::Mat LaneDetector::roi_mask(const cv::Mat& img) {
    cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC1);  // MUST be 1-channel

    int h = img.rows, w = img.cols;
    std::vector<cv::Point> poly = {
        cv::Point(w*0.1, h*0.95),
        cv::Point(w*0.4, h*0.6),
        cv::Point(w*0.6, h*0.6),
        cv::Point(w*0.9, h*0.95)
    };

    cv::fillConvexPoly(mask, poly, cv::Scalar(255));  // grayscale mask

    return mask;
}


std::vector<cv::Vec4i> LaneDetector::detect_lines(const cv::Mat& edges) {
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(edges, lines, 1, CV_PI/180, 50, 50, 15);
    return lines;
}

cv::Mat LaneDetector::draw_lines(const cv::Mat& img, const std::vector<cv::Vec4i>& lines) {
    cv::Mat out;
    img.copyTo(out);
    for (auto &l : lines) {
        cv::line(out, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,255,0), 3);
    }
    return out;
}

cv::Mat LaneDetector::detect_and_draw(const cv::Mat& frame) {
    cv::Mat gray, blur, edges;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blur, cv::Size(5,5), 1.5);
    cv::Canny(blur, edges, 50, 150);

    cv::Mat mask = roi_mask(frame);
    cv::Mat masked_edges;
    edges.copyTo(masked_edges, mask(cv::Rect(0,0,mask.cols, mask.rows)));

    auto lines = detect_lines(masked_edges);
    cv::Mat out = draw_lines(frame, lines);
    return out;
}
