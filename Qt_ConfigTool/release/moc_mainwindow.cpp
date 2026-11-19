/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[212];
    char stringdata0[7263];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 31), // "on_pushButtonOpenSerial_clicked"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 11), // "receiveInfo"
QT_MOC_LITERAL(4, 56, 26), // "on_pushButtonSend1_clicked"
QT_MOC_LITERAL(5, 83, 30), // "on_pushButtonClearSend_clicked"
QT_MOC_LITERAL(6, 114, 29), // "on_pushButtonClearRec_clicked"
QT_MOC_LITERAL(7, 144, 19), // "timerRecDataProcess"
QT_MOC_LITERAL(8, 164, 18), // "timerAutoOCProcess"
QT_MOC_LITERAL(9, 183, 22), // "timerCheckCloseProcess"
QT_MOC_LITERAL(10, 206, 21), // "timerAutoFreshProcess"
QT_MOC_LITERAL(11, 228, 20), // "timerRealTimeProcess"
QT_MOC_LITERAL(12, 249, 22), // "timerCycleSend1Process"
QT_MOC_LITERAL(13, 272, 22), // "timerCycleSend2Process"
QT_MOC_LITERAL(14, 295, 22), // "timerCycleSend3Process"
QT_MOC_LITERAL(15, 318, 22), // "timerCycleSend4Process"
QT_MOC_LITERAL(16, 341, 22), // "timerCycleSend5Process"
QT_MOC_LITERAL(17, 364, 22), // "timerCycleSend6Process"
QT_MOC_LITERAL(18, 387, 26), // "on_pushButtonSend2_clicked"
QT_MOC_LITERAL(19, 414, 26), // "on_pushButtonSend3_clicked"
QT_MOC_LITERAL(20, 441, 32), // "on_pushButtonClearSend_2_clicked"
QT_MOC_LITERAL(21, 474, 32), // "on_pushButtonClearSend_3_clicked"
QT_MOC_LITERAL(22, 507, 34), // "on_pushButtonClearSendArea_cl..."
QT_MOC_LITERAL(23, 542, 11), // "handleError"
QT_MOC_LITERAL(24, 554, 28), // "QSerialPort::SerialPortError"
QT_MOC_LITERAL(25, 583, 5), // "error"
QT_MOC_LITERAL(26, 589, 30), // "on_pushButtonFreshPort_clicked"
QT_MOC_LITERAL(27, 620, 26), // "on_pushButtonSend4_clicked"
QT_MOC_LITERAL(28, 647, 26), // "on_pushButtonSend5_clicked"
QT_MOC_LITERAL(29, 674, 26), // "on_pushButtonSend6_clicked"
QT_MOC_LITERAL(30, 701, 33), // "on_pushButton_ReadVoltage_cli..."
QT_MOC_LITERAL(31, 735, 35), // "on_pushButton_ReadFrequency_c..."
QT_MOC_LITERAL(32, 771, 29), // "on_pushButton_ReadMAC_clicked"
QT_MOC_LITERAL(33, 801, 31), // "on_pushButton_ReadAsset_clicked"
QT_MOC_LITERAL(34, 833, 36), // "on_pushButton_ReadProtectBit_..."
QT_MOC_LITERAL(35, 870, 30), // "on_pushButton_WriteMAC_clicked"
QT_MOC_LITERAL(36, 901, 32), // "on_pushButtonClearSend_4_clicked"
QT_MOC_LITERAL(37, 934, 32), // "on_pushButtonClearSend_5_clicked"
QT_MOC_LITERAL(38, 967, 32), // "on_pushButtonClearSend_6_clicked"
QT_MOC_LITERAL(39, 1000, 36), // "on_pushButton_WriteAssetCode_..."
QT_MOC_LITERAL(40, 1037, 37), // "on_pushButton_WriteProtectBit..."
QT_MOC_LITERAL(41, 1075, 33), // "on_pushButton_ReadOV_Para_cli..."
QT_MOC_LITERAL(42, 1109, 33), // "on_pushButton_ReadUV_Para_cli..."
QT_MOC_LITERAL(43, 1143, 37), // "on_pushButton_ReadIsland_Para..."
QT_MOC_LITERAL(44, 1181, 32), // "on_pushButton_DeviceInfo_clicked"
QT_MOC_LITERAL(45, 1214, 34), // "on_pushButton_WriteOV_Para_cl..."
QT_MOC_LITERAL(46, 1249, 34), // "on_pushButton_WriteUV_Para_cl..."
QT_MOC_LITERAL(47, 1284, 38), // "on_pushButton_WriteIsland_Par..."
QT_MOC_LITERAL(48, 1323, 30), // "on_pushButton_RS485Set_clicked"
QT_MOC_LITERAL(49, 1354, 32), // "on_pushButton_RemoteOpen_clicked"
QT_MOC_LITERAL(50, 1387, 40), // "on_pushButton_RemoteCloseAllo..."
QT_MOC_LITERAL(51, 1428, 33), // "on_pushButton_RemoteClose_cli..."
QT_MOC_LITERAL(52, 1462, 33), // "on_pushButton_calibration_cli..."
QT_MOC_LITERAL(53, 1496, 25), // "on_pushButton_FCT_clicked"
QT_MOC_LITERAL(54, 1522, 33), // "on_pushButton_ReadRunWord_cli..."
QT_MOC_LITERAL(55, 1556, 27), // "on_pushButton_about_clicked"
QT_MOC_LITERAL(56, 1584, 33), // "on_pushButton_ReadAddr645_cli..."
QT_MOC_LITERAL(57, 1618, 33), // "on_pushButton_protocol645_cli..."
QT_MOC_LITERAL(58, 1652, 37), // "on_pushButton_CreateFrameSend..."
QT_MOC_LITERAL(59, 1690, 34), // "on_pushButton_writeAddr645_cl..."
QT_MOC_LITERAL(60, 1725, 36), // "on_pushButton_ChangeBuadrate_..."
QT_MOC_LITERAL(61, 1762, 33), // "on_pushButton_ReadVolt645_cli..."
QT_MOC_LITERAL(62, 1796, 33), // "on_pushButton_ReadFreq645_cli..."
QT_MOC_LITERAL(63, 1830, 31), // "on_pushButton_ChangePWD_clicked"
QT_MOC_LITERAL(64, 1862, 34), // "on_pushButton_ReadDateTime_cl..."
QT_MOC_LITERAL(65, 1897, 36), // "on_pushButton_SoftwareVer645_..."
QT_MOC_LITERAL(66, 1934, 32), // "on_pushButton_HardVer645_clicked"
QT_MOC_LITERAL(67, 1967, 34), // "on_pushButton_AssetCode645_cl..."
QT_MOC_LITERAL(68, 2002, 39), // "on_pushButton_ProductionDate6..."
QT_MOC_LITERAL(69, 2042, 34), // "on_pushButton_RunWord1_645_cl..."
QT_MOC_LITERAL(70, 2077, 34), // "on_pushButton_RunWord2_645_cl..."
QT_MOC_LITERAL(71, 2112, 35), // "on_pushButton_ReadCtrlWord1_c..."
QT_MOC_LITERAL(72, 2148, 35), // "on_pushButton_ReadCtrlWord3_c..."
QT_MOC_LITERAL(73, 2184, 35), // "on_pushButton_ReadCtrlWord7_c..."
QT_MOC_LITERAL(74, 2220, 36), // "on_pushButton_WriteCtrlWord1_..."
QT_MOC_LITERAL(75, 2257, 36), // "on_pushButton_WriteCtrlWord3_..."
QT_MOC_LITERAL(76, 2294, 36), // "on_pushButton_WriteCtrlWord7_..."
QT_MOC_LITERAL(77, 2331, 34), // "on_pushButton_ReadOpenTime_cl..."
QT_MOC_LITERAL(78, 2366, 35), // "on_pushButton_WriteOpenTime_c..."
QT_MOC_LITERAL(79, 2402, 32), // "on_pushButton_CancelOpen_clicked"
QT_MOC_LITERAL(80, 2435, 35), // "on_pushButton_ReadCloseTime_c..."
QT_MOC_LITERAL(81, 2471, 36), // "on_pushButton_WriteCloseTime_..."
QT_MOC_LITERAL(82, 2508, 33), // "on_pushButton_CancelClose_cli..."
QT_MOC_LITERAL(83, 2542, 36), // "on_pushButton_GetSysDateTime_..."
QT_MOC_LITERAL(84, 2579, 35), // "on_pushButton_OtherFunction_c..."
QT_MOC_LITERAL(85, 2615, 35), // "on_pushButton_ManualSetTime_c..."
QT_MOC_LITERAL(86, 2651, 38), // "on_pushButton_ReadOpenTotalCn..."
QT_MOC_LITERAL(87, 2690, 39), // "on_pushButton_ClearTotalOpenC..."
QT_MOC_LITERAL(88, 2730, 31), // "on_pushButton_OVOpenCnt_clicked"
QT_MOC_LITERAL(89, 2762, 31), // "on_pushButton_UVOpenCnt_clicked"
QT_MOC_LITERAL(90, 2794, 31), // "on_pushButton_LPOpenCnt_clicked"
QT_MOC_LITERAL(91, 2826, 31), // "on_pushButton_LVOpenCnt_clicked"
QT_MOC_LITERAL(92, 2858, 35), // "on_pushButton_RemoteOpenCnt_c..."
QT_MOC_LITERAL(93, 2894, 35), // "on_pushButton_IslandOpenCnt_c..."
QT_MOC_LITERAL(94, 2930, 35), // "on_pushButton_ManualOpenCnt_c..."
QT_MOC_LITERAL(95, 2966, 38), // "on_pushButton_ClearOpenDataCn..."
QT_MOC_LITERAL(96, 3005, 41), // "on_pushButton_ClearProtectOpe..."
QT_MOC_LITERAL(97, 3047, 34), // "on_pushButton_TotalRunTime_cl..."
QT_MOC_LITERAL(98, 3082, 39), // "on_pushButton_ClearTotalRunTi..."
QT_MOC_LITERAL(99, 3122, 39), // "on_pushButton_StateChangeNewA..."
QT_MOC_LITERAL(100, 3162, 30), // "on_pushButton_ClearAll_clicked"
QT_MOC_LITERAL(101, 3193, 37), // "on_pushButton_ReadOVThreshold..."
QT_MOC_LITERAL(102, 3231, 37), // "on_pushButton_ReadUVThreshold..."
QT_MOC_LITERAL(103, 3269, 37), // "on_pushButton_ReadOVDelayTime..."
QT_MOC_LITERAL(104, 3307, 37), // "on_pushButton_ReadUVDelayTime..."
QT_MOC_LITERAL(105, 3345, 38), // "on_pushButton_WriteOVThreshol..."
QT_MOC_LITERAL(106, 3384, 38), // "on_pushButton_WriteUVThreshol..."
QT_MOC_LITERAL(107, 3423, 38), // "on_pushButton_WriteOVDelayTim..."
QT_MOC_LITERAL(108, 3462, 38), // "on_pushButton_WriteUVDelayTim..."
QT_MOC_LITERAL(109, 3501, 40), // "on_pushButton_ReadOVRecoverTh..."
QT_MOC_LITERAL(110, 3542, 40), // "on_pushButton_ReadUVRecoverTh..."
QT_MOC_LITERAL(111, 3583, 44), // "on_pushButton_ReadOVRecoverDe..."
QT_MOC_LITERAL(112, 3628, 44), // "on_pushButton_ReadUVRecoverDe..."
QT_MOC_LITERAL(113, 3673, 41), // "on_pushButton_WriteOVRecoverT..."
QT_MOC_LITERAL(114, 3715, 41), // "on_pushButton_WriteUVRecoverT..."
QT_MOC_LITERAL(115, 3757, 45), // "on_pushButton_WriteOVRecoverD..."
QT_MOC_LITERAL(116, 3803, 45), // "on_pushButton_WriteUVRecoverD..."
QT_MOC_LITERAL(117, 3849, 36), // "on_pushButton_Calibration645_..."
QT_MOC_LITERAL(118, 3886, 36), // "on_pushButton_MotorPosMCBPos_..."
QT_MOC_LITERAL(119, 3923, 39), // "on_pushButton_CheckProtectEve..."
QT_MOC_LITERAL(120, 3963, 43), // "on_pushButton_CheckStateChang..."
QT_MOC_LITERAL(121, 4007, 38), // "on_pushButton_CheckIslandEven..."
QT_MOC_LITERAL(122, 4046, 41), // "on_pushButton_CheckSwitchingE..."
QT_MOC_LITERAL(123, 4088, 40), // "on_pushButton_AutoCloseOpenSt..."
QT_MOC_LITERAL(124, 4129, 39), // "on_pushButton_AutoCloseOpenSt..."
QT_MOC_LITERAL(125, 4169, 33), // "on_pushButton_ResetConfig_cli..."
QT_MOC_LITERAL(126, 4203, 35), // "on_pushButton_GetDeviceType_c..."
QT_MOC_LITERAL(127, 4239, 29), // "on_pushButtonOpenFile_clicked"
QT_MOC_LITERAL(128, 4269, 28), // "on_pushButtonUpgrate_clicked"
QT_MOC_LITERAL(129, 4298, 32), // "on_pushButtonStopUpgrade_clicked"
QT_MOC_LITERAL(130, 4331, 32), // "on_pushButton_MbReadAddr_clicked"
QT_MOC_LITERAL(131, 4364, 33), // "on_pushButton_ClearMbInfo_cli..."
QT_MOC_LITERAL(132, 4398, 34), // "on_pushButton_MbRemoteOpen_cl..."
QT_MOC_LITERAL(133, 4433, 38), // "on_pushButton_CloseLeakProtec..."
QT_MOC_LITERAL(134, 4472, 39), // "on_pushButton_EnableLeakProte..."
QT_MOC_LITERAL(135, 4512, 30), // "on_pushButton_MbReadUa_clicked"
QT_MOC_LITERAL(136, 4543, 30), // "on_pushButton_MbReadIa_clicked"
QT_MOC_LITERAL(137, 4574, 32), // "on_pushButton_MbReadFreq_clicked"
QT_MOC_LITERAL(138, 4607, 30), // "on_pushButton_MbReadPa_clicked"
QT_MOC_LITERAL(139, 4638, 38), // "on_pushButton_MbReadComboAEAl..."
QT_MOC_LITERAL(140, 4677, 36), // "on_pushButton_MbReadPosAEAll_..."
QT_MOC_LITERAL(141, 4714, 36), // "on_pushButton_MbReadNegAEAll_..."
QT_MOC_LITERAL(142, 4751, 31), // "on_pushButton_MbReadPFa_clicked"
QT_MOC_LITERAL(143, 4783, 44), // "on_pushButton_ReadLcdDisplayI..."
QT_MOC_LITERAL(144, 4828, 36), // "on_pushButton_AutoFreshStart_..."
QT_MOC_LITERAL(145, 4865, 35), // "on_pushButton_AutoFreshStop_c..."
QT_MOC_LITERAL(146, 4901, 35), // "on_pushButton_MbReadOVThres_c..."
QT_MOC_LITERAL(147, 4937, 34), // "on_pushButton_MbReadOVTime_cl..."
QT_MOC_LITERAL(148, 4972, 42), // "on_pushButton_WriteLcdInterva..."
QT_MOC_LITERAL(149, 5015, 35), // "on_pushButton_MbReadSysTime_c..."
QT_MOC_LITERAL(150, 5051, 38), // "on_pushButton_MbGetWinTime2Se..."
QT_MOC_LITERAL(151, 5090, 35), // "on_pushButton_MbManuTimeSet_c..."
QT_MOC_LITERAL(152, 5126, 34), // "on_pushButton_MbSetOVThres_cl..."
QT_MOC_LITERAL(153, 5161, 33), // "on_pushButton_MbSetOVTime_cli..."
QT_MOC_LITERAL(154, 5195, 33), // "on_pushButton_MbWriteAddr_cli..."
QT_MOC_LITERAL(155, 5229, 39), // "on_pushButton_MbReadSoftVersi..."
QT_MOC_LITERAL(156, 5269, 36), // "on_pushButton_MbReadSN_PCODE_..."
QT_MOC_LITERAL(157, 5306, 37), // "on_pushButton_MbWriteSN_PCODE..."
QT_MOC_LITERAL(158, 5344, 33), // "on_pushButton_MbSoftReset_cli..."
QT_MOC_LITERAL(159, 5378, 38), // "on_pushButton_MbClearAllEnerg..."
QT_MOC_LITERAL(160, 5417, 40), // "on_pushButton_MbStartCalibrat..."
QT_MOC_LITERAL(161, 5458, 39), // "on_pushButton_MbExitCalibrati..."
QT_MOC_LITERAL(162, 5498, 31), // "on_pushButton_MbCali_Ia_clicked"
QT_MOC_LITERAL(163, 5530, 31), // "on_pushButton_MbCali_Ua_clicked"
QT_MOC_LITERAL(164, 5562, 31), // "on_pushButton_MbCali_Pa_clicked"
QT_MOC_LITERAL(165, 5594, 36), // "on_pushButton_MbRs485ParaSet_..."
QT_MOC_LITERAL(166, 5631, 42), // "on_pushButton_MbCheckProtectS..."
QT_MOC_LITERAL(167, 5674, 39), // "on_pushButton_MbWriteProtectB..."
QT_MOC_LITERAL(168, 5714, 32), // "on_pushButton_MbReadTemp_clicked"
QT_MOC_LITERAL(169, 5747, 36), // "on_pushButton_MbReadSoftTime_..."
QT_MOC_LITERAL(170, 5784, 32), // "on_pushButton_MbReadLife_clicked"
QT_MOC_LITERAL(171, 5817, 36), // "on_pushButton_MbReadLeakLife_..."
QT_MOC_LITERAL(172, 5854, 39), // "on_pushButton_MbReadEventReco..."
QT_MOC_LITERAL(173, 5894, 33), // "on_pushButton_MbReadPosAE_cli..."
QT_MOC_LITERAL(174, 5928, 33), // "on_pushButton_MbReadNegAE_cli..."
QT_MOC_LITERAL(175, 5962, 33), // "on_pushButton_MbReadPosRE_cli..."
QT_MOC_LITERAL(176, 5996, 33), // "on_pushButton_MbReadNegRE_cli..."
QT_MOC_LITERAL(177, 6030, 37), // "on_pushButton_MbReadAppEnergy..."
QT_MOC_LITERAL(178, 6068, 34), // "on_pushButton_MbWritePosAE_cl..."
QT_MOC_LITERAL(179, 6103, 34), // "on_pushButton_MbWriteNegAE_cl..."
QT_MOC_LITERAL(180, 6138, 34), // "on_pushButton_MbWritePosRE_cl..."
QT_MOC_LITERAL(181, 6173, 34), // "on_pushButton_MbWriteNegRE_cl..."
QT_MOC_LITERAL(182, 6208, 38), // "on_pushButton_MbWriteAppEnerg..."
QT_MOC_LITERAL(183, 6247, 38), // "on_pushButton_MbReadComboNEAl..."
QT_MOC_LITERAL(184, 6286, 35), // "on_pushButton_MbReadAppEngA_c..."
QT_MOC_LITERAL(185, 6322, 31), // "on_pushButton_MbReadNPa_clicked"
QT_MOC_LITERAL(186, 6354, 33), // "on_pushButton_MbReadAppPa_cli..."
QT_MOC_LITERAL(187, 6388, 36), // "on_pushButton_MbReadPosRectE_..."
QT_MOC_LITERAL(188, 6425, 36), // "on_pushButton_MbReadNegRectE_..."
QT_MOC_LITERAL(189, 6462, 34), // "on_pushButton_AddCrc16Data_cl..."
QT_MOC_LITERAL(190, 6497, 35), // "on_pushButton_MBNewDataSend_c..."
QT_MOC_LITERAL(191, 6533, 37), // "on_pushButton_MBCaliStatement..."
QT_MOC_LITERAL(192, 6571, 31), // "on_pushButton_CycleSend_clicked"
QT_MOC_LITERAL(193, 6603, 35), // "on_pushButton_BroadcastOpen_c..."
QT_MOC_LITERAL(194, 6639, 38), // "on_pushButton_BroadcastTimeSe..."
QT_MOC_LITERAL(195, 6678, 35), // "on_pushButton_MbReadUVThres_c..."
QT_MOC_LITERAL(196, 6714, 34), // "on_pushButton_MbReadUVTime_cl..."
QT_MOC_LITERAL(197, 6749, 34), // "on_pushButton_MbSetUVThres_cl..."
QT_MOC_LITERAL(198, 6784, 33), // "on_pushButton_MbSetUVTime_cli..."
QT_MOC_LITERAL(199, 6818, 32), // "on_pushButton_BinAddInfo_clicked"
QT_MOC_LITERAL(200, 6851, 29), // "on_pushButton_Hex2bin_clicked"
QT_MOC_LITERAL(201, 6881, 38), // "on_pushButton_ReadCommCombine..."
QT_MOC_LITERAL(202, 6920, 39), // "on_pushButton_WriteCommCombin..."
QT_MOC_LITERAL(203, 6960, 30), // "on_pushButton_HexMerge_clicked"
QT_MOC_LITERAL(204, 6991, 27), // "on_pushButton_fHash_clicked"
QT_MOC_LITERAL(205, 7019, 26), // "on_pushButton_VOFA_clicked"
QT_MOC_LITERAL(206, 7046, 35), // "on_pushButton_BeyondCompare_c..."
QT_MOC_LITERAL(207, 7082, 32), // "on_pushButton_ModbusTool_clicked"
QT_MOC_LITERAL(208, 7115, 33), // "on_pushButton_RemoteLcdOn_cli..."
QT_MOC_LITERAL(209, 7149, 39), // "on_pushButton_DataVisualizati..."
QT_MOC_LITERAL(210, 7189, 36), // "on_pushButton_MbWriteInValue_..."
QT_MOC_LITERAL(211, 7226, 36) // "on_pushButton_MbPaOffsetCali_..."

    },
    "MainWindow\0on_pushButtonOpenSerial_clicked\0"
    "\0receiveInfo\0on_pushButtonSend1_clicked\0"
    "on_pushButtonClearSend_clicked\0"
    "on_pushButtonClearRec_clicked\0"
    "timerRecDataProcess\0timerAutoOCProcess\0"
    "timerCheckCloseProcess\0timerAutoFreshProcess\0"
    "timerRealTimeProcess\0timerCycleSend1Process\0"
    "timerCycleSend2Process\0timerCycleSend3Process\0"
    "timerCycleSend4Process\0timerCycleSend5Process\0"
    "timerCycleSend6Process\0"
    "on_pushButtonSend2_clicked\0"
    "on_pushButtonSend3_clicked\0"
    "on_pushButtonClearSend_2_clicked\0"
    "on_pushButtonClearSend_3_clicked\0"
    "on_pushButtonClearSendArea_clicked\0"
    "handleError\0QSerialPort::SerialPortError\0"
    "error\0on_pushButtonFreshPort_clicked\0"
    "on_pushButtonSend4_clicked\0"
    "on_pushButtonSend5_clicked\0"
    "on_pushButtonSend6_clicked\0"
    "on_pushButton_ReadVoltage_clicked\0"
    "on_pushButton_ReadFrequency_clicked\0"
    "on_pushButton_ReadMAC_clicked\0"
    "on_pushButton_ReadAsset_clicked\0"
    "on_pushButton_ReadProtectBit_clicked\0"
    "on_pushButton_WriteMAC_clicked\0"
    "on_pushButtonClearSend_4_clicked\0"
    "on_pushButtonClearSend_5_clicked\0"
    "on_pushButtonClearSend_6_clicked\0"
    "on_pushButton_WriteAssetCode_clicked\0"
    "on_pushButton_WriteProtectBit_clicked\0"
    "on_pushButton_ReadOV_Para_clicked\0"
    "on_pushButton_ReadUV_Para_clicked\0"
    "on_pushButton_ReadIsland_Para_clicked\0"
    "on_pushButton_DeviceInfo_clicked\0"
    "on_pushButton_WriteOV_Para_clicked\0"
    "on_pushButton_WriteUV_Para_clicked\0"
    "on_pushButton_WriteIsland_Para_clicked\0"
    "on_pushButton_RS485Set_clicked\0"
    "on_pushButton_RemoteOpen_clicked\0"
    "on_pushButton_RemoteCloseAllowed_clicked\0"
    "on_pushButton_RemoteClose_clicked\0"
    "on_pushButton_calibration_clicked\0"
    "on_pushButton_FCT_clicked\0"
    "on_pushButton_ReadRunWord_clicked\0"
    "on_pushButton_about_clicked\0"
    "on_pushButton_ReadAddr645_clicked\0"
    "on_pushButton_protocol645_clicked\0"
    "on_pushButton_CreateFrameSend_clicked\0"
    "on_pushButton_writeAddr645_clicked\0"
    "on_pushButton_ChangeBuadrate_clicked\0"
    "on_pushButton_ReadVolt645_clicked\0"
    "on_pushButton_ReadFreq645_clicked\0"
    "on_pushButton_ChangePWD_clicked\0"
    "on_pushButton_ReadDateTime_clicked\0"
    "on_pushButton_SoftwareVer645_clicked\0"
    "on_pushButton_HardVer645_clicked\0"
    "on_pushButton_AssetCode645_clicked\0"
    "on_pushButton_ProductionDate645_clicked\0"
    "on_pushButton_RunWord1_645_clicked\0"
    "on_pushButton_RunWord2_645_clicked\0"
    "on_pushButton_ReadCtrlWord1_clicked\0"
    "on_pushButton_ReadCtrlWord3_clicked\0"
    "on_pushButton_ReadCtrlWord7_clicked\0"
    "on_pushButton_WriteCtrlWord1_clicked\0"
    "on_pushButton_WriteCtrlWord3_clicked\0"
    "on_pushButton_WriteCtrlWord7_clicked\0"
    "on_pushButton_ReadOpenTime_clicked\0"
    "on_pushButton_WriteOpenTime_clicked\0"
    "on_pushButton_CancelOpen_clicked\0"
    "on_pushButton_ReadCloseTime_clicked\0"
    "on_pushButton_WriteCloseTime_clicked\0"
    "on_pushButton_CancelClose_clicked\0"
    "on_pushButton_GetSysDateTime_clicked\0"
    "on_pushButton_OtherFunction_clicked\0"
    "on_pushButton_ManualSetTime_clicked\0"
    "on_pushButton_ReadOpenTotalCnt_clicked\0"
    "on_pushButton_ClearTotalOpenCnt_clicked\0"
    "on_pushButton_OVOpenCnt_clicked\0"
    "on_pushButton_UVOpenCnt_clicked\0"
    "on_pushButton_LPOpenCnt_clicked\0"
    "on_pushButton_LVOpenCnt_clicked\0"
    "on_pushButton_RemoteOpenCnt_clicked\0"
    "on_pushButton_IslandOpenCnt_clicked\0"
    "on_pushButton_ManualOpenCnt_clicked\0"
    "on_pushButton_ClearOpenDataCnt_clicked\0"
    "on_pushButton_ClearProtectOpenCnt_clicked\0"
    "on_pushButton_TotalRunTime_clicked\0"
    "on_pushButton_ClearTotalRunTime_clicked\0"
    "on_pushButton_StateChangeNewAdd_clicked\0"
    "on_pushButton_ClearAll_clicked\0"
    "on_pushButton_ReadOVThreshold_clicked\0"
    "on_pushButton_ReadUVThreshold_clicked\0"
    "on_pushButton_ReadOVDelayTime_clicked\0"
    "on_pushButton_ReadUVDelayTime_clicked\0"
    "on_pushButton_WriteOVThreshold_clicked\0"
    "on_pushButton_WriteUVThreshold_clicked\0"
    "on_pushButton_WriteOVDelayTime_clicked\0"
    "on_pushButton_WriteUVDelayTime_clicked\0"
    "on_pushButton_ReadOVRecoverThres_clicked\0"
    "on_pushButton_ReadUVRecoverThres_clicked\0"
    "on_pushButton_ReadOVRecoverDelayTime_clicked\0"
    "on_pushButton_ReadUVRecoverDelayTime_clicked\0"
    "on_pushButton_WriteOVRecoverThres_clicked\0"
    "on_pushButton_WriteUVRecoverThres_clicked\0"
    "on_pushButton_WriteOVRecoverDelayTime_clicked\0"
    "on_pushButton_WriteUVRecoverDelayTime_clicked\0"
    "on_pushButton_Calibration645_clicked\0"
    "on_pushButton_MotorPosMCBPos_clicked\0"
    "on_pushButton_CheckProtectEvent_clicked\0"
    "on_pushButton_CheckStateChangeEvent_clicked\0"
    "on_pushButton_CheckIslandEvent_clicked\0"
    "on_pushButton_CheckSwitchingEvent_clicked\0"
    "on_pushButton_AutoCloseOpenStart_clicked\0"
    "on_pushButton_AutoCloseOpenStop_clicked\0"
    "on_pushButton_ResetConfig_clicked\0"
    "on_pushButton_GetDeviceType_clicked\0"
    "on_pushButtonOpenFile_clicked\0"
    "on_pushButtonUpgrate_clicked\0"
    "on_pushButtonStopUpgrade_clicked\0"
    "on_pushButton_MbReadAddr_clicked\0"
    "on_pushButton_ClearMbInfo_clicked\0"
    "on_pushButton_MbRemoteOpen_clicked\0"
    "on_pushButton_CloseLeakProtect_clicked\0"
    "on_pushButton_EnableLeakProtect_clicked\0"
    "on_pushButton_MbReadUa_clicked\0"
    "on_pushButton_MbReadIa_clicked\0"
    "on_pushButton_MbReadFreq_clicked\0"
    "on_pushButton_MbReadPa_clicked\0"
    "on_pushButton_MbReadComboAEAll_clicked\0"
    "on_pushButton_MbReadPosAEAll_clicked\0"
    "on_pushButton_MbReadNegAEAll_clicked\0"
    "on_pushButton_MbReadPFa_clicked\0"
    "on_pushButton_ReadLcdDisplayInterval_clicked\0"
    "on_pushButton_AutoFreshStart_clicked\0"
    "on_pushButton_AutoFreshStop_clicked\0"
    "on_pushButton_MbReadOVThres_clicked\0"
    "on_pushButton_MbReadOVTime_clicked\0"
    "on_pushButton_WriteLcdIntervalTime_clicked\0"
    "on_pushButton_MbReadSysTime_clicked\0"
    "on_pushButton_MbGetWinTime2Set_clicked\0"
    "on_pushButton_MbManuTimeSet_clicked\0"
    "on_pushButton_MbSetOVThres_clicked\0"
    "on_pushButton_MbSetOVTime_clicked\0"
    "on_pushButton_MbWriteAddr_clicked\0"
    "on_pushButton_MbReadSoftVersion_clicked\0"
    "on_pushButton_MbReadSN_PCODE_clicked\0"
    "on_pushButton_MbWriteSN_PCODE_clicked\0"
    "on_pushButton_MbSoftReset_clicked\0"
    "on_pushButton_MbClearAllEnergy_clicked\0"
    "on_pushButton_MbStartCalibration_clicked\0"
    "on_pushButton_MbExitCalibration_clicked\0"
    "on_pushButton_MbCali_Ia_clicked\0"
    "on_pushButton_MbCali_Ua_clicked\0"
    "on_pushButton_MbCali_Pa_clicked\0"
    "on_pushButton_MbRs485ParaSet_clicked\0"
    "on_pushButton_MbCheckProtectStatus_clicked\0"
    "on_pushButton_MbWriteProtectBit_clicked\0"
    "on_pushButton_MbReadTemp_clicked\0"
    "on_pushButton_MbReadSoftTime_clicked\0"
    "on_pushButton_MbReadLife_clicked\0"
    "on_pushButton_MbReadLeakLife_clicked\0"
    "on_pushButton_MbReadEventRecord_clicked\0"
    "on_pushButton_MbReadPosAE_clicked\0"
    "on_pushButton_MbReadNegAE_clicked\0"
    "on_pushButton_MbReadPosRE_clicked\0"
    "on_pushButton_MbReadNegRE_clicked\0"
    "on_pushButton_MbReadAppEnergy_clicked\0"
    "on_pushButton_MbWritePosAE_clicked\0"
    "on_pushButton_MbWriteNegAE_clicked\0"
    "on_pushButton_MbWritePosRE_clicked\0"
    "on_pushButton_MbWriteNegRE_clicked\0"
    "on_pushButton_MbWriteAppEnergy_clicked\0"
    "on_pushButton_MbReadComboNEAll_clicked\0"
    "on_pushButton_MbReadAppEngA_clicked\0"
    "on_pushButton_MbReadNPa_clicked\0"
    "on_pushButton_MbReadAppPa_clicked\0"
    "on_pushButton_MbReadPosRectE_clicked\0"
    "on_pushButton_MbReadNegRectE_clicked\0"
    "on_pushButton_AddCrc16Data_clicked\0"
    "on_pushButton_MBNewDataSend_clicked\0"
    "on_pushButton_MBCaliStatement_clicked\0"
    "on_pushButton_CycleSend_clicked\0"
    "on_pushButton_BroadcastOpen_clicked\0"
    "on_pushButton_BroadcastTimeSet_clicked\0"
    "on_pushButton_MbReadUVThres_clicked\0"
    "on_pushButton_MbReadUVTime_clicked\0"
    "on_pushButton_MbSetUVThres_clicked\0"
    "on_pushButton_MbSetUVTime_clicked\0"
    "on_pushButton_BinAddInfo_clicked\0"
    "on_pushButton_Hex2bin_clicked\0"
    "on_pushButton_ReadCommCombined_clicked\0"
    "on_pushButton_WriteCommCombined_clicked\0"
    "on_pushButton_HexMerge_clicked\0"
    "on_pushButton_fHash_clicked\0"
    "on_pushButton_VOFA_clicked\0"
    "on_pushButton_BeyondCompare_clicked\0"
    "on_pushButton_ModbusTool_clicked\0"
    "on_pushButton_RemoteLcdOn_clicked\0"
    "on_pushButton_DataVisualization_clicked\0"
    "on_pushButton_MbWriteInValue_clicked\0"
    "on_pushButton_MbPaOffsetCali_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
     208,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0, 1054,    2, 0x08 /* Private */,
       3,    0, 1055,    2, 0x08 /* Private */,
       4,    0, 1056,    2, 0x08 /* Private */,
       5,    0, 1057,    2, 0x08 /* Private */,
       6,    0, 1058,    2, 0x08 /* Private */,
       7,    0, 1059,    2, 0x08 /* Private */,
       8,    0, 1060,    2, 0x08 /* Private */,
       9,    0, 1061,    2, 0x08 /* Private */,
      10,    0, 1062,    2, 0x08 /* Private */,
      11,    0, 1063,    2, 0x08 /* Private */,
      12,    0, 1064,    2, 0x08 /* Private */,
      13,    0, 1065,    2, 0x08 /* Private */,
      14,    0, 1066,    2, 0x08 /* Private */,
      15,    0, 1067,    2, 0x08 /* Private */,
      16,    0, 1068,    2, 0x08 /* Private */,
      17,    0, 1069,    2, 0x08 /* Private */,
      18,    0, 1070,    2, 0x08 /* Private */,
      19,    0, 1071,    2, 0x08 /* Private */,
      20,    0, 1072,    2, 0x08 /* Private */,
      21,    0, 1073,    2, 0x08 /* Private */,
      22,    0, 1074,    2, 0x08 /* Private */,
      23,    1, 1075,    2, 0x08 /* Private */,
      26,    0, 1078,    2, 0x08 /* Private */,
      27,    0, 1079,    2, 0x08 /* Private */,
      28,    0, 1080,    2, 0x08 /* Private */,
      29,    0, 1081,    2, 0x08 /* Private */,
      30,    0, 1082,    2, 0x08 /* Private */,
      31,    0, 1083,    2, 0x08 /* Private */,
      32,    0, 1084,    2, 0x08 /* Private */,
      33,    0, 1085,    2, 0x08 /* Private */,
      34,    0, 1086,    2, 0x08 /* Private */,
      35,    0, 1087,    2, 0x08 /* Private */,
      36,    0, 1088,    2, 0x08 /* Private */,
      37,    0, 1089,    2, 0x08 /* Private */,
      38,    0, 1090,    2, 0x08 /* Private */,
      39,    0, 1091,    2, 0x08 /* Private */,
      40,    0, 1092,    2, 0x08 /* Private */,
      41,    0, 1093,    2, 0x08 /* Private */,
      42,    0, 1094,    2, 0x08 /* Private */,
      43,    0, 1095,    2, 0x08 /* Private */,
      44,    0, 1096,    2, 0x08 /* Private */,
      45,    0, 1097,    2, 0x08 /* Private */,
      46,    0, 1098,    2, 0x08 /* Private */,
      47,    0, 1099,    2, 0x08 /* Private */,
      48,    0, 1100,    2, 0x08 /* Private */,
      49,    0, 1101,    2, 0x08 /* Private */,
      50,    0, 1102,    2, 0x08 /* Private */,
      51,    0, 1103,    2, 0x08 /* Private */,
      52,    0, 1104,    2, 0x08 /* Private */,
      53,    0, 1105,    2, 0x08 /* Private */,
      54,    0, 1106,    2, 0x08 /* Private */,
      55,    0, 1107,    2, 0x08 /* Private */,
      56,    0, 1108,    2, 0x08 /* Private */,
      57,    0, 1109,    2, 0x08 /* Private */,
      58,    0, 1110,    2, 0x08 /* Private */,
      59,    0, 1111,    2, 0x08 /* Private */,
      60,    0, 1112,    2, 0x08 /* Private */,
      61,    0, 1113,    2, 0x08 /* Private */,
      62,    0, 1114,    2, 0x08 /* Private */,
      63,    0, 1115,    2, 0x08 /* Private */,
      64,    0, 1116,    2, 0x08 /* Private */,
      65,    0, 1117,    2, 0x08 /* Private */,
      66,    0, 1118,    2, 0x08 /* Private */,
      67,    0, 1119,    2, 0x08 /* Private */,
      68,    0, 1120,    2, 0x08 /* Private */,
      69,    0, 1121,    2, 0x08 /* Private */,
      70,    0, 1122,    2, 0x08 /* Private */,
      71,    0, 1123,    2, 0x08 /* Private */,
      72,    0, 1124,    2, 0x08 /* Private */,
      73,    0, 1125,    2, 0x08 /* Private */,
      74,    0, 1126,    2, 0x08 /* Private */,
      75,    0, 1127,    2, 0x08 /* Private */,
      76,    0, 1128,    2, 0x08 /* Private */,
      77,    0, 1129,    2, 0x08 /* Private */,
      78,    0, 1130,    2, 0x08 /* Private */,
      79,    0, 1131,    2, 0x08 /* Private */,
      80,    0, 1132,    2, 0x08 /* Private */,
      81,    0, 1133,    2, 0x08 /* Private */,
      82,    0, 1134,    2, 0x08 /* Private */,
      83,    0, 1135,    2, 0x08 /* Private */,
      84,    0, 1136,    2, 0x08 /* Private */,
      85,    0, 1137,    2, 0x08 /* Private */,
      86,    0, 1138,    2, 0x08 /* Private */,
      87,    0, 1139,    2, 0x08 /* Private */,
      88,    0, 1140,    2, 0x08 /* Private */,
      89,    0, 1141,    2, 0x08 /* Private */,
      90,    0, 1142,    2, 0x08 /* Private */,
      91,    0, 1143,    2, 0x08 /* Private */,
      92,    0, 1144,    2, 0x08 /* Private */,
      93,    0, 1145,    2, 0x08 /* Private */,
      94,    0, 1146,    2, 0x08 /* Private */,
      95,    0, 1147,    2, 0x08 /* Private */,
      96,    0, 1148,    2, 0x08 /* Private */,
      97,    0, 1149,    2, 0x08 /* Private */,
      98,    0, 1150,    2, 0x08 /* Private */,
      99,    0, 1151,    2, 0x08 /* Private */,
     100,    0, 1152,    2, 0x08 /* Private */,
     101,    0, 1153,    2, 0x08 /* Private */,
     102,    0, 1154,    2, 0x08 /* Private */,
     103,    0, 1155,    2, 0x08 /* Private */,
     104,    0, 1156,    2, 0x08 /* Private */,
     105,    0, 1157,    2, 0x08 /* Private */,
     106,    0, 1158,    2, 0x08 /* Private */,
     107,    0, 1159,    2, 0x08 /* Private */,
     108,    0, 1160,    2, 0x08 /* Private */,
     109,    0, 1161,    2, 0x08 /* Private */,
     110,    0, 1162,    2, 0x08 /* Private */,
     111,    0, 1163,    2, 0x08 /* Private */,
     112,    0, 1164,    2, 0x08 /* Private */,
     113,    0, 1165,    2, 0x08 /* Private */,
     114,    0, 1166,    2, 0x08 /* Private */,
     115,    0, 1167,    2, 0x08 /* Private */,
     116,    0, 1168,    2, 0x08 /* Private */,
     117,    0, 1169,    2, 0x08 /* Private */,
     118,    0, 1170,    2, 0x08 /* Private */,
     119,    0, 1171,    2, 0x08 /* Private */,
     120,    0, 1172,    2, 0x08 /* Private */,
     121,    0, 1173,    2, 0x08 /* Private */,
     122,    0, 1174,    2, 0x08 /* Private */,
     123,    0, 1175,    2, 0x08 /* Private */,
     124,    0, 1176,    2, 0x08 /* Private */,
     125,    0, 1177,    2, 0x08 /* Private */,
     126,    0, 1178,    2, 0x08 /* Private */,
     127,    0, 1179,    2, 0x08 /* Private */,
     128,    0, 1180,    2, 0x08 /* Private */,
     129,    0, 1181,    2, 0x08 /* Private */,
     130,    0, 1182,    2, 0x08 /* Private */,
     131,    0, 1183,    2, 0x08 /* Private */,
     132,    0, 1184,    2, 0x08 /* Private */,
     133,    0, 1185,    2, 0x08 /* Private */,
     134,    0, 1186,    2, 0x08 /* Private */,
     135,    0, 1187,    2, 0x08 /* Private */,
     136,    0, 1188,    2, 0x08 /* Private */,
     137,    0, 1189,    2, 0x08 /* Private */,
     138,    0, 1190,    2, 0x08 /* Private */,
     139,    0, 1191,    2, 0x08 /* Private */,
     140,    0, 1192,    2, 0x08 /* Private */,
     141,    0, 1193,    2, 0x08 /* Private */,
     142,    0, 1194,    2, 0x08 /* Private */,
     143,    0, 1195,    2, 0x08 /* Private */,
     144,    0, 1196,    2, 0x08 /* Private */,
     145,    0, 1197,    2, 0x08 /* Private */,
     146,    0, 1198,    2, 0x08 /* Private */,
     147,    0, 1199,    2, 0x08 /* Private */,
     148,    0, 1200,    2, 0x08 /* Private */,
     149,    0, 1201,    2, 0x08 /* Private */,
     150,    0, 1202,    2, 0x08 /* Private */,
     151,    0, 1203,    2, 0x08 /* Private */,
     152,    0, 1204,    2, 0x08 /* Private */,
     153,    0, 1205,    2, 0x08 /* Private */,
     154,    0, 1206,    2, 0x08 /* Private */,
     155,    0, 1207,    2, 0x08 /* Private */,
     156,    0, 1208,    2, 0x08 /* Private */,
     157,    0, 1209,    2, 0x08 /* Private */,
     158,    0, 1210,    2, 0x08 /* Private */,
     159,    0, 1211,    2, 0x08 /* Private */,
     160,    0, 1212,    2, 0x08 /* Private */,
     161,    0, 1213,    2, 0x08 /* Private */,
     162,    0, 1214,    2, 0x08 /* Private */,
     163,    0, 1215,    2, 0x08 /* Private */,
     164,    0, 1216,    2, 0x08 /* Private */,
     165,    0, 1217,    2, 0x08 /* Private */,
     166,    0, 1218,    2, 0x08 /* Private */,
     167,    0, 1219,    2, 0x08 /* Private */,
     168,    0, 1220,    2, 0x08 /* Private */,
     169,    0, 1221,    2, 0x08 /* Private */,
     170,    0, 1222,    2, 0x08 /* Private */,
     171,    0, 1223,    2, 0x08 /* Private */,
     172,    0, 1224,    2, 0x08 /* Private */,
     173,    0, 1225,    2, 0x08 /* Private */,
     174,    0, 1226,    2, 0x08 /* Private */,
     175,    0, 1227,    2, 0x08 /* Private */,
     176,    0, 1228,    2, 0x08 /* Private */,
     177,    0, 1229,    2, 0x08 /* Private */,
     178,    0, 1230,    2, 0x08 /* Private */,
     179,    0, 1231,    2, 0x08 /* Private */,
     180,    0, 1232,    2, 0x08 /* Private */,
     181,    0, 1233,    2, 0x08 /* Private */,
     182,    0, 1234,    2, 0x08 /* Private */,
     183,    0, 1235,    2, 0x08 /* Private */,
     184,    0, 1236,    2, 0x08 /* Private */,
     185,    0, 1237,    2, 0x08 /* Private */,
     186,    0, 1238,    2, 0x08 /* Private */,
     187,    0, 1239,    2, 0x08 /* Private */,
     188,    0, 1240,    2, 0x08 /* Private */,
     189,    0, 1241,    2, 0x08 /* Private */,
     190,    0, 1242,    2, 0x08 /* Private */,
     191,    0, 1243,    2, 0x08 /* Private */,
     192,    0, 1244,    2, 0x08 /* Private */,
     193,    0, 1245,    2, 0x08 /* Private */,
     194,    0, 1246,    2, 0x08 /* Private */,
     195,    0, 1247,    2, 0x08 /* Private */,
     196,    0, 1248,    2, 0x08 /* Private */,
     197,    0, 1249,    2, 0x08 /* Private */,
     198,    0, 1250,    2, 0x08 /* Private */,
     199,    0, 1251,    2, 0x08 /* Private */,
     200,    0, 1252,    2, 0x08 /* Private */,
     201,    0, 1253,    2, 0x08 /* Private */,
     202,    0, 1254,    2, 0x08 /* Private */,
     203,    0, 1255,    2, 0x08 /* Private */,
     204,    0, 1256,    2, 0x08 /* Private */,
     205,    0, 1257,    2, 0x08 /* Private */,
     206,    0, 1258,    2, 0x08 /* Private */,
     207,    0, 1259,    2, 0x08 /* Private */,
     208,    0, 1260,    2, 0x08 /* Private */,
     209,    0, 1261,    2, 0x08 /* Private */,
     210,    0, 1262,    2, 0x08 /* Private */,
     211,    0, 1263,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 24,   25,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_pushButtonOpenSerial_clicked(); break;
        case 1: _t->receiveInfo(); break;
        case 2: _t->on_pushButtonSend1_clicked(); break;
        case 3: _t->on_pushButtonClearSend_clicked(); break;
        case 4: _t->on_pushButtonClearRec_clicked(); break;
        case 5: _t->timerRecDataProcess(); break;
        case 6: _t->timerAutoOCProcess(); break;
        case 7: _t->timerCheckCloseProcess(); break;
        case 8: _t->timerAutoFreshProcess(); break;
        case 9: _t->timerRealTimeProcess(); break;
        case 10: _t->timerCycleSend1Process(); break;
        case 11: _t->timerCycleSend2Process(); break;
        case 12: _t->timerCycleSend3Process(); break;
        case 13: _t->timerCycleSend4Process(); break;
        case 14: _t->timerCycleSend5Process(); break;
        case 15: _t->timerCycleSend6Process(); break;
        case 16: _t->on_pushButtonSend2_clicked(); break;
        case 17: _t->on_pushButtonSend3_clicked(); break;
        case 18: _t->on_pushButtonClearSend_2_clicked(); break;
        case 19: _t->on_pushButtonClearSend_3_clicked(); break;
        case 20: _t->on_pushButtonClearSendArea_clicked(); break;
        case 21: _t->handleError((*reinterpret_cast< QSerialPort::SerialPortError(*)>(_a[1]))); break;
        case 22: _t->on_pushButtonFreshPort_clicked(); break;
        case 23: _t->on_pushButtonSend4_clicked(); break;
        case 24: _t->on_pushButtonSend5_clicked(); break;
        case 25: _t->on_pushButtonSend6_clicked(); break;
        case 26: _t->on_pushButton_ReadVoltage_clicked(); break;
        case 27: _t->on_pushButton_ReadFrequency_clicked(); break;
        case 28: _t->on_pushButton_ReadMAC_clicked(); break;
        case 29: _t->on_pushButton_ReadAsset_clicked(); break;
        case 30: _t->on_pushButton_ReadProtectBit_clicked(); break;
        case 31: _t->on_pushButton_WriteMAC_clicked(); break;
        case 32: _t->on_pushButtonClearSend_4_clicked(); break;
        case 33: _t->on_pushButtonClearSend_5_clicked(); break;
        case 34: _t->on_pushButtonClearSend_6_clicked(); break;
        case 35: _t->on_pushButton_WriteAssetCode_clicked(); break;
        case 36: _t->on_pushButton_WriteProtectBit_clicked(); break;
        case 37: _t->on_pushButton_ReadOV_Para_clicked(); break;
        case 38: _t->on_pushButton_ReadUV_Para_clicked(); break;
        case 39: _t->on_pushButton_ReadIsland_Para_clicked(); break;
        case 40: _t->on_pushButton_DeviceInfo_clicked(); break;
        case 41: _t->on_pushButton_WriteOV_Para_clicked(); break;
        case 42: _t->on_pushButton_WriteUV_Para_clicked(); break;
        case 43: _t->on_pushButton_WriteIsland_Para_clicked(); break;
        case 44: _t->on_pushButton_RS485Set_clicked(); break;
        case 45: _t->on_pushButton_RemoteOpen_clicked(); break;
        case 46: _t->on_pushButton_RemoteCloseAllowed_clicked(); break;
        case 47: _t->on_pushButton_RemoteClose_clicked(); break;
        case 48: _t->on_pushButton_calibration_clicked(); break;
        case 49: _t->on_pushButton_FCT_clicked(); break;
        case 50: _t->on_pushButton_ReadRunWord_clicked(); break;
        case 51: _t->on_pushButton_about_clicked(); break;
        case 52: _t->on_pushButton_ReadAddr645_clicked(); break;
        case 53: _t->on_pushButton_protocol645_clicked(); break;
        case 54: _t->on_pushButton_CreateFrameSend_clicked(); break;
        case 55: _t->on_pushButton_writeAddr645_clicked(); break;
        case 56: _t->on_pushButton_ChangeBuadrate_clicked(); break;
        case 57: _t->on_pushButton_ReadVolt645_clicked(); break;
        case 58: _t->on_pushButton_ReadFreq645_clicked(); break;
        case 59: _t->on_pushButton_ChangePWD_clicked(); break;
        case 60: _t->on_pushButton_ReadDateTime_clicked(); break;
        case 61: _t->on_pushButton_SoftwareVer645_clicked(); break;
        case 62: _t->on_pushButton_HardVer645_clicked(); break;
        case 63: _t->on_pushButton_AssetCode645_clicked(); break;
        case 64: _t->on_pushButton_ProductionDate645_clicked(); break;
        case 65: _t->on_pushButton_RunWord1_645_clicked(); break;
        case 66: _t->on_pushButton_RunWord2_645_clicked(); break;
        case 67: _t->on_pushButton_ReadCtrlWord1_clicked(); break;
        case 68: _t->on_pushButton_ReadCtrlWord3_clicked(); break;
        case 69: _t->on_pushButton_ReadCtrlWord7_clicked(); break;
        case 70: _t->on_pushButton_WriteCtrlWord1_clicked(); break;
        case 71: _t->on_pushButton_WriteCtrlWord3_clicked(); break;
        case 72: _t->on_pushButton_WriteCtrlWord7_clicked(); break;
        case 73: _t->on_pushButton_ReadOpenTime_clicked(); break;
        case 74: _t->on_pushButton_WriteOpenTime_clicked(); break;
        case 75: _t->on_pushButton_CancelOpen_clicked(); break;
        case 76: _t->on_pushButton_ReadCloseTime_clicked(); break;
        case 77: _t->on_pushButton_WriteCloseTime_clicked(); break;
        case 78: _t->on_pushButton_CancelClose_clicked(); break;
        case 79: _t->on_pushButton_GetSysDateTime_clicked(); break;
        case 80: _t->on_pushButton_OtherFunction_clicked(); break;
        case 81: _t->on_pushButton_ManualSetTime_clicked(); break;
        case 82: _t->on_pushButton_ReadOpenTotalCnt_clicked(); break;
        case 83: _t->on_pushButton_ClearTotalOpenCnt_clicked(); break;
        case 84: _t->on_pushButton_OVOpenCnt_clicked(); break;
        case 85: _t->on_pushButton_UVOpenCnt_clicked(); break;
        case 86: _t->on_pushButton_LPOpenCnt_clicked(); break;
        case 87: _t->on_pushButton_LVOpenCnt_clicked(); break;
        case 88: _t->on_pushButton_RemoteOpenCnt_clicked(); break;
        case 89: _t->on_pushButton_IslandOpenCnt_clicked(); break;
        case 90: _t->on_pushButton_ManualOpenCnt_clicked(); break;
        case 91: _t->on_pushButton_ClearOpenDataCnt_clicked(); break;
        case 92: _t->on_pushButton_ClearProtectOpenCnt_clicked(); break;
        case 93: _t->on_pushButton_TotalRunTime_clicked(); break;
        case 94: _t->on_pushButton_ClearTotalRunTime_clicked(); break;
        case 95: _t->on_pushButton_StateChangeNewAdd_clicked(); break;
        case 96: _t->on_pushButton_ClearAll_clicked(); break;
        case 97: _t->on_pushButton_ReadOVThreshold_clicked(); break;
        case 98: _t->on_pushButton_ReadUVThreshold_clicked(); break;
        case 99: _t->on_pushButton_ReadOVDelayTime_clicked(); break;
        case 100: _t->on_pushButton_ReadUVDelayTime_clicked(); break;
        case 101: _t->on_pushButton_WriteOVThreshold_clicked(); break;
        case 102: _t->on_pushButton_WriteUVThreshold_clicked(); break;
        case 103: _t->on_pushButton_WriteOVDelayTime_clicked(); break;
        case 104: _t->on_pushButton_WriteUVDelayTime_clicked(); break;
        case 105: _t->on_pushButton_ReadOVRecoverThres_clicked(); break;
        case 106: _t->on_pushButton_ReadUVRecoverThres_clicked(); break;
        case 107: _t->on_pushButton_ReadOVRecoverDelayTime_clicked(); break;
        case 108: _t->on_pushButton_ReadUVRecoverDelayTime_clicked(); break;
        case 109: _t->on_pushButton_WriteOVRecoverThres_clicked(); break;
        case 110: _t->on_pushButton_WriteUVRecoverThres_clicked(); break;
        case 111: _t->on_pushButton_WriteOVRecoverDelayTime_clicked(); break;
        case 112: _t->on_pushButton_WriteUVRecoverDelayTime_clicked(); break;
        case 113: _t->on_pushButton_Calibration645_clicked(); break;
        case 114: _t->on_pushButton_MotorPosMCBPos_clicked(); break;
        case 115: _t->on_pushButton_CheckProtectEvent_clicked(); break;
        case 116: _t->on_pushButton_CheckStateChangeEvent_clicked(); break;
        case 117: _t->on_pushButton_CheckIslandEvent_clicked(); break;
        case 118: _t->on_pushButton_CheckSwitchingEvent_clicked(); break;
        case 119: _t->on_pushButton_AutoCloseOpenStart_clicked(); break;
        case 120: _t->on_pushButton_AutoCloseOpenStop_clicked(); break;
        case 121: _t->on_pushButton_ResetConfig_clicked(); break;
        case 122: _t->on_pushButton_GetDeviceType_clicked(); break;
        case 123: _t->on_pushButtonOpenFile_clicked(); break;
        case 124: _t->on_pushButtonUpgrate_clicked(); break;
        case 125: _t->on_pushButtonStopUpgrade_clicked(); break;
        case 126: _t->on_pushButton_MbReadAddr_clicked(); break;
        case 127: _t->on_pushButton_ClearMbInfo_clicked(); break;
        case 128: _t->on_pushButton_MbRemoteOpen_clicked(); break;
        case 129: _t->on_pushButton_CloseLeakProtect_clicked(); break;
        case 130: _t->on_pushButton_EnableLeakProtect_clicked(); break;
        case 131: _t->on_pushButton_MbReadUa_clicked(); break;
        case 132: _t->on_pushButton_MbReadIa_clicked(); break;
        case 133: _t->on_pushButton_MbReadFreq_clicked(); break;
        case 134: _t->on_pushButton_MbReadPa_clicked(); break;
        case 135: _t->on_pushButton_MbReadComboAEAll_clicked(); break;
        case 136: _t->on_pushButton_MbReadPosAEAll_clicked(); break;
        case 137: _t->on_pushButton_MbReadNegAEAll_clicked(); break;
        case 138: _t->on_pushButton_MbReadPFa_clicked(); break;
        case 139: _t->on_pushButton_ReadLcdDisplayInterval_clicked(); break;
        case 140: _t->on_pushButton_AutoFreshStart_clicked(); break;
        case 141: _t->on_pushButton_AutoFreshStop_clicked(); break;
        case 142: _t->on_pushButton_MbReadOVThres_clicked(); break;
        case 143: _t->on_pushButton_MbReadOVTime_clicked(); break;
        case 144: _t->on_pushButton_WriteLcdIntervalTime_clicked(); break;
        case 145: _t->on_pushButton_MbReadSysTime_clicked(); break;
        case 146: _t->on_pushButton_MbGetWinTime2Set_clicked(); break;
        case 147: _t->on_pushButton_MbManuTimeSet_clicked(); break;
        case 148: _t->on_pushButton_MbSetOVThres_clicked(); break;
        case 149: _t->on_pushButton_MbSetOVTime_clicked(); break;
        case 150: _t->on_pushButton_MbWriteAddr_clicked(); break;
        case 151: _t->on_pushButton_MbReadSoftVersion_clicked(); break;
        case 152: _t->on_pushButton_MbReadSN_PCODE_clicked(); break;
        case 153: _t->on_pushButton_MbWriteSN_PCODE_clicked(); break;
        case 154: _t->on_pushButton_MbSoftReset_clicked(); break;
        case 155: _t->on_pushButton_MbClearAllEnergy_clicked(); break;
        case 156: _t->on_pushButton_MbStartCalibration_clicked(); break;
        case 157: _t->on_pushButton_MbExitCalibration_clicked(); break;
        case 158: _t->on_pushButton_MbCali_Ia_clicked(); break;
        case 159: _t->on_pushButton_MbCali_Ua_clicked(); break;
        case 160: _t->on_pushButton_MbCali_Pa_clicked(); break;
        case 161: _t->on_pushButton_MbRs485ParaSet_clicked(); break;
        case 162: _t->on_pushButton_MbCheckProtectStatus_clicked(); break;
        case 163: _t->on_pushButton_MbWriteProtectBit_clicked(); break;
        case 164: _t->on_pushButton_MbReadTemp_clicked(); break;
        case 165: _t->on_pushButton_MbReadSoftTime_clicked(); break;
        case 166: _t->on_pushButton_MbReadLife_clicked(); break;
        case 167: _t->on_pushButton_MbReadLeakLife_clicked(); break;
        case 168: _t->on_pushButton_MbReadEventRecord_clicked(); break;
        case 169: _t->on_pushButton_MbReadPosAE_clicked(); break;
        case 170: _t->on_pushButton_MbReadNegAE_clicked(); break;
        case 171: _t->on_pushButton_MbReadPosRE_clicked(); break;
        case 172: _t->on_pushButton_MbReadNegRE_clicked(); break;
        case 173: _t->on_pushButton_MbReadAppEnergy_clicked(); break;
        case 174: _t->on_pushButton_MbWritePosAE_clicked(); break;
        case 175: _t->on_pushButton_MbWriteNegAE_clicked(); break;
        case 176: _t->on_pushButton_MbWritePosRE_clicked(); break;
        case 177: _t->on_pushButton_MbWriteNegRE_clicked(); break;
        case 178: _t->on_pushButton_MbWriteAppEnergy_clicked(); break;
        case 179: _t->on_pushButton_MbReadComboNEAll_clicked(); break;
        case 180: _t->on_pushButton_MbReadAppEngA_clicked(); break;
        case 181: _t->on_pushButton_MbReadNPa_clicked(); break;
        case 182: _t->on_pushButton_MbReadAppPa_clicked(); break;
        case 183: _t->on_pushButton_MbReadPosRectE_clicked(); break;
        case 184: _t->on_pushButton_MbReadNegRectE_clicked(); break;
        case 185: _t->on_pushButton_AddCrc16Data_clicked(); break;
        case 186: _t->on_pushButton_MBNewDataSend_clicked(); break;
        case 187: _t->on_pushButton_MBCaliStatement_clicked(); break;
        case 188: _t->on_pushButton_CycleSend_clicked(); break;
        case 189: _t->on_pushButton_BroadcastOpen_clicked(); break;
        case 190: _t->on_pushButton_BroadcastTimeSet_clicked(); break;
        case 191: _t->on_pushButton_MbReadUVThres_clicked(); break;
        case 192: _t->on_pushButton_MbReadUVTime_clicked(); break;
        case 193: _t->on_pushButton_MbSetUVThres_clicked(); break;
        case 194: _t->on_pushButton_MbSetUVTime_clicked(); break;
        case 195: _t->on_pushButton_BinAddInfo_clicked(); break;
        case 196: _t->on_pushButton_Hex2bin_clicked(); break;
        case 197: _t->on_pushButton_ReadCommCombined_clicked(); break;
        case 198: _t->on_pushButton_WriteCommCombined_clicked(); break;
        case 199: _t->on_pushButton_HexMerge_clicked(); break;
        case 200: _t->on_pushButton_fHash_clicked(); break;
        case 201: _t->on_pushButton_VOFA_clicked(); break;
        case 202: _t->on_pushButton_BeyondCompare_clicked(); break;
        case 203: _t->on_pushButton_ModbusTool_clicked(); break;
        case 204: _t->on_pushButton_RemoteLcdOn_clicked(); break;
        case 205: _t->on_pushButton_DataVisualization_clicked(); break;
        case 206: _t->on_pushButton_MbWriteInValue_clicked(); break;
        case 207: _t->on_pushButton_MbPaOffsetCali_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 208)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 208;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 208)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 208;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
