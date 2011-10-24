MY_PATH := $(call my-dir)
LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/lib/math.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

include $(LOCAL_PATH)/lib/Android.mk

LOCAL_PATH := $(MY_PATH)
include $(CLEAR_VARS)

ENGINE_PATH := ../../engine

APP_MODULES     := gcube
LOCAL_MODULE    := gcube
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := android/ndk-interface.cpp \
                   android/ui-interface.cpp \
                   android/UIController.cpp \
                   renderer/Renderer.cpp \
                   renderer/TitleRenderer.cpp \
                   scene/SceneTitle.cpp \
                   scene/SceneGame.cpp \
                   scene/SceneHelp.cpp \
                   scene/Scene3D.cpp \
                   main.cpp

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
                   $(ENGINE_PATH)/lib/shader \
                   $(ENGINE_PATH)/lib/ui \
                   $(ENGINE_PATH)/lib/util \
                   $(ENGINE_PATH)/lib/xml \
                   $(ENGINE_PATH)/lib/xml/expat \
                   $(ENGINE_PATH)/external/math \
                   $(MY_PATH)/data \
                   $(MY_PATH)/renderer \
                   $(MY_PATH)/scene \
                   $(ENGINE_PATH)/util
LOCAL_STATIC_LIBRARIES := common xml math

include $(BUILD_SHARED_LIBRARY)
