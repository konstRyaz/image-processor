#include ".././image/image.h"
#include <iostream>
#include <stdexcept>

namespace {
constexpr size_t TestWidth1 = 100;
constexpr size_t TestHeight1 = 200;
constexpr size_t TestWidth2 = 2;
constexpr size_t TestHeight2 = 2;
constexpr size_t TestWidth3 = 10;
constexpr size_t TestHeight3 = 10;
constexpr size_t OutOfBoundsCoord = 15;
constexpr uint8_t RedValue = 255;
constexpr uint8_t ZeroValue = 0;
}  // namespace

void TestImageCreation() {
    std::cout << "Running TestImageCreation... ";
    Image img(TestWidth1, TestHeight1);
    if (img.GetWidth() != TestWidth1 || img.GetHeight() != TestHeight1) {
        std::cerr << "FAILED: Wrong dimensions\n";
        return;
    }
    std::cout << "PASSED\n";
}

void TestPixelAccess() {
    std::cout << "Running TestPixelAccess... ";
    Image img(TestWidth2, TestHeight2);
    Color test_color = {RedValue, ZeroValue, ZeroValue};
    img.SetColor(0, 0, test_color);

    Color retrieved = img.GetColor(0, 0);
    if (retrieved.red != RedValue || retrieved.green != ZeroValue || retrieved.blue != ZeroValue) {
        std::cerr << "FAILED: Color mismatch\n";
        return;
    }
    std::cout << "PASSED\n";
}

void TestOutOfBounds() {
    std::cout << "Running TestOutOfBounds... ";
    Image img(TestWidth3, TestHeight3);
    try {
        img.GetColor(OutOfBoundsCoord, OutOfBoundsCoord);
        std::cerr << "FAILED: No exception thrown\n";
    } catch (const std::out_of_range&) {
        std::cout << "PASSED\n";
    }
}
