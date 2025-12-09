cmake_minimum_required(VERSION 3.20)

project(error_aware
    VERSION 0.1.0
    DESCRIPTION "error_aware: error-aware stable numerics on machine arithmetic (header-only C++20)"
    LANGUAGES CXX
)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    option(ERROR_AWARE_ENABLE_CONAN
        "Allow fetching dependencies via Conan when system packages are missing"
        ON)
else()
    option(ERROR_AWARE_ENABLE_CONAN
        "Allow fetching dependencies via Conan when system packages are missing"
        OFF)
endif()

add_library(error_aware INTERFACE)
add_library(error_aware::error_aware ALIAS error_aware)

target_compile_features(error_aware INTERFACE cxx_std_20)

target_include_directories(error_aware
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

set(EA_EIGEN_TARGET "")

find_package(Eigen3 3.3 QUIET NO_MODULE)

if(Eigen3_FOUND)
    message(STATUS "error_aware: found system Eigen3: ${Eigen3_DIR}")
    set(EA_EIGEN_TARGET Eigen3::Eigen)

else()
    set(EA_BUNDLED_EIGEN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs/eigen")

    if(EXISTS "${EA_BUNDLED_EIGEN_DIR}/Eigen/Core")
        message(STATUS "error_aware: using bundled Eigen in ${EA_BUNDLED_EIGEN_DIR}")

        add_library(ea_bundled_eigen INTERFACE)

        target_include_directories(ea_bundled_eigen
            INTERFACE
                "${EA_BUNDLED_EIGEN_DIR}"
        )

        if(NOT TARGET Eigen3::Eigen)
            add_library(Eigen3::Eigen INTERFACE IMPORTED)
            target_link_libraries(Eigen3::Eigen
                INTERFACE ea_bundled_eigen
            )
        endif()

        set(EA_EIGEN_TARGET Eigen3::Eigen)

    elseif(ERROR_AWARE_ENABLE_CONAN)
        message(STATUS "error_aware: Eigen3 not found on system and no bundled libs/eigen. Trying Conan...")

        include(FetchContent)

        set(CONAN_CMAKE "${CMAKE_BINARY_DIR}/conan.cmake")

        if(NOT EXISTS "${CONAN_CMAKE}")
            message(STATUS "error_aware: downloading conan.cmake from GitHub")
            file(DOWNLOAD
                "https://github.com/conan-io/cmake-conan/raw/v0.18.1/conan.cmake"
                "${CONAN_CMAKE}"
                TLS_VERIFY ON
            )
        endif()

        include("${CONAN_CMAKE}")

        conan_cmake_run(
            REQUIRES eigen/3.4.0
            BASIC_SETUP CMAKE_TARGETS
            BUILD missing
        )

        if(TARGET CONAN_PKG::eigen)
            message(STATUS "error_aware: using Eigen from Conan (CONAN_PKG::eigen)")
            if(NOT TARGET Eigen3::Eigen)
                add_library(Eigen3::Eigen INTERFACE IMPORTED)
                target_link_libraries(Eigen3::Eigen
                    INTERFACE CONAN_PKG::eigen
                )
            endif()
            set(EA_EIGEN_TARGET Eigen3::Eigen)
        else()
            message(WARNING
                "error_aware: Conan did not provide CONAN_PKG::eigen target. "
                "Eigen-based functionality may be unavailable."
            )
        endif()

    else()
        message(WARNING
            "error_aware: Eigen3 not found, libs/eigen not present "
            "and ERROR_AWARE_ENABLE_CONAN=OFF. "
            "Eigen-based functionality (EA_HAS_EIGEN) will be disabled."
        )
    endif()
endif()

if(EA_EIGEN_TARGET)
    target_link_libraries(error_aware
        INTERFACE
            ${EA_EIGEN_TARGET}
    )
    target_compile_definitions(error_aware
        INTERFACE
            EA_HAS_EIGEN=1
    )
else()
    message(STATUS "error_aware: building without Eigen support (EA_HAS_EIGEN not defined).")
endif()
