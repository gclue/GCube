LOCAL_PATH := $(call my-dir)/../../../../engine/external/math
include $(CLEAR_VARS)

LOCAL_MODULE    := math
LOCAL_SRC_FILES := matrixUtil.c \
                   vectorUtil.c \
                   math/MathExpression.cpp
LOCAL_LDLIBS    := -llog
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/

include $(BUILD_STATIC_LIBRARY)
