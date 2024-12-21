// ================================================================================
// ================================================================================
// - File:    test_dstructures.c
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

#include "test_dstructures.h"
// ================================================================================
// ================================================================================ 

void test_init_xsec(void **state) {
	(void) state;
    xsec_t* cross_sec = init_xsec(10);
    assert_int_equal(xsec_size(cross_sec), 0);
    assert_int_equal(xsec_alloc(cross_sec), 10);
    free_xsec(cross_sec);
}
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
    void test_init_xsec_gbc(void **state) {
	    (void) state;
        xsec_t* cross_sec XSEC_GBC = init_xsec(10);
        assert_int_equal(xsec_size(cross_sec), 0);
        assert_int_equal(xsec_alloc(cross_sec), 10);
}
// --------------------------------------------------------------------------------

void test_push_xsec(void **state) {
    (void) state;
    xsec_t* cross_sec = init_xsec(4);
    push_xsec(cross_sec, 1.f, 1.f);
    push_xsec(cross_sec, 2.f, 2.f);
    push_xsec(cross_sec, 3.f, 3.f);
    push_xsec(cross_sec, 4.f, 4.f);
    float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    for (size_t i = 0; i < 4; i++) {
        assert_float_equal(data[i], get_xsec(cross_sec, i), 1.0e-3); 
        assert_float_equal(data[i], get_xsec_energy(cross_sec, i), 1.0e-3);
    }
    assert_int_equal(xsec_size(cross_sec), 4);
    assert_int_equal(xsec_alloc(cross_sec), 4);
    free_xsec(cross_sec);
}
// --------------------------------------------------------------------------------

void test_get_xsec_data(void **state) {
    (void) state;
    xsec_t* cross_sec = init_xsec(4);
    push_xsec(cross_sec, 1.f, 1.f);
    push_xsec(cross_sec, 2.f, 2.f);
    push_xsec(cross_sec, 3.f, 3.f);
    push_xsec(cross_sec, 4.f, 4.f);
    float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    xsecData dat;
    for (size_t i = 0; i < 4; i++) {
        dat = get_xsec_data(cross_sec, i);
        assert_float_equal(data[i], dat.xs, 1.0e-3); 
        assert_float_equal(data[i], dat.energy, 1.0e-3);
    }
    assert_int_equal(xsec_size(cross_sec), 4);
    assert_int_equal(xsec_alloc(cross_sec), 4);
    free_xsec(cross_sec);
}
// --------------------------------------------------------------------------------

void test_xsec_resize(void **state) {
    (void) state;
    xsec_t* cross_sec = init_xsec(4);
    push_xsec(cross_sec, 1.f, 1.f);
    push_xsec(cross_sec, 2.f, 2.f);
    push_xsec(cross_sec, 3.f, 3.f);
    push_xsec(cross_sec, 4.f, 4.f);
    push_xsec(cross_sec, 5.f, 5.f);
    float data[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    xsecData dat;
    for (size_t i = 0; i < 5; i++) {
        dat = get_xsec_data(cross_sec, i);
        assert_float_equal(data[i], dat.xs, 1.0e-3); 
        assert_float_equal(data[i], dat.energy, 1.0e-3);
    }
    assert_int_equal(xsec_size(cross_sec), 5);
    assert_int_equal(xsec_alloc(cross_sec), 8);
    free_xsec(cross_sec);
}
// --------------------------------------------------------------------------------

void test_null_xsec_pointer(void **state) {
    (void) state;
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    bool test = push_xsec(NULL, 1.f, 1.f);

    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;

    assert_false(test);
}
// ================================================================================
// ================================================================================ 
#endif
// ================================================================================
// ================================================================================
// eof
