/*Tab A8 code for SR-AX6300-01-405 by luxinjun at 2021/09/09 start*/
/*
 * Copyright (C) 2010 - 2021 Novatek, Inc.
 *
 * $Revision: 77624 $
 * $Date: 2021-02-05 10:03:05 +0800 (週五, 05 二月 2021) $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 */
#ifndef 	_LINUX_NVT_TOUCH_H
#define		_LINUX_NVT_TOUCH_H

#include <linux/delay.h>
#include <linux/input.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/uaccess.h>
#include <linux/input/sec_cmd.h>
#include <linux/regulator/consumer.h>

#ifdef CONFIG_HAS_EARLYSUSPEND
#include <linux/earlysuspend.h>
#endif

#include <linux/power_supply.h>
#include "nt36xxx_mem_map.h"

#ifdef CONFIG_MTK_SPI
/* Please copy mt_spi.h file under mtk spi driver folder */
#include "mt_spi.h"
#endif

#ifdef CONFIG_SPI_MT65XX
#include <linux/platform_data/spi-mt65xx.h>
#endif
#include <linux/touchscreen_info.h>

//---GPIO number---
#define NVTTOUCH_RST_PIN 980
#define NVTTOUCH_INT_PIN 943

//---INT trigger mode---
//#define IRQ_TYPE_EDGE_RISING 1
//#define IRQ_TYPE_EDGE_FALLING 2
//#define INT_TRIGGER_TYPE IRQ_TYPE_EDGE_FALLING


//---SPI driver info.---
#define NVT_SPI_NAME "NVT-ts"

//---Input device info.---
//#define NVT_TS_NAME "NVTCapacitiveTouchScreen"
#define NVT_TS_NAME "sec_touchscreen"


//---Touch info.---
#define TOUCH_DEFAULT_MAX_WIDTH 1200
#define TOUCH_DEFAULT_MAX_HEIGHT 1920
#define TOUCH_MAX_FINGER_NUM 10
#define TOUCH_KEY_NUM 0
#if TOUCH_KEY_NUM > 0
extern const uint16_t touch_key_array[TOUCH_KEY_NUM];
#endif
#define TOUCH_FORCE_NUM 1000

/* Enable only when module have tp reset pin and connected to host */
#define NVT_TOUCH_SUPPORT_HW_RST 0

// If sec relative function is not present, turn SEC_2_NVT_DEBUG on
// Or turn it off
//#define SEC_2_NVT_DEBUG 0
#define SEC_2_NVT_DEBUG 1
/*Tab A8 code for AX6300DEV-1956 by huangzhongjie at 2021/11/04 start*/
#define NVT_DEBUG 0
/*Tab A8 code for AX6300DEV-1956 by huangzhongjie at 2021/11/04 end*/
#if SEC_2_NVT_DEBUG
#if NVT_DEBUG
#define NVT_LOG(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#else
#define NVT_LOG(fmt, args...)    pr_info("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)
#endif
#define NVT_ERR(fmt, args...)    pr_err("[%s] %s %d: " fmt, NVT_SPI_NAME, __func__, __LINE__, ##args)

#ifdef input_info
#undef input_info
#endif
#ifdef input_err
#undef input_err
#endif

#define input_info(en, dev_ptr,fmt, args...)   NVT_LOG(fmt, ##args)
#define input_err(en, dev_ptr,fmt, args...)   NVT_ERR(fmt, ##args)
#endif

//---Customerized func.---
#define NVT_TOUCH_PROC 1
#define NVT_TOUCH_EXT_PROC 1
#define NVT_TOUCH_MP 1
#define MT_PROTOCOL_B 1
#define LCD_SETTING 0
#define PROXIMITY_FUNCTION 0
#define SEC_TOUCH_CMD 1

//---Customized Test---
#define LPWG_TEST 1
#if LPWG_TEST
#define FDM_TEST  1	//Depend on LPWG_TEST
#else
#define FDM_TEST  0	//Depend on LPWG_TEST
#endif

extern const uint16_t gesture_key_array[];
/*Tab A8 code for SR-AX6300-01-376 by luxinjun at 2021/09/10 start*/
#define USB_DETECT_IN 1
#define USB_DETECT_OUT 0
#define CMD_CHARGER_ON	(0x53)
#define CMD_CHARGER_OFF (0x51)
int32_t nvt_set_charger(uint8_t charger_on_off);
/*Tab A8 code for SR-AX6300-01-376 by luxinjun at 2021/09/10 end*/

