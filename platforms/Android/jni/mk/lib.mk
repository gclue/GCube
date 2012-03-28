LOCAL_PATH := $(call my-dir)/../../../../engine/lib
include $(CLEAR_VARS)

LOCAL_MODULE    := common
LOCAL_SRC_FILES := math/Vector2D.cpp \
                   math/Vector3D.cpp \
                   math/Matrix3D.cpp \
                   math/DampedOscillator.cpp \
                   math/Oscillator.cpp \
                   gl/Camera.cpp \
                   gl/Figure.cpp \
                   gl/Texture.cpp \
                   gl/SharedTexture.cpp \
                   gl/PackerTexture.cpp \
                   gl/PrimitiveObjectBuilder.cpp \
                   gl/Joint.cpp \
                   gl/Light.cpp \
                   gl/collada/Accessor.cpp \
                   gl/collada/Collada.cpp \
                   gl/collada/ColladaDataObject.cpp \
                   gl/collada/ColladaLoader.cpp \
                   gl/collada/FloatArray.cpp \
                   gl/collada/Geometry.cpp \
                   gl/collada/GeometryLibrary.cpp \
                   gl/collada/Image.cpp \
                   gl/collada/ImagesLibrary.cpp \
                   gl/collada/InitFrom.cpp \
                   gl/collada/Input.cpp \
                   gl/collada/Mesh.cpp \
                   gl/collada/NameArray.cpp \
                   gl/collada/P.cpp \
                   gl/collada/Param.cpp \
                   gl/collada/Source.cpp \
                   gl/collada/TechniqueCommon.cpp \
                   gl/collada/Triangles.cpp \
                   gl/collada/Vertices.cpp \
                   gl/loader/WFObjLoader.cpp \
                   controller/GCContext.cpp \
                   controller/ApplicationController.cpp \
                   controller/TextureManager.cpp \
                   scene/Scene.cpp \
                   scene/Layer2D.cpp \
                   shader/Shader.cpp \
                   shader/SimpleShader.cpp \
                   shader/PointSpriteShader.cpp \
                   shader/BoneShader.cpp \
                   sound/MusicPlayer.cpp \
                   sound/OpenALPlayer.cpp \
                   sound/SoundEffect.cpp \
                   sound/SoundPlayer.cpp \
                   sound/OggLoader.cpp \
                   ui/View.cpp \
                   ui/ViewGroup.cpp \
                   ui/Button.cpp \
                   ui/ImageButton.cpp \
                   ui/ImageAnimationView.cpp \
                   ui/ImageView.cpp \
                   ui/GridView.cpp \
                   ui/ScrollView.cpp \
                   ui/RadioButton.cpp \
                   ui/RadioGroup.cpp \
                   ui/NumberView.cpp \
                   ui/WebView.cpp \
                   util/Animation.cpp \
                   util/AnimationSet.cpp \
                   util/AssetManager.cpp \
                   util/Easing.cpp \
                   util/PngData.cpp \
                   util/FigureCache.cpp \
                   util/HttpClient.cpp \
                   util/Storage.cpp \
                   xml/XMLParser.cpp 
#                   util/BulletWorld.cpp \
#                   scene/Layer3D.cpp 
                   
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(LOCAL_PATH)/.. \
                   $(LOCAL_PATH)/controller \
                   $(LOCAL_PATH)/gl \
                   $(LOCAL_PATH)/math \
                   $(LOCAL_PATH)/porting \
                   $(LOCAL_PATH)/scene \
                   $(LOCAL_PATH)/shader \
                   $(LOCAL_PATH)/sound \
                   $(LOCAL_PATH)/ui \
                   $(LOCAL_PATH)/util \
                   $(LOCAL_PATH)/xml \
                   $(LOCAL_PATH)/../external/lpng1246 \
                   $(LOCAL_PATH)/../external/xml \
                   $(LOCAL_PATH)/../external/xml/expat \
                   $(LOCAL_PATH)/../external/math \
                   $(LOCAL_PATH)/../external/SmartPtr \
                   $(LOCAL_PATH)/../external/Bullet \
                   $(LOCAL_PATH)/../external/OpenAL \
                   $(LOCAL_PATH)/../external/OpenAL/Alc \
                   $(LOCAL_PATH)/../external/OpenAL/include \
                   $(LOCAL_PATH)/../external/OpenAL/OpanAL32/include \
                   $(LOCAL_PATH)/../external/Ogg/libogg-1.3.0/include \
                   $(LOCAL_PATH)/../external/Ogg/libogg-1.3.0/src \
                   $(LOCAL_PATH)/../external/Ogg/libvorbis-1.3.2/include \
                   $(LOCAL_PATH)/../external/Ogg/libvorbis-1.3.2/lib \
                   $(LOCAL_PATH)/../external/Bullet/BulletDynamics/Dynamics \
				   $(LOCAL_PATH)/../external/Bullet/Extras/Serialize/BulletWorldImporter  \
				   $(LOCAL_PATH)/../external/Bullet/Extras/Serialize/BulletFileLoader \
				   $(LOCAL_PATH)/../../main/ \
LOCAL_STATIC_LIBRARIES := math bullet xml png

include $(BUILD_STATIC_LIBRARY)
