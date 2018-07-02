/*
 *
 * Gray Code Structured Light Utils
 *
 * @author 	: keith@robot9.me
 * @date	: 2018/7/2
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include<opencv2/opencv.hpp>

class CvUtils {
public:
    static void writeMatToFile(cv::Mat &m, const char *filename);
};

#endif // UTILS_H
