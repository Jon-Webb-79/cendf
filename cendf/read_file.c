// ================================================================================
// ================================================================================
// - File:    rad.c
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

#include "include/read_file.h"

#include <string.h>
#include <errno.h>

// ================================================================================ 
// ================================================================================ 

float read_amu(const char *filename, const float neutron_mass) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        errno = ENOENT;
        fprintf(stderr, 
                "Error: Unable to open file %s: %s\n", filename, 
                strerror(errno)
        );
        return -1.0f; // Return a negative value to indicate an error.
    }

    char line[256];

    // Skip the first line
    if (!fgets(line, sizeof(line), file)) {
        errno = ENOEXEC;
        fprintf(stderr, 
                "Error: Unable to read the first line from %s with error %s", 
                filename, 
                strerror(errno)
        );
        fclose(file);
        return -1.0f;
    }

    // Read the second line
    if (!fgets(line, sizeof(line), file)) {
        errno = ENOEXEC;
        fprintf(stderr, 
                "Error: Unable to read the second line from %s with error %s", 
                filename, strerror(errno)
        );
        fclose(file);
        return -1.0f;
    }

    // Extract the second floating-point number (atomic mass) from the second line
    float zaid, atomic_mass;
    if (sscanf(line, "%f %f", &zaid, &atomic_mass) != 2) {
        errno = EINVAL;
        fprintf(stderr, 
                "Error: Unable to parse atomic mass from the second line\n"
        );
        fclose(file);
        return -1.0f;
    }

    fclose(file);
    return atomic_mass * neutron_mass;
}
// ================================================================================
// ================================================================================
// eof
