# ================================================================================
# ================================================================================
# - File:    CMakeLists.txt
# - Purpose: Primary CMake file for C++ build system
#
# Source Metadata
# - Author:  Jonathan A. Webb 
# - Date:    December 17, 2024
# - Version: 1.0
# - Copyright: Copyright 2024, Jonathan A. Webb Inc.
# ================================================================================
# ================================================================================
# Set minimum cmake version

# Define the test executable
add_executable(unit_tests
    unit_test.c
    test_read_files.c
    test_dstructures.c
)

# Link the test executable against the `endf` library and CMocka
target_link_libraries(unit_tests cendf cmocka)

# Register the unit_tests executable as a test for CTest
add_test(NAME unit_tests COMMAND unit_tests)
# ================================================================================
# ================================================================================
# eof
