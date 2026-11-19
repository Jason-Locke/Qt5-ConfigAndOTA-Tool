#ifndef DLT698_FRAME_H
#define DLT698_FRAME_H

/**帧状态信息*/
typedef struct
{
    unsigned char  aSa[16];                 ///<服务器地址       服务器地址
    unsigned short userStart;                       ///<APDU数据偏移
    unsigned short userLen;                         ///<APDU数据长度
    unsigned short uiFrameLength;                   ///<帧总长度（包括帧头 帧尾）
    unsigned char  ucSaType;                        ///<服务器地址类型 0:单地址   1:通配地址   2：组地址   3：广播地址
    unsigned char  ucLogicAddr;                     ///<逻辑地址
    unsigned char  ucSalength;                      ///<服务器地址长度   0～15 表示 1～16
    unsigned char  ucFunCode;                       ///<功能码标识       1：链路管理 3：用户数据
    unsigned char  ucDir;                           ///<方向指示        0：客户机发出  1：服务器发出
    unsigned char  ucPrm;                           ///<启动标识        1：客户机发起  0：服务器发起
    unsigned char  ucSc;                            ///<国网新增加扰码编码格式    1:扰码格式（+33）
    unsigned char  ucFrameFlg;                      ///<分帧          1：表示APDU片段  0：完整APDU
    unsigned char  ucCa;                            ///<客户机地址       0:不关注客户机地址
}ReceieveFrame_Info_TPDF;

/**控制域*/
typedef union
{
    unsigned char Byte;
    struct
    {
        unsigned char bFunCode   : 3;   ///<功能码标识       1：链路管理 3：用户数据
        unsigned char bSc        : 1;   ///<扰码标志
        unsigned char bRev       : 1;   //保留
        unsigned char bFrameFlg  : 1;     ///<分帧            1：表示APDU片段  0：完整APDU
        unsigned char bPrm       : 1;   ///<启动标识      1：客户机发起  0：服务器发起
        unsigned char bDir       : 1;   ///<方向指示      0：客户机发出  1：服务器发出
    } Str;
} CtlTPDF;  ///<控制码数据类型

/**长度域*/
typedef union
{
    unsigned short Word;
    struct
    {
        unsigned short  uiLen   : 14; ///<length
        unsigned short  uiRev   : 2;  ///<reserve
    } Str;
} LengthTPDF;  ///<帧长度数据类型

///////////////////////////////////////////////
//CRC校验类型
///////////////////////////////////////////////
#define CRC16_X25           (0x00)
#define CRC16_XMODEM        (CRC16_X25 + 1)
#define CRC16_MAX           (CRC16_XMODEM + 1)

//bool DLT698_dCommStringFilter(const unsigned char *pRecive, unsigned short Reclen);
unsigned short servAlg_getCRC16(unsigned char crcType, const void *pData, unsigned short len);

#endif // DLT698_FRAME_H
