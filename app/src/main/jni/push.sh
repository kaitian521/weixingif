#!/usr/bin/env bash
set -x
cp -f libffmpeg.so ../libs/armeabi-v7a
cp -f libgnustl_shared.so ../libs/armeabi-v7a
cp -f libgnustl_shared.so ../libs/armeabi
cp -r -f ../libs/* ../../../../../tmp/hd-android/app/libs/
cp -r -f ../libs/* ../../../../../hd-android/app/libs/
