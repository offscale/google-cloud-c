FROM alpine

RUN apk add --no-cache gcc cmake make musl-dev linux-headers pkgconfig libcurl

COPY . /google-cloud-c

WORKDIR /google-cloud-c/build

RUN cmake -DCMAKE_BUILD_TYPE="Debug" .. && \
    cmake --build .
