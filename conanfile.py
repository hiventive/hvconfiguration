from conans import ConanFile, CMake, tools
import yaml

class HVConfigurationConan(ConanFile):
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
    no_copy_source = True # TODO: remove that (GLOB_RECURSIVE issue)
    exports = "settings.yml"
    exports_sources = "src/*", "CMakeLists.txt", "cmake/*"
    requires = "gtest/1.8.0@hiventive/stable", \
               "hvcommon/[>=0.3.0,<1]@hiventive/testing", \
               "cci/[>=1.0.0,<2]@hiventive/stable"

    def _configure_cmake(self):
        cmake = CMake(self)
        if self.settings.os != "Windows":
            cmake.definitions["CMAKE_POSITION_INDEPENDENT_CODE"] = self.options.fPIC or self.options.fPIE
        return cmake

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
            self.requires("yaml-cpp/0.6.2@hiventive/stable")
        else:
            self.requires("yaml-cpp/0.5.3@bincrafters/stable")

    def build(self):
        cmake = self._configure_cmake()
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = self._configure_cmake()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["hvconfiguration"]
