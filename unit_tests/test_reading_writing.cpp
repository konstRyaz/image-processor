#include "../reading_and_writing/reader.h"
#include "../reading_and_writing/writer.h"
#include <filesystem>
#include <iostream>

void TestInvalidFileHandling() {
    std::cout << "[IO] Invalid file... ";
    try {
        reading_and_writing::Reader("non_existent.bmp").ReadFile();
        std::cerr << "FAIL (No exception)\n";
    } catch (const std::runtime_error&) {
        std::cout << "PASS\n";
    }
}
