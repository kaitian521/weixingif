# Copyright (c) 2013 BillHoo. All Rights Reserved.
# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.
LOCAL_ROOT_PATH := $(call my-dir)

include $(LOCAL_ROOT_PATH)/jni.mk

$(call import-module,android/cpufeatures)
