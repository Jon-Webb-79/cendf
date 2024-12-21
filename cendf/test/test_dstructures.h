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

#ifndef test_dstructures_H
#define test_dstructures_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "../include/dstructures.h"
// ================================================================================
// ================================================================================ 

/*
 * Test basic xsec initialization and teardown
 */
void test_init_xsec(void **state);
// --------------------------------------------------------------------------------

/*
 * Test xsec initialization and teardown with garbage collection
 */
#if defined(__GNUC__) || defined(__clang__)
 void test_init_xsec_gbc(void **state);
#endif
 // --------------------------------------------------------------------------------
 
/*
 * Test the ability to push data to the xsec_t struct 
 */
void test_push_xsec(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the ability to extract data into the xsecData struct
 */
void test_get_xsec_data(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure xsec vector resizes correctly.
 */
void test_xsec_resize(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure proper safe failure if a null pointer is sent in place of xsec 
 */
void test_null_xsec_pointer(void **state);
// ================================================================================
// ================================================================================
#endif /* test_dstructures_H */
// ================================================================================
// ================================================================================
// eof
