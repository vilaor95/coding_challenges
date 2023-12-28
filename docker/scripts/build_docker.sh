#!/bin/bash

cp scripts/Dockerfile.template Dockerfile

docker buildx build --force-rm --squash --no-cache --tag devel:latest .
