cd ..
cmake ./maker
cmake --build . --config Release
mkdir build
copy /Y "Release/" "build/"