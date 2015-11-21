LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_ARM_MODE := arm
LOCAL_MODULE := libGifProcess
LOCAL_MODULE_TAGS := optional
LOCAL_CPP_EXTENSION := .cc .cpp
#MAGIC_PATH := $(LOCAL_PATH)/ImageMagick-6.7.3-0/Magick++
#MAGIC_WAND := $(LOCAL_PATH)/ImageMagick-6.7.3-0/wand

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

#LOCAL_SRC_FILES += \
#	$(MAGIC_PATH)/lib/Functions.cpp \
#	$(MAGIC_PATH)/lib/Drawable.cpp	\
#	$(MAGIC_PATH)/lib/Blob.cpp \
#	$(MAGIC_PATH)/lib/Image.cpp \
#	$(MAGIC_PATH)/lib/Thread.cpp \
#	$(MAGIC_PATH)/lib/Pixels.cpp \
##	$(MAGIC_PATH)/lib/BlobRef.cpp \
#	$(MAGIC_PATH)/lib/TypeMetric.cpp \
#	$(MAGIC_PATH)/lib/STL.cpp \
#	$(MAGIC_PATH)/lib/ImageRef.cpp \
#	$(MAGIC_PATH)/lib/Exception.cpp \
#	$(MAGIC_PATH)/lib/Montage.cpp \
#	$(MAGIC_PATH)/lib/Color.cpp \
#	$(MAGIC_PATH)/lib/CoderInfo.cpp \
#	$(MAGIC_PATH)/lib/Geometry.cpp \
#	$(MAGIC_PATH)/lib/Options.cpp


LOCAL_SRC_FILES +=\
	$(LOCAL_PATH)/kai_com_weixingif_gifsicle_GifSicleRequest.cpp \
	$(LOCAL_PATH)/tools.cpp \
	$(LOCAL_PATH)/gifsicle_wrapper.cpp \
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
# LOCAL_LDLIBS += $(LOCAL_PATH)/libimagemagick.so
#LOCAL_SHARED_LIBRARIES += ABC

LOCAL_CFLAGS := -DHAVE_CONFIG_H
ifndef NDK_ROOT
include external/stlport/libstlport.mk
endif
include $(BUILD_SHARED_LIBRARY)
