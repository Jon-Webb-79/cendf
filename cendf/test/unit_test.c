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
    cmocka_unit_test(test_xsec_resize),
    cmocka_unit_test(test_xsec_size_alloc),
    cmocka_unit_test(test_xsec_free_data),
    cmocka_unit_test(test_interp_xsec_exact_match),
    cmocka_unit_test(test_interp_xsec_interpolation),
    cmocka_unit_test(test_interp_xsec_below_range),
    cmocka_unit_test(test_interp_xsec_above_range),
    cmocka_unit_test(test_interp_xsec_single_point),
    cmocka_unit_test(test_interp_xsec_null_pointer),
    cmocka_unit_test(test_interp_xsec_bounds),
    cmocka_unit_test(test_init_string),
    cmocka_unit_test(test_init_string_strcmp),
    #if defined(__GNUC__) || defined(__clang__)
        cmocka_unit_test(test_init_string_gbc),
    #endif
    cmocka_unit_test(test_get_string),
    cmocka_unit_test(test_concat_string_literal),
    cmocka_unit_test(test_concat_string_string),
    cmocka_unit_test(test_string_size_alloc),
    cmocka_unit_test(test_string_free_data),
    cmocka_unit_test(test_copy_string),
    cmocka_unit_test(test_reserve_string),
    cmocka_unit_test(test_init_vector),
    cmocka_unit_test(test_push_back_vector),
    cmocka_unit_test(test_push_front_vector),
    cmocka_unit_test(test_insert_vector),
    #if defined (__GNUC__) || defined (__clang__)
        cmocka_unit_test(test_free_vector_gbc),
    #endif
    cmocka_unit_test(test_pop_back_vector),
    cmocka_unit_test(test_pop_front_vector), 
    cmocka_unit_test(test_pop_any_vector),
    cmocka_unit_test(test_vector_free_data),
    cmocka_unit_test(test_copy_vector),
    cmocka_unit_test(test_init_dictionary),
    cmocka_unit_test(test_insert_dictionary),
    cmocka_unit_test(test_pop_dictionary),
    cmocka_unit_test(test_free_dictionary_gbc),
    cmocka_unit_test(test_update_dictionary),
    cmocka_unit_test(test_update_dictionary_error),
    cmocka_unit_test(test_fetch_element_symbol),
    cmocka_unit_test(test_fetch_element_element),
    cmocka_unit_test(test_fetch_element_category),
    cmocka_unit_test(test_fetch_element_atomic_number),
    cmocka_unit_test(test_fetch_element_weight),
    cmocka_unit_test(test_fetch_element_electroneg),
    cmocka_unit_test(test_fetch_element_melting),
    cmocka_unit_test(test_fetch_element_boiling)
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

