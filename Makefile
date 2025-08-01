CC = clang++
COMMON_FLAGS = -Wall -Wextra -std=c++20
OPENCL_FLAGS = -I 'C:/Program Files (x86)/OCL_SDK_Light/include/' -L 'C:/Program Files (x86)/OCL_SDK_Light/lib/x86_64'
SRC_FLAGS = -I src
EXTERNAL_FLAGS = -I src/external
TEST_FLAGS = -I test

CC_ENHANCED = ${CC} ${COMMON_FLAGS} ${OPENCL_FLAGS} ${SRC_FLAGS} ${EXTERNAL_FLAGS}
CC_TEST = ${CC_ENHANCED} ${TEST_FLAGS}

clean:
	Remove-Item -Path "bin\*" -Recurse -Force

csr:
	${CC_ENHANCED} -o bin/csr.o -c src/models/linalg/csr.cpp -lOpenCL

dim2Tree:
	${CC_ENHANCED} -o bin/dim2Tree.o -c src/models/util/dim2Tree.cpp -lOpenCL

geoGraph:
	${CC_ENHANCED} -o bin/geoGraph.o -c src/models/geo/geoGraph.cpp -lOpenCL

gpu:
	${CC_ENHANCED} -o bin/gpu.o -c src/gpu/gpu.cpp -lOpenCL

io:
	${CC_ENHANCED} -o bin/io.o -c src/io/io.cpp -lOpenCL

data_io:
	${CC_ENHANCED} -o bin/data_io.o -c src/data_io/data_io.cpp -lOpenCL

main: gpu io
	${CC_ENHANCED} -o bin/main.o -c src/main.cpp -lOpenCL

run: gpu io data_io main csr
	${CC_ENHANCED} bin/main.o bin/io.o bin/gpu.o bin/data_io.o bin/csr.o -o bin/main.exe -lOpenCL -mconsole
	

# Using Catch for unit tests
catch: test/catch/catch-main.cpp
	${CC_TEST} -o bin/catch.o -c test/catch/catch-main.cpp

test_models_listWithSize: catch
	${CC_TEST} -o bin/test_models_listWithSize.o -c test/models/util/listWithSize.cpp

test_models_csr: catch csr
	${CC_TEST} -o bin/test_models_csr.o -c test/models/linalg/csr.cpp

test_models_dim2Tree: catch dim2Tree
	${CC_TEST} -o bin/test_models_dim2Tree.o -c test/models/util/dim2Tree.cpp

test: catch test_models_listWithSize test_models_csr test_models_dim2Tree
	${CC_TEST} bin/test_models_listWithSize.o bin/test_models_csr.o bin/test_models_dim2Tree.o bin/dim2Tree.o bin/csr.o bin/catch.o -o bin/runTest.exe -mconsole
