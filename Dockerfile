FROM ubuntu:22.04
RUN apt-get update
RUN apt-get install -y git g++ file cmake doxygen graphviz build-essential zlib1g-dev qtbase5-dev libhdf5-dev libprotobuf-dev libprotoc-dev protobuf-compiler libcurl4-openssl-dev libqwt-qt5-dev libyaml-cpp-dev  

RUN git clone --recurse-submodules -b v1.49.2 --depth 1 --shallow-submodules https://github.com/grpc/grpc \
    && cd grpc/cmake && mkdir _build && cd _build \
    && cmake -DgRPC_INSTALL=ON \
        -DgRPC_BUILD_TESTS=OFF \
        ../.. && make -j4 && make install 

ARG OPENTELEMETRY_CPP_VERSION=1.10.0
RUN git clone https://github.com/open-telemetry/opentelemetry-cpp \
	&& cd opentelemetry-cpp/ \
	&& mkdir build \
	&& cd build \
	&& cmake .. -DCMAKE_CXX_STANDARD=17 -DCMAKE_POSITION_INDEPENDENT_CODE=ON \
				-DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF \
				-DWITH_EXAMPLES=OFF -DWITH_OTLP_GRPC=ON \
	&& make -j$(nproc || sysctl -n hw.ncpu || echo 1) install && cd ../.. && rm -rf opentelemetry-cpp

RUN git clone https://github.com/eclipse-ecal/ecal.git \
    && cd ecal && git submodule init && git submodule update \
    && mkdir _build && cd _build \
    && cmake .. -DCMAKE_BUILD_TYPE=Release -DECAL_THIRDPARTY_BUILD_PROTOBUF=OFF -DECAL_THIRDPARTY_BUILD_CURL=OFF -DECAL_THIRDPARTY_BUILD_HDF5=OFF -DECAL_THIRDPARTY_BUILD_QWT=OFF \
    && make -j4 && cpack -G DEB && dpkg -i _deploy/eCAL-* && ldconfig && cd ../..


COPY ./tstPubSubApp /tstPubSubApp
RUN cd /tstPubSubApp/tstVehPub/ \
    && mkdir -p build && cd build \
    && cmake .. \
    && make -j$(nproc || sysctl -n hw.ncpu || echo 1) install
ENTRYPOINT ./usr/local/bin/tstVehPub