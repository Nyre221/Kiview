#SPDX-License-Identifier: BSD-2-Clause
#SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>

cmake_minimum_required(VERSION 3.16)
project(kiview VERSION 0.8.5)

set(QT_MIN_VERSION "6.5.0")
set(KF_MIN_VERSION "6.0.0")

#flatpak
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#flatpak


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
# set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${ECM_MODULE_PATH}) #qt6

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMAddAppIcon)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Quick Test Gui QuickControls2 Widgets DBus WebEngineQuick WebEngineCore )
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS I18n Kirigami2 CoreAddons )




set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(icons)

ki18n_install(po)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)



