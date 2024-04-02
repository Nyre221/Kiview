#SPDX-License-Identifier: BSD-2-Clause
#SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
add_executable(kiview
)



target_sources(kiview PRIVATE main.cpp resources.qrc
    contentmanager.h contentmanager.cpp
    textviewer.h textviewer.cpp
    imageviewer.h imageviewer.cpp
    documentviewer.h documentviewer.cpp
    videoviewer.h videoviewer.cpp
    containerviewer.h containerviewer.cpp
    dolphinbridge.h dolphinbridge.cpp)


target_link_libraries(kiview
    Qt6::Quick
    Qt6::Qml
    Qt6::Gui
    Qt6::QuickControls2
    Qt6::Widgets
    Qt6::DBus
    KF6::I18n
    KF6::CoreAddons
    Qt::WebEngineQuick
    Qt6::WebEngineCore

)


install(TARGETS kiview ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
