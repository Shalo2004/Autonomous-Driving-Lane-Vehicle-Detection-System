#include "video_pipeline.h"
#include <thread>
#include <iostream>

int main(int argc, char** argv) {
    std::string source = "data/sample_video.mp4";
    std::string cascade_path = "";
    std::string onnx_path = "";

    if (argc >= 2) source = argv[1];
    if (argc >= 3) cascade_path = argv[2];
    if (argc >= 4) onnx_path = argv[3];

    PipelineQueues queues;
    PipelineControl ctrl;

    // start threads
    std::thread t_capture(capture_thread_func, source, &queues, &ctrl);
    std::thread t_lane(lane_thread_func, &queues, &ctrl);
    std::thread t_vehicle(vehicle_thread_func, &queues, &ctrl, cascade_path, onnx_path);
    std::thread t_display(display_thread_func, &queues, &ctrl);

    std::cout << "[main] Press ESC in display window to quit.\n";
    // Wait for threads
    t_capture.join();
    // when capture stops, signal others to end gracefully
    ctrl.stop = true;
    // unblocking pushes if necessary: we don't do extra pushes here to keep it simple

    t_lane.join();
    t_vehicle.join();
    t_display.join();

    std::cout << "[main] Exiting\n";
    return 0;
}
