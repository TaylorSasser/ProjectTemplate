#ifndef PROJECT_TEMPLATE_PROJECT_HPP
#define PROJECT_TEMPLATE_PROJECT_HPP

#include <string>
#include <filesystem>
#include <fmt/format.h>

namespace pt
{
    using path = std::filesystem::path;
    class project
    {
    public:
        explicit project(std::string name, std::filesystem::path root_dir) noexcept;
        void create_lib() const noexcept;
        void create_bin() const noexcept;
    private:
        std::string const name_;
        std::filesystem::path const root_dir_;
    };
}


#endif //PROJECT_TEMPLATE_PROJECT_HPP
