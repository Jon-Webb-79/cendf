#!/usr/bin/zsh
# ================================================================================
# ================================================================================
# - File:    release.zsh
# - Purpose: This file contains a script that will build c and c++ software
#            using CMake
#
# Source Metadata
# - Author:  Jonathan A. Webb
# - Date:    February 26, 2022
# - Version: 1.0
# - Copyright: Copyright 2022, Jon Webb Inc.
# ================================================================================
# ================================================================================

cmake -S ../../cendf/ -B ../../cendf/build/debug/ -DCMAKE_BUILD_TYPE=Debug
cmake --build ../../cendf/build/debug/
# ================================================================================
# ================================================================================
# eof
