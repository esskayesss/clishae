#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network/context.h"
#include "utils/logging.h"

// Test result enumeration
typedef enum {
    TEST_SUCCESS,
    TEST_FAILURE
} TestResult;

// Function prototypes
void setup();
void teardown();
TestResult test_req_post_success();
TestResult test_req_get_success();
TestResult test_req_get_failure_ctxlen();
TestResult test_req_get_failure_version();
TestResult test_req_post_with_expect();
TestResult test_req_post_with_large_body();
TestResult test_req_invalid_method();

// Setup function to initialize resources
void setup() {
    initialize_logger(NULL, LEVEL_TRACE);
}

// Teardown function to clean up resources
void teardown() {
    // Add any necessary cleanup code here
}

// Function to run a single test and report the result
void run_test(const char *test_name, TestResult (*test_func)()) {
    setup();
    TestResult result = test_func();
    if (result == TEST_SUCCESS) {
        LOG_SUCCESS("Test `%s` succeeded", test_name);
    } else {
        LOG_ERROR("Test `%s` failed", test_name);
    }
    teardown();
}

// Test implementations
TestResult test_req_post_success() {
    char* request =
        "POST /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (!ctx) return TEST_FAILURE;
    free_context(ctx);

    return TEST_SUCCESS;
}

TestResult test_req_get_success() {
    char* request =
        "GET /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (!ctx) return TEST_FAILURE;
    free_context(ctx);

    return TEST_SUCCESS;
}

TestResult test_req_get_failure_ctxlen() {
    char* request =
        "GET /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 17\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (ctx) {
        free_context(ctx);
        return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}

TestResult test_req_get_failure_version() {
    char* request =
        "GET /path/to/resource HTTP/1.0\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (ctx) {
        free_context(ctx);
        return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}

TestResult test_req_post_with_expect() {
    char* request =
        "POST /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Expect: 100-continue\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (!ctx) return TEST_FAILURE;
    if (get_header_value(ctx->headers, "expect") == NULL) {
        LOG_ERROR("Expect header was not parsed correctly");
        free_context(ctx);
        return TEST_FAILURE;
    }
    free_context(ctx);

    return TEST_SUCCESS;
}

TestResult test_req_post_with_large_body() {
    char* request =
        "POST /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 9000\r\n"  // Large but within 8192 limit for the sake of this test
        "Connection: close\r\n"
        "\r\n"
        "1234567890...";  // Assume this continues to reach 9000 characters
    
    Context *ctx = parse_context(request);
    if (ctx) {
        LOG_ERROR("Large body should have failed but did not");
        free_context(ctx);
        return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}

TestResult test_req_invalid_method() {
    char* request =
        "PATCH /path/to/resource HTTP/1.1\r\n"
        "Host: example.com\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 12\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World!";
    
    Context *ctx = parse_context(request);
    if (ctx) {
        LOG_ERROR("Unsupported method should have failed but did not");
        free_context(ctx);
        return TEST_FAILURE;
    }

    return TEST_SUCCESS;
}

// Main function to run all tests
int main() {
    run_test("req_get_success", test_req_get_success);
    run_test("req_post_success", test_req_post_success);
    run_test("req_get_failure_ctxlen", test_req_get_failure_ctxlen);
    run_test("req_get_failure_version", test_req_get_failure_version);
    run_test("req_post_with_expect", test_req_post_with_expect);
    run_test("req_post_with_large_body", test_req_post_with_large_body);
    run_test("req_invalid_method", test_req_invalid_method);

    return 0;
}

