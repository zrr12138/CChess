set -uexo pipefail
git submodule update --init --recursive
mkdir -p build && cd build
cmake ..
make -j40
cd ..