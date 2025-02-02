#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

void displayHelp() {
    std::cout << "Available commands:\n"
              << "  ls                 List contents of the current directory\n"
              << "  cd <path>          Change the current directory to <path>\n"
              << "  mkdir <dir_name>   Create a directory named <dir_name>\n"
              << "  touch <file_name>  Create an empty file named <file_name>\n"
              << "  rm <name>          Remove the file or directory named <name>\n"
              << "  rename <old> <new> Rename (or move) <old> to <new>\n"
              << "  help               Show this help message\n"
              << "  clear              Clear the terminal\n"
              << "  exit               Exit the program\n";
}

std::vector<std::string> splitCommand(const std::string &input) {
    std::vector<std::string> tokens;
    std::string token;
    for (char c : input) {
        if (c == ' ') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token.push_back(c);
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

void clearTerminal(){
    system("clear");
}

int main() {
    fs::path currentPath = fs::current_path();

    std::cout << "Smart File Manager\n";
    std::cout << "Type 'help' for a list of commands.\n\n";

    while (true) {
        std::cout << currentPath.string() << " ~ % ";

        std::string input;
        if (!std::getline(std::cin, input)) {
            std::cout << "\nExiting...\n";
            break;
        }

        std::vector<std::string> tokens = splitCommand(input);
        if (tokens.empty()) {
            continue;
        }

        // The first token is the command
        std::string command = tokens[0];

        if (command == "ls") {
            try {
                for (const auto &entry : fs::directory_iterator(currentPath)) {
                    std::cout << (entry.is_directory() ? "[DIR]  " : "[FILE] ")
                              << entry.path().filename().string() << "\n";
                }
            } catch (const fs::filesystem_error &e) {
                std::cerr << "Error listing directory: " << e.what() << "\n";
            }
        }
        else if (command == "cd") {
            if (tokens.size() < 2) {
                std::cerr << "Incorrect usage of 'cd', enter 'help' for assistance.\n";
            } else {
                fs::path newPath = currentPath / tokens[1];
                try {
                    if (fs::exists(newPath) && fs::is_directory(newPath)) {
                        currentPath = fs::canonical(newPath);
                    } else {
                        std::cerr << "Directory does not exist: " << newPath << "\n";
                    }
                } catch (const fs::filesystem_error &e) {
                    std::cerr << "Error changing directory: " << e.what() << "\n";
                }
            }
        }
        else if (command == "mkdir") {
            if (tokens.size() < 2) {
                std::cerr << "Incorrect usage of 'mkdir', enter 'help' for assistance.\n";
            } else {
                fs::path dirName = currentPath / tokens[1];
                try {
                    if (fs::create_directory(dirName)) {
                        std::cout << "Directory created: " << dirName << "\n";
                    } else {
                        std::cerr << "Failed to create directory: " << dirName << "\n";
                    }
                } catch (const fs::filesystem_error &e) {
                    std::cerr << "Error creating directory: " << e.what() << "\n";
                }
            }
        }
        else if (command == "touch") {
            if (tokens.size() < 2) {
                std::cerr << "Incorrect usage of 'touch', enter 'help' for assistance.\n";
            } else {
                fs::path filePath = currentPath / tokens[1];
                try {
                    std::ofstream ofs(filePath.string(), std::ios::out | std::ios::app);
                    ofs.close();
                    if (fs::exists(filePath)) {
                        std::cout << "File created: " << filePath << "\n";
                    } else {
                        std::cerr << "Failed to create file: " << filePath << "\n";
                    }
                } catch (const std::exception &e) {
                    std::cerr << "Error creating file: " << e.what() << "\n";
                }
            }
        }
        else if (command == "rm"){
            if (tokens.size() < 2){
                std::cerr << "Incorrect usage of 'rm', enter 'help' for assistance.\n";
            } else {
                fs::path targetPath = currentPath / tokens[1];
                try {
                    if (fs::exists(targetPath)) {
                        if (fs::is_directory(targetPath)) {
                            std::string confirmation;
                            std::cout << targetPath << " is a directory, enter \"CONFIRM\" to remove, \"CANCEL\" to cancel: ";
                            bool cancel = false;
                            while(true){
                                std::getline(std::cin,confirmation);
                                if(confirmation == "CONFIRM"){
                                    break;
                                }
                                else if(confirmation == "CANCEL"){
                                    cancel = true;
                                    break;
                                }
                                else{
                                    std::cerr << "Invalid input\n";
                                }
                            }
                            if(cancel) continue;
                            
                            fs::remove_all(targetPath);
                            std::cout << "Directory removed: " << targetPath << "\n";
                        } else {
                            fs::remove(targetPath);
                            std::cout << "File removed: " << targetPath << "\n";
                        }
                    } else {
                        std::cerr << "Path does not exist: " << targetPath << "\n";
                    }
                } catch (const fs::filesystem_error &e) {
                    std::cerr << "Error removing path: " << e.what() << "\n";
                }
            }
        }
        else if (command == "rename") {
            if (tokens.size() < 3) {
                std::cerr << "Incorrect Usage of 'rename', enter 'help' for assistance.\n";
            } else {
                fs::path oldPath = currentPath / tokens[1];
                fs::path newPath = currentPath / tokens[2];
                try {
                    if (fs::exists(oldPath)) {
                        fs::rename(oldPath, newPath);
                        std::cout << "Renamed/Moved: " << oldPath << " -> " << newPath << "\n";
                    } else {
                        std::cerr << "Old path does not exist: " << oldPath << "\n";
                    }
                } catch (const fs::filesystem_error &e) {
                    std::cerr << "Error renaming/moving: " << e.what() << "\n";
                }
            }
        }
        else if (command == "help") {
            displayHelp();
        }
        else if (command == "exit") {
            break;
        }
        else if (command == "clear") {
            clearTerminal();
        }
        else {
            std::cerr << "Command not found: " << command << "\n";
            std::cerr << "Type 'help' to see the list of commands.\n";
        }
    }

    return 0;
}
