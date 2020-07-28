
//=======================================================================
//                                  start of capability parser defination
//=======================================================================

struct CapabilityDescriptor{
    unsigned char NumConfigs;
    struct MultiStreamCfg *Cfg_Desc;
    unsigned char NumDemuxers;
    struct MultiStreamDemuxer *demuxer_Desc;
    unsigned char NumFrameIntervals;
    struct MultiStreamFrameInterval *FrameInt_Desc; 
    unsigned char NumBitrate;
    struct MultiStreamBitrate *Bitrate_Desc;
};

struct MultiStreamCfg{
    unsigned char NumStreams;
    struct MultiStreamCap *MS_Cap;  //capability for each stream
};


struct MultiStreamCap{
    unsigned char UVCInterfaceNum;
    unsigned char UVCFormatIndex;
    unsigned char UVCFrameIndex;
    unsigned char DemuxerIndex;
    unsigned char FPSIndex;
    unsigned char BRCIndex;
    unsigned char OSDIndex;
    unsigned char MDIndex;
    unsigned char PTZIIndex;
    unsigned char FPSGroup;
    unsigned char BRCGroup;
    unsigned char OSDGroup;
};

struct MultiStreamDemuxer{
    unsigned char MSCDemuxIndex;
    unsigned char DemuxID;
    unsigned short Width;
    unsigned short Height;
};

struct MultiStreamFrameInterval{
    unsigned char FPSIndex;
    unsigned char FPSCount;
    unsigned char *FPS;  // (10^7) / FrameInterval
};

struct MultiStreamBitrate{
    unsigned char BRCIndex;
    unsigned char BRCMode;
};

enum{
	MSC_HEADER = 0,
	MSC_CONFIG,
	MSC_CAPABILITY,
	MSC_DEMUXER,
	MSC_FRAMEINTERVAL,
	MSC_BITRATE,
	MSC_OSD
};

#define MSC_HEADER_LENGTH 5

int ParseCapability(unsigned char *pCapability, int Length, struct CapabilityDescriptor *Cap_Desc);

//===================end of capability parser defination====================================

