# ImageWizard
Qt C++ Application that allows user to load and modify JPEG and PNG images. The image can then be converted to a YUV file and back by using FFMPEG. Saturation of the YUV file can also be changed.

For the Image Wizard to function as intended, FFMPEG must be installed (https://www.ffmpeg.org/download.html).
To build and run this Qt application, using Qt Creator would be easiest. Otherwise running qmake on the project file will work too.

My Build Steps are:
qmake.exe ImageWizard.pro
mingw32-make.exe -j8 in X:\Qt Projects\build-ImageWizard-Desktop_Qt_6_1_2_MinGW_64_bit-Debug
The directory above is my own, other users can choose another.

Regarding use of FFMPEG
I hope this was an acceptable approach. I originally tried to use the libraries behind FFMPEG, but their configure script (which generates an essential config.h) was not working properly on my system. In the end I resorted to installing FFMPEG, as it seemed the only way I'd be able to work with YUV files in time to submit this.

Regarding YUV File
I did not develop a way to view the YUV file format.

Steps to Use:
Load a JPEG or PNG either by dragging and dropping into app, hitting Ctrl+O, or using dropdown menu
Under Edit dropdown, user can modify the RGB image
Or user can convert the current loaded image into a YUV image (this is done in the application directory)
Then the user can edit the YUV's saturation, or convert back to RGB. The new RGB image will be loaded automatically
Finally, user can save the image as a JPEG or PNG
