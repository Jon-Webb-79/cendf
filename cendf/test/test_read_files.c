// ================================================================================
// ================================================================================
// - File:    test.c
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    August 31, 2022
// - Version: 1.0
// - Copyright: Copyright 2022, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include "test_read_files.h"

#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
// ================================================================================
// ================================================================================

void test_read_amu_nominal(void **state) {
	(void) state;
    const char *filename = "../../../../data/test/photoat-047_Ag_000.endf";
    float mass = read_amu(filename, 1.00866);
    assert_float_equal(mass, 107.867104, 1.0e-3);
}
// --------------------------------------------------------------------------------

void test_read_amu_failure(void **state) {
    (void) state;

    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }

    const char *filename = "../../../../data/test/fail_read_mass.endf";
    errno = 0;  // Reset errno before calling the function
    float mass = read_amu(filename, 1.00866f);

    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;

    // Assertions
    assert_float_equal(mass, -1.0f, 1.0e-3); // Check the return value
    assert_int_equal(errno, 22);              // Check errno EINVAL (22)
}
// --------------------------------------------------------------------------------

void test_read_amu_no_file(void **state) {
	(void) state;
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    const char *filename = "../../../../data/test/no_file.endf";
    errno = 0;
    float mass = read_amu(filename, 1.00866);

    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;
    assert_float_equal(mass, -1.0, 1.0e-3);
    assert_int_equal(errno, 2);
}
// ================================================================================
// ================================================================================
// eof
