# Copyright (c) 2013 BillHoo. All Rights Reserved.
# Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
#
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file in the root of the source
# tree. An additional intellectual property rights grant can be found
# in the file PATENTS.  All contributing project authors may
# be found in the AUTHORS file in the root of the source tree.
#include $(BUILD_SHARED_LIBRARY)
#LOCAL_STATIC_LIBRARIES := cpufeatures
#include $(call all-subdir-makefiles)
LOCAL_ROOT_PATH := $(call my-dir)
#include $(LOCAL_ROOT_PATH)/../../protobufStandalone/jni/zlib/Android.mk
#include $(LOCAL_ROOT_PATH)/../../protobufStandalone/jni/google/protobuf/Android.mk
#include $(LOCAL_ROOT_PATH)/zlib.mk
#include $(LOCAL_ROOT_PATH)/protobuf.mk
#include $(LOCAL_ROOT_PATH)/cpp-core-lib/Android.mk
#include $(LOCAL_ROOT_PATH)/protobuf-standalone/Android.mk
#include $(LOCAL_ROOT_PATH)/cpp-hongdian-session/Android.mk
#include $(LOCAL_ROOT_PATH)/hdagent/Android.mk
#include $(LOCAL_ROOT_PATH)/jni.mk

include $(LOCAL_ROOT_PATH)/jni.mk
#include $(LOCAL_ROOT_PATH)/MediaLibFFmpeg.mk

#include $(LOCAL_ROOT_PATH)/HDClientNetworkFramework/Android.mk
#include $(LOCAL_ROOT_PATH)/HDAgent.mk
$(call import-module,android/cpufeatures)
