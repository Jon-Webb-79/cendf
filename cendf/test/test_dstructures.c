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

#include <float.h>
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
// --------------------------------------------------------------------------------

void test_xsec_size_alloc(void **state) {
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
    assert_int_equal(size(cross_sec), 4);
    assert_int_equal(alloc(cross_sec), 4);
    free_xsec(cross_sec);
}
// --------------------------------------------------------------------------------

void test_xsec_free_data(void **state) {
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
    assert_int_equal(size(cross_sec), 4);
    assert_int_equal(alloc(cross_sec), 4);
    free_data(cross_sec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_exact_match(void **state) {
    (void) state; // Unused
    xsec_t* xsec = init_xsec(5);
    push_xsec(xsec, 10.f, 1.f);
    push_xsec(xsec, 20.f, 2.f);
    push_xsec(xsec, 30.f, 3.f);
    push_xsec(xsec, 40.f, 4.f);
    push_xsec(xsec, 50.f, 5.f);
    float result = interp_xsec(xsec, 3.0);
    assert_float_equal(result, 30.0, 1e-6);
    free_xsec(xsec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_interpolation(void **state) {
    (void) state; // Unused
    xsec_t* xsec = init_xsec(5);
    assert_non_null(xsec);

    push_xsec(xsec, 10.f, 1.f);
    push_xsec(xsec, 20.f, 2.f);
    push_xsec(xsec, 30.f, 3.f);
    push_xsec(xsec, 40.f, 4.f);
    push_xsec(xsec, 50.f, 5.f);

    float result = interp_xsec(xsec, 2.5);
    assert_float_equal(result, 25.0, 1e-6);

    free_xsec(xsec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_below_range(void **state) {
    (void) state; // Unused
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    xsec_t* xsec = init_xsec(5);
    assert_non_null(xsec);
    push_xsec(xsec, 10.f, 1.f);
    push_xsec(xsec, 20.f, 2.f);
    push_xsec(xsec, 30.f, 3.f);
    push_xsec(xsec, 40.f, 4.f);
    push_xsec(xsec, 50.f, 5.f);

    float result = interp_xsec(xsec, 0.5);
    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;
    assert_float_equal(result, -1.0, 1e-6); // Assuming -1.0 indicates out of bounds

    free_xsec(xsec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_above_range(void **state) {
    (void) state; // Unused
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    xsec_t* xsec = init_xsec(5);
    assert_non_null(xsec);

    push_xsec(xsec, 10.f, 1.f);
    push_xsec(xsec, 20.f, 2.f);
    push_xsec(xsec, 30.f, 3.f);
    push_xsec(xsec, 40.f, 4.f);
    push_xsec(xsec, 50.f, 5.f);

    float result = interp_xsec(xsec, 5.5);
    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;
    assert_float_equal(result, -1.0, 1e-6); // Assuming -1.0 indicates out of bounds

    free_xsec(xsec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_single_point(void **state) {
    (void) state; // Unused
    xsec_t* xsec = init_xsec(1);
    assert_non_null(xsec);

    push_xsec(xsec, 30.f, 3.f);

    float result = interp_xsec(xsec, 3.0);
    assert_float_equal(result, 30.0, 1e-6);

    free_xsec(xsec);
}
// --------------------------------------------------------------------------------

void test_interp_xsec_null_pointer(void **state) {
    (void) state; // Unused
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    float result = interp_xsec(NULL, 3.0);
    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;
    assert_float_equal(result, -1.0, 1e-6); // Assuming -1.0 indicates invalid input
}
// --------------------------------------------------------------------------------

void test_interp_xsec_bounds(void **state) {
    (void) state; // Unused
    xsec_t* xsec = init_xsec(5);
    assert_non_null(xsec);

    push_xsec(xsec, 10.f, 1.f);
    push_xsec(xsec, 20.f, 2.f);
    push_xsec(xsec, 30.f, 3.f);
    push_xsec(xsec, 40.f, 4.f);
    push_xsec(xsec, 50.f, 5.f);

    float lower_result = interp_xsec(xsec, 1.0);
    float upper_result = interp_xsec(xsec, 5.0);

    assert_float_equal(lower_result, 10.0, 1e-6);
    assert_float_equal(upper_result, 50.0, 1e-6);

    free_xsec(xsec);
}
// ================================================================================
// ================================================================================ 

void test_init_string(void **state) {
    (void) state;

    string_t* str = init_string("Hello");
    int cmp = compare_strings(str, "Hello");
    assert_int_equal(cmp, 0);
    assert_int_equal(5, string_size(str));
    assert_int_equal(6, string_alloc(str));
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_init_string_strcmp(void **state) {
    (void) state;

    string_t* str1 = init_string("Hello");
    string_t* str2 = init_string("Hello");
    int cmp = compare_strings(str1, str2);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, string_size(str1));
    assert_int_equal(6, string_alloc(str1));
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

#if defined (__GNUC__) || defined(__clang__)
    void test_init_string_gbc(void **state) {
        (void) state;

        string_t* str STRING_GBC = init_string("Hello");
        int cmp = compare_strings(str, "Hello");
        assert_int_equal(cmp, 0);
        assert_int_equal(5, string_size(str));
        assert_int_equal(6, string_alloc(str));
    }
#endif
// --------------------------------------------------------------------------------

void test_get_string(void **state) {
    (void) state;

    string_t* str = init_string("Hello");
    const char* new_str = get_string(str);
    int cmp = compare_strings(str, (char*)new_str);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, string_size(str));
    assert_int_equal(6, string_alloc(str));
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_string_literal(void **state) {
    string_t* str = init_string("Hello");
    bool test = string_concat(str, " World!");
    assert_true(test);
    int cmp = compare_strings(str,"Hello World!");
    assert_int_equal(0, cmp);
    assert_int_equal(12, string_size(str));
    assert_int_equal(13, string_alloc(str));
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_concat_string_string(void **state) {
    string_t* str1 = init_string("Hello");
    string_t* str2 = init_string(" World!");
    bool test = string_concat(str1, str2);
    assert_true(test);
    int cmp = compare_strings(str1,"Hello World!");
    assert_int_equal(0, cmp);
    assert_int_equal(12, string_size(str1));
    assert_int_equal(13, string_alloc(str1));
    free_string(str1);
    free_string(str2);
}
// --------------------------------------------------------------------------------

void test_string_size_alloc(void **state) {
    (void) state;

    string_t* str = init_string("Hello");
    const char* new_str = get_string(str);
    int cmp = compare_strings(str, (char*)new_str);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, size(str));
    assert_int_equal(6, alloc(str));
    free_string(str);
}
// --------------------------------------------------------------------------------

void test_string_free_data(void **state) {
    (void) state;
    string_t* str = init_string("Hello");
    const char* new_str = get_string(str);
    int cmp = compare_strings(str, (char*)new_str);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, size(str));
    assert_int_equal(6, alloc(str));
    free_data(str);
}
// --------------------------------------------------------------------------------

void test_copy_string(void **state) {
    (void) state;
    string_t* str = init_string("Hello");
    reserve_string(str, 20);
    string_t* new_str = copy_string(str);
    int cmp = compare_strings(str, new_str);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, size(new_str));
    assert_int_equal(20, alloc(new_str));
    free_data(str);
    free_data(new_str);
}
// --------------------------------------------------------------------------------

void test_reserve_string(void **state) {
    (void) state;
    string_t* str = init_string("Hello");
    reserve_string(str, 20);
    const char* new_str = get_string(str);
    int cmp = compare_strings(str, (char*)new_str);
    assert_int_equal(cmp, 0);
    assert_int_equal(5, size(str));
    assert_int_equal(20, alloc(str));
    free_data(str);
}
// ================================================================================
// ================================================================================ 

void test_init_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(10);
    assert_int_equal(0, size(vec));
    assert_int_equal(10, alloc(vec));
    free_vector(vec);
}
// --------------------------------------------------------------------------------

void test_push_back_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(5, size(vec));
    float dat[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);
}
// --------------------------------------------------------------------------------

void test_push_front_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_front_vector(vec, 1.0);
    push_front_vector(vec, 2.0); 
    push_front_vector(vec, 3.0);
    push_front_vector(vec, 4.0);
    push_front_vector(vec, 5.0);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(5, size(vec));
    float dat[5] = {5.f, 4.f, 3.f, 2.f, 1.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);    
}
// --------------------------------------------------------------------------------

void test_insert_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(6);
    push_front_vector(vec, 1.0);
    push_front_vector(vec, 2.0); 
    push_front_vector(vec, 3.0);
    insert_vector(vec, 4.0, 0);
    insert_vector(vec, 5.0, 2);
    insert_vector(vec, 12.0, 5);
    assert_int_equal(6, alloc(vec));
    assert_int_equal(6, size(vec));
    float dat[6] = {4.f, 3.f, 5.f, 2.f, 1.f, 12.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);    
}
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined (__clang__)
    void test_free_vector_gbc(void **state) {
        (void) state;
        vector_t* vec VECTOR_GBC = init_vector(5);
        push_back_vector(vec, 1.0);
        push_back_vector(vec, 2.0); 
        push_back_vector(vec, 3.0);
        push_back_vector(vec, 4.0);
        push_back_vector(vec, 5.0);
        assert_int_equal(5, alloc(vec));
        assert_int_equal(5, size(vec));
        float dat[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
        for (size_t i = 0; i < size(vec); i++) {
            assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
        }
    }
#endif
// --------------------------------------------------------------------------------

void test_pop_back_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    float var = pop_back_vector(vec);
    assert_float_equal(5.f, var, 1.0e-3);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(4, size(vec));
    float dat[4] = {1.f, 2.f, 3.f, 4.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);
}
// --------------------------------------------------------------------------------

void test_pop_front_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    float val;
    val = pop_front_vector(vec);
    assert_float_equal(val, 1.f, 1.0e-3);
    val = pop_front_vector(vec);
    assert_float_equal(val, 2.f, 1.0e-3);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(3, size(vec));
    float dat[3] = {3.f, 4.f, 5.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);    
}
// --------------------------------------------------------------------------------

void test_pop_any_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(6);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    push_back_vector(vec, 6.0);
    float val;
    val = pop_any_vector(vec, 0);
    assert_float_equal(val, 1.f, 1.0e-3);
    val = pop_any_vector(vec, size(vec) - 1);
    assert_float_equal(val, 6.f, 1.0e-3);
    val = pop_any_vector(vec, 1);
    assert_float_equal(val, 3.f, 1.0e-3);
    assert_int_equal(6, alloc(vec));
    assert_int_equal(3, size(vec));
    float dat[3] = {2.f, 4.f, 5.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_vector(vec);    
}
// --------------------------------------------------------------------------------

void test_vector_free_data(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    float var = pop_back_vector(vec);
    assert_float_equal(5.f, var, 1.0e-3);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(4, size(vec));
    float dat[4] = {1.f, 2.f, 3.f, 4.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }
    free_data(vec);    
}
// --------------------------------------------------------------------------------

void test_copy_vector(void **state) {
    (void) state;
    vector_t* vec = init_vector(5);
    push_back_vector(vec, 1.0);
    push_back_vector(vec, 2.0); 
    push_back_vector(vec, 3.0);
    push_back_vector(vec, 4.0);
    push_back_vector(vec, 5.0);
    assert_int_equal(5, alloc(vec));
    assert_int_equal(5, size(vec));
    float dat[5] = {1.f, 2.f, 3.f, 4.f, 5.f};
    for (size_t i = 0; i < size(vec); i++) {
        assert_float_equal(dat[i], get_vector(vec, i), 1.0e-3);
    }

    vector_t* new_vec = copy_vector(vec);

    assert_int_equal(5, alloc(new_vec));
    assert_int_equal(5, size(new_vec));
    for (size_t i = 0; i < size(new_vec); i++) {
        assert_float_equal(dat[i], get_vector(new_vec, i), 1.0e-3);
    }
    free_vector(vec);
    free_vector(new_vec);
}
// ================================================================================
// ================================================================================
// TEST DICTIONARY 

void test_init_dictionary(void **state) {
    dict_t* dict = init_dict();
    assert_int_equal(size(dict), 0);
    assert_int_equal(alloc(dict), 3);
    assert_int_equal(dict_hash_size(dict), 0);
    free_data(dict);
}
// --------------------------------------------------------------------------------

void test_insert_dictionary(void **state) {
    dict_t* dict = init_dict();
    insert_dict(dict, "One", 1.0);
    insert_dict(dict, "Two", 2.0);
    insert_dict(dict, "Three", 3.0);
    assert_int_equal(size(dict), 3);
    assert_int_equal(alloc(dict), 3);
    assert_int_equal(dict_hash_size(dict), 3);
    assert_float_equal(1.0, get_dict_value(dict, "One"), 1.0e-6);
    assert_float_equal(2.0, get_dict_value(dict, "Two"), 1.0e-6);
    assert_float_equal(3.0, get_dict_value(dict, "Three"), 1.0e-6);
    free_data(dict);
}
// --------------------------------------------------------------------------------

void test_pop_dictionary(void **state) {
    dict_t* dict = init_dict();
    insert_dict(dict, "One", 1.0);
    insert_dict(dict, "Two", 2.0);
    insert_dict(dict, "Three", 3.0);
    float value = pop_dict(dict, "Three");
    assert_float_equal(3.0, value, 1.0e-3);
    assert_int_equal(size(dict), 2);
    assert_int_equal(alloc(dict), 3);
    assert_int_equal(dict_hash_size(dict), 3);
    assert_float_equal(1.0, get_dict_value(dict, "One"), 1.0e-6);
    assert_float_equal(2.0, get_dict_value(dict, "Two"), 1.0e-6);
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    assert_float_equal(FLT_MAX, get_dict_value(dict, "Three"), 1.0e-6);
    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;
    free_data(dict);
}
// --------------------------------------------------------------------------------

#if defined(__GNUC__) || defined(__clang__)
    void test_free_dictionary_gbc(void **state) {
        dict_t* dict DICT_GBC = init_dict();
        insert_dict(dict, "One", 1.0);
        insert_dict(dict, "Two", 2.0);
        insert_dict(dict, "Three", 3.0);
        assert_int_equal(size(dict), 3);
        assert_int_equal(alloc(dict), 3);
        assert_int_equal(dict_hash_size(dict), 3);
        assert_float_equal(1.0, get_dict_value(dict, "One"), 1.0e-6);
        assert_float_equal(2.0, get_dict_value(dict, "Two"), 1.0e-6);
        assert_float_equal(3.0, get_dict_value(dict, "Three"), 1.0e-6);
    }
#endif
// --------------------------------------------------------------------------------

void test_update_dictionary(void **state) {
    dict_t* dict = init_dict();
    insert_dict(dict, "One", 1.0);
    insert_dict(dict, "Two", 2.0);
    insert_dict(dict, "Three", 3.0);
    update_dict(dict, "Three", 4.0);
    assert_int_equal(size(dict), 3);
    assert_int_equal(alloc(dict), 3);
    assert_int_equal(dict_hash_size(dict), 3);
    assert_float_equal(1.0, get_dict_value(dict, "One"), 1.0e-6);
    assert_float_equal(2.0, get_dict_value(dict, "Two"), 1.0e-6);
    assert_float_equal(4.0, get_dict_value(dict, "Three"), 1.0e-6);
    free_data(dict);
}
// --------------------------------------------------------------------------------

void test_update_dictionary_error(void **state) {
    dict_t* dict = init_dict();
    insert_dict(dict, "One", 1.0);
    insert_dict(dict, "Two", 2.0);
    insert_dict(dict, "Three", 3.0);
    // Backup original stderr
    FILE *original_stderr = stderr;

    // Redirect stderr to /dev/null to suppress output
    stderr = fopen("/dev/null", "w");
    if (!stderr) {
        fprintf(original_stderr, "Failed to redirect stderr\n");
        return;
    }
    bool test = update_dict(dict, "Five", 4.0);
    assert_false(test);
    // Close the redirected stderr and restore the original stderr
    fclose(stderr);
    stderr = original_stderr;

    assert_int_equal(size(dict), 3);
    assert_int_equal(alloc(dict), 3);
    assert_int_equal(dict_hash_size(dict), 3);
    assert_float_equal(1.0, get_dict_value(dict, "One"), 1.0e-6);
    assert_float_equal(2.0, get_dict_value(dict, "Two"), 1.0e-6);
    assert_float_equal(3.0, get_dict_value(dict, "Three"), 1.0e-6);
    free_data(dict);
}
// ================================================================================
// ================================================================================ 
#endif
// ================================================================================
// ================================================================================
// eof
