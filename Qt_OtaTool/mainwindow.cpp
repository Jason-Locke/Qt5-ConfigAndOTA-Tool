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

#include "QProcess" //260818 调用外部exe文件用

//lzl add=====================================
#include "DLT698_frame.h"
#include <QRegularExpression> //正则表达式
#include <QDateTime>
//============================================

//#include <synchapi.h> //Sleep()相关

//#define CMD_MAX_SIZE 256
//#define PACKAGE_SIZE 200
#define PACKAGE_SIZE 252  //260310

//quint8 RecData_tmp[256] = {0};
QString RecData_tmp;
//QByteArray info;

const quint8 Rx_OK[3]  = {0x5A, 0x01, 0x01};//返回成功帧
const quint8 Rx_Done_OK[3]  = {0x5A, 0x02, 0x01};
const quint8 Rx_Err[3] = {0x5A, 0x01, 0x00};
const quint8 Rx_Done_Err[3]  = {0x5A, 0x02, 0x00};
const quint8 GetUpgradeCmdOK[3]  = {0x5A, 0xFE, 0x01};
const quint8 GetUpgradeCmdErr[3]  = {0x5A, 0xFE, 0x00};
quint8 OTA_data[265] = {0}; //OTA每帧数据最大长度
quint16 OTA_Len = 0; //待发送的实际长度
quint8  Version[4] = {0};
//quint8  Crc32Get[4] = {0};
quint8  Resend_cnt = 0; //Ota包重发次数

bool bin_available = false;//升级包是否可用 260818

quint8 gModbus_Addr = 3;//modbus全局地址，默认为3
//quint32 gModbus_binCRC32 = 0;//固件crcr32校验值

typedef enum
{
    OTA_MODBUS,
    OTA_DLT645,
    OTA_DLT698,
    OTA_CUSTOM,
}OtaTypeTPDF;

OtaTypeTPDF OtaType; //升级协议类型

//bool isCloseCMD_send = false;
//quint32 CloseCheckOK_times = 0;
//quint32 CloseCmdSend_times = 0;
//quint8 InputCloseDelayTime = 0;
//quint8 InputOpenDelayTime = 0;

char MAC645[6] = {0,0,0,0,0,2};//645地址全局变量

//MB升级相关查询标志
bool isMbReadSoftVerCmd = false;

//bool isReadVoltSend = false;
//bool isReadFreqSend = false;
//bool isReadMACSend = false;


QString AddTimeTag2Log(QString str) //260818
{
    QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss.zzz]");
    QString string_combin = time_tmp+str;
    return string_combin;
}

QString bytesToHex(quint8 *bytes,quint16 len) {
    QString hex;
    for (int i = 0; i < len; ++i) {
        quint8 byte = bytes[i];
        // 使用QString::number()将byte转换为十六进制，并追加到hex字符串中
        hex += QString::number(byte, 16).rightJustified(2, '0').toUpper(); // 使用.toUpper()确保十六进制是大写的
        hex += ' ';
    }
    return hex;
}

void setLineEditColor(QLineEdit *edit, const QColor &color) //设置LineEdit颜色
{
    edit->setStyleSheet(QString("color: %1;").arg(color.name()));
}

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

quint32 OtaCrc32(quint32 ini_crc, const quint8* data, quint32 len)
{
    quint32 crc = ini_crc;
    for (quint32 i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1u) {
                crc >>= 1;
                crc ^= 0xEDB88320UL;
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}

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
//------------------------------------------------------------------------

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
    ui->comboBox_baudrate->setCurrentIndex(5); //默认19200bps

//    ui->comboBox_baudrate_2->addItems(bautRatesList);
//    ui->comboBox_baudrate_2->setCurrentIndex(4); //默认9600bps

    //添加校验位列表
    QStringList parityList;
    parityList << "None" << "Odd" << "Even";
    ui->comboBoxParity->addItems(parityList);
    ui->comboBoxParity->setCurrentIndex(0);//默认无校验

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
//    AutoCloseOpen_timer = new QTimer(this);
//    connect(AutoCloseOpen_timer,SIGNAL(timeout()),this,SLOT(timerAutoOCProcess()));

//    CheckCloseStatus_timer = new QTimer(this);
//    connect(CheckCloseStatus_timer,SIGNAL(timeout()),this,SLOT(timerCheckCloseProcess()));
    //===============================================================================

    //260820 接收数据超时定时器=========================================================
    RecTimeout_timer = new QTimer(this);
    connect(RecTimeout_timer,SIGNAL(timeout()),this,SLOT(timerRecTimeoutProcess()));
    //===============================================================================

//    ui->pushButton_protocol645->setStyleSheet("QPushButton { color: red; }");

//    qRegisterMetaType<QSerialPort::SerialPortError>("SerialThread");
//    connect(m_serialPort, &QSerialPort::errorOccurred,this, &SerialThread::handleSerialError);
    connect(m_serialPort,SIGNAL(errorOccurred(QSerialPort::SerialPortError)),this,SLOT(handleError(QSerialPort::SerialPortError error)));

//    ui->pushButtonSync->setEnabled(false);
//    ui->pushButtonErase->setEnabled(false);

//    ui->pushButton_AutoCloseOpenStop->setEnabled(false);

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

    ui->pushButton_UpgradeMB->setEnabled(false);
    ui->pushButton_Upgrade698->setEnabled(false);
    ui->pushButton_Upgrade645->setEnabled(false);

    ui->checkBox_TimeTagSend->setChecked(true); //默认开启发送数据时间戳功能。
    ui->checkBox_TimeTag->setChecked(true);//默认开启接收数据时间戳功能。

//    ui->textEdit_OTAlog->append("******欢迎使用终端产品升级小助手******");
    QString StrTmp = "******欢迎使用终端产品升级小助手******";
    ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

    LoadConfig();
}

