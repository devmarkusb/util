#!/usr/bin/env bash
# Automatically fills the app bundle dir with full Qt and compiler dependencies of the app binary (for MacOS).
# Expects param 1 to be the full path of the app bundle dir, param 2 to be the full path to macdeployqt,
# and the remaining params to be all the designated arguments for macdeployqt.

set -x

app_bundle_dir=$1
macdeployqt_path=$2

shift
shift
until [ -z "$1" ]
do
  params="$params $1"
  shift
done

# 'workaround', otherwise the tool doesn't seem to be able to find qml files
cd $macdeployqt_path

./macdeployqt $app_bundle_dir $params
