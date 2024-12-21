// ================================================================================
// ================================================================================
// - File:    dstructures.c
// - Purpose: Describe the file purpose here
//
// Source Metadata
// - Author:  Jonathan A. Webb
// - Date:    December 18, 2024
// - Version: 1.0
// - Copyright: Copyright 2024, Jon Webb Inc.
// ================================================================================
// ================================================================================
// Include modules here

#include "include/dstructures.h"

#include <errno.h>
#include <string.h>
#include <stdio.h>

static const size_t XSEC_THRESHOLD = 1 * 1024 * 1024;  // 1 MB
static const size_t XSEC_FIXED_AMOUNT = 1 * 1024 * 1024;  // 1 MB
// ================================================================================
// ================================================================================

// define xsec_t
struct xsec_t {
    float* xs;
    float* energy;
    size_t len; 
    size_t alloc;
};
// -------------------------------------------------------------------------------- 

xsec_t* init_xsec(size_t buffer_length) {
    xsec_t *struct_ptr = malloc(sizeof(xsec_t));
    if (struct_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        return NULL;
    }

    float *xsec_ptr = malloc(sizeof(float) * buffer_length);
    if (xsec_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        free(struct_ptr);
        return NULL;
    }
    float *energy_ptr = malloc(sizeof(float) * buffer_length);
    if (energy_ptr == NULL) {
        errno = ENOMEM;
        fprintf(stderr, "xsec allocation failed with error %s\n", strerror(errno));
        free(struct_ptr);
        free(xsec_ptr);
        return NULL;
    }
    struct_ptr->xs = xsec_ptr;
    struct_ptr->energy = energy_ptr;
    struct_ptr->len = 0;
    struct_ptr->alloc = buffer_length;
    return struct_ptr;
}
// --------------------------------------------------------------------------------

bool push_xsec(xsec_t* cross_section, float xsec, float energy) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross_section passed to push_xsec function\n");
        return false;
    }

    // Check if reallocation is needed
    if (cross_section->alloc <= cross_section->len) {
        size_t new_alloc = cross_section->alloc > 0 ? cross_section->alloc : 16;  // Start with a reasonable size
        if (new_alloc < XSEC_THRESHOLD) {
            new_alloc *= 2;  // Exponential growth for smaller allocations
        } else {
            new_alloc += XSEC_FIXED_AMOUNT;  // Fixed growth beyond threshold
        }

        // Attempt to reallocate the cross-section array
        float* new_xs = realloc(cross_section->xs, new_alloc * sizeof(float));
        if (!new_xs) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate xs in push_xsec\n");
            return false;
        }

        // Attempt to reallocate the energy array
        float* new_energy = realloc(cross_section->energy, new_alloc * sizeof(float));
        if (!new_energy) {
            errno = ENOMEM;
            fprintf(stderr, "Failed to reallocate energy in push_xsec\n");
            free(new_xs);  // Free successfully reallocated xs to prevent memory leak
            return false;
        }

        // Update structure only after successful reallocation
        cross_section->xs = new_xs;
        cross_section->energy = new_energy;
        cross_section->alloc = new_alloc;
    }

    // Append new data
    cross_section->xs[cross_section->len] = xsec;
    cross_section->energy[cross_section->len] = energy;
    cross_section->len++;

    return true;
}
// -------------------------------------------------------------------------------- 

static bool validate_xsec(xsec_t* cross_section, size_t index) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross_section passed to function\n");
        return false;
    }
    if (index >= cross_section->len) {
        errno = EINVAL;
        fprintf(stderr, "Index %zu out of bounds (len: %zu)\n", index, cross_section->len);
        return false;
    }
    return true;
}

// --------------------------------------------------------------------------------

float get_xsec(xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return -1.0f;  // Or define a constant for invalid value
    }
    return cross_section->xs[index];
}
// --------------------------------------------------------------------------------

float get_xsec_energy(xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return -1.0f;  // Or define a constant for invalid value
    }
    return cross_section->energy[index];
}
// --------------------------------------------------------------------------------

const xsecData get_xsec_data(xsec_t* cross_section, size_t index) {
    if (!validate_xsec(cross_section, index)) {
        return (xsecData){ .xs = -1.f, .energy = -1.f };  // Or define an invalid xsec_data value
    }
    return (xsecData){
        .xs = cross_section->xs[index],
        .energy = cross_section->energy[index]
    };
}
// --------------------------------------------------------------------------------

size_t xsec_size(xsec_t* cross_section) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross section passed to xsec_size\n");
        return -1;
    }
    return cross_section->len;
}
// --------------------------------------------------------------------------------

size_t xsec_alloc(xsec_t* cross_section) {
    if (!cross_section || !cross_section->xs || !cross_section->energy) {
        errno = EINVAL;
        fprintf(stderr, "Invalid cross section passed to xsec_alloc\n");
        return -1;
    }
    return cross_section->alloc;
}
// --------------------------------------------------------------------------------

void free_xsec(xsec_t* cross_section) {
    if (!cross_section) {
        errno = EINVAL;
        fprintf(stderr, "Cross section NULL, possible double free\n");
    }
    if (cross_section->xs) { 
        free(cross_section->xs);
        cross_section->xs = NULL;
    }
    if (cross_section->energy) {
        free(cross_section->energy);
        cross_section->energy = NULL;
    }
    free(cross_section);
    cross_section = NULL;
}
// --------------------------------------------------------------------------------

void _free_xsec(xsec_t** cross_section) {
    if (cross_section && *cross_section) {
        free_xsec(*cross_section);
    }
}
// ================================================================================
// ================================================================================
// eof