MainWindow::~MainWindow()
{
    if (m_serialPort->isOpen())
    {
        m_serialPort->close();
    }
    delete m_serialPort;

    SaveConfig();

    if(isFileLoad) //关闭上位机清除new变量 260818
    {
        isFileLoad = false;
        delete[] MybinByte;
    }

    delete ui;
}

void MainWindow::DisplayAndSendHex(quint8 *bytes,quint16 len)
{
//    m_serialPort->write((char*)&bytes,len);
    m_serialPort->write((char*)bytes,len);

    QString SendTmp = bytesToHex(bytes,len);
    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
    {
        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
        QString string_combin = time_tmp+SendTmp;
        ui->textEditSend->append(string_combin);
    }
    else
    {
        ui->textEditSend->append(SendTmp);
    }
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
    isMbReadSoftVerCmd = false;

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
            qDebug()<<m_portNameList[ui->comboBox_serialNum->currentIndex()]<<"打开失败!";
            QMessageBox::warning(this,"OpenComm Message", "打开串口失败，检查是否被占用");
            return;
        }

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
        rec_timer->start(50);//接收数据后开始超时计数 251218

//        ui->recData->append(strReceiveData);
    }
    else //ASCII输出
    {
        strReceiveData = info;

        //避免中文乱码
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString tmpQStr = tc->toUnicode(info);

        ui->recData->append(tmpQStr);
        rec_timer->start(50);//接收数据后开始超时计数 251218
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


#if 1
  if(false == isUpdating) //不是升级数据
  {
        if(isMbReadSoftVerCmd)
        {
            isMbReadSoftVerCmd = false;

            //从QString中解析Hex数组
            QByteArray ba = QByteArray::fromHex(RecData_tmp.toLatin1());
            QVector<quint8> data;
            quint16 data_len = ba.size();
            data.reserve(data_len);
            for (char c : ba) {
                data.append(static_cast<quint8>(c));
            }
            //检查CRC16
            quint16 crc_calc = modbus_rtu_crc16(&data[0],data_len-2);
            qDebug("data_len = %d, data[data_len-2] = 0x%02X, data[data_len-1] = 0x%02X",data_len,data[data_len-2],data[data_len-1]);
            quint16 crc_get = (quint16)((data[data_len-2]<<8) + data[data_len-1]);
            if(3 == data[1] && crc_get == crc_calc)
            {
                qDebug("CRC16 matched.");
                char buffer[128] = {0};
                QByteArray byteArray = RecData_tmp.toUtf8();
                strncpy(buffer, byteArray.constData(), sizeof(buffer));
                QString SoftVer = "";
                SoftVer = SoftVer+buffer[6]+buffer[7]+"."+buffer[8]+buffer[9]+"."+buffer[10]+buffer[11]+"."+buffer[12]+buffer[13];
                ui->lineEdit_ReadVerMB->setText(SoftVer);

                QString StrTmp = "[MB]读软件版本成功.";
                ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

            }
            else
            {
                qDebug("CRC16 not match.crc_calc = 0x%04X, crc_get = 0x%04X",crc_calc,crc_get);
                QString SoftVer = "";
                ui->lineEdit_ReadVerMB->setText(SoftVer);

                QString StrTmp = "[MB]读软件版本失败.";
                ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
            }



//            char buffer[128] = {0};

//            QByteArray byteArray = RecData_tmp.toUtf8();
//            strncpy(buffer, byteArray.constData(), sizeof(buffer));

//            if(('0' == buffer[2] && '3' == buffer[3]))
//            {
//                QString SoftVer = "";
//                SoftVer = SoftVer+buffer[6]+buffer[7]+"."+buffer[8]+buffer[9]+"."+buffer[10]+buffer[11]+"."+buffer[12]+buffer[13];
//                ui->lineEdit_ReadVerMB->setText(SoftVer);

//                QString StrTmp = "[MB]读软件版本成功.";
//                ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
//            }
//            else
//            {
//                QString StrTmp = "[MB]读软件版本失败.";
//                ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
//            }
        }
  }
  else //处理升级数据
  {
        RecTimeout_timer->stop();

        QByteArray RecArray = RecData_tmp.toUtf8();

//        qDebug()<<"转换为utf8数组个数: "<<RecArray.size();
//        qDebug()<<"转换为utf8数据: "<<RecArray.constData();

        quint16 RecBuf_len = RecArray.size();
        qDebug()<<"[OTA]RecBuf_len: "<<RecBuf_len;

        switch(OtaType) //在 switch 的某个 case 里定义了“有构造/析构的对象”，而该对象的作用域被后面的 case 穿透了，编译器禁止这种跳转。每个case内容用大括号包围。
        {
            case OTA_MODBUS:
            {
                const char MB_RetOK[3] = {0x10,0x00,0x00};//下位机返回成功的信息
                quint8 MbRemainSize = 11;
//                const char *MB_DataBuf = RecArray.data();//
//                qDebug("[MB]MB_DataBuf[1-3]: %02X, %02X, %02X\n",MB_DataBuf[1],MB_DataBuf[2],MB_DataBuf[3]);

                char buf_get1[8]={0};
                quint8 ConvertData_Temp1[3] = {0};

                strncpy(buf_get1, RecArray.constData(), 8);
                ConvertData_Temp1[0] = convertCharToHex(buf_get1[2])*16+convertCharToHex(buf_get1[3]);
                ConvertData_Temp1[1] = convertCharToHex(buf_get1[4])*16+convertCharToHex(buf_get1[5]);
                ConvertData_Temp1[2] = convertCharToHex(buf_get1[6])*16+convertCharToHex(buf_get1[7]);
                qDebug("[MB]ConvertData_Temp1[0-2]: %02X, %02X, %02X\n",ConvertData_Temp1[0],ConvertData_Temp1[1],ConvertData_Temp1[2]);
                if((0 == strncmp((const char*)ConvertData_Temp1,MB_RetOK,3))) //MCU返回成功
                {
                    qDebug("[MB]OK, package_ID = %d, package_num = %d",package_ID,package_num);

                    if(1 == package_ID)
                    {
                        QString StrTmp = "[MB]升级中...";
                        ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
                    }

                    //待发送数据长度处理
                    if(package_num - package_ID > 1)
                    {
                        OTA_Len = PACKAGE_SIZE+MbRemainSize;
                        qDebug("[MB]OTA len = %d",OTA_Len);
                    }
                    else if(package_num - package_ID == 1)
                    {
                        if(0 == (binSize % PACKAGE_SIZE))
                        {
                             OTA_Len = PACKAGE_SIZE+MbRemainSize;
                        }
                        else
                        {
                            OTA_Len = (binSize % PACKAGE_SIZE) + MbRemainSize; //最后一帧数据长度
                            qDebug("[MB]OTA last len = %d",OTA_Len);
                        }
                    }
                    //发送升级数据
                    if(package_num - package_ID >= 1) //发送数据本体
                    {
                        OTA_data[0] = gModbus_Addr; //帧头,Modbus地址
                        OTA_data[1] = 0x10; //烧写进行命令
                        OTA_data[2] = 0x00;
                        OTA_data[3] = 0x00;
                        OTA_data[4] = 0x00;
                        OTA_data[5] = (OTA_Len-MbRemainSize+2)/2;
                        qDebug("OTA_data[5] = %d\n",OTA_data[5]);
                        OTA_data[6] = OTA_Len-MbRemainSize+2;
                        qDebug("OTA_data[6] = %d\n",OTA_data[6]);
                        OTA_data[7] = 0x50;
                        OTA_data[8] = 0x01;
                        memcpy(&OTA_data[9],&MybinByte[package_ID*PACKAGE_SIZE],OTA_Len-MbRemainSize);
    //                    qDebug("OTA copy done");
                        quint16 crc16_MB = modbus_rtu_crc16(OTA_data,OTA_Len-2);
                        qDebug("[MB]crc16_MB = %d",crc16_MB);
                        OTA_data[OTA_Len-2] = (quint8)(crc16_MB>>8);
                        OTA_data[OTA_Len-1] = (quint8)crc16_MB;

    //                    qDebug("OTA send data: %02X, %02X, %02X, %02X, OTA_Len = %d",OTA_data[0],OTA_data[1],OTA_data[2],OTA_data[3],OTA_Len);
//                        m_serialPort->write((char*)&OTA_data,OTA_Len);
                        DisplayAndSendHex(OTA_data,OTA_Len);
                        qDebug("DisplayAndSendHex len = %d\n",OTA_Len);
    //                    m_serialPort->write((char*)OTA_data,OTA_Len);
                        double dProgress = (package_ID+1) * 100.0  / package_num;
                        ui->progressBar_MB->setValue(dProgress); //更新进度条
    //                    qDebug("OTA send done");
                        package_ID++;
                    }
                    else //数据本体已发送完成
                    {
                        OTA_data[0] = gModbus_Addr; //帧头
                        OTA_data[1] = 0x10;
                        OTA_data[2] = 0;
                        OTA_data[3] = 0;
                        OTA_data[4] = 0;
                        OTA_data[5] = 0x05;
                        OTA_data[6] = 0x0A;
                        OTA_data[7] = 0x50;
                        OTA_data[8] = 0x02;//结束升级命令 0x5002

                        //CRC32校验值，大端序
                        OTA_data[9] =  (quint8)(Bin_checksum>>24);
                        OTA_data[10] = (quint8)(Bin_checksum>>16);
                        OTA_data[11] = (quint8)(Bin_checksum>>8);
                        OTA_data[12] = (quint8)Bin_checksum;
                        //固件大小 bytes
                        quint32 BinSizeSend = binSize - 12;
                        OTA_data[13] =  (quint8)(BinSizeSend>>24);
                        OTA_data[14] = (quint8)(BinSizeSend>>16);
                        OTA_data[15] = (quint8)(BinSizeSend>>8);
                        OTA_data[16] = (quint8)BinSizeSend;
                        //更新MB校验值
                        quint16 crc16_MB_end = modbus_rtu_crc16(OTA_data,17);
                        OTA_data[17] = (quint8)(crc16_MB_end>>8);
                        OTA_data[18] = (quint8)crc16_MB_end;

                        DisplayAndSendHex(OTA_data,19);
                        qDebug("[MB]send OTA bin info.");

                        package_ID = 0;
                        Resend_cnt = 0;

                        ui->pushButton_UpgradeMB->setEnabled(true);
                        ui->pushButton_StopUpgradeMB->setEnabled(false);
                        ui->pushButton_Upgrade698->setEnabled(true);
                        ui->pushButton_StopUpgrade698->setEnabled(false);
                        ui->pushButton_Upgrade645->setEnabled(true);
                        ui->pushButton_StopUpgrade645->setEnabled(false);
                        ui->pushButtonUpgrate->setEnabled(true);
                        ui->pushButtonStopUpgrade->setEnabled(false);

                        ui->lineEdit_InputMBAddr->setEnabled(true);
                        ui->pushButtonReadVerMB->setEnabled(true);

                        setAllRecFlagsFalse();
                        isUpdating = false;

                        QString StrTmp = "[MB]升级完成.";
                        ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

                        qDebug("[MB]send OTA bin done.");
                    }

                }
                else  //MCU返回错误，返回异常帧
                {
                    qDebug("[MB]Err, package_ID = %d, package_num = %d",package_ID,package_num);

                    QString StrTmp = "[MB]升级异常!!!";
                    ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

                    //接收错误重发至多三次
                    if(Resend_cnt<3)
                    {
                        DisplayAndSendHex(OTA_data,OTA_Len);
                        Resend_cnt++;

                    }
                    else //重发三次仍失败
                    {
                        qDebug()<<"[OTA]Resend still failed.";
                        QString StrTmp = "[MB]重发3次仍异常，中断升级.";
                        ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

                        ui->pushButton_UpgradeMB->setEnabled(true);
                        ui->pushButton_StopUpgradeMB->setEnabled(false);
                        ui->pushButton_Upgrade698->setEnabled(true);
                        ui->pushButton_StopUpgrade698->setEnabled(false);
                        ui->pushButton_Upgrade645->setEnabled(true);
                        ui->pushButton_StopUpgrade645->setEnabled(false);
                        ui->pushButtonUpgrate->setEnabled(true);
                        ui->pushButtonStopUpgrade->setEnabled(false);

                        ui->lineEdit_InputMBAddr->setEnabled(true);
                        ui->pushButtonReadVerMB->setEnabled(true);

                        setAllRecFlagsFalse();
                        isUpdating = false;
                    }
                }

                break;
            }
            case OTA_DLT645:

                break;
            case OTA_DLT698:

                break;
            case OTA_CUSTOM:
            {
                quint16 crc16_calc = 0;
    //            quint16 len; //每帧长度
//                qint16 write_num_ret = 0;

                char buf_get[6]={0};
                quint8 ConvertData_Temp[3] = {0};

                strncpy(buf_get, RecArray.constData(), RecBuf_len);
                ConvertData_Temp[0] = convertCharToHex(buf_get[0])*16+convertCharToHex(buf_get[1]);
                ConvertData_Temp[1] = convertCharToHex(buf_get[2])*16+convertCharToHex(buf_get[3]);
                ConvertData_Temp[2] = convertCharToHex(buf_get[4])*16+convertCharToHex(buf_get[5]);

    //            QString Str;//打印测试
    //            for(int i=0;i<RecBuf_len/2;i++)
    //            {
    //                qDebug() << Str.asprintf("0x%02x",ConvertData_Temp[i]);//16进制打印
    //            }
    //            rec_timer->stop();
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
    //                    qDebug("OTA copy done");
                        crc16_calc = GetCRC16_Software(OTA_data,OTA_Len-3,0);
                        qDebug("crc16_calc = %d",crc16_calc);
                        OTA_data[OTA_Len-3] = (quint8)(crc16_calc>>8);
                        OTA_data[OTA_Len-2] = (quint8)crc16_calc;
                        OTA_data[OTA_Len-1] = 0xA5; //帧尾

    //                    qDebug("OTA send data: %02X, %02X, %02X, %02X, OTA_Len = %d",OTA_data[0],OTA_data[1],OTA_data[2],OTA_data[3],OTA_Len);
//                        write_num_ret = m_serialPort->write((char*)&OTA_data,OTA_Len);
                        DisplayAndSendHex(OTA_data,OTA_Len);
//                        qDebug("write_num_ret = %d",write_num_ret);

    //                    m_serialPort->write((char*)OTA_data,OTA_Len);
                        double dProgress = (package_ID+1) * 100.0  / package_num;
                        ui->progressBar->setValue(dProgress); //更新进度条
    //                    qDebug("OTA send done");
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
//                        m_serialPort->write((char*)&OTA_data,OTA_Len);
                        DisplayAndSendHex(OTA_data,OTA_Len);
                        qDebug("send OTA tail info");
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
//                        m_serialPort->write((char*)&OTA_data,OTA_Len);
                        DisplayAndSendHex(OTA_data,OTA_Len);
                        Resend_cnt++;
                    }
                    else //重发三次仍失败
                    {
                        qDebug()<<"[OTA]Resend still failed.";
                        package_ID = 0;
                        Resend_cnt = 0;
                        ui->pushButtonOpenFile->setEnabled(true);
                        ui->pushButtonUpgrate->setEnabled(true);
                        ui->pushButtonStopUpgrade->setEnabled(false);
                        setAllRecFlagsFalse();
                        isUpdating = false;
                    }
                }

                break;
            }
            default:
                break;

        }
        if(isUpdating) //更新过程中重置定时器，更新完成或更新中止则停止定时器
            RecTimeout_timer->start(3000);
  }
