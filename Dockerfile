FROM alpine:latest

LABEL maintainer="zklib"

WORKDIR /zklib

RUN \
    apk add --no-cache \
    gcc \
    gcovr \
    git \
    meson \
    musl-dev \
    pkgconfig \
    valgrind

# install commitlint
RUN \
    apk add --no-cache \
    nodejs \
    npm && \
    npm install -g @commitlint/cli @commitlint/config-conventional

