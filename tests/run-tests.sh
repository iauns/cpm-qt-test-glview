#!/bin/bash
cd "$(dirname "$0")"

if [ ! -d ./bin ]; then
  mkdir -p ./bin
fi

# Ensure we fail immediately if any command fails.
set -e

pushd ./bin > /dev/null
  cmake -DCMAKE_BUILD_TYPE=Debug ..
  make
  # We only test the build on Travis. There aren't any tests that we run
  # yet.
  #echo "Running test (viewer)"
  #./glview_test
popd

