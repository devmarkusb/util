#!/usr/bin/env bash
# Automatically fills the app bundle dir with full Qt and compiler dependencies of the app binary (for Linux).
# Expects param 1 to be the full path of the app desktop file,
# and the remaining params to be all the designated arguments for linuxdeployqt.

set -x

app_desktop_file=$1

shift
until [ -z "$1" ]
do
  params="$params $1"
  shift
done

./linuxdeployqt-continuous-x86_64.AppImage $app_desktop_file $params