#endif
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

void MainWindow::timerRecTimeoutProcess() //接收数据超时处理
{
    QString StrTmp = "[MB]接收数据超时，升级中止.";
    ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

    ui->pushButton_UpgradeMB->setEnabled(true);
    ui->pushButton_StopUpgradeMB->setEnabled(false);
    ui->pushButton_Upgrade698->setEnabled(true);
    ui->pushButton_StopUpgrade698->setEnabled(false);
    ui->pushButton_Upgrade645->setEnabled(true);
    ui->pushButton_StopUpgrade645->setEnabled(false);
    ui->pushButtonUpgrate->setEnabled(true);
    ui->pushButtonStopUpgrade->setEnabled(false);

    switch(OtaType)
    {
        case OTA_MODBUS:
        {
            ui->lineEdit_InputMBAddr->setEnabled(true);
            ui->pushButtonReadVerMB->setEnabled(true);
            break;
        }
        case OTA_DLT645:
        {
            ui->lineEdit_Input645Addr->setEnabled(true);
            ui->pushButtonReadVer645->setEnabled(true);
            break;
        }
        case OTA_DLT698:
        {
            ui->lineEdit_Input698Addr->setEnabled(true);
            ui->pushButtonReadVer698->setEnabled(true);
            break;
        }
        case OTA_CUSTOM:
        {
            ui->lineEdit_Input698Addr->setEnabled(true);
            ui->pushButtonReadVer698->setEnabled(true);
            break;
        }
        default:
            break;
    }

    package_ID = 0;
    Resend_cnt = 0;
    isUpdating = false;

    RecTimeout_timer->stop();
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

    //研发测试相关
//    configIniWrite->setValue("/Last_Message/lineEdit_AutoCloseDelay",ui->lineEdit_AutoCloseDelay->text());
//    configIniWrite->setValue("/Last_Message/lineEdit_AutoOpenDelay",ui->lineEdit_AutoOpenDelay->text());


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

        //研发测试相关
//        ui->lineEdit_AutoCloseDelay->setText(configIniRead->value("/Last_Message/lineEdit_AutoCloseDelay").toString());
//        ui->lineEdit_AutoOpenDelay->setText(configIniRead->value("/Last_Message/lineEdit_AutoOpenDelay").toString());


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

void MainWindow::on_pushButton_about_clicked()
{
    QMessageBox::about(NULL,"关于软件","软件版本：V1.0.3 (2026-08-25)  基于Qt5.14.2开发\n更新说明：实现Modbus版本的升级逻辑。\n适用产品：正泰终端产品\n开发人员：Luo Zhenglei (Jason Locke)\n微信: luozhenglei\nemail: lzlei@chint.com  luozhenglei397@163.com  luozhenglei397@gmail.com\n\
Gitee url: https://gitee.com/jasonlocke/gvt-breaker-config-tool\nGitLab url: http://10.120.5.109/mcb/nbp_gvt_island/-/tree/develop/SW/CODE/GVT_ConfigTool_QT");
}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    quint16 remain = 0;

    quint32 Ver_offset = 0;//固件信息在bin文件末尾追加的数据
    quint32 Ver_value = 0;//读取bin结尾的版本号
    quint32 Crc32_value = 0;//读取bin结尾的crc32值

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
    quint32 RealBinSize = binSize - 12;//去除固件信息的实际bin文件大小。
    qDebug("Total bin size = %d, real bin size = %d\n",binSize,RealBinSize);
    ui->lineEdit_OtaFileSize->setText(QString::number(RealBinSize));//展示是原始bin大小，不是追加信息后的bin大小
    remain = RealBinSize % PACKAGE_SIZE;
    if(remain>0)
    {
        package_num = (RealBinSize / PACKAGE_SIZE) +1;
    }
    else
    {
        package_num = RealBinSize / PACKAGE_SIZE;
    }

//    remain = binSize % PACKAGE_SIZE;
//    if(remain>0)
//    {
//        package_num = (binSize / PACKAGE_SIZE) +1;
//    }
//    else
//    {
//        package_num = binSize / PACKAGE_SIZE;
//    }

    package_ID = 0;//每打开一个文件，从第0包开始
    qDebug("package_num = %d, remain = %d ",package_num,remain);

    QFile* file = new QFile;
    file->setFileName(fileName);
    bool ok = file->open(QIODevice::ReadOnly);
    if(ok) //从bin文件中读取CRC32和版本号
    {
        Ver_offset = binSize-8;//读版本值
        if(!file->seek(Ver_offset)) //文件指针偏移到Ver_offset
        {
            qDebug()<< "无法定位到版本偏移量";
        }
        else
        {
            QDataStream stream(file);
            stream.setByteOrder(QDataStream::BigEndian); // 根据实际情况设置字节序
            stream >> Ver_value;
            qDebug()<< "版本整数："<<Ver_value;
            Version[0] = (quint8)(Ver_value >> 24);//大版本号
            Version[1] = (quint8)(Ver_value >> 16);//中版本号
            Version[2] = (quint8)(Ver_value>>8); //小版本号
            Version[3] = (quint8)Ver_value;      //测试版本号
            QString SoftVer = QString("%1.%2.%3.%4").arg(Version[0]).arg(Version[1]).arg(Version[2]).arg(Version[3]);
            ui->lineEdit_GetBinVersion->setText(SoftVer);
        }
        Ver_offset = binSize-12;//读CRC32值
        if(!file->seek(Ver_offset)) //文件指针偏移到Ver_offset
        {
            qDebug()<< "无法定位到CRC32偏移量";
        }
        else
        {
            QDataStream IN(file);
            IN.setByteOrder(QDataStream::LittleEndian); // 按你的文件格式选
            IN >> Crc32_value;
            qDebug()<< "CRC32整数："<<Crc32_value;
            QString hexString = QString::asprintf("0x%X", Crc32_value); // 使用%X得到大写的16进制表示
            ui->lineEdit_GetCRC32Value->setText(hexString);
        }
        Ver_offset = binSize-4;//读规格值
        if(!file->seek(Ver_offset)) //文件指针偏移到Ver_offset
        {
            qDebug()<< "无法定位到规格值偏移量";
        }
        else
        {
            QDataStream IN(file);
            IN.setByteOrder(QDataStream::BigEndian); // 按你的文件格式选
            quint16 RateVal = 0;
            IN >> RateVal;
            qDebug()<< "规格整数："<<RateVal;
            ui->lineEdit_GetRate->setText(QString::number(RateVal));
        }
        Ver_offset = binSize-2;//读级数
        if(!file->seek(Ver_offset)) //文件指针偏移到Ver_offset
        {
            qDebug()<< "无法定位到级数值偏移量";
        }
        else
        {
            QDataStream IN(file);
            IN.setByteOrder(QDataStream::BigEndian); // 按你的文件格式选
            quint16 PoleVal = 0;
            IN >> PoleVal;
            qDebug()<< "级数整数："<<PoleVal;
            ui->lineEdit_GetPole->setText(QString::number(PoleVal));
        }

        file->seek(0);//文件指针返回开头处
    }
    else
    {
        QMessageBox::information(this,"Error Message", "File Open Error" + file->errorString());
        return;
    }
    QDataStream in(file);

    char * binByte = new char[RealBinSize];

    in.setVersion (QDataStream::Qt_5_9);

    statusBar()->showMessage(tr("准备读取数据")); //用于获取或创建主窗口底部的状态栏（QStatusBar），用于向用户显示应用程序的实时状态、提示信息或永久性控件。
    //in.readRawData (binByte, binSize);      //读出文件到缓存
    qint32 ret = in.readRawData(binByte, RealBinSize);
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
        delete[] MybinByte;//内容清空，重新加载。
    }

