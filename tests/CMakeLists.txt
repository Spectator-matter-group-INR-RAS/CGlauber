find_package(GTest REQUIRED)

include(GoogleTest)

add_executable(CGlauberTest cglauber_test.cc)

set_target_properties(
    CGlauberTest
    PROPERTIES CXX_STANDARD 17
               CXX_STANDARD_REQUIRED YES
               CXX_EXTENSIONS NO
               CXX_CLANG_TIDY "${COLA_CMAKE_CXX_CLANG_TIDY}"
)

target_link_libraries(CGlauberTest CGlauber GTest::GTest GTest::Main)

gtest_discover_tests(CGlauberTest DISCOVERY_TIMEOUT 30)
