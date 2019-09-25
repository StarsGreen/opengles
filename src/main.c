
#include "include.h"

//pthread_t camera_thread;
int main(int argc, char ** argv)
{
        ESContext esContext;
        Camera camera;
        memset ( &esContext, 0, sizeof( esContext ) );
        memset ( &camera, 0, sizeof( camera ) );

        esContext.height=720;
        esContext.width=1280;
        #if DEBUG
        printf("hello camera\n");
        #endif
        
        if(!initEGL(&esContext))
        {
            printf("initEGL error\n");
            return EGL_FALSE;
        }
      if(!initShader(&esContext))
        {
            printf("initShader error\n");
            return EGL_FALSE;
        }
      camera.cameraName="/dev/video12";
      camera.format=V4L2_PIX_FMT_YUYV;

      #if(PLATFORM==1)
      IplImage* img=cvLoadImage("test0.jpg",CV_LOAD_IMAGE_COLOR);
      //cvShowImage("test",img);
      unsigned char* pixes= img->imageData;
      camera.height=734;
      camera.width=1024;
      #else
       unsigned char pixes[12]={
      255,   0,   0, // Red
        0, 255,   0, // Green
        0,   0, 255, // Blue
      255, 255,   0  // Yellow
       };
      camera.height=2;
      camera.width=2;
      #endif


      camera.bufferInfo[0].imageBuff=pixes;
      #if 0
      int fd =initCamera(&camera);
      if(fd<0)
      {
        printf("init camera error");
        exit;
      }


      //pthread_create(&camera_thread,NULL,(void*)cameraCollect,&fd);

      cameraCollect(fd);

      FILE* file;
      if((file=fopen("image.yuyv","wb"))==NULL)
        {
          printf("file open error\n");
          exit;
        }
      fwrite((void*)camera.bufferInfo[0].imageBuff,camera.bufferInfo[0].length,1,file);
      fclose(file);
    #endif
    #if 1
      CreateTexture2D(& esContext,camera.bufferInfo[0].imageBuff,camera.width,camera.height);
      draw(&esContext);

      eglSwapBuffers(esContext.eglDisplay,esContext.eglSurface);
    #endif
      #if(PLATFORM==1)
      cvWaitKey(0);
      #endif
      while (1);
      //sleep(10);
      return EGL_TRUE;
}