LOCAL_PATH := $(call my-dir)

EMULAOTR_SRC_PATH := src
EMULAOTR_HEADER_PATH := $(LOCAL_PATH)/src \
						$(LOCAL_PATH)/src/emulator

########################################################################
include $(CLEAR_VARS)

LOCAL_MODULE    := emulator
LOCAL_SRC_FILES := $(EMULAOTR_SRC_PATH)/main.cpp \
					$(EMULAOTR_SRC_PATH)/emulator/Emulator.cpp
					
LOCAL_C_INCLUDES := $(EMULAOTR_HEADER_PATH)

###-DPS_EMU DFC_EMU定义不同的宏，可以调用不同核心的函数
LOCAL_CFLAGS += -DPS_EMU

LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)

########################################################################
# ps 
include $(CLEAR_VARS)

LOCAL_MODULE    := ps
LOCAL_SRC_FILES := $(EMULAOTR_SRC_PATH)/core/ps/ps_core.cpp

LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)

########################################################################
# fc 
include $(CLEAR_VARS)

LOCAL_MODULE    := fc
LOCAL_SRC_FILES := $(EMULAOTR_SRC_PATH)/core/fc/fc_core.cpp

LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)

########################################################################
# nds 
include $(CLEAR_VARS)

LOCAL_MODULE    := nds
LOCAL_SRC_FILES := $(EMULAOTR_SRC_PATH)/core/nds/nds_core.cpp

LOCAL_LDLIBS := -ldl -llog

include $(BUILD_SHARED_LIBRARY)

########################################################################
