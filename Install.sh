#! /usr/bin/env bash
#SPDX-License-Identifier: GPL-3.0-or-later
#SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
# cmake -B build/ -DCMAKE_INSTALL_PREFIX="~/kde5/usr"
cmake -B build/
cmake --build build/
# cmake --install build/
sudo cmake --install build/
