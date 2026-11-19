#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QtCore/QtGlobal> //lzl
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>  //串口类头文件

#include <QSoundEffect> //lzl 播放音频类，在.pro文件中添加  QT += core gui multimedia

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QStringList getPortNameList();//获取所有可用的串口列表
    
    void SaveConfig();
    void LoadConfig();



private slots:
    void on_pushButtonOpenSerial_clicked();
    void receiveInfo();
    void on_pushButtonSend1_clicked();

    //void on_pushButtonOpenFile_clicked();

    void on_pushButtonClearSend_clicked();

    void on_pushButtonClearRec_clicked();

    //void on_pushButtonUpgrate_clicked();

    //void timerTransData();//定时发送
    //void on_pushButtonSync_clicked();

    void timerRecDataProcess();//250818

    void timerAutoOCProcess();//251217
    void timerCheckCloseProcess();//251217

    void timerAutoFreshProcess();//260423

    void timerRealTimeProcess();//260513

    void timerCycleSend1Process();
    void timerCycleSend2Process();
    void timerCycleSend3Process();
    void timerCycleSend4Process();
    void timerCycleSend5Process();
    void timerCycleSend6Process();

    //void on_pushButtonErase_clicked();

    //void on_pushButtonStopUpgrade_clicked();

    //void on_pushButtonEnterBoot_clicked();

    void on_pushButtonSend2_clicked();

    void on_pushButtonSend3_clicked();

    void on_pushButtonClearSend_2_clicked();

    void on_pushButtonClearSend_3_clicked();
    void on_pushButtonClearSendArea_clicked();

    void handleError(QSerialPort::SerialPortError error);

    //void on_comboBox_serialNum_activated(const QString &arg1);

    void on_pushButtonFreshPort_clicked();

    void on_pushButtonSend4_clicked();
    void on_pushButtonSend5_clicked();
    void on_pushButtonSend6_clicked();

    void on_pushButton_ReadVoltage_clicked();

    void on_pushButton_ReadFrequency_clicked();

    void on_pushButton_ReadMAC_clicked();

    void on_pushButton_ReadAsset_clicked();

    void on_pushButton_ReadProtectBit_clicked();

    void on_pushButton_WriteMAC_clicked();

    void on_pushButtonClearSend_4_clicked();

    void on_pushButtonClearSend_5_clicked();

    void on_pushButtonClearSend_6_clicked();

    void on_pushButton_WriteAssetCode_clicked();

    void on_pushButton_WriteProtectBit_clicked();
    void on_pushButton_ReadOV_Para_clicked();


    void on_pushButton_ReadUV_Para_clicked();

    void on_pushButton_ReadIsland_Para_clicked();

    void on_pushButton_DeviceInfo_clicked();

    void on_pushButton_WriteOV_Para_clicked();

    void on_pushButton_WriteUV_Para_clicked();

    void on_pushButton_WriteIsland_Para_clicked();

    void on_pushButton_RS485Set_clicked();

    void on_pushButton_RemoteOpen_clicked();

    void on_pushButton_RemoteCloseAllowed_clicked();

    void on_pushButton_RemoteClose_clicked();

    void on_pushButton_calibration_clicked();

    void on_pushButton_FCT_clicked();
    void on_pushButton_ReadRunWord_clicked();

    void on_pushButton_about_clicked();

    void on_pushButton_ReadAddr645_clicked();

