#include <iostream>

void TestImageCreation();
void TestPixelAccess();
void TestOutOfBounds();

void TestInvalidFileHandling();

void TestGrayscaleFilter();
void TestCropFilter();
void TestEdgeDetection();

int main() {
    try {
        TestImageCreation();
        TestPixelAccess();
        TestOutOfBounds();
        TestInvalidFileHandling();
        TestGrayscaleFilter();
        TestCropFilter();
        TestEdgeDetection();

        std::cout << "All tests passed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}
