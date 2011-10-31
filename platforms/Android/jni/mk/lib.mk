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
                   scene/3D/BulletWorld.cpp \
                   scene/Scene.cpp \
                   scene/Layer2D.cpp \
                   scene/Layer3D.cpp \
                   shader/Shader.cpp \
                   shader/SimpleShader.cpp \
                   shader/PointSpriteShader.cpp \
                   shader/BoneShader.cpp \
                   sound/MusicPlayer.cpp \
                   sound/SoundEffect.cpp \
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
                   util/Animation.cpp \
                   util/AnimationSet.cpp \
                   util/AssetManager.cpp \
                   util/Easing.cpp \
                   util/FigureCache.cpp \
                   util/Storage.cpp \
                   xml/expat/xmlparse.c \
                   xml/expat/xmlrole.c \
                   xml/expat/xmltok_impl.c \
                   xml/expat/xmltok_ns.c \
                   xml/expat/xmltok.c \
                   xml/XMLParser.cpp \
                   yspng/yspng.cpp
LOCAL_LDLIBS    := -llog -lGLESv2
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/ \
                   $(LOCAL_PATH)/.. \
                   $(LOCAL_PATH)/controller \
                   $(LOCAL_PATH)/gl \
                   $(LOCAL_PATH)/math \
                   $(LOCAL_PATH)/porting \
                   $(LOCAL_PATH)/scene \
                   $(LOCAL_PATH)/scene/3D \
                   $(LOCAL_PATH)/shader \
                   $(LOCAL_PATH)/sound \
                   $(LOCAL_PATH)/ui \
                   $(LOCAL_PATH)/util \
                   $(LOCAL_PATH)/xml \
                   $(LOCAL_PATH)/xml/expat \
                   $(LOCAL_PATH)/../external/math \
                   $(LOCAL_PATH)/../external/Bullet \
                   $(LOCAL_PATH)/../external/Bullet/BulletDynamics/Dynamics \
				   $(LOCAL_PATH)/../external/Bullet/Extras/Serialize/BulletWorldImporter  \
				   $(LOCAL_PATH)/../external/Bullet/Extras/Serialize/BulletFileLoader \
                   $(LOCAL_PATH)/yspng
LOCAL_STATIC_LIBRARIES := math bullet

include $(BUILD_STATIC_LIBRARY)
