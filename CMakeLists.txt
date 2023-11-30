#SPDX-License-Identifier: BSD-2-Clause
#SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
cmake_minimum_required(VERSION 3.16)
project(kiview VERSION 0.7)

set(KF_MIN_VERSION "5.68.0")
set(QT_MIN_VERSION "5.12.0")

#flatpak
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
#flatpak


find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMInstallIcons)
include(ECMAddAppIcon)

find_package(Qt5  REQUIRED NO_MODULE COMPONENTS Core Quick Test Gui QuickControls2 Widgets DBus)
find_package(KF5 ${KF_MIN_VERSION} REQUIRED COMPONENTS Kirigami2 I18n CoreAddons)
find_package(Qt5 ${QT_MIN_VERSION} REQUIRED COMPONENTS Pdf )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)
add_subdirectory(icons)



install(PROGRAMS org.kde.kiview.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.kiview.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})




ki18n_install(po)
feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
