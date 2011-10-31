MY_PATH := $(call my-dir)
LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/mk/math.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/mk/bullet.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/mk/lib.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

ENGINE_PATH := ../../engine
MAIN_PATH := ../../../main

APP_MODULES     := gcube
LOCAL_MODULE    := gcube
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := android/ndk-interface.cpp \
                   android/ui-interface.cpp \
                   android/UIController.cpp \
                   $(MAIN_PATH)/SceneTitle.cpp \
                   $(MAIN_PATH)/Scene3D.cpp \
                   $(MAIN_PATH)/main.cpp

LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_C_INCLUDES:= $(MY_PATH) \
                   $(MY_PATH)/android \
                   $(ENGINE_PATH)/lib \
                   $(ENGINE_PATH)/lib/porting \
                   $(ENGINE_PATH)/lib/controller \
                   $(ENGINE_PATH)/lib/gl \
                   $(ENGINE_PATH)/lib/gl/collada \
                   $(ENGINE_PATH)/lib/gl/loader \
                   $(ENGINE_PATH)/lib/math \
                   $(ENGINE_PATH)/lib/scene \
                   $(ENGINE_PATH)/lib/scene/3D \
                   $(ENGINE_PATH)/lib/shader \
                   $(ENGINE_PATH)/lib/ui \
                   $(ENGINE_PATH)/lib/util \
                   $(ENGINE_PATH)/lib/xml \
                   $(ENGINE_PATH)/lib/xml/expat \
                   $(ENGINE_PATH)/external/math \
                   $(ENGINE_PATH)/external/Bullet \
				   $(ENGINE_PATH)/external/Bullet/Extras/Serialize/BulletWorldImporter  \
				   $(ENGINE_PATH)/external/Bullet/Extras/Serialize/BulletFileLoader \
                   $(MAIN_PATH) \
                   $(ENGINE_PATH)/util
LOCAL_STATIC_LIBRARIES := common math bullet

include $(BUILD_SHARED_LIBRARY)
