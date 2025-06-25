CC = clang++
COMMON_FLAGS = -Wall -Wextra -std=c++20
OPENCL_FLAGS = -I 'C:/Program Files (x86)/OCL_SDK_Light/include/' -L 'C:/Program Files (x86)/OCL_SDK_Light/lib/x86_64'
SRC_FLAGS = -I src
TEST_FLAGS = -I test

CC_ENHANCED = ${CC} ${COMMON_FLAGS} ${OPENCL_FLAGS} ${SRC_FLAGS}
CC_TEST = ${CC_ENHANCED} ${TEST_FLAGS}

clean:
	Remove-Item -Path "bin\*" -Recurse -Force

csr:
	${CC_ENHANCED} -o bin/csr.o -c src/models/linalg/csr.cpp -lOpenCL

gpu:
	${CC_ENHANCED} -o bin/gpu.o -c src/gpu/gpu.cpp -lOpenCL

io:
	${CC_ENHANCED} -o bin/io.o -c src/io/io.cpp -lOpenCL

main: gpu io
	${CC_ENHANCED} -o bin/main.o -c src/main.cpp -lOpenCL

run: gpu io main csr
	${CC_ENHANCED} bin/main.o bin/io.o bin/gpu.o bin/csr.o -o bin/main.exe -lOpenCL -mconsole
	

# Using Catch for unit tests
# TODO: Support checking for memory leaks in tests
catch: test/catch/catch-main.cpp
	${CC_TEST} -o bin/catch.o -c test/catch/catch-main.cpp

test_models_listWithSize: catch
	${CC_TEST} -o bin/test_models_listWithSize.o -c test/models/listWithSize.cpp

test_models_csr: catch csr
	${CC_TEST} -o bin/test_models_csr.o -c test/models/linalg/csr.cpp

test: catch test_models_listWithSize test_models_csr
	${CC_TEST} bin/test_models_listWithSize.o bin/test_models_csr.o bin/csr.o bin/catch.o -o bin/runTest.exe -mconsole