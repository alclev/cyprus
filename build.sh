rm -rf build
mkdir build
cd build
cmake ..
make
echo "cyprus --> /usr/local/bin"
cp cyprus /usr/local/bin