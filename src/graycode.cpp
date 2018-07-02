/*
 *
 * Gray Code Structured Light Utils
 *
 * @author 	: keith@robot9.me
 * @date	: 2018/7/2
 *
 */

#include "graycode.h"

GrayCode::GrayCode(int width, int height, cv::Scalar oddColor, cv::Scalar evenColor)
    : _width(width)
    , _height(height)
    , _oddColor(oddColor)
    , _evenColor(evenColor)
{
    _bitsX = static_cast<int>(log2(width));
    _bitsY = static_cast<int>(log2(height));
    _divX = width / (double) (1 << _bitsX);
    _divY = height / (double) (1 << _bitsY);
}

cv::Mat GrayCode::encodePattern(int idx, GrayCodeDirection direction)
{
    cv::Mat image(_height, _width, CV_8UC3, cv::Scalar(0, 0, 0));

    for (int y = 0; y < _height; y++) {
        uchar *row = image.ptr<uchar>(y);
        for (int x = 0; x < _width; x++) {
            uchar *p = &row[x * 3];
            fillPixel(p, x, y, idx, direction);
        }
    }

    return image;
}

void GrayCode::fillPixel(uchar* p, int x, int y, int idx, GrayCodeDirection direction)
{
    unsigned num = (direction == Horizontal ? floor(x / _divX) : floor(y / _divY));
    unsigned gray = binaryToGray(num);

    bool odd = (gray >> ((direction == Horizontal ? _bitsX : _bitsY) - idx - 1)) & 1;

    p[0] = odd ? _oddColor[0] : _evenColor[0];
    p[1] = odd ? _oddColor[1] : _evenColor[1];
    p[2] = odd ? _oddColor[2] : _evenColor[2];
}

void GrayCode::decodePattern(std::vector<cv::Mat> &imagesH, std::vector<cv::Mat> &imagesV, cv::Mat &out)
{
    int width = imagesH[0].cols;
    int height = imagesH[0].rows;

    cv::Mat patternX = decodePattern(imagesH, imagesH.size(), width, height);
    cv::Mat patternY = decodePattern(imagesV, imagesV.size(), width, height);

    out = cv::Mat(height, width, CV_32SC2, cv::Scalar(0, 0));

    for (int y = 0; y < height; y++) {
        uint *rowPatternX = patternX.ptr<uint>(y);
        uint *rowPatternY = patternY.ptr<uint>(y);
        uint *rowOut = out.ptr<uint>(y);
        for (int x = 0; x < width; x++) {
            uint *pOut = &rowOut[x * 2];

            pOut[0] = rowPatternX[x];
            pOut[1] = rowPatternY[x];
        }
    }
}

cv::Mat GrayCode::pattern2Texture(cv::Mat pattern)
{
    cv::Mat texture = cv::Mat(pattern.rows, pattern.cols, CV_8UC4, cv::Scalar(0, 0, 0, 0));
    for (int y = 0; y < pattern.rows; y++) {
        uint *rowPattern = pattern.ptr<uint>(y);
        uchar *rowTexture = texture.ptr<uchar>(y);
        for (int x = 0; x < pattern.cols; x++) {
            uint *pPattern = &rowPattern[x * 2];
            uchar *pTexture = &rowTexture[x * 4];

            pTexture[0] = (pPattern[0]) >> 8;
            pTexture[1] = (pPattern[0]) & 0xFF;
            pTexture[2] = (pPattern[1]) >> 8;
            pTexture[3] = (pPattern[1]) & 0xFF;
        }
    }

    return texture;
}

cv::Mat GrayCode::decodePattern(std::vector<cv::Mat> &images, int bits, int width, int height)
{
    cv::Mat pattern(height, width, CV_32SC1, cv::Scalar(0));

    // determine gray bits
    for (int idx = 0; idx < bits; idx++) {
        cv::Mat &img = images[idx];
        for (int y = 0; y < height; y++) {
            uint *rowPattern = pattern.ptr<uint>(y);
            uchar *row = img.ptr<uchar>(y);
            for (int x = 0; x < width; x++) {
                uint *pPattern = &rowPattern[x];
                uchar *p = &row[x * 3];

                if (isPixelOdd(p)) {
                    pPattern[0] |= 1 << (bits - idx - 1);
                }
            }
        }
    }

    // gray > binary
    for (int y = 0; y < height; y++) {
        uint *rowPattern = pattern.ptr<uint>(y);
        for (int x = 0; x < width; x++) {
            uint *pPattern = &rowPattern[x];
            pPattern[0] = grayToBinary(pPattern[0]);
        }
    }

    return pattern;
}

bool GrayCode::isPixelOdd(uchar *pixel)
{
    if (pixel[0] + pixel[1] + pixel[2] > 128 * 3) {
        return true;
    }
    return false;
}

unsigned GrayCode::binaryToGray(unsigned num) {
    return num ^ (num >> 1);
}

unsigned GrayCode::grayToBinary(unsigned num, unsigned numBits) {
    for (unsigned shift = 1; shift < numBits; shift <<= 1) {
        num ^= num >> shift;
    }
    return num;
}

