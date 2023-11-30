#SPDX-License-Identifier: BSD-2-Clause
#SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
add_executable(kiview)



target_sources(kiview PRIVATE main.cpp resources.qrc
    contentmanager.h contentmanager.cpp
    textviewer.h textviewer.cpp
    imageviewer.h imageviewer.cpp
    documentviewer.h documentviewer.cpp
    videoviewer.h videoviewer.cpp
    containerviewer.h containerviewer.cpp
    dolphinbridge.h dolphinbridge.cpp)


target_link_libraries(kiview Qt5::Quick Qt5::Qml Qt5::Gui Qt5::QuickControls2 Qt5::Widgets KF5::Kirigami2 KF5::I18n Qt5::Pdf Qt5::DBus )

install(TARGETS kiview ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