//    tempByte = new QByteArray(binByte, RealBinSize); //格式转换
    MybinByte = new quint8[RealBinSize]; //去除尾部固件信息后的纯净固件
    isFileLoad = true;
    for(quint32 i = 0;i<RealBinSize;i++)
    {
        MybinByte[i] = (quint8)binByte[i];
        //qDebug("%02X",MybinByte[i]);
    }

//    Bin_checksum = crc32_compute(MybinByte, RealBinSize, NULL);
    Bin_checksum = OtaCrc32(0xFFFFFFFFUL, MybinByte, RealBinSize);//整个bin文件计算校验值，要验证
    qDebug("CRC32 calc = 0x%08X",Bin_checksum);
    QString hex1String = QString::asprintf("0x%X", Bin_checksum); // 使用%X得到大写的16进制表示
    ui->lineEdit_CalcCRC32Value->setText(hex1String);
    if(Crc32_value == Bin_checksum) //校验值相同
    {
        ui->lineEdit_CalcCRC32Value->setStyleSheet(
            "color: black;"
            "background-color: green;"
        );
        bin_available = true;
//        gModbus_binCRC32 = Bin_checksum;

        ui->pushButton_UpgradeMB->setEnabled(true);
        ui->pushButton_StopUpgradeMB->setEnabled(false);
        ui->pushButton_Upgrade698->setEnabled(true);
        ui->pushButton_StopUpgrade698->setEnabled(false);
        ui->pushButton_Upgrade645->setEnabled(true);
        ui->pushButton_StopUpgrade645->setEnabled(false);
        ui->pushButtonUpgrate->setEnabled(true);
        ui->pushButtonStopUpgrade->setEnabled(false);

        QString StrTmp = "升级固件已加载，本地校验通过！";
        ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
    }
