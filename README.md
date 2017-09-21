# Demo_for_Windows
### Arcsoft ArcfaceDemo for Windows, VS2013 C++

### 使用步骤：
1.	下载SDK包，32位Windows平台将三个SDK包里lib中的文件到 Demo\lib\Win32下；64位Windows平台将三个SDK包里lib中的文件到 Demo\lib\x64下。
2.	在VS2013中打开Demo.sln文件，运行工程，修改工程配置选择32位或64位。
3.	前往官网申请appid和sdkkey, 修改 Demo\stdafx.h 下面的对应的  
	#define APPID ""  
	#define FD_SDKKEY ""  
	#define FT_SDKKEY ""  
	#define FR_SDKKEY "" 
4.	如果出现下图的错误，32位平台将Demo\lib\Win32文件夹下的全部dll文件拷贝到Release或Debug文件夹中；64位平台将Demo\lib\x64文件夹下的全部dll文件拷贝到x64\Release或x64\Debug文件夹中。
	
    ![](https://github.com/ArcJonSnow/source/raw/master/1.png)  
5.	运行程序，界面如下图。使用方法：
	1.	点击“选择图片添加到人脸库”选择bmp图片，将图片中的人脸特征信息添加到人脸库，人脸信息会显示到右侧人脸库，编辑图片下方的文字修改图片对应的名字。
	2.	点击“识别图片”按钮选择图片进行人脸识别。
	3.	点击“识别视频”按钮选择视频文件进行人脸识别。

 	![](https://github.com/ArcJonSnow/source/raw/master/2.png) 