//    void on_pushButton_Create645Frame_clicked();


    void on_pushButton_protocol645_clicked();
    void on_pushButton_CreateFrameSend_clicked();

    void on_pushButton_writeAddr645_clicked();

    void on_pushButton_ChangeBuadrate_clicked();


    void on_pushButton_ReadVolt645_clicked();

    void on_pushButton_ReadFreq645_clicked();

    void on_pushButton_ChangePWD_clicked();
    void on_pushButton_ReadDateTime_clicked();


    void on_pushButton_SoftwareVer645_clicked();

    void on_pushButton_HardVer645_clicked();
    void on_pushButton_AssetCode645_clicked();


    void on_pushButton_ProductionDate645_clicked();

    void on_pushButton_RunWord1_645_clicked();
    void on_pushButton_RunWord2_645_clicked();


    void on_pushButton_ReadCtrlWord1_clicked();

    void on_pushButton_ReadCtrlWord3_clicked();

    void on_pushButton_ReadCtrlWord7_clicked();

    void on_pushButton_WriteCtrlWord1_clicked();

    void on_pushButton_WriteCtrlWord3_clicked();

    void on_pushButton_WriteCtrlWord7_clicked();

    void on_pushButton_ReadOpenTime_clicked();
    void on_pushButton_WriteOpenTime_clicked();


    void on_pushButton_CancelOpen_clicked();

    void on_pushButton_ReadCloseTime_clicked();

    void on_pushButton_WriteCloseTime_clicked();

    void on_pushButton_CancelClose_clicked();
    void on_pushButton_GetSysDateTime_clicked();


    void on_pushButton_OtherFunction_clicked();

    void on_pushButton_ManualSetTime_clicked();
    void on_pushButton_ReadOpenTotalCnt_clicked();


    void on_pushButton_ClearTotalOpenCnt_clicked();

    void on_pushButton_OVOpenCnt_clicked();

    void on_pushButton_UVOpenCnt_clicked();

    void on_pushButton_LPOpenCnt_clicked();

    void on_pushButton_LVOpenCnt_clicked();

    void on_pushButton_RemoteOpenCnt_clicked();

    void on_pushButton_IslandOpenCnt_clicked();

    void on_pushButton_ManualOpenCnt_clicked();

    void on_pushButton_ClearOpenDataCnt_clicked();

    void on_pushButton_ClearProtectOpenCnt_clicked();
    void on_pushButton_TotalRunTime_clicked();


    void on_pushButton_ClearTotalRunTime_clicked();

    void on_pushButton_StateChangeNewAdd_clicked();

    void on_pushButton_ClearAll_clicked();

    void on_pushButton_ReadOVThreshold_clicked();

    void on_pushButton_ReadUVThreshold_clicked();

    void on_pushButton_ReadOVDelayTime_clicked();

    void on_pushButton_ReadUVDelayTime_clicked();

    void on_pushButton_WriteOVThreshold_clicked();

    void on_pushButton_WriteUVThreshold_clicked();

    void on_pushButton_WriteOVDelayTime_clicked();

    void on_pushButton_WriteUVDelayTime_clicked();

    void on_pushButton_ReadOVRecoverThres_clicked();

    void on_pushButton_ReadUVRecoverThres_clicked();

    void on_pushButton_ReadOVRecoverDelayTime_clicked();

    void on_pushButton_ReadUVRecoverDelayTime_clicked();

    void on_pushButton_WriteOVRecoverThres_clicked();

    void on_pushButton_WriteUVRecoverThres_clicked();

    void on_pushButton_WriteOVRecoverDelayTime_clicked();

    void on_pushButton_WriteUVRecoverDelayTime_clicked();

    void on_pushButton_Calibration645_clicked();

    void on_pushButton_MotorPosMCBPos_clicked();
    void on_pushButton_CheckProtectEvent_clicked();


    void on_pushButton_CheckStateChangeEvent_clicked();

    void on_pushButton_CheckIslandEvent_clicked();
    void on_pushButton_CheckSwitchingEvent_clicked();


    void on_pushButton_AutoCloseOpenStart_clicked();

    void on_pushButton_AutoCloseOpenStop_clicked();
    void on_pushButton_ResetConfig_clicked();

    void on_pushButton_GetDeviceType_clicked();

    void on_pushButtonOpenFile_clicked();

    void on_pushButtonUpgrate_clicked();


    void on_pushButtonStopUpgrade_clicked();
    void on_pushButton_MbReadAddr_clicked();


    void on_pushButton_ClearMbInfo_clicked();

    void on_pushButton_MbRemoteOpen_clicked();

    void on_pushButton_CloseLeakProtect_clicked();

    void on_pushButton_EnableLeakProtect_clicked();

    void on_pushButton_MbReadUa_clicked();

    void on_pushButton_MbReadIa_clicked();

    void on_pushButton_MbReadFreq_clicked();

    void on_pushButton_MbReadPa_clicked();

    void on_pushButton_MbReadComboAEAll_clicked();

    void on_pushButton_MbReadPosAEAll_clicked();

    void on_pushButton_MbReadNegAEAll_clicked();

    void on_pushButton_MbReadPFa_clicked();

    void on_pushButton_ReadLcdDisplayInterval_clicked();

    void on_pushButton_AutoFreshStart_clicked();

    void on_pushButton_AutoFreshStop_clicked();
    void on_pushButton_MbReadOVThres_clicked();


    void on_pushButton_MbReadOVTime_clicked();
    void on_pushButton_WriteLcdIntervalTime_clicked();


    void on_pushButton_MbReadSysTime_clicked();

    void on_pushButton_MbGetWinTime2Set_clicked();

    void on_pushButton_MbManuTimeSet_clicked();

    void on_pushButton_MbSetOVThres_clicked();

    void on_pushButton_MbSetOVTime_clicked();

    void on_pushButton_MbWriteAddr_clicked();

    void on_pushButton_MbReadSoftVersion_clicked();

    void on_pushButton_MbReadSN_PCODE_clicked();

    void on_pushButton_MbWriteSN_PCODE_clicked();
    void on_pushButton_MbSoftReset_clicked();


    void on_pushButton_MbClearAllEnergy_clicked();

    void on_pushButton_MbStartCalibration_clicked();

    void on_pushButton_MbExitCalibration_clicked();

    void on_pushButton_MbCali_Ia_clicked();

    void on_pushButton_MbCali_Ua_clicked();

    void on_pushButton_MbCali_Pa_clicked();

    void on_pushButton_MbRs485ParaSet_clicked();
    void on_pushButton_MbCheckProtectStatus_clicked();


    void on_pushButton_MbWriteProtectBit_clicked();

    void on_pushButton_MbReadTemp_clicked();

    void on_pushButton_MbReadSoftTime_clicked();

    void on_pushButton_MbReadLife_clicked();

    void on_pushButton_MbReadLeakLife_clicked();

    void on_pushButton_MbReadEventRecord_clicked();

    void on_pushButton_MbReadPosAE_clicked();

    void on_pushButton_MbReadNegAE_clicked();

    void on_pushButton_MbReadPosRE_clicked();

    void on_pushButton_MbReadNegRE_clicked();

    void on_pushButton_MbReadAppEnergy_clicked();

    void on_pushButton_MbWritePosAE_clicked();
    void on_pushButton_MbWriteNegAE_clicked();


    void on_pushButton_MbWritePosRE_clicked();

    void on_pushButton_MbWriteNegRE_clicked();

    void on_pushButton_MbWriteAppEnergy_clicked();

    void on_pushButton_MbReadComboNEAll_clicked();

    void on_pushButton_MbReadAppEngA_clicked();

    void on_pushButton_MbReadNPa_clicked();

    void on_pushButton_MbReadAppPa_clicked();

    void on_pushButton_MbReadPosRectE_clicked();

    void on_pushButton_MbReadNegRectE_clicked();
    void on_pushButton_AddCrc16Data_clicked();


    void on_pushButton_MBNewDataSend_clicked();
    void on_pushButton_MBCaliStatement_clicked();


    void on_pushButton_CycleSend_clicked();
    void on_pushButton_BroadcastOpen_clicked();


    void on_pushButton_BroadcastTimeSet_clicked();
    void on_pushButton_MbReadUVThres_clicked();


    void on_pushButton_MbReadUVTime_clicked();

    void on_pushButton_MbSetUVThres_clicked();

    void on_pushButton_MbSetUVTime_clicked();
    void on_pushButton_BinAddInfo_clicked();

    void on_pushButton_Hex2bin_clicked();

    void on_pushButton_ReadCommCombined_clicked();


    void on_pushButton_WriteCommCombined_clicked();
    void on_pushButton_HexMerge_clicked();


    void on_pushButton_fHash_clicked();

    void on_pushButton_VOFA_clicked();

    void on_pushButton_BeyondCompare_clicked();

    void on_pushButton_ModbusTool_clicked();
    void on_pushButton_RemoteLcdOn_clicked();

    void on_pushButton_DataVisualization_clicked();

    void on_pushButton_MbWriteInValue_clicked();

    void on_pushButton_MbPaOffsetCali_clicked();


