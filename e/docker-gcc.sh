#!/bin/bash

CONTAINER_NAME="gcc-multilib-container"

# 检查镜像是否存在
if [[ "$(podman images -q gcc-multilib:5.4 2> /dev/null)" == "" ]]; then
  echo "镜像 gcc-multilib:5.4 不存在，正在构建..."
  podman build -t gcc-multilib:5.4 .
fi

podman run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp gcc-multilib:5.4 gcc "$@"