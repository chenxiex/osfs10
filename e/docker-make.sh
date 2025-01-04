#!/bin/bash
IMAGE_NAME="oslab"

# 检查镜像是否存在
if [[ "$(podman images -q $IMAGE_NAME 2> /dev/null)" == "" ]]; then
  echo "$IMAGE_NAME 不存在，正在构建..."
  podman build -t $IMAGE_NAME .
fi

podman run --rm -v "$PWD":/usr/src/myapp -w /usr/src/myapp $IMAGE_NAME make "$@"