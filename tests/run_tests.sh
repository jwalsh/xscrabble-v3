#!/bin/bash

# Simple test runner for XScrabble

echo "Running XScrabble tests..."

# Directory containing test executables
TEST_DIR="tests"

# Find and run all test executables
for test in ${TEST_DIR}/test_*; do
    if [ -x "$test" ]; then
        echo "Running $test..."
        $test
        if [ $? -ne 0 ]; then
            echo "Test $test failed!"
            exit 1
        fi
    fi
done

echo "All tests passed!"
exit 0
