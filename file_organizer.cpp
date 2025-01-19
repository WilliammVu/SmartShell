#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <stack>

class File{
public:
    std::string name;
    File* parent;
    std::vector<File*> subFiles;
    std::string contents;
};

class FileOrganizer{
    File* Documents; //root node
};
