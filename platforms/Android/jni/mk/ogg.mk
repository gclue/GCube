##################################################
# Ogg
##################################################
LOCAL_PATH := $(call my-dir)/../../../../engine/external/Ogg
include $(CLEAR_VARS)

LOCAL_MODULE    := ogg
LOCAL_SRC_FILES :=  libogg-1.3.0/src/bitwise.c \
                    libogg-1.3.0/src/framing.c \
                    libvorbis-1.3.2/lib/analysis.c \
                    libvorbis-1.3.2/lib/registry.c \
                    libvorbis-1.3.2/lib/vorbisenc.c \
                    libvorbis-1.3.2/lib/bitrate.c \
                    libvorbis-1.3.2/lib/block.c \
                    libvorbis-1.3.2/lib/codebook.c \
                    libvorbis-1.3.2/lib/envelope.c \
                    libvorbis-1.3.2/lib/floor0.c \
                    libvorbis-1.3.2/lib/floor1.c \
                    libvorbis-1.3.2/lib/info.c \
                    libvorbis-1.3.2/lib/lookup.c \
                    libvorbis-1.3.2/lib/lpc.c \
                    libvorbis-1.3.2/lib/lsp.c \
                    libvorbis-1.3.2/lib/mapping0.c \
                    libvorbis-1.3.2/lib/mdct.c \
                    libvorbis-1.3.2/lib/psy.c \
                    libvorbis-1.3.2/lib/res0.c \
                    libvorbis-1.3.2/lib/sharedbook.c \
                    libvorbis-1.3.2/lib/smallft.c \
                    libvorbis-1.3.2/lib/synthesis.c \
                    libvorbis-1.3.2/lib/vorbisfile.c \
                    libvorbis-1.3.2/lib/window.c 
                    
LOCAL_C_INCLUDES := $(LOCAL_PATH)/libogg-1.3.0/include \
                    $(LOCAL_PATH)/libogg-1.3.0/src \
                    $(LOCAL_PATH)/libvorbis-1.3.2/include \
                    $(LOCAL_PATH)/libvorbis-1.3.2/lib
                   
include $(BUILD_STATIC_LIBRARY)
