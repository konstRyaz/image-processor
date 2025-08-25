#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "image/image.h"
#include "reading_and_writing/reader.h"
#include "reading_and_writing/writer.h"
#include "filters/filter.h"
#include "filters/grayscale_filter.h"
#include "filters/negative_filter.h"
#include "filters/crop_filter.h"
#include "filters/sharpening_filter.h"
#include "filters/edge_detection_filter.h"
#include "filters/gaussian_blur_filter.h"
#include "filters/custom_filter.h"

namespace {
constexpr int DefaultCellSize = 15;
}  // namespace

void PrintHelp() {
    std::cout << "Usage: image_processor <input.bmp> <output.bmp> [FILTERS...]\n"
              << "Available filters:\n"
              << "-gs                       Grayscale\n"
              << "-neg                      Negative\n"
              << "-crop <width> <height>    Crop\n"
              << "-sharp                    Sharpening\n"
              << "-edge <threshold>         Edge Detection\n"
              << "-blur <sigma>             Gaussian Blur\n"
              << "-custom [cell_size]       Crystallize (default: 15)\n"
              << "Example:\n"
              << "./image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5\n";
}

std::unique_ptr<Filter> CreateFilter(const std::string& filter_name, const std::vector<std::string>& params) {
    if (filter_name == "-gs") {
        return std::make_unique<GrayscaleFilter>();
    } else if (filter_name == "-neg") {
        return std::make_unique<NegativeFilter>();
    } else if (filter_name == "-crop") {
        if (params.size() < 2) {
            throw std::invalid_argument("Crop filter requires 2 parameters");
        }
        size_t width = std::stoul(params[0]);
        size_t height = std::stoul(params[1]);
        return std::make_unique<CropFilter>(width, height);
    } else if (filter_name == "-sharp") {
        return std::make_unique<SharpeningFilter>();
    } else if (filter_name == "-edge") {
        if (params.empty()) {
            throw std::invalid_argument("Edge detection requires threshold");
        }
        float threshold = std::stof(params[0]);
        return std::make_unique<EdgeDetectionFilter>(threshold);
    } else if (filter_name == "-blur") {
        if (params.empty()) {
            throw std::invalid_argument("Gaussian blur requires sigma");
        }
        float sigma = std::stof(params[0]);
        return std::make_unique<GaussianBlurFilter>(sigma);
    } else if (filter_name == "-custom") {
        int cell_size = DefaultCellSize;
        if (!params.empty()) {
            cell_size = std::stoi(params[0]);
        }
        return std::make_unique<CrystallizeFilter>(cell_size);
    }
    throw std::invalid_argument("Unknown filter: " + filter_name);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        PrintHelp();
        return 0;
    }

    try {
        reading_and_writing::Reader reader(argv[1]);
        Image image = reader.ReadFile();

        std::vector<std::unique_ptr<Filter>> filters;
        for (int i = 3; i < argc;) {
            std::string filter_name = argv[i++];
            std::vector<std::string> params;

            while (i < argc && argv[i][0] != '-') {
                params.push_back(argv[i++]);
            }

            filters.push_back(CreateFilter(filter_name, params));
        }

        for (auto& filter : filters) {
            filter->Apply(image);
        }

        reading_and_writing::Writer writer(argv[2]);
        writer.WriteFile(image);

        std::cout << "Success! Filtered image saved to: " << argv[2] << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
