#!/bin/bash

# Generic test runner using CTest and Catch2
# Automatically runs all tests declared in CMakeLists.txt
# Usage: ./run_tests.sh

BUILD_DIR="cmake-build-debug"
REPORTS_DIR="test_reports"

echo "=== Building tests ==="

# Create reports directory
mkdir -p "$REPORTS_DIR"

# Configure and build (cmake -B creates build dir automatically)
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR"

echo ""
echo "=== Running tests ==="

if "$BUILD_DIR/test_main" --reporter console --reporter "junit::out=$REPORTS_DIR/test_main.xml"; then
    echo ""
    echo "All tests passed!"
else
    echo ""
    echo "Some tests failed!"
    exit 1
fi