private:
    Ui::MainWindow *ui;
    QSerialPort* m_serialPort; //串口类
    QStringList m_portNameList;//串口列表

    //bool isRecDone;//250818 接收数据是否完成

    quint64 binSize;//bin文件大小
    QByteArray *tempByte;//接收数据显示窗口暂存
    QString fileName;//bin文件路径
    QLabel *status= nullptr;//状态栏显示信息（左下角）

    //QTimer *timer;//定时器，用于定时发送

    QTimer *rec_timer;//250818 定时器，用于数据接收超时处理
    QTimer *AutoCloseOpen_timer;//定时器，用于自动分合闸
    QTimer *CheckCloseStatus_timer;//用于检测合闸是否成功
    QTimer *AutoFresh_timer;//用于自动发送指定命令的定时器
    QTimer *RealTime_timer; //系统时间更新定时器

    //循环发送定时器 260520
    QTimer *Cycle_timer1;
    QTimer *Cycle_timer2;
    QTimer *Cycle_timer3;
    QTimer *Cycle_timer4;
    QTimer *Cycle_timer5;
    QTimer *Cycle_timer6;

    quint32 Bin_checksum;//加载的bin文件的整体校验值
    bool isRecOK;//分包正确接收标志
    quint8 step;//烧写步骤编号，0：烧写，1：完成。
    quint16 package_num;//当前bin文件分包数
    quint16 package_ID; //当前烧录的包ID

    quint8 * MybinByte;
    bool isFileLoad;
//    bool isBooting;
    bool isUpdating;

//    void send_sync_cmd();
//    void send_erase_cmd();
//    void send_package_cmd();
//    void send_verify_cmd();
//    void send_reboot_cmd();
//    void send_boot_cmd();

    //语音相关
    QSoundEffect *effect_ComOccupied;
    QSoundEffect *effect_ComChanged;
    QSoundEffect *effect_WelcomUse;

    void displayHexArrayInWidget(const quint8* data, int size); //251217

    void OpenCmdSend(void);
    void CloseCmdSend(void);
    void CloseStatusCheck(void);

    //void handleError(QSerialPort::SerialPortError error);
};

#endif // MAINWINDOW_H
