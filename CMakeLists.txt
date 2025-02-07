# COLA Wrapper for CERN ROOT's TGlauber implementation of Glauber model
# Copyright (C) 2024 Savva Savenkov
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
project(CGlauber VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(COLA REQUIRED)
find_package(ROOT REQUIRED)

include(CMakePackageConfigHelpers)

set(CMAKE_INSTALL_PREFIX ${COLA_DIR})
set(COLA_MODULE_NAME CGlauber)

# Add library target
set(srcs Wrapper/CGlauberFactory.cc Wrapper/CGlauberGenerator.cc TGlauber/TGlauNucleon.cc TGlauber/TGlauNucleus.cc TGlauber/TGlauberMC.cc FermiMomentum/FermiMomentum.cc)
add_library(CGlauber SHARED ${srcs})

target_include_directories(CGlauber PUBLIC
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/TGlauber;${PROJECT_SOURCE_DIR}/Wrapper;${PROJECT_SOURCE_DIR}/FermiMomentum;${PROJECT_SOURCE_DIR}>"
        $<INSTALL_INTERFACE:include/${COLA_MODULE_NAME}>)

target_link_libraries(CGlauber PUBLIC COLA ROOT::RIO ROOT::MathMore ${ROOT_LIBRARIES})

# Specify public headers for installation
set(PROJECT_PUBLIC_HEADERS "Wrapper/CGlauber.hh" "Wrapper/CGlauberFactory.hh" "Wrapper/CGlauberGenerator.hh"
        "TGlauber/TGlauberMC.hh" "TGlauber/TGlauNucleon.hh" "TGlauber/TGlauNucleus.hh"
        "GlauberConst.hh"
        "FermiMomentum/FermiMomentum.hh")

set_target_properties(CGlauber PROPERTIES PUBLIC_HEADER "${PROJECT_PUBLIC_HEADERS}")

# Add installation path for library to find .dat files:
add_compile_definitions(DATA_INSTALL="${CMAKE_INSTALL_PREFIX}/data/${COLA_MODULE_NAME}/")

# Configure config files
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/CGlauberConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfig.cmake"
        INSTALL_DESTINATION lib/cmake/${COLA_MODULE_NAME}
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion)

# Install library
install(TARGETS CGlauber
        EXPORT CGlauberExport
        LIBRARY DESTINATION lib/${COLA_MODULE_NAME}
        PUBLIC_HEADER DESTINATION include/${COLA_MODULE_NAME}
        INCLUDES DESTINATION include/${COLA_MODULE_NAME})

# Install export file and config files
install(EXPORT CGlauberExport
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/CGlauberConfigVersion.cmake"
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

#install .dat files for proper TGlauber generation
install(FILES ${PROJECT_SOURCE_DIR}/data/ca40_alv.dat
        ${PROJECT_SOURCE_DIR}/data/carbon_plaintext.dat
        ${PROJECT_SOURCE_DIR}/data/h3_plaintext.dat
        ${PROJECT_SOURCE_DIR}/data/he3_plaintext.dat
        ${PROJECT_SOURCE_DIR}/data/he4_plaintext.dat
        ${PROJECT_SOURCE_DIR}/data/o16_alv.dat
        ${PROJECT_SOURCE_DIR}/data/oxygen_plaintext.dat
        DESTINATION data/${COLA_MODULE_NAME})
