

FROM ubuntu:latest

LABEL maintainer="zklib"

WORKDIR /workdir

ENV TZ=Europe/Amsterdam
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN \
    apt-get update && apt-get install -y \
        ninja-build \
        python3-pip \
    && rm -rf /var/lib/apt/lists/*

RUN \
     pip3 install --no-cache-dir \
        meson

