#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <memory>

#include "JpegEncoder.hpp"

struct Arguments {
    std::string inputFileName;
    std::string outputFileName;
    int quality;
    std::string format;
};

Arguments parseArguments(int argc, const char** argv) {
    Arguments args;

    // Default values
    args.quality = 50;
    args.format = "444";

    // Map of option names to their values
    std::unordered_map<std::string, std::string> options;

    // Parse the arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.substr(0, 1) == "-") {
            // Found an option
            std::string optName = arg.substr(1);
            i++;
            if (i >= argc) {
                // Error: option is missing a value
                throw std::runtime_error("Option " + optName + " requires a value.");
            }
            std::string optValue = argv[i];
            options[optName] = optValue;
        } else {
            // Found an argument (not an option)
            // This is an error if we've already found an input file name
            if (args.inputFileName != "") {
                throw std::runtime_error("Multiple input file names specified.");
            }
            args.inputFileName = arg;
        }
    }

    // Validate the options and their values
    if (options.count("i")) {
        args.inputFileName  = options["i"];
    } else {
        throw std::runtime_error("Input file name not specified. Usage example: ./jpeg_encoder -i xx.png -o xxx.jpg -q 50 -f 420, where -q is the quality range [1,100], -f is the yuvformat [444, 420, 4422]");
    } 

    if (options.count("o")) {
        args.outputFileName = options["o"];
    } else {
        throw std::runtime_error("Output file name not specified.");
    }

    if (options.count("q")) {
        try {
            args.quality = std::stoi(options["q"]);
        } catch (...) {
            throw std::runtime_error("Invalid value for quality.");
        }
    }

    if (options.count("f")) {
        std::string format = options["f"];
        if (format != "444" && format != "420" && format != "422") {
            throw std::runtime_error("Invalid value for format.");
        }
        args.format = format;
    }

    // Validate that we have an input file name
    if (args.inputFileName == "") {
        throw std::runtime_error("Input file name not specified.");
    }

    return args;
}



int main(int argc, const char** argv) {

    try {
        Arguments args = parseArguments(argc, argv);
        std::cout << "Input image: " << args.inputFileName << std::endl;
        std::cout << "Output jpeg: " << args.outputFileName << std::endl;
        std::cout << "Quality: " << args.quality << std::endl;
        std::cout << "YUVFormat: " << args.format << std::endl;

        // Read a RGB image
        Image<uint8_t> image(args.inputFileName.c_str());
        const int width = image.cols();
        const int height = image.rows();
        std::cout << "image width:" << width << " height:" << height << std::endl;

        YUVFormat format;
        if (args.format == "444") format = YUVFormat::YUV444;
        else if(args.format == "420") format = YUVFormat::YUV420;
        else if(args.format == "422") format = YUVFormat::YUV422;

        std::cout<<"encoded JPEG image to "<< args.format << std::endl;
        std::shared_ptr<JpegEncoder> jpegEncoder = std::make_shared<JpegEncoder>(args.outputFileName);
        jpegEncoder->encodeRGB(image, args.quality, format);
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
