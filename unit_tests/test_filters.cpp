#include "../filters/grayscale_filter.h"
#include "../filters/crop_filter.h"
#include "../filters/edge_detection_filter.h"
#include <iostream>
#include <cassert>

namespace {
constexpr uint8_t TestGrayValue1 = 100;
constexpr uint8_t TestGrayValue2 = 150;
constexpr uint8_t TestGrayValue3 = 200;
constexpr uint8_t ExpectedGrayValue = 140;
constexpr uint8_t BlackValue = 0;
constexpr uint8_t WhiteValue = 255;
constexpr size_t LargeImageSize = 500;
constexpr size_t MediumImageSize = 300;
constexpr size_t SmallImageSize = 200;
constexpr size_t TestPixelPosition = 100;
constexpr size_t EdgeTestSize = 3;
constexpr float EdgeThreshold = 0.5f;
}  // namespace

void TestGrayscaleFilter() {
    std::cout << "[Filter] Grayscale... " << std::flush;

    Image img(2, 2);
    img.SetColor(0, 0, {TestGrayValue1, TestGrayValue2, TestGrayValue3});
    GrayscaleFilter().Apply(img);

    Color result = img.GetColor(0, 0);
    if (result.red != ExpectedGrayValue || result.green != ExpectedGrayValue || result.blue != ExpectedGrayValue) {
        std::cerr << "FAIL (Expected " << ExpectedGrayValue << ", got " << result.red << ")\n";
        return;
    }

    Image img2(1, 2);
    img2.SetColor(0, 0, {BlackValue, BlackValue, BlackValue});
    img2.SetColor(0, 1, {WhiteValue, WhiteValue, WhiteValue});
    GrayscaleFilter().Apply(img2);

    assert(img2.GetColor(0, 0).red == BlackValue);
    assert(img2.GetColor(0, 1).red == WhiteValue);

    std::cout << "PASS\n";
}

void TestCropFilter() {
    std::cout << "[Filter] Crop... " << std::flush;

    Image img(LargeImageSize, LargeImageSize);
    img.SetColor(TestPixelPosition, TestPixelPosition, {WhiteValue, BlackValue, BlackValue});
    CropFilter(MediumImageSize, MediumImageSize).Apply(img);

    if (img.GetWidth() != MediumImageSize || img.GetHeight() != MediumImageSize) {
        std::cerr << "FAIL (Wrong size)\n";
        return;
    }

    Color pixel = img.GetColor(TestPixelPosition, TestPixelPosition);
    if (pixel.red != WhiteValue || pixel.green != BlackValue || pixel.blue != BlackValue) {
        std::cerr << "FAIL (Data corruption)\n";
        return;
    }

    Image img2(SmallImageSize, SmallImageSize);
    CropFilter(LargeImageSize, LargeImageSize).Apply(img2);
    if (img2.GetWidth() != SmallImageSize || img2.GetHeight() != SmallImageSize) {
        std::cerr << "FAIL (Upscale crop error)\n";
        return;
    }

    std::cout << "PASS\n";
}

void TestEdgeDetection() {
    std::cout << "[Filter] Edge detection... " << std::flush;

    Image img(EdgeTestSize, EdgeTestSize);
    for (size_t y = 0; y < EdgeTestSize; ++y) {
        for (size_t x = 0; x < EdgeTestSize; ++x) {
            img.SetColor(x, y, {BlackValue, BlackValue, BlackValue});
        }
    }
    img.SetColor(1, 1, {WhiteValue, WhiteValue, WhiteValue});

    EdgeDetectionFilter(EdgeThreshold).Apply(img);

    if (img.GetColor(1, 1).red != BlackValue) {
        std::cerr << "FAIL (Center not black)\n";
        return;
    }

    if (img.GetColor(0, 0).red != WhiteValue) {
        std::cerr << "FAIL (Edge not white)\n";
        return;
    }

    std::cout << "PASS\n";
}
