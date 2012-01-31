LOCAL_PATH := $(call my-dir)/../../../..
JNI_PATH := $(LOCAL_PATH)/platforms/Android/jni
include $(CLEAR_VARS)

include $(JNI_PATH)/mk/openal.mk

include $(CLEAR_VARS)

include $(JNI_PATH)/mk/libpng.mk

include $(CLEAR_VARS)

include $(JNI_PATH)/mk/ogg.mk

include $(CLEAR_VARS)

include $(JNI_PATH)/mk/math.mk

include $(CLEAR_VARS)

include $(JNI_PATH)/mk/bullet.mk

include $(CLEAR_VARS)

include $(JNI_PATH)/mk/lib.mk

