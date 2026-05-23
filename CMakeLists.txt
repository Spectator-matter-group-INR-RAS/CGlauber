# COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
# Copyright (C) 2024-2026 Savva Savenkov
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.22)

project(
    CGlauber
    VERSION 1.0.0
    LANGUAGES CXX
)

find_package(COLA REQUIRED)
find_package(ROOT REQUIRED)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

set(CMAKE_INSTALL_PREFIX ${COLA_DIR})
set(COLA_MODULE_NAME CGlauber)

set(COLA_CMAKE_CXX_CLANG_TIDY
    ""
    CACHE STRING "Passed to CMAKE_CXX_CLANG_TIDY for ${COLA_MODULE_NAME} targets"
)

set(WRAPPER_SRCS Wrapper/CGlauberFactory.cc Wrapper/CGlauberGenerator.cc Wrapper/CGlauberModule.cc
                 FermiMomentum/FermiMomentum.cc
)

set(VENDOR_SRCS TGlauber/TGlauNucleon.cc TGlauber/TGlauNucleus.cc TGlauber/TGlauberMC.cc)

add_library(${COLA_MODULE_NAME} SHARED ${WRAPPER_SRCS} ${VENDOR_SRCS})

if (COLA_CMAKE_CXX_CLANG_TIDY)
    set_source_files_properties(
        ${WRAPPER_SRCS} PROPERTIES CXX_CLANG_TIDY "${COLA_CMAKE_CXX_CLANG_TIDY}"
    )
endif ()

target_compile_features(${COLA_MODULE_NAME} PRIVATE cxx_std_17)

target_include_directories(
    ${COLA_MODULE_NAME}
    PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/TGlauber>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/Wrapper>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/FermiMomentum>"
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>"
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}>
)

target_link_libraries(
    ${COLA_MODULE_NAME} PUBLIC COLA ROOT::Core ROOT::RIO ROOT::MathMore ${ROOT_LIBRARIES}
)

set(PROJECT_PUBLIC_HEADERS
    "Wrapper/CGlauberFactory.hh"
    "Wrapper/CGlauberGenerator.hh"
    "Wrapper/CGlauberModule.hh"
    "TGlauber/TGlauberMC.hh"
    "TGlauber/TGlauNucleon.hh"
    "TGlauber/TGlauNucleus.hh"
    "GlauberConst.hh"
    "FermiMomentum/FermiMomentum.hh"
)

set_target_properties(
    ${COLA_MODULE_NAME}
    PROPERTIES PUBLIC_HEADER "${PROJECT_PUBLIC_HEADERS}"
               VERSION "${PROJECT_VERSION}"
               SOVERSION "${PROJECT_VERSION_MAJOR}"
               BUILD_WITH_INSTALL_RPATH TRUE
               INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Add installation path for library to find .dat files:
target_compile_definitions(
    ${COLA_MODULE_NAME} PRIVATE DATA_INSTALL="${CMAKE_INSTALL_PREFIX}/data/${COLA_MODULE_NAME}/"
)

configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${COLA_MODULE_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME}
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfigVersion.cmake COMPATIBILITY AnyNewerVersion
)

# Install library
install(
    TARGETS ${COLA_MODULE_NAME}
    EXPORT CGlauberExport
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/${COLA_MODULE_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${COLA_MODULE_NAME}
)

install(EXPORT CGlauberExport DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${COLA_MODULE_NAME}
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/data/ DESTINATION data/${COLA_MODULE_NAME})

enable_testing()
if (BUILD_TESTING)
    add_subdirectory(tests)
endif ()

setup_cola_quality_tools(EXCLUDE_PATTERNS ".*/COLA/.*" ".*/TGlauber/.*" ".*/tests/.*")
