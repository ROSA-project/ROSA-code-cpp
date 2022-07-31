#!/bin/bash

jobs=$1 # for makefiles, number of jobs (for -j option)

# load versions
. deps_version.sh

mkdir -p external_deps/build
mkdir -p external_deps/include

pushd external_deps > /dev/null


# Nlohmann Json library
echo "installing nlohmann json ..."
if [ -d	 "include/nlohmann" ]
then
	echo "-- already installed"
else
	pushd include > /dev/null
	mkdir nlohmann
	pushd nlohmann > /dev/null
	wget https://github.com/nlohmann/json/releases/download/v${NLOHMANN_JSON_VERSION}/json.hpp
	popd > /dev/null
	popd > /dev/null
	echo "-- successfully installed nlohmann json!"
fi

# spdlog
echo "installing spdlog ..."
if [ -d "include/spdlog" ] 
then
	version=$(head -n 1 build/spdlog/version_used.txt)
	echo "-- already installed (version: ${version})" 
else
	# Build library
	git clone https://github.com/gabime/spdlog.git build/spdlog
	cd build/spdlog
	git checkout ${SPD_LOG_COMMIT}
	mkdir build && cd build && cmake .. && make -j${jobs}
	cd ..
	echo ${SPD_LOG_COMMIT} > version_used.txt
	cd ../..

	# Copy the header files into corresponding include folder
	cp -r build/spdlog/include/spdlog include/	
fi

# cd to root
popd > /dev/null