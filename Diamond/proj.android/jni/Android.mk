LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp
                   
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../../ \
    $(LOCAL_PATH)/../../../CocosDenshion/include	\
	$(LOCAL_PATH)/../../Classes	\
	$(LOCAL_PATH)/../../Classes/base \
	$(LOCAL_PATH)/../../Classes/datapool \
	$(LOCAL_PATH)/../../Classes/platform \
	$(LOCAL_PATH)/../../Classes/tinyxml \
	$(LOCAL_PATH)/../../Classes/sound \
	$(LOCAL_PATH)/../../Classes/ui \
	$(LOCAL_PATH)/../../Classes/ui/layer \
	$(LOCAL_PATH)/../../Classes/ui/scene \
	$(LOCAL_PATH)/../../Classes/ui/sprite \
	$(LOCAL_PATH)/../../Classes/ui/action \

# base
include $(LOCAL_PATH)/../../Classes/base/base.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/base/, $(SOURCE))

# datapool
include $(LOCAL_PATH)/../../Classes/datapool/datapool.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/datapool/, $(SOURCE))

# sound
include $(LOCAL_PATH)/../../Classes/sound/sound.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/sound/, $(SOURCE))

# platform
include $(LOCAL_PATH)/../../Classes/platform/platform.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/platform/, $(SOURCE))

# tinyxml
include $(LOCAL_PATH)/../../Classes/tinyxml/tinyxml.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/tinyxml/, $(SOURCE))

# ui action
include $(LOCAL_PATH)/../../Classes/ui/action/action.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/ui/action/, $(SOURCE))

# ui layer
include $(LOCAL_PATH)/../../Classes/ui/layer/layer.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/ui/layer/, $(SOURCE))

# ui scene
include $(LOCAL_PATH)/../../Classes/ui/scene/scene.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/ui/scene/, $(SOURCE))

# ui sprite
include $(LOCAL_PATH)/../../Classes/ui/sprite/sprite.mk
LOCAL_SRC_FILES += $(addprefix ./../../Classes/ui/sprite/, $(SOURCE))

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static
            
include $(BUILD_SHARED_LIBRARY)

$(call import-module,CocosDenshion/android) \
$(call import-module,cocos2dx) \
$(call import-module,extensions)
