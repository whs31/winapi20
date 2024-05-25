import os
from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout, CMakeDeps
from conan.tools.build import check_min_cppstd
from conan.tools.files import rmdir


class WinAPI20Recipe(ConanFile):
    name = "winapi20"
    version = "dev"
    description = "Windows API wrapper in modern C++20"
    license = "MIT"
    author = "whs31 <whs31@github.io>"
    topics = ("windows", "api", "wrapper", "modern", "cpp20")

    settings = "os", "arch", "compiler", "build_type"
    options = {
        "shared": [True, False],
        "test": [True, False],
        "examples": [True, False],
    }
    default_options = {
        "shared": True,
        "test": False,
        "examples": False,
    }

    exports_sources = "*"

    @property
    def _min_cppstd(self):
        return "20"

    def requirements(self):
        self.requires("fmt/[^10.1.0]", transitive_headers=True, transitive_libs=True)
        if self.options.test:
            self.requires("gtest/1.14.0")

    def layout(self):
        cmake_layout(self)

    def validate(self):
        if self.settings.get_safe("compiler.cppstd"):
            check_min_cppstd(self, self._min_cppstd)

    def configure(self):
        self.options["fmt/*"].shared = self.options.shared

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.cache_variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.cache_variables["TESTS"] = self.options.test
        tc.cache_variables["EXAMPLES"] = self.options.examples
        print("<> tests: ", self.options.test)
        print("<> examples: ", self.options.examples)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
        rmdir(self, os.path.join(self.package_folder, "lib", "cmake"))
        rmdir(self, os.path.join(self.package_folder, "lib", "pkgconfig"))
        rmdir(self, os.path.join(self.package_folder, "res"))
        rmdir(self, os.path.join(self.package_folder, "share"))

    def package_info(self):
        self.cpp_info.set_property("cmake_file_name", "winapi20")
        self.cpp_info.set_property("cmake_target_name", "winapi::winapi20")
        self.cpp_info.libs = ["winapi20"]
        self.cpp_info.requires = ["fmt::fmt"]
