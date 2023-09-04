#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <optional>
#include <queue>

enum class TokenType {
    _return,
    int_lit,
    semicol
};

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

std::queue<Token> tokenize(const std::string& source) {
    std::queue<Token> token_queue;

    std::string buffer;

    size_t length = source.length();
    for(size_t i = 0; i < length; i++) {
        char c = source.at(i);

        if(std::isalpha(c)) {
            buffer.push_back(c);

            while(i + 1 < length && std::isalnum(source.at(i + 1))) {
                buffer.push_back(source.at(i + 1));
                i++;
            }

            if(buffer == "reditus") {
                token_queue.push({ .type = TokenType::_return });
                buffer.clear();
            }
            else {
                std::cerr << "token error";
                exit(EXIT_FAILURE);
            }
        }
        else if(std::isdigit(c)) {
            buffer.push_back(c);

            while(i + 1 < length && std::isdigit(source.at(i + 1))) {
                buffer.push_back(source.at(i + 1));
                i++;
            }

            token_queue.push({ .type = TokenType::int_lit, .value = buffer });
            buffer.clear();
        }
        else if(c == ';') {
            token_queue.push({ .type = TokenType::semicol });
        }
        else if(std::isspace(c)){
            continue;
        }
        else {
            std::cerr << "syntax error";
            exit(EXIT_FAILURE);
        }
    }

    return token_queue;
}

std::string read_file(const char* path) {
    std::string file_lines;
    {
        std::fstream input(path, std::ios::in);
        std::stringstream contents_stream;

        contents_stream << input.rdbuf();

        file_lines = contents_stream.str().c_str();
    }

    return file_lines;
}

int main(int argc, char* argv[]) {

    if(argc != 2) {
        std::cerr << "Incorrect argument count.\n" << "Example usage: \"codex <file.cl>\"\n";
        exit(EXIT_FAILURE);
    }

    std::string file_lines(read_file(argv[1]));

    auto tokens = tokenize(file_lines);

    return EXIT_SUCCESS;
}