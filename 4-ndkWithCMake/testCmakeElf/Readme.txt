安装了Android Studio
C:\Users\JL7GPC3\AppData\Local\Android\Sdk\ 目录下面有cmake和ndk

在系统变量中添加ANDROID_SDK_HOME_CMD = C:\Users\JL7GPC3\AppData\Local\Android\Sdk
在系统环境变量path中添加C:\Users\JL7GPC3\AppData\Local\Android\Sdk\cmake\3.22.1\bin，方便使用cmake和ninja

使用buildRunOnAndroidMake.bat和buildRunOnAndroidMake.bat的差別：

  -DCMAKE_GENERATOR="Ninja" ^
  -DCMAKE_MAKE_PROGRAM=%ANDROID_SDK_HOME_CMD%/cmake/3.22.1/bin/ninja.exe ^
  ..

ninja

  -DCMAKE_GENERATOR="Unix Makefiles" ^
  -DCMAKE_MAKE_PROGRAM=%ANDROID_SDK_HOME_CMD%/ndk/23.1.7779620/prebuilt/windows-x86_64/bin/make.exe ^
  ..

%ANDROID_SDK_HOME_CMD%/ndk/23.1.7779620/prebuilt/windows-x86_64/bin/make



