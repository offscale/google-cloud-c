FROM alpine

RUN apk add --no-cache cmake --repository=http://dl-cdn.alpinelinux.org/alpine/edge/main
RUN apk update && \
    apk add --no-cache \
      # bash is needed for unit testing only
      bash \
      # the remaining packages are needed for monsid development
      build-base \
      cmake \
      curl \
      git \
      g++ \
      libcurl \
      linux-headers \
      make \
      musl-dev \
      ninja \
      pkgconfig \
      tar \
      unzip \
      zip

CMD ['/usr/local/bin/cmake', '--version']

RUN git clone --depth=1 --branch='project0' https://github.com/offscale/vcpkg && \
    ./vcpkg/bootstrap-vcpkg.sh && \
    ./vcpkg/vcpkg install c89stringutils cauthflow parson libcurl-simple-https

COPY . /google-cloud-c

WORKDIR /google-cloud-c/build

RUN cmake \
    -DCMAKE_BUILD_TYPE="Debug" \
    -DCMAKE_TOOLCHAIN_FILE="/vcpkg/scripts/buildsystems/vcpkg.cmake" \
    .. && \
    cmake --build .
