/*
 *
 * Gray Code Structured Light Utils
 *
 * @author 	: keith@robot9.me
 * @date	: 2018/7/2
 *
 */

#ifndef GRAYCODE_H
#define GRAYCODE_H

#include<opencv2/opencv.hpp>

typedef enum {
    Horizontal = 0,
    Vertical
} GrayCodeDirection;

class GrayCode {
public:
    GrayCode(int width, int height,
             cv::Scalar oddColor = cv::Scalar(255, 255, 255), cv::Scalar evenColor = cv::Scalar(0, 0, 0));

    cv::Mat encodePattern(int idx, GrayCodeDirection direction = Horizontal);

    static void decodePattern(std::vector<cv::Mat> &imagesH, std::vector<cv::Mat> &imagesV, cv::Mat &out);
    static cv::Mat pattern2Texture(cv::Mat pattern);

    static inline unsigned binaryToGray(unsigned num);
    static inline unsigned grayToBinary(unsigned num, unsigned numBits = 32);

    inline int getBitsX() { return _bitsX; }
    inline int getBitsY() { return _bitsY; }

protected:
    static inline bool isPixelOdd(uchar *pixel);

private:
    void fillPixel(uchar *p, int x, int y, int idx, GrayCodeDirection direction);
    static cv::Mat decodePattern(std::vector<cv::Mat> &images, int bits, int width, int height);

private:
    int _bitsX;
    int _bitsY;
    int _width;
    int _height;
    cv::Scalar _oddColor;
    cv::Scalar _evenColor;

    double _divX;
    double _divY;
};

#endif // GRAYCODE_H