//        setLineEditColor(ui->lineEdit_CalcCRC32Value, Qt::green);   //检验通过
    else
    {
        ui->lineEdit_CalcCRC32Value->setStyleSheet(
            "color: black;"
            "background-color: red;"
        );
        bin_available = false;

        ui->pushButton_UpgradeMB->setEnabled(false);
        ui->pushButton_StopUpgradeMB->setEnabled(true);
        ui->pushButton_Upgrade698->setEnabled(false);
        ui->pushButton_StopUpgrade698->setEnabled(true);
        ui->pushButton_Upgrade645->setEnabled(false);
        ui->pushButton_StopUpgrade645->setEnabled(true);
        ui->pushButtonUpgrate->setEnabled(false);
        ui->pushButtonStopUpgrade->setEnabled(true);

        QString StrTmp = "升级固件本地校验不通过，请检查！";
        ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));
    }
//        setLineEditColor(ui->lineEdit_CalcCRC32Value, Qt::red);     //校验不通过

//    qDebug() <<"binSize = " << binSize;
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

//    delete tempByte;
    delete[] binByte;

//    delete tempStr;

    file->close();
    delete file;
    //delete pcsfileInfo;
}

//void MainWindow::on_pushButtonUpgrate_clicked()
//{
//    if(0 == binSize)
//    {
//        QMessageBox::information(this,"Error Message", "Please Select a bin File");
//        return;
//    }
//    quint8 cmd[6] = {0x5A,0xFE,0x00,0x00,0xC4,0xA5}; //发送启动升级命令
////    QString start = "\n[Info] ********* start update ************\n";
//    ui->pushButtonOpenFile->setEnabled(false);
////    ui->checkBoxHexReceive->setChecked(false);//接收框修改成接收字符串
////    ui->recData->insertPlainText (start);

