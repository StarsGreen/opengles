#include "include.h"

Camera camera;

/////////////////////////////////////////////

//////////////////////////////////////////////
int initCamera(Camera* camera)
{
	int i=0, ret;
	int fd=-1;
	#if DEBUG
	printf("start init camera success\n");
	#endif	
	
	fd = open(camera->cameraName, O_RDWR, 0);
	if (fd < 0) {
		printf("Open %s failed\n", camera->cameraName);
		return -1;
	}
	#if DEBUG
	printf("open camera success\n");
	#endif
	struct v4l2_capability cap;
	ret = ioctl(fd, VIDIOC_QUERYCAP, &cap);
	if (ret < 0) {
		printf("VIDIOC_QUERYCAP failed (%d)\n", ret);
        return -1;

	}
	#if DEBUG
	printf("query camera success\n");
	#endif
	struct v4l2_format fmt;
	memset(&fmt, 0, sizeof(fmt));
	fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.width = camera->width;
	fmt.fmt.pix.height = camera->height;
	fmt.fmt.pix.pixelformat =  camera->format;
	fmt.fmt.pix.field = V4L2_FIELD_INTERLACED;
	ret = ioctl(fd, VIDIOC_S_FMT, &fmt);
	if (ret < 0) {
		printf("VIDIOC_S_FMT failed (%d)", ret);
        return -1;
	}

	// ret = ioctl(fd, VIDIOC_G_FMT, &fmt);
	// if (ret < 0) {
	// 	printf("VIDIOC_G_FMT failed (%d)\n", ret);
    //     return -1;

	// }
	
	#if DEBUG
	printf("set camera format success\n");
	#endif
	struct v4l2_requestbuffers reqbuf;
	bzero(&reqbuf,sizeof(reqbuf));
	reqbuf.count = BUFFER_COUNT;
	reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	#if VIDEO_CAPTURE_METHOD
	reqbuf.memory = V4L2_MEMORY_MMAP;
    #else
	reqbuf.memory = V4L2_MEMORY_USERPTR;
	#endif
	ret = ioctl(fd , VIDIOC_REQBUFS, &reqbuf);
	if(ret < 0) {
		printf("VIDIOC_REQBUFS failed (%d)\n", ret);
        return -1;

	}
	#if DEBUG
	printf("query camera buffer success\n");
	#endif
	//VideoBuffer*  buffers = calloc( reqbuf.count, sizeof(*buffers) );
	struct v4l2_buffer buf;
	int length[BUFFER_COUNT];
	void *start[BUFFER_COUNT];

	for (i = 0; i < BUFFER_COUNT; i++)
	{
	  bzero(&buf,sizeof(buf));
      buf.index = i;
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	  #if VIDEO_CAPTURE_METHOD
      buf.memory = V4L2_MEMORY_MMAP;
	  #else
      buf.memory = V4L2_MEMORY_USERPTR;
	  #endif
      ret = ioctl(fd , VIDIOC_QUERYBUF, &buf);
      if(ret < 0) {
		printf("VIDIOC_QUERYBUF (%d) failed (%d)\n", i, ret);
        return -1;
	    }
	  #if VIDEO_CAPTURE_METHOD
	    start[i]=(unsigned char *) mmap(0, buf.length,
        PROT_READ|PROT_WRITE, MAP_SHARED,
        fd, buf.m.offset);
		length[i]=buf.length;
	  #else
	  	start[i]=calloc(1,buf.length);
	    length[i]=buf.length;
        buf.m.userptr=(unsigned long)start[i];
	  #endif
      camera->bufferInfo[i].imageBuff=start[i];
      camera->bufferInfo[i].length=length[i];
        // Queen buffer
      ret = ioctl(fd , VIDIOC_QBUF, &buf);
      if (ret < 0) {
        printf("VIDIOC_QBUF (%d) failed (%d)\n", i, ret);
        return -1;
      }
	}
	#if 1
	#if DEBUG
	printf("alloc camera buffer success\n");
	#endif
	enum v4l2_buf_type vtype = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ret = ioctl(fd, VIDIOC_STREAMON, &vtype);
	if (ret < 0) {
		printf("VIDEOC_STREAMON failed (%d)\n", ret);
        return -1;
	}
	#endif
    struct v4l2_buffer v4l2buf;
	bzero(&v4l2buf,sizeof(v4l2buf));
	v4l2buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	#if VIDEO_CAPTURE_METHOD
	v4l2buf.memory= V4L2_MEMORY_MMAP;
	#else
	v4l2buf.memory= V4L2_MEMORY_USERPTR;
	#endif
	i=0;
  	#if DEBUG
	printf("start camera collection\n");
	#endif
    while(i<BUFFER_COUNT)
    {
	  v4l2buf.index=i%BUFFER_COUNT;
	  #if DEBUG
	  printf("dequeue camera collection\n");
	  #endif
	  ret = ioctl(fd, VIDIOC_DQBUF, &v4l2buf);
	  if (ret < 0) {
		printf("VIDIOC_DQBUF failed (%d)\n", ret);
	  }
	  #if DEBUG
	  printf("queue camera collection\n");
	  #endif
	// Re-queen buffer
	  ret = ioctl(fd, VIDIOC_QBUF, &v4l2buf);
	  if (ret < 0) {
		printf("VIDIOC_QBUF failed (%d)\n", ret);
	  }
      i++;
   }
  	#if DEBUG
	printf("stop camera collection\n");
	#endif
    return fd;
}

void * cameraCollect(int fd)
{
	/*
    int ret;
    struct v4l2_buffer v4l2buf;
	bzero(&v4l2buf,sizeof(v4l2buf));
	v4l2buf.type= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	#if VIDEO_CAPTURE_METHOD
	v4l2buf.memory= V4L2_MEMORY_MMAP;
	#else
	v4l2buf.memory= V4L2_MEMORY_USERPTR;
	#endif
	int i=0;
  	#if DEBUG
	printf("start camera collection\n");
	#endif
    while(i<BUFFER_COUNT)
    {
	  v4l2buf.index=i%BUFFER_COUNT;
	  #if DEBUG
	  printf("dequeue camera collection\n");
	  #endif
	  ret = ioctl(fd, VIDIOC_DQBUF, &v4l2buf);
	  if (ret < 0) {
		printf("VIDIOC_DQBUF failed (%d)\n", ret);
	  }
	  #if DEBUG
	  printf("queue camera collection\n");
	  #endif
	// Re-queen buffer
	  ret = ioctl(fd, VIDIOC_QBUF, &v4l2buf);
	  if (ret < 0) {
		printf("VIDIOC_QBUF failed (%d)\n", ret);
	  }
      i++;
   }
  	#if DEBUG
	printf("stop camera collection\n");
	#endif
	*/
}

