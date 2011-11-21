##################################################
# Ogg
##################################################
LOCAL_PATH := $(call my-dir)/../../../../engine/external/xml/expat
include $(CLEAR_VARS)

LOCAL_MODULE    := xml
LOCAL_SRC_FILES :=  xmlparse.c \
                    xmlparse.c \
                    xmlrole.c \
                    xmltok_impl.c \
                    xmltok_ns.c \
                    xmltok.c \
                    
LOCAL_C_INCLUDES := $(LOCAL_PATH) \
                   
include $(BUILD_STATIC_LIBRARY)
