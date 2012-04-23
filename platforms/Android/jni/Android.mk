MY_PATH := $(call my-dir)
include $(CLEAR_VARS)

include $(MY_PATH)/mk/all.mk

LOCAL_PATH := $(MY_PATH)/../../..
include $(CLEAR_VARS)

MAIN_PATH := main
ENGINE_PATH := $(LOCAL_PATH)/engine
INC_PATH := $(LOCAL_PATH)/main


APP_MODULES     := gcube
LOCAL_MODULE    := gcube
LOCAL_CFLAGS    := -Werror
LOCAL_SRC_FILES := platforms/Android/jni/android/ndk-interface.cpp \
                   platforms/Android/jni/android/ui-interface.cpp \
                   platforms/Android/jni/android/UIController.cpp \
                   $(MAIN_PATH)/main.cpp \
                   $(MAIN_PATH)/SceneTitle.cpp \
                   $(MAIN_PATH)/SceneSample1.cpp \
                   $(MAIN_PATH)/SceneTwitterTest.cpp \
                   $(MAIN_PATH)/SceneGameCenterSample.cpp \
#                   $(MAIN_PATH)/Scene3D.cpp
                   
LOCAL_LDLIBS    := -llog -lGLESv2 -lz
LOCAL_C_INCLUDES:= $(MY_PATH)/ \
                   $(MY_PATH)/android/ \
                   $(ENGINE_PATH)/lib \
                   $(ENGINE_PATH)/lib/porting \
                   $(ENGINE_PATH)/lib/controller \
                   $(ENGINE_PATH)/lib/gl \
                   $(ENGINE_PATH)/lib/gl/collada \
                   $(ENGINE_PATH)/lib/gl/loader \
                   $(ENGINE_PATH)/lib/math \
                   $(ENGINE_PATH)/lib/scene \
                   $(ENGINE_PATH)/lib/sound \
                   $(ENGINE_PATH)/lib/scene/3D \
                   $(ENGINE_PATH)/lib/shader \
                   $(ENGINE_PATH)/lib/ui \
                   $(ENGINE_PATH)/lib/util \
                   $(ENGINE_PATH)/lib/xml \
                   $(ENGINE_PATH)/external \
                   $(ENGINE_PATH)/external/lpng1246 \
                   $(ENGINE_PATH)/external/xml \
                   $(ENGINE_PATH)/external/xml/expat \
                   $(ENGINE_PATH)/external/math \
                   $(ENGINE_PATH)/external/math/math \
                   $(ENGINE_PATH)/external/SmartPtr \
                   $(ENGINE_PATH)/external/OpenAL/ \
                   $(ENGINE_PATH)/external/OpenAL/include \
                   $(ENGINE_PATH)/external/OpenAL/OpenAL32/Include \
                   $(ENGINE_PATH)/external/Bullet \
                   $(ENGINE_PATH)/external/Box2D \
                   $(ENGINE_PATH)/external/Box2D/Collision \
                   $(ENGINE_PATH)/external/Box2D/Dynamics \
                   $(ENGINE_PATH)/external/Box2D/Common \
                   $(ENGINE_PATH)/external/Box2D/Rope \
				   $(ENGINE_PATH)/external/Bullet/Extras/Serialize/BulletWorldImporter  \
				   $(ENGINE_PATH)/external/Bullet/Extras/Serialize/BulletFileLoader \
                   $(ENGINE_PATH)/external/Ogg/libogg-1.3.0/include \
                   $(ENGINE_PATH)/external/Ogg/libogg-1.3.0/src \
                   $(ENGINE_PATH)/external/Ogg/libvorbis-1.3.2/include \
                   $(ENGINE_PATH)/external/Ogg/libvorbis-1.3.2/lib \
                   $(INC_PATH)
LOCAL_STATIC_LIBRARIES := common box2d xml math bullet ogg openal png

include $(BUILD_SHARED_LIBRARY)