//    package_ID = 0;
//    Resend_cnt = 0;
//    isUpdating = true;

//    ui->pushButtonUpgrate->setEnabled(false);
//    ui->pushButtonStopUpgrade->setEnabled(true);
//    setAllRecFlagsFalse();
//    m_serialPort->write((char*)&cmd,6);
//}

//void MainWindow::on_pushButtonStopUpgrade_clicked()
//{
//    quint8 cmd[6] = {0x5A,0xEE,0x00,0xE0,0xC7,0xA5}; //发送停止升级命令
//    isUpdating = false;
//    Resend_cnt = 0;
//    package_ID = 0;

////    rec_timer->stop();

//    ui->pushButtonOpenFile->setEnabled(true);
//    ui->pushButtonUpgrate->setEnabled(true);
//    ui->pushButtonStopUpgrade->setEnabled(false);
//    ui->progressBar->setValue(0);

//    setAllRecFlagsFalse();
//    m_serialPort->write((char*)&cmd,6);
//    qDebug("stop upgrade.");
////    timer->stop();

//}

//void MainWindow::on_pushButton_WriteOVRecoverThres_clicked()
//{}

//void MainWindow::on_pushButton_ClearAll_clicked()
//{}

void MainWindow::on_pushButton_ClearOTAlog_clicked()
{
    ui->textEdit_OTAlog->setText(NULL);
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

void MainWindow::on_pushButton_UpgradeMB_clicked()
{
    if(0 == binSize)
    {
        QMessageBox::information(this,"Error Message", "Please Select a bin File");
        return;
    }

    if(!bin_available) //bin文件本地校验失败直接退出。
    {
        QMessageBox::information(this,"Error Message", "Local bin file check failed.");
        return;
    }

    OtaType = OTA_MODBUS;
    quint8 cmd[11] = {0x03,0x10,0x00,0x00,0x00,0x01,0x02,0x50,0x00,0x83,0x30};//发送启动升级命令，地址0x0000，写0x5000

    quint8 DI_tmp[3] = {0};

    //检查输入的数据有效性
    QString new_TimeData = ui->lineEdit_InputMBAddr->text();
    quint8 TimeData_length = new_TimeData.length();
    qDebug()<<"MbAddr_length: "<<TimeData_length;
    if(0==TimeData_length)
    {
        QMessageBox::critical(this,"Message", "输入地址数据不能为空");
        return;
    }
    else if(TimeData_length>3)
    {
        QMessageBox::critical(this,"Message", "输入地址数据长度应为3位数以内且范围：[1,247]");
        return;
    }
    else if(!isNumOnlyString(new_TimeData))//检查是否有非法字符
    {
        QMessageBox::critical(this,"Message", "输入地址数据只能包含数字");
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

    gModbus_Addr = tmpp;
    if(3 != tmpp)
    {
        //更新地址
        cmd[0] = tmpp;
    }

    //更新CRC
    quint16 crc_new = modbus_rtu_crc16(cmd,9);
    cmd[9] = (quint8)(crc_new>>8 & 0xff);
    cmd[10] = (quint8)(crc_new & 0xff);

//    QString start = "\n[Info] ********* start update ************\n";
//    ui->pushButtonOpenFile->setEnabled(false);
//    ui->checkBoxHexReceive->setChecked(false);//接收框修改成接收字符串
//    ui->recData->insertPlainText (start);

    package_ID = 0;
    Resend_cnt = 0;
    isUpdating = true;

    ui->pushButton_UpgradeMB->setEnabled(false);
    ui->pushButton_StopUpgradeMB->setEnabled(true);
    ui->pushButtonReadVerMB->setEnabled(false); //读版本不可用
    ui->lineEdit_InputMBAddr->setEnabled(false); //地址不准输入
    setAllRecFlagsFalse();

    //其它升级方法都不可用
    ui->pushButton_Upgrade698->setEnabled(false);
    ui->pushButton_StopUpgrade698->setEnabled(false);
    ui->pushButton_Upgrade645->setEnabled(false);
    ui->pushButton_StopUpgrade645->setEnabled(false);
    ui->pushButtonUpgrate->setEnabled(false);
    ui->pushButtonStopUpgrade->setEnabled(false);


    QString StrTmp = "[MB]启动升级";
    ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

    RecTimeout_timer->start(3000);//3s超时

    DisplayAndSendHex(cmd,11);

//    m_serialPort->write((char*)&cmd,11);
//    QString SendTmp = bytesToHex(cmd,11);
//    if(ui->checkBox_TimeTagSend->isChecked()) //打开时间戳功能
//    {
//        QString time_tmp = QDateTime::currentDateTime().toString("[yyyy-MM-dd_hh:mm:ss.zzz]");
//        QString string_combin = time_tmp+SendTmp;
//        ui->textEditSend->append(string_combin);
//    }
//    else
//    {
//        ui->textEditSend->append(SendTmp);
//    }

}

void MainWindow::on_pushButton_StopUpgradeMB_clicked() //中断升级
{
    isUpdating = false;
    Resend_cnt = 0;
    package_ID = 0;

//    ui->pushButtonOpenFile->setEnabled(true);
//    ui->pushButtonUpgrate->setEnabled(true);
//    ui->pushButtonStopUpgrade->setEnabled(false);

    ui->pushButton_UpgradeMB->setEnabled(true);
    ui->pushButton_StopUpgradeMB->setEnabled(false);
    ui->pushButton_Upgrade698->setEnabled(true);
    ui->pushButton_StopUpgrade698->setEnabled(false);
    ui->pushButton_Upgrade645->setEnabled(true);
    ui->pushButton_StopUpgrade645->setEnabled(false);
    ui->pushButtonUpgrate->setEnabled(true);
    ui->pushButtonStopUpgrade->setEnabled(false);

    ui->lineEdit_InputMBAddr->setEnabled(true);
    ui->pushButtonReadVerMB->setEnabled(true);

    ui->progressBar_MB->setValue(0); //进度条复位

    QString StrTmp = "[MB]中止升级";
    ui->textEdit_OTAlog->append(AddTimeTag2Log(StrTmp));

//    setAllRecFlagsFalse();
//    m_serialPort->write((char*)&cmd,6);
    qDebug("stop upgrade.");
}

void MainWindow::on_pushButtonReadVerMB_clicked()
{
    quint8 cmd[8] = {0xFA, 0x03, 0x00, 0x32, 0x00, 0x02, 0x70, 0x4F}; //通配读版本

//    displayHexArrayInWidget(cmd,8);
//    m_serialPort->write((char*)&cmd,8);

    DisplayAndSendHex(cmd,8);

    setAllRecFlagsFalse();
    isMbReadSoftVerCmd = true;
}
