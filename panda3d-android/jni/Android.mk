# Copyright (C) 2010 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)


# Compiles our sample game
LOCAL_MODULE    := pandagame
LOCAL_SRC_FILES := main.cpp nativeEngine.cpp pandaGame.cpp
LOCAL_LDLIBS    := -llog -landroid -ldl -lEGL -lGLESv1_CM -L./$(LOCAL_PATH)/$(PANDA_LIB_PATH)/$(TARGET_ARCH_ABI) -lp3android
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_SHARED_LIBRARIES := libp3androiddisplay
include $(BUILD_SHARED_LIBRARY)


# We need to build the p3androiddisplay
include $(CLEAR_VARS)
LOCAL_MODULE    := libp3androiddisplay
LOCAL_CPP_EXTENSION += .cxx
LOCAL_C_INCLUDES := $(PANDA_INCLUDE_PATH)
LOCAL_CFLAGS += -DOPENGLES_1
LOCAL_SRC_FILES := androiddisplay/config_androiddisplay.cxx \
				   androiddisplay/androidGraphicsPipe.cxx \
				   androiddisplay/androidGraphicsStateGuardian.cxx \
				   androiddisplay/androidGraphicsWindow.cxx 				   
LOCAL_LDLIBS    := -llog -landroid -ldl -lEGL -lGLESv1_CM -L./$(LOCAL_PATH)/$(PANDA_LIB_PATH)/$(TARGET_ARCH_ABI) -lp3android
LOCAL_STATIC_LIBRARIES := android_native_app_glue
LOCAL_EXPORT_C_INCLUDES := $(PANDA_INCLUDE_PATH) $(LOCAL_PATH)/androiddisplay
include $(BUILD_SHARED_LIBRARY)


# We have already the p3android, so we make use of it.
include $(CLEAR_VARS)
LOCAL_MODULE := libp3android
LOCAL_SRC_FILES := $(PANDA_LIB_PATH)/$(TARGET_ARCH_ABI)/libp3android.so
LOCAL_EXPORT_C_INCLUDES := $(PANDA_INCLUDE_PATH)
#LOCAL_LDLIBS    := -llog -landroid -lEGL -lGLESv1_CM -lGLESv2


include $(PREBUILT_SHARED_LIBRARY)

# We need the native_app_glue to statically link against.
$(call import-module,android/native_app_glue)
