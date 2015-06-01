LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE := libGifProcess
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_SRC_FILES += \
    $(LOCAL_PATH)/giflossy/src/fmalloc.c \
    $(LOCAL_PATH)/giflossy/src/xform.c \
    $(LOCAL_PATH)/giflossy/src/giffunc.c \
    $(LOCAL_PATH)/giflossy/src/support.c \
    $(LOCAL_PATH)/giflossy/src/optimize.c \
    $(LOCAL_PATH)/giflossy/src/gifwrite.c \
    $(LOCAL_PATH)/giflossy/src/merge.c \
    $(LOCAL_PATH)/giflossy/src/gifunopt.c \
    $(LOCAL_PATH)/giflossy/src/quantize.c \
    $(LOCAL_PATH)/giflossy/src/gifread.c \
    $(LOCAL_PATH)/giflossy/src/strerror.c \
    $(LOCAL_PATH)/giflossy/src/gifsicle.c \
    $(LOCAL_PATH)/giflossy/src/clp.c

LOCAL_SRC_FILES +=\
	$(LOCAL_PATH)/kai_com_weixingif_gifsicle_GifSicleRequest.cpp \
	$(LOCAL_PATH)/tools.cpp \
	$(LOCAL_PATH)/cloudatlas/base/logger.cpp

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/cloudatlas/ \
    $(LOCAL_PATH)/cloudatlas/base \
    $(LOCAL_PATH)/giflossy/src/ \
    $(LOCAL_PATH)/giflossy/ \
    $(LOCAL_PATH)/giflossy/include \
    $(LOCAL_PATH)/giflossy/include/lcdf \
    $(LOCAL_PATH)/giflossy/include/lsdfgif

LOCAL_LDLIBS+= -L$(SYSROOT)/usr/lib -llog -lz -lm
#LOCAL_LDLIBS += $(LOCAL_PATH)/libffmpeg.so
#LOCAL_SHARED_LIBRARIES += ABC

ifndef NDK_ROOT
include external/stlport/libstlport.mk
endif
include $(BUILD_SHARED_LIBRARY)
