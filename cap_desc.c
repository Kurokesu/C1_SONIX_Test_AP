#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include "debug.h"
#include "sonix_xu_ctrls.h"
#include "cap_desc.h"

int GetCapability(int fd, struct CapabiltyBinaryData *pCapData)
{
    unsigned int SensorInitTableAddr, CapAddr, CapSize, Section11_addr;
    unsigned char *pBuffer;
    // get SensorInitTableAddr
    pBuffer = malloc(4 * sizeof(unsigned char));
    if(XU_SF_Read(fd, 0x017f, pBuffer,4)<0)
    {
        TestAp_Printf(TESTAP_DBG_ERR, "%s:read SF error\n",__FUNCTION__);
        return -1;
    }
    SensorInitTableAddr = (pBuffer[0]<<24) | (pBuffer[1]<<16) | (pBuffer[2]<<8) | pBuffer[3];
    TestAp_Printf(TESTAP_DBG_BW, "%s: SensorInitTableAddr = 0x%x\n",__FUNCTION__, SensorInitTableAddr);
    free(pBuffer);

    // get CapAddr, CapSize
    pBuffer = malloc(25 * sizeof(unsigned char));   // 1 + 2 * (10 + 1) + 2
    if(XU_SF_Read(fd, SensorInitTableAddr, pBuffer,25)<0)
    {
        TestAp_Printf(TESTAP_DBG_ERR, "%s:read SF error\n",__FUNCTION__);
        return -1;
    }
    CapAddr = SensorInitTableAddr + ((pBuffer[21]<<8) | pBuffer[22]);
    Section11_addr  = SensorInitTableAddr + ((pBuffer[23]<<8) | pBuffer[24]);
    CapSize = Section11_addr - CapAddr; 
    TestAp_Printf(TESTAP_DBG_BW, "%s: CapAddr = 0x%x, CapSize = 0x%x\n",__FUNCTION__, CapAddr, CapSize);
    free(pBuffer);
    
    //get capability
    pCapData->pbuf= malloc(CapSize * sizeof(unsigned char)); 
    pCapData->Lenght= CapSize;
    
    if(XU_SF_Read(fd, CapAddr, pCapData->pbuf,CapSize)<0)
    {
        TestAp_Printf(TESTAP_DBG_ERR, "%s:read SF error\n",__FUNCTION__);
        return -1;
    }
    return 0;
}

int GetInterface(int fd,struct InterfaceDesc *Interface_Desc)
{
        int ret, i, j, k;
        struct v4l2_fmtdesc fmt;
        struct v4l2_frmsizeenum fsize;
        struct v4l2_frmivalenum fival;

        memset(Interface_Desc, 0 , sizeof(struct InterfaceDesc));

        //get format
        memset(&fmt, 0, sizeof(fmt));
        fmt.index = 0;
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        while ((ret = ioctl(fd, VIDIOC_ENUM_FMT, &fmt)) == 0) {
            Interface_Desc->NumFormat ++;
            Interface_Desc->fmt[fmt.index] = fmt.pixelformat; 
            fmt.index++;
#if 0
            TestAp_Printf(TESTAP_DBG_BW,"{ pixelformat = '%c%c%c%c', '%lx', description = '%s' }\n",
                    fmt.pixelformat & 0xFF, (fmt.pixelformat >> 8) & 0xFF,
                    (fmt.pixelformat >> 16) & 0xFF, (fmt.pixelformat >> 24) & 0xFF, fmt.pixelformat,
                    fmt.description);
#endif
        }

        //get frame size
        memset(&fsize, 0, sizeof(fsize));
        for(i = 0; i < Interface_Desc->NumFormat;i++)
        {
            fsize.index = 0;
            fsize.pixel_format = Interface_Desc->fmt[i];
            while ((ret = ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &fsize)) == 0) 
            {
                if (fsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) 
                { 
                    Interface_Desc->NumFrame[i]++;
                    Interface_Desc->frame_info[i][fsize.index].width = fsize.discrete.width;
                    Interface_Desc->frame_info[i][fsize.index].height= fsize.discrete.height;
                }
                fsize.index++;
            }
        }

        //get frame Interval(fps)
        memset(&fival, 0, sizeof(fival));
        for(i = 0; i < Interface_Desc->NumFormat;i++)
        {
            for(j = 0; j < Interface_Desc->NumFrame[i];j++)
            {
                fival.index = 0;
                fival.pixel_format = Interface_Desc->fmt[i];
                fival.width = Interface_Desc->frame_info[i][j].width;
                fival.height = Interface_Desc->frame_info[i][j].height;
                //Interface_Desc->frame_info[i][j].NumFPS = 0;
                while ((ret = ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &fival)) == 0) 
                {
		            if (fival.type == V4L2_FRMIVAL_TYPE_DISCRETE) 
                    {      
                        Interface_Desc->frame_info[i][j].NumFPS++;
                        Interface_Desc->frame_info[i][j].FPS[fival.index] = (unsigned char)fival.discrete.denominator;
                    }
                    fival.index ++;
                }
            }
        }


#if 0
    TestAp_Printf(TESTAP_DBG_BW, "INTERFACE:\n");
    TestAp_Printf(TESTAP_DBG_BW, "format num = %d\n", Interface_Desc->NumFormat);
    for(i = 0; i < Interface_Desc->NumFormat;i++)
    {
        TestAp_Printf(TESTAP_DBG_BW, "\tformat[%d] = %x\n", i, Interface_Desc->fmt[i]);
        for(j = 0; j < Interface_Desc->NumFrame[i];j++)
        {
            TestAp_Printf(TESTAP_DBG_BW, "\t\tframe[%d]: size = %d x %d \n", j, Interface_Desc->frame_info[i][j].width, Interface_Desc->frame_info[i][j].height);
            for(k = 0; k<Interface_Desc->frame_info[i][j].NumFPS; k++)
                TestAp_Printf(TESTAP_DBG_BW, "\t\t\tfps[%d]: fps = %d\n", k,  Interface_Desc->frame_info[i][j].FPS[k]);

        }

    }
#endif

}


