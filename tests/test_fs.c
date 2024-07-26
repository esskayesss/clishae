#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils/fsutils.h"
#include "utils/error.h"
#include "utils/logging.h"

#define TEST_FILE "test_file.txt"
#define TEST_CONTENT "Hello, World!"

int main() {
    char *read_content = NULL;

    // Step 1: Create a file with given content
    LOG_INFO("Creating file: %s", TEST_FILE);
    if (new_file(TEST_FILE, TEST_CONTENT) != 0) {
        LOG_FATAL("Failed to create file: %s", TEST_FILE);
        return 1;
    }
    LOG_SUCCESS("File created successfully: %s", TEST_FILE);

    // Step 2: Read the file and check the content
    LOG_INFO("Reading file: %s", TEST_FILE);
    int read_result = read_file(TEST_FILE, &read_content);
    if (read_result < 0) {
        LOG_FATAL("Failed to read file: %s", TEST_FILE);
        return 1;
    }
    LOG_SUCCESS("File read successfully: %s", TEST_FILE);

    if (strcmp(read_content, TEST_CONTENT) != 0) {
        LOG_ERROR("File content does not match expected content.");
        LOG_DEBUG("Expected: %s", TEST_CONTENT);
        LOG_DEBUG("Got: %s", read_content);
        free(read_content);
        return 1;
    }
    LOG_SUCCESS("File content matches expected content.");
    free(read_content);

    // Step 3: Delete the file
    LOG_INFO("Deleting file: %s", TEST_FILE);
    if (delete_file(TEST_FILE) != 0) {
        LOG_FATAL("Failed to delete file: %s", TEST_FILE);
        return 1;
    }
    LOG_SUCCESS("File deleted successfully: %s", TEST_FILE);

    // Step 4: Try to read the file again, it should fail
    LOG_INFO("Attempting to read deleted file: %s", TEST_FILE);
    if (read_file(TEST_FILE, &read_content) == 0) {
        LOG_ERROR("File should not exist but was read successfully: %s", TEST_FILE);
        free(read_content);
        return 1;
    }
    LOG_SUCCESS("File deletion confirmed, read attempt failed as expected.");

    return 0;
}

