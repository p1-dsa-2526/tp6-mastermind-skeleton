#!/bin/bash

# Generic test runner using CTest
# Automatically runs all tests declared in CMakeLists.txt
# Usage: ./run_tests.sh

set -e

BUILD_DIR="cmake-build-debug"
REPORTS_DIR="test_reports"

echo "=== Building tests ==="

# Create reports directory
mkdir -p "$REPORTS_DIR"

# Configure and build (cmake -B creates build dir automatically)
cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR"

echo ""
echo "=== Running tests with CTest ==="

# Run all tests declared in CMakeLists.txt using CTest
cd "$BUILD_DIR"
ctest --output-on-failure --verbose
