#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cap_desc_parser.h"

//=======================================================================
//                                  start of capability parser
//=======================================================================


int ParseMultiStreamConfig(unsigned char *pConfig,  struct MultiStreamCfg *Cfg_Desc)
{
    int i;
    unsigned char *pMultistreamCap;
    Cfg_Desc->NumStreams = pConfig[2];

    //parse multistream capability descriptor
    Cfg_Desc->MS_Cap = malloc(Cfg_Desc->NumStreams * sizeof(struct MultiStreamCap));
     pMultistreamCap = pConfig + pConfig[0];
     for(i=0; i<Cfg_Desc->NumStreams; i++)
     {
         memcpy((unsigned char *)&Cfg_Desc->MS_Cap[i], (unsigned char *)pMultistreamCap+2 ,12);
         pMultistreamCap += pMultistreamCap[0];
     }
 
 }
 
 int ParseCapability(unsigned char *pData, int Length, struct CapabilityDescriptor *Cap_Desc)
 {
    
    int i, j;
    int ConfigLength;
    unsigned char *pConfigData, *pDemuxerData, *pFrameIntervalData, *pBitrateData;
    //check capability  header
    if(pData[0] != MSC_HEADER_LENGTH || pData[1] != MSC_HEADER)
    {
        //printf("%s:Not Capability data \n",__FUNCTION__);
        return -1;
    }
    //parse header
    Cap_Desc->NumConfigs = pData[4];

    //parse configuration descriptor
    Cap_Desc->Cfg_Desc = malloc(Cap_Desc->NumConfigs * sizeof(struct MultiStreamCfg));
    pConfigData = pData + pData[0];
    ConfigLength = pConfigData[0];
    for(i=0; i<Cap_Desc->NumConfigs; i++)
    {
        ParseMultiStreamConfig(pConfigData,  &Cap_Desc->Cfg_Desc[i]);
        pConfigData += (ConfigLength + Cap_Desc->Cfg_Desc[i].NumStreams * pConfigData[ConfigLength]);
    
    }

    //calculate number of demuxer descriptor
    pDemuxerData = pConfigData;
    i = 0;
    while(pDemuxerData[1]==MSC_DEMUXER)
    {
        pDemuxerData += pDemuxerData[0];
        i++;
    }
    Cap_Desc->NumDemuxers = i;    

    //parse demuxer descriptor
    pDemuxerData = pConfigData;
    Cap_Desc->demuxer_Desc = malloc(Cap_Desc->NumDemuxers * sizeof(struct MultiStreamDemuxer));
    for(i = 0; i < Cap_Desc->NumDemuxers; i++)
    {
        Cap_Desc->demuxer_Desc[i].MSCDemuxIndex = pDemuxerData[2];
        Cap_Desc->demuxer_Desc[i].DemuxID= pDemuxerData[3];
        Cap_Desc->demuxer_Desc[i].Width = (pDemuxerData[4]<<8) | pDemuxerData[5];
        Cap_Desc->demuxer_Desc[i].Height= (pDemuxerData[6]<<8) | pDemuxerData[7];
        pDemuxerData += pDemuxerData[0];
    }

    //calculat number of  frameinterval descriptor    
    pFrameIntervalData = pDemuxerData;
    i = 0;
    while(pFrameIntervalData[1] == MSC_FRAMEINTERVAL)
    {
        pFrameIntervalData += pFrameIntervalData[0];    
        i++;
    }
    Cap_Desc->NumFrameIntervals = i;

    //parse frameinterval descriptor   
    pFrameIntervalData = pDemuxerData;
    Cap_Desc->FrameInt_Desc = malloc(Cap_Desc->NumFrameIntervals * sizeof(struct MultiStreamFrameInterval));
    for(i = 0; i < Cap_Desc->NumFrameIntervals; i++)
    {
        unsigned int FrameInternal, fps;
        Cap_Desc->FrameInt_Desc[i].FPSIndex = pFrameIntervalData[2];
        Cap_Desc->FrameInt_Desc[i].FPSCount = pFrameIntervalData[3];
        Cap_Desc->FrameInt_Desc[i].FPS = malloc(Cap_Desc->FrameInt_Desc[i].FPSCount * sizeof(unsigned char));
        for(j = 0; j < Cap_Desc->FrameInt_Desc[i].FPSCount; j++)
        {
            FrameInternal = (pFrameIntervalData[4+j*4]<<24) | (pFrameIntervalData[4+j*4+1]<<16) + 
                 (pFrameIntervalData[4+j*4+2]<<8) | pFrameIntervalData[4+j*4+3];
            fps = 10000000/FrameInternal;
            Cap_Desc->FrameInt_Desc[i].FPS[j] = (unsigned char)fps;    
        }
        pFrameIntervalData += pFrameIntervalData[0];    
    }

    //calculate number of bitrate descriptor
    pBitrateData = pFrameIntervalData;
    i = 0;
    while(pBitrateData[1]==MSC_BITRATE)
    {
        pBitrateData += pBitrateData[0];
        i++;
    }
    Cap_Desc->NumBitrate = i;
    
    //parse bitrate descriptor
    pBitrateData = pFrameIntervalData;
    
    Cap_Desc->Bitrate_Desc = malloc(Cap_Desc->NumBitrate*sizeof(struct MultiStreamBitrate));
    for(i = 0; i<Cap_Desc->NumBitrate; i++)
    {
        memcpy(&Cap_Desc->Bitrate_Desc[i], pBitrateData+2, 2);
        pBitrateData += pBitrateData[0];
    }
    if((pBitrateData-pData)>Length)
    {
        printf("%s:data length error \n",__FUNCTION__);
        return -1;
    }       
    free(pData);
    return 0;
}

//===================end of capability parser====================================

