
/*******************************************************************************
#             uvccapture: USB UVC Video Class Snapshot Software                #
#This package work with the Logitech UVC based webcams with the mjpeg feature  #
#                                                                              #
#       Orginally Copyright (C) 2005 2006 Laurent Pinchart &&  Michel Xhaard   #
#       Modifications Copyright (C) 2006 Gabriel A. Devenyi                    #
#                                                                              #
# This program is free software; you can redistribute it and/or modify         #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation; either version 2 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# This program is distributed in the hope that it will be useful,              #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with this program; if not, write to the Free Software                  #
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA    #
#                                                                              #
*******************************************************************************/

#define NB_BUFFER 16
#define DHT_SIZE 420

#ifndef V4L2_CID_SHARPNESS
#define V4L2_CID_SHARPNESS			(V4L2_CID_BASE+27)
#endif

struct vdIn {
  int fd;
  char *videodevice;
  char *status;
  char *pictName;
  struct v4l2_capability cap;
  struct v4l2_format fmt;
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers rb;
  void *mem[NB_BUFFER];
  unsigned char *tmpbuffer;
  unsigned char *framebuffer;
  int isstreaming;
  int grabmethod;
  int width;
  int height;
  int formatIn;
  int formatOut;
  int framesizeIn;
  int signalquit;
  int toggleAvi;
  int getPict;

};

int
  init_videoIn (struct vdIn *vd, char *device, int width, int height,
		int format, int grabmethod);
int uvcGrab (struct vdIn *vd);
int close_v4l2 (struct vdIn *vd);

int v4l2GetControl (int fd, int control);
int v4l2SetControl (int fd, int control, int value);
int v4l2UpControl (int fd, int control);
int v4l2DownControl (int fd, int control);
int v4l2ToggleControl (int fd, int control);
int v4l2ResetControl (int fd, int control);
