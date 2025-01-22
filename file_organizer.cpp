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
              << "  exit               Exit the program\n";
}
