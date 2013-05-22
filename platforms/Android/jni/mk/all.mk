LOCAL_PATH := $(call my-dir)
MK_PATH := $(LOCAL_PATH)


include $(CLEAR_VARS)

include $(MK_PATH)/box2d.mk

include $(CLEAR_VARS)

include $(MK_PATH)/openal.mk

include $(CLEAR_VARS)

include $(MK_PATH)/libpng.mk

include $(CLEAR_VARS)

include $(MK_PATH)/ogg.mk

include $(CLEAR_VARS)

include $(MK_PATH)/math.mk

include $(CLEAR_VARS)

include $(MK_PATH)/xml.mk

include $(CLEAR_VARS)

include $(MK_PATH)/bullet.mk

include $(CLEAR_VARS)

include $(MK_PATH)/lib.mk

