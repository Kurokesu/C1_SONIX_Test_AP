#define MAX_FORMAT  5
#define MAX_FRAME  10
#define MAX_FPS  10

struct CapabiltyBinaryData{
    unsigned char *pbuf;
    int Lenght;
};

struct FrameTypeDesc{
    unsigned short width;
    unsigned short height;
    unsigned char NumFPS;
    unsigned char FPS[MAX_FPS];
};


struct InterfaceDesc{
    unsigned char NumFormat;
    unsigned int fmt[MAX_FORMAT];
    unsigned char NumFrame[MAX_FORMAT];
    struct FrameTypeDesc frame_info[MAX_FORMAT][MAX_FRAME];
};


int GetCapability(int fd, struct CapabiltyBinaryData *pCapData);
int GetInterface(int fd,struct InterfaceDesc *InterfaceDesc);



