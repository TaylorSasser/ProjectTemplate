#include <pt/project.hpp>
#include <algorithm>
#include <cmrc/cmrc.hpp>
#include <fmt/format.h>
#include <fstream>
#include <utility>

CMRC_DECLARE(resources);

namespace pt {

    auto const& mkdir = static_cast<bool (*)(path const&)>(std::filesystem::create_directories);
    auto const& write_simple = [](path const& p, std::string const& resource_path, auto&&... args)
    {
        std::ofstream template_file(p, std::ofstream::binary);
        auto fs = cmrc::resources::get_filesystem();
        auto iter = fs.open("assets/" + resource_path);
        auto buff = std::string{iter.begin(), iter.end()};

        if constexpr (sizeof...(args) == 0)
            std::copy(buff.begin(), buff.end(), std::ostreambuf_iterator<char>(template_file));
        else
        {
            auto out = fmt::memory_buffer();
            fmt::format_to(std::back_inserter(out), buff, std::forward<decltype(args)>(args)...);
            std::copy(out.begin(), out.end(), std::ostreambuf_iterator<char>(template_file));
        }
    };


    project::project(std::string name, path root_dir) noexcept
      : name_{std::move(name)}
      , root_dir_{std::move(root_dir)}
    {
        mkdir(root_dir_);
        mkdir(root_dir_ / "build");
        mkdir(root_dir_ / "build" / "debug");
        mkdir(root_dir_ / "build" / "release");
        mkdir(root_dir_ / "cmake");
        mkdir(root_dir_ / "include" / name_);

        std::string guard{name_};
        std::transform(guard.begin(), guard.end(), guard.begin(), ::toupper);

        write_simple(root_dir_ / "include" / name_ / "factorial.hpp", "include/project_template/factorial.template",
            fmt::arg("GUARD", guard), fmt::arg("PROJECT_NAME", name_));

        mkdir(root_dir_ / "src");
        write_simple(root_dir_ / "src" / "factorial.cpp", "src/factorial.template", fmt::arg("PROJECT_NAME", name_));

        write_simple(root_dir_ / ".gitignore", ".gitignore.template");
        write_simple(root_dir_ / ".gitattributes", ".gitattributes.template");
        write_simple(root_dir_ / ".clang-format", ".clang-format.template", fmt::arg("PROJECT_NAME", name_));
        write_simple(root_dir_ / ".clang-tidy", ".clang-tidy.template", fmt::arg("PROJECT_NAME", name_));
    }

    void project::create_lib() const noexcept
    {
        write_simple(root_dir_ / "CMakeLists.txt", "library/CMakeLists.template", fmt::arg("PROJECT_NAME", project::name_),
            fmt::arg("PROJECT_VERSION", "{PROJECT_VERSION}"), fmt::arg("CMAKE_CXX_FLAGS", "{CMAKE_CXX_FLAGS}"),
            fmt::arg("INSTALL_CONFIGDIR", "{INSTALL_CONFIGDIR}"), fmt::arg("CMAKE_INSTALL_LIBDIR", "{CMAKE_INSTALL_LIBDIR}"),
            fmt::arg("CMAKE_CURRENT_SOURCE_DIR", "{CMAKE_CURRENT_SOURCE_DIR}"),
            fmt::arg("CMAKE_INSTALL_INCLUDEDIR", "{CMAKE_INSTALL_INCLUDEDIR}"),
            fmt::arg("CMAKE_CURRENT_BINARY_DIR", "{CMAKE_CURRENT_BINARY_DIR}"));

        mkdir(root_dir_ / "tests");
        write_simple(root_dir_ / "tests" / "CMakeLists.txt", "library/tests/CMakeLists.template", fmt::arg("PROJECT_NAME", name_));
        write_simple(root_dir_ / "tests" / (name_ + "_example.cpp"), "library/tests/example_test.template");

        mkdir(root_dir_ / "example");
        write_simple(root_dir_ / "example" / "CMakeLists.txt", "library/example/CMakeLists.template", fmt::arg("PROJECT_NAME", name_));
        write_simple(root_dir_ / "example" / "main.cpp", "library/example/main.template", fmt::arg("PROJECT_NAME", name_));
    }

    void project::create_bin() const noexcept
    {
        mkdir(root_dir_ / "src");
        write_simple(root_dir_ / "src" / "main.cpp", "executable/src/main.template", fmt::arg("PROJECT_NAME", name_));

        write_simple(root_dir_ / "CMakeLists.txt", "executable/CMakeLists.template", fmt::arg("PROJECT_NAME", project::name_),
            fmt::arg("PROJECT_VERSION", "{PROJECT_VERSION}"), fmt::arg("CMAKE_CXX_FLAGS", "{CMAKE_CXX_FLAGS}"),
            fmt::arg("INSTALL_CONFIGDIR", "{INSTALL_CONFIGDIR}"), fmt::arg("CMAKE_INSTALL_LIBDIR", "{CMAKE_INSTALL_LIBDIR}"),
            fmt::arg("CMAKE_CURRENT_SOURCE_DIR", "{CMAKE_CURRENT_SOURCE_DIR}"),
            fmt::arg("CMAKE_INSTALL_INCLUDEDIR", "{CMAKE_INSTALL_INCLUDEDIR}"),
            fmt::arg("CMAKE_CURRENT_BINARY_DIR", "{CMAKE_CURRENT_BINARY_DIR}"));
    }

}    // namespace pt