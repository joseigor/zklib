

FROM alpine:latest

LABEL maintainer="zklib"

WORKDIR /workdir

RUN \
    apk add --no-cache \
        gcc \
        gcovr \
        git \
        meson \
        musl-dev \
        pkgconfig \
        valgrind \
