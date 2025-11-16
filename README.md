# 🚘 Autonomous Driving — Lane & Vehicle Detection System  
### C++17 · OpenCV · ONNX Runtime · Multithreading · YOLOv5

This project implements a **real-time autonomous driving perception system** using:

- **C++17**
- **OpenCV 4.x**
- **ONNX Runtime 1.23.2**
- **YOLOv5 ONNX model**
- **Multithreading (4-thread video pipeline)**

It performs **lane detection** and **vehicle detection** on real-time webcam/video input.

---

## ✨ Features

### 🚦 **Multithreaded Real-Time Pipeline**
| Thread | Task |
|-------|------|
| 1 | Frame Capture |
| 2 | Lane Detection |
| 3 | Vehicle Detection (YOLOv5) |
| 4 | Display Output |

This ensures smooth video processing with minimal frame drops.

---

### 🛣 **Lane Detection Pipeline**
- HLS color filtering (white + yellow lanes)
- ROI trapezoid mask
- Morphological smoothing
- Gaussian blur
- Canny edge detection
- HoughLinesP
- Averaged & stable lane lines

---

### 🚗 **Vehicle Detection**
- **Primary:** YOLOv5s ONNX model  
- **Optional:** Haar Cascade  
- **Fallback:** Motion-based detection  

YOLOv5 provides high-accuracy detection even for moving dashcam videos.

---

## 📁 Folder Structure

autonomous-vision/
│
├── CMakeLists.txt
├── README.md
│
├── data/
│ ├── sample_video.mp4
│ ├── vehicle_yolov5.onnx
│ └── haarcascade_car.xml
│
├── src/
│ ├── main.cpp
│ ├── video_pipeline.cpp
│ ├── lane_detector.cpp
│ ├── lane_detector.h
│ ├── vehicle_detector.cpp
│ ├── vehicle_detector.h
│ └── utils.cpp
│
└── build/
├── Release/
│ └── autovision.exe

yaml
Copy code

---

# 📥 Downloads

## 🎥 Sample Video (YouTube)
Used for testing lane & vehicle detection:

👉 **https://www.youtube.com/watch?v=KBsqQez-O4w**

Download using any YouTube downloader and save as:

data/sample_video.mp4

yaml
Copy code

---

## 🧠 YOLOv5 ONNX Model  
Requested download link:

👉 **https://sourceforge.net/projects/yolov5.mirror/postdownload**


Save the ONNX model here:

data/vehicle_yolov5.onnx


# 🛠 Installation (Windows)

### Requirements
- Visual Studio 2022
- CMake ≥ 3.10
- OpenCV 4.x (Windows prebuilt installer)
- ONNX Runtime 1.23.2
- C++17 enabled

---

## 1️⃣ Install OpenCV
Download **opencv-4.x.x-windows.exe**  
Extract to:

C:/opencv/

Ensure this file exists:

C:/opencv/build/x64/vc16/lib/cmake/opencv4/OpenCVConfig.cmake

---

## 2️⃣ Install ONNX Runtime
Extract:

onnxruntime-win-x64-1.23.2.zip

to:

C:/onnxruntime-win-x64-1.23.2/


---

## 3️⃣ Build the Project

cd build
cmake .. -DOpenCV_DIR="C:/opencv/build/x64/vc16/lib/cmake/opencv4"
cmake --build . --config Release
▶️ Running the Application
⭐ With YOLOv5 ONNX (Recommended)


./autovision.exe ../../data/sample_video.mp4 ../../data/vehicle_yolov5.onnx
Using Haar Cascade

./autovision.exe ../../data/sample_video.mp4 ../../data/haarcascade_car.xml
Fallback Motion Detection

./autovision.exe ../../data/sample_video.mp4


🚀 Future Improvements
Add YOLOv8 or YOLOv11 ONNX

Add road segmentation models

Add lane curvature + steering angle prediction

Save output video file

GPU inference using ONNX Runtime CUDA

👤 Author
Shalo Sharjan
C++ Developer | Computer Vision | Embedded Systems