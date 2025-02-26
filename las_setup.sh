#!/bin/bash

if command -v podman &> /dev/null; then
  echo -e "Podman is installed on the system"
else
  echo -e "Podman is not installed on the system"
  exit 1
fi

echo -e "Setuping las tool inside ${HOME}"

LAS_HOME_DIR=$HOME/.las
echo -e "Las home directory: ${LAS_HOME_DIR}"
mkdir -p ${LAS_HOME_DIR}

LAS_APP_CONTENT="CMakeLists.txt Containerfile build_run_las.sh src test"
cp -r ${LAS_APP_CONTENT} ${LAS_HOME_DIR}
cd ${LAS_HOME_DIR}

LAS_IMAGE="las_app_image"

if podman inspect ${LAS_IMAGE} > /dev/null 2>&1; then
  echo -e "Las image named: ${LAS_IMAGE} already exists.\nIf you want to remove las image run podman rmi ${LAS_IMAGE}"
else
  echo -e "Building ${LAS_IMAGE} image"
  podman build . -t ${LAS_IMAGE}
fi

