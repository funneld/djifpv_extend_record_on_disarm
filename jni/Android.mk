LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS += -fPIC -std=c99
LOCAL_LDFLAGS += -fPIC 
LOCAL_LDLIBS := -llog

LOCAL_MODULE    := extend_record_on_disarm 
LOCAL_SRC_FILES := extend_record_on_disarm.c

include $(BUILD_SHARED_LIBRARY)
