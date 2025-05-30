#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <filesystem>
#include <mutex>

namespace fs = std::filesystem;

// 全局变量，用于同步线程
std::mutex mtx;

// 线程函数，用于读取和显示 RTSP 流
void processStream(const std::string& rtspUrl) {
    cv::VideoCapture capture(rtspUrl);

    if (!capture.isOpened()) {
        std::cerr << "无法打开 RTSP 流: " << rtspUrl << std::endl;
        return;
    }

    cv::Mat frame;
    while (true) {
        capture >> frame;
        if (frame.empty()) {
            std::cerr << "无法读取帧: " << rtspUrl << std::endl;
            break;
        }

        std::lock_guard<std::mutex> lock(mtx);
        cv::imshow(rtspUrl, frame);

        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    capture.release();
    cv::destroyWindow(rtspUrl);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "用法: " << argv[0] << " <RTSP IP> <本地路径>" << std::endl;
        return -1;
    }

    std::string rtspIp = argv[1];
    std::string localPath = argv[2];

    // 检查本地路径是否存在
    if (!fs::exists(localPath) || !fs::is_directory(localPath)) {
        std::cerr << "本地路径不存在或不是目录: " << localPath << std::endl;
        return -1;
    }

    // 找到所有 .264 文件
    std::vector<std::string> files;
    for (const auto& entry : fs::directory_iterator(localPath)) {
        if (entry.path().extension() == ".264") {
            files.push_back(entry.path().filename().string());
        }
    }

    if (files.empty()) {
        std::cerr << "未找到任何 .264 文件" << std::endl;
        return -1;
    }

    // 创建线程池
    std::vector<std::thread> threads;
    for (const auto& file : files) {
        std::string rtspUrl = rtspIp + file;
        threads.emplace_back(processStream, rtspUrl);
    }

    // 等待所有线程完成
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
