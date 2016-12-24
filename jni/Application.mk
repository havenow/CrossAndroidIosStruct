APP_ABI := armeabi-v7a
APP_PLATFORM := android-9

ifeq ($(NDK_DEBUG), 1)
APP_CPPFLAGS :=  -std=c++11 
APP_OPTIM := debug
else
APP_CPPFLAGS :=  -std=c++11 
APP_OPTIM := release
endif

APP_LDFLAGS := -latomic
APP_STL := gnustl_static