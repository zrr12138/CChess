set -uexo pipefail
mkdir -p build && cd build
cmake ..
make -j40
cd ..