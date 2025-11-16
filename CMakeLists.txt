cmake_minimum_required(VERSION 3.10)
project(AutonomousVision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# -----------------------------
# OpenCV
# -----------------------------
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# -----------------------------
# ONNX Runtime (your folder)
# -----------------------------
set(ONNXRUNTIME_DIR "C:/onnxruntime-win-x64-1.23.2")

include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/lib)

# -----------------------------
# Source Files
# -----------------------------
include_directories(src)

add_executable(autovision
    src/main.cpp
    src/video_pipeline.cpp
    src/lane_detector.cpp
    src/vehicle_detector.cpp
    src/utils.cpp
)

# -----------------------------
# Link Libraries
# -----------------------------
target_link_libraries(autovision 
    ${OpenCV_LIBS}
    onnxruntime
)

