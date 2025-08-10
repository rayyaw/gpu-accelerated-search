CC = clang++
COMMON_FLAGS = -Wall -Wextra -std=c++20
OPENCL_FLAGS = -I 'C:/Program Files (x86)/OCL_SDK_Light/include/' 
LINKER_FLAGS = -L 'C:/Program Files (x86)/OCL_SDK_Light/lib/x86_64' -mconsole
SRC_FLAGS = -I src
EXTERNAL_FLAGS = -I src/external
TEST_FLAGS = -I test

CC_ENHANCED = ${CC} ${COMMON_FLAGS} ${OPENCL_FLAGS} ${SRC_FLAGS} ${EXTERNAL_FLAGS}
CC_TEST = ${CC_ENHANCED} ${TEST_FLAGS}

# MARK: Compilation building blocks
clean:
	Remove-Item -Path "bin\*" -Recurse -Force

csr:
	${CC_ENHANCED} -o bin/csr.o -c src/models/linalg/csr.cpp

dim2Tree:
	${CC_ENHANCED} -o bin/dim2Tree.o -c src/models/util/dim2Tree.cpp

geoGraph:
	${CC_ENHANCED} -o bin/geoGraph.o -c src/models/geo/geoGraph.cpp

gpu:
	${CC_ENHANCED} -o bin/gpu.o -c src/gpu/gpu.cpp -lOpenCL

io:
	${CC_ENHANCED} -o bin/io.o -c src/io/io.cpp

# MARK: Scripts
main_graphJsonToBinary:
	${CC_ENHANCED} -o bin/main_graphJsonToBinary.o -c src/scripts/graphJsonToBinary.cpp

# MARK: Executables
graphJsonToBinary: csr dim2Tree geoGraph main_graphJsonToBinary
	${CC_ENHANCED} bin/csr.o bin/dim2Tree.o bin/geoGraph.o bin/main_graphJsonToBinary.o -o bin/graphJsonToBinary.exe ${LINKER_FLAGS}

run: gpu io main csr
	${CC_ENHANCED} bin/main.o bin/io.o bin/gpu.o bin/data_io.o bin/csr.o -o bin/main.exe -lOpenCL -mconsole
	

# MARK: Unit tests
catch: test/catch/catch-main.cpp
	${CC_TEST} -o bin/catch.o -c test/catch/catch-main.cpp

test_models_listWithSize: catch
	${CC_TEST} -o bin/test_models_listWithSize.o -c test/models/util/listWithSize.cpp

test_models_csr: catch csr
	${CC_TEST} -o bin/test_models_csr.o -c test/models/linalg/csr.cpp

test_models_dim2Tree: catch dim2Tree
	${CC_TEST} -o bin/test_models_dim2Tree.o -c test/models/util/dim2Tree.cpp

test_models_geoGraph: geoGraph
	${CC_TEST} -o bin/test_models_geoGraph.o -c test/models/geo/geoGraph.cpp

test: catch test_models_listWithSize test_models_csr test_models_dim2Tree test_models_geoGraph
	${CC_TEST} bin/test_models_geoGraph.o bin/geoGraph.o bin/test_models_listWithSize.o bin/test_models_csr.o bin/test_models_dim2Tree.o bin/dim2Tree.o bin/csr.o bin/catch.o -o bin/runTest.exe ${LINKER_FLAGS}
