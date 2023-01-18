#include <pt/project.hpp>
#include <filesystem>
#include <cctype>

#include <iostream>

//usage make-proj lab_01 . --type=lib
int main(int argc, char* argv[])
{
    std::string project_name = "lab_01";
    std::string cwd = "C:/Users/Taylor/Desktop/";
    std::string type = "lib";

    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    std::filesystem::path folder_path{cwd + project_name};
    pt::project p{project_name, folder_path};

    if (type == "lib" || type == "l") p.create_lib();
    else if (type == "bin" || type == "b") p.create_bin();

    return 0;
}


