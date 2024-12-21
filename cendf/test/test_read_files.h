// ================================================================================
// ================================================================================
// - File:    test_dstructures.h
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

#ifndef test_read_files_H
#define test_read_files_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../include/read_file.h"
// ================================================================================
// ================================================================================ 

/*
 * Test read_amu with expected results
 */
void test_read_amu_nominal(void **state);
// --------------------------------------------------------------------------------

/*
 * Test read_amu with expected parsing failure
 */
void test_read_amu_failure(void **state);
// --------------------------------------------------------------------------------

/*
 * Test read_amu with bas file name
 */
void test_read_amu_no_file(void **state);
// ================================================================================
// ================================================================================
#endif /* test_read_files_H */
// ================================================================================
// ================================================================================
// eof
