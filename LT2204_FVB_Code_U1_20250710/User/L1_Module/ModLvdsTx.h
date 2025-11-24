//////////////////////////////////////
// Project:   LT2204
// Filename:  ChipLvds.h
// Version:   V1.0
// Copyright: Lontium
//////////////////////////////////////
#ifndef _CHIPLVDSTX_H
#define _CHIPLVDSTX_H

#include "LtType.h"

#if TX_PORT_SIGN == LVDS_PORT

typedef enum
{
    P_VIDEO_FMT = 0,
    I_VIDEO_FMT,
} LVDS_VIDEO_FMT;

typedef enum
{
    YoCb_FIRST = 0x1b,
    YoCr_FIRST = 0x93,
    CbYo_FIRST = 0x4b,
    CrYo_FIRST = 0x63,

} SONY_YUV422BPC_DATA_FMT;

/**
 * 25103101: 打开SSC会闪屏，正常和画面倾斜来回切换
 * 25103102: 关闭SSC，打开DEMODE，不出图
 * 25103103: 关闭DEMODE，调整EDID，增加HBlank(160->260)，画面左上边白底图片还是会有一点闪动，拔插上行几次也握手成功
 * 25103104: 调整EDID，增加HBlank(260->300)，还是左上角闪
 * 25103105: 调整EDID，增加HBlank(300->380)，还是左上角闪
 * 25103106: 调整EDID，HBlank还是300，VBlank增加(50->60)，还是左上角闪
 * 25103107: VBlank(60->71 VESA标准)
 * 25103108: 调整左右眼画面，还是左边闪，看板子左边走线较远。考虑加强LVDS输出驱动和swing
 * 25103109: 调整了Swing，刷寄存器刷到最大还是左边会有。吴工给的说法是，如果调换Port不跟着走，和固件关系不大
 * 25103110: Swing和Capability调到最大
 * 25103111: 按25103103的版本调整2/3D切换, 切换2D有竖纹
 * 25103112: 调整2D的EDID，从3D的版本上更新, 2D还是有竖纹
 * 
 * 25103113: 调整g_stLvdsTx.ucLMTXPortSel, 从原先的0xDD->0x44。客户初步测试没什么问题。
 *  2D_90Hz_EDID: 2D_1920x1200_90Hz_25103101.txt; 
 *  3D_90Hz_EDID: 3D_3840x1200_90Hz_25103101.txt
 * 
 * 25103114: 在2D_90Hz_EDID上调整为120Hz，2D模式下120Hz。测试正常。
 *  2D_120Hz_EDID: 2D_1920x1200_120Hz_25103101.txt
 * 
 * 25103115: 在3D_90Hz_EDID上调整为120Hz，3D模式。需要走DSC，吴工提供代码不支持
 * 25103116: 2D_90Hz/3D_90Hz/2D_120Hz 切换, 测试正常
 * 
 * 25110301: 吴工提供DSC库重新测试3D_120Hz，使能 DPRX_FEC_SUPPORT 和 DPRX_DSC_DEC_SUPPORT, 其他没做调整。测试可以实现，ModDpRx.c文件里面有部分关于HDMI的内容，注释掉强制执行DSC。
 *  3D_120Hz_EDID: 3D_3840x1200_120Hz_25103101.txt
 * 
 * 25110302: 2D_90Hz/3D_90Hz/2D_120Hz/3D_120Hz 切换，笔记本测试正常，但是客户BOX还是不良，报DSCFIFO ERR错误
 * 25110501: ModDpRx.c文件里面修改FIFO的if判断条件，关键词 "g_stDpRx.ucDscFifoEmpty"
 * 25110502: 吴工反馈调整 DPRX_SWING_MIN_LEVEL, 0-3 Level都试试, 当前版本给3
 * 25110503: DPRX_SWING_MIN_LEVEL给LEVEL_0
 * 25110504: DPRX_SWING_MIN_LEVEL给LEVEL_2
 * 
 * 25110701: 测试3D_120Hz HBR3模式, 关掉DSC
 * 25110702: 修改EDID版本为1.4, 选择6bit色深模式
 * 
 * 25111801: 客户需求在 25110302 基础上修改EDID信息; HDCP打开后报错，封库代码里面没有这部分
 * 25111802: 吴工重新封库测试HDCP。在PC上测试可以显示
 * 
 * 25/11/24_01: 调整代码为3D_90Hz HBR3模式, 关闭DSC, 还原 25110702 版本, EDID版本1.4, 6bit色深模式
 */
#define LVDS_SSC                    DISABLED            // ENABLED & DISABLED
#define LVDS_SSC_SEL                SSC_1920x1080_30k5
#define LVDS_DEMODE                 DISABLED            // ENABLED & DISABLED   如果选择disabled 那就意味着选择Sync mode
#define LVDS_PORTNUM                LVDS_4PORT          // LVDS_1PORT/LVDS_2PORT/LVDS_4PORT
#define LVDS_3D                     SIDE_BY_SIDE        // NO_3D_MODE/SIDE_BY_SIDE
#define LVDS_SUB_MODE_LANE          LVDSTX_8LANE        // SUB LVDS LANE Sel
#define LVDS_SONY422_DATA_MAP_SEL   YoCb_FIRST