#define BOOT_UPDATE_FIRMWARE 1
//#define BOOT_UPDATE_FIRMWARE_NAME "tsp_novatek/nt36525_a01core_tp.bin"
//#define MP_UPDATE_FIRMWARE_NAME   "tsp_novatek/nt36525_a01core_mp.bin"
#define POINT_DATA_CHECKSUM 1
#define POINT_DATA_CHECKSUM_LEN 65

#define NVT_TS_DEFAULT_UMS_FW		"/sdcard/firmware/tsp/nvt.bin"

//---ESD Protect.---
/*Tab A7 Lite T618 code for SR-AX6189A-01-152 by suyurui at 2022/1/12 start*/
#define NVT_TOUCH_ESD_PROTECT 0
/*Tab A7 Lite T618 code for SR-AX6189A-01-152 by suyurui at 2022/1/12 end*/
#define NVT_TOUCH_ESD_CHECK_PERIOD 1500	/* ms */
#define NVT_TOUCH_WDT_RECOVERY 1

#define TOUCH_PRINT_INFO_DWORK_TIME	30000	/* 30s */

#if PROXIMITY_FUNCTION
struct nvt_ts_event_proximity {
	u8 reserved_1:3;
	u8 id:5;
	u8 data_page;
	u8 status;
	u8 reserved_2;
} __attribute__ ((packed));
#endif

struct nvt_ts_event_coord {
	u8 status:3;
	u8 id:5;
	u8 x_11_4;
	u8 y_11_4;
	u8 y_3_0:4;
	u8 x_3_0:4;
	u8 w_major;
	u8 pressure_7_0;
} __attribute__ ((packed));

struct nvt_ts_coord {
	u16 x;
	u16 y;
	u16 p;
	u16 p_x;
	u16 p_y;
	u8 w_major;
	u8 w_minor;
	u8 palm;
	u8 metal_jig;
	u8 status;
	u8 p_status;
	bool press;
	bool p_press;
	int move_count;
};

struct nvt_ts_platdata {
	int irq_gpio;
	u32 irq_flags;
	int cs_gpio;
	u8 x_num;
	u8 y_num;
	u16 abs_x_max;
	u16 abs_y_max;
	u32 area_indicator;
	u32 area_navigation;
	u32 area_edge;
	u8 max_touch_num;
	struct pinctrl *pinctrl;
#if PROXIMITY_FUNCTION
	bool support_ear_detect;	// if support proximity mode feature
#endif
	bool enable_settings_aot;	// if support aot feature
/*#Tab A8 code for AX6300DEV-3657 by fengzhigang at 2021/12/09 start*/
	char *md_name;
	char *firmware_name;
	char *firmware_name_mp;
/*#Tab A8 code for AX6300DEV-3657 by fengzhigang at 2021/12/09 end*/
	u32 open_test_spec[2];
	u32 short_test_spec[2];
	u32 diff_test_frame;
#if (FDM_TEST && LPWG_TEST)
	u32 fdm_x_num;
	u32 fdm_diff_test_frame;
#endif
	int lcd_id;
#if LCD_SETTING
	const char *regulator_lcd_vdd;
	const char *regulator_lcd_reset;
	const char *regulator_lcd_bl;
#endif
};

