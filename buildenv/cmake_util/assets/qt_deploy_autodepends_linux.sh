#!/usr/bin/env bash
# Automatically fills the app bundle dir with full Qt and compiler dependencies of the app binary (for Linux).
# Expects param 1 to be the full path of the app binary,
# and the remaining params to be all the designated arguments for linuxdeployqt.

set -x

app_binary=$1

shift
until [ -z "$1" ]
do
  params="$params $1"
  shift
done

./linuxdeployqt-6-x86_64.AppImage $app_binary $params
