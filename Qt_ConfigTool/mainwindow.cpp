#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSerialPort/QSerialPortInfo> //串口类头文件
#include <QDebug>
#include <QComboBox>
#include <QTextCodec>

#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QDataStream>

#include <QTimer>

#include <QMessageBox>
#include <QLabel>

#include <QSettings>

#include "QProcess" //260716 调用外部exe文件用

//#include <QSoundEffect> //lzl 播放音频类，在.pro文件中添加  QT += core gui multimedia

//lzl add=====================================
#include "DLT698_frame.h"
#include <QRegularExpression> //正则表达式
#include <QDateTime>
//============================================

//#include <synchapi.h> //Sleep()相关

//#define CMD_MAX_SIZE 256
//#define PACKAGE_SIZE 200
#define PACKAGE_SIZE 256  //260310

//quint8 RecData_tmp[256] = {0};
QString RecData_tmp;
//QByteArray info;

const quint8 Rx_OK[3]  = {0x5A, 0x01, 0x01};//返回成功帧
const quint8 Rx_Done_OK[3]  = {0x5A, 0x02, 0x01};
const quint8 Rx_Err[3] = {0x5A, 0x01, 0x00};
const quint8 Rx_Done_Err[3]  = {0x5A, 0x02, 0x00};
const quint8 GetUpgradeCmdOK[3]  = {0x5A, 0xFE, 0x01};
const quint8 GetUpgradeCmdErr[3]  = {0x5A, 0xFE, 0x00};
quint8 OTA_data[263] = {0}; //OTA每帧数据最大长度
quint16 OTA_Len = 0; //待发送的实际长度
quint8  Version[3] = {0};
quint8  Resend_cnt = 0; //Ota包重发次数

bool isCloseCMD_send = false;
quint32 CloseCheckOK_times = 0;
quint32 CloseCmdSend_times = 0;
quint8 InputCloseDelayTime = 0;
quint8 InputOpenDelayTime = 0;

char MAC645[6] = {0,0,0,0,0,2};//645地址全局变量

quint8 gModbus_Addr = 3;//modbus全局地址，默认为3

bool isReadVoltSend = false;
bool isReadFreqSend = false;
bool isReadMACSend = false;
bool isReadAssetSend = false;
bool isReadProtectBitSend = false;
bool isWriteProtectBitSend = false;
bool isWriteMACSend = false;
bool isWriteAssetSend = false;
bool isReadOVParaSend = false;
bool isReadUVParaSend = false;
bool isReadIslandParaSend = false;
bool isReadDeviceInfoSend = false;
bool isWriteOVParaSend = false;
bool isWriteUVParaSend = false;
bool isWriteIslandParaSend = false;
bool isWriteRS485ParaSend = false;
bool isRemoteOpenCmdSend = false;
bool isRemoteCloseAllowedCmdSend = false;
bool isRemoteCloseCmdSend = false;
bool isCalibrationCmdSend = false;
bool isFCTcmdSend = false;
bool isReadRunWordCmdSend = false;
//-------------645协议相关----------------------------
bool isReadAddr645_Send = false;
bool isManualRead645_Send = false;
bool isWriteMACSend645 = false;
bool isChangeBaudrate645 = false;
bool isReadVolt645 = false;
bool isReadFreq645 = false;
bool isChangePWD645 = false;
bool isReadDateTime645 = false;
bool isReadSoftwareVere645 = false;
bool isReadHardwareVere645 = false;
bool isReadAssetCode645 = false;
bool isReadProductionDate645 = false;
bool isReadRunWord1_645 = false;
bool isReadRunWord2_645 = false;
bool isReadCtrlWord1_645 = false;
bool isReadCtrlWord3_645 = false;
bool isReadCtrlWord7_645 = false;
bool isWriteCtrlWord1 = false;
bool isWriteCtrlWord3 = false;
bool isWriteCtrlWord7 = false;
bool isReadOpenTimeSend = false;
bool isWriteOpenTime = false;
bool isCancelOpenSend = false;
bool isReadCloseTimeSend = false;
bool isWriteCloseTime = false;
bool isCancelCloseSend = false;
bool isAutoSetDateTimeSend = false;
bool isManualSetDateTimeSend = false;
bool isReadTotalOpenCntSend = false;
bool isClearTotalOpenCntSend = false;
bool isReadOVOpenCntSend = false;
bool isReadUVOpenCntSend = false;
bool isReadLPOpenCntSend = false;
bool isReadLVOpenCntSend = false;
bool isReadRemoteOpenCntSend = false;
bool isReadIslandOpenCntSend = false;
bool isReadManualOpenCntSend = false;
bool isReadClearOpenCntSend = false;
bool isClearAllProtectOpenCntSend = false;
bool isReadTotalRunTimeSend = false;
bool isClearTotalRunTimeSend = false;
bool isReadStateChangeNewAddSend = false;
bool isClearAllSend = false;
bool isReadReadOVThresholdSend = false;
bool isReadReadUVThresholdSend = false;
bool isReadOVDelayTimeSend = false;
bool isReadUVDelayTimeSend = false;
bool isWriteOVThresholdSend = false;
bool isWriteUVThresholdSend = false;
bool isWriteOVDelayTimeSend = false;
bool isWriteUVDelayTimeSend = false;
bool isReadOVRecoverThresSend = false;
bool isReadUVRecoverThresSend = false;
bool isReadOVRecoverDelayTimeSend = false;
bool isReadUVRecoverDelayTimeSend = false;
bool isWriteOVRecoverThresSend = false;
bool isWriteUVRecoverThresSend = false;
bool isWriteOVRecoverDelayTimeSend = false;
bool isWriteUVRecoverDelayTimeSend = false;
bool isCalibration645Send = false;
bool isMotorPosMCBPosSend = false;
bool isEventCheckNo = false; //保护事件查询
bool isStateChangeNo = false; //闸位变化事件
bool isIslandEventNo = false; //孤岛事件
bool isSwitchingEventNo = false; //投退事件
bool isResetConfigSend = false;//一键恢复出厂配置
bool isGetDeviceTypeSend = false;//获取设备型号
bool isCloseStatusCheckSend = false;//检查合闸状态
//--------modbus相关--------------------------------
bool isMbReadAddrCheck = false;
bool isSendRemoteOpenCmd = false;
bool isSendCloseLeakCmd = false;
bool isSendEnableLeakCmd = false;
bool isMbReadUaCmd = false;
bool isMbReadIaCmd = false;
bool isMbReadFreqCmd = false;
bool isMbReadPaCmd = false;
bool isMbReadComboActECmd = false;
bool isMbReadPosActECmd = false;
bool isMbReadNegActECmd = false;
bool isMbReadPFaCmd = false;
bool isReadLcdDisplayIntervalTimeCmd = false;
bool isReadOVThresCmd = false;
bool isMbReadOVTimeCmd = false;
bool isWriteLcdIntervalTimeCmd = false;
bool isMbReadSysTimeCmd = false;
bool isMbGetWinTime2SetCmd = false;
bool isMbManuTimeSetCmd = false;
bool isMbSetOVThresCmd = false;
bool isMbSetOVTimeCmd = false;
bool isMbWriteAddrCmd = false;
bool isMbReadSoftVerCmd = false;
bool isMbReadSNPCodeCmd = false;
bool isMbWriteSNPcodeCmd = false;
bool isMbSoftResetCmd = false;
bool isMbClearAllEnergyCmd = false;
bool isMbStartCaliCmd = false;
bool isMbExitCaliCmd = false;
bool isMbCaliIaCmd = false;
bool isMbCaliUaCmd = false;
bool isMbCaliPaCmd = false;
bool isMbRs485ParaSetCmd = false;
bool isMbCheckProtectBitCmd = false;
bool isMbWriteProtectBitCmd = false;
bool isMbReadTempCmd = false;
bool isMbReadSoftTimeCmd = false;
bool isMbReadLifeCmd = false;
bool isMbReadLeakLifeCmd = false;
bool isMbReadEventRecordCmd = false;
bool isMbReadPosAEbaseCmd = false;
bool isMbReadNegAEbaseCmd = false;
bool isMbReadPosREbaseCmd = false;
bool isMbReadNegREbaseCmd = false;
bool isMbReadAppEnergybaseCmd = false;
bool isMbWritePosAEnergyCmd = false;
bool isMbWriteNegAEnergyCmd = false;
bool isMbWritePosREnergyCmd = false;
bool isMbWriteNegREnergyCmd = false;
bool isMbWriteAppEnergyCmd = false;
bool isMbReadRectEnergyAllCmd = false;
bool isMbReadAppEnergyAllCmd = false;
bool isMbReadRectPaCmd = false;
bool isMbReadAppPaCmd = false;
bool isMbReadRectEnergyCmd = false;
bool isMbReadNegRectEnergyCmd = false;
bool isReadUVThresCmd = false;
bool isMbReadUVTimeCmd = false;
bool isMbSetUVThresCmd = false;
bool isMbSetUVTimeCmd = false;
bool isMbReadCommCombined = false;
bool isMbSetCommCombined = false;
bool isMbRemoteCtrlLcdOnCmd = false;
bool isMbSetInValue = false;
bool isMbPaOffsetCaliCmd = false;

//32位补码转换成原码
//int comp32_to_orig(int x)
//{
//    /* 正数或 0，原码 = 补码 */
////    if (x >= 0)
////        return x;

//    /* 对数值位取反 + 1 */
//    int orig = ~x + 1;

//    /* 恢复符号位（最高位） */
//    return orig | 0x80000000;
//}

void HexPrint(char *packet, unsigned char size)
{
    QByteArray data((const char*)packet, size);
    QString hexString;
    for (int i = 0; i < data.size(); i++) {
        hexString += QString("%1 ").arg((quint8)data.at(i), 2, 16, QChar('0'));
    }
    qDebug() << hexString;
}

//判读字符串中是否只包含数字和不大于F或f的内容
bool isValidString(const QString &str) {
    QRegularExpression re("^[A-Fa-f0-9]+$"); // 匹配一个或多个数字或A-F或a-f的字符
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}
//是否只包含数字，用于判断BCD码输入是否正确
bool isNumOnlyString(const QString &str) {
    QRegularExpression re("^[0-9]+$");
    QRegularExpressionMatch match = re.match(str);
    return match.hasMatch();
}

quint8 Get_645_CS(quint8* data, quint8 len)
{
     quint8 i=0,ret=0;
     quint16 sum = 0;
     for(i=0;i<len;i++)
     {
         sum+=data[i];
     }
     ret = (quint8)sum;
     return ret;
}

quint8 HexToBcd(quint8 hex)
{
    return (hex % 10) + ((hex / 10) << 4);
}

quint8 BcdToHex(quint8 bcd)
{
    return ( ((bcd>>4) * 10) + (bcd & 0x0F) );
}

void MainWindow::displayHexArrayInWidget(const quint8* data, int size) {
    QString hexString;
//    int size = sizeof(data)* 2;
//    qDebug()<<"Hex size: "<<size;
    for (int i = 0; i < size; ++i) {
        QString hex = QString::number(data[i], 16).toUpper();
        if (hex.length() < 2) {
            hex.prepend('0');
        }
        hexString += hex;
        if (i < size - 1) {
            hexString += " ";
        }
    }

    ui->textEditSend->append(hexString);
}

//QString displayHexArrayInWidget(const quint8* data) {
//    QString hexString;
////    int size = sizeof(data)/sizeof(data[0]);
//    int size = sizeof(data) * 2;
//    qDebug()<<"Hex size: "<<size;
//    for (int i = 0; i < size; ++i) {
//        QString hex = QString::number(data[i], 16).toUpper();
//        if (hex.length() < 2) {
//            hex.prepend('0');
//        }
//        hexString += hex;
//        if (i < size - 1) {
//            hexString += " ";
//        }
//    }

//    return hexString;
//}

//-------------CRC32校验-----------------------------------------------------
//ulong crc32_compute(unsigned char* p_data, quint32 size, ulong const* p_crc) //这里p_crc = NULL
//{
//    ulong crc;
//    qDebug()<<"crc32_compute size: "<<size;
//    crc = (p_crc == NULL) ? 0xFFFFFFFF : ~(*p_crc);
//    for (quint32 i = 0; i < size; i++)
//    {
//        //qDebug()<<p_data[i];
//        crc = crc ^ p_data[i];
//        for (quint32 j = 8; j > 0; j--)
//        {
//            crc = (crc >> 1) ^ (0xEDB88320U & ((crc & 1) ? 0xFFFFFFFF : 0));
//        }
//    }
//    return ~crc;
//}

quint16 GetCRC16_Software(quint8* buf, qint32 len, quint16 initcrc)
{
    quint16 crc16 = initcrc;
    quint16 i = 0, j = 0;

    for (i = 0; i < len; i++)
    {
        //  u = ReverseBits((uint8_t)buf[i]);
        crc16 ^= (quint16)(buf[i] << 8);
        for (j = 0; j < 8; j++)
        {
            if ((crc16 & 0x8000) > 0)
            {
                crc16 = (quint16)((crc16 << 1) ^ 0x8005);
            }
            else
            {
                crc16 <<= 1;
            }
        }
    }
    return crc16;
}

//----------------modbus CRC16-------------------------------------------
/** Table of CRC values for high-order byte */
static const uint8_t _table_crc_hi[] =
    {
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
        0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
        0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40};

/** Table of CRC values for low-order byte */
static const uint8_t _table_crc_lo[] =
    {
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
        0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
        0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
        0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
        0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
        0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
        0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
        0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
        0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
        0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
        0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
        0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
        0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
        0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
        0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
        0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
        0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
        0x43, 0x83, 0x41, 0x81, 0x80, 0x40};
/**
 * @}
 */

/** @defgroup RTU_Private_Functions RTU Private Functions
 * @{
 */

/**
 * @brief   RTU CRC16 calculation
 * @param   buffer data pointer
 * @param   buffer_length data length
 * @return  CRC16 value
 */
static uint16_t modbus_rtu_crc16(uint8_t *buffer, uint16_t buffer_length)
{
    uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
    uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
    unsigned int i;        /* will index into CRC lookup */

    /* pass through message buffer */
    while (buffer_length--) {
        i = crc_hi ^ *buffer++; /* calculate the CRC  */
        crc_hi = crc_lo ^ _table_crc_hi[i];
        crc_lo = _table_crc_lo[i];
    }

    return (crc_hi << 8 | crc_lo);
}
//------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //添加串口列表
    m_serialPort = new QSerialPort();
    m_portNameList = getPortNameList();
    ui->comboBox_serialNum->addItems(m_portNameList);
    //添加波特率列表
    QStringList bautRatesList;
    bautRatesList << "600"<< "1200" << "2400" << "4800" << "9600" << "19200" << "57600" << "115200";
    ui->comboBox_baudrate->addItems(bautRatesList);
    ui->comboBox_baudrate->setCurrentIndex(4); //默认9600bps

    ui->comboBox_baudrate_2->addItems(bautRatesList);
    ui->comboBox_baudrate_2->setCurrentIndex(4); //默认9600bps

    //添加校验位列表
    QStringList parityList;
    parityList << "None" << "Odd" << "Even";
    ui->comboBoxParity->addItems(parityList);
    ui->comboBoxParity->setCurrentIndex(0);//默认无校验

    ui->comboBoxParity_2->addItems(parityList);
    ui->comboBoxParity_2->setCurrentIndex(2);//默认偶校验

    //添加645密级列表
    QStringList PassCodeLevelList;
    PassCodeLevelList << "00" << "01" << "02";
    ui->comboBox_PassCodeLevel->addItems(PassCodeLevelList);
    ui->comboBox_PassCodeLevel->setCurrentIndex(0);//默认最高级00

    //添加645控制码列表
    QStringList CtrlCodeList;
    CtrlCodeList << "11" << "14";
    ui->comboBox_CtrlCode->addItems(CtrlCodeList);
    ui->comboBox_CtrlCode->setCurrentIndex(0);

    //添加645组帧数据部分数据类型
    QStringList InputDataTypeList;
//    InputDataTypeList << "BCD" << "ASCII" <<"BIN";
    InputDataTypeList << "BCD" << "ASCII";
    ui->comboBox_InputDataType->addItems(InputDataTypeList);
    ui->comboBox_InputDataType->setCurrentIndex(0);

    //645更改波特率列表
    QStringList Baudrate645TypeList;
    Baudrate645TypeList << "115200" << "19200" << "9600" << "4800" << "2400" << "1200" << "600";
    ui->comboBox_BaudrateList->addItems(Baudrate645TypeList);
    ui->comboBox_BaudrateList->setCurrentIndex(2);

    //645预约分闸时间单位列表
    QStringList OpenTimeUnitList;
    OpenTimeUnitList << "分" << "时" ;
    ui->comboBox_OpenTimeUnit->addItems(OpenTimeUnitList);
    ui->comboBox_OpenTimeUnit->setCurrentIndex(0);
    //645预约合闸时间单位列表
    QStringList CloseTimeUnitList;
    CloseTimeUnitList << "分" << "时" ;
    ui->comboBox_CloseTimeUnit->addItems(CloseTimeUnitList);
    ui->comboBox_CloseTimeUnit->setCurrentIndex(0);

    //645保护事件上n次列表
    QStringList ProtectEventList;
    ProtectEventList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";
    ui->comboBox_ProtectEventCnt->addItems(ProtectEventList);
    ui->comboBox_ProtectEventCnt->setCurrentIndex(0);

    //645闸位变化事件上n次列表
    QStringList StateChangeEventList;
    StateChangeEventList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";
    ui->comboBox_StateChangeCnt->addItems(StateChangeEventList);
    ui->comboBox_StateChangeCnt->setCurrentIndex(0);

    //645保护投退事件上n次列表
    QStringList SwitchingEventList;
    SwitchingEventList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";
    ui->comboBox_SwitchingEventCnt->addItems(SwitchingEventList);
    ui->comboBox_SwitchingEventCnt->setCurrentIndex(0);

    //645孤岛事件上n次列表
    QStringList IslandEventList;
    IslandEventList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";
    ui->comboBox_IslandEvent->addItems(IslandEventList);
    ui->comboBox_IslandEvent->setCurrentIndex(0);

    //NXBLU125D-Modbus更改波特率列表
    QStringList BaudrateMbTypeList;
    BaudrateMbTypeList << "115200" << "19200" << "9600" << "4800" << "2400" << "1200";
    ui->comboBox_MbBaudrateChoose->addItems(BaudrateMbTypeList);
    ui->comboBox_MbBaudrateChoose->setCurrentIndex(2);

    //NXBLU125D-Modbus校验位列表
    QStringList parityMbList;
    parityMbList << "None" << "Odd" << "Even";
    ui->comboBox_MbCheckBitChoose->addItems(parityMbList);
    ui->comboBox_MbCheckBitChoose->setCurrentIndex(0);//默认无校验

    //NXBLU125D-Modbus事件上n次列表
    QStringList MbProtectEventList;
    MbProtectEventList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10";
    ui->comboBox_MbEventRecord->addItems(MbProtectEventList);
    ui->comboBox_MbEventRecord->setCurrentIndex(0);

    //NXBLU125D-Modbus点表组合列表
    QStringList MbCommCombinedList;
    MbCommCombinedList << "研+仪表" << "研+仪表+德力西" << "研+仪表+天正";
    ui->comboBox_SetCommCombined->addItems(MbCommCombinedList);
    ui->comboBox_SetCommCombined->setCurrentIndex(0);

    //NXBLU125D-Modbus电流规格列表
    QStringList InValueMbList;
    InValueMbList << "125A" << "100A" << "80A"<< "63A";
    ui->comboBox_MbChooseInValue->addItems(InValueMbList);
    ui->comboBox_MbChooseInValue->setCurrentIndex(0);//默认无校验

    //645数据域顺序不可修改
    ui->checkBox_DataOrder->setEnabled(false);

    status = new QLabel(u8"欢迎使用，联系作者：lzlei@chint.com 或 luozhenglei397@163.com");
    status->setMinimumWidth(200);
    statusBar()->addWidget(status);//状态栏显示只能代码编辑创建，不能在UI中创建

    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);
    isRecOK = false;
    step = 0;
    package_num = 0;
    package_ID = 0;
    isFileLoad = false;
//    isBooting = false;
    isUpdating = false;
    binSize = 0;

//    timer = new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(timerTransData()));

    //250818 串口数据接收超时定时器 =====================================================
    rec_timer = new QTimer(this);
    connect(rec_timer,SIGNAL(timeout()),this,SLOT(timerRecDataProcess()));
    //===============================================================================

    //251217 自动分合闸定时器===========================================================
    AutoCloseOpen_timer = new QTimer(this);
    connect(AutoCloseOpen_timer,SIGNAL(timeout()),this,SLOT(timerAutoOCProcess()));

    CheckCloseStatus_timer = new QTimer(this);
    connect(CheckCloseStatus_timer,SIGNAL(timeout()),this,SLOT(timerCheckCloseProcess()));
    //===============================================================================

    //260423 自动发送指定命令定时器======================================================
    AutoFresh_timer = new QTimer(this);
    connect(AutoFresh_timer,SIGNAL(timeout()),this,SLOT(timerAutoFreshProcess()));
    //===============================================================================

    //260513 自动更新系统时间定时器======================================================
    RealTime_timer = new QTimer(this);
    connect(RealTime_timer,SIGNAL(timeout()),this,SLOT(timerRealTimeProcess()));
    RealTime_timer->start(1000);//1s更新一次
    //===============================================================================

    //260520 循环发送定时器=============================================================
    Cycle_timer1 = new QTimer(this);
    connect(Cycle_timer1,SIGNAL(timeout()),this,SLOT(timerCycleSend1Process()));
    Cycle_timer2 = new QTimer(this);
    connect(Cycle_timer2,SIGNAL(timeout()),this,SLOT(timerCycleSend2Process()));
    Cycle_timer3 = new QTimer(this);
    connect(Cycle_timer3,SIGNAL(timeout()),this,SLOT(timerCycleSend3Process()));
    Cycle_timer4 = new QTimer(this);
    connect(Cycle_timer4,SIGNAL(timeout()),this,SLOT(timerCycleSend4Process()));
    Cycle_timer5 = new QTimer(this);
    connect(Cycle_timer5,SIGNAL(timeout()),this,SLOT(timerCycleSend5Process()));
    Cycle_timer6 = new QTimer(this);
    connect(Cycle_timer6,SIGNAL(timeout()),this,SLOT(timerCycleSend6Process()));
    //===============================================================================


    ui->pushButton_protocol645->setStyleSheet("QPushButton { color: red; }");

    ui->pushButton_MBCaliStatement->setStyleSheet("QPushButton { color: red; }");

    ui->pushButton_ReadCommCombined->setStyleSheet("QPushButton { color: green; }");
    ui->pushButton_WriteCommCombined->setStyleSheet("QPushButton { color: red; }");

//    qRegisterMetaType<QSerialPort::SerialPortError>("SerialThread");
//    connect(m_serialPort, &QSerialPort::errorOccurred,this, &SerialThread::handleSerialError);
    connect(m_serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleError(QSerialPort::SerialPortError error)));

//    ui->pushButtonSync->setEnabled(false);
//    ui->pushButtonErase->setEnabled(false);

    ui->pushButton_AutoCloseOpenStop->setEnabled(false);

    ui->pushButtonSend1->setEnabled(false);
    ui->pushButtonSend2->setEnabled(false);
    ui->pushButtonSend3->setEnabled(false);
    ui->pushButtonSend4->setEnabled(false);
    ui->pushButtonSend5->setEnabled(false);
    ui->pushButtonSend6->setEnabled(false);

//    ui->tabWidget->setTabEnabled(0, false);
//    ui->tabWidget->setTabEnabled(1, false);
//    ui->tabWidget->setTabEnabled(2, false);
//    ui->tabWidget->setTabEnabled(3, false);

    ui->pushButton_AutoFreshStart->setEnabled(true);
    ui->pushButton_AutoFreshStop->setEnabled(false);

    LoadConfig();

    //语言提示相关
//    #define Debug_Voice  //语音调试开关

    effect_ComOccupied = new QSoundEffect(this);
    effect_ComChanged = new QSoundEffect(this);
    effect_WelcomUse = new QSoundEffect(this);
#ifdef Debug_Voice //仅在调试时使用，否则打包后的文件夹无声音。
    effect_ComOccupied->setSource(QUrl::fromLocalFile("release/Com_Occupied.wav"));
    effect_ComChanged->setSource(QUrl::fromLocalFile("release/Com_Changed.wav"));
    effect_WelcomUse->setSource(QUrl::fromLocalFile("release/Com_WelcomUse.wav"));
#else
    effect_ComOccupied->setSource(QUrl::fromLocalFile("Com_Occupied.wav"));//正式发布时使用
    effect_ComChanged->setSource(QUrl::fromLocalFile("Com_Changed.wav"));
    effect_WelcomUse->setSource(QUrl::fromLocalFile("Com_WelcomUse.wav"));
#endif

    //提示串口当前状态
    QString ComStatus = "串口未打开！！！";
    ui->label_ComStatus->setText(QString("<span style='color:red;'>%1</span>").arg(ComStatus));

    effect_WelcomUse->setVolume(1.0f);
    effect_WelcomUse->play();

}

MainWindow::~MainWindow()
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;

    SaveConfig();

//    if(isFileLoad)
//    {
//        isFileLoad = false;
//        delete[] MybinByte;
//    }

    delete ui;
}

QStringList MainWindow::getPortNameList()
{
    QStringList m_serialPortName;
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        m_serialPortName << info.portName();
        qDebug()<<"serialPortName:"<<info.portName();
    }
    return m_serialPortName;
}

void setAllRecFlagsFalse(void)
{
    isCloseCMD_send = false;

    isReadVoltSend = false;
    isReadFreqSend = false;
    isReadMACSend = false;
    isReadAssetSend = false;
    isReadProtectBitSend = false;
    isWriteProtectBitSend = false;
    isWriteMACSend = false;
    isWriteAssetSend = false;
    isReadOVParaSend = false;
    isReadUVParaSend = false;
    isReadIslandParaSend = false;
    isReadDeviceInfoSend = false;
    isWriteOVParaSend = false;
    isWriteUVParaSend = false;
    isWriteIslandParaSend = false;
    isWriteRS485ParaSend = false;
    isRemoteOpenCmdSend = false;
    isRemoteCloseAllowedCmdSend = false;
    isRemoteCloseCmdSend = false;
    isCalibrationCmdSend = false;
    isFCTcmdSend = false;
    isReadRunWordCmdSend = false;
    //----------645协议相关----------------------------
    isReadAddr645_Send = false;
    isManualRead645_Send = false;
    isWriteMACSend645 = false;
    isChangeBaudrate645 = false;
    isReadVolt645 = false;
    isReadFreq645 = false;
    isChangePWD645 = false;
    isReadDateTime645 = false;
    isReadSoftwareVere645 = false;
    isReadHardwareVere645 = false;
    isReadAssetCode645 = false;
    isReadProductionDate645 = false;
    isReadRunWord1_645 = false;
    isReadRunWord2_645 = false;
    isReadCtrlWord1_645 = false;
    isReadCtrlWord3_645 = false;
    isReadCtrlWord7_645 = false;
    isWriteCtrlWord1 = false;
    isWriteCtrlWord3 = false;
    isWriteCtrlWord7 = false;
    isReadOpenTimeSend = false;
    isWriteOpenTime = false;
    isCancelOpenSend = false;
    isReadCloseTimeSend = false;
    isWriteCloseTime = false;
    isCancelCloseSend = false;
    isAutoSetDateTimeSend = false;
    isManualSetDateTimeSend = false;
    isReadTotalOpenCntSend = false;
    isClearTotalOpenCntSend = false;
    isReadOVOpenCntSend = false;
    isReadUVOpenCntSend = false;
    isReadLPOpenCntSend = false;
    isReadLVOpenCntSend = false;
    isReadRemoteOpenCntSend = false;
    isReadIslandOpenCntSend = false;
    isReadManualOpenCntSend = false;
    isReadClearOpenCntSend = false;
    isClearAllProtectOpenCntSend = false;
    isReadTotalRunTimeSend = false;
    isClearTotalRunTimeSend = false;
    isReadStateChangeNewAddSend = false;
    isClearAllSend = false;
    isReadReadOVThresholdSend = false;
    isReadReadUVThresholdSend = false;
    isReadOVDelayTimeSend = false;
    isReadUVDelayTimeSend = false;
    isWriteOVThresholdSend = false;
    isWriteUVThresholdSend = false;
    isWriteOVDelayTimeSend = false;
    isWriteUVDelayTimeSend = false;
    isReadOVRecoverThresSend = false;
    isReadUVRecoverThresSend = false;
    isReadOVRecoverDelayTimeSend = false;
    isReadUVRecoverDelayTimeSend = false;
    isWriteOVRecoverThresSend = false;
    isWriteUVRecoverThresSend = false;
    isWriteOVRecoverDelayTimeSend = false;
    isWriteUVRecoverDelayTimeSend = false;
    isCalibration645Send = false;
    isMotorPosMCBPosSend = false;
    isEventCheckNo = false; //保护事件查询
    isStateChangeNo = false; //闸位变化事件
    isIslandEventNo = false; //孤岛事件
    isSwitchingEventNo = false; //投退事件
    isResetConfigSend = false;
    isGetDeviceTypeSend = false;
    isCloseStatusCheckSend = false;
    isMbReadAddrCheck = false;
    isSendRemoteOpenCmd = false;
    isSendCloseLeakCmd = false;
    isSendEnableLeakCmd = false;
    isMbReadUaCmd = false;
    isMbReadIaCmd = false;
    isMbReadFreqCmd = false;
    isMbReadPaCmd = false;
    isMbReadComboActECmd = false;
    isMbReadPosActECmd = false;
    isMbReadNegActECmd = false;
    isMbReadPFaCmd = false;
    isReadLcdDisplayIntervalTimeCmd = false;
    isReadOVThresCmd = false;
    isMbReadOVTimeCmd = false;
    isWriteLcdIntervalTimeCmd = false;
    isMbReadSysTimeCmd = false;
    isMbGetWinTime2SetCmd = false;
    isMbManuTimeSetCmd = false;
    isMbSetOVThresCmd = false;
    isMbSetOVTimeCmd = false;
    isMbWriteAddrCmd = false;
    isMbReadSoftVerCmd = false;
    isMbReadSNPCodeCmd = false;
    isMbWriteSNPcodeCmd = false;
    isMbSoftResetCmd = false;
    isMbClearAllEnergyCmd = false;
    isMbStartCaliCmd = false;
    isMbExitCaliCmd = false;
    isMbCaliIaCmd = false;
    isMbCaliUaCmd = false;
    isMbCaliPaCmd = false;
    isMbRs485ParaSetCmd = false;
    isMbCheckProtectBitCmd = false;
    isMbWriteProtectBitCmd = false;
    isMbReadTempCmd = false;
    isMbReadSoftTimeCmd = false;
    isMbReadLifeCmd = false;
    isMbReadLeakLifeCmd = false;
    isMbReadEventRecordCmd = false;
    isMbReadPosAEbaseCmd = false;
    isMbReadNegAEbaseCmd = false;
    isMbReadPosREbaseCmd = false;
    isMbReadNegREbaseCmd = false;
    isMbReadAppEnergybaseCmd = false;
    isMbWritePosAEnergyCmd = false;
    isMbWriteNegAEnergyCmd = false;
    isMbWritePosREnergyCmd = false;
    isMbWriteNegREnergyCmd = false;
    isMbWriteAppEnergyCmd = false;
    isMbReadRectEnergyAllCmd = false;
    isMbReadAppEnergyAllCmd = false;
    isMbReadRectPaCmd = false;
    isMbReadAppPaCmd = false;
    isMbReadRectEnergyCmd = false;
    isMbReadNegRectEnergyCmd = false;
    isReadUVThresCmd = false;
    isMbReadUVTimeCmd = false;
    isMbSetUVThresCmd = false;
    isMbSetUVTimeCmd = false;
    isMbReadCommCombined = false;
    isMbSetCommCombined = false;
    isMbRemoteCtrlLcdOnCmd = false;
    isMbSetInValue = false;
    isMbPaOffsetCaliCmd = false;
}

void MainWindow::on_pushButtonOpenSerial_clicked()
{
    if(ui->comboBox_serialNum->currentText() == "")
    {
        qDebug()<<"serialPort is null";
        return;
    }

    if (ui->pushButtonOpenSerial->text()=="打开串口")
    {
        if(m_serialPort->isOpen())
        {
            m_serialPort->clear();
            m_serialPort->close();
        }

        m_serialPort->setPortName(m_portNameList[ui->comboBox_serialNum->currentIndex()]);

        if(!m_serialPort->open(QIODevice::ReadWrite))
        {
            effect_ComOccupied->setVolume(1.0f);
            effect_ComOccupied->play();
            qDebug()<<m_portNameList[ui->comboBox_serialNum->currentIndex()]<<"打开失败!";
            QMessageBox::warning(this,"OpenComm Message", "打开串口失败，检查是否被占用");
            return;
        }

        //提示串口当前状态
        QString ComStatus = "串口已打开";
        ui->label_ComStatus->setText(QString("<span style='color:green;'>%1</span>").arg(ComStatus));

        ui->pushButtonFreshPort->setEnabled(false);

        //打开成功
        m_serialPort->setBaudRate(ui->comboBox_baudrate->currentText().toInt(),QSerialPort::AllDirections);//设置波特率和读写方向
        m_serialPort->setDataBits(QSerialPort::Data8);              //数据位为8位
        m_serialPort->setFlowControl(QSerialPort::NoFlowControl);   //无流控制
        //m_serialPort->setParity(QSerialPort::EvenParity);             //无校验位
        if(ui->comboBoxParity->currentText() == "None")
        {
            m_serialPort->setParity(QSerialPort::NoParity);             //无校验位
        }
        else if(ui->comboBoxParity->currentText() == "Odd")
        {
            m_serialPort->setParity(QSerialPort::OddParity);
        }
        else if(ui->comboBoxParity->currentText() == "Even")
        {
            m_serialPort->setParity(QSerialPort::EvenParity);
        }
        m_serialPort->setStopBits(QSerialPort::OneStop);            //一位停止位

        connect(m_serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));//当缓冲区中收到串口数据的时候，readyRead() 信号将被发射，我们定义个槽 void receiveInfo() 来解析收到的数据。

        ui->pushButtonOpenSerial->setText("关闭串口");

        ui->pushButtonSend1->setEnabled(true);
        ui->pushButtonSend2->setEnabled(true);
        ui->pushButtonSend3->setEnabled(true);
        ui->pushButtonSend4->setEnabled(true);
        ui->pushButtonSend5->setEnabled(true);
        ui->pushButtonSend6->setEnabled(true);

        ui->tabWidget->setTabEnabled(0, true);
        ui->tabWidget->setTabEnabled(1, true);
        ui->tabWidget->setTabEnabled(2, true);
        ui->tabWidget->setTabEnabled(3, true);
    } else
    {
        m_serialPort->close();
        ui->pushButtonOpenSerial->setText("打开串口");
        ui->pushButtonFreshPort->setEnabled(true);

        ui->pushButtonSend1->setEnabled(false);
        ui->pushButtonSend2->setEnabled(false);
        ui->pushButtonSend3->setEnabled(false);
        ui->pushButtonSend4->setEnabled(false);
        ui->pushButtonSend5->setEnabled(false);
        ui->pushButtonSend6->setEnabled(false);

        //提示串口当前状态
        QString ComStatus = "串口已关闭！！！";
        ui->label_ComStatus->setText(QString("<span style='color:red;'>%1</span>").arg(ComStatus));

//        ui->tabWidget->setTabEnabled(0, false);
//        ui->tabWidget->setTabEnabled(1, false);
//        ui->tabWidget->setTabEnabled(2, false);
//        ui->tabWidget->setTabEnabled(3, false);

        setAllRecFlagsFalse();
    }
}

void MainWindow::receiveInfo()
{
    //QByteArray temp = m_serialPort->readAll();
    //this->ui->recData->setText(temp.toHex());
    ////this->ui->recData->setText(temp);

#if 1
    //03 10 00 00 00 01 00 2B 00
//    quint8 boot_cmd[] = {0x03, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x2B, 0x00};
//    QString boot1 = "\n[OK] *********reboot to bootloader succeed************\n";
//    QString done = "\n[Info] ********* update done ************\n";
//    QString Err1 = "\n[err] *********Package CRC32 Err************\n";
//    QString Err2 = "\n[err] *********Package Verify Err***********\n";

    qDebug()<<"\nreceiveInfo()";
    QByteArray info = m_serialPort->readAll();
    quint16 buf_len = info.size();
    qDebug()<<"RAW LEN: "<<buf_len;

    if(0 == buf_len)
    {
        qDebug()<<"\nRec empty";
        return;
    }

    //-----处理设备返回的信息：校验------------------
#if 0
    unsigned char buf_get[16]={0};//大小待定 实际接收 FE00000088BEFD47EF
    char* buf = info.data();
    QString Str;
    for(int i=0;i<buf_len;i++)
    {
        buf_get[i] = (unsigned char)buf[i];
        //qDebug() << Str.asprintf("0x%02x",buf_get[i]);//16进制打印
    }

    if(isBooting)
    {
        if(0==strcmp((char *)&boot_cmd, (char *)buf_get))
        {
            qDebug("boot ret right!!!!!");
            ui->recData->insertPlainText (boot1);
        }
    }
    qDebug("isBooting = %d",isBooting);
    quint16 len = buf_get[2] | (buf_get[3]<<8);
    qDebug()<<"Data LEN: "<<len;
    ulong crc32result = crc32_compute((unsigned char*)&buf_get, len+4, NULL);
    qDebug()<<"crc32result_Caculate: "<<crc32result;//10进制
    qDebug()<<"crc32result_rec: "<<*(quint32 *)(buf_get + 4 + len);
    if(isUpdating && !isBooting && (crc32result != *(quint32 *)(buf_get + 4 + len)))//如果校验错误或返回的状态异常，重发异常包
    {
        isRecOK = false;
        if(!isBooting)
        {
            ui->recData->insertPlainText (Err1);
            qDebug()<<"[err] Wrong crc32";
        }
    }
    else if(isUpdating && !isBooting && (buf_get[1] != 0))
    {
        isRecOK = false;
        ui->recData->insertPlainText (Err2);
        qDebug()<<"[err] Wrong verification";
    }
    else
    {
        if(isUpdating && (2 == step) && (package_num - package_ID > 1))
        {
            package_ID++;//包ID从序号0开始
        }
        else
        {
            if(!isBooting && isUpdating)
            {
                step++;
            }

        }
        double dProgress = package_ID * 100.0  / (package_num-1);
        ui->progressBar->setValue(dProgress);

        if(100 == dProgress)
        {
            ui->recData->insertPlainText (done);
        }


        isRecOK = true;
        qDebug()<<"Correct verification";
    }
#endif
    //-------------------------------------------

    QString strReceiveData = "";
    rec_timer->stop();
    if(ui->checkBoxHexReceive->isChecked()) //十六进制输出
    {
        QByteArray hexData = info.toHex();
        strReceiveData = hexData.toUpper();

        RecData_tmp.append(strReceiveData);//追加数据

//        qDebug()<<"接收到串口数据: "<<strReceiveData;

//        for(int i=0; i<strReceiveData.size(); i+=2+1)
//            strReceiveData.insert(i, QLatin1String(" "));
//        strReceiveData.remove(0, 1);

//        qDebug()<<"处理后的串口数据: "<<strReceiveData;

        //250818
        //isRecDone = false;

//        rec_timer->start(30);//接收数据后开始超时计数,波特率小于等于600bps开始异常

//        rec_timer->start(50);//接收数据后开始超时计数 251218

        //根据选择的波特率调整超时时间
        quint32 buad = ui->comboBox_baudrate->currentText().toInt();
        qDebug()<<"解析的波特率: "<<buad;
        switch(buad)
        {
            case 600:
                rec_timer->start(50);
                break;
            case 1200:
            case 2400:
                rec_timer->start(40);
                break;
            case 4800:
            case 9600:
                rec_timer->start(35);
                break;
            case 19200:

            case 57600:
                rec_timer->start(20);
                break;
            case 115200:
                rec_timer->start(10);
                break;
            default:
                rec_timer->start(50);
                break;
        }

    }
    else //ASCII输出
    {
        strReceiveData = info;

        //避免中文乱码
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString tmpQStr = tc->toUnicode(info);

        ui->recData->append(tmpQStr);
//        rec_timer->start(50);//接收数据后开始超时计数 251218

        //根据选择的波特率调整超时时间
        quint32 buad = ui->comboBox_baudrate->currentText().toInt();
        qDebug()<<"解析的波特率: "<<buad;
        switch(buad)
        {
            case 600:
                rec_timer->start(50);
                break;
            case 1200:
            case 2400:
                rec_timer->start(40);
                break;
            case 4800:
            case 9600:
                rec_timer->start(35);
                break;
            case 19200:

            case 57600:
                rec_timer->start(20);
                break;
            case 115200:
                rec_timer->start(10);
                break;
            default:
                rec_timer->start(50);
                break;
        }
    }
#endif
    //ui->txtReceiveData->append("\r\n");
}


//另一个 函数 char 转为 16进制
char convertCharToHex(char ch)
{
    /*
    0x30等于十进制的48，48也是0的ASCII值，，
    1-9的ASCII值是49-57，，所以某一个值－0x30，，
    就是将字符0-9转换为0-9
    */
    if((ch >= '0') && (ch <= '9'))
         return ch-0x30;
     else if((ch >= 'A') && (ch <= 'F'))
         return ch-'A'+10;
     else if((ch >= 'a') && (ch <= 'f'))
         return ch-'a'+10;
     else return (-1);
}

//基本和单片机交互 数据 都是16进制的 我们这里自己写一个 Qstring 转为 16进制的函数
void convertStringToHex(const QString &str, QByteArray &byteData)
{
    int hexdata,lowhexdata;
    int hexdatalen = 0;
    int len = str.length();
    byteData.resize(len/2);
    char lstr,hstr;
    for(int i=0; i<len; )
    {
        //char lstr,
        hstr=str[i].toLatin1();
        if(hstr == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= len)
            break;
        lstr = str[i].toLatin1();
        hexdata = convertCharToHex(hstr);
        lowhexdata = convertCharToHex(lstr);
        if((hexdata == 16) || (lowhexdata == 16))
            break;
        else
            hexdata = hexdata*16+lowhexdata;
        i++;
        byteData[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }
    byteData.resize(hexdatalen);
}

void MainWindow::on_pushButtonSend1_clicked()
{
    QString m_strSendData = ui->lineEditSend->text();

    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
//    ui->textEditSend->append(m_strSendData);
    if(ui->checkBoxHexSend->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButtonClearSend_clicked()
{
    ui->lineEditSend->setText(NULL);
}

void MainWindow::on_pushButtonClearRec_clicked()
{
    ui->recData->setText(NULL);
}

void MainWindow::timerRecDataProcess()
{
    //qDebug("\timerRecDataProcess() once ");
    //rec_timer->stop();

//    char buffer[256] = {0};

    qDebug()<<"接收到串口数据: "<<RecData_tmp;

    //如果是发送指令请求的数据，这里要提取需要的数据==============
//    QByteArray byteArray = RecData_tmp.toUtf8();
//    qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
//    qDebug()<<"转换为utf8数据: "<<byteArray.constData();
//    strncpy(buffer, byteArray.constData(), sizeof(buffer));

  if(false == isUpdating) //不是升级数据
  {
    if(isReadVoltSend)
    {
        isReadVoltSend = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[3] = {0};//三相电压值

        QByteArray byteArray = RecData_tmp.toUtf8();

        //QByteArray byteArray;
        //convertStringToHex(RecData_tmp, byteArray);

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));
//        qDebug()<<"测试数据0: "<<buffer[0];
//        qDebug()<<"测试数据1: "<<buffer[1];

        //quint8 dd = convertCharToHex(buffer[1]);
        //qDebug()<<"转换数据: "<<dd;

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[58]);
            VoltData_tmp[1] = convertCharToHex(buffer[59]);
            VoltData_tmp[2] = convertCharToHex(buffer[60]);
            VoltData_tmp[3] = convertCharToHex(buffer[61]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"A相电压: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_VA->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[64]);
            VoltData_tmp[1] = convertCharToHex(buffer[65]);
            VoltData_tmp[2] = convertCharToHex(buffer[66]);
            VoltData_tmp[3] = convertCharToHex(buffer[67]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"B相电压: "<<VoltData[1];
            float VB = VoltData[1]/10.0;
            QString textB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_VB->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[70]);
            VoltData_tmp[1] = convertCharToHex(buffer[71]);
            VoltData_tmp[2] = convertCharToHex(buffer[72]);
            VoltData_tmp[3] = convertCharToHex(buffer[73]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"C相电压: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_VC->setText(textC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[50]);
            VoltData_tmp[1] = convertCharToHex(buffer[51]);
            VoltData_tmp[2] = convertCharToHex(buffer[52]);
            VoltData_tmp[3] = convertCharToHex(buffer[53]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"A相电压: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_VA->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[56]);
            VoltData_tmp[1] = convertCharToHex(buffer[57]);
            VoltData_tmp[2] = convertCharToHex(buffer[58]);
            VoltData_tmp[3] = convertCharToHex(buffer[59]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"B相电压: "<<VoltData[1];
            float VB = VoltData[1]/10.0;
            QString textB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_VB->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[62]);
            VoltData_tmp[1] = convertCharToHex(buffer[63]);
            VoltData_tmp[2] = convertCharToHex(buffer[64]);
            VoltData_tmp[3] = convertCharToHex(buffer[65]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"C相电压: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_VC->setText(textC);
        }
    }
    else if(isReadFreqSend)
    {
        isReadFreqSend = false;

        char buffer[128] = {0};
        quint8 FreqData_tmp[4] = {0};
        quint16 FreqData[3] = {0};//三相电压值

        QByteArray byteArray = RecData_tmp.toUtf8();

        //QByteArray byteArray;
        //convertStringToHex(RecData_tmp, byteArray);

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));
//        qDebug()<<"测试数据0: "<<buffer[0];
//        qDebug()<<"测试数据1: "<<buffer[1];

        //quint8 dd = convertCharToHex(buffer[1]);
        //qDebug()<<"转换数据: "<<dd;

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //A相
            FreqData_tmp[0] = convertCharToHex(buffer[58]);
            FreqData_tmp[1] = convertCharToHex(buffer[59]);
            FreqData_tmp[2] = convertCharToHex(buffer[60]);
            FreqData_tmp[3] = convertCharToHex(buffer[61]);
            FreqData[0]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"A相频率: "<<FreqData[0];
            float VA = FreqData[0]/100.0;
            QString textA = QString::number(VA, 'f', 2); // 保留两位小数
            ui->lineEdit_FA->setText(textA);
            //B相
            FreqData_tmp[0] = convertCharToHex(buffer[64]);
            FreqData_tmp[1] = convertCharToHex(buffer[65]);
            FreqData_tmp[2] = convertCharToHex(buffer[66]);
            FreqData_tmp[3] = convertCharToHex(buffer[67]);
            FreqData[1]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"B相频率: "<<FreqData[1];
            float VB = FreqData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); // 保留两位小数
            ui->lineEdit_FB->setText(textB);
            //C相
            FreqData_tmp[0] = convertCharToHex(buffer[70]);
            FreqData_tmp[1] = convertCharToHex(buffer[71]);
            FreqData_tmp[2] = convertCharToHex(buffer[72]);
            FreqData_tmp[3] = convertCharToHex(buffer[73]);
            FreqData[2]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"C相频率: "<<FreqData[2];
            float VC = FreqData[2]/100.0;
            QString textC = QString::number(VC, 'f', 2); // 保留两位小数
            ui->lineEdit_FC->setText(textC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //A相
            FreqData_tmp[0] = convertCharToHex(buffer[50]);
            FreqData_tmp[1] = convertCharToHex(buffer[51]);
            FreqData_tmp[2] = convertCharToHex(buffer[52]);
            FreqData_tmp[3] = convertCharToHex(buffer[53]);
            FreqData[0]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"A相频率: "<<FreqData[0];
            float VA = FreqData[0]/100.0;
            QString textA = QString::number(VA, 'f', 2); // 保留两位小数
            ui->lineEdit_FA->setText(textA);
            //B相
            FreqData_tmp[0] = convertCharToHex(buffer[56]);
            FreqData_tmp[1] = convertCharToHex(buffer[57]);
            FreqData_tmp[2] = convertCharToHex(buffer[58]);
            FreqData_tmp[3] = convertCharToHex(buffer[59]);
            FreqData[1]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"B相频率: "<<FreqData[1];
            float VB = FreqData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); // 保留两位小数
            ui->lineEdit_FB->setText(textB);
            //C相
            FreqData_tmp[0] = convertCharToHex(buffer[62]);
            FreqData_tmp[1] = convertCharToHex(buffer[63]);
            FreqData_tmp[2] = convertCharToHex(buffer[64]);
            FreqData_tmp[3] = convertCharToHex(buffer[65]);
            FreqData[2]     = FreqData_tmp[0]*4096+FreqData_tmp[1]*256+FreqData_tmp[2]*16+FreqData_tmp[3];
            qDebug()<<"C相频率: "<<FreqData[2];
            float VC = FreqData[2]/100.0;
            QString textC = QString::number(VC, 'f', 2); // 保留两位小数
            ui->lineEdit_FC->setText(textC);
        }
    }
    else if(isReadMACSend)
    {
        isReadMACSend = false;
        QChar firstChar;
        QChar SecondChar;
        if(!RecData_tmp.isEmpty())
        {
            firstChar = RecData_tmp.at(0);
            SecondChar = RecData_tmp.at(1);
        }
        else{}

        if(('F'==firstChar && 'E'==SecondChar) || ('f'==firstChar && 'e'==SecondChar))
        {
            QString str = RecData_tmp.mid(56, 12);
            ui->lineEdit_MAC->setText(str);
        }
        else if('6'==firstChar && '8'==SecondChar)
        {
            QString str = RecData_tmp.mid(48, 12);
            ui->lineEdit_MAC->setText(str);
        }
    }
    else if(isReadAssetSend)
    {
        isReadAssetSend = false;
        QChar firstChar;
        QChar SecondChar;
        quint8 i = 0;
        if(!RecData_tmp.isEmpty())
        {
            firstChar = RecData_tmp.at(0);
            SecondChar = RecData_tmp.at(1);
        }
        else{}
        qDebug()<<"read AssetCode: "<<firstChar;
        if(('F'==firstChar && 'E'==SecondChar) || ('f'==firstChar && 'e'==SecondChar))
        {
            qDebug()<<"read AssetCode with FE: "<<RecData_tmp.at(57);
            QString str = RecData_tmp.at(57);
            for(i=2;i<44;i+=2)
            {
                str.append(RecData_tmp.at(57+i));
            }
            ui->lineEdit_AssetCode->setText(str);
        }
        else if('6'==firstChar && '8'==SecondChar)
        {
            QString str = RecData_tmp.at(49);
            for(i=2;i<44;i+=2)
            {
                str.append(RecData_tmp.at(49+i));
            }
            ui->lineEdit_AssetCode->setText(str);
        }
    }
    else if(isReadProtectBitSend)
    {
        isReadProtectBitSend = false;
        char buffer[128] = {0};
        quint8 bit_tmp[3] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            bit_tmp[0] = convertCharToHex(buffer[56]);
            bit_tmp[1] = convertCharToHex(buffer[57]);
            bit_tmp[2] = convertCharToHex(buffer[58]);

            qDebug()<<"bit0: "<<bit_tmp[0]<<"bit1: "<<bit_tmp[1]<<"bit2: "<<bit_tmp[2];

            if(bit_tmp[0] & 0x08) //失压保护开启
                ui->checkBox_LV->setChecked(true);
            else
                ui->checkBox_LV->setChecked(false);

            if(bit_tmp[0] & 0x04) //断相保护开启
                ui->checkBox_LP->setChecked(true);
            else
                ui->checkBox_LP->setChecked(false);

            if(bit_tmp[0] & 0x02) //欠压保护开启
                ui->checkBox_UV->setChecked(true);
            else
                ui->checkBox_UV->setChecked(false);

            if(bit_tmp[0] & 0x01) //过压保护开启
                ui->checkBox_OV->setChecked(true);
            else
                ui->checkBox_OV->setChecked(false);
//-----------------------------------------------------------------
            if(bit_tmp[1] & 0x08) //幅值孤岛保护开启
                ui->checkBox_IslandVolt->setChecked(true);
            else
                ui->checkBox_IslandVolt->setChecked(false);

            if(bit_tmp[1] & 0x04) //频率孤岛保护开启
                ui->checkBox_IslandFreq->setChecked(true);
            else
                ui->checkBox_IslandFreq->setChecked(false);

            if(bit_tmp[1] & 0x02) //孤岛保护与或关系
                ui->checkBox_IslandAndOr->setChecked(true);
            else
                ui->checkBox_IslandAndOr->setChecked(false);

            if(bit_tmp[1] & 0x01) //自锁开启
                ui->checkBox_SelfLock->setChecked(true);
            else
                ui->checkBox_SelfLock->setChecked(false);
//--------------------------------------------------------------------
            if(bit_tmp[2] & 0x08) //孤岛保护与或关系
                ui->checkBox_FreqProtect->setChecked(true);
            else
                ui->checkBox_FreqProtect->setChecked(false);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            bit_tmp[0] = convertCharToHex(buffer[48]);
            bit_tmp[1] = convertCharToHex(buffer[49]);
            bit_tmp[2] = convertCharToHex(buffer[50]);

            qDebug()<<"bit0: "<<bit_tmp[0]<<"bit1: "<<bit_tmp[1]<<"bit2: "<<bit_tmp[2];

            if(bit_tmp[0] & 0x08) //失压保护开启
                ui->checkBox_LV->setChecked(true);
            else
                ui->checkBox_LV->setChecked(false);

            if(bit_tmp[0] & 0x04) //断相保护开启
                ui->checkBox_LP->setChecked(true);
            else
                ui->checkBox_LP->setChecked(false);

            if(bit_tmp[0] & 0x02) //欠压保护开启
                ui->checkBox_UV->setChecked(true);
            else
                ui->checkBox_UV->setChecked(false);

            if(bit_tmp[0] & 0x01) //过压保护开启
                ui->checkBox_OV->setChecked(true);
            else
                ui->checkBox_OV->setChecked(false);
//-----------------------------------------------------------------
            if(bit_tmp[1] & 0x08) //幅值孤岛保护开启
                ui->checkBox_IslandVolt->setChecked(true);
            else
                ui->checkBox_IslandVolt->setChecked(false);

            if(bit_tmp[1] & 0x04) //频率孤岛保护开启
                ui->checkBox_IslandFreq->setChecked(true);
            else
                ui->checkBox_IslandFreq->setChecked(false);

            if(bit_tmp[1] & 0x02) //孤岛保护与或关系
                ui->checkBox_IslandAndOr->setChecked(true);
            else
                ui->checkBox_IslandAndOr->setChecked(false);

            if(bit_tmp[1] & 0x01) //自锁开启
                ui->checkBox_SelfLock->setChecked(true);
            else
                ui->checkBox_SelfLock->setChecked(false);
//--------------------------------------------------------------------
            if(bit_tmp[2] & 0x08) //孤岛保护与或关系
                ui->checkBox_FreqProtect->setChecked(true);
            else
                ui->checkBox_FreqProtect->setChecked(false);
        }

    }
    else if(isWriteProtectBitSend)
    {
        isWriteProtectBitSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "保护位设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "保护位设置失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "保护位设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "保护位设置失败");
            }
        }
    }
    else if(isWriteMACSend)
    {
        isWriteMACSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "新地址写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "新地址写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "新地址写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "新地址写入失败");
            }
        }
    }
    else if(isWriteAssetSend)
    {
        isWriteAssetSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "新资产码写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "新资产码写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "新资产码写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "新资产码写入失败");
            }
        }
    }
    else if(isReadOVParaSend)
    {
        isReadOVParaSend = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //过压阈值
            VoltData_tmp[0] = convertCharToHex(buffer[58]);
            VoltData_tmp[1] = convertCharToHex(buffer[59]);
            VoltData_tmp[2] = convertCharToHex(buffer[60]);
            VoltData_tmp[3] = convertCharToHex(buffer[61]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_OV_Thres->setText(textA);
            //过压动作时间s
            VoltData_tmp[0] = convertCharToHex(buffer[64]);
            VoltData_tmp[1] = convertCharToHex(buffer[65]);
            VoltData_tmp[2] = convertCharToHex(buffer[66]);
            VoltData_tmp[3] = convertCharToHex(buffer[67]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/1.0;
            QString textB = QString::number(VB, 'f', 0); //无小数
            ui->lineEdit_OV_delay->setText(textB);
            //过压恢复阈值
            VoltData_tmp[0] = convertCharToHex(buffer[70]);
            VoltData_tmp[1] = convertCharToHex(buffer[71]);
            VoltData_tmp[2] = convertCharToHex(buffer[72]);
            VoltData_tmp[3] = convertCharToHex(buffer[73]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_OV_recover->setText(textC);
            //过压恢复延时
            VoltData_tmp[0] = convertCharToHex(buffer[76]);
            VoltData_tmp[1] = convertCharToHex(buffer[77]);
            VoltData_tmp[2] = convertCharToHex(buffer[78]);
            VoltData_tmp[3] = convertCharToHex(buffer[79]);
            VoltData[3]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压恢复时间: "<<VoltData[3];
            float VD = VoltData[3]/1.0;
            QString textD = QString::number(VD, 'f', 0); // 无小数
            ui->lineEdit_OV_recover_delay->setText(textD);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //过压阈值
            VoltData_tmp[0] = convertCharToHex(buffer[50]);
            VoltData_tmp[1] = convertCharToHex(buffer[51]);
            VoltData_tmp[2] = convertCharToHex(buffer[52]);
            VoltData_tmp[3] = convertCharToHex(buffer[53]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_OV_Thres->setText(textA);
            //过压动作时间s
            VoltData_tmp[0] = convertCharToHex(buffer[56]);
            VoltData_tmp[1] = convertCharToHex(buffer[57]);
            VoltData_tmp[2] = convertCharToHex(buffer[58]);
            VoltData_tmp[3] = convertCharToHex(buffer[59]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/1.0;
            QString textB = QString::number(VB, 'f', 0); //无小数
            ui->lineEdit_OV_delay->setText(textB);
            //过压恢复阈值
            VoltData_tmp[0] = convertCharToHex(buffer[62]);
            VoltData_tmp[1] = convertCharToHex(buffer[63]);
            VoltData_tmp[2] = convertCharToHex(buffer[64]);
            VoltData_tmp[3] = convertCharToHex(buffer[65]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_OV_recover->setText(textC);
            //过压恢复延时
            VoltData_tmp[0] = convertCharToHex(buffer[68]);
            VoltData_tmp[1] = convertCharToHex(buffer[69]);
            VoltData_tmp[2] = convertCharToHex(buffer[70]);
            VoltData_tmp[3] = convertCharToHex(buffer[71]);
            VoltData[3]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"过压恢复时间: "<<VoltData[3];
            float VD = VoltData[3]/1.0;
            QString textD = QString::number(VD, 'f', 0); // 无小数
            ui->lineEdit_OV_recover_delay->setText(textD);
        }
    }
    else if(isReadUVParaSend)
    {
        isReadUVParaSend = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //欠压阈值
            VoltData_tmp[0] = convertCharToHex(buffer[58]);
            VoltData_tmp[1] = convertCharToHex(buffer[59]);
            VoltData_tmp[2] = convertCharToHex(buffer[60]);
            VoltData_tmp[3] = convertCharToHex(buffer[61]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_UV_Thres->setText(textA);
            //欠压动作时间s
            VoltData_tmp[0] = convertCharToHex(buffer[64]);
            VoltData_tmp[1] = convertCharToHex(buffer[65]);
            VoltData_tmp[2] = convertCharToHex(buffer[66]);
            VoltData_tmp[3] = convertCharToHex(buffer[67]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/1.0;
            QString textB = QString::number(VB, 'f', 0); //无小数
            ui->lineEdit_UV_delay->setText(textB);
            //欠压恢复阈值
            VoltData_tmp[0] = convertCharToHex(buffer[70]);
            VoltData_tmp[1] = convertCharToHex(buffer[71]);
            VoltData_tmp[2] = convertCharToHex(buffer[72]);
            VoltData_tmp[3] = convertCharToHex(buffer[73]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_UV_recover->setText(textC);
            //欠压恢复延时
            VoltData_tmp[0] = convertCharToHex(buffer[76]);
            VoltData_tmp[1] = convertCharToHex(buffer[77]);
            VoltData_tmp[2] = convertCharToHex(buffer[78]);
            VoltData_tmp[3] = convertCharToHex(buffer[79]);
            VoltData[3]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复时间: "<<VoltData[3];
            float VD = VoltData[3]/1.0;
            QString textD = QString::number(VD, 'f', 0); // 无小数
            ui->lineEdit_UV_recover_delay->setText(textD);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //欠压阈值
            VoltData_tmp[0] = convertCharToHex(buffer[50]);
            VoltData_tmp[1] = convertCharToHex(buffer[51]);
            VoltData_tmp[2] = convertCharToHex(buffer[52]);
            VoltData_tmp[3] = convertCharToHex(buffer[53]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_UV_Thres->setText(textA);
            //欠压动作时间s
            VoltData_tmp[0] = convertCharToHex(buffer[56]);
            VoltData_tmp[1] = convertCharToHex(buffer[57]);
            VoltData_tmp[2] = convertCharToHex(buffer[58]);
            VoltData_tmp[3] = convertCharToHex(buffer[59]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/1.0;
            QString textB = QString::number(VB, 'f', 0); //无小数
            ui->lineEdit_UV_delay->setText(textB);
            //欠压恢复阈值
            VoltData_tmp[0] = convertCharToHex(buffer[62]);
            VoltData_tmp[1] = convertCharToHex(buffer[63]);
            VoltData_tmp[2] = convertCharToHex(buffer[64]);
            VoltData_tmp[3] = convertCharToHex(buffer[65]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_UV_recover->setText(textC);
            //欠压恢复延时
            VoltData_tmp[0] = convertCharToHex(buffer[68]);
            VoltData_tmp[1] = convertCharToHex(buffer[69]);
            VoltData_tmp[2] = convertCharToHex(buffer[70]);
            VoltData_tmp[3] = convertCharToHex(buffer[71]);
            VoltData[3]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复时间: "<<VoltData[3];
            float VD = VoltData[3]/1.0;
            QString textD = QString::number(VD, 'f', 0); // 无小数
            ui->lineEdit_UV_recover_delay->setText(textD);
        }
    }
    else if(isReadIslandParaSend)
    {
        isReadIslandParaSend = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[3] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //电压摆动幅值
            VoltData_tmp[0] = convertCharToHex(buffer[58]);
            VoltData_tmp[1] = convertCharToHex(buffer[59]);
            VoltData_tmp[2] = convertCharToHex(buffer[60]);
            VoltData_tmp[3] = convertCharToHex(buffer[61]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwing->setText(textA);
            //频率摆动幅值
            VoltData_tmp[0] = convertCharToHex(buffer[64]);
            VoltData_tmp[1] = convertCharToHex(buffer[65]);
            VoltData_tmp[2] = convertCharToHex(buffer[66]);
            VoltData_tmp[3] = convertCharToHex(buffer[67]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); //保留两位小数
            ui->lineEdit_IslandFreqSwing->setText(textB);
            //告警延时ms
            VoltData_tmp[0] = convertCharToHex(buffer[70]);
            VoltData_tmp[1] = convertCharToHex(buffer[71]);
            VoltData_tmp[2] = convertCharToHex(buffer[72]);
            VoltData_tmp[3] = convertCharToHex(buffer[73]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/1.0;
            QString textC = QString::number(VC, 'f', 0); // 无小数
            ui->lineEdit_Island_delay->setText(textC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //电压摆动幅值
            VoltData_tmp[0] = convertCharToHex(buffer[50]);
            VoltData_tmp[1] = convertCharToHex(buffer[51]);
            VoltData_tmp[2] = convertCharToHex(buffer[52]);
            VoltData_tmp[3] = convertCharToHex(buffer[53]);
            VoltData[0]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压阈值: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwing->setText(textA);
            //频率摆动幅值
            VoltData_tmp[0] = convertCharToHex(buffer[56]);
            VoltData_tmp[1] = convertCharToHex(buffer[57]);
            VoltData_tmp[2] = convertCharToHex(buffer[58]);
            VoltData_tmp[3] = convertCharToHex(buffer[59]);
            VoltData[1]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压动作时间: "<<VoltData[1];
            float VB = VoltData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); //保留两位小数
            ui->lineEdit_IslandFreqSwing->setText(textB);
            //告警延时ms
            VoltData_tmp[0] = convertCharToHex(buffer[62]);
            VoltData_tmp[1] = convertCharToHex(buffer[63]);
            VoltData_tmp[2] = convertCharToHex(buffer[64]);
            VoltData_tmp[3] = convertCharToHex(buffer[65]);
            VoltData[2]     = VoltData_tmp[0]*4096+VoltData_tmp[1]*256+VoltData_tmp[2]*16+VoltData_tmp[3];
            qDebug()<<"欠压恢复阈值: "<<VoltData[2];
            float VC = VoltData[2]/1.0;
            QString textC = QString::number(VC, 'f', 0); // 无小数
            ui->lineEdit_Island_delay->setText(textC);
        }
    }
    else if(isReadDeviceInfoSend)
    {
        isReadDeviceInfoSend = false;

        QChar firstChar;
        QChar SecondChar;

        QChar VersionChar1;
        QChar VersionChar2;
        QChar VersionChar3;
        QChar VersionChar4;
        QChar VersionChar5;
        QChar VersionChar6;

        if(!RecData_tmp.isEmpty())
        {
            firstChar = RecData_tmp.at(0);
            SecondChar = RecData_tmp.at(1);
        }
        else{}

        if(('F'==firstChar && 'E'==SecondChar) || ('f'==firstChar && 'e'==SecondChar))
        {
            QString str;
            VersionChar1 = RecData_tmp.at(73); str.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(75); str.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(77); str.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(79); str.append(VersionChar4);
            ui->lineEdit_SoftwareVersion->setText(str);

            QString str2;
            VersionChar1 = RecData_tmp.at(85); str2.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(87); str2.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(89); str2.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(91); str2.append(VersionChar4);
            VersionChar5 = RecData_tmp.at(93); str2.append(VersionChar5);
            VersionChar6 = RecData_tmp.at(95); str2.append(VersionChar6);
            ui->lineEdit_SoftwareDate->setText(str2);

            QString str3;
            VersionChar1 = RecData_tmp.at(101); str3.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(103); str3.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(105); str3.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(107); str3.append(VersionChar4);
            ui->lineEdit_HardwareVersion->setText(str3);

            QString str4;
            VersionChar1 = RecData_tmp.at(113); str4.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(115); str4.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(117); str4.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(119); str4.append(VersionChar4);
            VersionChar5 = RecData_tmp.at(121); str4.append(VersionChar5);
            VersionChar6 = RecData_tmp.at(123); str4.append(VersionChar6);
            ui->lineEdit_HardwareDate->setText(str4);

            QChar qch_tmp1 = RecData_tmp.at(126);
            QChar qch_tmp2 = RecData_tmp.at(127);
            char ch_tmp1 = qch_tmp1.toLatin1();
            char ch_tmp2 = qch_tmp2.toLatin1();
            quint8 ExtendInfo_len = convertCharToHex(ch_tmp1)*16+convertCharToHex(ch_tmp2)-1; //去除‘\0’
            //quint8 ascii_collect[ExtendInfo_len*2] = {0};
            quint8 ascii_collect[128] = {0};
            QString str5;
            for(quint8 i=0;i<ExtendInfo_len*2;i+=2)
            {
                qch_tmp1 = RecData_tmp.at(128+i);
                qch_tmp2 = RecData_tmp.at(129+i);
                ch_tmp1 = qch_tmp1.toLatin1();
                ch_tmp2 = qch_tmp2.toLatin1();
                ascii_collect[i] = convertCharToHex(ch_tmp1)*16+convertCharToHex(ch_tmp2);
                QChar ch(ascii_collect[i]);
                str5.append(ch);
            }
            ui->lineEdit_ManufacturerExtend->setText(str5);

        }
        else if('6'==firstChar && '8'==SecondChar)
        {
            QString str;
            VersionChar1 = RecData_tmp.at(65); str.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(67); str.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(69); str.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(71); str.append(VersionChar4);
            ui->lineEdit_SoftwareVersion->setText(str);

            QString str2;
            VersionChar1 = RecData_tmp.at(77); str2.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(79); str2.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(81); str2.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(83); str2.append(VersionChar4);
            VersionChar5 = RecData_tmp.at(85); str2.append(VersionChar5);
            VersionChar6 = RecData_tmp.at(87); str2.append(VersionChar6);
            ui->lineEdit_SoftwareDate->setText(str2);

            QString str3;
            VersionChar1 = RecData_tmp.at(93); str3.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(95); str3.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(97); str3.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(99); str3.append(VersionChar4);
            ui->lineEdit_HardwareVersion->setText(str3);

            QString str4;
            VersionChar1 = RecData_tmp.at(105); str4.append(VersionChar1);
            VersionChar2 = RecData_tmp.at(107); str4.append(VersionChar2);
            VersionChar3 = RecData_tmp.at(109); str4.append(VersionChar3);
            VersionChar4 = RecData_tmp.at(111); str4.append(VersionChar4);
            VersionChar5 = RecData_tmp.at(113); str4.append(VersionChar5);
            VersionChar6 = RecData_tmp.at(115); str4.append(VersionChar6);
            ui->lineEdit_HardwareDate->setText(str4);

            QChar qch_tmp1 = RecData_tmp.at(118);
            QChar qch_tmp2 = RecData_tmp.at(119);
            char ch_tmp1 = qch_tmp1.toLatin1();
            char ch_tmp2 = qch_tmp2.toLatin1();
            quint8 ExtendInfo_len = convertCharToHex(ch_tmp1)*16+convertCharToHex(ch_tmp2)-1; //去除‘\0’
//            quint8 ascii_collect[ExtendInfo_len*2] = {0};
            quint8 ascii_collect[128] = {0};
            QString str5;
            for(quint8 i=0;i<ExtendInfo_len*2;i+=2)
            {
                qch_tmp1 = RecData_tmp.at(120+i);
                qch_tmp2 = RecData_tmp.at(121+i);
                ch_tmp1 = qch_tmp1.toLatin1();
                ch_tmp2 = qch_tmp2.toLatin1();
                ascii_collect[i] = convertCharToHex(ch_tmp1)*16+convertCharToHex(ch_tmp2);
                QChar ch(ascii_collect[i]);
                str5.append(ch);
            }
            ui->lineEdit_ManufacturerExtend->setText(str5);
        }
    }
    else if(isWriteOVParaSend)
    {
        isWriteOVParaSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "过压参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "过压参数写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "过压参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "过压参数写入失败");
            }
        }
    }
    else if(isWriteUVParaSend)
    {
        isWriteUVParaSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "欠压参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "欠压参数写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "欠压参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "欠压参数写入失败");
            }
        }
    }
    else if(isWriteIslandParaSend)
    {
        isWriteIslandParaSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "孤岛参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "孤岛参数写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "孤岛参数写入成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "孤岛参数写入失败");
            }
        }
    }
    else if(isWriteRS485ParaSend)
    {
        isWriteRS485ParaSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "串口参数写入成功，请关闭串口重新设置后打开使用");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "串口参数写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "串口参数写入成功，请关闭串口重新设置后打开使用");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "串口参数写入失败");
            }
        }
    }
    else if(isRemoteOpenCmdSend)
    {
        isRemoteOpenCmdSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程分闸成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程分闸失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程分闸成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程分闸失败");
            }
        }
    }
    else if(isRemoteCloseAllowedCmdSend)
    {
        isRemoteCloseAllowedCmdSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程合闸允许成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程合闸允许失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程合闸允许成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程合闸允许失败");
            }
        }

    }
    else if(isRemoteCloseCmdSend)
    {
        isRemoteCloseCmdSend=false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程合闸成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程合闸失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "远程合闸成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "远程合闸失败");
            }
        }
    }
    else if(isCalibrationCmdSend)
    {
        isCalibrationCmdSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "698校准成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "校准失败，请检查输入电压值");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "698校准成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "校准失败，请检查输入电压值");
            }
        }
    }
    else if(isFCTcmdSend)
    {
        isFCTcmdSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[51]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "进入FCT模式成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "进入FCT模式失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[43]);
            if(0==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Set Message", "进入FCT模式成功");
            }
            else
            {
                QMessageBox::critical(this,"Set Message", "进入FCT模式失败");
            }
        }
    }
    else if(isReadRunWordCmdSend)
    {
        isReadRunWordCmdSend = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 RunWord1,RunWord2;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            QString str;
            str.append(RecData_tmp.at(56));
            str.append(RecData_tmp.at(57));
            ui->lineEdit_RunWord->setText(str);

            RunWord1 = convertCharToHex(buffer[56]);
            RunWord2 = convertCharToHex(buffer[57]);

            quint8 bit_value = RunWord2 & 1;//bit7
            if(0 == bit_value)
                ui->lineEdit_StatusSelfLock->setText("正常");
            else
                ui->lineEdit_StatusSelfLock->setText("自锁");

            bit_value = (RunWord2>>1) & 1;//bit6
            if(0 == bit_value)
                ui->lineEdit_statusBattery->setText("正常");
            else
                ui->lineEdit_statusBattery->setText("故障");

            bit_value = (RunWord2>>2) & 1;//bit5
            if(0 == bit_value)
                ui->lineEdit_statusMeg->setText("正常");
            else
                ui->lineEdit_statusMeg->setText("保护");

            bit_value = (RunWord2>>3) & 1;//bit4
            if(0 == bit_value)
                ui->lineEdit_statusAM->setText("手动");
            else
                ui->lineEdit_statusAM->setText("自动");
            //------------------------------------------------------------
            bit_value = RunWord1 & 1;//bit3
            if(0 == bit_value)
                ui->lineEdit_statusAction->setText("非用户行为");
            else
                ui->lineEdit_statusAction->setText("用户行为");

            bit_value = (RunWord1>>1) & 1;//bit2
            if(0 == bit_value)
                ui->lineEdit_statusCloseAllowed->setText("无效");
            else
                ui->lineEdit_statusCloseAllowed->setText("允许");

            bit_value = (RunWord1>>2) & 1;//bit1
            if(0 == bit_value)
                ui->lineEdit_statusRealstate->setText("合闸");
            else
                ui->lineEdit_statusRealstate->setText("分闸");

            bit_value = (RunWord1>>3) & 1;//bit0
            if(0 == bit_value)
                ui->lineEdit_statusCMDstate->setText("合闸");
            else
                ui->lineEdit_statusCMDstate->setText("分闸");

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            QString str;
            str.append(RecData_tmp.at(48));
            str.append(RecData_tmp.at(49));
            ui->lineEdit_RunWord->setText(str);

            RunWord1 = convertCharToHex(buffer[48]);
            RunWord2 = convertCharToHex(buffer[49]);

            quint8 bit_value = RunWord2 & 1;//bit7
            if(0 == bit_value)
                ui->lineEdit_StatusSelfLock->setText("正常");
            else
                ui->lineEdit_StatusSelfLock->setText("自锁");

            bit_value = (RunWord2>>1) & 1;//bit6
            if(0 == bit_value)
                ui->lineEdit_statusBattery->setText("正常");
            else
                ui->lineEdit_statusBattery->setText("故障");

            bit_value = (RunWord2>>2) & 1;//bit5
            if(0 == bit_value)
                ui->lineEdit_statusMeg->setText("正常");
            else
                ui->lineEdit_statusMeg->setText("保护");

            bit_value = (RunWord2>>3) & 1;//bit4
            if(0 == bit_value)
                ui->lineEdit_statusAM->setText("手动");
            else
                ui->lineEdit_statusAM->setText("自动");
            //------------------------------------------------------------
            bit_value = RunWord1 & 1;//bit3
            if(0 == bit_value)
                ui->lineEdit_statusAction->setText("非用户行为");
            else
                ui->lineEdit_statusAction->setText("用户行为");

            bit_value = (RunWord1>>1) & 1;//bit2
            if(0 == bit_value)
                ui->lineEdit_statusCloseAllowed->setText("无效");
            else
                ui->lineEdit_statusCloseAllowed->setText("允许");

            bit_value = (RunWord1>>2) & 1;//bit1
            if(0 == bit_value)
                ui->lineEdit_statusRealstate->setText("合闸");
            else
                ui->lineEdit_statusRealstate->setText("分闸");

            bit_value = (RunWord1>>3) & 1;//bit0
            if(0 == bit_value)
                ui->lineEdit_statusCMDstate->setText("合闸");
            else
                ui->lineEdit_statusCMDstate->setText("分闸");
        }
    }
    else if(isReadAddr645_Send)
    {
        isReadAddr645_Send = false;

        quint8 char2hex[12];
//        char hex_tmp[6] = {0};
        QChar firstChar;
        QChar SecondChar;
        if(!RecData_tmp.isEmpty())
        {
            firstChar = RecData_tmp.at(0);
            SecondChar = RecData_tmp.at(1);
        }
        else{}

        if(('F'==firstChar && 'E'==SecondChar) || ('f'==firstChar && 'e'==SecondChar))
        {
            char2hex[0]=convertCharToHex(RecData_tmp.at(28).toLatin1());
            char2hex[1]=convertCharToHex(RecData_tmp.at(29).toLatin1());
            MAC645[5] = char2hex[0]*16+char2hex[1]-0x33;
            char2hex[2]=convertCharToHex(RecData_tmp.at(30).toLatin1());
            char2hex[3]=convertCharToHex(RecData_tmp.at(31).toLatin1());
            MAC645[4] = char2hex[2]*16+char2hex[3]-0x33;
            char2hex[4]=convertCharToHex(RecData_tmp.at(32).toLatin1());
            char2hex[5]=convertCharToHex(RecData_tmp.at(33).toLatin1());
            MAC645[3] = char2hex[4]*16+char2hex[5]-0x33;

            char2hex[6]=convertCharToHex(RecData_tmp.at(34).toLatin1());
            char2hex[7]=convertCharToHex(RecData_tmp.at(35).toLatin1());
            MAC645[2] = char2hex[6]*16+char2hex[7]-0x33;
            char2hex[8]=convertCharToHex(RecData_tmp.at(36).toLatin1());
            char2hex[9]=convertCharToHex(RecData_tmp.at(37).toLatin1());
            MAC645[1] = char2hex[8]*16+char2hex[9]-0x33;
            char2hex[10]=convertCharToHex(RecData_tmp.at(38).toLatin1());
            char2hex[11]=convertCharToHex(RecData_tmp.at(39).toLatin1());
            MAC645[0] = char2hex[10]*16+char2hex[11]-0x33;
//            HexPrint(MAC645, sizeof(MAC645));
            QByteArray data((const char*)MAC645, sizeof(MAC645));
            QString str;
            for (int i = 0; i < data.size(); i++) {
                str += QString("%1").arg((quint8)data.at(i), 2, 16, QChar('0'));
            }
            ui->lineEdit_ReadAddr645->setText(str);
        }
        else if('6'==firstChar && '8'==SecondChar)
        {
            char2hex[0]=convertCharToHex(RecData_tmp.at(20).toLatin1());
            char2hex[1]=convertCharToHex(RecData_tmp.at(21).toLatin1());
            MAC645[5] = char2hex[0]*16+char2hex[1]-0x33;
            char2hex[2]=convertCharToHex(RecData_tmp.at(22).toLatin1());
            char2hex[3]=convertCharToHex(RecData_tmp.at(23).toLatin1());
            MAC645[4] = char2hex[2]*16+char2hex[3]-0x33;
            char2hex[4]=convertCharToHex(RecData_tmp.at(24).toLatin1());
            char2hex[5]=convertCharToHex(RecData_tmp.at(25).toLatin1());
            MAC645[3] = char2hex[4]*16+char2hex[5]-0x33;

            char2hex[6]=convertCharToHex(RecData_tmp.at(26).toLatin1());
            char2hex[7]=convertCharToHex(RecData_tmp.at(27).toLatin1());
            MAC645[2] = char2hex[6]*16+char2hex[7]-0x33;
            char2hex[8]=convertCharToHex(RecData_tmp.at(28).toLatin1());
            char2hex[9]=convertCharToHex(RecData_tmp.at(29).toLatin1());
            MAC645[1] = char2hex[8]*16+char2hex[9]-0x33;
            char2hex[10]=convertCharToHex(RecData_tmp.at(30).toLatin1());
            char2hex[11]=convertCharToHex(RecData_tmp.at(31).toLatin1());
            MAC645[0] = char2hex[10]*16+char2hex[11]-0x33;
//            HexPrint(MAC645, sizeof(MAC645));
            QByteArray data((const char*)MAC645, sizeof(MAC645));
            QString str;
            for (int i = 0; i < data.size(); i++) {
                str += QString("%1").arg((quint8)data.at(i), 2, 16, QChar('0'));
            }
            ui->lineEdit_ReadAddr645->setText(str);
        }
    }
    else if(isManualRead645_Send)
    {
        isManualRead645_Send = false;
    }
    else if(isWriteMACSend645)
    {
        isWriteMACSend645 = false;
        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"SetMAC645 Message", "645新地址写入成功");
            }
            else
            {
                QMessageBox::critical(this,"SetMAC645 Message", "645新地址写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"SetMAC645 Message", "645新地址写入成功");
            }
            else
            {
                QMessageBox::critical(this,"SetMAC645 Message", "645新地址写入失败");
            }
        }
    }
    else if(isChangeBaudrate645)
    {
        isChangeBaudrate645 = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ChangeBaudrate645 Message", "新波特率写入成功，请关闭串口并切换到新波特率，再打开串口");
            }
            else
            {
                QMessageBox::critical(this,"ChangeBaudrate645 Message", "新波特率写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"SetMAC645 Message", "新波特率写入成功，请关闭串口并切换到新波特率，再打开串口");
            }
            else
            {
                QMessageBox::critical(this,"SetMAC645 Message", "新波特率写入失败");
            }
        }
    }
    else if(isReadVolt645)
    {
        isReadVolt645 = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[3] = {0};//三相电压值

        QByteArray byteArray = RecData_tmp.toUtf8();

//        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
//        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[36]);
            VoltData_tmp[1] = convertCharToHex(buffer[37]);
            VoltData_tmp[2] = convertCharToHex(buffer[38]);
            VoltData_tmp[3] = convertCharToHex(buffer[39]);
            VoltData[0]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"A相电压: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltA645->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[40]);
            VoltData_tmp[1] = convertCharToHex(buffer[41]);
            VoltData_tmp[2] = convertCharToHex(buffer[42]);
            VoltData_tmp[3] = convertCharToHex(buffer[43]);
            VoltData[1]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"B相电压: "<<VoltData[1];
            float VB = VoltData[1]/10.0;
            QString textB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltB645->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[44]);
            VoltData_tmp[1] = convertCharToHex(buffer[45]);
            VoltData_tmp[2] = convertCharToHex(buffer[46]);
            VoltData_tmp[3] = convertCharToHex(buffer[47]);
            VoltData[2]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"C相电压: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltC645->setText(textC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[28]);
            VoltData_tmp[1] = convertCharToHex(buffer[29]);
            VoltData_tmp[2] = convertCharToHex(buffer[30]);
            VoltData_tmp[3] = convertCharToHex(buffer[31]);
            VoltData[0]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"A相电压: "<<VoltData[0];
            float VA = VoltData[0]/10.0;
            QString textA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltA645->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[32]);
            VoltData_tmp[1] = convertCharToHex(buffer[33]);
            VoltData_tmp[2] = convertCharToHex(buffer[34]);
            VoltData_tmp[3] = convertCharToHex(buffer[35]);
            VoltData[1]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"B相电压: "<<VoltData[1];
            float VB = VoltData[1]/10.0;
            QString textB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltB645->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[36]);
            VoltData_tmp[1] = convertCharToHex(buffer[37]);
            VoltData_tmp[2] = convertCharToHex(buffer[38]);
            VoltData_tmp[3] = convertCharToHex(buffer[39]);
            VoltData[2]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"C相电压: "<<VoltData[2];
            float VC = VoltData[2]/10.0;
            QString textC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_VoltC645->setText(textC);
        }
    }
    else if(isReadFreq645)
    {
        isReadFreq645 = false;

        char buffer[128] = {0};
        quint8 VoltData_tmp[4] = {0};
        quint16 VoltData[3] = {0};//三相电压值

        QByteArray byteArray = RecData_tmp.toUtf8();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[36]);
            VoltData_tmp[1] = convertCharToHex(buffer[37]);
            VoltData_tmp[2] = convertCharToHex(buffer[38]);
            VoltData_tmp[3] = convertCharToHex(buffer[39]);
            VoltData[0]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"A相频率: "<<VoltData[0];
            float VA = VoltData[0]/100.0;
            QString textA = QString::number(VA, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqA645->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[40]);
            VoltData_tmp[1] = convertCharToHex(buffer[41]);
            VoltData_tmp[2] = convertCharToHex(buffer[42]);
            VoltData_tmp[3] = convertCharToHex(buffer[43]);
            VoltData[1]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"B相频率: "<<VoltData[1];
            float VB = VoltData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqB645->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[44]);
            VoltData_tmp[1] = convertCharToHex(buffer[45]);
            VoltData_tmp[2] = convertCharToHex(buffer[46]);
            VoltData_tmp[3] = convertCharToHex(buffer[47]);
            VoltData[2]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"C相频率: "<<VoltData[2];
            float VC = VoltData[2]/100.0;
            QString textC = QString::number(VC, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqC645->setText(textC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //A相
            VoltData_tmp[0] = convertCharToHex(buffer[28]);
            VoltData_tmp[1] = convertCharToHex(buffer[29]);
            VoltData_tmp[2] = convertCharToHex(buffer[30]);
            VoltData_tmp[3] = convertCharToHex(buffer[31]);
            VoltData[0]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"A相频率: "<<VoltData[0];
            float VA = VoltData[0]/100.0;
            QString textA = QString::number(VA, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqA645->setText(textA);
            //B相
            VoltData_tmp[0] = convertCharToHex(buffer[32]);
            VoltData_tmp[1] = convertCharToHex(buffer[33]);
            VoltData_tmp[2] = convertCharToHex(buffer[34]);
            VoltData_tmp[3] = convertCharToHex(buffer[35]);
            VoltData[1]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"B相频率: "<<VoltData[1];
            float VB = VoltData[1]/100.0;
            QString textB = QString::number(VB, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqB645->setText(textB);
            //C相
            VoltData_tmp[0] = convertCharToHex(buffer[36]);
            VoltData_tmp[1] = convertCharToHex(buffer[37]);
            VoltData_tmp[2] = convertCharToHex(buffer[38]);
            VoltData_tmp[3] = convertCharToHex(buffer[39]);
            VoltData[2]     = BcdToHex(VoltData_tmp[2]*16+VoltData_tmp[3]-0x33)*100+BcdToHex(VoltData_tmp[0]*16+VoltData_tmp[1] - 0x33);
            qDebug()<<"C相频率: "<<VoltData[2];
            float VC = VoltData[2]/100.0;
            QString textC = QString::number(VC, 'f', 2); // 保留2位小数
            ui->lineEdit_FreqC645->setText(textC);
        }
    }
    else if(isChangePWD645)
    {
        isChangePWD645 = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ChangePWD Message", "新密码设置成功");
            }
            else
            {
                QMessageBox::critical(this,"ChangePWD Message", "新密码设置失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ChangePWD Message", "新密码设置成功");
            }
            else
            {
                QMessageBox::critical(this,"ChangePWD Message", "新密码设置失败");
            }
        }
    }
    else if(isReadDateTime645)
    {
        isReadDateTime645 = false;

        char buffer[128] = {0};
        quint8 DateTime_tmp[14] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            //年
            DateTime_tmp[0] = convertCharToHex(buffer[48]);
            DateTime_tmp[1] = convertCharToHex(buffer[49]);
            quint16 year =BcdToHex(DateTime_tmp[0]*16+DateTime_tmp[1]-0x33) + 2000;
            QString text_year = QString::number(year);
            //月
            DateTime_tmp[2] = convertCharToHex(buffer[46]);
            DateTime_tmp[3] = convertCharToHex(buffer[47]);
            quint16 month =BcdToHex(DateTime_tmp[2]*16+DateTime_tmp[3]-0x33);
            QString text_month = QString::number(month);
            //日
            DateTime_tmp[4] = convertCharToHex(buffer[44]);
            DateTime_tmp[5] = convertCharToHex(buffer[45]);
            quint16 day =BcdToHex(DateTime_tmp[4]*16+DateTime_tmp[5]-0x33);
            QString text_day = QString::number(day);
            //周
            DateTime_tmp[6] = convertCharToHex(buffer[42]);
            DateTime_tmp[7] = convertCharToHex(buffer[43]);
            quint16 week =BcdToHex(DateTime_tmp[6]*16+DateTime_tmp[7]-0x33);
            QString text_week = QString::number(week);
            //时
            DateTime_tmp[8] = convertCharToHex(buffer[40]);
            DateTime_tmp[9] = convertCharToHex(buffer[41]);
            quint16 hour =BcdToHex(DateTime_tmp[8]*16+DateTime_tmp[9]-0x33);
            QString text_hour = QString::number(hour);
            //分
            DateTime_tmp[10] = convertCharToHex(buffer[38]);
            DateTime_tmp[11] = convertCharToHex(buffer[39]);
            quint16 minu =BcdToHex(DateTime_tmp[10]*16+DateTime_tmp[11]-0x33);
            QString text_minu = QString::number(minu);
            //秒
            DateTime_tmp[12] = convertCharToHex(buffer[36]);
            DateTime_tmp[13] = convertCharToHex(buffer[37]);
            quint16 sec =BcdToHex(DateTime_tmp[12]*16+DateTime_tmp[13]-0x33);
            QString text_sec = QString::number(sec);

            QString out = text_year+"-"+text_month+"-"+text_day+"-"+text_week+" "+text_hour+":"+text_minu+":"+text_sec;
            ui->lineEdit_ReadDateTime->setText(out);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            //年
            DateTime_tmp[0] = convertCharToHex(buffer[40]);
            DateTime_tmp[1] = convertCharToHex(buffer[41]);
            quint16 year =BcdToHex(DateTime_tmp[0]*16+DateTime_tmp[1]-0x33) + 2000;
            QString text_year = QString::number(year);
            //月
            DateTime_tmp[2] = convertCharToHex(buffer[38]);
            DateTime_tmp[3] = convertCharToHex(buffer[39]);
            quint16 month =BcdToHex(DateTime_tmp[2]*16+DateTime_tmp[3]-0x33);
            QString text_month = QString::number(month);
            //日
            DateTime_tmp[4] = convertCharToHex(buffer[36]);
            DateTime_tmp[5] = convertCharToHex(buffer[37]);
            quint16 day =BcdToHex(DateTime_tmp[4]*16+DateTime_tmp[5]-0x33);
            QString text_day = QString::number(day);
            //周
            DateTime_tmp[6] = convertCharToHex(buffer[34]);
            DateTime_tmp[7] = convertCharToHex(buffer[35]);
            quint16 week =BcdToHex(DateTime_tmp[6]*16+DateTime_tmp[7]-0x33);
            QString text_week = QString::number(week);
            //时
            DateTime_tmp[8] = convertCharToHex(buffer[32]);
            DateTime_tmp[9] = convertCharToHex(buffer[33]);
            quint16 hour =BcdToHex(DateTime_tmp[8]*16+DateTime_tmp[9]-0x33);
            QString text_hour = QString::number(hour);
            //分
            DateTime_tmp[10] = convertCharToHex(buffer[30]);
            DateTime_tmp[11] = convertCharToHex(buffer[31]);
            quint16 minu =BcdToHex(DateTime_tmp[10]*16+DateTime_tmp[11]-0x33);
            QString text_minu = QString::number(minu);
            //秒
            DateTime_tmp[12] = convertCharToHex(buffer[28]);
            DateTime_tmp[13] = convertCharToHex(buffer[29]);
            quint16 sec =BcdToHex(DateTime_tmp[12]*16+DateTime_tmp[13]-0x33);
            QString text_sec = QString::number(sec);

            QString out = text_year+"-"+text_month+"-"+text_day+"-"+text_week+" "+text_hour+":"+text_minu+":"+text_sec;
            ui->lineEdit_ReadDateTime->setText(out);
        }
    }
    else if(isReadSoftwareVere645)
    {
        isReadSoftwareVere645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[46]);
            data_tmp[1] = convertCharToHex(buffer[47]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[44]);
            data_tmp[3] = convertCharToHex(buffer[45]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[38]);
            data_tmp[7] = convertCharToHex(buffer[39]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            QString SoftVer = "";
            SoftVer = SoftVer+v1+v2+"."+v3+v4;
            ui->lineEdit_SoftwareVer645->setText(SoftVer);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[38]);
            data_tmp[1] = convertCharToHex(buffer[39]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[36]);
            data_tmp[3] = convertCharToHex(buffer[37]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[32]);
            data_tmp[5] = convertCharToHex(buffer[33]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[30]);
            data_tmp[7] = convertCharToHex(buffer[31]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            QString SoftVer = "";
            SoftVer = SoftVer+v1+v2+"."+v3+v4;
            ui->lineEdit_SoftwareVer645->setText(SoftVer);
        }
    }
    else if(isReadHardwareVere645)
    {
        isReadHardwareVere645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[46]);
            data_tmp[1] = convertCharToHex(buffer[47]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[44]);
            data_tmp[3] = convertCharToHex(buffer[45]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[38]);
            data_tmp[7] = convertCharToHex(buffer[39]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            QString HardVer = "";
            HardVer = HardVer+v1+v2+"."+v3+v4;
            ui->lineEdit_HardwareVer645->setText(HardVer);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[38]);
            data_tmp[1] = convertCharToHex(buffer[39]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[36]);
            data_tmp[3] = convertCharToHex(buffer[37]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[32]);
            data_tmp[5] = convertCharToHex(buffer[33]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[30]);
            data_tmp[7] = convertCharToHex(buffer[31]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            QString HardVer = "";
            HardVer = HardVer+v1+v2+"."+v3+v4;
            ui->lineEdit_HardwareVer645->setText(HardVer);
        }
    }
    else if(isReadAssetCode645)
    {
        isReadAssetCode645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[44] = {0};//资产码22位

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[78]);
            data_tmp[1] = convertCharToHex(buffer[79]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[76]);
            data_tmp[3] = convertCharToHex(buffer[77]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[74]);
            data_tmp[5] = convertCharToHex(buffer[75]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[72]);
            data_tmp[7] = convertCharToHex(buffer[73]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[70]);
            data_tmp[9] = convertCharToHex(buffer[71]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[68]);
            data_tmp[11] = convertCharToHex(buffer[69]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            data_tmp[12] = convertCharToHex(buffer[66]);
            data_tmp[13] = convertCharToHex(buffer[67]);
            QChar v7 =data_tmp[12]*16+data_tmp[13]-0x33;

            data_tmp[14] = convertCharToHex(buffer[64]);
            data_tmp[15] = convertCharToHex(buffer[65]);
            QChar v8 =data_tmp[14]*16+data_tmp[15]-0x33;
            //------------------------------------------------------
            data_tmp[16] = convertCharToHex(buffer[62]);
            data_tmp[17] = convertCharToHex(buffer[63]);
            QChar v9 =data_tmp[16]*16+data_tmp[17]-0x33;

            data_tmp[18] = convertCharToHex(buffer[60]);
            data_tmp[19] = convertCharToHex(buffer[61]);
            QChar v10 =data_tmp[18]*16+data_tmp[19]-0x33;

            data_tmp[20] = convertCharToHex(buffer[58]);
            data_tmp[21] = convertCharToHex(buffer[59]);
            QChar v11 =data_tmp[20]*16+data_tmp[21]-0x33;

            data_tmp[22] = convertCharToHex(buffer[56]);
            data_tmp[23] = convertCharToHex(buffer[57]);
            QChar v12 =data_tmp[22]*16+data_tmp[23]-0x33;

            data_tmp[24] = convertCharToHex(buffer[54]);
            data_tmp[25] = convertCharToHex(buffer[55]);
            QChar v13 =data_tmp[24]*16+data_tmp[25]-0x33;

            data_tmp[26] = convertCharToHex(buffer[52]);
            data_tmp[27] = convertCharToHex(buffer[53]);
            QChar v14 =data_tmp[26]*16+data_tmp[27]-0x33;

            data_tmp[28] = convertCharToHex(buffer[50]);
            data_tmp[29] = convertCharToHex(buffer[51]);
            QChar v15 =data_tmp[28]*16+data_tmp[29]-0x33;

            data_tmp[30] = convertCharToHex(buffer[48]);
            data_tmp[31] = convertCharToHex(buffer[49]);
            QChar v16 =data_tmp[30]*16+data_tmp[31]-0x33;
            //------------------------------------------------------
            data_tmp[32] = convertCharToHex(buffer[46]);
            data_tmp[33] = convertCharToHex(buffer[47]);
            QChar v17 =data_tmp[32]*16+data_tmp[33]-0x33;

            data_tmp[34] = convertCharToHex(buffer[44]);
            data_tmp[35] = convertCharToHex(buffer[45]);
            QChar v18 =data_tmp[34]*16+data_tmp[35]-0x33;

            data_tmp[36] = convertCharToHex(buffer[42]);
            data_tmp[37] = convertCharToHex(buffer[43]);
            QChar v19 =data_tmp[36]*16+data_tmp[37]-0x33;

            data_tmp[38] = convertCharToHex(buffer[40]);
            data_tmp[39] = convertCharToHex(buffer[41]);
            QChar v20 =data_tmp[38]*16+data_tmp[39]-0x33;

            data_tmp[40] = convertCharToHex(buffer[38]);
            data_tmp[41] = convertCharToHex(buffer[39]);
            QChar v21 =data_tmp[40]*16+data_tmp[41]-0x33;

            data_tmp[42] = convertCharToHex(buffer[36]);
            data_tmp[43] = convertCharToHex(buffer[37]);
            QChar v22 =data_tmp[42]*16+data_tmp[43]-0x33;

            QString AssetCodeStr = "";
            AssetCodeStr = AssetCodeStr+v1+v2+v3+v4+v5+v6+v7+v8+v9+v10+v11+v12+v13+v14+v15+v16+v17+v18+v19+v20+v21+v22;
            ui->lineEdit_AssetCode645->setText(AssetCodeStr);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[70]);
            data_tmp[1] = convertCharToHex(buffer[71]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[68]);
            data_tmp[3] = convertCharToHex(buffer[69]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[66]);
            data_tmp[5] = convertCharToHex(buffer[67]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[64]);
            data_tmp[7] = convertCharToHex(buffer[65]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[62]);
            data_tmp[9] = convertCharToHex(buffer[63]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[60]);
            data_tmp[11] = convertCharToHex(buffer[61]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            data_tmp[12] = convertCharToHex(buffer[58]);
            data_tmp[13] = convertCharToHex(buffer[59]);
            QChar v7 =data_tmp[12]*16+data_tmp[13]-0x33;

            data_tmp[14] = convertCharToHex(buffer[56]);
            data_tmp[15] = convertCharToHex(buffer[57]);
            QChar v8 =data_tmp[14]*16+data_tmp[15]-0x33;
            //------------------------------------------------------
            data_tmp[16] = convertCharToHex(buffer[54]);
            data_tmp[17] = convertCharToHex(buffer[55]);
            QChar v9 =data_tmp[16]*16+data_tmp[17]-0x33;

            data_tmp[18] = convertCharToHex(buffer[52]);
            data_tmp[19] = convertCharToHex(buffer[53]);
            QChar v10 =data_tmp[18]*16+data_tmp[19]-0x33;

            data_tmp[20] = convertCharToHex(buffer[50]);
            data_tmp[21] = convertCharToHex(buffer[51]);
            QChar v11 =data_tmp[20]*16+data_tmp[21]-0x33;

            data_tmp[22] = convertCharToHex(buffer[48]);
            data_tmp[23] = convertCharToHex(buffer[49]);
            QChar v12 =data_tmp[22]*16+data_tmp[23]-0x33;

            data_tmp[24] = convertCharToHex(buffer[46]);
            data_tmp[25] = convertCharToHex(buffer[47]);
            QChar v13 =data_tmp[24]*16+data_tmp[25]-0x33;

            data_tmp[26] = convertCharToHex(buffer[44]);
            data_tmp[27] = convertCharToHex(buffer[45]);
            QChar v14 =data_tmp[26]*16+data_tmp[27]-0x33;

            data_tmp[28] = convertCharToHex(buffer[42]);
            data_tmp[29] = convertCharToHex(buffer[43]);
            QChar v15 =data_tmp[28]*16+data_tmp[29]-0x33;

            data_tmp[30] = convertCharToHex(buffer[40]);
            data_tmp[31] = convertCharToHex(buffer[41]);
            QChar v16 =data_tmp[30]*16+data_tmp[31]-0x33;
            //------------------------------------------------------
            data_tmp[32] = convertCharToHex(buffer[38]);
            data_tmp[33] = convertCharToHex(buffer[39]);
            QChar v17 =data_tmp[32]*16+data_tmp[33]-0x33;

            data_tmp[34] = convertCharToHex(buffer[36]);
            data_tmp[35] = convertCharToHex(buffer[37]);
            QChar v18 =data_tmp[34]*16+data_tmp[35]-0x33;

            data_tmp[36] = convertCharToHex(buffer[34]);
            data_tmp[37] = convertCharToHex(buffer[35]);
            QChar v19 =data_tmp[36]*16+data_tmp[37]-0x33;

            data_tmp[38] = convertCharToHex(buffer[32]);
            data_tmp[39] = convertCharToHex(buffer[33]);
            QChar v20 =data_tmp[38]*16+data_tmp[39]-0x33;

            data_tmp[40] = convertCharToHex(buffer[30]);
            data_tmp[41] = convertCharToHex(buffer[31]);
            QChar v21 =data_tmp[40]*16+data_tmp[41]-0x33;

            data_tmp[42] = convertCharToHex(buffer[28]);
            data_tmp[43] = convertCharToHex(buffer[29]);
            QChar v22 =data_tmp[42]*16+data_tmp[43]-0x33;

            QString AssetCodeStr = "";
            AssetCodeStr = AssetCodeStr+v1+v2+v3+v4+v5+v6+v7+v8+v9+v10+v11+v12+v13+v14+v15+v16+v17+v18+v19+v20+v21+v22;
            ui->lineEdit_AssetCode645->setText(AssetCodeStr);
        }
    }
    else if(isReadProductionDate645)
    {
        isReadProductionDate645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[12] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[46]);
            data_tmp[1] = convertCharToHex(buffer[47]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[44]);
            data_tmp[3] = convertCharToHex(buffer[45]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[42]);
            data_tmp[5] = convertCharToHex(buffer[43]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[40]);
            data_tmp[7] = convertCharToHex(buffer[41]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[38]);
            data_tmp[9] = convertCharToHex(buffer[39]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[36]);
            data_tmp[11] = convertCharToHex(buffer[37]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            QString ProductDate = "";
            ProductDate = ProductDate+"20"+v1+v2+"-"+v3+v4+"-"+v5+v6;
            ui->lineEdit_ProductionDate645->setText(ProductDate);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[38]);
            data_tmp[1] = convertCharToHex(buffer[39]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[36]);
            data_tmp[3] = convertCharToHex(buffer[37]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[34]);
            data_tmp[5] = convertCharToHex(buffer[35]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[32]);
            data_tmp[7] = convertCharToHex(buffer[33]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[30]);
            data_tmp[9] = convertCharToHex(buffer[31]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[28]);
            data_tmp[11] = convertCharToHex(buffer[29]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            QString ProductDate = "";
            ProductDate = ProductDate+"20"+v1+v2+"-"+v3+v4+"-"+v5+v6;
            ui->lineEdit_ProductionDate645->setText(ProductDate);
        }
    }
    else if(isReadRunWord1_645)
    {
        isReadRunWord1_645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 Runword1 = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Runword1 = data_tmp[0]*16+data_tmp[1]-0x33;

            QString out = QString::number(Runword1,16);
            ui->lineEdit_RunWord1_645->setText(out);

            if(Runword1 & 0x20)
            {
                QString out1 = "合闸";
                ui->lineEdit_MCB_State645->setText(out1);
            }
            else
            {
                QString out2 = "分闸";
                ui->lineEdit_MCB_State645->setText(out2);
            }
//-----------------------------------------------------------
            if(0 == (Runword1 & 0x1f))//正常
            {
                QString out3 = "正常";
                ui->lineEdit_FaultReason645->setText(out3);
            }
            else if(0x07 == (Runword1 & 0x1f))
            {
                QString out4 = "缺相";
                ui->lineEdit_FaultReason645->setText(out4);
            }
            else if(0x08 == (Runword1 & 0x1f))
            {
                QString out5 = "欠压";
                ui->lineEdit_FaultReason645->setText(out5);
            }
            else if(0x09 == (Runword1 & 0x1f))
            {
                QString out6 = "过压";
                ui->lineEdit_FaultReason645->setText(out6);
            }
            else if(0x0B == (Runword1 & 0x1f))
            {
                QString out7 = "失压";
                ui->lineEdit_FaultReason645->setText(out7);
            }
            else if(0x1E == (Runword1 & 0x1f))
            {
                QString out8 = "软遥控";
                ui->lineEdit_FaultReason645->setText(out8);
            }
            else if(0x1D == (Runword1 & 0x1f))
            {
                QString out9 = "孤岛";
                ui->lineEdit_FaultReason645->setText(out9);
            }
            else if(0x1F == (Runword1 & 0x1f))
            {
                QString out10 = "其它";
                ui->lineEdit_FaultReason645->setText(out10);
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            Runword1 = data_tmp[0]*16+data_tmp[1]-0x33;

            QString out = QString::number(Runword1,16);
            ui->lineEdit_RunWord1_645->setText(out);

            if(Runword1 & 0x20)
            {
                QString out1 = "合闸";
                ui->lineEdit_MCB_State645->setText(out1);
            }
            else
            {
                QString out2 = "分闸";
                ui->lineEdit_MCB_State645->setText(out2);
            }
//-----------------------------------------------------------
            if(0 == (Runword1 & 0x1f))//正常
            {
                QString out3 = "正常";
                ui->lineEdit_FaultReason645->setText(out3);
            }
            else if(0x07 == (Runword1 & 0x1f))
            {
                QString out4 = "缺相";
                ui->lineEdit_FaultReason645->setText(out4);
            }
            else if(0x08 == (Runword1 & 0x1f))
            {
                QString out5 = "欠压";
                ui->lineEdit_FaultReason645->setText(out5);
            }
            else if(0x09 == (Runword1 & 0x1f))
            {
                QString out6 = "过压";
                ui->lineEdit_FaultReason645->setText(out6);
            }
            else if(0x0B == (Runword1 & 0x1f))
            {
                QString out7 = "失压";
                ui->lineEdit_FaultReason645->setText(out7);
            }
            else if(0x1E == (Runword1 & 0x1f))
            {
                QString out8 = "软遥控";
                ui->lineEdit_FaultReason645->setText(out8);
            }
            else if(0x1D == (Runword1 & 0x1f))
            {
                QString out9 = "孤岛";
                ui->lineEdit_FaultReason645->setText(out9);
            }
            else if(0x1F == (Runword1 & 0x1f))
            {
                QString out10 = "其它";
                ui->lineEdit_FaultReason645->setText(out10);
            }
        }
    }
    else if(isReadRunWord2_645)
    {
        isReadRunWord2_645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 Runword2 = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Runword2 = data_tmp[0]*16+data_tmp[1]-0x33;

            QString out = QString::number(Runword2,16);
            ui->lineEdit_RunWord2_645->setText(out);

            if(Runword2 & 0x40)
            {
                QString out1 = "发生";
                ui->lineEdit_Warning->setText(out1);
            }
            else
            {
                QString out2 = "恢复";
                ui->lineEdit_Warning->setText(out2);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x20)
            {
                QString out3 = "变化";
                ui->lineEdit_MCBchange->setText(out3);
            }
            else
            {
                QString out4 = "未变";
                ui->lineEdit_MCBchange->setText(out4);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x10)
            {
                QString out5 = "运行";
                ui->lineEdit_ProtectEnable->setText(out5);
            }
            else
            {
                QString out6 = "退出";
                ui->lineEdit_ProtectEnable->setText(out6);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x08)
            {
                QString out7 = "掉电";
                ui->lineEdit_LoseVoltCheck->setText(out7);
            }
            else
            {
                QString out8 = "复电";
                ui->lineEdit_LoseVoltCheck->setText(out8);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x04)
            {
                QString out9 = "动作";
                ui->lineEdit_ProtectAction->setText(out9);
            }
            else
            {
                QString out10 = "未动";
                ui->lineEdit_ProtectAction->setText(out10);
            }
//-----------------------------------------------------------

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            Runword2 = data_tmp[0]*16+data_tmp[1]-0x33;

            QString out = QString::number(Runword2,16);
            ui->lineEdit_RunWord2_645->setText(out);

            if(Runword2 & 0x40)
            {
                QString out1 = "发生";
                ui->lineEdit_Warning->setText(out1);
            }
            else
            {
                QString out2 = "恢复";
                ui->lineEdit_Warning->setText(out2);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x20)
            {
                QString out3 = "变化";
                ui->lineEdit_MCBchange->setText(out3);
            }
            else
            {
                QString out4 = "未变";
                ui->lineEdit_MCBchange->setText(out4);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x10)
            {
                QString out5 = "运行";
                ui->lineEdit_ProtectEnable->setText(out5);
            }
            else
            {
                QString out6 = "退出";
                ui->lineEdit_ProtectEnable->setText(out6);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x08)
            {
                QString out7 = "掉电";
                ui->lineEdit_LoseVoltCheck->setText(out7);
            }
            else
            {
                QString out8 = "复电";
                ui->lineEdit_LoseVoltCheck->setText(out8);
            }
//-----------------------------------------------------------
            if(Runword2 & 0x04)
            {
                QString out9 = "动作";
                ui->lineEdit_ProtectAction->setText(out9);
            }
            else
            {
                QString out10 = "未动";
                ui->lineEdit_ProtectAction->setText(out10);
            }
//-----------------------------------------------------------
        }
    }
    else if(isReadCtrlWord1_645)
    {
        isReadCtrlWord1_645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 Ctrlword1 = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Ctrlword1 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword1 & 0x08) //掉电保护已使能
            {
                ui->checkBox_LV_ActionEn->setChecked(true);
            }
            else
            {
                ui->checkBox_LV_ActionEn->setChecked(false);
            }
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            Ctrlword1 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword1 & 0x08) //掉电保护已使能
            {
                ui->checkBox_LV_ActionEn->setChecked(true);
            }
            else
            {
                ui->checkBox_LV_ActionEn->setChecked(false);
            }
        }
    }
    else if(isReadCtrlWord3_645)
    {
        isReadCtrlWord3_645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 Ctrlword3 = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Ctrlword3 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword3 & 0x80) //缺相保护
            {
                ui->checkBox_isLPProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isLPProtectEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword3 & 0x20) //欠压保护
            {
                ui->checkBox_isUVProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isUVProtectEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword3 & 0x08) //过压保护
            {
                ui->checkBox_isOVProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isOVProtectEn->setChecked(false);
            }
//----------------------------------------------------------------

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            Ctrlword3 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword3 & 0x80) //缺相保护
            {
                ui->checkBox_isLPProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isLPProtectEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword3 & 0x20) //欠压保护
            {
                ui->checkBox_isUVProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isUVProtectEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword3 & 0x08) //过压保护
            {
                ui->checkBox_isOVProtectEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isOVProtectEn->setChecked(false);
            }
//----------------------------------------------------------------
        }
    }
    else if(isReadCtrlWord7_645)
    {
        isReadCtrlWord7_645 = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 Ctrlword7 = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Ctrlword7 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword7 & 0x04) //过欠频保护
            {
                ui->checkBox_isOFUFEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isOFUFEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword7 & 0x02) //频率孤岛
            {
                ui->checkBox_isIslandFreqEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isIslandFreqEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword7 & 0x01) //幅值孤岛
            {
                ui->checkBox_isIslandVoltEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isIslandVoltEn->setChecked(false);
            }
//----------------------------------------------------------------

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            Ctrlword7 = data_tmp[0]*16+data_tmp[1]-0x33;

            if(Ctrlword7 & 0x04) //过欠频保护
            {
                ui->checkBox_isOFUFEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isOFUFEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword7 & 0x02) //频率孤岛
            {
                ui->checkBox_isIslandFreqEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isIslandFreqEn->setChecked(false);
            }
//----------------------------------------------------------------
            if(Ctrlword7 & 0x01) //幅值孤岛
            {
                ui->checkBox_isIslandVoltEn->setChecked(true);
            }
            else
            {
                ui->checkBox_isIslandVoltEn->setChecked(false);
            }
//----------------------------------------------------------------
        }
    }
    else if(isWriteCtrlWord1)
    {
        isWriteCtrlWord1 = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord1 Message", "控制字1写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord1 Message", "控制字1写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord1 Message", "控制字1写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord1 Message", "控制字1写入失败");
            }
        }
    }
    else if(isWriteCtrlWord3)
    {
        isWriteCtrlWord3 = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord3 Message", "控制字3写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord3 Message", "控制字3写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord3 Message", "控制字3写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord3 Message", "控制字3写入失败");
            }
        }
    }
    else if(isWriteCtrlWord7)
    {
        isWriteCtrlWord7 = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord7 Message", "控制字7写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord7 Message", "控制字7写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"ControlWord7 Message", "控制字7写入成功");
            }
            else
            {
                QMessageBox::critical(this,"ControlWord7 Message", "控制字7写入失败");
            }
        }
    }
    else if(isReadOpenTimeSend)
    {
        isReadOpenTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};
        quint8 OpenTimeData = 0;
        quint8 OpenTimeUnit = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            OpenTimeData = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            qDebug()<<"OpenTimeData: "<<OpenTimeData;
            QString text_data = QString::number(OpenTimeData);
            ui->lineEdit_OpenTimeData->setText(text_data);

            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            OpenTimeUnit = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            if(2 == OpenTimeUnit)
            {
                QString out_unit = "分";
                ui->lineEdit_OpenTimeUnit->setText(out_unit);
            }
            else if(3 == OpenTimeUnit)
            {
                QString out_unit1 = "时";
                ui->lineEdit_OpenTimeUnit->setText(out_unit1);
            }
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            OpenTimeData = data_tmp[0]*16+data_tmp[1]-0x33;
            QString text_data = QString::number(OpenTimeData);
            ui->lineEdit_OpenTimeData->setText(text_data);

            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            OpenTimeUnit = data_tmp[2]*16+data_tmp[3]-0x33;
            if(2 == OpenTimeUnit)
            {
                QString out_unit = "分";
                ui->lineEdit_OpenTimeUnit->setText(out_unit);
            }
            else if(3 == OpenTimeUnit)
            {
                QString out_unit1 = "时";
                ui->lineEdit_OpenTimeUnit->setText(out_unit1);
            }
        }
    }
    else if(isWriteOpenTime)
    {
        isWriteOpenTime = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"OpenTimeSet Message", "预约分闸时间写入成功");
            }
            else
            {
                QMessageBox::critical(this,"OpenTimeSet Message", "预约分闸时间写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"OpenTimeSet Message", "预约分闸时间写入成功");
            }
            else
            {
                QMessageBox::critical(this,"OpenTimeSet Message", "预约分闸时间写入失败");
            }
        }
    }
    else if(isCancelOpenSend)
    {
        isCancelOpenSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"CancelOpenCmd Message", "取消预约分闸成功");
            }
            else
            {
                QMessageBox::critical(this,"CancelOpenCmd Message", "取消预约分闸失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"CancelOpenCmd Message", "取消预约分闸成功");
            }
            else
            {
                QMessageBox::critical(this,"CancelOpenCmd Message", "取消预约分闸失败");
            }
        }
    }
    else if(isReadCloseTimeSend)
    {
        isReadCloseTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};
        quint8 CloseTimeData = 0;
        quint8 CloseTimeUnit = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            CloseTimeData = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            QString text_data = QString::number(CloseTimeData);
            ui->lineEdit_ReadCloseTimeData->setText(text_data);

            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            CloseTimeUnit = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            if(2 == CloseTimeUnit)
            {
                QString out_unit = "分";
                ui->lineEdit_CloseTimeUnit->setText(out_unit);
            }
            else if(3 == CloseTimeUnit)
            {
                QString out_unit1 = "时";
                ui->lineEdit_CloseTimeUnit->setText(out_unit1);
            }
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            CloseTimeData = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            QString text_data = QString::number(CloseTimeData);
            ui->lineEdit_ReadCloseTimeData->setText(text_data);

            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            CloseTimeUnit = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            if(2 == CloseTimeUnit)
            {
                QString out_unit = "分";
                ui->lineEdit_CloseTimeUnit->setText(out_unit);
            }
            else if(3 == CloseTimeUnit)
            {
                QString out_unit1 = "时";
                ui->lineEdit_CloseTimeUnit->setText(out_unit1);
            }
        }
    }
    else if(isWriteCloseTime)
    {
        isWriteCloseTime = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"OpenTimeSet Message", "预约合闸时间写入成功");
            }
            else
            {
                QMessageBox::critical(this,"OpenTimeSet Message", "预约合闸时间写入失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"OpenTimeSet Message", "预约合闸时间写入成功");
            }
            else
            {
                QMessageBox::critical(this,"OpenTimeSet Message", "预约合闸时间写入失败");
            }
        }
    }
    else if(isCancelCloseSend)
    {
        isCancelCloseSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"CancelCloseCmd Message", "取消预约合闸成功");
            }
            else
            {
                QMessageBox::critical(this,"CancelCloseCmd Message", "取消预约合闸失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"CancelCloseCmd Message", "取消预约合闸成功");
            }
            else
            {
                QMessageBox::critical(this,"CancelCloseCmd Message", "取消预约合闸失败");
            }
        }
    }
    else if(isAutoSetDateTimeSend)
    {
        isAutoSetDateTimeSend = false;
    }
    else if(isManualSetDateTimeSend)
    {
        isManualSetDateTimeSend = false;
    }
    else if(isReadTotalOpenCntSend)
    {
        isReadTotalOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_TotalOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_TotalOpenCntData->setText(text_data);
        }
    }
    else if(isClearTotalOpenCntSend)
    {
        isClearTotalOpenCntSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                QMessageBox::information(this,"Message", "清除跳闸总次数成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除跳闸总次数失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                QMessageBox::information(this,"Message", "清除跳闸总次数成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除跳闸总次数失败");
            }
        }
    }
    else if(isReadOVOpenCntSend)
    {
        isReadOVOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_OVOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_OVOpenCntData->setText(text_data);
        }
    }
    else if(isReadUVOpenCntSend)
    {
        isReadUVOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_UVOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_UVOpenCntData->setText(text_data);
        }
    }
    else if(isReadLPOpenCntSend)
    {
        isReadLPOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_LPOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_LPOpenCntData->setText(text_data);
        }
    }
    else if(isReadLVOpenCntSend)
    {
        isReadLVOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_LVOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_LVOpenCntData->setText(text_data);
        }
    }
    else if(isReadRemoteOpenCntSend)
    {
        isReadRemoteOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_RemoteOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_RemoteOpenCntData->setText(text_data);
        }
    }
    else if(isReadIslandOpenCntSend)
    {
        isReadIslandOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_IslandOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_IslandOpenCntData->setText(text_data);
        }
    }
    else if(isReadManualOpenCntSend)
    {
        isReadManualOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ManualOpenCntData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ManualOpenCntData->setText(text_data);
        }
    }
    else if(isReadClearOpenCntSend)
    {
        isReadClearOpenCntSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[6] = {0};

        quint32 total_OpenCnt = 0;
        quint32 total_OpenCnt_H = 0;
        quint32 total_OpenCnt_M = 0;
        quint32 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_M = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            total_OpenCnt_H = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33) * 10000;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_M + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ClearOpenDataCnt->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_M = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            data_tmp[4] = convertCharToHex(buffer[32]);
            data_tmp[5] = convertCharToHex(buffer[33]);
            total_OpenCnt_H = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33) * 10000;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_M + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ClearOpenDataCnt->setText(text_data);
        }
    }
    else if(isClearAllProtectOpenCntSend)
    {
        isClearAllProtectOpenCntSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                ui->lineEdit_OVOpenCntData->setText(text_data);
                ui->lineEdit_UVOpenCntData->setText(text_data);
                ui->lineEdit_LPOpenCntData->setText(text_data);
                ui->lineEdit_LVOpenCntData->setText(text_data);
                ui->lineEdit_RemoteOpenCntData->setText(text_data);
                ui->lineEdit_IslandOpenCntData->setText(text_data);
                ui->lineEdit_ManualOpenCntData->setText(text_data);
                QMessageBox::information(this,"Message", "清除所有保护跳闸次数成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除所有保护跳闸次数失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                ui->lineEdit_OVOpenCntData->setText(text_data);
                ui->lineEdit_UVOpenCntData->setText(text_data);
                ui->lineEdit_LPOpenCntData->setText(text_data);
                ui->lineEdit_LVOpenCntData->setText(text_data);
                ui->lineEdit_RemoteOpenCntData->setText(text_data);
                ui->lineEdit_IslandOpenCntData->setText(text_data);
                ui->lineEdit_ManualOpenCntData->setText(text_data);

                QMessageBox::information(this,"Message", "清除所有保护跳闸次数成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除所有保护跳闸次数失败");
            }
        }
    }
    else if(isReadTotalRunTimeSend)
    {
        isReadTotalRunTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        quint32 total_OpenCnt = 0;
        quint32 total_OpenCnt_H = 0;
        quint32 total_OpenCnt_M = 0;
        quint32 total_OpenCnt_M2 = 0;
        quint32 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_M = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            total_OpenCnt_M2 = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33) * 10000;
            data_tmp[6] = convertCharToHex(buffer[42]);
            data_tmp[7] = convertCharToHex(buffer[43]);
            total_OpenCnt_H = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33) * 1000000;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_M + total_OpenCnt_M2 + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_TotalRunTimeData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_M = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            data_tmp[4] = convertCharToHex(buffer[32]);
            data_tmp[5] = convertCharToHex(buffer[33]);
            total_OpenCnt_M2 = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33) * 10000;
            data_tmp[6] = convertCharToHex(buffer[34]);
            data_tmp[7] = convertCharToHex(buffer[35]);
            total_OpenCnt_H = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33) * 1000000;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_M + total_OpenCnt_M2 + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_TotalRunTimeData->setText(text_data);
        }
    }
    else if(isClearTotalRunTimeSend)
    {
        isClearTotalRunTimeSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalRunTimeData->setText(text_data);
                QMessageBox::information(this,"Message", "清除运行总时间成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除运行总时间失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalRunTimeData->setText(text_data);

                QMessageBox::information(this,"Message", "清除运行总时间成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "清除运行总时间失败");
            }
        }
    }
    else if(isReadStateChangeNewAddSend)
    {
        isReadStateChangeNewAddSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};

        quint16 total_OpenCnt = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_StateChangeNewAddData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_StateChangeNewAddData->setText(text_data);
        }
    }
    else if(isClearAllSend)
    {
        isClearAllSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                ui->lineEdit_OVOpenCntData->setText(text_data);
                ui->lineEdit_UVOpenCntData->setText(text_data);
                ui->lineEdit_LPOpenCntData->setText(text_data);
                ui->lineEdit_LVOpenCntData->setText(text_data);
                ui->lineEdit_RemoteOpenCntData->setText(text_data);
                ui->lineEdit_IslandOpenCntData->setText(text_data);
                ui->lineEdit_ManualOpenCntData->setText(text_data);
                ui->lineEdit_TotalRunTimeData->setText(text_data);
                ui->lineEdit_StateChangeNewAddData->setText(text_data);
                QMessageBox::information(this,"Message", "事件总清成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "事件总清失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QString text_data = "0";
                ui->lineEdit_TotalOpenCntData->setText(text_data);
                ui->lineEdit_OVOpenCntData->setText(text_data);
                ui->lineEdit_UVOpenCntData->setText(text_data);
                ui->lineEdit_LPOpenCntData->setText(text_data);
                ui->lineEdit_LVOpenCntData->setText(text_data);
                ui->lineEdit_RemoteOpenCntData->setText(text_data);
                ui->lineEdit_IslandOpenCntData->setText(text_data);
                ui->lineEdit_ManualOpenCntData->setText(text_data);
                ui->lineEdit_TotalRunTimeData->setText(text_data);
                ui->lineEdit_StateChangeNewAddData->setText(text_data);

                QMessageBox::information(this,"Message", "事件总清成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "事件总清失败");
            }
        }
    }
    else if(isReadReadOVThresholdSend)
    {
        isReadReadOVThresholdSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_ReadOVThresData->setText(textV);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_ReadOVThresData->setText(textV);
        }
    }
    else if(isReadReadUVThresholdSend)
    {
        isReadReadUVThresholdSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_ReadUVThresData->setText(textV);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_ReadUVThresData->setText(textV);
        }
    }
    else if(isReadOVDelayTimeSend)
    {
        isReadOVDelayTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ReadOVDelayTime->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ReadOVDelayTime->setText(text_data);
        }
    }
    else if(isReadUVDelayTimeSend)
    {
        isReadUVDelayTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ReadUVDelayTime->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_ReadUVDelayTime->setText(text_data);
        }
    }
    else if(isWriteOVThresholdSend)
    {
        isWriteOVThresholdSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteUVThresholdSend)
    {
        isWriteUVThresholdSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteOVDelayTimeSend)
    {
        isWriteOVDelayTimeSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteUVDelayTimeSend)
    {
        isWriteUVDelayTimeSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isReadOVRecoverThresSend)
    {
        isReadOVRecoverThresSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_OVRecoverThresData->setText(textV);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_OVRecoverThresData->setText(textV);
        }
    }
    else if(isReadUVRecoverThresSend)
    {
        isReadUVRecoverThresSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_UVRecoverThresData->setText(textV);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            float V = total_OpenCnt/10.0;
            QString textV = QString::number(V, 'f', 1); // 保留一位小数
            ui->lineEdit_UVRecoverThresData->setText(textV);
        }
    }
    else if(isReadOVRecoverDelayTimeSend)
    {
        isReadOVRecoverDelayTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_OVRecoverDelayTimeData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_OVRecoverDelayTimeData->setText(text_data);
        }
    }
    else if(isReadUVRecoverDelayTimeSend)
    {
        isReadUVRecoverDelayTimeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        quint16 total_OpenCnt = 0;
        quint16 total_OpenCnt_H = 0;
        quint16 total_OpenCnt_L = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_UVRecoverDelayTimeData->setText(text_data);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            total_OpenCnt_L = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            data_tmp[2] = convertCharToHex(buffer[30]);
            data_tmp[3] = convertCharToHex(buffer[31]);
            total_OpenCnt_H = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33) * 100;
            total_OpenCnt = total_OpenCnt_H + total_OpenCnt_L;
            QString text_data = QString::number(total_OpenCnt);
            ui->lineEdit_UVRecoverDelayTimeData->setText(text_data);
        }
    }
    else if(isWriteOVRecoverThresSend)
    {
        isWriteOVRecoverThresSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteUVRecoverThresSend)
    {
        isWriteUVRecoverThresSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复阈值设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复阈值设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteOVRecoverDelayTimeSend)
    {
        isWriteOVRecoverDelayTimeSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "过压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "过压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isWriteUVRecoverDelayTimeSend)
    {
        isWriteUVRecoverDelayTimeSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "欠压恢复时间设置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "欠压恢复时间设置失败，请检查设置值是否超范围或权限密码是否匹配");
            }
        }
    }
    else if(isCalibration645Send)
    {
        isCalibration645Send = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "645校准成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "校准失败，请检查校准电压");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "645校准成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "校准失败，请检查校准电压");
            }
        }
    }
    else if(isMotorPosMCBPosSend)
    {
        isMotorPosMCBPosSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};
        quint8 PosGet = 0;
        QString Pos1 = "脱扣";
        QString Pos2 = "初始";
        QString Pos3 = "合闸";
        QString Pos4 = "手柄合闸";
        QString Pos5 = "手柄分闸";
        QString MotorPos;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            PosGet = data_tmp[0]*16+data_tmp[1]-0x33;

            if(PosGet & 0x01) //脱扣位
            {
                MotorPos += Pos1;
                MotorPos += " ";
            }

            if(PosGet & 0x02) //初始位
            {
                MotorPos += Pos2;
                MotorPos += " ";
            }

            if(PosGet & 0x04) //初始位
            {
                MotorPos += Pos3;
                MotorPos += " ";
            }
            ui->lineEdit_MotorPos->setText(MotorPos);

            if(PosGet & 0x08) //手柄位
            {
                ui->lineEdit_MCB_Pos->setText(Pos4);
            }
            else
            {
                ui->lineEdit_MCB_Pos->setText(Pos5);
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[28]);
            data_tmp[1] = convertCharToHex(buffer[29]);
            PosGet = data_tmp[0]*16+data_tmp[1]-0x33;

            if(PosGet & 0x01) //脱扣位
            {
                MotorPos += Pos1;
                MotorPos += " ";
            }

            if(PosGet & 0x02) //初始位
            {
                MotorPos += Pos2;
                MotorPos += " ";
            }

            if(PosGet & 0x04) //初始位
            {
                MotorPos += Pos3;
                MotorPos += " ";
            }
            ui->lineEdit_MotorPos->setText(MotorPos);

            if(PosGet & 0x08) //手柄位
            {
                ui->lineEdit_MCB_Pos->setText(Pos4);
            }
            else
            {
                ui->lineEdit_MCB_Pos->setText(Pos5);
            }
        }
    }
    else if(isEventCheckNo)
    {
        isEventCheckNo = false;

        char buffer[128] = {0};
        quint8 data_tmp[34] = {0};

        quint8 FaultReason = 0;
        quint8 FaultPhase = 0;
        quint8 Record_year = 0;
        quint8  Record_month = 0;
        quint8  Record_day = 0;
        quint8  Record_hour = 0;
        quint8  Record_min = 0;
        quint8  Record_sec = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            FaultReason = data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            FaultPhase = data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            Record_year = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[6] = convertCharToHex(buffer[42]);
            data_tmp[7] = convertCharToHex(buffer[43]);
            Record_month = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[8] = convertCharToHex(buffer[44]);
            data_tmp[9] = convertCharToHex(buffer[45]);
            Record_day = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[10] = convertCharToHex(buffer[46]);
            data_tmp[11] = convertCharToHex(buffer[47]);
            Record_hour = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[12] = convertCharToHex(buffer[48]);
            data_tmp[13] = convertCharToHex(buffer[49]);
            Record_min = BcdToHex(data_tmp[12]*16+data_tmp[13]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[14] = convertCharToHex(buffer[50]);
            data_tmp[15] = convertCharToHex(buffer[51]);
            Record_sec = BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_ProtectTime->setText(out);

            //A相
            data_tmp[16] = convertCharToHex(buffer[52]);
            data_tmp[17] = convertCharToHex(buffer[53]);
            data_tmp[18] = convertCharToHex(buffer[54]);
            data_tmp[19] = convertCharToHex(buffer[55]);
            data_tmp[20] = convertCharToHex(buffer[56]);
            data_tmp[21] = convertCharToHex(buffer[57]);
//            quint16 Va = BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33)*10000+BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33)*100+BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            quint16 Va = BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33)*100+BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            qDebug()<<"A相电压: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVA->setText(textVA);
            //B相
            data_tmp[22] = convertCharToHex(buffer[58]);
            data_tmp[23] = convertCharToHex(buffer[59]);
            data_tmp[24] = convertCharToHex(buffer[60]);
            data_tmp[25] = convertCharToHex(buffer[61]);
            data_tmp[26] = convertCharToHex(buffer[62]);
            data_tmp[27] = convertCharToHex(buffer[63]);
            quint16 Vb = BcdToHex(data_tmp[24]*16+data_tmp[25]-0x33)*100+BcdToHex(data_tmp[22]*16+data_tmp[23]-0x33);
            qDebug()<<"B相电压: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVB->setText(textVB);
            //C相
            data_tmp[28] = convertCharToHex(buffer[64]);
            data_tmp[29] = convertCharToHex(buffer[65]);
            data_tmp[30] = convertCharToHex(buffer[66]);
            data_tmp[31] = convertCharToHex(buffer[67]);
            data_tmp[32] = convertCharToHex(buffer[68]);
            data_tmp[33] = convertCharToHex(buffer[69]);
            quint16 Vc = BcdToHex(data_tmp[30]*16+data_tmp[31]-0x33)*100+BcdToHex(data_tmp[28]*16+data_tmp[29]-0x33);
            qDebug()<<"C相电压: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVC->setText(textVC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[36-8]);
            data_tmp[1] = convertCharToHex(buffer[37-8]);
            FaultReason = data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[38-8]);
            data_tmp[3] = convertCharToHex(buffer[39-8]);
            FaultPhase = data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40-8]);
            data_tmp[5] = convertCharToHex(buffer[41-8]);
            Record_year = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[6] = convertCharToHex(buffer[42-8]);
            data_tmp[7] = convertCharToHex(buffer[43-8]);
            Record_month = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[8] = convertCharToHex(buffer[44-8]);
            data_tmp[9] = convertCharToHex(buffer[45-8]);
            Record_day = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[10] = convertCharToHex(buffer[46-8]);
            data_tmp[11] = convertCharToHex(buffer[47-8]);
            Record_hour = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[12] = convertCharToHex(buffer[48-8]);
            data_tmp[13] = convertCharToHex(buffer[49-8]);
            Record_min = BcdToHex(data_tmp[12]*16+data_tmp[13]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[14] = convertCharToHex(buffer[50-8]);
            data_tmp[15] = convertCharToHex(buffer[51-8]);
            Record_sec = BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_ProtectTime->setText(out);

            //A相
            data_tmp[16] = convertCharToHex(buffer[52-8]);
            data_tmp[17] = convertCharToHex(buffer[53-8]);
            data_tmp[18] = convertCharToHex(buffer[54-8]);
            data_tmp[19] = convertCharToHex(buffer[55-8]);
            data_tmp[20] = convertCharToHex(buffer[56-8]);
            data_tmp[21] = convertCharToHex(buffer[57-8]);
//            quint16 Va = BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33)*10000+BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33)*100+BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            quint16 Va = BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33)*100+BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            qDebug()<<"A相电压: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVA->setText(textVA);
            //B相
            data_tmp[22] = convertCharToHex(buffer[58-8]);
            data_tmp[23] = convertCharToHex(buffer[59-8]);
            data_tmp[24] = convertCharToHex(buffer[60-8]);
            data_tmp[25] = convertCharToHex(buffer[61-8]);
            data_tmp[26] = convertCharToHex(buffer[62-8]);
            data_tmp[27] = convertCharToHex(buffer[63-8]);
            quint16 Vb = BcdToHex(data_tmp[24]*16+data_tmp[25]-0x33)*100+BcdToHex(data_tmp[22]*16+data_tmp[23]-0x33);
            qDebug()<<"B相电压: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVB->setText(textVB);
            //C相
            data_tmp[28] = convertCharToHex(buffer[64-8]);
            data_tmp[29] = convertCharToHex(buffer[65-8]);
            data_tmp[30] = convertCharToHex(buffer[66-8]);
            data_tmp[31] = convertCharToHex(buffer[67-8]);
            data_tmp[32] = convertCharToHex(buffer[68-8]);
            data_tmp[33] = convertCharToHex(buffer[69-8]);
            quint16 Vc = BcdToHex(data_tmp[30]*16+data_tmp[31]-0x33)*100+BcdToHex(data_tmp[28]*16+data_tmp[29]-0x33);
            qDebug()<<"C相电压: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_ProtectVC->setText(textVC);
        }

        switch (FaultReason) {
        case 0:
            ui->lineEdit_ProtectReason->setText("恢复正常");
            break;
        case 6:
            ui->lineEdit_ProtectReason->setText("过压");
            break;
        case 7:
            ui->lineEdit_ProtectReason->setText("欠压");
            break;
        case 8:
            ui->lineEdit_ProtectReason->setText("缺相");
            break;
        case 21:
            ui->lineEdit_ProtectReason->setText("孤岛");
            break;
        case 23:
            ui->lineEdit_ProtectReason->setText("远程分闸");
            break;
        case 24:
            ui->lineEdit_ProtectReason->setText("远程合闸");
            break;
        case 25:
            ui->lineEdit_ProtectReason->setText("手动分闸");
            break;
        case 26:
            ui->lineEdit_ProtectReason->setText("手动合闸");
            break;
        case 33:
            ui->lineEdit_ProtectReason->setText("失压");
            break;
        case 34:
            ui->lineEdit_ProtectReason->setText("过欠频");
            break;
        default:
            ui->lineEdit_ProtectReason->setText("未知");
            break;
        }

        switch (FaultPhase) {
        case 0:
            ui->lineEdit_ProtectPhase->setText("NULL");
            break;
        case 1:
            ui->lineEdit_ProtectPhase->setText("A");
            break;
        case 2:
            ui->lineEdit_ProtectPhase->setText("B");
            break;
        case 3:
            ui->lineEdit_ProtectPhase->setText("AB");
            break;
        case 4:
            ui->lineEdit_ProtectPhase->setText("C");
            break;
        case 5:
            ui->lineEdit_ProtectPhase->setText("AC");
            break;
        case 6:
            ui->lineEdit_ProtectPhase->setText("BC");
            break;
        case 7:
            ui->lineEdit_ProtectPhase->setText("ABC");
            break;
        default:
            ui->lineEdit_ProtectPhase->setText("异常");
            break;
        }
    }
    else if(isStateChangeNo)
    {
        isStateChangeNo = false;

        char buffer[128] = {0};
        quint8 data_tmp[36] = {0};

        quint8 SwitchDirection = 0;//闸位变化状态
        quint8 FaultReason = 0;
        quint8 FaultPhase = 0;
        quint8 Record_year = 0;
        quint8  Record_month = 0;
        quint8  Record_day = 0;
        quint8  Record_hour = 0;
        quint8  Record_min = 0;
        quint8  Record_sec = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            SwitchDirection = data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            FaultReason = data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            FaultPhase = data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[42]);
            data_tmp[7] = convertCharToHex(buffer[43]);
            Record_year = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[8] = convertCharToHex(buffer[44]);
            data_tmp[9] = convertCharToHex(buffer[45]);
            Record_month = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[10] = convertCharToHex(buffer[46]);
            data_tmp[11] = convertCharToHex(buffer[47]);
            Record_day = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[12] = convertCharToHex(buffer[48]);
            data_tmp[13] = convertCharToHex(buffer[49]);
            Record_hour = BcdToHex(data_tmp[12]*16+data_tmp[13]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[14] = convertCharToHex(buffer[50]);
            data_tmp[15] = convertCharToHex(buffer[51]);
            Record_min = BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[16] = convertCharToHex(buffer[52]);
            data_tmp[17] = convertCharToHex(buffer[53]);
            Record_sec = BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_StateChangeTime->setText(out);

            //A相
            data_tmp[18] = convertCharToHex(buffer[54]);
            data_tmp[19] = convertCharToHex(buffer[55]);
            data_tmp[20] = convertCharToHex(buffer[56]);
            data_tmp[21] = convertCharToHex(buffer[57]);
            data_tmp[22] = convertCharToHex(buffer[58]);
            data_tmp[23] = convertCharToHex(buffer[59]);
            quint16 Va = BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33)*100+BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33);
            qDebug()<<"A相电压: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVA->setText(textVA);
            //B相
            data_tmp[24] = convertCharToHex(buffer[60]);
            data_tmp[25] = convertCharToHex(buffer[61]);
            data_tmp[26] = convertCharToHex(buffer[62]);
            data_tmp[27] = convertCharToHex(buffer[63]);
            data_tmp[28] = convertCharToHex(buffer[64]);
            data_tmp[29] = convertCharToHex(buffer[65]);
            quint16 Vb = BcdToHex(data_tmp[26]*16+data_tmp[27]-0x33)*100+BcdToHex(data_tmp[24]*16+data_tmp[25]-0x33);
            qDebug()<<"B相电压: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVB->setText(textVB);
            //C相
            data_tmp[30] = convertCharToHex(buffer[66]);
            data_tmp[31] = convertCharToHex(buffer[67]);
            data_tmp[32] = convertCharToHex(buffer[68]);
            data_tmp[33] = convertCharToHex(buffer[69]);
            data_tmp[34] = convertCharToHex(buffer[70]);
            data_tmp[35] = convertCharToHex(buffer[71]);
            quint16 Vc = BcdToHex(data_tmp[32]*16+data_tmp[33]-0x33)*100+BcdToHex(data_tmp[30]*16+data_tmp[31]-0x33);
            qDebug()<<"C相电压: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVC->setText(textVC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[36-8]);
            data_tmp[1] = convertCharToHex(buffer[37-8]);
            SwitchDirection = data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[38-8]);
            data_tmp[3] = convertCharToHex(buffer[39-8]);
            FaultReason = data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[40-8]);
            data_tmp[5] = convertCharToHex(buffer[41-8]);
            FaultPhase = data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[42-8]);
            data_tmp[7] = convertCharToHex(buffer[43-8]);
            Record_year = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[8] = convertCharToHex(buffer[44-8]);
            data_tmp[9] = convertCharToHex(buffer[45-8]);
            Record_month = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[10] = convertCharToHex(buffer[46-8]);
            data_tmp[11] = convertCharToHex(buffer[47-8]);
            Record_day = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[12] = convertCharToHex(buffer[48-8]);
            data_tmp[13] = convertCharToHex(buffer[49-8]);
            Record_hour = BcdToHex(data_tmp[12]*16+data_tmp[13]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[14] = convertCharToHex(buffer[50-8]);
            data_tmp[15] = convertCharToHex(buffer[51-8]);
            Record_min = BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[16] = convertCharToHex(buffer[52-8]);
            data_tmp[17] = convertCharToHex(buffer[53-8]);
            Record_sec = BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_StateChangeTime->setText(out);

            //A相
            data_tmp[18] = convertCharToHex(buffer[54-8]);
            data_tmp[19] = convertCharToHex(buffer[55-8]);
            data_tmp[20] = convertCharToHex(buffer[56-8]);
            data_tmp[21] = convertCharToHex(buffer[57-8]);
            data_tmp[22] = convertCharToHex(buffer[58-8]);
            data_tmp[23] = convertCharToHex(buffer[59-8]);
            quint16 Va = BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33)*100+BcdToHex(data_tmp[18]*16+data_tmp[19]-0x33);
            qDebug()<<"A相电压: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVA->setText(textVA);
            //B相
            data_tmp[24] = convertCharToHex(buffer[60-8]);
            data_tmp[25] = convertCharToHex(buffer[61-8]);
            data_tmp[26] = convertCharToHex(buffer[62-8]);
            data_tmp[27] = convertCharToHex(buffer[63-8]);
            data_tmp[28] = convertCharToHex(buffer[64-8]);
            data_tmp[29] = convertCharToHex(buffer[65-8]);
            quint16 Vb = BcdToHex(data_tmp[26]*16+data_tmp[27]-0x33)*100+BcdToHex(data_tmp[24]*16+data_tmp[25]-0x33);
            qDebug()<<"B相电压: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVB->setText(textVB);
            //C相
            data_tmp[30] = convertCharToHex(buffer[66-8]);
            data_tmp[31] = convertCharToHex(buffer[67-8]);
            data_tmp[32] = convertCharToHex(buffer[68-8]);
            data_tmp[33] = convertCharToHex(buffer[69-8]);
            data_tmp[34] = convertCharToHex(buffer[70-8]);
            data_tmp[35] = convertCharToHex(buffer[71-8]);
            quint16 Vc = BcdToHex(data_tmp[32]*16+data_tmp[33]-0x33)*100+BcdToHex(data_tmp[30]*16+data_tmp[31]-0x33);
            qDebug()<<"C相电压: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_StateChangeVC->setText(textVC);
        }

        switch (SwitchDirection) {
        case 1:
            ui->lineEdit_SwitchChangeState->setText("分合");
            break;
        case 2:
            ui->lineEdit_SwitchChangeState->setText("合分");
            break;
        default:
            ui->lineEdit_SwitchChangeState->setText("无效");
            break;
        }

        switch (FaultReason) {
        case 0:
            ui->lineEdit_StateChangeReason->setText("恢复正常");
            break;
        case 6:
            ui->lineEdit_StateChangeReason->setText("过压");
            break;
        case 7:
            ui->lineEdit_StateChangeReason->setText("欠压");
            break;
        case 8:
            ui->lineEdit_StateChangeReason->setText("缺相");
            break;
        case 21:
            ui->lineEdit_StateChangeReason->setText("孤岛");
            break;
        case 23:
            ui->lineEdit_StateChangeReason->setText("远程分闸");
            break;
        case 24:
            ui->lineEdit_StateChangeReason->setText("远程合闸");
            break;
        case 25:
            ui->lineEdit_StateChangeReason->setText("手动分闸");
            break;
        case 26:
            ui->lineEdit_StateChangeReason->setText("手动合闸");
            break;
        case 33:
            ui->lineEdit_StateChangeReason->setText("失压");
            break;
        case 34:
            ui->lineEdit_StateChangeReason->setText("过欠频");
            break;
        default:
            ui->lineEdit_StateChangeReason->setText("未知");
            break;
        }

        switch (FaultPhase) {
        case 0:
            ui->lineEdit_StateChangePhase->setText("NULL");
            break;
        case 1:
            ui->lineEdit_StateChangePhase->setText("A");
            break;
        case 2:
            ui->lineEdit_StateChangePhase->setText("B");
            break;
        case 3:
            ui->lineEdit_StateChangePhase->setText("AB");
            break;
        case 4:
            ui->lineEdit_StateChangePhase->setText("C");
            break;
        case 5:
            ui->lineEdit_StateChangePhase->setText("AC");
            break;
        case 6:
            ui->lineEdit_StateChangePhase->setText("BC");
            break;
        case 7:
            ui->lineEdit_StateChangePhase->setText("ABC");
            break;
        default:
            ui->lineEdit_StateChangePhase->setText("异常");
            break;
        }
    }
    else if(isIslandEventNo)
    {
        isIslandEventNo = false;

        char buffer[128] = {0};
        quint8 data_tmp[68] = {0};

        quint8 FaultPhase = 0;
        quint8 Record_year = 0;
        quint8  Record_month = 0;
        quint8  Record_day = 0;
        quint8  Record_hour = 0;
        quint8  Record_min = 0;
        quint8  Record_sec = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Record_year = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            Record_month = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            Record_day = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[6] = convertCharToHex(buffer[42]);
            data_tmp[7] = convertCharToHex(buffer[43]);
            Record_hour = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[8] = convertCharToHex(buffer[44]);
            data_tmp[9] = convertCharToHex(buffer[45]);
            Record_min = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[10] = convertCharToHex(buffer[46]);
            data_tmp[11] = convertCharToHex(buffer[47]);
            Record_sec = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_IslandEventTime->setText(out);

            data_tmp[12] = convertCharToHex(buffer[48]);
            data_tmp[13] = convertCharToHex(buffer[49]);
            FaultPhase = data_tmp[12]*16+data_tmp[13]-0x33;

            //A相幅值摆幅
            data_tmp[14] = convertCharToHex(buffer[50]);
            data_tmp[15] = convertCharToHex(buffer[51]);
            data_tmp[16] = convertCharToHex(buffer[52]);
            data_tmp[17] = convertCharToHex(buffer[53]);
            data_tmp[18] = convertCharToHex(buffer[54]);
            data_tmp[19] = convertCharToHex(buffer[55]);
            quint16 Va = BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33)*100+BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            qDebug()<<"A相幅值摆动: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingA->setText(textVA);
            //B相幅值摆幅
            data_tmp[20] = convertCharToHex(buffer[56]);
            data_tmp[21] = convertCharToHex(buffer[57]);
            data_tmp[22] = convertCharToHex(buffer[58]);
            data_tmp[23] = convertCharToHex(buffer[59]);
            data_tmp[24] = convertCharToHex(buffer[60]);
            data_tmp[25] = convertCharToHex(buffer[61]);
            quint16 Vb = BcdToHex(data_tmp[22]*16+data_tmp[23]-0x33)*100+BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33);
            qDebug()<<"B相幅值摆动: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingB->setText(textVB);
            //C相幅值摆幅
            data_tmp[26] = convertCharToHex(buffer[62]);
            data_tmp[27] = convertCharToHex(buffer[63]);
            data_tmp[28] = convertCharToHex(buffer[64]);
            data_tmp[29] = convertCharToHex(buffer[65]);
            data_tmp[30] = convertCharToHex(buffer[66]);
            data_tmp[31] = convertCharToHex(buffer[67]);
            quint16 Vc = BcdToHex(data_tmp[28]*16+data_tmp[29]-0x33)*100+BcdToHex(data_tmp[26]*16+data_tmp[27]-0x33);
            qDebug()<<"C相幅值摆动: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingC->setText(textVC);

            //A相频率摆动
            data_tmp[32] = convertCharToHex(buffer[68]);
            data_tmp[33] = convertCharToHex(buffer[69]);
            data_tmp[34] = convertCharToHex(buffer[70]);
            data_tmp[35] = convertCharToHex(buffer[71]);
            data_tmp[36] = convertCharToHex(buffer[72]);
            data_tmp[37] = convertCharToHex(buffer[73]);
            quint16 Fa = BcdToHex(data_tmp[34]*16+data_tmp[35]-0x33)*100+BcdToHex(data_tmp[32]*16+data_tmp[33]-0x33);
            qDebug()<<"A相频率摆动: "<<Fa;
            float FA = Fa/100.0;
            QString textFA = QString::number(FA, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingA->setText(textFA);
            //B相频率摆动
            data_tmp[38] = convertCharToHex(buffer[74]);
            data_tmp[39] = convertCharToHex(buffer[75]);
            data_tmp[40] = convertCharToHex(buffer[76]);
            data_tmp[41] = convertCharToHex(buffer[77]);
            data_tmp[42] = convertCharToHex(buffer[78]);
            data_tmp[43] = convertCharToHex(buffer[79]);
            quint16 Fb = BcdToHex(data_tmp[40]*16+data_tmp[41]-0x33)*100+BcdToHex(data_tmp[38]*16+data_tmp[39]-0x33);
            qDebug()<<"B相频率摆动: "<<Fb;
            float FB = Fb/100.0;
            QString textFB = QString::number(FB, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingB->setText(textFB);
            //C相频率摆动
            data_tmp[44] = convertCharToHex(buffer[80]);
            data_tmp[45] = convertCharToHex(buffer[81]);
            data_tmp[46] = convertCharToHex(buffer[82]);
            data_tmp[47] = convertCharToHex(buffer[83]);
            data_tmp[48] = convertCharToHex(buffer[84]);
            data_tmp[49] = convertCharToHex(buffer[85]);
            quint16 Fc = BcdToHex(data_tmp[46]*16+data_tmp[47]-0x33)*100+BcdToHex(data_tmp[44]*16+data_tmp[45]-0x33);
            qDebug()<<"C相频率摆动: "<<Fc;
            float FC = Fc/100.0;
            QString textFC = QString::number(FC, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingC->setText(textFC);

            //A相频率超范围
            data_tmp[50] = convertCharToHex(buffer[86]);
            data_tmp[51] = convertCharToHex(buffer[87]);
            data_tmp[52] = convertCharToHex(buffer[88]);
            data_tmp[53] = convertCharToHex(buffer[89]);
            data_tmp[54] = convertCharToHex(buffer[90]);
            data_tmp[55] = convertCharToHex(buffer[91]);
            quint16 Faa = BcdToHex(data_tmp[52]*16+data_tmp[53]-0x33)*100+BcdToHex(data_tmp[50]*16+data_tmp[51]-0x33);
            qDebug()<<"A相频率超范围: "<<Faa;
            float FAA = Faa/100.0;
            QString textFAA = QString::number(FAA, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutA->setText(textFAA);
            //B相频率超范围
            data_tmp[56] = convertCharToHex(buffer[92]);
            data_tmp[57] = convertCharToHex(buffer[93]);
            data_tmp[58] = convertCharToHex(buffer[94]);
            data_tmp[59] = convertCharToHex(buffer[95]);
            data_tmp[60] = convertCharToHex(buffer[96]);
            data_tmp[61] = convertCharToHex(buffer[97]);
            quint16 Fbb = BcdToHex(data_tmp[58]*16+data_tmp[59]-0x33)*100+BcdToHex(data_tmp[56]*16+data_tmp[57]-0x33);
            qDebug()<<"B相频率超范围: "<<Fbb;
            float FBB = Fbb/100.0;
            QString textFBB = QString::number(FBB, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutB->setText(textFBB);
            //C相频率超范围
            data_tmp[62] = convertCharToHex(buffer[98]);
            data_tmp[63] = convertCharToHex(buffer[99]);
            data_tmp[64] = convertCharToHex(buffer[100]);
            data_tmp[65] = convertCharToHex(buffer[101]);
            data_tmp[66] = convertCharToHex(buffer[102]);
            data_tmp[67] = convertCharToHex(buffer[103]);
            quint16 Fcc = BcdToHex(data_tmp[64]*16+data_tmp[65]-0x33)*100+BcdToHex(data_tmp[62]*16+data_tmp[63]-0x33);
            qDebug()<<"C相频率超范围: "<<Fcc;
            float FCC = Fcc/100.0;
            QString textFCC = QString::number(FCC, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutC->setText(textFCC);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[36-8]);
            data_tmp[1] = convertCharToHex(buffer[37-8]);
            Record_year = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[2] = convertCharToHex(buffer[38-8]);
            data_tmp[3] = convertCharToHex(buffer[39-8]);
            Record_month = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[4] = convertCharToHex(buffer[40-8]);
            data_tmp[5] = convertCharToHex(buffer[41-8]);
            Record_day = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[6] = convertCharToHex(buffer[42-8]);
            data_tmp[7] = convertCharToHex(buffer[43-8]);
            Record_hour = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[8] = convertCharToHex(buffer[44-8]);
            data_tmp[9] = convertCharToHex(buffer[45-8]);
            Record_min = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[10] = convertCharToHex(buffer[46-8]);
            data_tmp[11] = convertCharToHex(buffer[47-8]);
            Record_sec = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_IslandEventTime->setText(out);

            data_tmp[12] = convertCharToHex(buffer[48-8]);
            data_tmp[13] = convertCharToHex(buffer[49-8]);
            FaultPhase = data_tmp[12]*16+data_tmp[13]-0x33;

            //A相幅值摆幅
            data_tmp[14] = convertCharToHex(buffer[50-8]);
            data_tmp[15] = convertCharToHex(buffer[51-8]);
            data_tmp[16] = convertCharToHex(buffer[52-8]);
            data_tmp[17] = convertCharToHex(buffer[53-8]);
            data_tmp[18] = convertCharToHex(buffer[54-8]);
            data_tmp[19] = convertCharToHex(buffer[55-8]);
            quint16 Va = BcdToHex(data_tmp[16]*16+data_tmp[17]-0x33)*100+BcdToHex(data_tmp[14]*16+data_tmp[15]-0x33);
            qDebug()<<"A相幅值摆动: "<<Va;
            float VA = Va/10.0;
            QString textVA = QString::number(VA, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingA->setText(textVA);
            //B相幅值摆幅
            data_tmp[20] = convertCharToHex(buffer[56-8]);
            data_tmp[21] = convertCharToHex(buffer[57-8]);
            data_tmp[22] = convertCharToHex(buffer[58-8]);
            data_tmp[23] = convertCharToHex(buffer[59-8]);
            data_tmp[24] = convertCharToHex(buffer[60-8]);
            data_tmp[25] = convertCharToHex(buffer[61-8]);
            quint16 Vb = BcdToHex(data_tmp[22]*16+data_tmp[23]-0x33)*100+BcdToHex(data_tmp[20]*16+data_tmp[21]-0x33);
            qDebug()<<"B相幅值摆动: "<<Vb;
            float VB = Vb/10.0;
            QString textVB = QString::number(VB, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingB->setText(textVB);
            //C相幅值摆幅
            data_tmp[26] = convertCharToHex(buffer[62-8]);
            data_tmp[27] = convertCharToHex(buffer[63-8]);
            data_tmp[28] = convertCharToHex(buffer[64-8]);
            data_tmp[29] = convertCharToHex(buffer[65-8]);
            data_tmp[30] = convertCharToHex(buffer[66-8]);
            data_tmp[31] = convertCharToHex(buffer[67-8]);
            quint16 Vc = BcdToHex(data_tmp[28]*16+data_tmp[29]-0x33)*100+BcdToHex(data_tmp[26]*16+data_tmp[27]-0x33);
            qDebug()<<"C相幅值摆动: "<<Vc;
            float VC = Vc/10.0;
            QString textVC = QString::number(VC, 'f', 1); // 保留一位小数
            ui->lineEdit_IslandVoltSwingC->setText(textVC);

            //A相频率摆动
            data_tmp[32] = convertCharToHex(buffer[68-8]);
            data_tmp[33] = convertCharToHex(buffer[69-8]);
            data_tmp[34] = convertCharToHex(buffer[70-8]);
            data_tmp[35] = convertCharToHex(buffer[71-8]);
            data_tmp[36] = convertCharToHex(buffer[72-8]);
            data_tmp[37] = convertCharToHex(buffer[73-8]);
            quint16 Fa = BcdToHex(data_tmp[34]*16+data_tmp[35]-0x33)*100+BcdToHex(data_tmp[32]*16+data_tmp[33]-0x33);
            qDebug()<<"A相频率摆动: "<<Fa;
            float FA = Fa/100.0;
            QString textFA = QString::number(FA, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingA->setText(textFA);
            //B相频率摆动
            data_tmp[38] = convertCharToHex(buffer[74-8]);
            data_tmp[39] = convertCharToHex(buffer[75-8]);
            data_tmp[40] = convertCharToHex(buffer[76-8]);
            data_tmp[41] = convertCharToHex(buffer[77-8]);
            data_tmp[42] = convertCharToHex(buffer[78-8]);
            data_tmp[43] = convertCharToHex(buffer[79-8]);
            quint16 Fb = BcdToHex(data_tmp[40]*16+data_tmp[41]-0x33)*100+BcdToHex(data_tmp[38]*16+data_tmp[39]-0x33);
            qDebug()<<"B相频率摆动: "<<Fb;
            float FB = Fb/100.0;
            QString textFB = QString::number(FB, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingB->setText(textFB);
            //C相频率摆动
            data_tmp[44] = convertCharToHex(buffer[80-8]);
            data_tmp[45] = convertCharToHex(buffer[81-8]);
            data_tmp[46] = convertCharToHex(buffer[82-8]);
            data_tmp[47] = convertCharToHex(buffer[83-8]);
            data_tmp[48] = convertCharToHex(buffer[84-8]);
            data_tmp[49] = convertCharToHex(buffer[85-8]);
            quint16 Fc = BcdToHex(data_tmp[46]*16+data_tmp[47]-0x33)*100+BcdToHex(data_tmp[44]*16+data_tmp[45]-0x33);
            qDebug()<<"C相频率摆动: "<<Fc;
            float FC = Fc/100.0;
            QString textFC = QString::number(FC, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqSwingC->setText(textFC);

            //A相频率超范围
            data_tmp[50] = convertCharToHex(buffer[86-8]);
            data_tmp[51] = convertCharToHex(buffer[87-8]);
            data_tmp[52] = convertCharToHex(buffer[88-8]);
            data_tmp[53] = convertCharToHex(buffer[89-8]);
            data_tmp[54] = convertCharToHex(buffer[90-8]);
            data_tmp[55] = convertCharToHex(buffer[91-8]);
            quint16 Faa = BcdToHex(data_tmp[52]*16+data_tmp[53]-0x33)*100+BcdToHex(data_tmp[50]*16+data_tmp[51]-0x33);
            qDebug()<<"A相频率超范围: "<<Faa;
            float FAA = Faa/100.0;
            QString textFAA = QString::number(FAA, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutA->setText(textFAA);
            //B相频率超范围
            data_tmp[56] = convertCharToHex(buffer[92-8]);
            data_tmp[57] = convertCharToHex(buffer[93-8]);
            data_tmp[58] = convertCharToHex(buffer[94-8]);
            data_tmp[59] = convertCharToHex(buffer[95-8]);
            data_tmp[60] = convertCharToHex(buffer[96-8]);
            data_tmp[61] = convertCharToHex(buffer[97-8]);
            quint16 Fbb = BcdToHex(data_tmp[58]*16+data_tmp[59]-0x33)*100+BcdToHex(data_tmp[56]*16+data_tmp[57]-0x33);
            qDebug()<<"B相频率超范围: "<<Fbb;
            float FBB = Fbb/100.0;
            QString textFBB = QString::number(FBB, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutB->setText(textFBB);
            //C相频率超范围
            data_tmp[62] = convertCharToHex(buffer[98-8]);
            data_tmp[63] = convertCharToHex(buffer[99-8]);
            data_tmp[64] = convertCharToHex(buffer[100-8]);
            data_tmp[65] = convertCharToHex(buffer[101-8]);
            data_tmp[66] = convertCharToHex(buffer[102-8]);
            data_tmp[67] = convertCharToHex(buffer[103-8]);
            quint16 Fcc = BcdToHex(data_tmp[64]*16+data_tmp[65]-0x33)*100+BcdToHex(data_tmp[62]*16+data_tmp[63]-0x33);
            qDebug()<<"C相频率超范围: "<<Fcc;
            float FCC = Fcc/100.0;
            QString textFCC = QString::number(FCC, 'f', 2); // 保留2位小数
            ui->lineEdit_IslandFreqOutC->setText(textFCC);
        }

        switch (FaultPhase) {
        case 0:
            ui->lineEdit_IslandPhase->setText("NULL");
            break;
        case 1:
            ui->lineEdit_IslandPhase->setText("A");
            break;
        case 2:
            ui->lineEdit_IslandPhase->setText("B");
            break;
        case 3:
            ui->lineEdit_IslandPhase->setText("AB");
            break;
        case 4:
            ui->lineEdit_IslandPhase->setText("C");
            break;
        case 5:
            ui->lineEdit_IslandPhase->setText("AC");
            break;
        case 6:
            ui->lineEdit_IslandPhase->setText("BC");
            break;
        case 7:
            ui->lineEdit_IslandPhase->setText("ABC");
            break;
        default:
            ui->lineEdit_IslandPhase->setText("异常");
            break;
        }
    }
    else if(isSwitchingEventNo)
    {
        isSwitchingEventNo = false;

        char buffer[128] = {0};
        quint8 data_tmp[18] = {0};

        quint8 InputCompare = 0; //投入定值
        quint8 SwitchingType = 0; //投退类别
        quint8 SwitchingWay = 0;  //投退方式

        quint8 LastInputValue = 0; //上一次投入使用的定值
        quint8 CurrentInputValue = 0;//当前要投入使用的定值

        quint8  Record_year = 0;
        quint8  Record_month = 0;
        quint8  Record_day = 0;
        quint8  Record_hour = 0;
        quint8  Record_min = 0;
        quint8  Record_sec = 0;

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[36]);
            data_tmp[1] = convertCharToHex(buffer[37]);
            Record_year = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[2] = convertCharToHex(buffer[38]);
            data_tmp[3] = convertCharToHex(buffer[39]);
            Record_month = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[4] = convertCharToHex(buffer[40]);
            data_tmp[5] = convertCharToHex(buffer[41]);
            Record_day = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[6] = convertCharToHex(buffer[42]);
            data_tmp[7] = convertCharToHex(buffer[43]);
            Record_hour = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[8] = convertCharToHex(buffer[44]);
            data_tmp[9] = convertCharToHex(buffer[45]);
            Record_min = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[10] = convertCharToHex(buffer[46]);
            data_tmp[11] = convertCharToHex(buffer[47]);
            Record_sec = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_SwitchingEventTime->setText(out);

            data_tmp[12] = convertCharToHex(buffer[48]);
            data_tmp[13] = convertCharToHex(buffer[49]);
            InputCompare = data_tmp[12]*16+data_tmp[13]-0x33;
            CurrentInputValue = InputCompare & 0x03;
            LastInputValue = (InputCompare & 0x0C) >> 2;
            QString text_LastInputValue = QString::number(LastInputValue);
            ui->lineEdit_LastSwitchingValue->setText(text_LastInputValue);
            QString text_CurrentInputValue = QString::number(CurrentInputValue);
            ui->lineEdit_CurrentSwitchingValue->setText(text_CurrentInputValue);

            data_tmp[14] = convertCharToHex(buffer[50]);
            data_tmp[15] = convertCharToHex(buffer[51]);
            SwitchingType = data_tmp[14]*16+data_tmp[15]-0x33;
            switch (SwitchingType) {
            case 33: //断电跳闸（失压）保护
                ui->lineEdit_SwitchingType->setText("断电跳闸");
                break;
            case 6: //过压保护
                ui->lineEdit_SwitchingType->setText("过压");
                break;
            case 7: //欠压保护
                ui->lineEdit_SwitchingType->setText("欠压");
                break;
            case 8: //缺相保护
                ui->lineEdit_SwitchingType->setText("缺相");
                break;
            case 21: //防孤岛保护
                ui->lineEdit_SwitchingType->setText("孤岛");
                break;
            case 34: //过欠频保护
                ui->lineEdit_SwitchingType->setText("过欠频");
                break;
            default:
                ui->lineEdit_SwitchingType->setText("Err");
                break;
            }

            data_tmp[16] = convertCharToHex(buffer[52]);
            data_tmp[17] = convertCharToHex(buffer[53]);
            SwitchingWay = data_tmp[16]*16+data_tmp[17]-0x33;
            switch (SwitchingWay) {
            case 1:
                ui->lineEdit_SwitchingWay->setText("远方");
                break;
            case 0:
                ui->lineEdit_SwitchingWay->setText("本地");
                break;
            default:
                ui->lineEdit_SwitchingWay->setText("Err");
                break;
            }
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[36-8]);
            data_tmp[1] = convertCharToHex(buffer[37-8]);
            Record_year = BcdToHex(data_tmp[0]*16+data_tmp[1]-0x33);
            qDebug()<<"Record_year: "<<Record_year;
            QString text_year = QString::number(Record_year);
            data_tmp[2] = convertCharToHex(buffer[38-8]);
            data_tmp[3] = convertCharToHex(buffer[39-8]);
            Record_month = BcdToHex(data_tmp[2]*16+data_tmp[3]-0x33);
            qDebug()<<"Record_month: "<<Record_month;
            QString text_month = QString::number(Record_month);
            data_tmp[4] = convertCharToHex(buffer[40-8]);
            data_tmp[5] = convertCharToHex(buffer[41-8]);
            Record_day = BcdToHex(data_tmp[4]*16+data_tmp[5]-0x33);
            QString text_day = QString::number(Record_day);
            data_tmp[6] = convertCharToHex(buffer[42-8]);
            data_tmp[7] = convertCharToHex(buffer[43-8]);
            Record_hour = BcdToHex(data_tmp[6]*16+data_tmp[7]-0x33);
            QString text_hour = QString::number(Record_hour);
            data_tmp[8] = convertCharToHex(buffer[44-8]);
            data_tmp[9] = convertCharToHex(buffer[45-8]);
            Record_min = BcdToHex(data_tmp[8]*16+data_tmp[9]-0x33);
            QString text_min = QString::number(Record_min);
            data_tmp[10] = convertCharToHex(buffer[46-8]);
            data_tmp[11] = convertCharToHex(buffer[47-8]);
            Record_sec = BcdToHex(data_tmp[10]*16+data_tmp[11]-0x33);
            QString text_sec = QString::number(Record_sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_SwitchingEventTime->setText(out);

            data_tmp[12] = convertCharToHex(buffer[48-8]);
            data_tmp[13] = convertCharToHex(buffer[49-8]);
            InputCompare = data_tmp[12]*16+data_tmp[13]-0x33;
            CurrentInputValue = InputCompare & 0x03;
            LastInputValue = (InputCompare & 0x0C) >> 2;
            QString text_LastInputValue = QString::number(LastInputValue);
            ui->lineEdit_LastSwitchingValue->setText(text_LastInputValue);
            QString text_CurrentInputValue = QString::number(CurrentInputValue);
            ui->lineEdit_CurrentSwitchingValue->setText(text_CurrentInputValue);

            data_tmp[14] = convertCharToHex(buffer[50-8]);
            data_tmp[15] = convertCharToHex(buffer[51-8]);
            SwitchingType = data_tmp[14]*16+data_tmp[15]-0x33;
            switch (SwitchingType) {
            case 33: //断电跳闸（失压）保护
                ui->lineEdit_SwitchingType->setText("断电跳闸");
                break;
            case 6: //过压保护
                ui->lineEdit_SwitchingType->setText("过压");
                break;
            case 7: //欠压保护
                ui->lineEdit_SwitchingType->setText("欠压");
                break;
            case 8: //缺相保护
                ui->lineEdit_SwitchingType->setText("缺相");
                break;
            case 21: //防孤岛保护
                ui->lineEdit_SwitchingType->setText("孤岛");
                break;
            case 34: //过欠频保护
                ui->lineEdit_SwitchingType->setText("过欠频");
                break;
            default:
                ui->lineEdit_SwitchingType->setText("Err");
                break;
            }

            data_tmp[16] = convertCharToHex(buffer[52-8]);
            data_tmp[17] = convertCharToHex(buffer[53-8]);
            SwitchingWay = data_tmp[16]*16+data_tmp[17]-0x33;
            switch (SwitchingWay) {
            case 1:
                ui->lineEdit_SwitchingWay->setText("远方");
                break;
            case 0:
                ui->lineEdit_SwitchingWay->setText("本地");
                break;
            default:
                ui->lineEdit_SwitchingWay->setText("Err");
                break;
            }
        }
    }
    else if(isCloseStatusCheckSend)
    {
        isCloseStatusCheckSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 RunWord1;

        QByteArray byteArray = RecData_tmp.toUtf8();

//        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
//        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            RunWord1 = convertCharToHex(buffer[56]);
            quint8 bit_value = (RunWord1>>2) & 1;//bit1
            if(0 == bit_value)//合闸
            {
                CloseCheckOK_times++;
                QString text_data = QString::number(CloseCheckOK_times);
                ui->lineEdit_AutoCloseSuccessCnt->setText(text_data);

            }
//                ui->lineEdit_statusRealstate->setText("合闸");
            else{

            }
//                ui->lineEdit_statusRealstate->setText("分闸");
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            RunWord1 = convertCharToHex(buffer[48]);
            quint8 bit_value = (RunWord1>>2) & 1;//bit1
            if(0 == bit_value)
            {
                CloseCheckOK_times++;
                QString text_data = QString::number(CloseCheckOK_times);
                ui->lineEdit_AutoCloseSuccessCnt->setText(text_data);
            }
//                ui->lineEdit_statusRealstate->setText("合闸");
            else{

            }
//                ui->lineEdit_statusRealstate->setText("分闸");
        }
    }
    else if(isResetConfigSend)
    {
        isResetConfigSend = false;

        //检查返回帧是否写入成功
        char buffer[128] = {0};
        quint8 Write_feedback;

        QByteArray byteArray = RecData_tmp.toUtf8();

        qDebug()<<"转换为utf8数组个数: "<<byteArray.size();
        qDebug()<<"转换为utf8数据: "<<byteArray.constData();

        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            Write_feedback = convertCharToHex(buffer[24]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "恢复出厂配置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "恢复出厂配置失败");
            }

        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            Write_feedback = convertCharToHex(buffer[16]);
            if(9==Write_feedback)//设置成功
            {
                QMessageBox::information(this,"Message", "恢复出厂配置成功");
            }
            else
            {
                QMessageBox::critical(this,"Message", "恢复出厂配置失败");
            }
        }
    }
    else if(isGetDeviceTypeSend)
    {
        isGetDeviceTypeSend = false;

        char buffer[128] = {0};
        quint8 data_tmp[22] = {0};//设备类型11字节

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('F' == buffer[0] && 'E' == buffer[1]) || ('f' == buffer[0] && 'e' == buffer[1])) //FE开头
        {
            data_tmp[0] = convertCharToHex(buffer[58]);
            data_tmp[1] = convertCharToHex(buffer[59]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[56]);
            data_tmp[3] = convertCharToHex(buffer[57]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[54]);
            data_tmp[5] = convertCharToHex(buffer[55]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[52]);
            data_tmp[7] = convertCharToHex(buffer[53]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[50]);
            data_tmp[9] = convertCharToHex(buffer[51]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[48]);
            data_tmp[11] = convertCharToHex(buffer[49]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            data_tmp[12] = convertCharToHex(buffer[46]);
            data_tmp[13] = convertCharToHex(buffer[47]);
            QChar v7 =data_tmp[12]*16+data_tmp[13]-0x33;

            data_tmp[14] = convertCharToHex(buffer[44]);
            data_tmp[15] = convertCharToHex(buffer[45]);
            QChar v8 =data_tmp[14]*16+data_tmp[15]-0x33;

            data_tmp[16] = convertCharToHex(buffer[42]);
            data_tmp[17] = convertCharToHex(buffer[43]);
            QChar v9 =data_tmp[16]*16+data_tmp[17]-0x33;

            data_tmp[18] = convertCharToHex(buffer[40]);
            data_tmp[19] = convertCharToHex(buffer[41]);
            QChar v10 =data_tmp[18]*16+data_tmp[19]-0x33;

            data_tmp[20] = convertCharToHex(buffer[38]);
            data_tmp[21] = convertCharToHex(buffer[39]);
            QChar v11 =data_tmp[20]*16+data_tmp[21]-0x33;

            QString DeviceType = "";
            DeviceType = DeviceType+v1+v2+v3+v4+v5+v6+v7+v8+v9+v10+v11;
            ui->lineEdit_DeviceType->setText(DeviceType);
        }
        else if('6' == buffer[0] && '8' == buffer[1]) //68开头
        {
            data_tmp[0] = convertCharToHex(buffer[50]);
            data_tmp[1] = convertCharToHex(buffer[51]);
            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[2] = convertCharToHex(buffer[48]);
            data_tmp[3] = convertCharToHex(buffer[49]);
            QChar v2 =data_tmp[2]*16+data_tmp[3]-0x33;

            data_tmp[4] = convertCharToHex(buffer[46]);
            data_tmp[5] = convertCharToHex(buffer[47]);
            QChar v3 =data_tmp[4]*16+data_tmp[5]-0x33;

            data_tmp[6] = convertCharToHex(buffer[44]);
            data_tmp[7] = convertCharToHex(buffer[45]);
            QChar v4 =data_tmp[6]*16+data_tmp[7]-0x33;

            data_tmp[8] = convertCharToHex(buffer[42]);
            data_tmp[9] = convertCharToHex(buffer[43]);
            QChar v5 =data_tmp[8]*16+data_tmp[9]-0x33;

            data_tmp[10] = convertCharToHex(buffer[40]);
            data_tmp[11] = convertCharToHex(buffer[41]);
            QChar v6 =data_tmp[10]*16+data_tmp[11]-0x33;

            data_tmp[12] = convertCharToHex(buffer[38]);
            data_tmp[13] = convertCharToHex(buffer[39]);
            QChar v7 =data_tmp[12]*16+data_tmp[13]-0x33;

            data_tmp[14] = convertCharToHex(buffer[36]);
            data_tmp[15] = convertCharToHex(buffer[37]);
            QChar v8 =data_tmp[14]*16+data_tmp[15]-0x33;

            data_tmp[16] = convertCharToHex(buffer[34]);
            data_tmp[17] = convertCharToHex(buffer[35]);
            QChar v9 =data_tmp[16]*16+data_tmp[17]-0x33;

            data_tmp[18] = convertCharToHex(buffer[32]);
            data_tmp[19] = convertCharToHex(buffer[33]);
            QChar v10 =data_tmp[18]*16+data_tmp[19]-0x33;

            data_tmp[20] = convertCharToHex(buffer[30]);
            data_tmp[21] = convertCharToHex(buffer[31]);
            QChar v11 =data_tmp[20]*16+data_tmp[21]-0x33;

            QString DeviceType = "";
            DeviceType = DeviceType+v1+v2+v3+v4+v5+v6+v7+v8+v9+v10+v11;
            ui->lineEdit_DeviceType->setText(DeviceType);
        }
    }
    else if(isMbReadAddrCheck)
    {
        isMbReadAddrCheck = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
//            data_tmp[0] = convertCharToHex(buffer[58]);
//            data_tmp[1] = convertCharToHex(buffer[59]);
//            QChar v1 =data_tmp[0]*16+data_tmp[1]-0x33;

            data_tmp[0] = convertCharToHex(buffer[8]);
            data_tmp[1] = convertCharToHex(buffer[9]);
            quint8 v1 =data_tmp[0]*16+data_tmp[1];

            gModbus_Addr = v1; //更新全局地址

            QString textV = QString::number(v1, 'f', 0); // 无小数
            ui->lineEdit_MbReadAddr->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取地址成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取地址失败.");
        }
    }
    else if(isSendRemoteOpenCmd)
    {
        isSendRemoteOpenCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("远程分闸成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("远程分闸失败.");
        }
    }
    else if(isSendCloseLeakCmd)
    {
        isSendCloseLeakCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("关闭漏电保护成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("关闭漏电保护失败.");
        }
    }
    else if(isSendEnableLeakCmd)
    {
        isSendEnableLeakCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("开启漏电保护成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("开启漏电保护失败.");
        }
    }
    else if(isMbReadUaCmd)
    {
        isMbReadUaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];

            float fv = v1/100.0;

            QString textV = QString::number(fv, 'f', 2); // 电压两位小数
            ui->lineEdit_MbReadUa->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相电压成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相电压失败.");
        }
    }
    else if(isMbReadIaCmd)
    {
        isMbReadIaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            float ff = v1/1000.0;
            qDebug("Mb Ia Read = %f",ff);
            QString textV = QString::number(ff, 'f', 3); // 电流三位小数
            ui->lineEdit_MbReadIa->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相电流成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相电流失败.");
        }
    }
    else if(isMbReadFreqCmd)
    {
        isMbReadFreqCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];

            float ff = v1/100.0;
            QString textV = QString::number(ff, 'f', 2); // 频率两位小数
            ui->lineEdit_MbReadFreq->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相频率成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相频率失败.");
        }
    }
    else if(isMbReadPaCmd)
    {
        isMbReadPaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            if(data_tmp[0]<=7)//非负数
            {
                quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                float f1 = v1/10.0;

                if(f1>0 && f1<9) //小功率清零
                    f1 = 0;

                QString textV = QString::number(f1, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadPa->setText(textV);
            }
            else
            {
                quint32 v2 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                qint32 v3 = (qint32)v2;
                float f2 = v3/10.0;

                if(f2<0 && f2>(-10)) //小功率清零
                    f2 = 0;

                QString textV2 = QString::number(f2, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadPa->setText(textV2);
            }

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相有功功率成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相有功功率失败.");
        }
    }
    else if(isMbReadComboActECmd)
    {
        isMbReadComboActECmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0); // 总有功电能0位小数  单位Wh
            ui->lineEdit_MbReadComboAEAll->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总有功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总有功电能失败.");
        }
    }
    else if(isMbReadPosActECmd) //正有功电能
    {
        isMbReadPosActECmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0); // 有功电能0位小数  单位Wh
            ui->lineEdit_MbReadPosAEAll->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A正有功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A正有功电能失败.");
        }
    }
    else if(isMbReadNegActECmd)
    {
        isMbReadNegActECmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0); // 有功电能0位小数  单位Wh
            ui->lineEdit_MbReadNegAEAll->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A负有功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A负有功电能失败.");
        }
    }
    else if(isMbReadPFaCmd)  //功率因数 有正负
    {
        isMbReadPFaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            if(data_tmp[0]<=7)
            {
                quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
                float ff = v1/1000.0;
                QString textV = QString::number(ff, 'f', 3);
                ui->lineEdit_MbReadPFa->setText(textV);
            }
            else //负数
            {
                quint16 v2 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
                qint16 v3 = (qint16)v2;
                float fff = v3/1000.0;
                QString textV2 = QString::number(fff, 'f', 3);
                ui->lineEdit_MbReadPFa->setText(textV2);
            }


            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相功率因数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相功率因数失败.");
        }

    }
    else if(isReadLcdDisplayIntervalTimeCmd)
    {
        isReadLcdDisplayIntervalTimeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[8]);
            data_tmp[1] = convertCharToHex(buffer[9]);
            quint8 v1 =data_tmp[0]*16+data_tmp[1];

            QString textV = QString::number(v1, 'f', 0); // 无小数
            ui->lineEdit_ReadLcdDisplayTime->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取LCD轮显间隔时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取LCD轮显间隔时间失败.");
        }
    }
    else if(isReadOVThresCmd)
    {
        isReadOVThresCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadOVThres->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取过压动作阈值成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取过压动作阈值失败.");
        }
    }
    else if(isMbReadOVTimeCmd)
    {
        isMbReadOVTimeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadOVTime->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取过压动作时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取过压动作时间失败.");
        }
    }
    else if(isWriteLcdIntervalTimeCmd)
    {
        isWriteLcdIntervalTimeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("设置LCD轮显间隔时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("设置LCD轮显间隔时间失败.");
        }
    }
    else if(isMbReadSysTimeCmd)
    {
        isMbReadSysTimeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[12] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            quint16 year =data_tmp[0]*16+data_tmp[1] + 2000;
            QString text_year = QString::number(year);

            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 month =data_tmp[2]*16+data_tmp[3];
            QString text_month = QString::number(month);

            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            quint16 day =data_tmp[4]*16+data_tmp[5];
            QString text_day = QString::number(day);

            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint16 hour =data_tmp[6]*16+data_tmp[7];
            QString text_hour = QString::number(hour);

            data_tmp[8] = convertCharToHex(buffer[14]);
            data_tmp[9] = convertCharToHex(buffer[15]);
            quint16 min =data_tmp[8]*16+data_tmp[9];
            QString text_min = QString::number(min);

            data_tmp[10] = convertCharToHex(buffer[16]);
            data_tmp[11] = convertCharToHex(buffer[17]);
            quint16 sec =data_tmp[10]*16+data_tmp[11];
            QString text_sec = QString::number(sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_MbReadSysTime->setText(out);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取系统时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取系统时间失败.");
        }
    }
    else if(isMbGetWinTime2SetCmd)
    {
        isMbGetWinTime2SetCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("获取电脑时间并校时成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("获取电脑时间并校时失败.");
        }
    }
    else if(isMbManuTimeSetCmd)
    {
        isMbManuTimeSetCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("手动校时成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("手动校时失败.");
        }
    }
    else if(isMbSetOVThresCmd)
    {
        isMbSetOVThresCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写过压阈值成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写过压阈值失败.");
        }
    }
    else if(isMbSetOVTimeCmd)
    {
        isMbSetOVTimeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写过压动作时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写过压动作时间失败.");
        }

    }
    else if(isMbWriteAddrCmd)
    {
        isMbWriteAddrCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写通信地址成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写通信地址失败.");
        }
    }
    else if(isMbReadSoftVerCmd)
    {
        isMbReadSoftVerCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3]))
        {
            QString SoftVer = "";
            SoftVer = SoftVer+buffer[6]+buffer[7]+"."+buffer[8]+buffer[9]+"."+buffer[10]+buffer[11]+"."+buffer[12]+buffer[13];
            ui->lineEdit_MbReadSoftVer->setText(SoftVer);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读软件版本成功.");
        }
        else
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读软件版本失败.");
        }
    }
    else if(isMbReadSNPCodeCmd)
    {
        isMbReadSNPCodeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3]))
        {
            QString SN = "";
            SN = SN+buffer[6]+buffer[7]+" "+buffer[8]+buffer[9]+" "+buffer[10]+buffer[11]+" "+buffer[12]+buffer[13]+" "+buffer[14]+buffer[15]+" "+buffer[16]+buffer[17] \
                    +" "+buffer[18]+buffer[19]+" "+buffer[20]+buffer[21]+" "+buffer[22]+buffer[23]+" "+buffer[24]+buffer[25]+" "+buffer[26]+buffer[27]+" "+buffer[28]+buffer[29];
            ui->lineEdit_MbReadSN_PCODE->setText(SN);

            QString In = "";
            In = In+buffer[12]+buffer[13];
            if("98" == In)
                ui->lineEdit_InValue->setText("63");
            else if("99" == In)
                ui->lineEdit_InValue->setText("80");
            else if("9A" == In)
                ui->lineEdit_InValue->setText("100");
            else if("9B" == In)
                ui->lineEdit_InValue->setText("125");
            else
                ui->lineEdit_InValue->setText("未知");

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读SN+PCODE成功.");
        }
        else
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读SN+PCODE失败.");
        }
    }
    else if(isMbWriteSNPcodeCmd)
    {
        isMbWriteSNPcodeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写SN+PCODE成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写SN+PCODE失败.");
        }
    }
    else if(isMbSoftResetCmd)
    {
        isMbSoftResetCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("软件复位成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("软件复位失败.");
        }
    }
    else if(isMbClearAllEnergyCmd)
    {
        isMbClearAllEnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("清除所有电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("清除所有电能失败.");
        }
    }
    else if(isMbStartCaliCmd)
    {
        isMbStartCaliCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("进入校准模式成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("进入校准模式失败.");
        }
    }
    else if(isMbExitCaliCmd)
    {
        isMbExitCaliCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("退出校准模式成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("退出校准模式失败.");
        }
    }
    else if(isMbCaliIaCmd)
    {
        isMbCaliIaCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("电流校准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("电流校准失败.");
        }
    }
    else if(isMbCaliUaCmd)
    {
        isMbCaliUaCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("电压校准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("电压校准失败.");
        }
    }
    else if(isMbCaliPaCmd)
    {
        isMbCaliPaCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("功率校准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("功率校准失败.");
        }
    }
    else if(isMbRs485ParaSetCmd)
    {
        isMbRs485ParaSetCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("RS485波特率和校验位设置成功.");

            effect_ComChanged->setVolume(1.0f);
            effect_ComChanged->play();
            QMessageBox::information(this,"Message", "通信参数写入成功，请关闭串口并切换到新波特率和校验位，再打开串口");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("RS485波特率和校验位设置失败.");

            QMessageBox::critical(this,"Message", "RS485波特率和校验位设置失败");
        }
    }
    else if(isMbCheckProtectBitCmd)
    {
        isMbCheckProtectBitCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            if('0' == buffer[6] && '0' == buffer[7] && '0' == buffer[8])
            {
                quint8 ProtectBit = convertCharToHex(buffer[9]);
                if(ProtectBit & 0x01)
                    ui->checkBox_MbOVProtectBit->setChecked(true);
                else
                    ui->checkBox_MbOVProtectBit->setChecked(false);

                if(ProtectBit & 0x02)
                    ui->checkBox_MbUVProtectBit->setChecked(true);
                else
                    ui->checkBox_MbUVProtectBit->setChecked(false);

                if(ProtectBit & 0x08)
                    ui->checkBox_MbOLProtectBit->setChecked(true);
                else
                    ui->checkBox_MbOLProtectBit->setChecked(false);

                if(ui->checkBox_PrintOkRet->isChecked())
                    ui->textEdit_MbInfo->append("读取保护使能位成功.");
            }
            else
            {
                if(ui->checkBox_PrintOkRet->isChecked())
                    ui->textEdit_MbInfo->append("<font color='red'>读取保护使能位数据异常.</font>");  //append("<font color='red'>这是红色的文本</font>");
            }


        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("<font color='red'>读取保护使能位失败.</font>");
        }
    }
    else if(isMbWriteProtectBitCmd)
    {
        isMbWriteProtectBitCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("设置保护使能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("设置保护使能失败.");
        }
    }
    else if(isMbReadTempCmd)
    {
        isMbReadTempCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            qint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];

            QString textV = QString::number(v1, 'f', 0); // 无小数
            ui->lineEdit_MbReadTemp->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取内部温度成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取内部温度失败.");
        }
    }
    else if(isMbReadSoftTimeCmd)
    {
        isMbReadSoftTimeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3]))
        {
            QString SN = "20";
            SN = SN+buffer[6]+buffer[7]+"-"+buffer[8]+buffer[9]+"-"+buffer[10]+buffer[11]+" "+buffer[12]+buffer[13]+":"+buffer[14]+buffer[15]+":"+buffer[16]+buffer[17];
            ui->lineEdit_MbReadSoftTime->setText(SN);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读软件发布日期成功.");
        }
        else
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读软件发布日期失败.");
        }
    }
    else if(isMbReadLifeCmd)
    {
        isMbReadLifeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadLife->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取总分闸次数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取总分闸次数失败.");
        }
    }
    else if(isMbReadLeakLifeCmd)
    {
        isMbReadLeakLifeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadLeakLife->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取漏电分闸次数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取漏电分闸次数失败.");
        }
    }
    else if(isMbReadEventRecordCmd)
    {
        isMbReadEventRecordCmd = false;

        char buffer[128] = {0};
//        quint8 data_tmp[20] = {0};
        quint8 data_tmp[24] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[8]);
            data_tmp[1] = convertCharToHex(buffer[9]);
            quint8 Reason = data_tmp[0]*16+data_tmp[1];
            qDebug("Reason = %d\n",Reason);
            switch(Reason)
            {
                case 1:
                    ui->lineEdit_MbEventReason->setText("励磁输入");
                    break;
                case 2:
                    ui->lineEdit_MbEventReason->setText("过载");
                    break;
                case 4:
                    ui->lineEdit_MbEventReason->setText("过压");
                    break;
                case 16:
                    ui->lineEdit_MbEventReason->setText("遥控分闸");
                    break;
                case 32:
                    ui->lineEdit_MbEventReason->setText("漏电流");
                    break;
                case 64:
                    ui->lineEdit_MbEventReason->setText("欠压");
                    break;
                default:
                    ui->lineEdit_MbEventReason->setText("未知或异常");
                    break;
            }

            data_tmp[2] = convertCharToHex(buffer[10]);
            data_tmp[3] = convertCharToHex(buffer[11]);
            quint16 year =data_tmp[2]*16+data_tmp[3] + 2000;
            QString text_year = QString::number(year);

            data_tmp[4] = convertCharToHex(buffer[12]);
            data_tmp[5] = convertCharToHex(buffer[13]);
            quint16 month =data_tmp[4]*16+data_tmp[5];
            QString text_month = QString::number(month);

            data_tmp[6] = convertCharToHex(buffer[14]);
            data_tmp[7] = convertCharToHex(buffer[15]);
            quint16 day =data_tmp[6]*16+data_tmp[7];
            QString text_day = QString::number(day);

            data_tmp[8] = convertCharToHex(buffer[16]);
            data_tmp[9] = convertCharToHex(buffer[17]);
            quint16 hour =data_tmp[8]*16+data_tmp[9];
            QString text_hour = QString::number(hour);

            data_tmp[10] = convertCharToHex(buffer[18]);
            data_tmp[11] = convertCharToHex(buffer[19]);
            quint16 min =data_tmp[10]*16+data_tmp[11];
            QString text_min = QString::number(min);

            data_tmp[12] = convertCharToHex(buffer[20]);
            data_tmp[13] = convertCharToHex(buffer[21]);
            quint16 sec =data_tmp[12]*16+data_tmp[13];
            QString text_sec = QString::number(sec);

            QString out = text_year+"-"+text_month+"-"+text_day+" "+text_hour+":"+text_min+":"+text_sec;
            ui->lineEdit_MbEventTime->setText(out);

            data_tmp[14] = convertCharToHex(buffer[24]);
            data_tmp[15] = convertCharToHex(buffer[25]);
            quint8 Status = data_tmp[14]*16+data_tmp[15];
            if(3==Status) //11
                ui->lineEdit_MbCheckCaliTime->setText("合闸/已校时");
            else if(1==data_tmp[14]) //01
                ui->lineEdit_MbCheckCaliTime->setText("合闸/未校时");
            else if(0==data_tmp[14]) //00
                ui->lineEdit_MbCheckCaliTime->setText("分闸/未校时");
            else if(2==data_tmp[14]) //10
                ui->lineEdit_MbCheckCaliTime->setText("分闸/已校时");
            else
                ui->lineEdit_MbCheckCaliTime->setText("未知");


            if(4==Reason) //过压
            {
                data_tmp[16] = convertCharToHex(buffer[28]);
                data_tmp[17] = convertCharToHex(buffer[29]);
                data_tmp[18] = convertCharToHex(buffer[26]);
                data_tmp[19] = convertCharToHex(buffer[27]);
                quint16 v1 =data_tmp[16]*4096+data_tmp[17]*256+data_tmp[18]*16+data_tmp[19];
                float ff = v1/100.0;
                QString textV = QString::number(ff, 'f', 2);
                ui->lineEdit_MbEventValue->setText(textV);
            }
            else if(2==Reason) //过载
            {
//                data_tmp[16] = convertCharToHex(buffer[28]);
//                data_tmp[17] = convertCharToHex(buffer[29]);
//                data_tmp[18] = convertCharToHex(buffer[26]);
//                data_tmp[19] = convertCharToHex(buffer[27]);
//                quint16 v2 =data_tmp[16]*4096+data_tmp[17]*256+data_tmp[18]*16+data_tmp[19];

                //uint32数据
                data_tmp[16] = convertCharToHex(buffer[32]);
                data_tmp[17] = convertCharToHex(buffer[33]);
                data_tmp[18] = convertCharToHex(buffer[30]);
                data_tmp[19] = convertCharToHex(buffer[31]);
                data_tmp[20] = convertCharToHex(buffer[28]);
                data_tmp[21] = convertCharToHex(buffer[29]);
                data_tmp[22] = convertCharToHex(buffer[26]);
                data_tmp[23] = convertCharToHex(buffer[27]);
                quint32 v2 =data_tmp[16]*268435456+data_tmp[17]*16777216+data_tmp[18]*1048576+data_tmp[19]*65536+data_tmp[20]*4096+data_tmp[21]*256+data_tmp[22]*16+data_tmp[23];

                float fff = v2/1000.0;
                QString textI = QString::number(fff, 'f', 3);
                ui->lineEdit_MbEventValue->setText(textI);
            }
            else if(32==Reason)
                ui->lineEdit_MbEventValue->setText("30");
            else if(64==Reason) //欠压
            {
                data_tmp[16] = convertCharToHex(buffer[28]);
                data_tmp[17] = convertCharToHex(buffer[29]);
                data_tmp[18] = convertCharToHex(buffer[26]);
                data_tmp[19] = convertCharToHex(buffer[27]);
                quint16 v1 =data_tmp[16]*4096+data_tmp[17]*256+data_tmp[18]*16+data_tmp[19];
                float ff = v1/100.0;
                QString textV = QString::number(ff, 'f', 2);
                ui->lineEdit_MbEventValue->setText(textV);
            }
            else
                ui->lineEdit_MbEventValue->setText("/");


            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取事件成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取事件失败.");
        }
    }
    else if(isMbReadPosAEbaseCmd)
    {
        isMbReadPosAEbaseCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadPosAE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取正有功电能基准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取正有功电能基准失败.");
        }
    }
    else if(isMbReadNegAEbaseCmd)
    {
        isMbReadNegAEbaseCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadNegAE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取负有功电能基准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取负有功电能基准失败.");
        }
    }
    else if(isMbReadPosREbaseCmd)
    {
        isMbReadPosREbaseCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadPosRE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取正无功电能基准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取正无功电能基准失败.");
        }
    }
    else if(isMbReadNegREbaseCmd)
    {
        isMbReadNegREbaseCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadNegRE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取负无功电能基准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取负无功电能基准失败.");
        }
    }
    else if(isMbReadAppEnergybaseCmd)
    {
        isMbReadAppEnergybaseCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadAppEnergy->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取视在电能基准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取视在电能基准失败.");
        }
    }
    else if(isMbWritePosAEnergyCmd)
    {
        isMbWritePosAEnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写正有功电能基准参数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写正有功电能基准参数失败.");
        }
    }
    else if(isMbWriteNegAEnergyCmd)
    {
        isMbWriteNegAEnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写负有功电能基准参数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写负有功电能基准参数失败.");
        }
    }
    else if(isMbWritePosREnergyCmd)
    {
        isMbWritePosREnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写正无功电能基准参数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写正无功电能基准参数失败.");
        }
    }
    else if(isMbWriteNegREnergyCmd)
    {
        isMbWriteNegREnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写负无功电能基准参数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写负无功电能基准参数失败.");
        }
    }
    else if(isMbWriteAppEnergyCmd)
    {
        isMbWriteAppEnergyCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写视在电能基准参数成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写视在电能基准参数失败.");
        }
    }
    else if(isMbReadRectEnergyAllCmd)
    {
        isMbReadRectEnergyAllCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadComboNEAll->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总无功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总无功电能失败.");
        }
    }
    else if(isMbReadAppEnergyAllCmd)
    {
        isMbReadAppEnergyAllCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadAppEngA->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总视在电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A总视在电能失败.");
        }
    }
    else if(isMbReadRectPaCmd)
    {
        isMbReadRectPaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            if(data_tmp[0]<=7)//非负数
            {
                quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                float f1 = v1/10.0;

                QString textV = QString::number(f1, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadNPa->setText(textV);
            }
            else
            {
                quint32 v2 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                qint32 v3 = (qint32)v2;
//                qint32 v3 = comp32_to_orig((qint32)v2);
                qDebug("isMbReadRectPaCmd v3 = 0x%8X",v3);
                float f2 = v3/10.0;

//                if(f2<0 && f2>(-20)) //小值清零
                if(f2<0 && f2>(-4)) //小值清零
                    f2 = 0;

                QString textV2 = QString::number(f2, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadNPa->setText(textV2);
            }

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相无功功率成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相无功功率失败.");
        }
    }
    else if(isMbReadAppPaCmd)
    {
        isMbReadAppPaCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            if(data_tmp[0]<=7)//非负数
            {
                quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                float f1 = v1/10.0;
                QString textV = QString::number(f1, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadAppPa->setText(textV);
            }
            else
            {
                quint32 v2 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];
                qint32 v3 = (qint32)v2;
                float f2 = v3/10.0;
                QString textV2 = QString::number(f2, 'f', 1); // 功率1位小数
                ui->lineEdit_MbReadAppPa->setText(textV2);
            }

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相视在功率成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A相视在功率失败.");
        }
    }
    else if(isMbReadRectEnergyCmd)
    {
        isMbReadRectEnergyCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0); // 有功电能0位小数  单位Wh
            ui->lineEdit_MbReadPosRectE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A正无功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A正无功电能失败.");
        }
    }
    else if(isMbReadNegRectEnergyCmd)
    {
        isMbReadNegRectEnergyCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[8] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            data_tmp[4] = convertCharToHex(buffer[10]);
            data_tmp[5] = convertCharToHex(buffer[11]);
            data_tmp[6] = convertCharToHex(buffer[12]);
            data_tmp[7] = convertCharToHex(buffer[13]);
            quint32 v1 =data_tmp[0]*268435456+data_tmp[1]*16777216+data_tmp[2]*1048576+data_tmp[3]*65536+data_tmp[4]*4096+data_tmp[5]*256+data_tmp[6]*16+data_tmp[7];

            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadNegRectE->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取A负无功电能成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取A负无功电能失败.");
        }
    }
    else if(isReadUVThresCmd)
    {
        isReadUVThresCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadUVThres->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取欠压动作阈值成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取欠压动作阈值失败.");
        }
    }
    else if(isMbReadUVTimeCmd)
    {
        isMbReadUVTimeCmd = false;

        char buffer[128] = {0};
        quint8 data_tmp[4] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[6]);
            data_tmp[1] = convertCharToHex(buffer[7]);
            data_tmp[2] = convertCharToHex(buffer[8]);
            data_tmp[3] = convertCharToHex(buffer[9]);
            quint16 v1 =data_tmp[0]*4096+data_tmp[1]*256+data_tmp[2]*16+data_tmp[3];
            QString textV = QString::number(v1, 'f', 0);
            ui->lineEdit_MbReadUVTime->setText(textV);

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取欠压动作时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取欠压动作时间失败.");
        }
    }
    else if(isMbSetUVThresCmd)
    {
        isMbSetUVThresCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写欠压阈值成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写欠压阈值失败.");
        }
    }
    else if(isMbSetUVTimeCmd)
    {
        isMbSetUVTimeCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("写欠压动作时间成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("写欠压动作时间失败.");
        }
    }
    else if(isMbReadCommCombined)
    {
        isMbReadCommCombined = false;

        char buffer[128] = {0};
        quint8 data_tmp[2] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '3' == buffer[3])) //返回成功
        {
            data_tmp[0] = convertCharToHex(buffer[8]);
            data_tmp[1] = convertCharToHex(buffer[9]);
            quint8 v1 =data_tmp[0]*16+data_tmp[1];
//            QString textV = QString::number(v1, 'f', 0);
//            ui->lineEdit_MbReadUVTime->setText(textV);

            switch(v1)
            {
                case 3:
                    ui->lineEdit_ReadCommCombined->setText("研+仪表");
                    break;
                case 7:
                    ui->lineEdit_ReadCommCombined->setText("研+仪表+德力西");
                    break;
                case 11:
                    ui->lineEdit_ReadCommCombined->setText("研+仪表+天正");
                    break;
                default:
                    ui->lineEdit_ReadCommCombined->setText("未知");
                    break;
            }

            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("读取点表组合成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("读取点表组合失败.");
        }
    }
    else if(isMbSetCommCombined)
    {
        isMbSetCommCombined = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("组合点表设置成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("组合点表设置失败.");

            QMessageBox::critical(this,"Message", "RS485波特率和校验位设置失败");
        }
    }
    else if(isMbRemoteCtrlLcdOnCmd)
    {
        isMbRemoteCtrlLcdOnCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("远程控制Lcd亮屏成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("远程控制Lcd亮屏失败.");
        }
    }
    else if(isMbSetInValue)
    {
        isMbSetInValue = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('1' == buffer[2] && '0' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("电流规格设置成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("电流规格设置失败.");

            QMessageBox::critical(this,"Message", "电流规格设置失败");
        }
    }
    else if(isMbPaOffsetCaliCmd)
    {
        isMbPaOffsetCaliCmd = false;

        char buffer[128] = {0};

        QByteArray byteArray = RecData_tmp.toUtf8();
        strncpy(buffer, byteArray.constData(), sizeof(buffer));

        if(('0' == buffer[2] && '6' == buffer[3])) //返回成功
        {
            if(ui->checkBox_PrintOkRet->isChecked())
                ui->textEdit_MbInfo->append("有功功率偏置校准成功.");
        }
        else //返回失败
        {
            if(ui->checkBox_PrintErrRet->isChecked())
                ui->textEdit_MbInfo->append("有功功率偏置校准失败.");
        }
    }
  }
  else //处理升级数据
  {
        QByteArray RecArray = RecData_tmp.toUtf8();

//        qDebug()<<"转换为utf8数组个数: "<<RecArray.size();
//        qDebug()<<"转换为utf8数据: "<<RecArray.constData();

        quint16 RecBuf_len = RecArray.size();
        qDebug()<<"[OTA]RecBuf_len: "<<RecBuf_len;

        quint16 crc16_calc = 0;
//        quint16 len; //每帧长度
        qint16 write_num_ret = 0;

        char buf_get[6]={0};
        quint8 ConvertData_Temp[3] = {0};

        strncpy(buf_get, RecArray.constData(), RecBuf_len);
        ConvertData_Temp[0] = convertCharToHex(buf_get[0])*16+convertCharToHex(buf_get[1]);
        ConvertData_Temp[1] = convertCharToHex(buf_get[2])*16+convertCharToHex(buf_get[3]);
        ConvertData_Temp[2] = convertCharToHex(buf_get[4])*16+convertCharToHex(buf_get[5]);

//        QString Str;//打印测试
//        for(int i=0;i<RecBuf_len/2;i++)
//        {
//            qDebug() << Str.asprintf("0x%02x",ConvertData_Temp[i]);//16进制打印
//        }
//        rec_timer->stop();
        if((0 == strncmp((const char*)ConvertData_Temp,(const char*)Rx_OK,3)) || (0 == strncmp((const char*)ConvertData_Temp,(const char*)GetUpgradeCmdOK,3))) //单片机返回数据OK
        {
            qDebug("package_ID = %d, package_num = %d",package_ID,package_num);

            if(package_num - package_ID > 1)
            {
                OTA_Len = PACKAGE_SIZE+7;
                qDebug("OTA len = %d",OTA_Len);
            }
            else if(package_num - package_ID == 1)
            {
                if(0 == (binSize % PACKAGE_SIZE))
                {
                     OTA_Len = PACKAGE_SIZE+7;
                }
                else
                {
                    OTA_Len = (binSize % PACKAGE_SIZE) + 7; //最后一帧数据长度
                    qDebug("OTA last len = %d",OTA_Len);
                }
            }

            if(package_num - package_ID >= 1)
            {
                OTA_data[0] = 0x5A; //帧头
                OTA_data[1] = 0x01; //烧写进行命令
                OTA_data[2] = (quint8)((OTA_Len-7)>>8);
                OTA_data[3] = (quint8)(OTA_Len-7);
                memcpy(&OTA_data[4],&MybinByte[package_ID*PACKAGE_SIZE],OTA_Len-7);
//                qDebug("OTA copy done");
                crc16_calc = GetCRC16_Software(OTA_data,OTA_Len-3,0);
                qDebug("crc16_calc = %d",crc16_calc);
                OTA_data[OTA_Len-3] = (quint8)(crc16_calc>>8);
                OTA_data[OTA_Len-2] = (quint8)crc16_calc;
                OTA_data[OTA_Len-1] = 0xA5; //帧尾

//                qDebug("OTA send data: %02X, %02X, %02X, %02X, OTA_Len = %d",OTA_data[0],OTA_data[1],OTA_data[2],OTA_data[3],OTA_Len);
                write_num_ret = m_serialPort->write((char*)&OTA_data,OTA_Len);
                qDebug("write_num_ret = %d",write_num_ret);

//                m_serialPort->write((char*)OTA_data,OTA_Len);
                double dProgress = (package_ID+1) * 100.0  / package_num;
                ui->progressBar->setValue(dProgress); //更新进度条
//                qDebug("OTA send done");
                package_ID++;
            }
            else //bin文件发送完后再发送给单片机固件信息
            {
                //读取bin文件固定偏移地址的数据转化成版本号。

                OTA_data[0] = 0x5A; //帧头
                OTA_data[1] = 0x02; //烧写完成命令
                OTA_data[2] = 0;
                OTA_data[3] = 8;
                //魔数
                OTA_data[4] = 0xCA;
                OTA_data[5] = 0xFE;
                OTA_data[6] = 0xBA;
                OTA_data[7] = 0xBE;
                //bin文件大小 byte数
                OTA_data[8] = (quint8)(binSize>>24);
                OTA_data[9] = (quint8)(binSize>>16);
                OTA_data[10] = (quint8)(binSize>>8);
                OTA_data[11] = (quint8)(binSize);

                crc16_calc = GetCRC16_Software(OTA_data,12,0);
                OTA_data[12] = (quint8)(crc16_calc>>8);
                OTA_data[13] = (quint8)crc16_calc;
                OTA_data[14] = 0xA5; //帧尾

                OTA_Len = 15;
                m_serialPort->write((char*)&OTA_data,OTA_Len);
                qDebug("send OTA info");
                package_ID = 0;
                Resend_cnt = 0;
                ui->pushButtonOpenFile->setEnabled(true);
                ui->pushButtonUpgrate->setEnabled(true);
                ui->pushButtonStopUpgrade->setEnabled(false);
                setAllRecFlagsFalse();
                isUpdating = false;
            }
        }
        else if(0 == strncmp((const char*)ConvertData_Temp,(const char*)Rx_Done_OK,3)) //单片机返回接收完成，发送固件大小和版本信息给单片机
        {
            qDebug()<<"[OTA]Rx_Done_OK";
            package_ID = 0;
            Resend_cnt = 0;
            ui->pushButtonOpenFile->setEnabled(true);
            ui->pushButtonUpgrate->setEnabled(true);
            ui->pushButtonStopUpgrade->setEnabled(false);
            setAllRecFlagsFalse();
            isUpdating = false;
        }
        else if((0 == strncmp((const char*)ConvertData_Temp,(const char*)Rx_Err,3)) || (0 == strncmp((const char*)ConvertData_Temp,(const char*)GetUpgradeCmdErr,3))) //单片机返回数据异常
        {
            qDebug()<<"[OTA]Rx_Err or GetUpgradeCmdErr";
            //接收错误重发至多三次
            if(Resend_cnt<3)
            {
                m_serialPort->write((char*)&OTA_data,OTA_Len);
                Resend_cnt++;
            }
            else //重发三次仍失败
            {
                qDebug()<<"[OTA]Resend still failed.";
            }
        }

  }
    //==================================================================================================================================
    if(ui->checkBox_TimeTag->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");

        for(int i=0; i<RecData_tmp.size(); i+=2+1)
            RecData_tmp.insert(i, QLatin1String(" "));
        RecData_tmp.remove(0, 1);

        QString string_combin = time_tmp+RecData_tmp;

        qDebug()<<"处理后的串口数据1: "<<string_combin;
        ui->recData->append(string_combin);
    }
    else
    {
        for(int i=0; i<RecData_tmp.size(); i+=2+1)
            RecData_tmp.insert(i, QLatin1String(" "));
        RecData_tmp.remove(0, 1);

        qDebug()<<"处理后的串口数据2: "<<RecData_tmp;
        ui->recData->append(RecData_tmp);
    }

    RecData_tmp.clear();//清空数据
    rec_timer->stop();
}

//void MainWindow::on_pushButtonSync_clicked()
//{
//    send_sync_cmd();
//}

//void MainWindow::on_pushButtonErase_clicked()
//{
//    send_erase_cmd();
//}

//void MainWindow::on_pushButtonStopUpgrade_clicked()
//{
//    step = 0;
//    isUpdating = false;
//    package_ID = 0;
//    isBooting = false;

//    ui->pushButtonUpgrate->setEnabled(true);
//    ui->pushButtonEnterBoot->setEnabled(true);
//    ui->progressBar->setValue(0);
//    timer->stop();

//}

//void MainWindow::send_boot_cmd()
//{
//    //03 10 00 00 00 01 02 00 0A 3F 37
//    quint8 cmd[11] = {0x03, 0x10, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x0A, 0x3F, 0x37 };
//    isBooting = true;
//    m_serialPort->write((char*)&cmd,11);
//}

//void MainWindow::on_pushButtonEnterBoot_clicked()
//{
//    send_boot_cmd();
//}

void MainWindow::SaveConfig() {
    QSettings *configIniWrite = new QSettings("MyConfig.ini", QSettings::IniFormat, this);
    // ----------保存配置---------- //
    configIniWrite->setValue("/Last_Message/lineEdit",ui->lineEditSend->text());
    configIniWrite->setValue("/Last_Message/lineEdit2",ui->lineEditSend2->text());
    configIniWrite->setValue("/Last_Message/lineEdit3",ui->lineEditSend3->text());
    configIniWrite->setValue("/Last_Message/lineEdit4",ui->lineEditSend4->text());
    configIniWrite->setValue("/Last_Message/lineEdit5",ui->lineEditSend5->text());
    configIniWrite->setValue("/Last_Message/lineEdit6",ui->lineEditSend6->text());

    configIniWrite->setValue("/Last_Message/lineEditText1",ui->lineEditSend_Text1->text());
    configIniWrite->setValue("/Last_Message/lineEditText2",ui->lineEditSend_Text2->text());
    configIniWrite->setValue("/Last_Message/lineEditText3",ui->lineEditSend_Text3->text());
    configIniWrite->setValue("/Last_Message/lineEditText4",ui->lineEditSend_Text4->text());
    configIniWrite->setValue("/Last_Message/lineEditText5",ui->lineEditSend_Text5->text());
    configIniWrite->setValue("/Last_Message/lineEditText6",ui->lineEditSend_Text6->text());

    configIniWrite->setValue("/Last_Message/lineEdit_WriteMAC",ui->lineEdit_WriteMAC->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteAssetCode",ui->lineEdit_WriteAssetCode->text());
    configIniWrite->setValue("/Last_Message/lineEdit_OV_Thres_2",ui->lineEdit_OV_Thres_2->text());
    configIniWrite->setValue("/Last_Message/lineEdit_OV_delay_3",ui->lineEdit_OV_delay_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_OV_recover_3",ui->lineEdit_OV_recover_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_OV_recover_delay_3",ui->lineEdit_OV_recover_delay_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_UV_Thres_3",ui->lineEdit_UV_Thres_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_UV_delay_3",ui->lineEdit_UV_delay_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_UV_recover_3",ui->lineEdit_UV_recover_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_UV_recover_delay_3",ui->lineEdit_UV_recover_delay_3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_IslandVoltSwing_2",ui->lineEdit_IslandVoltSwing_2->text());
    configIniWrite->setValue("/Last_Message/lineEdit_IslandFreqSwing_2",ui->lineEdit_IslandFreqSwing_2->text());
    configIniWrite->setValue("/Last_Message/lineEdit_Island_delay_2",ui->lineEdit_Island_delay_2->text());

    //645相关
    configIniWrite->setValue("/Last_Message/lineEdit_PassCode",ui->lineEdit_PassCode->text());
    configIniWrite->setValue("/Last_Message/lineEdit_OperatorCode",ui->lineEdit_OperatorCode->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteAddr645",ui->lineEdit_WriteAddr645->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DI_code",ui->lineEdit_DI_code->text());
    configIniWrite->setValue("/Last_Message/lineEdit_Data_Frame",ui->lineEdit_Data_Frame->text());

    configIniWrite->setValue("/Last_Message/lineEdit_WriteOpenTimeData",ui->lineEdit_WriteOpenTimeData->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteCloseTimeData",ui->lineEdit_WriteCloseTimeData->text());

    configIniWrite->setValue("/Last_Message/lineEdit_WriteOVThresData",ui->lineEdit_WriteOVThresData->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteOVDelayTime",ui->lineEdit_WriteOVDelayTime->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteUVThresData",ui->lineEdit_WriteUVThresData->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteUVDelayTime",ui->lineEdit_WriteUVDelayTime->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteOVRecoverThresData",ui->lineEdit_WriteOVRecoverThresData->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteOVRecoverDelayTime",ui->lineEdit_WriteOVRecoverDelayTime->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteUVRecoverThresData",ui->lineEdit_WriteUVRecoverThresData->text());
    configIniWrite->setValue("/Last_Message/lineEdit_WriteUVRecoverDelayTime",ui->lineEdit_WriteUVRecoverDelayTime->text());

    //研发测试相关
    configIniWrite->setValue("/Last_Message/lineEdit_AutoCloseDelay",ui->lineEdit_AutoCloseDelay->text());
    configIniWrite->setValue("/Last_Message/lineEdit_AutoOpenDelay",ui->lineEdit_AutoOpenDelay->text());

    //NXBLU125D-modbus相关
    configIniWrite->setValue("/Last_Message/lineEdit_MbWriteSN_PCODE",ui->lineEdit_MbWriteSN_PCODE->text());
    configIniWrite->setValue("/Last_Message/lineEdit_MbCali_Ia",ui->lineEdit_MbCali_Ia->text());
    configIniWrite->setValue("/Last_Message/lineEdit_MbCali_Ua",ui->lineEdit_MbCali_Ua->text());
    configIniWrite->setValue("/Last_Message/lineEdit_MbCali_Pa",ui->lineEdit_MbCali_Pa->text());
    configIniWrite->setValue("/Last_Message/lineEdit_UpdateIntervalTime",ui->lineEdit_UpdateIntervalTime->text());

    //自定义循环发送区
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend1",ui->lineEdit_DelaySend1->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend2",ui->lineEdit_DelaySend2->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend3",ui->lineEdit_DelaySend3->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend4",ui->lineEdit_DelaySend4->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend5",ui->lineEdit_DelaySend5->text());
    configIniWrite->setValue("/Last_Message/lineEdit_DelaySend6",ui->lineEdit_DelaySend6->text());

    //configIniWrite->setValue("/Last_Message/lineEditBinPath",ui->lineEditBinFliePath->text());
}

void MainWindow::LoadConfig() {
    // ----------读取配置并进行设置---------- //
    if ( QFile::exists("MyConfig.ini") ) {
        QSettings *configIniRead = new QSettings("MyConfig.ini", QSettings::IniFormat, this);
        ui->lineEditSend->setText(configIniRead->value("/Last_Message/lineEdit").toString());
        ui->lineEditSend2->setText(configIniRead->value("/Last_Message/lineEdit2").toString());
        ui->lineEditSend3->setText(configIniRead->value("/Last_Message/lineEdit3").toString());
        //new
        ui->lineEditSend4->setText(configIniRead->value("/Last_Message/lineEdit4").toString());
        ui->lineEditSend5->setText(configIniRead->value("/Last_Message/lineEdit5").toString());
        ui->lineEditSend6->setText(configIniRead->value("/Last_Message/lineEdit6").toString());

        ui->lineEditSend_Text1->setText(configIniRead->value("/Last_Message/lineEditText1").toString());
        ui->lineEditSend_Text2->setText(configIniRead->value("/Last_Message/lineEditText2").toString());
        ui->lineEditSend_Text3->setText(configIniRead->value("/Last_Message/lineEditText3").toString());
        ui->lineEditSend_Text4->setText(configIniRead->value("/Last_Message/lineEditText4").toString());
        ui->lineEditSend_Text5->setText(configIniRead->value("/Last_Message/lineEditText5").toString());
        ui->lineEditSend_Text6->setText(configIniRead->value("/Last_Message/lineEditText6").toString());

        ui->lineEdit_WriteMAC->setText(configIniRead->value("/Last_Message/lineEdit_WriteMAC").toString());
        ui->lineEdit_WriteAssetCode->setText(configIniRead->value("/Last_Message/lineEdit_WriteAssetCode").toString());
        ui->lineEdit_OV_Thres_2->setText(configIniRead->value("/Last_Message/lineEdit_OV_Thres_2").toString());
        ui->lineEdit_OV_delay_3->setText(configIniRead->value("/Last_Message/lineEdit_OV_delay_3").toString());
        ui->lineEdit_OV_recover_3->setText(configIniRead->value("/Last_Message/lineEdit_OV_recover_3").toString());
        ui->lineEdit_OV_recover_delay_3->setText(configIniRead->value("/Last_Message/lineEdit_OV_recover_delay_3").toString());
        ui->lineEdit_UV_Thres_3->setText(configIniRead->value("/Last_Message/lineEdit_UV_Thres_3").toString());
        ui->lineEdit_UV_delay_3->setText(configIniRead->value("/Last_Message/lineEdit_UV_delay_3").toString());
        ui->lineEdit_UV_recover_3->setText(configIniRead->value("/Last_Message/lineEdit_UV_recover_3").toString());
        ui->lineEdit_UV_recover_delay_3->setText(configIniRead->value("/Last_Message/lineEdit_UV_recover_delay_3").toString());
        ui->lineEdit_IslandVoltSwing_2->setText(configIniRead->value("/Last_Message/lineEdit_IslandVoltSwing_2").toString());
        ui->lineEdit_IslandFreqSwing_2->setText(configIniRead->value("/Last_Message/lineEdit_IslandFreqSwing_2").toString());
        ui->lineEdit_Island_delay_2->setText(configIniRead->value("/Last_Message/lineEdit_Island_delay_2").toString());

        //645相关
        ui->lineEdit_PassCode->setText(configIniRead->value("/Last_Message/lineEdit_PassCode").toString());
        ui->lineEdit_OperatorCode->setText(configIniRead->value("/Last_Message/lineEdit_OperatorCode").toString());
        ui->lineEdit_WriteAddr645->setText(configIniRead->value("/Last_Message/lineEdit_WriteAddr645").toString());
        ui->lineEdit_DI_code->setText(configIniRead->value("/Last_Message/lineEdit_DI_code").toString());
        ui->lineEdit_Data_Frame->setText(configIniRead->value("/Last_Message/lineEdit_Data_Frame").toString());

        ui->lineEdit_WriteOpenTimeData->setText(configIniRead->value("/Last_Message/lineEdit_WriteOpenTimeData").toString());
        ui->lineEdit_WriteCloseTimeData->setText(configIniRead->value("/Last_Message/lineEdit_WriteCloseTimeData").toString());
        ui->lineEdit_WriteOVThresData->setText(configIniRead->value("/Last_Message/lineEdit_WriteOVThresData").toString());
        ui->lineEdit_WriteOVDelayTime->setText(configIniRead->value("/Last_Message/lineEdit_WriteOVDelayTime").toString());
        ui->lineEdit_WriteUVThresData->setText(configIniRead->value("/Last_Message/lineEdit_WriteUVThresData").toString());
        ui->lineEdit_WriteUVDelayTime->setText(configIniRead->value("/Last_Message/lineEdit_WriteUVDelayTime").toString());
        ui->lineEdit_WriteOVRecoverThresData->setText(configIniRead->value("/Last_Message/lineEdit_WriteOVRecoverThresData").toString());
        ui->lineEdit_WriteOVRecoverDelayTime->setText(configIniRead->value("/Last_Message/lineEdit_WriteOVRecoverDelayTime").toString());
        ui->lineEdit_WriteUVRecoverThresData->setText(configIniRead->value("/Last_Message/lineEdit_WriteUVRecoverThresData").toString());
        ui->lineEdit_WriteUVRecoverDelayTime->setText(configIniRead->value("/Last_Message/lineEdit_WriteUVRecoverDelayTime").toString());

        //研发测试相关
        ui->lineEdit_AutoCloseDelay->setText(configIniRead->value("/Last_Message/lineEdit_AutoCloseDelay").toString());
        ui->lineEdit_AutoOpenDelay->setText(configIniRead->value("/Last_Message/lineEdit_AutoOpenDelay").toString());

        //NXBLU125D-modbus相关
        ui->lineEdit_MbWriteSN_PCODE->setText(configIniRead->value("/Last_Message/lineEdit_MbWriteSN_PCODE").toString());
        ui->lineEdit_MbCali_Ia->setText(configIniRead->value("/Last_Message/lineEdit_MbCali_Ia").toString());
        ui->lineEdit_MbCali_Ua->setText(configIniRead->value("/Last_Message/lineEdit_MbCali_Ua").toString());
        ui->lineEdit_MbCali_Pa->setText(configIniRead->value("/Last_Message/lineEdit_MbCali_Pa").toString());
        ui->lineEdit_UpdateIntervalTime->setText(configIniRead->value("/Last_Message/lineEdit_UpdateIntervalTime").toString());

        //自定义循环发送区
        ui->lineEdit_DelaySend1->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend1").toString());
        ui->lineEdit_DelaySend2->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend2").toString());
        ui->lineEdit_DelaySend3->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend3").toString());
        ui->lineEdit_DelaySend4->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend4").toString());
        ui->lineEdit_DelaySend5->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend5").toString());
        ui->lineEdit_DelaySend6->setText(configIniRead->value("/Last_Message/lineEdit_DelaySend6").toString());

        //ui->lineEditBinFliePath->setText(configIniRead->value("/Last_Message/lineEditBinPath").toString());
    }
}

void MainWindow::on_pushButtonSend2_clicked()
{
    QString m_strSendData = ui->lineEditSend2->text();
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
//    ui->textEditSend->append(m_strSendData);
    if(ui->checkBoxHexSend_2->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButtonSend3_clicked()
{
    QString m_strSendData = ui->lineEditSend3->text();
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
    if(ui->checkBoxHexSend_3->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButtonClearSend_2_clicked()
{
    ui->lineEditSend2->setText(NULL);
}

void MainWindow::on_pushButtonClearSend_3_clicked()
{
    ui->lineEditSend3->setText(NULL);
}

void MainWindow::on_pushButtonClearSendArea_clicked()
{
    ui->textEditSend->setText(NULL);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    qDebug("serial err happed");
    if (error == QSerialPort::ResourceError)
    {
        m_serialPort->close();
        ui->pushButtonOpenSerial->setText("打开串口");
    }
}

void MainWindow::on_pushButtonFreshPort_clicked()
{
    ui->comboBox_serialNum->clear();
    m_portNameList = getPortNameList();

    ui->comboBox_serialNum->addItems(m_portNameList);
}

void MainWindow::on_pushButtonSend4_clicked()
{
    QString m_strSendData = ui->lineEditSend4->text();
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
    if(ui->checkBoxHexSend_4->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButtonSend5_clicked()
{
    QString m_strSendData = ui->lineEditSend5->text();
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
    if(ui->checkBoxHexSend_5->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButtonSend6_clicked()
{
    QString m_strSendData = ui->lineEditSend6->text();
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }
    if(ui->checkBoxHexSend_6->isChecked())
    {
        if (m_strSendData.contains(" "))
        {
            m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
        }

        QByteArray sendBuf;

        convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

        m_serialPort->write(sendBuf);
    }
    else
    {
        m_serialPort->write(m_strSendData.toLocal8Bit());
    }
}

void MainWindow::on_pushButton_ReadVoltage_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x20,0x00,0x02,0x00,0x00,0xE5,0xBE,0x16 };//通配读电压
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);

    setAllRecFlagsFalse();
    isReadVoltSend = true;
}

void MainWindow::on_pushButton_ReadFrequency_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x20,0x0F,0x02,0x00,0x00,0x1C,0x0C,0x16 };//通配读频率
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);

    setAllRecFlagsFalse();
    isReadFreqSend = true;
}

void MainWindow::on_pushButton_ReadMAC_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x10,0xDA,0x5F,0x05,0x01,0x10,0x40,0x01,0x02,0x00,0x00,0x5D,0x41,0x16 };//通配读地址
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);

    setAllRecFlagsFalse();
    isReadMACSend = true;
}

void MainWindow::on_pushButton_ReadAsset_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x41,0x03,0x02,0x00,0x00,0xDF,0x31,0x16 };//通配读资产码
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);

    setAllRecFlagsFalse();
    isReadAssetSend = true;
}

void MainWindow::on_pushButton_ReadProtectBit_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0xFF,0xA1,0x03,0x00,0x00,0x74,0xCF,0x16 };//通配读保护位
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);

    setAllRecFlagsFalse();
    isReadProtectBitSend = true;
}

bool isAllDigits(const QString &str) {
    QRegularExpression regExp("^\\d+$"); // 正则表达式，表示全部由数字组成
    return regExp.match(str).hasMatch();
}

void MainWindow::on_pushButton_WriteMAC_clicked()
{
    char buffer[12] = {0};
    quint8 MAC_element[12] = {0};
    quint8 cmd[33] = {0x68, 0x1F, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0xF1,0xF3,0x06,0x01,0x00,0x40,0x01,0x02,0x00,0x09,0x06,0x00,0x00, 0x00, 0x00, 0x00, 0x02, 0x00,0x34,0x0F,0x16 };//通配写地址
    QString new_MAC = ui->lineEdit_WriteMAC->text();
    quint8 MAC_length = new_MAC.length();
    qDebug()<<"MAC_length: "<<MAC_length;
    if(12 != MAC_length)
    {
        QMessageBox::critical(this,"SetMAC Message", "地址长度错误");
        return;
    }
    else if(!isAllDigits(new_MAC))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"SetMAC Message", "地址含有非法字符");
        return;
    }else{}

    QByteArray byteArray_tmp = new_MAC.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));

    for(quint8 i=0;i<12;i++)
    {
        MAC_element[i] = convertCharToHex(buffer[i]);
    }

    cmd[23] = MAC_element[0]*16+MAC_element[1];
    cmd[24] = MAC_element[2]*16+MAC_element[3];
    cmd[25] = MAC_element[4]*16+MAC_element[5];
    cmd[26] = MAC_element[6]*16+MAC_element[7];
    cmd[27] = MAC_element[8]*16+MAC_element[9];
    cmd[28] = MAC_element[10]*16+MAC_element[11];

    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 29);
    cmd[30] = (quint8)(CRC_tail);
    cmd[31] = (quint8)(CRC_tail>>8);

    displayHexArrayInWidget(cmd,33);
    m_serialPort->write((char*)&cmd,33);
    setAllRecFlagsFalse();
    isWriteMACSend = true;
}

void MainWindow::on_pushButtonClearSend_4_clicked()
{
    ui->lineEditSend4->setText(NULL);
}

void MainWindow::on_pushButtonClearSend_5_clicked()
{
    ui->lineEditSend5->setText(NULL);
}

void MainWindow::on_pushButtonClearSend_6_clicked()
{
    ui->lineEditSend6->setText(NULL);
}

void MainWindow::on_pushButton_WriteAssetCode_clicked()
{
    char buffer[21] = {0};
    quint8 Asset_element[21] = {0};
    quint8 cmd[48] = {0x68, 0x2E, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0xAF,0x35,0x06,0x01,0x00,0x41,0x03,0x02,0x00,0x0A,0x15,0x33,0x31, 0x33, 0x30, 0x30, 0x35, 0x36,0x31,0x30,0x30, 0x32, 0x35, 0x30, 0x34, 0x31, 0x30, 0x30, 0x30, 0x30,0x32, 0x32, 0x00, 0x6D, 0xF7, 0x16 };//通配写地址
    QString new_Asset = ui->lineEdit_WriteAssetCode->text();
    quint8 Asset_length = new_Asset.length();
    qDebug()<<"Asset_length: "<<Asset_length;
    if(21 != Asset_length)
    {
        QMessageBox::critical(this,"SetAsset Message", "资产码长度错误");
        return;
    }
    else if(!isAllDigits(new_Asset))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"SetAsset Message", "资产码含有非法字符");
        return;
    }else{}

    QByteArray byteArray_tmp = new_Asset.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));

    for(quint8 i=0;i<21;i++)
    {
        Asset_element[i] = convertCharToHex(buffer[i]);
    }

    for(quint8 j=0;j<21;j++)
    {
        cmd[23+j] = Asset_element[j]+0x30;
    }

    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 44);
    cmd[45] = (quint8)(CRC_tail);
    cmd[46] = (quint8)(CRC_tail>>8);

    displayHexArrayInWidget(cmd,48);
    m_serialPort->write((char*)&cmd,48);
    setAllRecFlagsFalse();
    isWriteAssetSend = true;
}

void MainWindow::on_pushButton_WriteProtectBit_clicked()
{
    quint8 cmd[29] = {0x68, 0x1B, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0xA4,0xAD,0x06,0x01,0x00,0xFF,0xA1,0x03,0x00,0x04,0x10,0xF1,0x80, 0x00, 0x51, 0xA7, 0x16 };//通配写保护位

    quint8 bit_tmp1=0;
    quint8 bit_tmp2=0;

    if(ui->checkBox_LV_2->isChecked())
        bit_tmp1 |= 0x80;
    else
        bit_tmp1 &= 0x7F;

    if(ui->checkBox_LP_2->isChecked())
        bit_tmp1 |= 0x40;
    else
        bit_tmp1 &= 0xBF;

    if(ui->checkBox_UV_2->isChecked())
        bit_tmp1 |= 0x20;
    else
        bit_tmp1 &= 0xDF;

    if(ui->checkBox_OV_2->isChecked())
        bit_tmp1 |= 0x10;
    else
        bit_tmp1 &= 0xEF;

    if(ui->checkBox_IslandVolt_2->isChecked())
        bit_tmp1 |= 0x08;
    else
        bit_tmp1 &= 0xF7;

    if(ui->checkBox_IslandFreq_2->isChecked())
        bit_tmp1 |= 0x04;
    else
        bit_tmp1 &= 0xFB;

    if(ui->checkBox_IslandAndOr_2->isChecked())
        bit_tmp1 |= 0x02;
    else
        bit_tmp1 &= 0xFD;

    if(ui->checkBox_SelfLock_2->isChecked())
        bit_tmp1 |= 0x01;
    else
        bit_tmp1 &= 0xFE;
    //-----------------------------------------
    if(ui->checkBox_FreqProtect_2->isChecked())
        bit_tmp2 |= 0x80;
    else
        bit_tmp2 &= 0x7F;
    //-----------------------------------------
    qDebug()<<"bit_tmp1: "<<bit_tmp1<<"bit_tmp2: "<<bit_tmp2;
    //保护位更新
    cmd[23] = bit_tmp1;
    cmd[24] = bit_tmp2;

    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 25);
    qDebug()<<"Tail_CRC: "<<CRC_tail;

    cmd[26] = (quint8)(CRC_tail);
    cmd[27] = (quint8)(CRC_tail>>8);
    qDebug()<<"cmd[26]: "<<cmd[26]<<"cmd[27]: "<<cmd[27];

    displayHexArrayInWidget(cmd,29);
    m_serialPort->write((char*)&cmd,29);
    setAllRecFlagsFalse();
    isWriteProtectBitSend = true;

}

void MainWindow::on_pushButton_ReadOV_Para_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x4E,0x20,0x02,0x00,0x00,0xBD,0xF1, 0x16 };//通配读过压参数
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isReadOVParaSend = true;
}

void MainWindow::on_pushButton_ReadUV_Para_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x4E,0x21,0x02,0x00,0x00,0x06,0xED, 0x16 };//通配读欠压参数
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isReadUVParaSend = true;
}

void MainWindow::on_pushButton_ReadIsland_Para_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x49,0xE0,0x02,0x00,0x00,0xB8,0xFA, 0x16 };//通配读孤岛参数
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isReadIslandParaSend = true;
}

void MainWindow::on_pushButton_DeviceInfo_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x43,0x00,0x02,0x00,0x00,0x9A,0x02, 0x16 };//通配读电气设备
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isReadDeviceInfoSend = true;
}

void MainWindow::on_pushButton_WriteOV_Para_clicked()
{
    char buffer[4] = {0};
    quint8 str_element[4] = {0};
    quint16 thres_tmp = 0;
    quint8 cmd[39] = {0x68, 0x25, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0xB6,0x77,0x06,0x01,0x00,0x4E,0x20,0x02,0x00,0x02,0x04,0x12,0x0B, 0x22, 0x12, 0x00, 0x02, 0x12, 0x0A,0xBE, 0x12,0x00,0x05,0x00,0xA5,0xE5,0x16 };//通配写过压参数
    //过压阈值参数更新----------------------------------------------------------------
    QString new_str = ui->lineEdit_OV_Thres_2->text(); //输入的过压阈值参数
    quint8 str_length = new_str.length();
    qDebug()<<"str_length: "<<str_length;
    if(4 != str_length)
    {
        QMessageBox::critical(this,"Set Message", "过压阈值错误");
        return;
    }
    else if(!isAllDigits(new_str))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "过压阈值含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp = new_str.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));
    for(quint8 i=0;i<4;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    qDebug()<<"OV_Thres: "<<thres_tmp;
    cmd[24] = (quint8)(thres_tmp>>8);
    cmd[25] = (quint8)thres_tmp;
    qDebug()<<"cmd[24]: "<<cmd[24]<<"cmd[25]: "<<cmd[25];
    //过压延时参数更新------------------------------------------------------------------
    QString new_str1 = ui->lineEdit_OV_delay_3->text(); //输入的过压延时参数
    quint8 str_length1 = new_str1.length();
    qDebug()<<"str_length1: "<<str_length1;
    if(str_length1>2)
    {
        QMessageBox::critical(this,"Set Message", "过压延时错误");
        return;
    }
    else if(!isAllDigits(new_str1))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "过压延时含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp1 = new_str1.toUtf8();
    strncpy(buffer, byteArray_tmp1.constData(), sizeof(buffer));
    if(1==str_length1)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        cmd[28] = str_element[0];
    }
    else if(2==str_length1)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        str_element[1] = convertCharToHex(buffer[1]);
        cmd[28] = str_element[0]*16+str_element[1];
    }
    //过压恢复阈值参数更新----------------------------------------------------------------
    QString new_str2 = ui->lineEdit_OV_recover_3->text(); //输入的过压恢复阈值参数
    quint8 str_length2 = new_str2.length();
    qDebug()<<"str_length2: "<<str_length2;
    if(4 != str_length2)
    {
        QMessageBox::critical(this,"Set Message", "过压恢复阈值错误");
        return;
    }
    else if(!isAllDigits(new_str2))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "过压恢复阈值含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp2 = new_str2.toUtf8();
    strncpy(buffer, byteArray_tmp2.constData(), sizeof(buffer));
    for(quint8 i=0;i<4;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    qDebug()<<"UV_Thres: "<<thres_tmp;
    cmd[30] = (quint8)(thres_tmp>>8);
    cmd[31] = (quint8)thres_tmp;
    qDebug()<<"cmd[30]: "<<cmd[30]<<"cmd[30]: "<<cmd[31];
    //过压恢复延时参数更新------------------------------------------------------------------
    QString new_str3 = ui->lineEdit_OV_recover_delay_3->text(); //输入的过压恢复延时参数
    quint8 str_length3 = new_str3.length();
    qDebug()<<"str_length3: "<<str_length3;
    if(str_length3>2)
    {
        QMessageBox::critical(this,"Set Message", "过压延时参数错误");
        return;
    }
    else if(!isAllDigits(new_str3))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "过压延时参数含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp3 = new_str3.toUtf8();
    strncpy(buffer, byteArray_tmp3.constData(), sizeof(buffer));

    if(1==str_length3)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        cmd[34] = str_element[0];
    }
    else if(2==str_length3)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        str_element[1] = convertCharToHex(buffer[1]);
        cmd[34] = str_element[0]*16+str_element[1];
    }
    //尾校验更新--------------------------------------------------------------------------
    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 35);
    cmd[36] = (quint8)(CRC_tail);
    cmd[37] = (quint8)(CRC_tail>>8);
    //更新后发送--------------------------------------------------------------------------
    displayHexArrayInWidget(cmd,39);
    m_serialPort->write((char*)&cmd,39);
    setAllRecFlagsFalse();
    isWriteOVParaSend = true;
}

void MainWindow::on_pushButton_WriteUV_Para_clicked()
{
    char buffer[4] = {0};
    quint8 str_element[4] = {0};
    quint16 thres_tmp = 0;
    quint8 cmd[39] = {0x68, 0x25, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0xB6,0x77,0x06,0x01,0x00,0x4E,0x21,0x02,0x00,0x02,0x04,0x12,0x06, 0x1E, 0x12, 0x00, 0x02, 0x12, 0x07,0x78, 0x12,0x00,0x06,0x00,0xE5,0x89,0x16 };//通配写欠压参数
    //欠压阈值参数更新----------------------------------------------------------------
    QString new_str = ui->lineEdit_UV_Thres_3->text(); //输入的欠压阈值参数
    quint8 str_length = new_str.length();
    qDebug()<<"str_length: "<<str_length;
    if(4 != str_length)
    {
        QMessageBox::critical(this,"Set Message", "欠压阈值错误");
        return;
    }
    else if(!isAllDigits(new_str))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "欠压阈值含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp = new_str.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));
    for(quint8 i=0;i<4;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    qDebug()<<"UV_Thres: "<<thres_tmp;
    cmd[24] = (quint8)(thres_tmp>>8);
    cmd[25] = (quint8)thres_tmp;
    qDebug()<<"cmd[24]: "<<cmd[24]<<"cmd[25]: "<<cmd[25];
    //欠压延时参数更新------------------------------------------------------------------
    QString new_str1 = ui->lineEdit_UV_delay_3->text(); //输入的欠压延时参数
    quint8 str_length1 = new_str1.length();
    qDebug()<<"str_length1: "<<str_length1;
    if(str_length1>2)
    {
        QMessageBox::critical(this,"Set Message", "欠压延时错误");
        return;
    }
    else if(!isAllDigits(new_str1))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "欠压延时含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp1 = new_str1.toUtf8();
    strncpy(buffer, byteArray_tmp1.constData(), sizeof(buffer));
    if(1==str_length1)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        cmd[28] = str_element[0];
    }
    else if(2==str_length1)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        str_element[1] = convertCharToHex(buffer[1]);
        cmd[28] = str_element[0]*16+str_element[1];
    }
    //欠压恢复阈值参数更新----------------------------------------------------------------
    QString new_str2 = ui->lineEdit_UV_recover_3->text(); //输入的欠压恢复阈值参数
    quint8 str_length2 = new_str2.length();
    qDebug()<<"str_length2: "<<str_length2;
    if(4 != str_length2)
    {
        QMessageBox::critical(this,"Set Message", "欠压恢复阈值错误");
        return;
    }
    else if(!isAllDigits(new_str2))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "欠压恢复阈值含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp2 = new_str2.toUtf8();
    strncpy(buffer, byteArray_tmp2.constData(), sizeof(buffer));
    for(quint8 i=0;i<4;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    qDebug()<<"UV_Thres: "<<thres_tmp;
    cmd[30] = (quint8)(thres_tmp>>8);
    cmd[31] = (quint8)thres_tmp;
    qDebug()<<"cmd[30]: "<<cmd[30]<<"cmd[30]: "<<cmd[31];
    //欠压恢复延时参数更新------------------------------------------------------------------
    QString new_str3 = ui->lineEdit_UV_recover_delay_3->text(); //输入的欠压恢复延时参数
    quint8 str_length3 = new_str3.length();
    qDebug()<<"str_length3: "<<str_length3;
    if(str_length3>2)
    {
        QMessageBox::critical(this,"Set Message", "欠压延时参数错误");
        return;
    }
    else if(!isAllDigits(new_str3))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "欠压延时参数含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp3 = new_str3.toUtf8();
    strncpy(buffer, byteArray_tmp3.constData(), sizeof(buffer));

    if(1==str_length3)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        cmd[34] = str_element[0];
    }
    else if(2==str_length3)
    {
        str_element[0] = convertCharToHex(buffer[0]);
        str_element[1] = convertCharToHex(buffer[1]);
        cmd[34] = str_element[0]*16+str_element[1];
    }
    //尾校验更新--------------------------------------------------------------------------
    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 35);
    cmd[36] = (quint8)(CRC_tail);
    cmd[37] = (quint8)(CRC_tail>>8);
    //更新后发送--------------------------------------------------------------------------
    displayHexArrayInWidget(cmd,39);
    m_serialPort->write((char*)&cmd,39);
    setAllRecFlagsFalse();
    isWriteUVParaSend = true;
}

void MainWindow::on_pushButton_WriteIsland_Para_clicked()
{
    char buffer[4] = {0};
    quint8 str_element[4] = {0};
    quint16 thres_tmp = 0;
    quint8 cmd[36] = {0x68, 0x22, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x50,0xD7,0x06,0x01,0x00,0x49,0xE0,0x02,0x00,0x02,0x03,0x12,0x01, 0x5E, 0x12, 0x00, 0x37, 0x12, 0x00,0x78, 0x00,0xAC,0x24,0x16 };//通配写孤岛参数
    //孤岛幅值摆动参数更新----------------------------------------------------------------
    QString new_str = ui->lineEdit_IslandVoltSwing_2->text();
    quint8 str_length = new_str.length();
    qDebug()<<"str_length: "<<str_length;
    if(str_length<2 || str_length>4)
    {
        QMessageBox::critical(this,"Set Message", "孤岛幅值摆动参数错误");
        return;
    }
    else if(!isAllDigits(new_str))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "孤岛幅值摆动参数含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp = new_str.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));
    for(quint8 i=0;i<str_length;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    if(2==str_length)
    {
        thres_tmp = str_element[0]*10+str_element[1];
    }
    else if(3==str_length)
    {
        thres_tmp = str_element[0]*100+str_element[1]*10+str_element[2];
    }
    else if(4==str_length)
    {
        thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    }

    qDebug()<<"IslandVoltSwing: "<<thres_tmp;
    cmd[24] = (quint8)(thres_tmp>>8);
    cmd[25] = (quint8)thres_tmp;
    qDebug()<<"cmd[24]: "<<cmd[24]<<"cmd[25]: "<<cmd[25];
    //孤岛频率参摆动数更新----------------------------------------------------------------
    QString new_str2 = ui->lineEdit_IslandFreqSwing_2->text();
    quint8 str_length2 = new_str2.length();
    qDebug()<<"str_length2: "<<str_length2;
    if(str_length2<2 || str_length2>4)
    {
        QMessageBox::critical(this,"Set Message", "孤岛频率摆动参数错误");
        return;
    }
    else if(!isAllDigits(new_str2))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "孤岛频率参数摆动含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp2 = new_str2.toUtf8();
    strncpy(buffer, byteArray_tmp2.constData(), sizeof(buffer));
    for(quint8 i=0;i<str_length2;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    if(2==str_length2)
    {
        thres_tmp = str_element[0]*10+str_element[1];
    }
    else if(3==str_length2)
    {
        thres_tmp = str_element[0]*100+str_element[1]*10+str_element[2];
    }
    else if(4==str_length2)
    {
        thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    }
    qDebug()<<"IslandFreqSwing: "<<thres_tmp;
    cmd[27] = (quint8)(thres_tmp>>8);
    cmd[28] = (quint8)thres_tmp;
    qDebug()<<"cmd[27]: "<<cmd[27]<<"cmd[28]: "<<cmd[28];
    //孤岛告警延时参数更新------------------------------------------------------------------
    QString new_str3 = ui->lineEdit_Island_delay_2->text();
    quint8 str_length3 = new_str3.length();
    qDebug()<<"str_length3: "<<str_length3;
    if(str_length3<2 || str_length3>4)
    {
        QMessageBox::critical(this,"Set Message", "孤岛告警延时参数错误");
        return;
    }
    else if(!isAllDigits(new_str3))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Set Message", "孤岛告警延时参数含有非法字符");
        return;
    }else{}
    QByteArray byteArray_tmp3 = new_str3.toUtf8();
    strncpy(buffer, byteArray_tmp3.constData(), sizeof(buffer));

    for(quint8 i=0;i<str_length3;i++)
    {
        str_element[i] = convertCharToHex(buffer[i]);
    }
    if(2==str_length3)
    {
        thres_tmp = str_element[0]*10+str_element[1];
    }
    else if(3==str_length3)
    {
        thres_tmp = str_element[0]*100+str_element[1]*10+str_element[2];
    }
    else if(4==str_length3)
    {
        thres_tmp = str_element[0]*1000+str_element[1]*100+str_element[2]*10+str_element[3];
    }
    qDebug()<<"IslandDelay: "<<thres_tmp;
    cmd[30] = (quint8)(thres_tmp>>8);
    cmd[31] = (quint8)thres_tmp;
    qDebug()<<"cmd[30]: "<<cmd[30]<<"cmd[31]: "<<cmd[31];
    //尾校验更新--------------------------------------------------------------------------
    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 32);
    cmd[33] = (quint8)(CRC_tail);
    cmd[34] = (quint8)(CRC_tail>>8);
    //更新后发送--------------------------------------------------------------------------
    displayHexArrayInWidget(cmd,36);
    m_serialPort->write((char*)&cmd,36);
    setAllRecFlagsFalse();
    isWriteIslandParaSend = true;
}

void MainWindow::on_pushButton_RS485Set_clicked()
{
    quint8 cmd[40] = {0x68, 0x26, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x10,0x84,0x99,0x07,0x01,0x00,0xF2,0x01,0x7F,0x00,0x02,0x03,0x51,0xF2, 0x01, 0x02, 0x01, 0x5F, 0x02, 0x00,0x08, 0x01,0x00,0x16,0x00,0x00,0xF4,0x1D,0x16 };//通配写串口参数

    quint32 buad = ui->comboBox_baudrate_2->currentText().toInt();
    qDebug()<<"buadrate: "<<buad;

    switch(buad)
    {
        case 1200:
            cmd[29]=0x02;
            break;
        case 2400:
            cmd[29]=0x03;
            break;
        case 4800:
            cmd[29]=0x04;
            break;
        case 9600:
            cmd[29]=0x06;
            break;
        case 19200:
            cmd[29]=0x07;
            break;
        case 57600:
            cmd[29]=0x09;
            break;
        case 115200:
            cmd[29]=0x0A;
            break;
        default:
            break;
    }

    if(ui->comboBoxParity_2->currentText() == "None")
    {
        qDebug()<<"None: ";
        cmd[30]=0x00;
    }
    else if(ui->comboBoxParity_2->currentText() == "Odd")
    {
        qDebug()<<"Odd: ";
        cmd[30]=0x01;
    }
    else if(ui->comboBoxParity_2->currentText() == "Even")
    {
        qDebug()<<"Even: ";
        cmd[30]=0x02;
    }
    //尾校验更新--------------------------------------------------------------------------
    quint16 CRC_tail = servAlg_getCRC16(CRC16_X25, &cmd[1], 36);
    cmd[37] = (quint8)(CRC_tail);
    cmd[38] = (quint8)(CRC_tail>>8);
    //更新后发送--------------------------------------------------------------------------
    displayHexArrayInWidget(cmd,40);
    m_serialPort->write((char*)&cmd,40);
    setAllRecFlagsFalse();
    isWriteRS485ParaSend = true;
}

void MainWindow::on_pushButton_RemoteOpen_clicked()
{
    quint8 cmd[41] = {0x68, 0x27, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x94,0xDC,0x07,0x01,0x00,0x80,0x00,0x81,0x00,0x01,0x01,0x02,0x04, 0x51, 0x00, 0x00, 0x00, 0x00, 0x11,0x00, 0x12,0x00,0x00,0x03,0x01,0x00,0x2D,0x2F,0x16 };
    displayHexArrayInWidget(cmd,41);
    m_serialPort->write((char*)&cmd,41);
    setAllRecFlagsFalse();
    isRemoteOpenCmdSend = true;
}

void MainWindow::on_pushButton_RemoteCloseAllowed_clicked()
{
    quint8 cmd[36] = {0x68, 0x22, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x50,0xD7,0x07,0x01,0x00,0x80,0x00,0x82,0x00,0x01,0x01,0x02,0x02, 0x51, 0x00, 0x00, 0x00, 0x00, 0x16,0x00, 0x00,0x2D,0xDC,0x16 };
    displayHexArrayInWidget(cmd,36);
    m_serialPort->write((char*)&cmd,36);
    setAllRecFlagsFalse();
    isRemoteCloseAllowedCmdSend = true;
}

void MainWindow::on_pushButton_RemoteClose_clicked()
{
    quint8 cmd[36] = {0x68, 0x22, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x50,0xD7,0x07,0x01,0x00,0x80,0x00,0x82,0x00,0x01,0x01,0x02,0x02, 0x51, 0x00, 0x00, 0x00, 0x00, 0x16,0x01, 0x00,0xF5,0xC5,0x16 };
    displayHexArrayInWidget(cmd,36);
    m_serialPort->write((char*)&cmd,36);
    setAllRecFlagsFalse();
    isRemoteCloseCmdSend = true;
}

void MainWindow::on_pushButton_calibration_clicked()
{
    quint8 cmd[26] = {0x68, 0x18, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x17,0x53,0x07,0x01,0x00,0xFF,0xA1,0x7F,0x00,0x00,0x00,0x9F,0x93, 0x16 };
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isCalibrationCmdSend = true;
}

void MainWindow::on_pushButton_FCT_clicked()
{
    quint8 cmd[26] = {0x68, 0x18, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x17,0x53,0x07,0x01,0x00,0xFF,0xA1,0x81,0x00,0x00,0x00,0xF6,0x4A, 0x16 };
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isFCTcmdSend = true;
}

void MainWindow::on_pushButton_ReadRunWord_clicked()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x20,0x42,0x02,0x00,0x00,0x24,0x91, 0x16 };
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isReadRunWordCmdSend = true;
}

void MainWindow::on_pushButton_about_clicked()
{
    QMessageBox::about(NULL,"关于软件","软件版本：V2.1.1.6 (2026-11-06 11:15:08)\n简要说明：新增有功功率偏置校准功能。\n适用产品：正泰GVT、OMCA(四可)、NB2LE-80ZT、NXBLU125D断路器等\n开发人员：Luo Zhenglei (Jason Locke)\n微信: luozhenglei\nemail: lzlei@chint.com  luozhenglei397@163.com  luozhenglei397@gmail.com\n\
Gitee url: https://gitee.com/jasonlocke/gvt-breaker-config-tool\nGitLab url: https://dy-git.chint.com/mcb/omca_-integrated_terminal/-/tree/develop_LZL/SW/CODE/OTA_Qt_Tool/Qt_serial_lzl_simplified");
}
//---------------------------645协议新增-----------------------------------------------------------------------------------------
void MainWindow::on_pushButton_ReadAddr645_clicked()
{
    quint8 cmd[12] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x13, 0x00,0xDF, 0x16 };//645通配读地址
    displayHexArrayInWidget(cmd,12);
    m_serialPort->write((char*)&cmd,12);
    setAllRecFlagsFalse();
    isReadAddr645_Send = true;
}

//void MainWindow::on_pushButton_Create645Frame_clicked()
//{
//    //先检查标识码输入是否正确
//    QString new_DI = ui->lineEdit_DI_code->text();
//    quint8 DI_length = new_DI.length();
//    qDebug()<<"DI_length: "<<DI_length;
//    if(8 != DI_length)
//    {
//        QMessageBox::critical(this,"DI set Message", "标识码长度错误");
//        return;
//    }
//    else if(!isValidString(new_DI))//检查是否有非法字符
//    {
//        QMessageBox::critical(this,"DI set Message", "标识码含有非法字符");
//        return;
//    }else{}

////    //再检查控制码输入是否正确
////    QString new_C = ui->lineEdit_Ctr_Code->text();
////    quint8 C_length = new_C.length();
////    qDebug()<<"Ctr_length: "<<C_length;
////    if(2 != C_length)
////    {
////        QMessageBox::critical(this,"CtrCode set Message", "控制码长度错误");
////        return;
////    }
////    else if(!isValidString(new_C))//检查是否有非法字符
////    {
////        QMessageBox::critical(this,"CtrCode set Message", "控制码含有非法字符");
////        return;
////    }else{}

//    //最后检查数据输入是否正确
//    QString new_data = ui->lineEdit_Data_Frame->text();
//    quint8 data_length = new_data.length();
//    qDebug()<<"data_length: "<<data_length;
//    if(0 == data_length)//数据内容为空，这是允许的
//    {
////        QMessageBox::critical(this,"data input Message", "数据长度错误");
////        return;
//    }
//    else if(0 != (data_length%2))
//    {
//        QMessageBox::critical(this,"data input Message", "数据个数不能为奇数");
//        return;
//    }
//    else if(!isValidString(new_data))//检查是否有非法字符
//    {
//        QMessageBox::critical(this,"data input Message", "输入数据含有非法字符");
//        return;
//    }else{}

//    //开始组帧
//    //先计算出校验值CS
//    quint8 data_len = data_length/2 + 12;//固定的12字节+数据长度
//    quint8 data_full[data_len] = {0};//完整数据，注意数据内容为逆序
//    data_full[0] = 0x68;
//    memcpy(&data_full[1],MAC645,6);
//    data_full[7] = 0x68;


//}

void MainWindow::on_pushButton_protocol645_clicked()
{
    QMessageBox::about(NULL,"关于本软件使用的645协议说明","参考文件：\n《配电物联〔2022〕8号 关于发布低压智能断路器通信协议的通知》\n《附录B-智能量测开关645协议扩展》");
}

void MainWindow::on_pushButton_CreateFrameSend_clicked()
{
        //先检查标识码输入是否正确
        QString new_DI = ui->lineEdit_DI_code->text();
        quint8 DI_length = new_DI.length();
        qDebug()<<"DI_length: "<<DI_length;
        if(8 != DI_length)
        {
            QMessageBox::critical(this,"DI set Message", "标识码长度错误");
            return;
        }
        else if(!isValidString(new_DI))//检查是否有非法字符
        {
            QMessageBox::critical(this,"DI set Message", "标识码含有非法字符");
            return;
        }else{}

        //再检查数据输入是否正确
        QString new_data = ui->lineEdit_Data_Frame->text();
        quint8 data_length = new_data.length();
        qDebug()<<"data_length: "<<data_length;
        if(0 == data_length)//数据内容为空，这是允许的
        {
    //        QMessageBox::critical(this,"data input Message", "数据长度错误");
    //        return;
        }
//        else if(0 != (data_length%2))
//        {
//            QMessageBox::critical(this,"data input Message", "数据个数不能为奇数");
//            return;
//        }
        else if(!isValidString(new_data))//检查是否有非法字符
        {
            QMessageBox::critical(this,"data input Message", "输入数据含有非法字符");
            return;
        }else{}
//-----------------------组帧----------------------------------------------------
        if(ui->comboBox_CtrlCode->currentText() == "11")//读操作
        {
            qDebug()<<"645 read cmd";
            quint8 DI_tmp[8] = {0}; //标识码提取
            quint8 DI[4] = {0};
            quint8 data_read_full[16] = {0x68,0,0,0,0,0,0,0x68,0x11,0x04,0,0,0,0,0,0x16};//读帧固定16字节

            //填充地址
            data_read_full[1] = MAC645[5];
            data_read_full[2] = MAC645[4];
            data_read_full[3] = MAC645[3];
            data_read_full[4] = MAC645[2];
            data_read_full[5] = MAC645[1];
            data_read_full[6] = MAC645[0];
            //填充标识码
            DI_tmp[0] = convertCharToHex(new_DI.at(0).toLatin1());
            DI_tmp[1] = convertCharToHex(new_DI.at(1).toLatin1());
            DI[3] = DI_tmp[0]*16+DI_tmp[1];
            DI_tmp[2] = convertCharToHex(new_DI.at(2).toLatin1());
            DI_tmp[3] = convertCharToHex(new_DI.at(3).toLatin1());
            DI[2] = DI_tmp[2]*16+DI_tmp[3];
            DI_tmp[4] = convertCharToHex(new_DI.at(4).toLatin1());
            DI_tmp[5] = convertCharToHex(new_DI.at(5).toLatin1());
            DI[1] = DI_tmp[4]*16+DI_tmp[5];
            DI_tmp[6] = convertCharToHex(new_DI.at(6).toLatin1());
            DI_tmp[7] = convertCharToHex(new_DI.at(7).toLatin1());
            DI[0] = DI_tmp[6]*16+DI_tmp[7];
//            if(ui->checkBox_DataOrder->isChecked())//逆序排序，常用
            {
                data_read_full[10] = DI[0]+0x33;//DI0
                data_read_full[11] = DI[1]+0x33;//DI1
                data_read_full[12] = DI[2]+0x33;//DI2
                data_read_full[13] = DI[3]+0x33;//DI3
            }
//            else //顺序排序，常用
//            {
//                data_read_full[10] = DI[3]+0x33;//DI0
//                data_read_full[11] = DI[2]+0x33;//DI1
//                data_read_full[12] = DI[1]+0x33;//DI2
//                data_read_full[13] = DI[0]+0x33;//DI3
//            }

            //计算校验码
            data_read_full[14] = Get_645_CS(data_read_full, 14);
            //在输出栏显示组好的帧
            // 使用quint8数组的指针和大小创建QByteArray
            QByteArray byteArray(reinterpret_cast<const char*>(data_read_full), 16);
            QString hexString = byteArray.toHex(' ');
            QString UpperhexString = hexString.toUpper();//转换为大写
            ui->lineEdit_manual645->setText(UpperhexString);  // 设置显示内容

            //发送组好的读数据帧
            m_serialPort->write((char*)&data_read_full,16);
            setAllRecFlagsFalse();
            isManualRead645_Send = true;

        }
        else if(ui->comboBox_CtrlCode->currentText() == "14")//写操作
        {
            qDebug()<<"645 write cmd";
//            quint8 data_len_all = 0;//写操作的数据域总长度
            //先检查密码输入是否正确
            QString new_Code = ui->lineEdit_PassCode->text();
            quint8 Code_length = new_Code.length();
            qDebug()<<"Code_length: "<<Code_length;
            if(0==Code_length) //密码为空，则为默认值000000
            {
                new_Code = "000000";
            }
            else if(6 != Code_length)
            {
                QMessageBox::critical(this,"Password set Message", "密码长度错误");
                return;
            }
            else if(!isValidString(new_Code))//检查是否有非法字符
            {
                QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
                return;
            }else{}
            //再检查操作者代码输入是否正确
            QString new_OpCode = ui->lineEdit_OperatorCode->text();
            quint8 OpCode_length = new_OpCode.length();
            qDebug()<<"OpCode_length: "<<OpCode_length;
            if(0==OpCode_length)//操作码为空，则为默认值00000000
            {
                new_OpCode = "00000000";
            }
            else if(8 != OpCode_length)
            {
                QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
                return;
            }
            else if(!isValidString(new_OpCode))//检查是否有非法字符
            {
                QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
                return;
            }else{}
//-------------------------------------------------------------------------------------------------
            quint8 DI_tmp[8] = {0}; //标识码提取
            quint8 DI[4] = {0};
            quint8 Code_tmp[6] = {0}; //密级和密码提取
            quint8 Code[3] = {0};
            quint8 data_write_full[64] = {0};//写帧长度为 24+m 字节，m为数据长度
            data_write_full[0] = 0x68;
            //填充地址
            data_write_full[1] = MAC645[5];
            data_write_full[2] = MAC645[4];
            data_write_full[3] = MAC645[3];
            data_write_full[4] = MAC645[2];
            data_write_full[5] = MAC645[1];
            data_write_full[6] = MAC645[0];

            data_write_full[7] = 0x68;
            data_write_full[8] = 0x14; //控制码14H

            //填充标识码
            DI_tmp[0] = convertCharToHex(new_DI.at(0).toLatin1());
            DI_tmp[1] = convertCharToHex(new_DI.at(1).toLatin1());
            DI[3] = DI_tmp[0]*16+DI_tmp[1];
            DI_tmp[2] = convertCharToHex(new_DI.at(2).toLatin1());
            DI_tmp[3] = convertCharToHex(new_DI.at(3).toLatin1());
            DI[2] = DI_tmp[2]*16+DI_tmp[3];
            DI_tmp[4] = convertCharToHex(new_DI.at(4).toLatin1());
            DI_tmp[5] = convertCharToHex(new_DI.at(5).toLatin1());
            DI[1] = DI_tmp[4]*16+DI_tmp[5];
            DI_tmp[6] = convertCharToHex(new_DI.at(6).toLatin1());
            DI_tmp[7] = convertCharToHex(new_DI.at(7).toLatin1());
            DI[0] = DI_tmp[6]*16+DI_tmp[7];
//            if(ui->checkBox_DataOrder->isChecked())//逆序排序，常用
            {
                data_write_full[10] = DI[0]+0x33;
                data_write_full[11] = DI[1]+0x33;
                data_write_full[12] = DI[2]+0x33;
                data_write_full[13] = DI[3]+0x33;
            }


            //填充密级和密码
            quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
            qDebug()<<"PasswordLevel: "<<PasswordLevel;
            data_write_full[14] = PasswordLevel+0x33;//密级

            Code_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
            Code_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
            Code[2] = Code_tmp[0]*16+Code_tmp[1];
            Code_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
            Code_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
            Code[1] = Code_tmp[2]*16+Code_tmp[3];
            Code_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
            Code_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
            Code[0] = Code_tmp[4]*16+Code_tmp[5];
//            if(ui->checkBox_DataOrder->isChecked())//逆序排序，常用
            {
                data_write_full[15] = Code[0]+0x33;
                data_write_full[16] = Code[1]+0x33;
                data_write_full[17] = Code[2]+0x33;
            }
            //填充操作者代码
            quint8 OpCode_tmp[8] = {0}; //操作者代码提取
            quint8 OpCode[4] = {0};
            OpCode_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
            OpCode_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
            OpCode[3] = OpCode_tmp[0]*16+OpCode_tmp[1];
            OpCode_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
            OpCode_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
            OpCode[2] = OpCode_tmp[2]*16+OpCode_tmp[3];
            OpCode_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
            OpCode_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
            OpCode[1] = OpCode_tmp[4]*16+OpCode_tmp[5];
            OpCode_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
            OpCode_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
            OpCode[0] = OpCode_tmp[6]*16+OpCode_tmp[7];
//            if(ui->checkBox_DataOrder->isChecked())//逆序排序，常用
            {
                data_write_full[18] = OpCode[0]+0x33;
                data_write_full[19] = OpCode[1]+0x33;
                data_write_full[20] = OpCode[2]+0x33;
                data_write_full[21] = OpCode[3]+0x33;
            }
            //填充data
            if(ui->comboBox_InputDataType->currentText() == "BCD") //数据格式为BCD码
            {
                if(0 == data_length) //输入数据为空
                {
                    data_write_full[9] = 0x0C;//长度为12
                    data_write_full[21] = Get_645_CS(data_write_full, 22);//检验码CS
                    data_write_full[22] = 0x16;//组帧结束
                }
                else if(!isNumOnlyString(new_data)) //BCD码只包含数字
                {
                    QMessageBox::critical(this,"Data input Message", "输入数据不满足BCD格式");
                    return;
                }
                else if(0 == data_length%2) //数据长度为偶数
                {
                    data_write_full[9] = data_length/2+12;//数据总长度

                    quint8 data_tmp[data_length] = {0}; //数据内容提取
                    quint8 Data_input[data_length/2] = {0};
                    for(quint8 i = 0;i<data_length/2;i++)
                    {
                        data_tmp[i*2] = convertCharToHex(new_data.at(data_length-2-2*i).toLatin1());
                        data_tmp[i*2+1] = convertCharToHex(new_data.at(data_length-1-2*i).toLatin1());
                        Data_input[i] = data_tmp[i*2]*16+data_tmp[i*2+1];
                        data_write_full[i+22] = Data_input[i]+0x33; //更新数据
                    }
                    data_write_full[data_length/2+22] = Get_645_CS(data_write_full, data_length/2+22);//检验码CS
                    data_write_full[data_length/2+23] = 0x16; //组帧结束
                }
                else if(1 == data_length%2) //数据长度为奇数
                {
                    data_length+=1;//长度加1，补0
                    new_data.insert(0,'0');//头部补充一个0；
                    data_write_full[9] = data_length/2+12;//总长度

                    quint8 data_tmp[data_length] = {0}; //数据内容提取
                    quint8 Data_input[data_length/2] = {0};
                    for(quint8 i = 0;i<data_length/2;i++)
                    {
                        data_tmp[i*2] = convertCharToHex(new_data.at(data_length-2-2*i).toLatin1());
                        data_tmp[i*2+1] = convertCharToHex(new_data.at(data_length-1-2*i).toLatin1());
                        Data_input[i] = data_tmp[i*2]*16+data_tmp[i*2+1];
                        data_write_full[i+22] = Data_input[i]+0x33; //更新数据
                    }
                    data_write_full[data_length/2+22] = Get_645_CS(data_write_full, data_length/2+22);//检验码CS
                    data_write_full[data_length/2+23] = 0x16; //组帧结束
                }
                //在输出栏显示组好的帧
                // 使用quint8数组的指针和大小创建QByteArray
                QByteArray byteArray(reinterpret_cast<const char*>(data_write_full), data_length/2+24);
                QString hexString = byteArray.toHex(' ');
                QString UpperhexString = hexString.toUpper();//转换为大写
                ui->lineEdit_manual645->setText(UpperhexString);  // 设置显示内容

                //发送组好的读数据帧
                displayHexArrayInWidget(data_write_full,data_length/2+24);
                m_serialPort->write((char*)&data_write_full,data_length/2+24);
                setAllRecFlagsFalse();
                isManualRead645_Send = true;
            }
            else if(ui->comboBox_InputDataType->currentText() == "ASCII") //数据格式为ASCII码
            {
                if(0 == data_length) //输入数据为空
                {
                    data_write_full[9] = 0x0C;//长度为12
                    data_write_full[21] = Get_645_CS(data_write_full, 22);//检验码CS
                    data_write_full[22] = 0x16;//组帧结束
                }
                else
                {
                    QChar data_tmp[data_length] = {0}; //数据内容提取
                    quint8 Data_input[data_length] = {0};
                    for(quint8 i = 0;i<data_length;i++)
                    {
                        data_tmp[i] = new_data.at(data_length-1-i);
                        Data_input[i] =  data_tmp[i].unicode() + 0x33;

                        data_write_full[i+22] = Data_input[i]; //更新数据
                    }
                    data_write_full[data_length+22] = Get_645_CS(data_write_full, data_length+22);//检验码CS
                    data_write_full[data_length+23] = 0x16; //组帧结束

                    //在输出栏显示组好的帧
                    // 使用quint8数组的指针和大小创建QByteArray
                    QByteArray byteArray(reinterpret_cast<const char*>(data_write_full), data_length+24);
                    QString hexString = byteArray.toHex(' ');
                    QString UpperhexString = hexString.toUpper();//转换为大写
                    ui->lineEdit_manual645->setText(UpperhexString);  // 设置显示内容

                    //发送组好的读数据帧
                    displayHexArrayInWidget(data_write_full,data_length+24);
                    m_serialPort->write((char*)&data_write_full,data_length+24);
                    setAllRecFlagsFalse();
                    isManualRead645_Send = true;
                }
            }
//            else if(ui->comboBox_InputDataType->currentText() == "BIN") //数据格式为BIN码
//            {

//            }
            else{}
        }
}

void MainWindow::on_pushButton_writeAddr645_clicked()
{
    char buffer[12] = {0};
    quint8 MAC_element[12] = {0};
    quint8 cmd[18] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x15, 0x06, 0x35, 0x33, 0x33, 0x33, 0x33, 0x33, 0x1B,0x16 };//通配写地址
    QString new_MAC = ui->lineEdit_WriteAddr645->text();
    quint8 MAC_length = new_MAC.length();
    qDebug()<<"645MAC_length: "<<MAC_length;
    if(12 != MAC_length)
    {
        QMessageBox::critical(this,"SetMAC645 Message", "地址长度错误");
        return;
    }
    else if(!isAllDigits(new_MAC))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"SetMAC645 Message", "地址含有非法字符");
        return;
    }else{}

    QByteArray byteArray_tmp = new_MAC.toUtf8();
    strncpy(buffer, byteArray_tmp.constData(), sizeof(buffer));

    for(quint8 i=0;i<12;i++)
    {
        MAC_element[i] = convertCharToHex(buffer[i]);
    }

    cmd[15] = MAC_element[0]*16+MAC_element[1]+0x33;
    cmd[14] = MAC_element[2]*16+MAC_element[3]+0x33;
    cmd[13] = MAC_element[4]*16+MAC_element[5]+0x33;
    cmd[12] = MAC_element[6]*16+MAC_element[7]+0x33;
    cmd[11] = MAC_element[8]*16+MAC_element[9]+0x33;
    cmd[10] = MAC_element[10]*16+MAC_element[11]+0x33;

    cmd[16] = Get_645_CS(cmd, 16);//检验码CS;

    displayHexArrayInWidget(cmd,18);
    m_serialPort->write((char*)&cmd,18);
    setAllRecFlagsFalse();
    isWriteMACSend645 = true;
}

void MainWindow::on_pushButton_ChangeBuadrate_clicked()
{
    quint8 cmd[13] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x17, 0x01, 0x53, 0x3D, 0x16 };
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    if(ui->comboBox_BaudrateList->currentText() == "115200")
        cmd[10] = 0x80 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "19200")
        cmd[10] = 0x40 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "9600")
        cmd[10] = 0x20 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "4800")
        cmd[10] = 0x10 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "2400")
        cmd[10] = 0x08 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "1200")
        cmd[10] = 0x04 + 0x33;
    else if(ui->comboBox_BaudrateList->currentText() == "600")
        cmd[10] = 0x02 + 0x33;
    else
        cmd[10] = 0x20 + 0x33;//默认9600

    cmd[11] = Get_645_CS(cmd, 11);//检验码CS;

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isChangeBaudrate645 = true;

}

void MainWindow::on_pushButton_ReadVolt645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x32, 0x34,0x35,0xB5,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadVolt645 = true;
}

void MainWindow::on_pushButton_ReadFreq645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x32, 0xC6,0x35,0x47,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadFreq645 = true;
}

void MainWindow::on_pushButton_ChangePWD_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x18, 0x0C, 0x33, 0x35, 0x33,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x34,0x61,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];
    //先检查标识符输入是否正确
    QString DI_Code = ui->lineEdit_DI645->text();
    quint8 DI_Code_length = DI_Code.length();
    qDebug()<<"DI_Code_length: "<<DI_Code_length;
    if(8 != DI_Code_length)
    {
        QMessageBox::critical(this,"DI_Code Message", "输入数据长度错误");
        return;
    }
    else if(!isValidString(DI_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"DI_Code Message", "输入数据含有非法字符");
        return;
    }else{}
    //更新标识符DI
    DI_tmp[0] = convertCharToHex(DI_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(DI_Code.at(1).toLatin1());
    cmd[10] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(DI_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(DI_Code.at(3).toLatin1());
    cmd[11] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(DI_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(DI_Code.at(5).toLatin1());
    cmd[12] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(DI_Code.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(DI_Code.at(7).toLatin1());
    cmd[13] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //再检查原密码输入是否正确
    QString Old_Code = ui->lineEdit_Old_PWD->text();
    quint8 Old_Code_length = Old_Code.length();
    qDebug()<<"Old_Code_length: "<<Old_Code_length;
    if(8 != Old_Code_length)
    {
        QMessageBox::critical(this,"Old_Code Message", "输入原密码长度错误");
        return;
    }
    else if(!isNumOnlyString(Old_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Old_Code Message", "输入原密码含有非法字符");
        return;
    }else{}
    //更新原密码
    DI_tmp[0] = convertCharToHex(Old_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(Old_Code.at(1).toLatin1());
    cmd[14] = DI_tmp[0]*16+DI_tmp[1];
    //判断密级是否有效
    if(cmd[14]>2)
    {
        QMessageBox::critical(this,"Old_Code Message", "输入原密码密级错误");
        return;
    }
    else
        cmd[14]+=0x33;

    DI_tmp[2] = convertCharToHex(Old_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(Old_Code.at(3).toLatin1());
    cmd[15] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(Old_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(Old_Code.at(5).toLatin1());
    cmd[16] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(Old_Code.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(Old_Code.at(7).toLatin1());
    cmd[17] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //最后检查新密码输入是否正确
    QString New_Code = ui->lineEdit_New_PWD->text();
    quint8 New_Code_length = New_Code.length();
    qDebug()<<"New_Code_length: "<<New_Code_length;
    if(8 != New_Code_length)
    {
        QMessageBox::critical(this,"New_Code Message", "输入新密码长度错误");
        return;
    }
    else if(!isNumOnlyString(New_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"New_Code Message", "输入新密码含有非法字符");
        return;
    }else{}
    //更新原密码
    DI_tmp[0] = convertCharToHex(New_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(New_Code.at(1).toLatin1());
    cmd[18] = DI_tmp[0]*16+DI_tmp[1];
    //判断密级是否有效
    if(cmd[18]>2)
    {
        QMessageBox::critical(this,"New_Code Message", "输入新密码密级错误");
        return;
    }
    else
        cmd[18]+=0x33;

    DI_tmp[2] = convertCharToHex(New_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(New_Code.at(3).toLatin1());
    cmd[19] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(New_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(New_Code.at(5).toLatin1());
    cmd[20] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(New_Code.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(New_Code.at(7).toLatin1());
    cmd[21] = DI_tmp[6]*16+DI_tmp[7]+0x33;


    //更新校验位
    cmd[22] = Get_645_CS(cmd, 22);//检验码CS;

    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isChangePWD645 = true;
}

void MainWindow::on_pushButton_ReadDateTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x36, 0x34, 0x35,0x37,0xBD,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadDateTime645 = true;
}

void MainWindow::on_pushButton_SoftwareVer645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x35, 0x34, 0x33,0x37,0xBA,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadSoftwareVere645 = true;
}

void MainWindow::on_pushButton_HardVer645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x36, 0x34, 0x33,0x37,0xBB,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadHardwareVere645 = true;
}

void MainWindow::on_pushButton_AssetCode645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x39, 0x33, 0x33,0x37,0xBD,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadAssetCode645 = true;
}

void MainWindow::on_pushButton_ProductionDate645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x34, 0x33,0x37,0xB9,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadProductionDate645 = true;
}

void MainWindow::on_pushButton_RunWord1_645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x38, 0x33,0x37,0xBD,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadRunWord1_645 = true;
}

void MainWindow::on_pushButton_RunWord2_645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x35, 0x38, 0x33,0x37,0xBE,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadRunWord2_645 = true;
}

void MainWindow::on_pushButton_ReadCtrlWord1_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x37, 0x38,0x37,0xC1,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadCtrlWord1_645 = true;
}

void MainWindow::on_pushButton_ReadCtrlWord3_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x36, 0x37, 0x38,0x37,0xC3,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadCtrlWord3_645 = true;
}

void MainWindow::on_pushButton_ReadCtrlWord7_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3A, 0x37, 0x38,0x37,0xC7,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadCtrlWord7_645 = true;
}

void MainWindow::on_pushButton_WriteCtrlWord1_clicked()
{
    quint8 cmd[25] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0D, 0x34, 0x37, 0x38,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x98,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //填充数据
    if(ui->checkBox_WriteCtrlWord1->isChecked())
        cmd[22] = 0x08+0x33;
    else
        cmd[22] = 0x33;

    //计算校验码
    cmd[23] = Get_645_CS(cmd, 23);
//    HexPrint((char*)&cmd,25);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isWriteCtrlWord1 = true;
}

void MainWindow::on_pushButton_WriteCtrlWord3_clicked()
{
    quint8 cmd[25] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0D, 0x36, 0x37, 0x38,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x9A,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //填充数据
    if(ui->checkBox_LPProtectEn->isChecked()) //缺相保护打开
        cmd[22] = 0x80;
    else
        cmd[22] = 0;

    if(ui->checkBox_UVProtectEn->isChecked()) //欠压保护打开
        cmd[22] |= 0x20;

    if(ui->checkBox_OVProtectEn->isChecked()) //过压保护打开
        cmd[22] |= 0x08;

    cmd[22]+=0x33;
    //计算校验码
    cmd[23] = Get_645_CS(cmd, 23);
//    HexPrint((char*)&cmd,25);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isWriteCtrlWord3 = true;
}

void MainWindow::on_pushButton_WriteCtrlWord7_clicked()
{
    quint8 cmd[25] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0D, 0x3A, 0x37, 0x38,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x9E,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //填充数据
    if(ui->checkBox_OFUFEn->isChecked()) //过欠频保护打开
        cmd[22] = 0x04;
    else
        cmd[22] = 0;

    if(ui->checkBox_IslandFreqEn->isChecked()) //频率孤岛
        cmd[22] |= 0x02;

    if(ui->checkBox_IslandVoltEn->isChecked()) //幅值孤岛
        cmd[22] |= 0x01;


    cmd[22]+=0x33;
    //计算校验码
    cmd[23] = Get_645_CS(cmd, 23);
//    HexPrint((char*)&cmd,25);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isWriteCtrlWord7 = true;
}

void MainWindow::on_pushButton_ReadOpenTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x34, 0x34,0x39,0xBC,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadOpenTimeSend = true;
}

void MainWindow::on_pushButton_WriteOpenTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1C, 0x0E, 0x34, 0x34, 0x34,0x39,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x34,0x35,0xD2,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(0 != PasswordLevel) //只有密级00有权限控制
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的时间数据有效性
    QString new_TimeData = ui->lineEdit_WriteOpenTimeData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"TimeData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入时间数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的时间数据长度不超过2位数");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的时间数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = DI_tmp[0]+0x33;//时间数据
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
//        cmd[22] = DI_tmp[0]*16+DI_tmp[1]+0x33;//时间数据
        cmd[22] = DI_tmp[0]*10+DI_tmp[1]+0x33;
    }

    if(ui->comboBox_OpenTimeUnit->currentText() == "分")
        cmd[23] = 0x02+0x33;//时间数据
    else if(ui->comboBox_OpenTimeUnit->currentText() == "时")
        cmd[23] = 0x03+0x33;//时间数据

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteOpenTime = true;
}

void MainWindow::on_pushButton_CancelOpen_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1C, 0x0C, 0x35, 0x34, 0x34,0x39,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x68,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(0 != PasswordLevel) //只有密级00有权限控制
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,24);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isCancelOpenSend = true;
}

void MainWindow::on_pushButton_ReadCloseTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x35, 0x34,0x39,0xBD,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadCloseTimeSend = true;
}

void MainWindow::on_pushButton_WriteCloseTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1C, 0x0E, 0x34, 0x35, 0x34,0x39,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x34,0x35,0xD3,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(0 != PasswordLevel) //只有密级00有权限控制
    {
        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的时间数据有效性
    QString new_TimeData = ui->lineEdit_WriteCloseTimeData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"TimeData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"CloseTimeSet Message", "输入时间数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"CloseTimeSet Message", "输入的时间数据长度不超过2位数");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"CloseTimeSet Message", "输入的时间数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = DI_tmp[0]+0x33;//时间数据
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
//        cmd[22] = DI_tmp[0]*16+DI_tmp[1]+0x33;//时间数据
        cmd[22] = DI_tmp[0]*10+DI_tmp[1]+0x33;
    }

    if(ui->comboBox_CloseTimeUnit->currentText() == "分")
        cmd[23] = 0x02+0x33;//时间数据
    else if(ui->comboBox_CloseTimeUnit->currentText() == "时")
        cmd[23] = 0x03+0x33;//时间数据

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteCloseTime = true;
}

void MainWindow::on_pushButton_CancelClose_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1C, 0x0C, 0x35, 0x35, 0x34,0x39,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x69,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(0 != PasswordLevel) //只有密级00有权限控制
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,24);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isCancelCloseSend = true;
}

void MainWindow::on_pushButton_GetSysDateTime_clicked()
{
    QDateTime dateTime = QDateTime::currentDateTime(); // 获取当前系统日期和时间

//    qDebug() << "Year:" << dateTime.date().year(); // 年份 2025
//    qDebug() << "Month:" << dateTime.date().month(); // 月份
//    qDebug() << "Day:" << dateTime.date().day(); // 日
//    qDebug() << "Hour:" << dateTime.time().hour(); // 小时
//    qDebug() << "Minute:" << dateTime.time().minute(); // 分钟
//    qDebug() << "Second:" << dateTime.time().second(); // 秒
//    qDebug() << "Millisecond:" << dateTime.time().msec(); // 毫秒

    QString currentTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    ui->lineEdit_SetDateTime->setText(currentTime);

    quint8 cmd[18] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x08, 0x06, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x12,0x16 };//广播校时

    cmd[15] = HexToBcd(dateTime.date().year() - 2000) + 0x33;
    cmd[14] = HexToBcd(dateTime.date().month()) + 0x33;
    cmd[13] = HexToBcd(dateTime.date().day()) + 0x33;
    cmd[12] = HexToBcd(dateTime.time().hour()) + 0x33;
    cmd[11] = HexToBcd(dateTime.time().minute()) + 0x33;
    cmd[10] = HexToBcd(dateTime.time().second()) + 0x33;

    cmd[16] = Get_645_CS(cmd, 16);//检验码CS;

    displayHexArrayInWidget(cmd,18);
    m_serialPort->write((char*)&cmd,18);
    setAllRecFlagsFalse();
    isAutoSetDateTimeSend = true;

}

void MainWindow::on_pushButton_OtherFunction_clicked()
{
    QMessageBox::about(NULL,"说明","其它功能如事件记录、读写时间等请使用前端同事开发的 Smart Power Manager Beta 上位机。");
}

void MainWindow::on_pushButton_ManualSetTime_clicked()
{
    quint8 cmd[18] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x08, 0x06, 0x34, 0x34, 0x34, 0x34, 0x34, 0x34, 0x12,0x16 };//广播校时
    quint8 DI_tmp[2] = {0};
    //检查输入的时间数据有效性
    //年
    QString newInput_year = ui->lineEdit_InputYear->text();//“年”数据检查
    quint8 TimeData_length = newInput_year.length();
    qDebug()<<"newInput_year_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'年'数据不能为空");
        return;
    }
    else if(2 != TimeData_length)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'年'数据长度必须为2位数（年的后两位）");
        return;
    }
    else if(!isNumOnlyString(newInput_year))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'年'数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(newInput_year.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(newInput_year.at(1).toLatin1());
    cmd[15] = DI_tmp[0]*16+DI_tmp[1]+0x33;//时间数据
//    qDebug() << "Year:" << (cmd[15]-0x33);
    //月
    QString newInput_month = ui->lineEdit_InputMonth->text();
    TimeData_length = newInput_month.length();
    qDebug()<<"newInput_month_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'月'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'月'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_month))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'月'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        cmd[14] = DI_tmp[0]+0x33;
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_month.at(1).toLatin1());
        if((DI_tmp[0]*10+DI_tmp[1])>12)
        {
            QMessageBox::critical(this,"OpenTimeSet Message", "输入'月'数据不能超过12");
            return;
        }

        cmd[14] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    }
//    qDebug() << "Month:" << (cmd[14]-0x33);
    //日
    QString newInput_day = ui->lineEdit_InputDay->text();
    TimeData_length = newInput_day.length();
    qDebug()<<"newInput_day_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'日'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_day))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        cmd[13] = DI_tmp[0]+0x33;
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_day.at(1).toLatin1());

        cmd[13] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    }
//    qDebug() << "Day:" << (cmd[13]-0x33);
    //时
    QString newInput_hour = ui->lineEdit_InputHour->text();
    TimeData_length = newInput_hour.length();
    qDebug()<<"newInput_hour_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'时'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'时'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_hour))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'时'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        cmd[12] = DI_tmp[0]+0x33;
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_hour.at(1).toLatin1());
        cmd[12] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    }
//    qDebug() << "Hour:" << (cmd[12]-0x33);
    //分
    QString newInput_min = ui->lineEdit_InputMin->text();
    TimeData_length = newInput_min.length();
    qDebug()<<"newInput_min_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'分'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'分'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_min))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'分'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        cmd[11] = DI_tmp[0]+0x33;
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_min.at(1).toLatin1());
        cmd[11] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    }
//    qDebug() << "Min:" << (cmd[11]-0x33);
    //秒
    QString newInput_sec = ui->lineEdit_InputSec->text();
    TimeData_length = newInput_sec.length();
    qDebug()<<"newInput_sec_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'秒'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'秒'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_sec))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'秒'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        cmd[10] = DI_tmp[0]+0x33;
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_sec.at(1).toLatin1());
        cmd[10] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    }
//    qDebug() << "Sec:" << (cmd[10]-0x33);
//----------------------------------------------------------------------
    cmd[16] = Get_645_CS(cmd, 16);//检验码CS;

    displayHexArrayInWidget(cmd,18);
    m_serialPort->write((char*)&cmd,18);
    setAllRecFlagsFalse();
    isManualSetDateTimeSend = true;
}

void MainWindow::on_pushButton_ReadOpenTotalCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x33, 0xB4,0x36,0x37,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadTotalOpenCntSend = true;
}

void MainWindow::on_pushButton_ClearTotalOpenCnt_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1B, 0x0C, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xFF, 0x34, 0x63,0x36,0x5D,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(PasswordLevel>1) //只有密级00和01有权限清除
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00或01");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[10] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[13] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[12] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[11] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[14] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isClearTotalOpenCntSend = true;
}

void MainWindow::on_pushButton_OVOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x39, 0x33, 0xB4,0x36,0x3D,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadOVOpenCntSend = true;
}

void MainWindow::on_pushButton_UVOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3A, 0x33, 0xB4,0x36,0x3E,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadUVOpenCntSend = true;
}

void MainWindow::on_pushButton_LPOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3B, 0x33, 0xB4,0x36,0x3F,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadLPOpenCntSend = true;
}

void MainWindow::on_pushButton_LVOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x53, 0x33, 0xB4,0x36,0x57,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadLVOpenCntSend = true;
}

void MainWindow::on_pushButton_RemoteOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x4A, 0x33, 0xB4,0x36,0x4E,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadRemoteOpenCntSend = true;
}

void MainWindow::on_pushButton_IslandOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x48, 0x33, 0xB4,0x36,0x4C,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadIslandOpenCntSend = true;
}

void MainWindow::on_pushButton_ManualOpenCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x54, 0x33, 0xB4,0x36,0x58,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadManualOpenCntSend = true;
}

void MainWindow::on_pushButton_ClearOpenDataCnt_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x34, 0x63,0x36,0xE7,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadClearOpenCntSend = true;
}

void MainWindow::on_pushButton_ClearProtectOpenCnt_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1B, 0x0C, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xFF, 0x33, 0xB4,0x36,0xAD,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(PasswordLevel>1) //只有密级00和01有权限清除
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00或01");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[10] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[13] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[12] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[11] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[14] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isClearAllProtectOpenCntSend = true;
}

void MainWindow::on_pushButton_TotalRunTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x34, 0xB4,0x36,0x39,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadTotalRunTimeSend = true;
}

void MainWindow::on_pushButton_ClearTotalRunTime_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1B, 0x0C, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xFF, 0x34, 0xB4,0x36,0xAE,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(PasswordLevel>1) //只有密级00和01有权限清除
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00或01");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[10] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[13] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[12] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[11] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[14] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isClearTotalRunTimeSend = true;
}

void MainWindow::on_pushButton_StateChangeNewAdd_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x37, 0x35, 0xB4,0x36,0x3D,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadStateChangeNewAddSend = true;
}

void MainWindow::on_pushButton_ClearAll_clicked()
{
    quint8 cmd[24] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x1B, 0x0C, 0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xFF, 0xFF, 0xFF,0xFF,0x8D,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    if(PasswordLevel>1) //只有密级00和01有权限清除
    {
        QMessageBox::critical(this,"Operator Code Message", "密级权限不够，请切换至00或01");
        return;
    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[10] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[13] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[12] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[11] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[14] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //计算校验码
    cmd[22] = Get_645_CS(cmd, 22);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,24);
    m_serialPort->write((char*)&cmd,24);
    setAllRecFlagsFalse();
    isClearAllSend = true;
}

void MainWindow::on_pushButton_ReadOVThreshold_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x35, 0x34,0x37,0xBB,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadReadOVThresholdSend = true;
}

void MainWindow::on_pushButton_ReadUVThreshold_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x36, 0x35, 0x34,0x37,0xBD,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadReadUVThresholdSend = true;
}

void MainWindow::on_pushButton_ReadOVDelayTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x35, 0x35, 0x34,0x37,0xBC,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadOVDelayTimeSend = true;
}

void MainWindow::on_pushButton_ReadUVDelayTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x37, 0x35, 0x34,0x37,0xBE,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadUVDelayTimeSend = true;
}

void MainWindow::on_pushButton_WriteOVThreshold_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x34, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0xB3,0x5B,0x6E,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
//    if(0 != PasswordLevel) //只有密级00有权限控制
//    {
//        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
//        return;
//    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteOVThresData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(4 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为4位数且范围：[253V,290V]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteOVThresholdSend = true;
}

void MainWindow::on_pushButton_WriteUVThreshold_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x36, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x53,0x49,0xFE,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteUVThresData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(4 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为4位数且范围：[154V,187V]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteUVThresholdSend = true;
}

void MainWindow::on_pushButton_WriteOVDelayTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x35, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x35,0x33,0xC9,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
//    if(0 != PasswordLevel) //只有密级00有权限控制
//    {
//        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
//        return;
//    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteOVDelayTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为1或2位数且范围：[1,10]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    if(1 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] = 0;
        DI_tmp[3] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] =convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(1).toLatin1()); //260114 忘记改了
        cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }

//    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
//    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
//    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
//    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
//    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
//    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteOVDelayTimeSend = true;
}

void MainWindow::on_pushButton_WriteUVDelayTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x37, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x35,0x33,0xCB,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
//    if(0 != PasswordLevel) //只有密级00有权限控制
//    {
//        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
//        return;
//    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteUVDelayTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为1或2位数且范围：[1,10]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    if(1 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] = 0;
        DI_tmp[3] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] =convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(1).toLatin1());//260114
        cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }

//    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
//    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
//    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
//    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
//    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
//    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteUVDelayTimeSend = true;
}

void MainWindow::on_pushButton_ReadOVRecoverThres_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3B, 0x35, 0x34,0x37,0xC2,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadOVRecoverThresSend = true;
}

void MainWindow::on_pushButton_ReadUVRecoverThres_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3D, 0x35, 0x34,0x37,0xC4,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadUVRecoverThresSend = true;
}

void MainWindow::on_pushButton_ReadOVRecoverDelayTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3C, 0x35, 0x34,0x37,0xC3,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadOVRecoverDelayTimeSend = true;
}

void MainWindow::on_pushButton_ReadUVRecoverDelayTime_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x3E, 0x35, 0x34,0x37,0xC5,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isReadUVRecoverDelayTimeSend = true;
}

void MainWindow::on_pushButton_WriteOVRecoverThres_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x3B, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x83,0x5A,0x44,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteOVRecoverThresData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVRecoverThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(4 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为4位数且范围：[243V, 280V]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteOVRecoverThresSend = true;
}

void MainWindow::on_pushButton_WriteUVRecoverThres_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x3D, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x83,0x4B,0x37,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteUVRecoverThresData->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(4 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为4位数且范围：[164V, 197V]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
    cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
    cmd[23] = HexToBcd(DI_tmp[0]*10+DI_tmp[1])+0x33;

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteUVRecoverThresSend = true;
}

void MainWindow::on_pushButton_WriteOVRecoverDelayTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x3C, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x3A,0x33,0xD5,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
//    if(0 != PasswordLevel) //只有密级00有权限控制
//    {
//        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
//        return;
//    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteOVRecoverDelayTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为1或2位数且范围：[2,10]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    if(1 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] = 0;
        DI_tmp[3] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] =convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(1).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteOVRecoverDelayTimeSend = true;
}

void MainWindow::on_pushButton_WriteUVRecoverDelayTime_clicked()
{
    quint8 cmd[26] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x14, 0x0E, 0x3E, 0x35, 0x34,0x37,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x33,0x3A,0x33,0xD7,0x16};
    quint8 DI_tmp[8] = {0};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    //检查密码输入是否正确
    QString new_Code = ui->lineEdit_PassCode->text();
    quint8 Code_length = new_Code.length();
    qDebug()<<"Code_length: "<<Code_length;
    if(0==Code_length) //密码为空，则为默认值000000
    {
        new_Code = "000000";
    }
    else if(6 != Code_length)
    {
        QMessageBox::critical(this,"Password set Message", "密码长度错误");
        return;
    }
    else if(!isValidString(new_Code))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Password set Message", "密码含有非法字符");
        return;
    }else{}

    //替换密级和密码
    quint8 PasswordLevel = ui->comboBox_PassCodeLevel->currentText().toInt();
//    if(0 != PasswordLevel) //只有密级00有权限控制
//    {
//        QMessageBox::critical(this,"Operator PWD Message", "密级权限不够，请切换至00");
//        return;
//    }
    qDebug()<<"PasswordLevel: "<<PasswordLevel;
    cmd[14] = PasswordLevel+0x33;//密级

    DI_tmp[0] = convertCharToHex(new_Code.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_Code.at(1).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1]+0x33;//P2
    DI_tmp[2] = convertCharToHex(new_Code.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_Code.at(3).toLatin1());
    cmd[16] = DI_tmp[2]*16+DI_tmp[3]+0x33;//P1
    DI_tmp[4] = convertCharToHex(new_Code.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_Code.at(5).toLatin1());
    cmd[15] = DI_tmp[4]*16+DI_tmp[5]+0x33;//P0

    //再检查操作者代码输入是否正确
    QString new_OpCode = ui->lineEdit_OperatorCode->text();
    quint8 OpCode_length = new_OpCode.length();
    qDebug()<<"OpCode_length: "<<OpCode_length;
    if(0==OpCode_length)//操作码为空，则为默认值00000000
    {
        new_OpCode = "00000000";
    }
    else if(8 != OpCode_length)
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码长度错误");
        return;
    }
    else if(!isValidString(new_OpCode))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Operator Code Message", "操作者代码含有非法字符");
        return;
    }else{}

    //替换操作者代码
    DI_tmp[0] = convertCharToHex(new_OpCode.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_OpCode.at(1).toLatin1());
    cmd[21] = DI_tmp[0]*16+DI_tmp[1]+0x33;
    DI_tmp[2] = convertCharToHex(new_OpCode.at(2).toLatin1());
    DI_tmp[3] = convertCharToHex(new_OpCode.at(3).toLatin1());
    cmd[20] = DI_tmp[2]*16+DI_tmp[3]+0x33;
    DI_tmp[4] = convertCharToHex(new_OpCode.at(4).toLatin1());
    DI_tmp[5] = convertCharToHex(new_OpCode.at(5).toLatin1());
    cmd[19] = DI_tmp[4]*16+DI_tmp[5]+0x33;
    DI_tmp[6] = convertCharToHex(new_OpCode.at(6).toLatin1());
    DI_tmp[7] = convertCharToHex(new_OpCode.at(7).toLatin1());
    cmd[18] = DI_tmp[6]*16+DI_tmp[7]+0x33;

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_WriteUVRecoverDelayTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length) //密码为空，则为默认值000000
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为1或2位数且范围：[2,10]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    if(1 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] = 0;
        DI_tmp[3] = convertCharToHex(new_TimeData.at(0).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = 0;
        DI_tmp[1] = 0;
        DI_tmp[2] =convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(1).toLatin1());
        cmd[22] = HexToBcd(DI_tmp[2]*10+DI_tmp[3])+0x33;
        cmd[23] = 0x33;
    }

    //计算校验码
    cmd[24] = Get_645_CS(cmd, 24);
//    HexPrint((char*)&cmd,26);
    //发送组好的数据帧
    displayHexArrayInWidget(cmd,26);
    m_serialPort->write((char*)&cmd,26);
    setAllRecFlagsFalse();
    isWriteUVRecoverDelayTimeSend = true;
}

void MainWindow::on_pushButton_Calibration645_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x33, 0x33,0xBD,0x3D,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isCalibration645Send = true;
}

void MainWindow::on_pushButton_MotorPosMCBPos_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x34, 0x43,0xBD,0x4E,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isMotorPosMCBPosSend = true;
}

void MainWindow::on_pushButton_CheckProtectEvent_clicked()
{
    bool ok;
    QString ID_string = ui->comboBox_ProtectEventCnt->currentText();
    QVariant ID_Num(ID_string);
    quint8 EventID = ID_Num.toInt(&ok);
//    if(ok)
//    {
//        qDebug()<<"Event ID: "<<EventID;
//    }
//    else
//    {
//        qDebug()<<"Event ID转换失败";
//        return;
//    }

    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x33, 0xC1,0x36,0x45,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    switch (EventID) {
    case 1: //上1次事件
        qDebug()<<"上1次故障事件查询";
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 2:
        qDebug()<<"上2次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 3:
        qDebug()<<"上3次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 4:
        qDebug()<<"上4次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 5:
        qDebug()<<"上5次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 6:
        qDebug()<<"上6次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 7:
        qDebug()<<"上7次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 8:
        qDebug()<<"上8次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 9:
        qDebug()<<"上9次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 10:
        qDebug()<<"上10次故障事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    default:
        break;
    }
    setAllRecFlagsFalse();
    isEventCheckNo = true;
}

void MainWindow::on_pushButton_CheckStateChangeEvent_clicked()
{
    bool ok;
    QString ID_string = ui->comboBox_StateChangeCnt->currentText();
    QVariant ID_Num(ID_string);
    quint8 EventID = ID_Num.toInt(&ok);

    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x33, 0xC4,0x36,0x48,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    switch (EventID) {
    case 1: //上1次事件
        qDebug()<<"上1次闸位变化事件查询";
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 2:
        qDebug()<<"上2次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 3:
        qDebug()<<"上3次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 4:
        qDebug()<<"上4次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 5:
        qDebug()<<"上5次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 6:
        qDebug()<<"上6次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 7:
        qDebug()<<"上7次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 8:
        qDebug()<<"上8次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 9:
        qDebug()<<"上9次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 10:
        qDebug()<<"上10次闸位变化事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    default:
        break;
    }
    setAllRecFlagsFalse();
    isStateChangeNo = true;
}

void MainWindow::on_pushButton_CheckIslandEvent_clicked()
{
    bool ok;
    QString ID_string = ui->comboBox_IslandEvent->currentText();
    QVariant ID_Num(ID_string);
    quint8 EventID = ID_Num.toInt(&ok);

    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x33, 0xCC,0x36,0x50,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    switch (EventID) {
    case 1: //上1次事件
        qDebug()<<"上1次孤岛事件查询";
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 2:
        qDebug()<<"上2次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 3:
        qDebug()<<"上3次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 4:
        qDebug()<<"上4次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 5:
        qDebug()<<"上5次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 6:
        qDebug()<<"上6次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 7:
        qDebug()<<"上7次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 8:
        qDebug()<<"上8次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 9:
        qDebug()<<"上9次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 10:
        qDebug()<<"上10次孤岛事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    default:
        break;
    }
    setAllRecFlagsFalse();
    isIslandEventNo = true;
}

void MainWindow::on_pushButton_CheckSwitchingEvent_clicked()
{
    bool ok;
    QString ID_string = ui->comboBox_SwitchingEventCnt->currentText();
    QVariant ID_Num(ID_string);
    quint8 EventID = ID_Num.toInt(&ok);

    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x34, 0x33, 0xC2,0x36,0x46,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    switch (EventID) {
    case 1:
        qDebug()<<"上1次投退事件查询";
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 2:
        qDebug()<<"上2次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 3:
        qDebug()<<"上3次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 4:
        qDebug()<<"上4次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 5:
        qDebug()<<"上5次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 6:
        qDebug()<<"上6次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 7:
        qDebug()<<"上7次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 8:
        qDebug()<<"上8次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 9:
        qDebug()<<"上9次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    case 10:
        qDebug()<<"上10次投退事件查询";
        cmd[10] +=EventID-1;
        cmd[14] = Get_645_CS(cmd, 14);//检验码CS;
        displayHexArrayInWidget(cmd,16);
        m_serialPort->write((char*)&cmd,16);
        break;
    default:
        break;
    }

    displayHexArrayInWidget(cmd,16);
    setAllRecFlagsFalse();
    isSwitchingEventNo = true;
}

//======================研发测试区==================================================================
void MainWindow::OpenCmdSend(void)
{
    quint8 cmd[41] = {0x68, 0x27, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x94,0xDC,0x07,0x01,0x00,0x80,0x00,0x81,0x00,0x01,0x01,0x02,0x04, 0x51, 0x00, 0x00, 0x00, 0x00, 0x11,0x00, 0x12,0x00,0x00,0x03,0x01,0x00,0x2D,0x2F,0x16 };
    displayHexArrayInWidget(cmd,41);
    m_serialPort->write((char*)&cmd,41);
}

void MainWindow::CloseCmdSend(void)
{
    quint8 cmd[36] = {0x68, 0x22, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x50,0xD7,0x07,0x01,0x00,0x80,0x00,0x82,0x00,0x01,0x01,0x02,0x02, 0x51, 0x00, 0x00, 0x00, 0x00, 0x16,0x01, 0x00,0xF5,0xC5,0x16 };
    displayHexArrayInWidget(cmd,36);
    m_serialPort->write((char*)&cmd,36);
}

void MainWindow::CloseStatusCheck()
{
    quint8 cmd[25] = {0x68, 0x17, 0x00, 0x43, 0x45, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,0x00,0x5B,0x4F,0x05,0x01,0x00,0x20,0x42,0x02,0x00,0x00,0x24,0x91, 0x16 };
    displayHexArrayInWidget(cmd,25);
    m_serialPort->write((char*)&cmd,25);
    setAllRecFlagsFalse();
    isCloseStatusCheckSend = true;
}

void MainWindow::timerAutoOCProcess()
{
    AutoCloseOpen_timer->stop();

    if(!isCloseCMD_send)
    {
        setAllRecFlagsFalse();
        isCloseCMD_send = true;
        CloseCmdSend();
        ui->label_60->setStyleSheet("QLabel { color : black; }");
        ui->label_58->setStyleSheet("QLabel { color : red; }");

        CheckCloseStatus_timer->start(2000);//启动检测合闸定时器，x秒后读状态判断是否合闸成功。
        CloseCmdSend_times++;
        QString text_data = QString::number(CloseCmdSend_times);
        ui->lineEdit_SendCloseCmdCnt->setText(text_data);

        AutoCloseOpen_timer->start(InputCloseDelayTime*1000);
        qDebug()<<"CloseCmd send.";
    }
    else
    {
        isCloseCMD_send = false;
        OpenCmdSend();
        ui->label_60->setStyleSheet("QLabel { color : red; }");
        ui->label_58->setStyleSheet("QLabel { color : black; }");

        AutoCloseOpen_timer->start(InputOpenDelayTime*1000);
        qDebug()<<"OpenCmd send.";
    }
}

void MainWindow::timerCheckCloseProcess()
{
    CheckCloseStatus_timer->stop();
    CloseStatusCheck();
}

void MainWindow::on_pushButton_AutoCloseOpenStart_clicked()
{
    isCloseCMD_send = false;

    //检查输入的时间数据有效性
    //合闸时间有效性
    quint8 DI_tmp[2] = {0};
    QString CloseDelayTimeStr = ui->lineEdit_AutoCloseDelay->text();
    quint8 CloseDelayTimeStr_length = CloseDelayTimeStr.length();
    qDebug()<<"TimeData_length: "<<CloseDelayTimeStr_length;
    if(0==CloseDelayTimeStr_length)
    {
        QMessageBox::critical(this,"Message", "合闸延时时间不能为空");
        return;
    }
    else if(CloseDelayTimeStr_length>2)
    {
        QMessageBox::critical(this,"Message", "合闸延时时间数据长度不超过2位数");
        return;
    }
    else if(!isNumOnlyString(CloseDelayTimeStr))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入的合闸时间数据只能包含数字");
        return;
    }else{}

    if(1==CloseDelayTimeStr_length)
    {
        DI_tmp[0] = convertCharToHex(CloseDelayTimeStr.at(0).toLatin1());
        if(DI_tmp[0]<=3)
        {
            QMessageBox::critical(this,"Message", "合闸延时时间要大于3s");
            return;
        }
        InputCloseDelayTime = DI_tmp[0];
        qDebug()<<"InputCloseDelayTime: "<<InputCloseDelayTime;
    }
    else if(2==CloseDelayTimeStr_length)
    {
        DI_tmp[0] = convertCharToHex(CloseDelayTimeStr.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(CloseDelayTimeStr.at(1).toLatin1());
        InputCloseDelayTime = DI_tmp[0]*10+DI_tmp[1];
        qDebug()<<"InputCloseDelayTime: "<<InputCloseDelayTime;
    }
    //分闸时间有效性
    QString OpenDelayTimeStr = ui->lineEdit_AutoOpenDelay->text();
    quint8 OpenDelayTimeStr_length = OpenDelayTimeStr.length();
    qDebug()<<"OpenDelayTimeStr: "<<OpenDelayTimeStr_length;
    if(0==OpenDelayTimeStr_length)
    {
        QMessageBox::critical(this,"Message", "分闸延时时间不能为空");
        return;
    }
    else if(OpenDelayTimeStr_length>2)
    {
        QMessageBox::critical(this,"Message", "分闸延时时间数据长度不超过2位数");
        return;
    }
    else if(!isNumOnlyString(OpenDelayTimeStr))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入的分闸时间数据只能包含数字");
        return;
    }else{}

    if(1==OpenDelayTimeStr_length)
    {
        DI_tmp[0] = convertCharToHex(OpenDelayTimeStr.at(0).toLatin1());
        if(DI_tmp[0]<=2)
        {
            QMessageBox::critical(this,"Message", "分闸延时时间要大于2s");
            return;
        }
        InputOpenDelayTime = DI_tmp[0];
        qDebug()<<"InputOpenDelayTime: "<<InputOpenDelayTime;
    }
    else if(2==OpenDelayTimeStr_length)
    {
        DI_tmp[0] = convertCharToHex(OpenDelayTimeStr.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(OpenDelayTimeStr.at(1).toLatin1());
        InputOpenDelayTime = DI_tmp[0]*10+DI_tmp[1];
        qDebug()<<"InputOpenDelayTime: "<<InputOpenDelayTime;
    }

    ui->pushButton_AutoCloseOpenStop->setEnabled(true);
    ui->pushButton_AutoCloseOpenStart->setEnabled(false);

    ui->lineEdit_AutoCloseDelay->setEnabled(false);
    ui->lineEdit_AutoOpenDelay->setEnabled(false);

    ui->lineEdit_AutoCloseSuccessCnt->setText("0");
    ui->lineEdit_SendCloseCmdCnt->setText("0");
    CloseCheckOK_times = 0;
    CloseCmdSend_times = 0;

    OpenCmdSend();
    ui->label_60->setStyleSheet("QLabel { color : red; }");     // 设置文本颜色为红色
    ui->label_58->setStyleSheet("QLabel { color : black; }");

    qDebug()<<"first OpenCmd send.";
    AutoCloseOpen_timer->start(InputOpenDelayTime*1000);

}

void MainWindow::on_pushButton_AutoCloseOpenStop_clicked()
{
    AutoCloseOpen_timer->stop();
    CheckCloseStatus_timer->stop();

    CloseCheckOK_times = 0;
    CloseCmdSend_times = 0;
    isCloseCMD_send = false;

    ui->label_60->setStyleSheet("QLabel { color : black; }");
    ui->label_58->setStyleSheet("QLabel { color : black; }");

    ui->pushButton_AutoCloseOpenStop->setEnabled(false);
    ui->pushButton_AutoCloseOpenStart->setEnabled(true);

    ui->lineEdit_AutoCloseDelay->setEnabled(true);
    ui->lineEdit_AutoOpenDelay->setEnabled(true);

    qDebug()<<"Stop OC cmd send.";
}

void MainWindow::on_pushButton_ResetConfig_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x33, 0x33, 0x44,0xBD,0x4E,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isResetConfigSend = true;
}

void MainWindow::on_pushButton_GetDeviceType_clicked()
{
    quint8 cmd[16] = {0x68, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x11, 0x04, 0x36, 0x33, 0x33,0x37,0xBA,0x16};
    //更新新地址
    cmd[1] = MAC645[5];
    cmd[2] = MAC645[4];
    cmd[3] = MAC645[3];
    cmd[4] = MAC645[2];
    cmd[5] = MAC645[1];
    cmd[6] = MAC645[0];

    cmd[14] = Get_645_CS(cmd, 14);//检验码CS;

    displayHexArrayInWidget(cmd,16);
    m_serialPort->write((char*)&cmd,16);
    setAllRecFlagsFalse();
    isGetDeviceTypeSend = true;
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    quint16 remain = 0;

//    quint32 Ver_offset = 0x0003E000;
//    quint32 Ver_value = 0;

    //get file name
    //fileName = QFileDialog::getOpenFileName(this,"Open File",QDir::currentPath());//打开所有格式文件
    fileName = QFileDialog::getOpenFileName(this,QString::fromLocal8Bit("bin file"),qApp->applicationDirPath(),QString::fromLocal8Bit("bin File(*.bin)"));//只打开bin格式文件
//    qDebug()<< "fileName is" << fileName;
    ui->lineEditBinFliePath->setText (fileName);
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,"Error Message", "Please Select a bin File");
        return;
    }
    QFileInfo *pcsfileInfo = new QFileInfo(fileName);
    binSize = pcsfileInfo->size(); //byte个数
    qDebug()<< "binSize is " << binSize;
    ui->lineEdit_OtaFileSize->setText(QString::number(binSize));
    remain = binSize % PACKAGE_SIZE;
    if(remain>0)
    {
        package_num = (binSize / PACKAGE_SIZE) +1;
    }
    else
    {
        package_num = binSize / PACKAGE_SIZE;
    }
    package_ID = 0;//每打开一个文件，从第0包开始
    qDebug("package_num = %d, remain = %d ",package_num,remain);

    QFile* file = new QFile;
    file->setFileName(fileName);
    bool ok = file->open(QIODevice::ReadOnly);
    if(ok) //从bin文件中读取版本号
    {
//        if(!file->seek(Ver_offset)) //文件指针偏移到Ver_offset
//        {
//            qDebug()<< "无法定位到偏移量";
//        }
//        else
//        {
//            QDataStream stream(file);
//            stream.setByteOrder(QDataStream::LittleEndian); // 根据实际情况设置字节序
//            if (stream.readRawData(reinterpret_cast<char*>(&Ver_value), 4) != 4) {
//                qDebug() << "读取数据失败";
//            }
//            else
//            {
//                qDebug()<< "版本整数："<<Ver_value;
//                Version[0] = (quint8)(Ver_value >> 16);//大版本号
//                Version[1] = (quint8)(Ver_value >> 8);//中版本号
//                Version[2] = (quint8)Ver_value; //小版本号

//                QString SoftVer = QString("%1.%2.%3").arg(Version[0]).arg(Version[1]).arg(Version[2]);
//                ui->lineEdit_OtaVersion->setText(SoftVer);
//            }

//        }
//        file->seek(0);//文件指针返回开头处
    }
    else
    {
        QMessageBox::information(this,"Error Message", "File Open Error" + file->errorString());
        return;
    }
    QDataStream in(file);

    char * binByte = new char[binSize];

    in.setVersion (QDataStream::Qt_5_9);

    statusBar()->showMessage(tr("准备读取数据")); //用于获取或创建主窗口底部的状态栏（QStatusBar），用于向用户显示应用程序的实时状态、提示信息或永久性控件。
    //in.readRawData (binByte, binSize);      //读出文件到缓存
    qint32 ret = in.readRawData(binByte, binSize);
    qDebug("Read file ret: %d",ret);
    if(ret<0)
    {
        qDebug("Read file err");
        //return;
    }

    statusBar()->showMessage(tr("读取数据完毕"));

    if(isFileLoad)
    {
        isFileLoad = false;
        delete[] MybinByte;
    }

    tempByte = new QByteArray(binByte, binSize);                //格式转换
    MybinByte = new quint8[binSize];
    isFileLoad = true;
    for(quint32 i = 0;i<binSize;i++)
    {
        MybinByte[i] = (quint8)binByte[i];
        //qDebug("%02X",MybinByte[i]);
    }

//    Bin_checksum = crc32_compute(MybinByte, binSize, NULL);//整个bin文件计算校验值，要验证
    qDebug() <<"binSize = " << binSize;
//    qDebug()<< "Bin_checksum is " << Bin_checksum;

//    QString *tempStr = new QString(tempByte->toHex().toUpper());
//    //显示文件内容
//    qint16 cnt = 1;
//    qint32 kcnt = 0;
//    for(qint64 i = 2; i < tempStr->size();)
//    {
//        tempStr->insert (i, ' ');//每个字节之间空一格
//        i += 3;
//        cnt++;
//        if(cnt == 8)//每8个字节空2格
//        {
//            tempStr->insert (i, ' ');
//            i += 1;
//        }
//        if(cnt == 16)//每16个字节空一格
//        {
//            cnt = 1;
//            kcnt ++;
//            if(kcnt == 64)//每64行，即1K数据，空一行
//            {
//                kcnt = 0;
//                tempStr->insert (i, '\n');
//                i++;
//            }
//            tempStr->insert (i, '\n');
//            i += 3;         //避免换行后开头一个空格,换行相当于从新插入
//        }

//    }
//    statusBar()->showMessage(tr("准备显示"));
//    ui->textEditSend->insertPlainText (*tempStr);
//    statusBar()->showMessage(tr("显示完毕"));

//    timer->start(1000);
//    serial->write(binByte,25);

    delete tempByte;
    delete[] binByte;
//    delete tempStr;

    file->close ();
    delete file;
    //delete pcsfileInfo;
}

void MainWindow::on_pushButtonUpgrate_clicked()
{
    if(0 == binSize)
    {
        QMessageBox::information(this,"Error Message", "Please Select a bin File");
        return;
    }
    quint8 cmd[6] = {0x5A,0xFE,0x00,0x00,0xC4,0xA5}; //发送启动升级命令
//    QString start = "\n[Info] ********* start update ************\n";
    ui->pushButtonOpenFile->setEnabled(false);
//    ui->checkBoxHexReceive->setChecked(false);//接收框修改成接收字符串
//    ui->recData->insertPlainText (start);

    package_ID = 0;
    Resend_cnt = 0;
    isUpdating = true;

    ui->pushButtonUpgrate->setEnabled(false);
    ui->pushButtonStopUpgrade->setEnabled(true);
    setAllRecFlagsFalse();
    m_serialPort->write((char*)&cmd,6);
}

void MainWindow::on_pushButtonStopUpgrade_clicked()
{
    quint8 cmd[6] = {0x5A,0xEE,0x00,0xE0,0xC7,0xA5}; //发送停止升级命令
    isUpdating = false;
    Resend_cnt = 0;
    package_ID = 0;

//    rec_timer->stop();

    ui->pushButtonOpenFile->setEnabled(true);
    ui->pushButtonUpgrate->setEnabled(true);
    ui->pushButtonStopUpgrade->setEnabled(false);
    ui->progressBar->setValue(0);

    setAllRecFlagsFalse();
    m_serialPort->write((char*)&cmd,6);
    qDebug("stop upgrade.");
//    timer->stop();

}

void MainWindow::on_pushButton_MbReadAddr_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x11, 0x00, 0x00, 0x01, 0x94, 0xBD};

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadAddrCheck = true;
}

void MainWindow::on_pushButton_ClearMbInfo_clicked()
{
    ui->textEdit_MbInfo->setText(NULL);
}

void MainWindow::on_pushButton_MbRemoteOpen_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x02, 0x5A, 0x5A, 0x98, 0xB3};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isSendRemoteOpenCmd = true;
}

void MainWindow::on_pushButton_CloseLeakProtect_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0xA0, 0x16, 0x00, 0x01, 0x8A, 0x2C};

//    quint16 tmp = modbus_rtu_crc16(cmd,6);
//    quint8 tmp_low  = (quint8)(tmp & 0xff);
//    quint8 tmp_high = (quint8)(tmp>>8 & 0xff);
//    qDebug("CRC: %X",tmp);
//    qDebug("CRC_LOW =  %X",tmp_low);
//    qDebug("CRC_HIGH =  %X",tmp_high);

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isSendCloseLeakCmd = true;
}

void MainWindow::on_pushButton_EnableLeakProtect_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0xA0, 0x16, 0x00, 0x00, 0x4B, 0xEC};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isSendEnableLeakCmd = true;
}

void MainWindow::on_pushButton_MbReadUa_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x48, 0x00, 0x01, 0x05, 0xFE};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadUaCmd = true;
}

void MainWindow::on_pushButton_MbReadIa_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x40, 0x00, 0x02, 0xC4, 0x3D};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadIaCmd = true;
}

void MainWindow::on_pushButton_MbReadFreq_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x4F, 0x00, 0x01, 0xB4, 0x3F};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadFreqCmd = true;
}

void MainWindow::on_pushButton_MbReadPa_clicked() //A相有功功率
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x51, 0x00, 0x02, 0x94, 0x38};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadPaCmd = true;
}

void MainWindow::on_pushButton_MbReadComboAEAll_clicked() //总有功电能
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x59, 0x00, 0x02, 0x15, 0xFA};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadComboActECmd = true;
}

void MainWindow::on_pushButton_MbReadPosAEAll_clicked() //正有功电能
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x72, 0x00, 0x02, 0x65, 0xF2};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadPosActECmd = true;
}

void MainWindow::on_pushButton_MbReadNegAEAll_clicked() //负有功电能
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x74, 0x00, 0x02, 0x85, 0xF3};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadNegActECmd = true;
}

void MainWindow::on_pushButton_MbReadPFa_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x6F, 0x00, 0x01, 0xB5, 0xF5};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadPFaCmd = true;
}

void MainWindow::on_pushButton_ReadLcdDisplayInterval_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0xA1, 0x08, 0x00, 0x01, 0x27, 0xD6};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isReadLcdDisplayIntervalTimeCmd = true;
}

quint32 IntervalTime = 0; //间隔时间
//quint8 func_choose[8] = {0};
//quint8 func_choose_All0[8] = {0};
quint8 func_choose[14] = {0};
quint8 func_choose_All0[14] = {0};
quint8 exe_num = 0;//需要执行的个数

//static quint8 pos_done[8] = {0};
static quint8 pos_done[14] = {0};
static quint8 exe_num_tmp = 0;

void MainWindow::on_pushButton_AutoFreshStart_clicked()
{   
    //检查输入的间隔时间数字是否合理
    QString text = ui->lineEdit_UpdateIntervalTime->text();
    QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
    QRegularExpressionMatch match = re.match(text);
    if (match.hasMatch()) {
        bool ok;
        quint32 number = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
        if (ok && number >= 200 && number <= 0xffffffff) {
            qDebug() << "数字在范围内";
            IntervalTime = number;
        } else {
            qDebug() << "数字不在范围内或者转换失败";
            QMessageBox::critical(this,"刷新时间设置", "设置的时间数字不在范围内");
            return;
        }
    } else {
        qDebug() << "文本不全是数字";
        QMessageBox::critical(this,"刷新时间设置", "设置的时间数字异常");
        return;
    }


//    check_status = 0;
    memset(func_choose,0,sizeof(func_choose));
    exe_num = 0;
    //检查是否勾选至少一个命令
    if(ui->checkBox_ReadUa->isChecked())
    {
//        check_status |= 0x01;
        func_choose[0] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadIa->isChecked())
    {
//        check_status |= 0x02;
        func_choose[1] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadFreq->isChecked())
    {
//        check_status |= 0x04;
        func_choose[2] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadPa->isChecked())
    {
//        check_status |= 0x08;
        func_choose[3] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadEa->isChecked())
    {
//        check_status |= 0x10;
        func_choose[4] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadAEa->isChecked())
    {
//        check_status |= 0x20;
        func_choose[5] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadNEa->isChecked())
    {
//        check_status |= 0x40;
        func_choose[6] = 1;
        exe_num++;
    }
    if(ui->checkBox_ReadPF->isChecked())
    {
//        check_status |= 0x80;
        func_choose[7] = 1;
        exe_num++;
    }
    //后续待添加
    if(ui->checkBox_MbReadComboNEAll->isChecked())
    {
        func_choose[8] = 1;
        exe_num++;
    }
    if(ui->checkBox_MbReadAppEngA->isChecked())
    {
        func_choose[9] = 1;
        exe_num++;
    }
    if(ui->checkBox_MbReadNPa->isChecked())
    {
        func_choose[10] = 1;
        exe_num++;
    }
    if(ui->checkBox_MbReadAppPa->isChecked())
    {
        func_choose[11] = 1;
        exe_num++;
    }
    if(ui->checkBox_MbReadPosRectE->isChecked())
    {
        func_choose[12] = 1;
        exe_num++;
    }
    if(ui->checkBox_MbReadNegRectE->isChecked())
    {
        func_choose[13] = 1;
        exe_num++;
    }


    if(0 != memcmp(func_choose, func_choose_All0, sizeof(func_choose))) //已勾选至少一条命令
    {
        ui->pushButton_AutoFreshStart->setEnabled(false);
        ui->pushButton_AutoFreshStop->setEnabled(true);

        ui->checkBox_ReadUa->setEnabled(false);
        ui->checkBox_ReadIa->setEnabled(false);
        ui->checkBox_ReadFreq->setEnabled(false);
        ui->checkBox_ReadPa->setEnabled(false);
        ui->checkBox_ReadEa->setEnabled(false);
        ui->checkBox_ReadAEa->setEnabled(false);
        ui->checkBox_ReadNEa->setEnabled(false);
        ui->checkBox_ReadPF->setEnabled(false);

        ui->checkBox_MbReadComboNEAll->setEnabled(false);
        ui->checkBox_MbReadAppEngA->setEnabled(false);
        ui->checkBox_MbReadNPa->setEnabled(false);
        ui->checkBox_MbReadAppPa->setEnabled(false);
        ui->checkBox_MbReadPosRectE->setEnabled(false);
        ui->checkBox_MbReadNegRectE->setEnabled(false);

        exe_num_tmp = exe_num;

        AutoFresh_timer->start(IntervalTime);
    }
    else
    {
        QMessageBox::critical(this,"刷新时间设置", "请勾选至少一条命令");
        return;
    }

}



void MainWindow::timerAutoFreshProcess()
{
    AutoFresh_timer->stop();

//    static quint8 pos_done[8] = {0};
//    static quint8 exe_num_tmp = exe_num;

//    qDebug("exe_num_tmp = %d,func_choose[0] = %d, pos_done[0] = %d ",exe_num_tmp,func_choose[0],pos_done[0]);
    if(ui->checkBox_ReadUa->isChecked() && func_choose[0] && 0==pos_done[0])
    {
        on_pushButton_MbReadUa_clicked();
        pos_done[0] = 1;
        exe_num_tmp--;
        qDebug("step 1 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadIa->isChecked() && func_choose[1] && 0==pos_done[1])
    {
        on_pushButton_MbReadIa_clicked();
        pos_done[1] = 1;
        exe_num_tmp--;
        qDebug("step 2 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadFreq->isChecked() && func_choose[2] && 0==pos_done[2])
    {
        on_pushButton_MbReadFreq_clicked();
        pos_done[2] = 1;
        exe_num_tmp--;
        qDebug("step 3 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadPa->isChecked() && func_choose[3] && 0==pos_done[3])
    {
        on_pushButton_MbReadPa_clicked();
        pos_done[3] = 1;
        exe_num_tmp--;
        qDebug("step 4 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadEa->isChecked() && func_choose[4] && 0==pos_done[4])
    {
        on_pushButton_MbReadComboAEAll_clicked();
        pos_done[4] = 1;
        exe_num_tmp--;
        qDebug("step 5 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadAEa->isChecked() && func_choose[5] && 0==pos_done[5])
    {
        on_pushButton_MbReadPosAEAll_clicked();
        pos_done[5] = 1;
        exe_num_tmp--;
        qDebug("step 6 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadNEa->isChecked() && func_choose[6] && 0==pos_done[6])
    {
        on_pushButton_MbReadNegAEAll_clicked();
        pos_done[6] = 1;
        exe_num_tmp--;
        qDebug("step 7 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_ReadPF->isChecked() && func_choose[7] && 0==pos_done[7])
    {
        on_pushButton_MbReadPFa_clicked();
        pos_done[7] = 1;
        exe_num_tmp--;
        qDebug("step 8 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadComboNEAll->isChecked() && func_choose[8] && 0==pos_done[8])
    {
        on_pushButton_MbReadComboNEAll_clicked();
        pos_done[8] = 1;
        exe_num_tmp--;
        qDebug("step 9 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadAppEngA->isChecked() && func_choose[9] && 0==pos_done[9])
    {
        on_pushButton_MbReadAppEngA_clicked();
        pos_done[9] = 1;
        exe_num_tmp--;
        qDebug("step 10 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadNPa->isChecked() && func_choose[10] && 0==pos_done[10])
    {
        on_pushButton_MbReadNPa_clicked();
        pos_done[10] = 1;
        exe_num_tmp--;
        qDebug("step 11 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadAppPa->isChecked() && func_choose[11] && 0==pos_done[11])
    {
        on_pushButton_MbReadAppPa_clicked();
        pos_done[11] = 1;
        exe_num_tmp--;
        qDebug("step 12 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadPosRectE->isChecked() && func_choose[12] && 0==pos_done[12])
    {
        on_pushButton_MbReadPosRectE_clicked();
        pos_done[12] = 1;
        exe_num_tmp--;
        qDebug("step 13 executed,exe_num_tmp = %d",exe_num_tmp);
    }
    else if(ui->checkBox_MbReadNegRectE->isChecked() && func_choose[13] && 0==pos_done[13])
    {
        on_pushButton_MbReadNegRectE_clicked();
        pos_done[13] = 1;
        exe_num_tmp--;
        qDebug("step 14 executed,exe_num_tmp = %d",exe_num_tmp);
    }

    if(0 == exe_num_tmp)
    {
        exe_num_tmp = exe_num;
        memset(pos_done,0,sizeof(pos_done));
    }


    AutoFresh_timer->start(IntervalTime);
}

void MainWindow::timerRealTimeProcess()
{
    QString CurrentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
//    ui->label_RealTime->setText(CurrentTime);
    ui->label_RealTime->setText(QString("<span style='color:red;'>%1</span>").arg(CurrentTime));
}

void MainWindow::on_pushButton_AutoFreshStop_clicked()
{
    AutoFresh_timer->stop();

    memset(pos_done,0,sizeof(pos_done));

    ui->pushButton_AutoFreshStart->setEnabled(true);
    ui->pushButton_AutoFreshStop->setEnabled(false);

    ui->checkBox_ReadUa->setEnabled(true);
    ui->checkBox_ReadIa->setEnabled(true);
    ui->checkBox_ReadFreq->setEnabled(true);
    ui->checkBox_ReadPa->setEnabled(true);
    ui->checkBox_ReadEa->setEnabled(true);
    ui->checkBox_ReadAEa->setEnabled(true);
    ui->checkBox_ReadNEa->setEnabled(true);
    ui->checkBox_ReadPF->setEnabled(true);

    ui->checkBox_MbReadComboNEAll->setEnabled(true);
    ui->checkBox_MbReadAppEngA->setEnabled(true);
    ui->checkBox_MbReadNPa->setEnabled(true);
    ui->checkBox_MbReadAppPa->setEnabled(true);
    ui->checkBox_MbReadPosRectE->setEnabled(true);
    ui->checkBox_MbReadNegRectE->setEnabled(true);
}

void MainWindow::on_pushButton_MbReadOVThres_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x11, 0x07, 0x00, 0x01, 0x31, 0x15};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isReadOVThresCmd = true;
}

void MainWindow::on_pushButton_MbReadOVTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x11, 0x08, 0x00, 0x01, 0x01, 0x16};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadOVTimeCmd = true;
}

void MainWindow::on_pushButton_WriteLcdIntervalTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0xA1, 0x08, 0x00, 0x03, 0x6A, 0x17};

    //检查输入的间隔时间数字是否合理
    QString text = ui->lineEdit_WriteLcdIntervalTime->text();
    QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
    QRegularExpressionMatch match = re.match(text);
    quint32 number;
    if (match.hasMatch()) {
        bool ok;
        number = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
        if (ok && number >= 2 && number <= 20) {
            qDebug() << "数字在范围内";
            IntervalTime = number;
        } else {
            qDebug() << "数字不在范围内或者转换失败";
            QMessageBox::critical(this,"刷新时间设置", "设置的数字不在范围内");
            return;
        }
    } else {
        qDebug() << "文本不全是数字";
        QMessageBox::critical(this,"刷新时间设置", "设置的数字异常");
        return;
    }

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新设置值
        cmd[5] = number;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }
    else
    {
        //更新设置值
        cmd[5] = number;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isWriteLcdIntervalTimeCmd = true;
}

void MainWindow::on_pushButton_MbReadSysTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x10, 0x00, 0x00, 0x03, 0x00, 0xE9};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadSysTimeCmd = true;
}

void MainWindow::on_pushButton_MbGetWinTime2Set_clicked()
{
    QDateTime dateTime = QDateTime::currentDateTime(); // 获取当前系统日期和时间
    QString currentTime = dateTime.toString("yyyy-MM-dd HH:mm:ss");
    ui->lineEdit_MbGetWinTime2Set->setText(currentTime);

    quint8 cmd[15] = {0x03, 0x10, 0x10, 0x00, 0x00, 0x03, 0x06, 0x1A, 0x04, 0x1B, 0x08,0x08, 0x08, 0xC7, 0xD8};

    cmd[7] = dateTime.date().year() - 2000;
    cmd[8] = dateTime.date().month();
    cmd[9] = dateTime.date().day();
    cmd[10] = dateTime.time().hour();
    cmd[11] = dateTime.time().minute();
    cmd[12] = dateTime.time().second();

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,13);
    cmd[13] = (quint8)(crc_new>>8 & 0xff);
    cmd[14] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,15);
    m_serialPort->write((char*)&cmd,15);
    setAllRecFlagsFalse();
    isMbGetWinTime2SetCmd = true;
}

void MainWindow::on_pushButton_MbManuTimeSet_clicked()
{
    quint8 cmd[15] = {0x03, 0x10, 0x10, 0x00, 0x00, 0x03, 0x06, 0x1A, 0x04, 0x1B, 0x08,0x08, 0x08, 0xC7, 0xD8};
    //手动输入的时间数字有效性检查
    quint8 DI_tmp[2] = {0};
    //检查输入的时间数据有效性
    //年
    QString newInput_year = ui->lineEdit_MbInputYear->text();//“年”数据检查
    quint8 TimeData_length = newInput_year.length();
    qDebug()<<"newInput_year_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'年'数据不能为空");
        return;
    }
    else if(2 != TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据长度必须为2位数（年的后两位）");
        return;
    }
    else if(!isNumOnlyString(newInput_year))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(newInput_year.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(newInput_year.at(1).toLatin1());
    cmd[7] = DI_tmp[0]*10+DI_tmp[1];//时间数据
    if(cmd[7]>99)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据错误");
        return;
    }
    qDebug() << "Year:" << cmd[7];
    //月
    QString newInput_month = ui->lineEdit_MbInputMonth->text();
    TimeData_length = newInput_month.length();
    qDebug()<<"newInput_month_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'月'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_month))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        cmd[8] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_month.at(1).toLatin1());
        if((DI_tmp[0]*10+DI_tmp[1])>12)
        {
            QMessageBox::critical(this,"TimeSet Message", "输入'月'数据不能超过12");
            return;
        }

        cmd[8] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[8]>12 || cmd[8]<1)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据错误");
        return;
    }
    qDebug() << "Month:" << cmd[8];
    //日
    QString newInput_day = ui->lineEdit_MbInputDay->text();
    TimeData_length = newInput_day.length();
    qDebug()<<"newInput_day_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'日'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_day))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        cmd[9] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_day.at(1).toLatin1());

        cmd[9] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[9]>31 || cmd[9]<1)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'日'数据错误");
        return;
    }
    qDebug() << "Day:" << cmd[9];
    //时
    QString newInput_hour = ui->lineEdit_MbInputHour->text();
    TimeData_length = newInput_hour.length();
    qDebug()<<"newInput_hour_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'时'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_hour))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        cmd[10] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_hour.at(1).toLatin1());
        cmd[10] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[10]>23)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据错误");
        return;
    }
    qDebug() << "Hour:" << cmd[10];
    //分
    QString newInput_min = ui->lineEdit_MbInputMin->text();
    TimeData_length = newInput_min.length();
    qDebug()<<"newInput_min_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'分'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_min))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        cmd[11] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_min.at(1).toLatin1());
        cmd[11] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[11]>59)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据错误");
        return;
    }
    qDebug() << "Min:" << cmd[11];
    //秒
    QString newInput_sec = ui->lineEdit_MbInputSec->text();
    TimeData_length = newInput_sec.length();
    qDebug()<<"newInput_sec_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'秒'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_sec))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        cmd[12] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_sec.at(1).toLatin1());
        cmd[12] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[12]>59)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据错误");
        return;
    }
    qDebug() << "Sec:" << cmd[12];
//----------------------------------------------------------------------
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,13);
    cmd[13] = (quint8)(crc_new>>8 & 0xff);
    cmd[14] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,15);
    m_serialPort->write((char*)&cmd,15);
    setAllRecFlagsFalse();
    isMbManuTimeSetCmd = true;

}

void MainWindow::on_pushButton_MbSetOVThres_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0x11, 0x07, 0x01, 0x09, 0xFD, 0x43};

    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbSetOVThres->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(3 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为3位数且范围：[234V,286V]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    quint16 tmpp = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];

    if(tmpp>286 || tmpp<234)
    {
        QMessageBox::critical(this,"Message", "过压数值不在规定的范围内[234V,286V]");
        return;
    }

//    qDebug("Input OV = %d",tmpp);
    cmd[4] = (quint8)(tmpp>>8);
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSetOVThresCmd = true;
}

void MainWindow::on_pushButton_MbSetOVTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0x11, 0x08, 0x00, 0x05, 0xCC, 0xD5};

    quint8 DI_tmp[2] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbSetOVTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVTimeData_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为2位数以内且范围：[2,20]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    quint16 tmpp = 0;
    if(1 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        tmpp = DI_tmp[0];
    }
    else
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpp = DI_tmp[0]*10+DI_tmp[1];
    }
    qDebug("Input OVTime = %d",tmpp);
    if(tmpp>20 || tmpp<2)
    {
        QMessageBox::critical(this,"Message", "过压时间不在规定的范围内[2,20]");
        return;
    }

    cmd[4] = (quint8)(tmpp>>8);
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSetOVTimeCmd = true;
}

void MainWindow::on_pushButton_MbWriteAddr_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0x11, 0x00, 0x00, 0x05, 0x59, 0x7E};

    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWriteAddr->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"MbAddr_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>3)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为3位数以内且范围：[1,247]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    quint16 tmpp = 0;
    if(1 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        tmpp = DI_tmp[0];
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpp = DI_tmp[0]*10+DI_tmp[1];
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        tmpp = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
    }

    qDebug("Input OVTime = %d",tmpp);
    if(tmpp>247 || tmpp<1)
    {
        QMessageBox::critical(this,"Message", "通信地址不在规定的范围内[1,247]");
        return;
    }

    cmd[4] = (quint8)(tmpp>>8);
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbWriteAddrCmd = true;
}

void MainWindow::on_pushButton_MbReadSoftVersion_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0xA1, 0x06, 0x00, 0x02, 0x12, 0x7D}; //通配读版本

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadSoftVerCmd = true;
}

void MainWindow::on_pushButton_MbReadSN_PCODE_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0xA1, 0x00, 0x00, 0x06, 0xF3, 0xBF}; //通配读SN+PCODE

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadSNPCodeCmd = true;
}

void MainWindow::on_pushButton_MbWriteSN_PCODE_clicked()
{
    quint8 cmd[21] = {0xFA, 0x10, 0xA1, 0x00, 0x00, 0x06, 0x0C, 0x01,0x04,0x0D,0x9B,0x26,0x04,0x27,0x00,0x00,0x01,0x12,0x02,0x4D,0xEE}; //通配写SN+PCODE

    quint8 DI_tmp[2] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWriteSN_PCODE->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"MbAddr_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(24 != TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据应为连续的24字符");
        return;
    }
    else{}

    if(!isValidString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    cmd[7] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(2).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(3).toLatin1());
    cmd[8] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(4).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(5).toLatin1());
    cmd[9] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(6).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(7).toLatin1());
    cmd[10] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(8).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(9).toLatin1());
    cmd[11] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(10).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(11).toLatin1());
    cmd[12] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(12).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(13).toLatin1());
    cmd[13] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(14).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(15).toLatin1());
    cmd[14] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(16).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(17).toLatin1());
    cmd[15] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(18).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(19).toLatin1());
    cmd[16] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(20).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(21).toLatin1());
    cmd[17] = DI_tmp[0]*16+DI_tmp[1];

    DI_tmp[0] = convertCharToHex(new_TimeData.at(22).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(23).toLatin1());
    cmd[18] = DI_tmp[0]*16+DI_tmp[1];

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,19);
    cmd[19] = (quint8)(crc_new>>8 & 0xff);
    cmd[20] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,21);
    m_serialPort->write((char*)&cmd,21);
    setAllRecFlagsFalse();
    isMbWriteSNPcodeCmd = true;
}

void MainWindow::on_pushButton_MbSoftReset_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x2D, 0x1A}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSoftResetCmd = true;
}

void MainWindow::on_pushButton_MbClearAllEnergy_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0x20, 0x01, 0x5A, 0x5A, 0x7C, 0xDA}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbClearAllEnergyCmd = true;
}

void MainWindow::on_pushButton_MbStartCalibration_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x00, 0x00, 0x01, 0x7F, 0x81}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbStartCaliCmd = true;
}

void MainWindow::on_pushButton_MbExitCalibration_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x00, 0x00, 0x00, 0xBE, 0x41}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbExitCaliCmd = true;
}

void MainWindow::on_pushButton_MbCali_Ia_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x01, 0x00, 0x0A, 0x6F, 0x86}; //通配

    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbCali_Ia->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"Ia Cali_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "校准电流数据不能为空");
        return;
    }
    else if(TimeData_length>3)
    {
        QMessageBox::critical(this,"Message", "校准电流数据长度应为3位数以内");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "校准电流数据只能包含数字");
        return;
    }else{}

    quint16 tmpCali=0;
    if(1 == TimeData_length)
    {
        tmpCali = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpCali = DI_tmp[0]*10+DI_tmp[1];
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        tmpCali = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
    }

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    quint16 tt = (quint16)(cmd[4]<<8) + cmd[5];
    if(tmpCali != 10)
    {
        //更新校准值
        cmd[4] = (quint8)(tmpCali>>8);
        cmd[5] = (quint8)(tmpCali & 0xff);
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbCaliIaCmd = true;
}

void MainWindow::on_pushButton_MbCali_Ua_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x04, 0x00, 0xDC, 0xFE, 0x19}; //通配
    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbCali_Ua->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"Ua Cali_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "校准电压数据不能为空");
        return;
    }
    else if(TimeData_length>3)
    {
        QMessageBox::critical(this,"Message", "校准电压数据长度应为3位数以内");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "校准电压数据只能包含数字");
        return;
    }else{}

    quint16 tmpCali=0;
    if(1 == TimeData_length)
    {
        tmpCali = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpCali = DI_tmp[0]*10+DI_tmp[1];
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        tmpCali = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
    }
    if(tmpCali != 220)
    {
        //更新校准值
        cmd[4] = (quint8)(tmpCali>>8);
        cmd[5] = (quint8)(tmpCali & 0xff);
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbCaliUaCmd = true;
}

void MainWindow::on_pushButton_MbCali_Pa_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x07, 0x04, 0x4C, 0x0C, 0xB5}; //通配
    quint8 DI_tmp[5] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbCali_Pa->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"Pa Cali_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "校准功率数据不能为空");
        return;
    }
    else if(TimeData_length>5 || TimeData_length<3)
    {
        QMessageBox::critical(this,"Message", "校准功率数据长度应为3-5位数");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "校准功率数据只能包含数字");
        return;
    }else{}

    quint32 tmpCali=0;
    if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        tmpCali = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        tmpCali = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        tmpCali = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];

        if(tmpCali>65535)
        {
            QMessageBox::critical(this,"Message", "输入校准功率数据超过0xffff");
            return;
        }
    }
    if(tmpCali != 1100)
    {
        //更新校准值
        cmd[4] = (quint8)(tmpCali>>8);
        cmd[5] = (quint8)(tmpCali & 0xff);
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbCaliPaCmd = true;
}

void MainWindow::on_pushButton_MbRs485ParaSet_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x11, 0x01, 0x00, 0x02, 0x04, 0x00, 0x03, 0x00, 0x00, 0x24, 0x14 };

    if(ui->comboBox_MbBaudrateChoose->currentText() == "115200")
        cmd[8] = 5;
    else if(ui->comboBox_MbBaudrateChoose->currentText() == "19200")
        cmd[8] = 4;
    else if(ui->comboBox_MbBaudrateChoose->currentText() == "9600")
        cmd[8] = 3;
    else if(ui->comboBox_MbBaudrateChoose->currentText() == "4800")
        cmd[8] = 2;
    else if(ui->comboBox_MbBaudrateChoose->currentText() == "2400")
        cmd[8] = 1;
    else if(ui->comboBox_MbBaudrateChoose->currentText() == "1200")
        cmd[8] = 0;
    else
        cmd[8] = 3;//默认9600

    if(ui->comboBox_MbCheckBitChoose->currentText() == "None")
        cmd[10] = 0;
    else if(ui->comboBox_MbCheckBitChoose->currentText() == "Odd")
        cmd[10] = 1;
    else if(ui->comboBox_MbCheckBitChoose->currentText() == "Even")
        cmd[10] = 2;
    else
        cmd[10] = 0;//默认无

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbRs485ParaSetCmd = true;
}

void MainWindow::on_pushButton_MbCheckProtectStatus_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x11, 0x0C, 0x00, 0x01, 0x54, 0xBE}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbCheckProtectBitCmd = true;
}

void MainWindow::on_pushButton_MbWriteProtectBit_clicked()
{
    quint8 cmd[8] = {0xFA, 0x06, 0x11, 0x0C, 0x00, 0x08, 0x58, 0xB8}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    quint8 tt = 0;
    if(ui->checkBox_MbOVProtectBit2->isChecked())
        tt |= 0x01;

    if(ui->checkBox_MbUVProtectBit2->isChecked())
        tt |= 0x02;

    if(ui->checkBox_MbOLProtectBit2->isChecked())
        tt |= 0x08;

    cmd[5] = tt;

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbWriteProtectBitCmd = true;
}

void MainWindow::on_pushButton_MbReadTemp_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x01, 0x00, 0x00, 0x01, 0x90, 0x7D}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadTempCmd = true;
}

void MainWindow::on_pushButton_MbReadSoftTime_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x00, 0x38, 0x00, 0x03, 0x91, 0x8D}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadSoftTimeCmd = true;
}

void MainWindow::on_pushButton_MbReadLife_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x08, 0x00, 0x00, 0x02, 0xD3, 0xE0}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadLifeCmd = true;
}

void MainWindow::on_pushButton_MbReadLeakLife_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x08, 0x02, 0x00, 0x02, 0x72, 0x20}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadLeakLifeCmd = true;
}

void MainWindow::on_pushButton_MbReadEventRecord_clicked()
{
    bool ok;
    QString ID_string = ui->comboBox_MbEventRecord->currentText();
    QVariant ID_Num(ID_string);
    quint8 EventID = ID_Num.toInt(&ok);

    quint8 cmd[8] = {0xFA, 0x03, 0x02, 0x00, 0x00, 0x0F, 0x11, 0xFD}; //通配

    cmd[3] = (EventID-1)*16+cmd[3];
    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadEventRecordCmd = true;

}

void MainWindow::on_pushButton_MbReadPosAE_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x12, 0x00, 0x00, 0x02, 0xD4, 0xF8}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadPosAEbaseCmd = true;
}

void MainWindow::on_pushButton_MbReadNegAE_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x12, 0x02, 0x00, 0x02, 0x75, 0x38}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadNegAEbaseCmd = true;
}

void MainWindow::on_pushButton_MbReadPosRE_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x12, 0x04, 0x00, 0x02, 0x95, 0x39}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadPosREbaseCmd = true;
}

void MainWindow::on_pushButton_MbReadNegRE_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x12, 0x06, 0x00, 0x02, 0x34, 0xF9}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadNegREbaseCmd = true;
}

void MainWindow::on_pushButton_MbReadAppEnergy_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x12, 0x08, 0x00, 0x02, 0x55, 0x3A}; //通配

//    quint8 cmd[8] = {0x03, 0x06, 0x20, 0x00, 0x5A, 0x5A, 0x39, 0x73};
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//        //更新CRC
//        quint16 crc_new = modbus_rtu_crc16(cmd,6);
//        cmd[6] = (quint8)(crc_new>>8 & 0xff);
//        cmd[7] = (quint8)(crc_new & 0xff);
//    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadAppEnergybaseCmd = true;
}

void MainWindow::on_pushButton_MbWritePosAE_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x12, 0x00, 0x00, 0x02, 0x04, 0x00, 0x00,0x00,0xFF,0x41,0x68}; //通配

    quint64 Value = 0;
    quint8 DI_tmp[10] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWritePosAE->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"PosAE_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>10)
    {
        QMessageBox::critical(this,"Message", "输入数据长度超过限制");
        return;
    }
    else{}

    if(!isNumOnlyString(new_TimeData))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    if(1 == TimeData_length)
    {
        cmd[10] = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        Value = DI_tmp[0]*10+DI_tmp[1];
        cmd[10] = Value;
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        Value = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        Value = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        Value = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(6 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        Value = DI_tmp[0]*100000+DI_tmp[1]*10000+DI_tmp[2]*1000+DI_tmp[3]*100+DI_tmp[4]*10+DI_tmp[5];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(7 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        Value = DI_tmp[0]*1000000+DI_tmp[1]*100000+DI_tmp[2]*10000+DI_tmp[3]*1000+DI_tmp[4]*100+DI_tmp[5]*10+DI_tmp[6];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(8 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        Value = DI_tmp[0]*10000000+DI_tmp[1]*1000000+DI_tmp[2]*100000+DI_tmp[3]*10000+DI_tmp[4]*1000+DI_tmp[5]*100+DI_tmp[6]*10+DI_tmp[7];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(9 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        Value = DI_tmp[0]*100000000+DI_tmp[1]*10000000+DI_tmp[2]*1000000+DI_tmp[3]*100000+DI_tmp[4]*10000+DI_tmp[5]*1000+DI_tmp[6]*100+DI_tmp[7]*10+DI_tmp[8];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(10 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        DI_tmp[9] = convertCharToHex(new_TimeData.at(9).toLatin1());
        Value = DI_tmp[0]*1000000000+DI_tmp[1]*100000000+DI_tmp[2]*10000000+DI_tmp[3]*1000000+DI_tmp[4]*100000+DI_tmp[5]*10000+DI_tmp[6]*1000+DI_tmp[7]*100+DI_tmp[8]*10+DI_tmp[9];

        if(Value>0xffffffff)
        {
            QMessageBox::critical(this,"Message", "输入数据超过最大限制");
            return;
        }
        else
        {
            cmd[7] = (quint8)(Value>>24 & 0xff);
            cmd[8] = (quint8)(Value>>16 & 0xff);
            cmd[9] = (quint8)(Value>>8 & 0xff);
            cmd[10] = (quint8)(Value & 0xff);
        }
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbWritePosAEnergyCmd = true;
}

void MainWindow::on_pushButton_MbWriteNegAE_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x12, 0x02, 0x00, 0x02, 0x04, 0x00, 0x00,0x00,0x78,0x80,0xD3}; //通配

    quint64 Value = 0;
    quint8 DI_tmp[10] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWriteNegAE->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"NegAE_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>10)
    {
        QMessageBox::critical(this,"Message", "输入数据长度超过限制");
        return;
    }
    else{}

    if(!isNumOnlyString(new_TimeData))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    if(1 == TimeData_length)
    {
        cmd[10] = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        Value = DI_tmp[0]*10+DI_tmp[1];
        cmd[10] = Value;
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        Value = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        Value = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        Value = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(6 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        Value = DI_tmp[0]*100000+DI_tmp[1]*10000+DI_tmp[2]*1000+DI_tmp[3]*100+DI_tmp[4]*10+DI_tmp[5];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(7 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        Value = DI_tmp[0]*1000000+DI_tmp[1]*100000+DI_tmp[2]*10000+DI_tmp[3]*1000+DI_tmp[4]*100+DI_tmp[5]*10+DI_tmp[6];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(8 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        Value = DI_tmp[0]*10000000+DI_tmp[1]*1000000+DI_tmp[2]*100000+DI_tmp[3]*10000+DI_tmp[4]*1000+DI_tmp[5]*100+DI_tmp[6]*10+DI_tmp[7];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(9 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        Value = DI_tmp[0]*100000000+DI_tmp[1]*10000000+DI_tmp[2]*1000000+DI_tmp[3]*100000+DI_tmp[4]*10000+DI_tmp[5]*1000+DI_tmp[6]*100+DI_tmp[7]*10+DI_tmp[8];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(10 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        DI_tmp[9] = convertCharToHex(new_TimeData.at(9).toLatin1());
        Value = DI_tmp[0]*1000000000+DI_tmp[1]*100000000+DI_tmp[2]*10000000+DI_tmp[3]*1000000+DI_tmp[4]*100000+DI_tmp[5]*10000+DI_tmp[6]*1000+DI_tmp[7]*100+DI_tmp[8]*10+DI_tmp[9];

        if(Value>0xffffffff)
        {
            QMessageBox::critical(this,"Message", "输入数据超过最大限制");
            return;
        }
        else
        {
            cmd[7] = (quint8)(Value>>24 & 0xff);
            cmd[8] = (quint8)(Value>>16 & 0xff);
            cmd[9] = (quint8)(Value>>8 & 0xff);
            cmd[10] = (quint8)(Value & 0xff);
        }
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbWriteNegAEnergyCmd = true;
}

void MainWindow::on_pushButton_MbWritePosRE_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x12, 0x04, 0x00, 0x02, 0x04, 0x00, 0x00,0x00,0xFF,0x40,0x9B}; //通配

    quint64 Value = 0;
    quint8 DI_tmp[10] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWritePosRE->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"PosRE_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>10)
    {
        QMessageBox::critical(this,"Message", "输入数据长度超过限制");
        return;
    }
    else{}

    if(!isNumOnlyString(new_TimeData))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    if(1 == TimeData_length)
    {
        cmd[10] = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        Value = DI_tmp[0]*10+DI_tmp[1];
        cmd[10] = Value;
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        Value = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        Value = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        Value = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(6 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        Value = DI_tmp[0]*100000+DI_tmp[1]*10000+DI_tmp[2]*1000+DI_tmp[3]*100+DI_tmp[4]*10+DI_tmp[5];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(7 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        Value = DI_tmp[0]*1000000+DI_tmp[1]*100000+DI_tmp[2]*10000+DI_tmp[3]*1000+DI_tmp[4]*100+DI_tmp[5]*10+DI_tmp[6];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(8 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        Value = DI_tmp[0]*10000000+DI_tmp[1]*1000000+DI_tmp[2]*100000+DI_tmp[3]*10000+DI_tmp[4]*1000+DI_tmp[5]*100+DI_tmp[6]*10+DI_tmp[7];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(9 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        Value = DI_tmp[0]*100000000+DI_tmp[1]*10000000+DI_tmp[2]*1000000+DI_tmp[3]*100000+DI_tmp[4]*10000+DI_tmp[5]*1000+DI_tmp[6]*100+DI_tmp[7]*10+DI_tmp[8];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(10 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        DI_tmp[9] = convertCharToHex(new_TimeData.at(9).toLatin1());
        Value = DI_tmp[0]*1000000000+DI_tmp[1]*100000000+DI_tmp[2]*10000000+DI_tmp[3]*1000000+DI_tmp[4]*100000+DI_tmp[5]*10000+DI_tmp[6]*1000+DI_tmp[7]*100+DI_tmp[8]*10+DI_tmp[9];

        if(Value>0xffffffff)
        {
            QMessageBox::critical(this,"Message", "输入数据超过最大限制");
            return;
        }
        else
        {
            cmd[7] = (quint8)(Value>>24 & 0xff);
            cmd[8] = (quint8)(Value>>16 & 0xff);
            cmd[9] = (quint8)(Value>>8 & 0xff);
            cmd[10] = (quint8)(Value & 0xff);
        }
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbWritePosREnergyCmd = true;
}

void MainWindow::on_pushButton_MbWriteNegRE_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x12, 0x06, 0x00, 0x02, 0x04, 0x00, 0x00,0x00,0xFF,0xC1,0x42}; //通配

    quint64 Value = 0;
    quint8 DI_tmp[10] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWriteNegRE->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"PosRE_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>10)
    {
        QMessageBox::critical(this,"Message", "输入数据长度超过限制");
        return;
    }
    else{}

    if(!isNumOnlyString(new_TimeData))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    if(1 == TimeData_length)
    {
        cmd[10] = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        Value = DI_tmp[0]*10+DI_tmp[1];
        cmd[10] = Value;
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        Value = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        Value = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        Value = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(6 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        Value = DI_tmp[0]*100000+DI_tmp[1]*10000+DI_tmp[2]*1000+DI_tmp[3]*100+DI_tmp[4]*10+DI_tmp[5];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(7 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        Value = DI_tmp[0]*1000000+DI_tmp[1]*100000+DI_tmp[2]*10000+DI_tmp[3]*1000+DI_tmp[4]*100+DI_tmp[5]*10+DI_tmp[6];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(8 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        Value = DI_tmp[0]*10000000+DI_tmp[1]*1000000+DI_tmp[2]*100000+DI_tmp[3]*10000+DI_tmp[4]*1000+DI_tmp[5]*100+DI_tmp[6]*10+DI_tmp[7];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(9 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        Value = DI_tmp[0]*100000000+DI_tmp[1]*10000000+DI_tmp[2]*1000000+DI_tmp[3]*100000+DI_tmp[4]*10000+DI_tmp[5]*1000+DI_tmp[6]*100+DI_tmp[7]*10+DI_tmp[8];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(10 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        DI_tmp[9] = convertCharToHex(new_TimeData.at(9).toLatin1());
        Value = DI_tmp[0]*1000000000+DI_tmp[1]*100000000+DI_tmp[2]*10000000+DI_tmp[3]*1000000+DI_tmp[4]*100000+DI_tmp[5]*10000+DI_tmp[6]*1000+DI_tmp[7]*100+DI_tmp[8]*10+DI_tmp[9];

        if(Value>0xffffffff)
        {
            QMessageBox::critical(this,"Message", "输入数据超过最大限制");
            return;
        }
        else
        {
            cmd[7] = (quint8)(Value>>24 & 0xff);
            cmd[8] = (quint8)(Value>>16 & 0xff);
            cmd[9] = (quint8)(Value>>8 & 0xff);
            cmd[10] = (quint8)(Value & 0xff);
        }
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbWriteNegREnergyCmd = true;
}

void MainWindow::on_pushButton_MbWriteAppEnergy_clicked()
{
    quint8 cmd[13] = {0xFA, 0x10, 0x12, 0x08, 0x00, 0x02, 0x04, 0x00, 0x00,0x00,0xFF,0x40,0xCE}; //通配

    quint64 Value = 0;
    quint8 DI_tmp[10] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbWriteAppEnergy->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"AppE_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>10)
    {
        QMessageBox::critical(this,"Message", "输入数据长度超过限制");
        return;
    }
    else{}

    if(!isNumOnlyString(new_TimeData))//检查是否有非数字字符
    {
        QMessageBox::critical(this,"Message", "输入数据含有非法字符");
        return;
    }

    if(1 == TimeData_length)
    {
        cmd[10] = convertCharToHex(new_TimeData.at(0).toLatin1());
    }
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        Value = DI_tmp[0]*10+DI_tmp[1];
        cmd[10] = Value;
    }
    else if(3 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        Value = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(4 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        Value = DI_tmp[0]*1000+DI_tmp[1]*100+DI_tmp[2]*10+DI_tmp[3];
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(5 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        Value = DI_tmp[0]*10000+DI_tmp[1]*1000+DI_tmp[2]*100+DI_tmp[3]*10+DI_tmp[4];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(6 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        Value = DI_tmp[0]*100000+DI_tmp[1]*10000+DI_tmp[2]*1000+DI_tmp[3]*100+DI_tmp[4]*10+DI_tmp[5];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(7 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        Value = DI_tmp[0]*1000000+DI_tmp[1]*100000+DI_tmp[2]*10000+DI_tmp[3]*1000+DI_tmp[4]*100+DI_tmp[5]*10+DI_tmp[6];
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(8 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        Value = DI_tmp[0]*10000000+DI_tmp[1]*1000000+DI_tmp[2]*100000+DI_tmp[3]*10000+DI_tmp[4]*1000+DI_tmp[5]*100+DI_tmp[6]*10+DI_tmp[7];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(9 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        Value = DI_tmp[0]*100000000+DI_tmp[1]*10000000+DI_tmp[2]*1000000+DI_tmp[3]*100000+DI_tmp[4]*10000+DI_tmp[5]*1000+DI_tmp[6]*100+DI_tmp[7]*10+DI_tmp[8];
        cmd[7] = (quint8)(Value>>24 & 0xff);
        cmd[8] = (quint8)(Value>>16 & 0xff);
        cmd[9] = (quint8)(Value>>8 & 0xff);
        cmd[10] = (quint8)(Value & 0xff);
    }
    else if(10 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
        DI_tmp[3] = convertCharToHex(new_TimeData.at(3).toLatin1());
        DI_tmp[4] = convertCharToHex(new_TimeData.at(4).toLatin1());
        DI_tmp[5] = convertCharToHex(new_TimeData.at(5).toLatin1());
        DI_tmp[6] = convertCharToHex(new_TimeData.at(6).toLatin1());
        DI_tmp[7] = convertCharToHex(new_TimeData.at(7).toLatin1());
        DI_tmp[8] = convertCharToHex(new_TimeData.at(8).toLatin1());
        DI_tmp[9] = convertCharToHex(new_TimeData.at(9).toLatin1());
        Value = DI_tmp[0]*1000000000+DI_tmp[1]*100000000+DI_tmp[2]*10000000+DI_tmp[3]*1000000+DI_tmp[4]*100000+DI_tmp[5]*10000+DI_tmp[6]*1000+DI_tmp[7]*100+DI_tmp[8]*10+DI_tmp[9];

        if(Value>0xffffffff)
        {
            QMessageBox::critical(this,"Message", "输入数据超过最大限制");
            return;
        }
        else
        {
            cmd[7] = (quint8)(Value>>24 & 0xff);
            cmd[8] = (quint8)(Value>>16 & 0xff);
            cmd[9] = (quint8)(Value>>8 & 0xff);
            cmd[10] = (quint8)(Value & 0xff);
        }
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbWriteAppEnergyCmd = true;
}

void MainWindow::on_pushButton_MbReadComboNEAll_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x5B, 0x00, 0x02, 0xB4, 0x3A};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadRectEnergyAllCmd = true;
}

void MainWindow::on_pushButton_MbReadAppEngA_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x5D, 0x00, 0x02, 0x54, 0x3B};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadAppEnergyAllCmd = true;
}

void MainWindow::on_pushButton_MbReadNPa_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x5F, 0x00, 0x02, 0xF5, 0xFB};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadRectPaCmd = true;
}

void MainWindow::on_pushButton_MbReadAppPa_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x67, 0x00, 0x02, 0x74, 0x36};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadAppPaCmd = true;
}

void MainWindow::on_pushButton_MbReadPosRectE_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x76, 0x00, 0x02, 0x24, 0x33};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadRectEnergyCmd = true;
}

void MainWindow::on_pushButton_MbReadNegRectE_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x00, 0x78, 0x00, 0x02, 0x45, 0xF0};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadNegRectEnergyCmd = true;
}

void MainWindow::on_pushButton_AddCrc16Data_clicked()
{
    QString m_strSendData = ui->lineEdit_AddCrc16RawData->text();

    if (m_strSendData.contains(" "))
    {
        m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
    }

    QByteArray sendBuf;

    convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

    const char* data = sendBuf.constData();
    quint8 len = sendBuf.size();

    quint16 crc_new = modbus_rtu_crc16((uint8_t*)data,len);
    quint8 CRC_H = (quint8)(crc_new>>8 & 0xff);
    quint8 CRC_L = (quint8)(crc_new & 0xff);

    quint8 New_data[len+2] = {0};
    memcpy(New_data,data,len);
    New_data[len] = CRC_H;
    New_data[len+1] = CRC_L;

//    displayHexArrayInWidget(New_data,len+2);

    QString hexString;
    for (int i = 0; i < len+2; ++i) {
        QString hex = QString::number(New_data[i], 16).toUpper();
        if (hex.length() < 2) {
            hex.prepend('0');
        }
        hexString += hex;
        if (i < (len+2 - 1)) {
            hexString += " ";
        }
    }

    ui->lineEdit_AddCrc16RawData->setText(hexString);

}

void MainWindow::on_pushButton_MBNewDataSend_clicked()
{
    QString m_strSendData = ui->lineEdit_AddCrc16RawData->text();

    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+m_strSendData;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(m_strSendData);
    }

    if (m_strSendData.contains(" "))
    {
        m_strSendData.replace(QString(" "),QString(""));    //把空格去掉
    }

    QByteArray sendBuf;

    convertStringToHex(m_strSendData, sendBuf);             //把QString 转换 为 hex

    m_serialPort->write(sendBuf);

}

void MainWindow::on_pushButton_MBCaliStatement_clicked()
{
    QMessageBox::about(NULL,"校准说明","请按以下步骤校准（确保校准时电压/电流/功率因数都是稳定的）\nStep1: 仅通电压AC220V，无电流（电流偏置校正），发命令进入校准模式；\nStep2: 发电压校准命令，等待4s进入下一步；\nStep3: 设置电源电流值（如10A），功率因数为0.5L，确保电源电流和功率因数数值已稳定后，发电流校准命令，等待3s；\nStep4:发功率相位校准命令，等待3s；\nStep5(常规校准可略过此步骤):设置电源电流值（1A），功率因数为1，确保电源电流和功率因数数值已稳定后，发有功功率偏置校准命令，等待3s；\nStep6: 发退出校准模式命令，校准结束。\n注：漏电保护为硬件漏电模块控制，不用校准。");
}

quint32 CycleTime1 = 0;
quint32 CycleTime2 = 0;
quint32 CycleTime3 = 0;
quint32 CycleTime4 = 0;
quint32 CycleTime5 = 0;
quint32 CycleTime6 = 0;
void MainWindow::timerCycleSend1Process()
{
    Cycle_timer1->stop();
    on_pushButtonSend1_clicked();

    if(ui->checkBox_DelaySend2->isChecked())
        Cycle_timer2->start(CycleTime2);
    else if(ui->checkBox_DelaySend3->isChecked())
        Cycle_timer3->start(CycleTime3);
    else if(ui->checkBox_DelaySend4->isChecked())
        Cycle_timer4->start(CycleTime4);
    else if(ui->checkBox_DelaySend5->isChecked())
        Cycle_timer5->start(CycleTime5);
    else if(ui->checkBox_DelaySend6->isChecked())
        Cycle_timer6->start(CycleTime6);
    else //只有本身选择
        Cycle_timer1->start(CycleTime1);
}

void MainWindow::timerCycleSend2Process()
{
    Cycle_timer2->stop();
    on_pushButtonSend2_clicked();

    if(ui->checkBox_DelaySend3->isChecked())
        Cycle_timer3->start(CycleTime3);
    else if(ui->checkBox_DelaySend4->isChecked())
        Cycle_timer4->start(CycleTime4);
    else if(ui->checkBox_DelaySend5->isChecked())
        Cycle_timer5->start(CycleTime5);
    else if(ui->checkBox_DelaySend6->isChecked())
        Cycle_timer6->start(CycleTime6);
    else if(ui->checkBox_DelaySend1->isChecked())
        Cycle_timer1->start(CycleTime1);
    else //只有本身选择
        Cycle_timer2->start(CycleTime2);
}

void MainWindow::timerCycleSend3Process()
{
    Cycle_timer3->stop();
    on_pushButtonSend3_clicked();

    if(ui->checkBox_DelaySend4->isChecked())
        Cycle_timer4->start(CycleTime4);
    else if(ui->checkBox_DelaySend5->isChecked())
        Cycle_timer5->start(CycleTime5);
    else if(ui->checkBox_DelaySend6->isChecked())
        Cycle_timer6->start(CycleTime6);
    else if(ui->checkBox_DelaySend1->isChecked())
        Cycle_timer1->start(CycleTime1);
    else if(ui->checkBox_DelaySend2->isChecked())
        Cycle_timer2->start(CycleTime2);
    else //只有本身选择
        Cycle_timer3->start(CycleTime3);
}

void MainWindow::timerCycleSend4Process()
{
    Cycle_timer4->stop();
    on_pushButtonSend4_clicked();

    if(ui->checkBox_DelaySend5->isChecked())
        Cycle_timer5->start(CycleTime5);
    else if(ui->checkBox_DelaySend6->isChecked())
        Cycle_timer6->start(CycleTime6);
    else if(ui->checkBox_DelaySend1->isChecked())
        Cycle_timer1->start(CycleTime1);
    else if(ui->checkBox_DelaySend2->isChecked())
        Cycle_timer2->start(CycleTime2);
    else if(ui->checkBox_DelaySend3->isChecked())
        Cycle_timer3->start(CycleTime3);
    else //只有本身选择
        Cycle_timer4->start(CycleTime4);
}

void MainWindow::timerCycleSend5Process()
{
    Cycle_timer5->stop();
    on_pushButtonSend5_clicked();

    if(ui->checkBox_DelaySend6->isChecked())
        Cycle_timer6->start(CycleTime6);
    else if(ui->checkBox_DelaySend1->isChecked())
        Cycle_timer1->start(CycleTime1);
    else if(ui->checkBox_DelaySend2->isChecked())
        Cycle_timer2->start(CycleTime2);
    else if(ui->checkBox_DelaySend3->isChecked())
        Cycle_timer3->start(CycleTime3);
    else if(ui->checkBox_DelaySend4->isChecked())
        Cycle_timer4->start(CycleTime4);
    else //只有本身选择
        Cycle_timer5->start(CycleTime5);
}

void MainWindow::timerCycleSend6Process()
{
    Cycle_timer6->stop();
    on_pushButtonSend6_clicked();

    if(ui->checkBox_DelaySend1->isChecked())
        Cycle_timer1->start(CycleTime1);
    else if(ui->checkBox_DelaySend2->isChecked())
        Cycle_timer2->start(CycleTime2);
    else if(ui->checkBox_DelaySend3->isChecked())
        Cycle_timer3->start(CycleTime3);
    else if(ui->checkBox_DelaySend4->isChecked())
        Cycle_timer4->start(CycleTime4);
    else if(ui->checkBox_DelaySend5->isChecked())
        Cycle_timer5->start(CycleTime5);
    else //只有本身选择
        Cycle_timer6->start(CycleTime6);
}

void MainWindow::on_pushButton_CycleSend_clicked()
{
    if(!ui->checkBox_DelaySend1->isChecked() && !ui->checkBox_DelaySend2->isChecked() && !ui->checkBox_DelaySend3->isChecked() && !ui->checkBox_DelaySend4->isChecked() && !ui->checkBox_DelaySend5->isChecked() && !ui->checkBox_DelaySend6->isChecked())
    {
        QMessageBox::critical(this,"Message", "请至少选择一个用于循环发送！");
        return;
    }

//    quint32 CycleTime = 0;

    if(ui->pushButton_CycleSend->text()=="开始")
    {
        if(ui->checkBox_DelaySend1->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend1->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number >= 200 && number <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime1 = number;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送1设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送1设置的时间数字异常");
                return;
            }
        }

        if(ui->checkBox_DelaySend2->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend2->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number2 = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number2 >= 200 && number2 <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime2 = number2;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送2设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送2设置的时间数字异常");
                return;
            }


        }

        if(ui->checkBox_DelaySend3->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend3->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number3 = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number3 >= 200 && number3 <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime3 = number3;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送3设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送3设置的时间数字异常");
                return;
            }


        }

        if(ui->checkBox_DelaySend4->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend4->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number4 = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number4 >= 200 && number4 <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime4 = number4;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送4设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送4设置的时间数字异常");
                return;
            }


        }

        if(ui->checkBox_DelaySend5->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend5->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number5 = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number5 >= 200 && number5 <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime5 = number5;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送5设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送5设置的时间数字异常");
                return;
            }


        }

        if(ui->checkBox_DelaySend6->isChecked())
        {
            //检查输入的间隔时间数字是否合理
            QString text = ui->lineEdit_DelaySend6->text();
            QRegularExpression re("^\\d+$");  // 正则表达式，确保字符串全为数字
            QRegularExpressionMatch match = re.match(text);
            if (match.hasMatch()) {
                bool ok;
                quint32 number6 = text.toInt(&ok);  // 尝试将文本转换为整数，并检查转换是否成功
                if (ok && number6 >= 200 && number6 <= 0xffffffff) {
                    qDebug() << "数字在范围内";
                    CycleTime6 = number6;
                } else {
                    qDebug() << "数字不在范围内(≥200ms)或者转换失败";
                    QMessageBox::critical(this,"时间设置", "发送6设置的时间数字不在范围内");
                    return;
                }
            } else {
                qDebug() << "文本不全是数字";
                QMessageBox::critical(this,"时间设置", "发送6设置的时间数字异常");
                return;
            }


        }
        //----------------------------------------------------------------------------------------
        if(ui->checkBox_DelaySend1->isChecked())
            Cycle_timer1->start(CycleTime1);
        else if(ui->checkBox_DelaySend2->isChecked())
            Cycle_timer2->start(CycleTime2);
        else if(ui->checkBox_DelaySend3->isChecked())
            Cycle_timer3->start(CycleTime3);
        else if(ui->checkBox_DelaySend4->isChecked())
            Cycle_timer4->start(CycleTime4);
        else if(ui->checkBox_DelaySend5->isChecked())
            Cycle_timer5->start(CycleTime5);
        else if(ui->checkBox_DelaySend6->isChecked())
            Cycle_timer6->start(CycleTime6);

        ui->pushButton_CycleSend->setText("停止");//todo：红色字体

        ui->checkBox_DelaySend1->setEnabled(false);
        ui->checkBox_DelaySend2->setEnabled(false);
        ui->checkBox_DelaySend3->setEnabled(false);
        ui->checkBox_DelaySend4->setEnabled(false);
        ui->checkBox_DelaySend5->setEnabled(false);
        ui->checkBox_DelaySend6->setEnabled(false);

        ui->pushButtonSend1->setEnabled(false);
        ui->pushButtonSend2->setEnabled(false);
        ui->pushButtonSend3->setEnabled(false);
        ui->pushButtonSend4->setEnabled(false);
        ui->pushButtonSend5->setEnabled(false);
        ui->pushButtonSend6->setEnabled(false);

        ui->pushButtonClearSend->setEnabled(false);
        ui->pushButtonClearSend_2->setEnabled(false);
        ui->pushButtonClearSend_3->setEnabled(false);
        ui->pushButtonClearSend_4->setEnabled(false);
        ui->pushButtonClearSend_5->setEnabled(false);
        ui->pushButtonClearSend_6->setEnabled(false);
    }
    else
    {
        Cycle_timer1->stop();
        Cycle_timer2->stop();
        Cycle_timer3->stop();
        Cycle_timer4->stop();
        Cycle_timer5->stop();
        Cycle_timer6->stop();

        ui->checkBox_DelaySend1->setEnabled(true);
        ui->checkBox_DelaySend2->setEnabled(true);
        ui->checkBox_DelaySend3->setEnabled(true);
        ui->checkBox_DelaySend4->setEnabled(true);
        ui->checkBox_DelaySend5->setEnabled(true);
        ui->checkBox_DelaySend6->setEnabled(true);

        ui->pushButtonSend1->setEnabled(true);
        ui->pushButtonSend2->setEnabled(true);
        ui->pushButtonSend3->setEnabled(true);
        ui->pushButtonSend4->setEnabled(true);
        ui->pushButtonSend5->setEnabled(true);
        ui->pushButtonSend6->setEnabled(true);

        ui->pushButtonClearSend->setEnabled(true);
        ui->pushButtonClearSend_2->setEnabled(true);
        ui->pushButtonClearSend_3->setEnabled(true);
        ui->pushButtonClearSend_4->setEnabled(true);
        ui->pushButtonClearSend_5->setEnabled(true);
        ui->pushButtonClearSend_6->setEnabled(true);


        ui->pushButton_CycleSend->setText("开始");
    }
}

void MainWindow::on_pushButton_BroadcastOpen_clicked()
{
    quint8 cmd[8] = {0x00, 0x06, 0x20, 0x02, 0x5A, 0x5A, 0x98, 0x80};//广播分闸，没有返回帧

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
}

void MainWindow::on_pushButton_BroadcastTimeSet_clicked()
{
    quint8 cmd[15] = {0x00, 0x10, 0x10, 0x00, 0x00, 0x03, 0x06, 0x1A, 0x06, 0x01, 0x08,0x00, 0x00, 0xBA, 0xC5};
    //手动输入的时间数字有效性检查
    quint8 DI_tmp[2] = {0};
    //检查输入的时间数据有效性
    //年
    QString newInput_year = ui->lineEdit_BroadcastYear->text();//“年”数据检查
    quint8 TimeData_length = newInput_year.length();
    qDebug()<<"newInput_year_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'年'数据不能为空");
        return;
    }
    else if(2 != TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据长度必须为2位数（年的后两位）");
        return;
    }
    else if(!isNumOnlyString(newInput_year))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(newInput_year.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(newInput_year.at(1).toLatin1());
    cmd[7] = DI_tmp[0]*10+DI_tmp[1];//时间数据
    if(cmd[7]>99)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'年'数据错误");
        return;
    }
    qDebug() << "Year:" << cmd[7];
    //月
    QString newInput_month = ui->lineEdit_BroadcastMonth->text();
    TimeData_length = newInput_month.length();
    qDebug()<<"newInput_month_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'月'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_month))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        cmd[8] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_month.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_month.at(1).toLatin1());
        if((DI_tmp[0]*10+DI_tmp[1])>12)
        {
            QMessageBox::critical(this,"TimeSet Message", "输入'月'数据不能超过12");
            return;
        }

        cmd[8] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[8]>12 || cmd[8]<1)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'月'数据错误");
        return;
    }
    qDebug() << "Month:" << cmd[8];
    //日
    QString newInput_day = ui->lineEdit_BroadcastDay->text();
    TimeData_length = newInput_day.length();
    qDebug()<<"newInput_day_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入'日'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_day))//检查是否有非法字符
    {
        QMessageBox::critical(this,"OpenTimeSet Message", "输入的'日'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        cmd[9] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_day.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_day.at(1).toLatin1());

        cmd[9] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[9]>31 || cmd[9]<1)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'日'数据错误");
        return;
    }
    qDebug() << "Day:" << cmd[9];
    //时
    QString newInput_hour = ui->lineEdit_BroadcastHour->text();
    TimeData_length = newInput_hour.length();
    qDebug()<<"newInput_hour_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'时'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_hour))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        cmd[10] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_hour.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_hour.at(1).toLatin1());
        cmd[10] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[10]>23)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'时'数据错误");
        return;
    }
    qDebug() << "Hour:" << cmd[10];
    //分
    QString newInput_min = ui->lineEdit_BroadcastMin->text();
    TimeData_length = newInput_min.length();
    qDebug()<<"newInput_min_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'分'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_min))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        cmd[11] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_min.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_min.at(1).toLatin1());
        cmd[11] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[11]>59)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'分'数据错误");
        return;
    }
    qDebug() << "Min:" << cmd[11];
    //秒
    QString newInput_sec = ui->lineEdit_BroadcastSec->text();
    TimeData_length = newInput_sec.length();
    qDebug()<<"newInput_sec_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入'秒'数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据长度错误");
        return;
    }
    else if(!isNumOnlyString(newInput_sec))//检查是否有非法字符
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据只能包含数字");
        return;
    }else{}

    if(1==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        cmd[12] = DI_tmp[0];
    }
    else if(2==TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(newInput_sec.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(newInput_sec.at(1).toLatin1());
        cmd[12] = DI_tmp[0]*10+DI_tmp[1];
    }
    if(cmd[12]>59)
    {
        QMessageBox::critical(this,"TimeSet Message", "输入的'秒'数据错误");
        return;
    }
    qDebug() << "Sec:" << cmd[12];
//----------------------------------------------------------------------
//    if(3 != gModbus_Addr)
//    {
//        //更新地址
//        cmd[0] = gModbus_Addr;
//    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,13);
    cmd[13] = (quint8)(crc_new>>8 & 0xff);
    cmd[14] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,15);
    m_serialPort->write((char*)&cmd,15);
    setAllRecFlagsFalse();
}

void MainWindow::on_pushButton_MbReadUVThres_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x11, 0x09, 0x00, 0x01, 0x50, 0xD6};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isReadUVThresCmd = true;
}

void MainWindow::on_pushButton_MbReadUVTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x03, 0x11, 0x0A, 0x00, 0x01, 0xA0, 0xD6};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadUVTimeCmd = true;
}

void MainWindow::on_pushButton_MbSetUVThres_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0x11, 0x09, 0x00, 0xA1, 0x9C, 0xAE};

    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbSetUVThres->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"UVThresData_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>3 || TimeData_length<2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度错误");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
    DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
    DI_tmp[2] = convertCharToHex(new_TimeData.at(2).toLatin1());
    quint16 tmpp = DI_tmp[0]*100+DI_tmp[1]*10+DI_tmp[2];

    if(tmpp>190 || tmpp<60)
    {
        QMessageBox::critical(this,"Message", "欠压数值不在规定的范围内[60V,190V]");
        return;
    }

//    qDebug("Input OV = %d",tmpp);
    cmd[4] = (quint8)(tmpp>>8);
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSetUVThresCmd = true;
}

void MainWindow::on_pushButton_MbSetUVTime_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0x11, 0x0A, 0x00, 0x02, 0x2C, 0xD7};

    quint8 DI_tmp[2] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_MbSetUVTime->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"OVTimeData_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度错误");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    quint16 tmpp = 0;
    if(1 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        tmpp = DI_tmp[0];
    }
    else
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpp = DI_tmp[0]*10+DI_tmp[1];
    }
    qDebug("Input OVTime = %d",tmpp);
    if(tmpp>10 || tmpp<1)
    {
        QMessageBox::critical(this,"Message", "欠压时间不在规定的范围内[1,10]");
        return;
    }

    cmd[4] = (quint8)(tmpp>>8);
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSetUVTimeCmd = true;
}

void MainWindow::on_pushButton_BinAddInfo_clicked()
{
    //QProcess::startDetached("release\\BinAddInfo_Release\\BinAddInfo.exe", QStringList()); //debug用
    QProcess::startDetached("BinAddInfo_Release\\BinAddInfo.exe", QStringList()); //release用
}

void MainWindow::on_pushButton_Hex2bin_clicked()
{
    //QProcess::startDetached("release\\hex2bin_Release\\hex2bin.exe", QStringList()); //debug用
    QProcess::startDetached("hex2bin_Release\\hex2bin.exe", QStringList()); //release用
}

void MainWindow::on_pushButton_ReadCommCombined_clicked() //读点表组合
{
    quint8 cmd[8] = {0x03, 0x03, 0xA1, 0x09, 0x00, 0x01, 0x76, 0x16};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbReadCommCombined = true;
}

void MainWindow::on_pushButton_WriteCommCombined_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0xA1, 0x09, 0x00, 0x0B, 0x3A, 0x11};

    if(ui->comboBox_SetCommCombined->currentText() == "研+仪表")
        cmd[5] = 3;
    else if(ui->comboBox_SetCommCombined->currentText() == "研+仪表+德力西")
        cmd[5] = 7;
    else if(ui->comboBox_SetCommCombined->currentText() == "研+仪表+天正")
        cmd[5] = 11;

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbSetCommCombined = true;
}

void MainWindow::on_pushButton_HexMerge_clicked()
{
    QProcess::startDetached("HexMerge_app\\HexMerge.exe", QStringList());
}

void MainWindow::on_pushButton_fHash_clicked()
{
    QProcess::startDetached("fHash_app\\fHash.exe", QStringList());
}

void MainWindow::on_pushButton_VOFA_clicked()
{
    QProcess::startDetached("VOFA+\\vofa+.exe", QStringList());
}

void MainWindow::on_pushButton_BeyondCompare_clicked()
{
    QProcess::startDetached("Beyond_Compare_Pro\\BCompare.exe", QStringList());
}

void MainWindow::on_pushButton_ModbusTool_clicked()
{
    QProcess::startDetached("MODBUS_Tool\\MODBUS调试助手.exe", QStringList());
}

void MainWindow::on_pushButton_RemoteLcdOn_clicked()
{
    quint8 cmd[8] = {0x03, 0x06, 0xA2, 0x00, 0x00, 0x1E, 0x3F, 0xF1};

    quint8 DI_tmp[2] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_RemoteLcdOnTime->text();
    quint8 TimeData_length = new_TimeData.length();
//    qDebug()<<"OVThresData_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入数据不能为空");
        return;
    }
    else if(TimeData_length>2)
    {
        QMessageBox::critical(this,"Message", "输入数据长度应为3位数且范围：[1,35]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入数据只能包含数字");
        return;
    }else{}

    quint16 tmpp = 0;
    if(1 == TimeData_length)
        tmpp = convertCharToHex(new_TimeData.at(0).toLatin1());
    else if(2 == TimeData_length)
    {
        DI_tmp[0] = convertCharToHex(new_TimeData.at(0).toLatin1());
        DI_tmp[1] = convertCharToHex(new_TimeData.at(1).toLatin1());
        tmpp = DI_tmp[0]*10+DI_tmp[1];
    }

    if(tmpp>35 || tmpp<1)
    {
        QMessageBox::critical(this,"Message", "远程点亮LCD屏幕时间设置范围为[1,35]s");
        return;
    }

//    qDebug("Input OV = %d",tmpp);
    cmd[4] = 0;
    cmd[5] = (quint8)tmpp;

    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,6);
    cmd[6] = (quint8)(crc_new>>8 & 0xff);
    cmd[7] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
//    isMbSetOVThresCmd = true;
    isMbRemoteCtrlLcdOnCmd = true;
}

void MainWindow::on_pushButton_DataVisualization_clicked()
{
    QProcess::startDetached("DataVisualizationGPT_Release\\DataVisualizationGPT.exe", QStringList());
}

void MainWindow::on_pushButton_MbWriteInValue_clicked()
{
    quint8 cmd[13] = {0x03, 0x10, 0xA1, 0x00, 0x00, 0x02, 0x04, 0x01, 0x04, 0x0D, 0x98, 0x65, 0x18};

    if(ui->comboBox_MbChooseInValue->currentText() == "125A")
        cmd[10] = 0x9B;
    else if(ui->comboBox_MbChooseInValue->currentText() == "100A")
        cmd[10] = 0x9A;
    else if(ui->comboBox_MbChooseInValue->currentText() == "80A")
        cmd[10] = 0x99;
    else if(ui->comboBox_MbChooseInValue->currentText() == "63A")
        cmd[10] = 0x98;

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,11);
    cmd[11] = (quint8)(crc_new>>8 & 0xff);
    cmd[12] = (quint8)(crc_new & 0xff);

    displayHexArrayInWidget(cmd,13);
    m_serialPort->write((char*)&cmd,13);
    setAllRecFlagsFalse();
    isMbSetInValue = true;
//    isMbSetCommCombined = true;
}

void MainWindow::on_pushButton_MbPaOffsetCali_clicked()
{
//    quint8 cmd[8] = {0xFA, 0x06, 0xA0, 0x00, 0x00, 0x01, 0x7F, 0x81}; //通配

    quint8 cmd[8] = {0x03, 0x06, 0xA0, 0x0D, 0x08, 0x98, 0x3D, 0x81};
    if(3 != gModbus_Addr)
    {
        //更新地址
        cmd[0] = gModbus_Addr;
        //更新CRC
        quint16 crc_new = modbus_rtu_crc16(cmd,6);
        cmd[6] = (quint8)(crc_new>>8 & 0xff);
        cmd[7] = (quint8)(crc_new & 0xff);
    }

    displayHexArrayInWidget(cmd,8);
    m_serialPort->write((char*)&cmd,8);
    setAllRecFlagsFalse();
    isMbPaOffsetCaliCmd = true;

//    isMbStartCaliCmd = true;
}
