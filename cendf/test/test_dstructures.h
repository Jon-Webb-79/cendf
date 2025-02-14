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
// --------------------------------------------------------------------------------

/*
 * Test the size and alloc macro for the xsec_t data type
 */
void test_xsec_size_alloc(void **state);
// --------------------------------------------------------------------------------

/*
 * Test free_data macro for xsec_t data type
 */
void test_xsec_free_data(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_exact_match(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_interpolation(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_below_range(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_above_range(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_single_point(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_null_pointer(void **state);
// --------------------------------------------------------------------------------

void test_interp_xsec_bounds(void **state);
// ================================================================================ 
// ================================================================================
// TEST STRING 

/*
 * Test the init_string function with string to literal comparison
 */
void test_init_string(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the init_string function with string to string comparison
 */
void test_init_string_strcmp(void **state);
// -------------------------------------------------------------------------------- 
/*
 * Test garbage collection for string_t data types
 */
#if defined (__GNUC__) || defined(__clang__)
    void test_init_string_gbc(void **state);
#endif
// --------------------------------------------------------------------------------

/*
 * Test to ensure get_string works properly
 */
void test_get_string(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the ability to concatenate a string_t data type with a string literal
 */
void test_concat_string_literal(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the ability to concatenate a string_t data type with another string_t data type
 */
void test_concat_string_string(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the size and alloc macros with the string_t data type
 */
void test_string_size_alloc(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the free_data macro for the string_t data type
 */
void test_string_free_data(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the copy_string function
 */
void test_copy_string(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the reserve function to ensure it adequately increase mem allocation
 */
void test_reserve_string(void **state);
// ================================================================================
// ================================================================================ 

/*
 * Test to ensure proper initialization of vector 
 */
void test_init_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the ability to push data to back of array
 */
void test_push_back_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure function pushes data to beginning of vector
 */
void test_push_front_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure function inserts anywhere in a vector
 */
void test_insert_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the garbage collection utility
 */
#if defined (__GNUC__) || defined(__clang__)
    void test_free_vector_gbc(void **state);
#endif 
// --------------------------------------------------------------------------------

/*
 * Test pop_back_vector
 */
void test_pop_back_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure function pops from beginning of vector
 */
void test_pop_front_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test to ensure function pops from location within vector
 */
void test_pop_any_vector(void **state);
// --------------------------------------------------------------------------------

/*
 * Test free_data with vector_t data type
 */
void test_vector_free_data(void **state);
// --------------------------------------------------------------------------------

/*
 * Test the ability to copy vector contents to another vector
 */
void test_copy_vector(void **state);
// ================================================================================ 
// ================================================================================ 

void test_init_dictionary(void **state);
// --------------------------------------------------------------------------------

void test_insert_dictionary(void **state);
// --------------------------------------------------------------------------------

void test_pop_dictionary(void **state);
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
    void test_free_dictionary_gbc(void **state);
#endif
// --------------------------------------------------------------------------------

void test_update_dictionary(void **state); 
// --------------------------------------------------------------------------------

void test_update_dictionary_error(void **state);
// ================================================================================ 
// ================================================================================

void test_fetch_element_symbol(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_element(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_category(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_atomic_number(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_weight(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_electroneg(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_melting(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_boiling(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_electron_affin(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_ionization(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_radius(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_hardness(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_modulus(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_density(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_thermal_conductivity(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_electrical_conductivity(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_specific_heat(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_vaporization_heat(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_fusion_heat(void **state);
// --------------------------------------------------------------------------------

void test_fetch_element_electron_config(void **state);
// ================================================================================
// ================================================================================
#endif /* test_dstructures_H */
// ================================================================================
// ================================================================================
// eof
