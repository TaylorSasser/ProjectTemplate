#include <pt/project.hpp>
#include <filesystem>
#include <cctype>
#include <iostream>
#include <vector>
#include <algorithm>



int main(int argc, char* argv[])
{
    std::vector<std::string> args{};
    for (auto i{0}; i < argc; ++i)
        args.emplace_back(argv[i]);

    std::string project_name = args[1];
    std::filesystem::path pwd = std::filesystem::current_path() / std::filesystem::relative(args[2]);
    std::filesystem::current_path(pwd);
    std::string type = args[3];

    std::transform(type.begin(), type.end(), type.begin(), ::tolower);
    std::filesystem::path folder_path{pwd / project_name};
    pt::project p{project_name, folder_path};

    if (type == "lib" || type == "l")
        p.create_lib();
    if (type == "bin" || type == "b")
        p.create_bin();
    return 0;
}


