# Demo_for_Windows
### Arcsoft ArcfaceDemo for Window, VS2013 Win32 C++

### 使用步骤：
1.	下载sdk包，解压三个sdk包里lib中的文件到 Demo\lib下。
2.	在VS2013中打开Demo.sln文件，运行工程。
3.	前往官网申请appid和sdkkey, 修改 Demo\stdafx.h 下面的对应的  
	#define APPID ""  
	#define FD_SDKKEY ""  
	#define FT_SDKKEY ""  
	#define FR_SDKKEY ""  
4.	如果出现下图的错误，将Demo\lib文件夹下的全部dll文件拷贝到Release或者Debug文件夹中。
	
    ![](https://github.com/ArcJonSnow/source/raw/master/1.png)  
5.	运行程序，界面如下图。使用方法：
	1.	点击“选择图片添加到人脸库”选择bmp图片，将图片中的人脸特征信息添加到人脸库，人脸信息会显示到右侧人脸库，编辑图片下方的文字修改图片对应的名字。
	2.	点击“识别图片”按钮选择图片进行人脸识别。
	3.	点击“识别视频”按钮选择视频文件进行人脸识别。

 	![](https://github.com/ArcJonSnow/source/raw/master/2.png) 