#define LVDS_DATAMAP                SONY_YUV422_8Bit    // SONY_YUV422_8Bit/VESA_RGB_8Bit/JEIDA_RGB_8Bit/VESA_RGB_10Bit/JEIDA_RGB_10Bit
#define LVDS_VIDEO_FMT              P_VIDEO_FMT         // REF LVDS_VIDEO_FMT 只有在sublvds 或者是内同步模式才可设置

#define LVDS_VBLANK_SYNC_CODE_EN            ENABLED // DISABLED in Vblank periodand in vactive period  ENABLED:Vblank not Send Sync code
#define LVDS_SYNC_CODE_SEND_INDEPENDENT     ENABLED // DISABLED:每个 port 同步码非独立传输, ENABLED:每个 port 同步码独立传输

#define LP0_EN ENABLED // PORT0 ENABLED &  DISABLED
#define LP1_EN ENABLED // PORT1 ENABLED &  DISABLED
#define LP2_EN ENABLED // PORT2 ENABLED &  DISABLED
#define LP3_EN ENABLED // PORT3 ENABLED &  DISABLED

//
// typedef enum
//{
//    BPC6_FMT,
//    BPC8_FMT,
//    BPC10_FMT,
//}LVDS_BPC_FMT;

//
// typedef enum
//{
//    RGB_FMT ,
//    Y422_FMT,
//    Y444_FMT,
//    Y420_FMT,
//
//}LVDS_COLOR_SPACE;

#define NO_SSC 0
#define SSC_1920x1080_30k5 1
#define SSC_3840x2160_30k5 2

typedef enum
{
    LVDS_1PORT = 1,
    LVDS_2PORT = 2,
    LVDS_4PORT = 4,

} LVDS_PORT_NUM;

typedef enum
{
    LVDSTX_1LANE = 1,
    LVDSTX_2LANE = 2,
    LVDSTX_4LANE = 4,
    LVDSTX_8LANE = 8,

} LVDS_LANE_NUM;

typedef enum
{
    NO_3D_MODE = 0,
    SIDE_BY_SIDE, // left&right
    LINE_ALTER,
    FRAME_PACKING, // up&down
} LMTX_3D;

typedef enum
{
    VESA_RGB_6Bit = 0, // RGB Data mode
    VESA_RGB_8Bit,     // RGB Data mode
    JEIDA_RGB_8Bit,    // RGB Data mode
    VESA_RGB_10Bit,    // RGB Data mode
    JEIDA_RGB_10Bit,   // RGB Data mode
    SONY_YUV422_8Bit,  // Sony YUV4228bpc mode;

    INTERNAL_SYNC_MODE,
    SUB_LVDS_MODE,
} LVDSTX_DATA_MAP;

typedef enum
{
    STATE_LVDSTX_POWER_ON_1 = 1,
    STATE_LVDSTX_UPSTREAM_VIDEO_READY_2,
    STATE_LVDSTX_PLL_CALI_3,
    STATE_LVDSTX_CONFIG_4,
    STATE_LVDSTX_VIDEO_OUT_5,
    STATE_LVDSTX_AUDIO_OUT_6,
    STATE_LVDSTX_PLAY_BACK_7
} EnumLvdsTxState;

typedef enum
{
    LVDSTX_SINK_PLUG_EVENT = 1,
    LVDSTX_SINK_UNPLUG_EVENT,
} EnumLvdsTxEvent;

typedef struct LvdsTx
{
    u8 ucTxState;
    u8 ucTxStateChangeFlag;
    u8 b1UpstreamVideoReady;

    u8 b1LvdsTxSscEn;
    u8 b1LvdsTxDeModeEn;
    u8 ucLvdsTx3DMode;
    u8 ucLvdsTxDataMap;
    u8 ucLvdsTxPortNum;
    u8 ucLvdsTxLaneNum;
    u8 ucLvdsTxVidFmt;
    u32 ulLvdsTxInputClk;
    u8 ucSonyYuvDataSel;
    u8 ucLvdsVblankSCEn;
    u8 ucLvdsSCSendIndependent;

    // u8 ucLVDSTXColor;
    // u8 ucLVDSTXDataBpc;
    u8 b1LvdsTxSscSel;
    u8 ucLMTXPortSel;

} StructLvdsTx;
extern StructLvdsTx g_stLvdsTx;

extern u8 szEdidData[512];
extern u8 code szLvdsEdid[512];

extern void Mod_LvdsTx_NotifyRegister(void (*pFunction)());
extern void Mod_LvdsTx_PowerOnInit(void);
extern void Mod_LvdsTx_Handler(void);
extern void Mod_LMTx_EventMonitor(u8 ucMsgType);
extern void Mod_LvdsTx_Pattern(void);
#endif
#endif
