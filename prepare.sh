#!/bin/bash

# load versions
. deps_version.sh

mkdir -p external_deps/build
mkdir -p external_deps/include

pushd external_deps > /dev/null

# Nlohmann Json library
echo "installing nlohmann json ..."
if [ -d	 "include/nlohmann" ]
then
	echo "-- already installed."
else
	pushd include > /dev/null
	mkdir nlohmann
	pushd nlohmann > /dev/null
	wget https://github.com/nlohmann/json/releases/download/v${NLOHMANN_JSON_VERSION}/json.hpp
	popd > /dev/null
	popd > /dev/null
	echo "-- successfully installed nlohmann json!"
fi

# SimpleJson
# if [ -d	 "build/simplejson" ]
# then
# 	echo ${simplejson} > build/simplejson/version_used.txt
# else
# 	# Build library
# 	git clone https://github.com/nbsdx/SimpleJSON build/simplejson
# 	cd build/simplejson
# 	git checkout ${simplejson}
# 	echo ${simplejson} > version_used.txt
# 	cd ../..

# 	# Copy the header files into corresponding include folder
# 	mkdir -p include/simplejson
# 	cp build/simplejson/json.hpp include/simplejson	
# fi

# cd to root
popd > /dev/null