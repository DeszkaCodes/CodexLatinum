#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

std::string* read_file(const char* path) {
    std::string* file_lines;
    {
        std::fstream input(path, std::ios::in);
        std::stringstream contents_stream;

        contents_stream << input.rdbuf();

        input.close();

        file_lines = &contents_stream.str();
    }

    return file_lines;
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cerr << "Incorrect argument count.\n" << "Example usage: \"codex <file.cl>\"\n";
        exit(EXIT_FAILURE);
    }

    std::string* file_lines(read_file(argv[1]));


    return EXIT_SUCCESS;
}