struct nvt_ts_data {
	struct spi_device *client;
	struct nvt_ts_platdata *platdata;
	struct nvt_ts_coord coords[TOUCH_MAX_FINGER_NUM];
	u8 touch_count;
	struct input_dev *input_dev;
#if PROXIMITY_FUNCTION
	struct input_dev *input_dev_proximity;
#endif
	struct delayed_work nvt_fwu_work;
	uint16_t addr;
	int8_t phys[32];
#if defined(CONFIG_FB)
#ifdef _MSM_DRM_NOTIFY_H_
	struct notifier_block drm_notif;
/*Tab A8 code for SR-AX6300-01-373 by luxinjun at 2021/09/10 start*/
#elif defined(CONFIG_DRM_PANEL)
	struct notifier_block drm_panel_notif;
/*Tab A8 code for SR-AX6300-01-373 by luxinjun at 2021/09/10 end*/
#else
	struct notifier_block fb_notif;
#endif
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
	bool suspended;
	uint8_t fw_ver;
	int32_t ic_idx;
//	uint8_t x_num;
//	uint8_t y_num;
//	uint16_t abs_x_max;
//	uint16_t abs_y_max;
//	uint8_t max_touch_num;
	uint8_t max_button_num;
//	uint32_t int_trigger_type;
//	int32_t irq_gpio;
//	uint32_t irq_flags;
	int32_t reset_gpio;
	uint32_t reset_flags;
	struct mutex lock;
	const struct nvt_ts_mem_map *mmap;
	uint8_t carrier_system;
	uint8_t hw_crc;
	uint16_t nvt_pid;
	uint8_t *rbuf;
	uint8_t *xbuf;
	struct mutex xbuf_lock;
	bool irq_enabled;
	u8 cascade;
	u8 fw_ver_ic[4];
	u8 fw_ver_ic_bar;
	u8 fw_ver_bin[4];
	u8 fw_ver_bin_bar;
	volatile int power_status;
	u8 aot_enable;
#if SEC_TOUCH_CMD
	struct sec_cmd_data sec;
	u16 sec_function;
	u8 cover_mode_restored;
#endif
	bool isUMS;
#ifdef CONFIG_MTK_SPI
	struct mt_chip_conf spi_ctrl;
#endif
#ifdef CONFIG_SPI_MT65XX
	struct mtk_chip_config spi_ctrl;
#endif
#if PROXIMITY_FUNCTION
	unsigned int ear_detect_enable;	//proximity mode
	long prox_power_off;	//proximity mode report off(sleep in)
	u8 hover_event;	//virtual_prox
#endif
	bool lcdoff_test;
//	struct delayed_work work_read_info;
	struct delayed_work work_print_info;
	u32 print_info_cnt_open;
	u32 print_info_cnt_release;
	u16 print_info_currnet_mode;
#if LCD_SETTING
	struct regulator *regulator_vdd;
	struct regulator *regulator_lcd_reset;
	struct regulator *regulator_lcd_bl_en;
#endif
	u8 noise_mode;
#ifdef CONFIG_PM
	bool dev_pm_suspend;
	struct completion dev_pm_resume_completion;
#endif
	/*Tab A8 code for SR-AX6300-01-376 by luxinjun at 2021/09/10 start*/
	struct notifier_block charger_notif;
	struct workqueue_struct *nvt_charger_notify_wq;
	struct work_struct charger_notify_work;
	/*Tab A8 code for AX6300DEV-1494 by luxinjun at 2021/11/03 start*/
	struct workqueue_struct *nvt_resume_wq;
	struct delayed_work nvt_resume_work;
	/*Tab A8 code for AX6300DEV-1494 by luxinjun at 2021/11/03 end*/
	int usb_plug_status;
	int fw_update_stat;
	/*Tab A8 code for SR-AX6300-01-376 by luxinjun at 2021/09/10 end*/
	/*#Tab A8 code for SR-AX6300-01-128 by luxinjun at 2021/11/18 start*/
	bool tp_is_enabled;
	/*#Tab A8 code for SR-AX6300-01-128 by luxinjun at 2021/11/18 end*/
};

#if NVT_TOUCH_PROC
struct nvt_flash_data{
	rwlock_t lock;
};
#endif

typedef enum {
	RESET_STATE_INIT = 0xA0,// IC reset
	RESET_STATE_REK,		// ReK baseline
	RESET_STATE_REK_FINISH,	// baseline is ready
	RESET_STATE_NORMAL_RUN,	// normal run
	RESET_STATE_MAX  = 0xAF
} RST_COMPLETE_STATE;

typedef enum {
	EVENT_MAP_HOST_CMD                      = 0x50,
	EVENT_MAP_HANDSHAKING_or_SUB_CMD_BYTE   = 0x51,
	EVENT_MAP_FUNCT_STATE                   = 0x5C,
	EVENT_MAP_RESET_COMPLETE                = 0x60,
	EVENT_MAP_FWINFO                        = 0x78,
	EVENT_MAP_PANEL                         = 0x8F,
	EVENT_MAP_PROJECTID                     = 0x9A,
	EVENT_MAP_SENSITIVITY_DIFF              = 0x9D,	// 18 bytes
#if PROXIMITY_FUNCTION
	EVENT_MAP_PROXIMITY_SUM 				= 0xB0, // 2 bytes
	EVENT_MAP_PROXIMITY_THD 				= 0xB2, // 2 bytes
#endif
} SPI_EVENT_MAP;

