#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main() {
    // 加载图像
    Mat image = imread("image.jpg", IMREAD_GRAYSCALE);
    if (image.empty()) {
        cerr << "Could not open or find the image!" << endl;
        return -1;
    }

    // 边缘检测
    Mat edges;
    Canny(image, edges, 100, 200);

    // 显示结果
    imshow("Original Image", image);
    imshow("Edge Detection Result", edges);
    waitKey(0);

    return 0;
}
