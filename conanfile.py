from conans import ConanFile, CMake, tools
from conans.util import files
import yaml

class ConfigurationConan(ConanFile):
    name = str(yaml.load(tools.load("settings.yml"))['conan']['name'])
    version = str(yaml.load(tools.load("settings.yml"))['project']['version'])
    license = "MIT"
    author = "Hiventive"
    url = "https://github.com/hiventive/hvconfiguration"
    description = "Configuration"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False], "fPIE": [True, False]}
    default_options = "shared=False", "fPIC=False", "fPIE=False"
    generators = "cmake"
    exports = "settings.yml"
    exports_sources = "src/*", "CMakeLists.txt"
    requires = "gtest/1.8.0@bincrafters/stable", \
               "common/0.2.0@hiventive/testing", \
               "systemc/[~2.3.2]@hiventive/stable", \
               "cci/1.0.0@hiventive/stable"

    def requirements(self):
        yamlCppNew = True
        compiler = self.settings.get_safe("compiler")
        if compiler and compiler == "Visual Studio":
            compilerVersion = self.settings.get_safe("compiler.version")
            if compilerVersion and compilerVersion == "11":
                compilerToolset = self.settings.get_safe("compiler.toolset")
                if compilerToolset and compilerToolset == "v120_CTP_Nov2012":
                   yamlCppNew = False
        if yamlCppNew:
            self.requires("yaml-cpp/0.6.2@bincrafters/stable")
        else:
            self.requires("yaml-cpp/0.5.3@bincrafters/stable")


    def build(self):
        cmake = CMake(self)
        files.mkdir("build")
        with tools.chdir("build"):
            if self.settings.os != "Windows":
                cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = self.options.fPIC or self.options.fPIE
            cmake.configure(build_dir=".", source_dir="../")
            cmake.build(build_dir=".")
            cmake.install(build_dir=".")

    def package(self):
        self.copy("*.h", dst="include", src="src")
        self.copy("*.hpp", dst="include", src="src")
        self.copy("HVConfiguration", dst="include", src="src")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.dylib*", dst="lib", keep_path=False)
        self.copy("*.so", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["configuration"]