typedef enum {
	TEST_RESULT_PASS = 0x00,
	TEST_RESULT_FAIL = 0x01,
} TEST_RESULT;

enum {
	POWER_OFF_STATUS = 0,
	POWER_LPM_STATUS,
#if PROXIMITY_FUNCTION
	POWER_PROX_STATUS,
#endif
	POWER_ON_STATUS
};

/*#Tab A8 code for AX6300DEV-3657 by fengzhigang at 2021/12/09 start*/
typedef struct
{
	const char *lcd_name;
	int fw_num;
}nvt_upgrade_module;

enum NVT_TP_MODEL {
	MODEL_DEFAULT = 0,
	/*Tab A7 Lite T618 code for SR-AX6189A-01-148|SR-AX6189A-01-847 by suyurui at 2022/1/13 start*/
	MODEL_TXD_MDT,
	MODEL_LS_HSD,
	/*Tab A7 Lite T618 code for SR-AX6189A-01-148|SR-AX6189A-01-847 by suyurui at 2022/1/13 end*/
};
/*#Tab A8 code for AX6300DEV-3657 by fengzhigang at 2021/12/09 end*/

//u8 aot_enable;
//bit control
enum {
	LPM_OFF = 0,
	LPM_DC = 0x01,	//Double Click
	LPM_Spay = 0x02	//Spay, slide up
};

//---SPI READ/WRITE---
#define SPI_WRITE_MASK(a)	(a | 0x80)
#define SPI_READ_MASK(a)	(a & 0x7F)

#define DUMMY_BYTES (1)
#define NVT_TRANSFER_LEN	(63*1024)
#define NVT_READ_LEN		(2*1024)
#define NVT_XBUF_LEN		(NVT_TRANSFER_LEN+1+DUMMY_BYTES)
#define BLOCK_64KB_NUM 4

typedef enum {
	NVTWRITE = 0,
	NVTREAD  = 1
} NVT_SPI_RW;

#if defined(CONFIG_EXYNOS_DPU30)
int get_lcd_info(char *arg);
#endif
#if LCD_SETTING
extern unsigned int lcdtype;
#endif

//---extern structures---
extern struct nvt_ts_data *ts;

//---extern functions---
int32_t CTP_SPI_READ(struct spi_device *client, uint8_t *buf, uint16_t len);
int32_t CTP_SPI_WRITE(struct spi_device *client, uint8_t *buf, uint16_t len);
void nvt_bootloader_reset(void);
void nvt_eng_reset(void);
void nvt_sw_reset(void);
void nvt_sw_reset_idle(void);
void nvt_boot_ready(void);
void nvt_bld_crc_enable(void);
void nvt_fw_crc_enable(void);
void nvt_tx_auto_copy_mode(void);
/*Tab A8 code for AX6300DEV-1494 by luxinjun at 2021/11/03 start*/
int32_t nvt_update_firmware(const char *firmware_name, uint8_t full);
/*Tab A8 code for AX6300DEV-1494 by luxinjun at 2021/11/03 end*/
int32_t nvt_check_fw_reset_state(RST_COMPLETE_STATE check_reset_state);
int32_t nvt_get_fw_info(void);
int32_t nvt_clear_fw_status(void);
int32_t nvt_check_fw_status(void);
int32_t nvt_check_spi_dma_tx_info(void);
int32_t nvt_set_page(uint32_t addr);
int32_t nvt_write_addr(uint32_t addr, uint8_t data);
#if NVT_TOUCH_ESD_PROTECT
extern void nvt_esd_check_enable(uint8_t enable);
#endif /* #if NVT_TOUCH_ESD_PROTECT */

extern enum tp_module_used tp_is_used;
/*Tab A7 T618 code for SR-AX6189A-01-700 by suyurui at 2021/12/29 start*/
#if defined(HQ_D85_BUILD) || defined(FTY_TP_GESTURE)
int nvt_ts_mode_switch(struct nvt_ts_data *ts, u8 cmd, bool print_log);
#endif
/*Tab A7 T618 code for SR-AX6189A-01-700 by suyurui at 2021/12/29 end*/
#endif /* _LINUX_NVT_TOUCH_H */
/*Tab A8 code for SR-AX6300-01-405 by luxinjun at 2021/09/09 end*/
