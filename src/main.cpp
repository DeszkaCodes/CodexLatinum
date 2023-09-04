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

std::string tokens_to_asm(const std::vector<Token>& tokens) {
    std::stringstream output;

    output << "global _start\n";
    output << "_start:\n";

    size_t length = tokens.size();

    for(size_t i = 0; i < length; i++) {
        const Token& token = tokens.at(i);

        switch (token.type)
        {
        case TokenType::_return:
            if(i + 2 >= length) {
                std::cerr << "too small tokens";
                exit(EXIT_FAILURE);
            }

            if(tokens.at(i + 1).type != TokenType::int_lit) {
                std::cerr << "not int";
                exit(EXIT_FAILURE);
            }

            if(tokens.at(i + 2).type != TokenType::semicol) {
                std::cerr << "missing ;";
                exit(EXIT_FAILURE);
            }

            output << "    mov rax, 60\n";
            output << "    mov rdi, " << tokens.at(i + 1).value.value() << "\n";
            output << "    syscall\n";

            i += 2;

            break;
        
        default:
            std::cerr << "error";
            exit(EXIT_FAILURE);
        }
    }

    return output.str().c_str();
}

std::vector<Token> tokenize(const std::string& source) {
    std::vector<Token> token_vector;

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
                token_vector.push_back({ .type = TokenType::_return });
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

            token_vector.push_back({ .type = TokenType::int_lit, .value = buffer });
            buffer.clear();
        }
        else if(c == ';') {
            token_vector.push_back({ .type = TokenType::semicol });
        }
        else if(std::isspace(c)){
            continue;
        }
        else {
            std::cerr << "syntax error";
            exit(EXIT_FAILURE);
        }
    }

    return token_vector;
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

    std::string assembly = tokens_to_asm(tokens);

    std::cout << assembly << "\n";

    return EXIT_SUCCESS;
}