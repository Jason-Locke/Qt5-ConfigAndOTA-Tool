/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionlzlei_chint_com;
    QWidget *centralwidget;
    QTextEdit *textEditSend;
    QTextEdit *recData;
    QTabWidget *tabWidget;
    QWidget *tab_4;
    QGroupBox *groupBox_OMCA_OTA;
    QPushButton *pushButtonUpgrate;
    QProgressBar *progressBar;
    QPushButton *pushButtonStopUpgrade;
    QPushButton *pushButtonReadVerSelfDefine;
    QLabel *label_3;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLineEdit *lineEdit_ReadVerSelfDef;
    QLineEdit *lineEdit_FrameHeadSelfDef;
    QLineEdit *lineEdit_FrameTailSelfDef;
    QLineEdit *lineEdit_FrameCmdStartSelfDef;
    QLineEdit *lineEdit_FrameCmdOTASelfDef;
    QLineEdit *lineEdit_FrameCmdEndSelfDef;
    QLabel *label_9;
    QLineEdit *lineEdit_FrameReadVerCmdSelfDef;
    QLineEdit *lineEdit_OtaFileSize;
    QLineEdit *lineEditBinFliePath;
    QLabel *label_111;
    QPushButton *pushButtonOpenFile;
    QTextEdit *textEdit_OTAlog;
    QGroupBox *groupBox_OtaByModbusRTU;
    QProgressBar *progressBar_MB;
    QPushButton *pushButton_UpgradeMB;
    QPushButton *pushButton_StopUpgradeMB;
    QPushButton *pushButtonReadVerMB;
    QLineEdit *lineEdit_ReadVerMB;
    QLabel *label_16;
    QLineEdit *lineEdit_InputMBAddr;
    QGroupBox *groupBox_OtaByDlt698;
    QProgressBar *progressBar_698;
    QPushButton *pushButton_Upgrade698;
    QPushButton *pushButton_StopUpgrade698;
    QPushButton *pushButtonReadVer698;
    QLineEdit *lineEdit_ReadVer698;
    QLabel *label_17;
    QLineEdit *lineEdit_Input698Addr;
    QGroupBox *groupBox_OtaByDlt645;
    QPushButton *pushButton_Upgrade645;
    QProgressBar *progressBar_645;
    QPushButton *pushButton_StopUpgrade645;
    QPushButton *pushButtonReadVer645;
    QLineEdit *lineEdit_ReadVer645;
    QLabel *label_18;
    QLineEdit *lineEdit_Input645Addr;
    QLabel *label_11;
    QLineEdit *lineEdit_GetCRC32Value;
    QLabel *label_12;
    QLineEdit *lineEdit_CalcCRC32Value;
    QFrame *line;
    QLabel *label_13;
    QLineEdit *lineEdit_GetBinVersion;
    QLabel *label_14;
    QLineEdit *lineEdit_GetRate;
    QLabel *label_15;
    QLineEdit *lineEdit_GetPole;
    QPushButton *pushButton_ClearOTAlog;
    QGroupBox *groupBox_BinProcess;
    QPushButton *pushButton_BinAddInfo;
    QPushButton *pushButton_Hex2bin;
    QWidget *layoutWidget;
    QGridLayout *gridLayout_10;
    QCheckBox *checkBoxHexReceive;
    QCheckBox *checkBox_TimeTag;
    QPushButton *pushButtonClearRec;
    QWidget *layoutWidget1;
    QGridLayout *gridLayout_11;
    QCheckBox *checkBoxHexSend;
    QLineEdit *lineEditSend_Text1;
    QLineEdit *lineEditSend;
    QPushButton *pushButtonSend1;
    QPushButton *pushButtonClearSend;
    QCheckBox *checkBoxHexSend_2;
    QLineEdit *lineEditSend_Text2;
    QLineEdit *lineEditSend2;
    QPushButton *pushButtonSend2;
    QPushButton *pushButtonClearSend_2;
    QCheckBox *checkBoxHexSend_3;
    QLineEdit *lineEditSend_Text3;
    QLineEdit *lineEditSend3;
    QPushButton *pushButtonSend3;
    QPushButton *pushButtonClearSend_3;
    QCheckBox *checkBoxHexSend_4;
    QLineEdit *lineEditSend_Text4;
    QLineEdit *lineEditSend4;
    QPushButton *pushButtonSend4;
    QPushButton *pushButtonClearSend_4;
    QCheckBox *checkBoxHexSend_5;
    QLineEdit *lineEditSend_Text5;
    QLineEdit *lineEditSend5;
    QPushButton *pushButtonSend5;
    QPushButton *pushButtonClearSend_5;
    QCheckBox *checkBoxHexSend_6;
    QLineEdit *lineEditSend_Text6;
    QLineEdit *lineEditSend6;
    QPushButton *pushButtonSend6;
    QPushButton *pushButtonClearSend_6;
    QFrame *line_3;
    QWidget *layoutWidget2;
    QGridLayout *gridLayout_12;
    QCheckBox *checkBox_TimeTagSend;
    QPushButton *pushButtonClearSendArea;
    QWidget *layoutWidget3;
    QGridLayout *gridLayout_9;
    QLabel *label;
    QComboBox *comboBox_serialNum;
    QLabel *label_2;
    QComboBox *comboBox_baudrate;
    QLabel *label_4;
    QComboBox *comboBoxParity;
    QPushButton *pushButtonOpenSerial;
    QPushButton *pushButtonFreshPort;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_about;
    QWidget *layoutWidget4;
    QHBoxLayout *horizontalLayout;
    QLabel *label_45;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label_46;
    QStatusBar *statusbar;
    QMenuBar *menubar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(919, 972);
        actionlzlei_chint_com = new QAction(MainWindow);
        actionlzlei_chint_com->setObjectName(QString::fromUtf8("actionlzlei_chint_com"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        textEditSend = new QTextEdit(centralwidget);
        textEditSend->setObjectName(QString::fromUtf8("textEditSend"));
        textEditSend->setGeometry(QRect(11, 41, 301, 111));
        textEditSend->setMaximumSize(QSize(548, 16777215));
        textEditSend->setReadOnly(true);
        recData = new QTextEdit(centralwidget);
        recData->setObjectName(QString::fromUtf8("recData"));
        recData->setGeometry(QRect(313, 41, 601, 111));
        recData->setMinimumSize(QSize(601, 0));
        recData->setReadOnly(true);
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setEnabled(true);
        tabWidget->setGeometry(QRect(10, 200, 901, 521));
        QFont font;
        font.setPointSize(9);
        tabWidget->setFont(font);
        tab_4 = new QWidget();
        tab_4->setObjectName(QString::fromUtf8("tab_4"));
        groupBox_OMCA_OTA = new QGroupBox(tab_4);
        groupBox_OMCA_OTA->setObjectName(QString::fromUtf8("groupBox_OMCA_OTA"));
        groupBox_OMCA_OTA->setGeometry(QRect(10, 360, 611, 121));
        pushButtonUpgrate = new QPushButton(groupBox_OMCA_OTA);
        pushButtonUpgrate->setObjectName(QString::fromUtf8("pushButtonUpgrate"));
        pushButtonUpgrate->setGeometry(QRect(10, 90, 51, 21));
        progressBar = new QProgressBar(groupBox_OMCA_OTA);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setGeometry(QRect(70, 90, 421, 23));
        progressBar->setValue(24);
        pushButtonStopUpgrade = new QPushButton(groupBox_OMCA_OTA);
        pushButtonStopUpgrade->setObjectName(QString::fromUtf8("pushButtonStopUpgrade"));
        pushButtonStopUpgrade->setGeometry(QRect(500, 90, 71, 21));
        pushButtonReadVerSelfDefine = new QPushButton(groupBox_OMCA_OTA);
        pushButtonReadVerSelfDefine->setObjectName(QString::fromUtf8("pushButtonReadVerSelfDefine"));
        pushButtonReadVerSelfDefine->setGeometry(QRect(440, 50, 71, 20));
        label_3 = new QLabel(groupBox_OMCA_OTA);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 30, 91, 16));
        label_5 = new QLabel(groupBox_OMCA_OTA);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 50, 91, 16));
        label_6 = new QLabel(groupBox_OMCA_OTA);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(190, 20, 111, 21));
        label_7 = new QLabel(groupBox_OMCA_OTA);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(190, 40, 111, 21));
        label_8 = new QLabel(groupBox_OMCA_OTA);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(190, 60, 111, 21));
        lineEdit_ReadVerSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_ReadVerSelfDef->setObjectName(QString::fromUtf8("lineEdit_ReadVerSelfDef"));
        lineEdit_ReadVerSelfDef->setGeometry(QRect(520, 50, 81, 20));
        lineEdit_ReadVerSelfDef->setReadOnly(true);
        lineEdit_FrameHeadSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameHeadSelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameHeadSelfDef"));
        lineEdit_FrameHeadSelfDef->setGeometry(QRect(100, 30, 61, 20));
        lineEdit_FrameTailSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameTailSelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameTailSelfDef"));
        lineEdit_FrameTailSelfDef->setGeometry(QRect(100, 50, 61, 20));
        lineEdit_FrameCmdStartSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameCmdStartSelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameCmdStartSelfDef"));
        lineEdit_FrameCmdStartSelfDef->setGeometry(QRect(310, 20, 51, 20));
        lineEdit_FrameCmdOTASelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameCmdOTASelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameCmdOTASelfDef"));
        lineEdit_FrameCmdOTASelfDef->setGeometry(QRect(310, 40, 51, 20));
        lineEdit_FrameCmdEndSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameCmdEndSelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameCmdEndSelfDef"));
        lineEdit_FrameCmdEndSelfDef->setGeometry(QRect(310, 60, 51, 20));
        label_9 = new QLabel(groupBox_OMCA_OTA);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(410, 30, 141, 21));
        lineEdit_FrameReadVerCmdSelfDef = new QLineEdit(groupBox_OMCA_OTA);
        lineEdit_FrameReadVerCmdSelfDef->setObjectName(QString::fromUtf8("lineEdit_FrameReadVerCmdSelfDef"));
        lineEdit_FrameReadVerCmdSelfDef->setGeometry(QRect(550, 30, 51, 20));
        lineEdit_OtaFileSize = new QLineEdit(tab_4);
        lineEdit_OtaFileSize->setObjectName(QString::fromUtf8("lineEdit_OtaFileSize"));
        lineEdit_OtaFileSize->setGeometry(QRect(110, 40, 61, 20));
        lineEditBinFliePath = new QLineEdit(tab_4);
        lineEditBinFliePath->setObjectName(QString::fromUtf8("lineEditBinFliePath"));
        lineEditBinFliePath->setGeometry(QRect(90, 10, 801, 20));
        label_111 = new QLabel(tab_4);
        label_111->setObjectName(QString::fromUtf8("label_111"));
        label_111->setGeometry(QRect(10, 40, 91, 21));
        pushButtonOpenFile = new QPushButton(tab_4);
        pushButtonOpenFile->setObjectName(QString::fromUtf8("pushButtonOpenFile"));
        pushButtonOpenFile->setGeometry(QRect(6, 10, 81, 21));
        pushButtonOpenFile->setBaseSize(QSize(0, 0));
        textEdit_OTAlog = new QTextEdit(tab_4);
        textEdit_OTAlog->setObjectName(QString::fromUtf8("textEdit_OTAlog"));
        textEdit_OTAlog->setGeometry(QRect(460, 70, 431, 301));
        textEdit_OTAlog->setReadOnly(true);
        groupBox_OtaByModbusRTU = new QGroupBox(tab_4);
        groupBox_OtaByModbusRTU->setObjectName(QString::fromUtf8("groupBox_OtaByModbusRTU"));
        groupBox_OtaByModbusRTU->setGeometry(QRect(10, 80, 441, 81));
        progressBar_MB = new QProgressBar(groupBox_OtaByModbusRTU);
        progressBar_MB->setObjectName(QString::fromUtf8("progressBar_MB"));
        progressBar_MB->setGeometry(QRect(70, 50, 281, 23));
        progressBar_MB->setValue(0);
        pushButton_UpgradeMB = new QPushButton(groupBox_OtaByModbusRTU);
        pushButton_UpgradeMB->setObjectName(QString::fromUtf8("pushButton_UpgradeMB"));
        pushButton_UpgradeMB->setGeometry(QRect(10, 50, 51, 20));
        pushButton_StopUpgradeMB = new QPushButton(groupBox_OtaByModbusRTU);
        pushButton_StopUpgradeMB->setObjectName(QString::fromUtf8("pushButton_StopUpgradeMB"));
        pushButton_StopUpgradeMB->setGeometry(QRect(360, 50, 71, 20));
        pushButtonReadVerMB = new QPushButton(groupBox_OtaByModbusRTU);
        pushButtonReadVerMB->setObjectName(QString::fromUtf8("pushButtonReadVerMB"));
        pushButtonReadVerMB->setGeometry(QRect(270, 20, 71, 20));
        lineEdit_ReadVerMB = new QLineEdit(groupBox_OtaByModbusRTU);
        lineEdit_ReadVerMB->setObjectName(QString::fromUtf8("lineEdit_ReadVerMB"));
        lineEdit_ReadVerMB->setGeometry(QRect(350, 20, 81, 20));
        lineEdit_ReadVerMB->setReadOnly(true);
        label_16 = new QLabel(groupBox_OtaByModbusRTU);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(10, 20, 31, 21));
        lineEdit_InputMBAddr = new QLineEdit(groupBox_OtaByModbusRTU);
        lineEdit_InputMBAddr->setObjectName(QString::fromUtf8("lineEdit_InputMBAddr"));
        lineEdit_InputMBAddr->setGeometry(QRect(40, 20, 41, 20));
        groupBox_OtaByDlt698 = new QGroupBox(tab_4);
        groupBox_OtaByDlt698->setObjectName(QString::fromUtf8("groupBox_OtaByDlt698"));
        groupBox_OtaByDlt698->setGeometry(QRect(10, 170, 441, 81));
        progressBar_698 = new QProgressBar(groupBox_OtaByDlt698);
        progressBar_698->setObjectName(QString::fromUtf8("progressBar_698"));
        progressBar_698->setGeometry(QRect(70, 50, 281, 23));
        progressBar_698->setValue(0);
        pushButton_Upgrade698 = new QPushButton(groupBox_OtaByDlt698);
        pushButton_Upgrade698->setObjectName(QString::fromUtf8("pushButton_Upgrade698"));
        pushButton_Upgrade698->setGeometry(QRect(10, 50, 51, 20));
        pushButton_StopUpgrade698 = new QPushButton(groupBox_OtaByDlt698);
        pushButton_StopUpgrade698->setObjectName(QString::fromUtf8("pushButton_StopUpgrade698"));
        pushButton_StopUpgrade698->setGeometry(QRect(360, 50, 71, 20));
        pushButtonReadVer698 = new QPushButton(groupBox_OtaByDlt698);
        pushButtonReadVer698->setObjectName(QString::fromUtf8("pushButtonReadVer698"));
        pushButtonReadVer698->setGeometry(QRect(270, 20, 71, 20));
        lineEdit_ReadVer698 = new QLineEdit(groupBox_OtaByDlt698);
        lineEdit_ReadVer698->setObjectName(QString::fromUtf8("lineEdit_ReadVer698"));
        lineEdit_ReadVer698->setGeometry(QRect(350, 20, 81, 20));
        lineEdit_ReadVer698->setReadOnly(true);
        label_17 = new QLabel(groupBox_OtaByDlt698);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(10, 20, 31, 21));
        lineEdit_Input698Addr = new QLineEdit(groupBox_OtaByDlt698);
        lineEdit_Input698Addr->setObjectName(QString::fromUtf8("lineEdit_Input698Addr"));
        lineEdit_Input698Addr->setGeometry(QRect(40, 20, 41, 20));
        groupBox_OtaByDlt645 = new QGroupBox(tab_4);
        groupBox_OtaByDlt645->setObjectName(QString::fromUtf8("groupBox_OtaByDlt645"));
        groupBox_OtaByDlt645->setGeometry(QRect(10, 260, 441, 81));
        pushButton_Upgrade645 = new QPushButton(groupBox_OtaByDlt645);
        pushButton_Upgrade645->setObjectName(QString::fromUtf8("pushButton_Upgrade645"));
        pushButton_Upgrade645->setGeometry(QRect(10, 50, 51, 20));
        progressBar_645 = new QProgressBar(groupBox_OtaByDlt645);
        progressBar_645->setObjectName(QString::fromUtf8("progressBar_645"));
        progressBar_645->setGeometry(QRect(70, 50, 281, 23));
        progressBar_645->setValue(0);
        pushButton_StopUpgrade645 = new QPushButton(groupBox_OtaByDlt645);
        pushButton_StopUpgrade645->setObjectName(QString::fromUtf8("pushButton_StopUpgrade645"));
        pushButton_StopUpgrade645->setGeometry(QRect(360, 50, 71, 20));
        pushButtonReadVer645 = new QPushButton(groupBox_OtaByDlt645);
        pushButtonReadVer645->setObjectName(QString::fromUtf8("pushButtonReadVer645"));
        pushButtonReadVer645->setGeometry(QRect(270, 20, 71, 20));
        lineEdit_ReadVer645 = new QLineEdit(groupBox_OtaByDlt645);
        lineEdit_ReadVer645->setObjectName(QString::fromUtf8("lineEdit_ReadVer645"));
        lineEdit_ReadVer645->setGeometry(QRect(350, 20, 81, 20));
        lineEdit_ReadVer645->setReadOnly(true);
        label_18 = new QLabel(groupBox_OtaByDlt645);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(10, 20, 31, 21));
        lineEdit_Input645Addr = new QLineEdit(groupBox_OtaByDlt645);
        lineEdit_Input645Addr->setObjectName(QString::fromUtf8("lineEdit_Input645Addr"));
        lineEdit_Input645Addr->setGeometry(QRect(40, 20, 41, 20));
        label_11 = new QLabel(tab_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(180, 40, 101, 21));
        lineEdit_GetCRC32Value = new QLineEdit(tab_4);
        lineEdit_GetCRC32Value->setObjectName(QString::fromUtf8("lineEdit_GetCRC32Value"));
        lineEdit_GetCRC32Value->setGeometry(QRect(280, 40, 71, 20));
        label_12 = new QLabel(tab_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(360, 40, 101, 21));
        lineEdit_CalcCRC32Value = new QLineEdit(tab_4);
        lineEdit_CalcCRC32Value->setObjectName(QString::fromUtf8("lineEdit_CalcCRC32Value"));
        lineEdit_CalcCRC32Value->setGeometry(QRect(460, 40, 81, 20));
        line = new QFrame(tab_4);
        line->setObjectName(QString::fromUtf8("line"));
        line->setGeometry(QRect(0, 63, 461, 16));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);
        label_13 = new QLabel(tab_4);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(550, 40, 61, 21));
        lineEdit_GetBinVersion = new QLineEdit(tab_4);
        lineEdit_GetBinVersion->setObjectName(QString::fromUtf8("lineEdit_GetBinVersion"));
        lineEdit_GetBinVersion->setGeometry(QRect(610, 40, 61, 20));
        label_14 = new QLabel(tab_4);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(680, 40, 31, 21));
        lineEdit_GetRate = new QLineEdit(tab_4);
        lineEdit_GetRate->setObjectName(QString::fromUtf8("lineEdit_GetRate"));
        lineEdit_GetRate->setGeometry(QRect(710, 40, 41, 20));
        label_15 = new QLabel(tab_4);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(760, 40, 31, 21));
        lineEdit_GetPole = new QLineEdit(tab_4);
        lineEdit_GetPole->setObjectName(QString::fromUtf8("lineEdit_GetPole"));
        lineEdit_GetPole->setGeometry(QRect(790, 40, 31, 20));
        pushButton_ClearOTAlog = new QPushButton(tab_4);
        pushButton_ClearOTAlog->setObjectName(QString::fromUtf8("pushButton_ClearOTAlog"));
        pushButton_ClearOTAlog->setGeometry(QRect(840, 372, 51, 21));
        groupBox_BinProcess = new QGroupBox(tab_4);
        groupBox_BinProcess->setObjectName(QString::fromUtf8("groupBox_BinProcess"));
        groupBox_BinProcess->setGeometry(QRect(630, 390, 261, 91));
        pushButton_BinAddInfo = new QPushButton(groupBox_BinProcess);
        pushButton_BinAddInfo->setObjectName(QString::fromUtf8("pushButton_BinAddInfo"));
        pushButton_BinAddInfo->setGeometry(QRect(10, 20, 241, 31));
        pushButton_Hex2bin = new QPushButton(groupBox_BinProcess);
        pushButton_Hex2bin->setObjectName(QString::fromUtf8("pushButton_Hex2bin"));
        pushButton_Hex2bin->setGeometry(QRect(10, 50, 81, 31));
        tabWidget->addTab(tab_4, QString());
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(691, 160, 225, 23));
        gridLayout_10 = new QGridLayout(layoutWidget);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_10->setContentsMargins(0, 0, 0, 0);
        checkBoxHexReceive = new QCheckBox(layoutWidget);
        checkBoxHexReceive->setObjectName(QString::fromUtf8("checkBoxHexReceive"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(9);
        checkBoxHexReceive->setFont(font1);
        checkBoxHexReceive->setIconSize(QSize(18, 18));
        checkBoxHexReceive->setChecked(true);

        gridLayout_10->addWidget(checkBoxHexReceive, 0, 0, 1, 1);

        checkBox_TimeTag = new QCheckBox(layoutWidget);
        checkBox_TimeTag->setObjectName(QString::fromUtf8("checkBox_TimeTag"));
        checkBox_TimeTag->setChecked(false);

        gridLayout_10->addWidget(checkBox_TimeTag, 0, 1, 1, 1);

        pushButtonClearRec = new QPushButton(layoutWidget);
        pushButtonClearRec->setObjectName(QString::fromUtf8("pushButtonClearRec"));
        pushButtonClearRec->setMaximumSize(QSize(80, 16777215));

        gridLayout_10->addWidget(pushButtonClearRec, 0, 2, 1, 1);

        layoutWidget1 = new QWidget(centralwidget);
        layoutWidget1->setObjectName(QString::fromUtf8("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(11, 740, 901, 170));
        gridLayout_11 = new QGridLayout(layoutWidget1);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_11->setContentsMargins(0, 0, 0, 0);
        checkBoxHexSend = new QCheckBox(layoutWidget1);
        checkBoxHexSend->setObjectName(QString::fromUtf8("checkBoxHexSend"));
        checkBoxHexSend->setFont(font1);
        checkBoxHexSend->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend, 0, 0, 1, 1);

        lineEditSend_Text1 = new QLineEdit(layoutWidget1);
        lineEditSend_Text1->setObjectName(QString::fromUtf8("lineEditSend_Text1"));
        lineEditSend_Text1->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text1->setFont(font1);
        lineEditSend_Text1->setClearButtonEnabled(false);

        gridLayout_11->addWidget(lineEditSend_Text1, 0, 1, 1, 1);

        lineEditSend = new QLineEdit(layoutWidget1);
        lineEditSend->setObjectName(QString::fromUtf8("lineEditSend"));
        lineEditSend->setMinimumSize(QSize(511, 0));
        lineEditSend->setFont(font1);

        gridLayout_11->addWidget(lineEditSend, 0, 2, 1, 1);

        pushButtonSend1 = new QPushButton(layoutWidget1);
        pushButtonSend1->setObjectName(QString::fromUtf8("pushButtonSend1"));
        pushButtonSend1->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend1, 0, 3, 1, 1);

        pushButtonClearSend = new QPushButton(layoutWidget1);
        pushButtonClearSend->setObjectName(QString::fromUtf8("pushButtonClearSend"));
        pushButtonClearSend->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend, 0, 4, 1, 1);

        checkBoxHexSend_2 = new QCheckBox(layoutWidget1);
        checkBoxHexSend_2->setObjectName(QString::fromUtf8("checkBoxHexSend_2"));
        checkBoxHexSend_2->setFont(font1);
        checkBoxHexSend_2->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend_2, 1, 0, 1, 1);

        lineEditSend_Text2 = new QLineEdit(layoutWidget1);
        lineEditSend_Text2->setObjectName(QString::fromUtf8("lineEditSend_Text2"));
        lineEditSend_Text2->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text2->setFont(font1);

        gridLayout_11->addWidget(lineEditSend_Text2, 1, 1, 1, 1);

        lineEditSend2 = new QLineEdit(layoutWidget1);
        lineEditSend2->setObjectName(QString::fromUtf8("lineEditSend2"));
        lineEditSend2->setMinimumSize(QSize(511, 0));
        lineEditSend2->setFont(font1);

        gridLayout_11->addWidget(lineEditSend2, 1, 2, 1, 1);

        pushButtonSend2 = new QPushButton(layoutWidget1);
        pushButtonSend2->setObjectName(QString::fromUtf8("pushButtonSend2"));
        pushButtonSend2->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend2, 1, 3, 1, 1);

        pushButtonClearSend_2 = new QPushButton(layoutWidget1);
        pushButtonClearSend_2->setObjectName(QString::fromUtf8("pushButtonClearSend_2"));
        pushButtonClearSend_2->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend_2, 1, 4, 1, 1);

        checkBoxHexSend_3 = new QCheckBox(layoutWidget1);
        checkBoxHexSend_3->setObjectName(QString::fromUtf8("checkBoxHexSend_3"));
        checkBoxHexSend_3->setFont(font1);
        checkBoxHexSend_3->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend_3, 2, 0, 1, 1);

        lineEditSend_Text3 = new QLineEdit(layoutWidget1);
        lineEditSend_Text3->setObjectName(QString::fromUtf8("lineEditSend_Text3"));
        lineEditSend_Text3->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text3->setFont(font1);

        gridLayout_11->addWidget(lineEditSend_Text3, 2, 1, 1, 1);

        lineEditSend3 = new QLineEdit(layoutWidget1);
        lineEditSend3->setObjectName(QString::fromUtf8("lineEditSend3"));
        lineEditSend3->setMinimumSize(QSize(511, 0));
        lineEditSend3->setFont(font1);

        gridLayout_11->addWidget(lineEditSend3, 2, 2, 1, 1);

        pushButtonSend3 = new QPushButton(layoutWidget1);
        pushButtonSend3->setObjectName(QString::fromUtf8("pushButtonSend3"));
        pushButtonSend3->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend3, 2, 3, 1, 1);

        pushButtonClearSend_3 = new QPushButton(layoutWidget1);
        pushButtonClearSend_3->setObjectName(QString::fromUtf8("pushButtonClearSend_3"));
        pushButtonClearSend_3->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend_3, 2, 4, 1, 1);

        checkBoxHexSend_4 = new QCheckBox(layoutWidget1);
        checkBoxHexSend_4->setObjectName(QString::fromUtf8("checkBoxHexSend_4"));
        checkBoxHexSend_4->setFont(font1);
        checkBoxHexSend_4->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend_4, 3, 0, 1, 1);

        lineEditSend_Text4 = new QLineEdit(layoutWidget1);
        lineEditSend_Text4->setObjectName(QString::fromUtf8("lineEditSend_Text4"));
        lineEditSend_Text4->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text4->setFont(font1);

        gridLayout_11->addWidget(lineEditSend_Text4, 3, 1, 1, 1);

        lineEditSend4 = new QLineEdit(layoutWidget1);
        lineEditSend4->setObjectName(QString::fromUtf8("lineEditSend4"));
        lineEditSend4->setMinimumSize(QSize(511, 0));
        lineEditSend4->setFont(font1);

        gridLayout_11->addWidget(lineEditSend4, 3, 2, 1, 1);

        pushButtonSend4 = new QPushButton(layoutWidget1);
        pushButtonSend4->setObjectName(QString::fromUtf8("pushButtonSend4"));
        pushButtonSend4->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend4, 3, 3, 1, 1);

        pushButtonClearSend_4 = new QPushButton(layoutWidget1);
        pushButtonClearSend_4->setObjectName(QString::fromUtf8("pushButtonClearSend_4"));
        pushButtonClearSend_4->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend_4, 3, 4, 1, 1);

        checkBoxHexSend_5 = new QCheckBox(layoutWidget1);
        checkBoxHexSend_5->setObjectName(QString::fromUtf8("checkBoxHexSend_5"));
        checkBoxHexSend_5->setFont(font1);
        checkBoxHexSend_5->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend_5, 4, 0, 1, 1);

        lineEditSend_Text5 = new QLineEdit(layoutWidget1);
        lineEditSend_Text5->setObjectName(QString::fromUtf8("lineEditSend_Text5"));
        lineEditSend_Text5->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text5->setFont(font1);

        gridLayout_11->addWidget(lineEditSend_Text5, 4, 1, 1, 1);

        lineEditSend5 = new QLineEdit(layoutWidget1);
        lineEditSend5->setObjectName(QString::fromUtf8("lineEditSend5"));
        lineEditSend5->setMinimumSize(QSize(511, 0));
        lineEditSend5->setFont(font1);

        gridLayout_11->addWidget(lineEditSend5, 4, 2, 1, 1);

        pushButtonSend5 = new QPushButton(layoutWidget1);
        pushButtonSend5->setObjectName(QString::fromUtf8("pushButtonSend5"));
        pushButtonSend5->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend5, 4, 3, 1, 1);

        pushButtonClearSend_5 = new QPushButton(layoutWidget1);
        pushButtonClearSend_5->setObjectName(QString::fromUtf8("pushButtonClearSend_5"));
        pushButtonClearSend_5->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend_5, 4, 4, 1, 1);

        checkBoxHexSend_6 = new QCheckBox(layoutWidget1);
        checkBoxHexSend_6->setObjectName(QString::fromUtf8("checkBoxHexSend_6"));
        checkBoxHexSend_6->setFont(font1);
        checkBoxHexSend_6->setChecked(true);

        gridLayout_11->addWidget(checkBoxHexSend_6, 5, 0, 1, 1);

        lineEditSend_Text6 = new QLineEdit(layoutWidget1);
        lineEditSend_Text6->setObjectName(QString::fromUtf8("lineEditSend_Text6"));
        lineEditSend_Text6->setMaximumSize(QSize(91, 16777215));
        lineEditSend_Text6->setFont(font1);

        gridLayout_11->addWidget(lineEditSend_Text6, 5, 1, 1, 1);

        lineEditSend6 = new QLineEdit(layoutWidget1);
        lineEditSend6->setObjectName(QString::fromUtf8("lineEditSend6"));
        lineEditSend6->setMinimumSize(QSize(511, 0));
        lineEditSend6->setFont(font1);

        gridLayout_11->addWidget(lineEditSend6, 5, 2, 1, 1);

        pushButtonSend6 = new QPushButton(layoutWidget1);
        pushButtonSend6->setObjectName(QString::fromUtf8("pushButtonSend6"));
        pushButtonSend6->setFont(font1);

        gridLayout_11->addWidget(pushButtonSend6, 5, 3, 1, 1);

        pushButtonClearSend_6 = new QPushButton(layoutWidget1);
        pushButtonClearSend_6->setObjectName(QString::fromUtf8("pushButtonClearSend_6"));
        pushButtonClearSend_6->setFont(font1);

        gridLayout_11->addWidget(pushButtonClearSend_6, 5, 4, 1, 1);

        line_3 = new QFrame(centralwidget);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        line_3->setGeometry(QRect(0, 180, 931, 16));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);
        layoutWidget2 = new QWidget(centralwidget);
        layoutWidget2->setObjectName(QString::fromUtf8("layoutWidget2"));
        layoutWidget2->setGeometry(QRect(10, 160, 149, 22));
        gridLayout_12 = new QGridLayout(layoutWidget2);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_12->setContentsMargins(0, 0, 0, 0);
        checkBox_TimeTagSend = new QCheckBox(layoutWidget2);
        checkBox_TimeTagSend->setObjectName(QString::fromUtf8("checkBox_TimeTagSend"));

        gridLayout_12->addWidget(checkBox_TimeTagSend, 0, 0, 1, 1);

        pushButtonClearSendArea = new QPushButton(layoutWidget2);
        pushButtonClearSendArea->setObjectName(QString::fromUtf8("pushButtonClearSendArea"));
        pushButtonClearSendArea->setMaximumSize(QSize(80, 16777215));

        gridLayout_12->addWidget(pushButtonClearSendArea, 0, 1, 1, 1);

        layoutWidget3 = new QWidget(centralwidget);
        layoutWidget3->setObjectName(QString::fromUtf8("layoutWidget3"));
        layoutWidget3->setGeometry(QRect(22, 10, 879, 24));
        gridLayout_9 = new QGridLayout(layoutWidget3);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget3);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_9->addWidget(label, 0, 0, 1, 1);

        comboBox_serialNum = new QComboBox(layoutWidget3);
        comboBox_serialNum->setObjectName(QString::fromUtf8("comboBox_serialNum"));

        gridLayout_9->addWidget(comboBox_serialNum, 0, 1, 1, 1);

        label_2 = new QLabel(layoutWidget3);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_9->addWidget(label_2, 0, 2, 1, 1);

        comboBox_baudrate = new QComboBox(layoutWidget3);
        comboBox_baudrate->setObjectName(QString::fromUtf8("comboBox_baudrate"));

        gridLayout_9->addWidget(comboBox_baudrate, 0, 3, 1, 1);

        label_4 = new QLabel(layoutWidget3);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_9->addWidget(label_4, 0, 4, 1, 1);

        comboBoxParity = new QComboBox(layoutWidget3);
        comboBoxParity->setObjectName(QString::fromUtf8("comboBoxParity"));

        gridLayout_9->addWidget(comboBoxParity, 0, 5, 1, 1);

        pushButtonOpenSerial = new QPushButton(layoutWidget3);
        pushButtonOpenSerial->setObjectName(QString::fromUtf8("pushButtonOpenSerial"));

        gridLayout_9->addWidget(pushButtonOpenSerial, 0, 6, 1, 1);

        pushButtonFreshPort = new QPushButton(layoutWidget3);
        pushButtonFreshPort->setObjectName(QString::fromUtf8("pushButtonFreshPort"));

        gridLayout_9->addWidget(pushButtonFreshPort, 0, 7, 1, 1);

        horizontalSpacer = new QSpacerItem(288, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer, 0, 8, 1, 1);

        pushButton_about = new QPushButton(layoutWidget3);
        pushButton_about->setObjectName(QString::fromUtf8("pushButton_about"));
        pushButton_about->setMaximumSize(QSize(51, 16777215));

        gridLayout_9->addWidget(pushButton_about, 0, 9, 1, 1);

        layoutWidget4 = new QWidget(centralwidget);
        layoutWidget4->setObjectName(QString::fromUtf8("layoutWidget4"));
        layoutWidget4->setGeometry(QRect(111, 720, 390, 22));
        horizontalLayout = new QHBoxLayout(layoutWidget4);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label_45 = new QLabel(layoutWidget4);
        label_45->setObjectName(QString::fromUtf8("label_45"));

        horizontalLayout->addWidget(label_45);

        horizontalSpacer_3 = new QSpacerItem(268, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label_46 = new QLabel(layoutWidget4);
        label_46->setObjectName(QString::fromUtf8("label_46"));

        horizontalLayout->addWidget(label_46);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 919, 22));
        MainWindow->setMenuBar(menubar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "GVT\345\212\251\346\211\213", nullptr));
        actionlzlei_chint_com->setText(QCoreApplication::translate("MainWindow", "lzlei@chint.com", nullptr));
        groupBox_OMCA_OTA->setTitle(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\345\215\217\350\256\256", nullptr));
        pushButtonUpgrate->setText(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247", nullptr));
        pushButtonStopUpgrade->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\215\207\347\272\247", nullptr));
        pushButtonReadVerSelfDefine->setText(QCoreApplication::translate("MainWindow", "\350\257\273\347\211\210\346\234\254\345\217\267", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\345\270\247\345\244\264Hex(1Byte)", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\345\270\247\345\260\276Hex(1Byte)", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\345\220\257\345\212\250\345\221\275\344\273\244Hex(1Byte)", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247\345\221\275\344\273\244Hex(1Byte)", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\235\237\345\221\275\344\273\244Hex(1Byte)", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\350\257\273\347\211\210\346\234\254\345\217\267\345\221\275\344\273\244Hex(1Byte)", nullptr));
        label_111->setText(QCoreApplication::translate("MainWindow", "\346\226\207\344\273\266\345\244\247\345\260\217(Bytes)", nullptr));
        pushButtonOpenFile->setText(QCoreApplication::translate("MainWindow", "\345\212\240\350\275\275bin\346\226\207\344\273\266", nullptr));
        groupBox_OtaByModbusRTU->setTitle(QCoreApplication::translate("MainWindow", "Modbus-RTU", nullptr));
        pushButton_UpgradeMB->setText(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247", nullptr));
        pushButton_StopUpgradeMB->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\215\207\347\272\247", nullptr));
        pushButtonReadVerMB->setText(QCoreApplication::translate("MainWindow", "\350\257\273\347\211\210\346\234\254\345\217\267", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\345\234\260\345\235\200", nullptr));
        groupBox_OtaByDlt698->setTitle(QCoreApplication::translate("MainWindow", "DLT698", nullptr));
        pushButton_Upgrade698->setText(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247", nullptr));
        pushButton_StopUpgrade698->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\215\207\347\272\247", nullptr));
        pushButtonReadVer698->setText(QCoreApplication::translate("MainWindow", "\350\257\273\347\211\210\346\234\254\345\217\267", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "\345\234\260\345\235\200", nullptr));
        groupBox_OtaByDlt645->setTitle(QCoreApplication::translate("MainWindow", "DLT645", nullptr));
        pushButton_Upgrade645->setText(QCoreApplication::translate("MainWindow", "\345\215\207\347\272\247", nullptr));
        pushButton_StopUpgrade645->setText(QCoreApplication::translate("MainWindow", "\345\201\234\346\255\242\345\215\207\347\272\247", nullptr));
        pushButtonReadVer645->setText(QCoreApplication::translate("MainWindow", "\350\257\273\347\211\210\346\234\254\345\217\267", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "\345\234\260\345\235\200", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "CRC32\350\216\267\345\217\226\345\200\274(Hex)", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "CRC32\350\256\241\347\256\227\345\200\274(Hex)", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\345\233\272\344\273\266\347\211\210\346\234\254\345\217\267", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\350\247\204\346\240\274", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\347\272\247\346\225\260", nullptr));
        pushButton_ClearOTAlog->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272", nullptr));
        groupBox_BinProcess->setTitle(QCoreApplication::translate("MainWindow", "\345\216\237\345\247\213hex\346\210\226bin\345\244\204\347\220\206\346\210\220\345\217\257\345\215\207\347\272\247bin\346\226\207\344\273\266", nullptr));
        pushButton_BinAddInfo->setText(QCoreApplication::translate("MainWindow", "bin\346\226\207\344\273\266\350\277\275\345\212\240\346\225\260\346\215\256\357\274\210CRC32+\347\211\210\346\234\254+\350\247\204\346\240\274+\347\272\247\346\225\260\357\274\211", nullptr));
        pushButton_Hex2bin->setText(QCoreApplication::translate("MainWindow", "hex\350\275\254bin\345\267\245\345\205\267", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_4), QCoreApplication::translate("MainWindow", "OTA\345\215\207\347\272\247", nullptr));
        checkBoxHexReceive->setText(QCoreApplication::translate("MainWindow", "Hex\346\230\276\347\244\272", nullptr));
        checkBox_TimeTag->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\210\263", nullptr));
        pushButtonClearRec->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\346\216\245\346\224\266", nullptr));
        checkBoxHexSend->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2011", nullptr));
        pushButtonSend1->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBoxHexSend_2->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2012", nullptr));
        pushButtonSend2->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend_2->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBoxHexSend_3->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2013", nullptr));
        pushButtonSend3->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend_3->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBoxHexSend_4->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2014", nullptr));
        pushButtonSend4->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend_4->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBoxHexSend_5->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2015", nullptr));
        pushButtonSend5->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend_5->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBoxHexSend_6->setText(QCoreApplication::translate("MainWindow", "Hex\345\217\221\351\200\2016", nullptr));
        pushButtonSend6->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        pushButtonClearSend_6->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201", nullptr));
        checkBox_TimeTagSend->setText(QCoreApplication::translate("MainWindow", "\346\227\266\351\227\264\346\210\263", nullptr));
        pushButtonClearSendArea->setText(QCoreApplication::translate("MainWindow", "\346\270\205\347\251\272\345\217\221\351\200\201\345\214\272", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\344\270\262\345\217\243\345\217\267", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\263\242\347\211\271\347\216\207", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\346\240\241\351\252\214\344\275\215", nullptr));
        pushButtonOpenSerial->setText(QCoreApplication::translate("MainWindow", "\346\211\223\345\274\200\344\270\262\345\217\243", nullptr));
        pushButtonFreshPort->setText(QCoreApplication::translate("MainWindow", "\345\210\267\346\226\260\344\270\262\345\217\243", nullptr));
        pushButton_about->setText(QCoreApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
        label_45->setText(QCoreApplication::translate("MainWindow", "\345\244\207\346\263\250\345\214\272", nullptr));
        label_46->setText(QCoreApplication::translate("MainWindow", "\350\207\252\345\256\232\344\271\211\346\225\260\346\215\256\345\214\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
