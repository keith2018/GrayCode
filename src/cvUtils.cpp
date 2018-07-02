/*
 *
 * Gray Code Structured Light Utils
 *
 * @author 	: keith@robot9.me
 * @date	: 2018/7/2
 *
 */

#include "cvutils.h"

void CvUtils::writeMatToFile(cv::Mat &m, const char *filename) {
    std::ofstream fout(filename);
    if (!fout) {
        std::cout << "File Not Opened" << std::endl;
        return;
    }
    fout << m;
    fout.close();
}
