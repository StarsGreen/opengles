// Book:      OpenGL(R) ES 3.0 Programming Guide, 2nd Edition
// Authors:   Dan Ginsburg, Budirijanto Purnomo, Dave Shreiner, Aaftab Munshi
// ISBN-10:   0-321-93388-5
// ISBN-13:   978-0-321-93388-1
// Publisher: Addison-Wesley Professional
// URLs:      http://www.opengles-book.com
//            http://my.safaribooksonline.com/book/animation-and-3d/9780133440133

#ifndef ESUTIL_H
#define ESUTIL_H

///
//  Includes
//
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "config.h"


#if (PLATFORM==1)
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/core/core_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgcodecs/imgcodecs_c.h>
#endif

#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <semaphore.h>
#include <asm/types.h>
#include <linux/videodev2.h>
#include <time.h>
//#include <pthread.h>

///
// Types
//

typedef struct
{
   // Handle to a program object
   GLuint programObject;

   // Sampler location
   GLint samplerLoc;

   // Texture handle
   GLuint textureId;

}RenderHandler;


typedef struct ESContext ESContext;

struct ESContext
{
   /// Put platform specific data here
   void       *platformData;

   /// Window width
   GLint       width;

   /// Window height
   GLint       height;

   /// Display handle
   EGLNativeDisplayType eglNativeDisplay;

   /// Window handle
   EGLNativeWindowType  eglNativeWindow;

   /// EGL display
   EGLDisplay  eglDisplay;

   /// EGL context
   EGLContext  eglContext;

   /// EGL surface
   EGLSurface  eglSurface;

   /// EGL surface
   EGLConfig eglConfig;

   RenderHandler renderHander;
};

#define BUFFER_COUNT 4
 typedef struct BufferInfo
{
   /* data */
   void* imageBuff;
   int length;
   int showFlag;

} BufferInfo;
 typedef struct Camera
{
   /* data */
   BufferInfo bufferInfo[BUFFER_COUNT];
   char* cameraName;
   int format;
   int width;
   int height;
} Camera;



struct MediaStruct
{
   /* data */
   ESContext context;
   Camera camera;
};


extern EGLBoolean initEGL(ESContext *  esContext);
extern EGLBoolean initShader ( ESContext * esContext);
extern void draw( ESContext * esContext );
extern EGLBoolean createNativeWindow(ESContext * esContext, const char *title);
extern int initCamera(Camera* camera);
extern void * cameraCollect(int fd);
extern GLuint CreateTexture2D(ESContext * esContext,GLubyte* pixels,int width,int height);



/* Maroc defination about camera*/
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV

#define DEBUG 1

/*1 stand for using memory mmap 
  0 stand for using user pointer*/
#define VIDEO_CAPTURE_METHOD 0


#endif // ESUTIL_H


