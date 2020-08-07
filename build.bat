if not exist .\build\NUL mkdir build
pushd
cd build
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=debug
popd