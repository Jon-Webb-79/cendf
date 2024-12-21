// ================================================================================
// ================================================================================
// - File:    unit_test.c
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 17, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>

#include "test_read_files.h"
#include "test_dstructures.h"
// ================================================================================
// ================================================================================
// Begin code

const struct CMUnitTest test_read_files[] = {
	cmocka_unit_test(test_read_amu_nominal),
    cmocka_unit_test(test_read_amu_failure),
    cmocka_unit_test(test_read_amu_no_file),
};
// -------------------------------------------------------------------------------- 

const struct CMUnitTest test_data_structures[] = {
	cmocka_unit_test(test_init_xsec),
    #if defined (__GNUC__) || defined(__clang__)
        cmocka_unit_test(test_init_xsec_gbc),
    #endif
    cmocka_unit_test(test_push_xsec),
    cmocka_unit_test(test_get_xsec_data),
    cmocka_unit_test(test_xsec_resize)
};
// ================================================================================ 
// ================================================================================ 

int main(int argc, const char * argv[]) {
    int status;

    status = cmocka_run_group_tests(test_read_files, NULL, NULL);
    if (status != 0) 
        return status;
    status = cmocka_run_group_tests(test_data_structures, NULL, NULL); 
	return status;
}
// ================================================================================
// ================================================================================
// eof

