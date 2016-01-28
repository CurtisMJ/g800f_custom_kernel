/*
 * Copyright (c) 2013 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * EXYNOS - GPIO setting in set board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/gpio.h>
#include <linux/serial_core.h>
#include <plat/devs.h>
#include <plat/gpio-cfg.h>
#include <plat/regs-serial.h>
#include <mach/gpio.h>
#include <plat/cpu.h>
#include <mach/pmu.h>
#include <mach/gpio-exynos.h>

#define S3C_GPIO_SLP_OUT0	((__force samsung_gpio_pull_t)0x00)
#define S3C_GPIO_SLP_OUT1	((__force samsung_gpio_pull_t)0x01)
#define S3C_GPIO_SLP_INPUT	((__force samsung_gpio_pull_t)0x02)
#define S3C_GPIO_SLP_PREV	((__force samsung_gpio_pull_t)0x03)

#define S3C_GPIO_SETPIN_ZERO	0
#define S3C_GPIO_SETPIN_ONE	1
#define S3C_GPIO_SETPIN_NONE	2

#define GPIO_TABLE(_ptr) \
	{.ptr = _ptr, \
	.size = ARRAY_SIZE(_ptr)} \

#define GPIO_TABLE_NULL \
	{.ptr = NULL, \
	.size = 0} \

struct sec_gpio_init_data {
	uint num;
	uint cfg;
	uint val;
	uint pud;
	uint drv;
};

/*
 * shannon222ap GPIO Init Table
 */
static struct sec_gpio_init_data shannon222ap_init_table[] = {
	{EXYNOS4_GPA1(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPA1(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

	{EXYNOS4_GPB(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NFC_SDA */
	{EXYNOS4_GPB(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NFC_SCL */

	{EXYNOS4_GPC0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* ES305_RESET */

	{EXYNOS4_GPD0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SENSOR_I2C_SDA */
	{EXYNOS4_GPD0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SENSOR_I2C_SCL */

	{EXYNOS4_GPD1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CODEC_I2C_SDA */
	{EXYNOS4_GPD1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CODEC_I2C_SCL */

	{EXYNOS4_GPX3(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF0(7), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* FG_SDA */
	{EXYNOS4_GPF1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
        {EXYNOS4_GPF1(4), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NFC_EN */
	{EXYNOS4_GPF1(5), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* FG_SCL */
	{EXYNOS4_GPF1(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF1(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF2(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF2(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF2(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF2(7), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF3(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF3(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPF3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
		
	{EXYNOS4_GPK0(3), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(0) */
    {EXYNOS4_GPK0(4), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(1) */
	{EXYNOS4_GPK0(5), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(2) */
	{EXYNOS4_GPK0(6), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(3) */
	{EXYNOS4_GPL0(0), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(4) */
	{EXYNOS4_GPL0(1), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(5) */
	{EXYNOS4_GPL0(2), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(6) */
	{EXYNOS4_GPL0(3), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NAND_D(7) */
	
    {EXYNOS4_GPK1(0), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_CLK */
	{EXYNOS4_GPK1(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_CMD */
	{EXYNOS4_GPK1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_DAT0 */
	{EXYNOS4_GPK1(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_DAT1 */
	{EXYNOS4_GPK1(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_DAT2 */
	{EXYNOS4_GPK1(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* WLAN_SDIO_DAT3 */

    {EXYNOS4_GPK2(1), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_CMD */
	{EXYNOS4_GPK2(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
    {EXYNOS4_GPK2(3), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_DATA_0 */
    {EXYNOS4_GPK2(4), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_DATA_1 */
    {EXYNOS4_GPK2(5), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_DATA_2 */
    {EXYNOS4_GPK2(6), S3C_GPIO_SFN(2), S3C_GPIO_SETPIN_NONE,
        S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_DATA_3 */

	{EXYNOS4_GPK3(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPK3(6), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

	{GPIO_DOC_SWITCH, S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPIO_DOC_SWITCH */
	{GPIO_UART_SEL, S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPIO_UART_SEL */
#if defined(CONFIG_MUIC_I2C_USE_I2C17_EMUL)
	{GPIO_MUIC_SDA, S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPIO_MUIC_SDA */
	{GPIO_MUIC_SCL, S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPIO_MUIC_SCL */
#endif /* CONFIG_MUIC_I2C_USE_I2C17_EMUL */
	{GPIO_MUIC_IRQ, S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPIO_MUIC_IRQ */

	{EXYNOS4_GPX0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* ES305_WAKEUP */

	{EXYNOS4_GPX0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDHI0_CD */
#if defined(CONFIG_MACH_DELOSLTE_KOR_SKT) || defined(CONFIG_MACH_DELOSLTE_KOR_KT) || defined(CONFIG_MACH_DELOSLTE_KOR_LGT)
	{EXYNOS4_GPX0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* GYRO_INT */
#endif
	{EXYNOS4_GPX1(0), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* LOW_BAT_DET */
	{EXYNOS4_GPX1(2), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* MAIN_CAM_RST */
	{EXYNOS4_GPX1(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CHG_INT */
	{EXYNOS4_GPX2(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* VBUS_INT */

	{EXYNOS4_GPX2(5), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* GPS_EN */

	{EXYNOS4_GPX3(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* LCD_ESD_DETECT */
	{EXYNOS4_GPX3(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* CHG_DET ->NC ver 1.0 */

	{EXYNOS4_GPY0(0), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
		S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* SDCARD_PWR_EN */
	{EXYNOS4_GPY0(1), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(3), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(4), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY0(5), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */
	{EXYNOS4_GPY1(2), S3C_GPIO_INPUT, S3C_GPIO_SETPIN_NONE,
		S3C_GPIO_PULL_DOWN, S5P_GPIO_DRVSTR_LV1}, /* NC */

        {EXYNOS4_GPM0(7), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CODEC_LDO_EN */

        {EXYNOS4_GPM1(2), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CAM_VT_nRST */ 
        {EXYNOS4_GPM1(3), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CAM_VT_nSTBY */ 
        {EXYNOS4_GPM1(5), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* NFC_FIRMWARE */ 
                
        {EXYNOS4_GPM3(4), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CAM_FLASH_EN */ 
        {EXYNOS4_GPM3(6), S3C_GPIO_OUTPUT, S3C_GPIO_SETPIN_ZERO,
                S3C_GPIO_PULL_NONE, S5P_GPIO_DRVSTR_LV1}, /* CAM_FLASH_SET */ 



}; /* shannon222ap_init_table */

#ifdef CONFIG_SEC_PM
extern unsigned int system_rev;

/* shannon222ap_sleep_gpio_table rev 09 */
static unsigned int shannon222ap_sleep_gpio_table_latest[][3] = {
	{EXYNOS4_GPA0(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /* BT_UART_RXD */
	{EXYNOS4_GPA0(1), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /* BT_UART_TXD */
	{EXYNOS4_GPA0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /* BT_UART_CTS */
	{EXYNOS4_GPA0(3), S5P_GPIO_PD_OUTPUT1, S5P_GPIO_PD_UPDOWN_DISABLE}, /* BT_UART_RTS */
	{EXYNOS4_GPA0(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE},	/*GPS_RXD*/
	{EXYNOS4_GPA0(5), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE},	/*GPS_TXD*/
	{EXYNOS4_GPA0(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE},	/*GPS_CTS*/
	{EXYNOS4_GPA0(7), S5P_GPIO_PD_OUTPUT1, S5P_GPIO_PD_UPDOWN_DISABLE},	/*GPS_RTS*/

	{EXYNOS4_GPA1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*TSP_SDA*/
	{EXYNOS4_GPA1(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*TSP_SCL*/
	{EXYNOS4_GPA1(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPA1(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
#if defined(CONFIG_TARGET_LOCALE_KOR)
	{EXYNOS4_GPB(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*GSENSE_SDA_1.8V*/
	{EXYNOS4_GPB(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*GSENSE_SCL_1.9V*/
#else
	{EXYNOS4_GPB(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*2MIC_SDA->NC*/
	{EXYNOS4_GPB(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*2MIC_SCL->NC*/
#endif	
	{EXYNOS4_GPB(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NFC_SDA*/
	{EXYNOS4_GPB(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NFC_SCL*/
#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
	{EXYNOS4_GPB(4), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TDMB_SPI_CLK*/
	{EXYNOS4_GPB(5), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TDMB_SPI_CS*/
	{EXYNOS4_GPB(6), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TDMB_SPI_MISO*/
	{EXYNOS4_GPB(7), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TDMB_SPI_MOSI*/
#else
	{EXYNOS4_GPB(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPB(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPB(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPB(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
#endif

	{EXYNOS4_GPC0(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CHG_SDA*/
	{EXYNOS4_GPC0(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*2MIC_RESET->NC*/
	{EXYNOS4_GPC0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CHG_SCL*/
	{EXYNOS4_GPC0(3), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*WLAN_EN*/
	{EXYNOS4_GPC0(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	{EXYNOS4_GPC1(0), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*UART_SEL*/
	{EXYNOS4_GPC1(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*MUIC_SCL*/
	{EXYNOS4_GPC1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*MUIC_SDA*/
	{EXYNOS4_GPC1(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*CAM_SDA_1.8V*/
	{EXYNOS4_GPC1(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*CAM_SCL_1.8V*/
	
	{EXYNOS4_GPD0(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPD0(1), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*LCD_BLIC_ON*/
	{EXYNOS4_GPD0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SENSOR_I2C_SDA*/
	{EXYNOS4_GPD0(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SENSOR_I2C_SCL*/

	{EXYNOS4_GPD1(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*PMIC_I2C_SDA0*/
	{EXYNOS4_GPD1(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*PMIC_I2C_SCL0*/
	{EXYNOS4_GPD1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CODEC_I2C_SDA*/
	{EXYNOS4_GPD1(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CODEC_I2C_SCL*/
	
	{EXYNOS4_GPF0(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF0(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*FG_SDA*/
	

	{EXYNOS4_GPF1(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF1(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF1(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF1(4), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NFC_EN*/
	{EXYNOS4_GPF1(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*FG_SCL*/
	{EXYNOS4_GPF1(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF1(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/


	{EXYNOS4_GPF2(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF2(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF2(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF2(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF2(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF2(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*HW_VER2*/
	{EXYNOS4_GPF2(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*HW_VER3*/
	{EXYNOS4_GPF2(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	{EXYNOS4_GPF3(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF3(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF3(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*HW_VER0*/
	{EXYNOS4_GPF3(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*HW_VER1*/
	{EXYNOS4_GPF3(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPF3(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	{EXYNOS4_GPK0(0), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_CLK*/
	{EXYNOS4_GPK0(1), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_CMD*/
	{EXYNOS4_GPK0(2), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*MMC_PWR_EN*/
	{EXYNOS4_GPK0(3), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(0)*/
	{EXYNOS4_GPK0(4), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(1)*/
	{EXYNOS4_GPK0(5), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(2)*/
	{EXYNOS4_GPK0(6), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(3)*/
	{EXYNOS4_GPK0(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*10K PD*/

	{EXYNOS4_GPL0(0), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(4)*/
	{EXYNOS4_GPL0(1), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(5)*/
	{EXYNOS4_GPL0(2), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(6)*/
	{EXYNOS4_GPL0(3), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NAND_D(7)*/
	{EXYNOS4_GPK1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*TP*/


	{EXYNOS4_GPK2(0), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_CLK*/
	{EXYNOS4_GPK2(1), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_CMD*/
	{EXYNOS4_GPK2(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK2(3), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_DATA0*/
	{EXYNOS4_GPK2(4), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_DATA1*/
	{EXYNOS4_GPK2(5), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_DATA2*/
	{EXYNOS4_GPK2(6), S5P_GPIO_PD_OUTPUT0, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDHI0_DATA3*/

	{EXYNOS4_GPK3(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/	
	{EXYNOS4_GPK3(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK3(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK3(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK3(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK3(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPK3(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*TP*/

	/*MP0_0*/
	{EXYNOS4_GPY0(0), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SDCARD_PWR_EN*/
	{EXYNOS4_GPY0(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY0(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY0(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY0(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	/*MP0_1*/
	{EXYNOS4_GPY1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	/*MP0_3*/
	{EXYNOS4_GPY3(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY3(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	/*MP0_4*/
	{EXYNOS4_GPY4(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY4(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	/*MP0_5*/
	{EXYNOS4_GPY5(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY5(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	/*MP0_6*/
	{EXYNOS4_GPY6(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPY6(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	{EXYNOS4_GPM0(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(1), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*PMIC_WARM_RESET_n*/
	{EXYNOS4_GPM0(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM0(7), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CODEC_LDO_EN*/

	{EXYNOS4_GPM1(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*TP*/
	{EXYNOS4_GPM1(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM1(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM1(5), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*NFC_FIRMWARE*/
	{EXYNOS4_GPM1(6), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*BT_WAKEUP*/

	{EXYNOS4_GPM2(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM2(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM2(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*RESET_CP->NC*/
	{EXYNOS4_GPM2(4), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*BT_EN*/

#if defined(CONFIG_TDMB) || defined(CONFIG_TDMB_MODULE)
	{EXYNOS4_GPM3(0), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TDMB_INT*/
#else
	{EXYNOS4_GPM3(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
#endif
	{EXYNOS4_GPM3(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM3(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM3(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM3(4), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CAM_FLASH_EN*/
	{EXYNOS4_GPM3(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*ISP_UART_TX(TP)*/
	{EXYNOS4_GPM3(6), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*CAM_FLASH_SET*/
	{EXYNOS4_GPM3(7), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*ISP_UART_RX(TP)*/
	
	{EXYNOS4_GPM4(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM4(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM4(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM4(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM4(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*MSENSE_RST*/
	{EXYNOS4_GPM4(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPM4(6), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*SUB_MIC_EN*/
	{EXYNOS4_GPM4(7), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*MAIN_MIC_EN*/

	{EXYNOS4_GPZ(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPZ(5), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/
	{EXYNOS4_GPZ(6), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*NC*/

	{EXYNOS4_GPZ(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*MM_I2S_CLK*/
	{EXYNOS4_GPZ(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*MM_I2S_SYNC*/
	{EXYNOS4_GPZ(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*MM_I2S_DI*/
	{EXYNOS4_GPZ(4), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_DOWN_ENABLE}, /*MM_I2S_DO*/

	/*Do not set GPX here*/

}; /* shannon222ap_sleep_gpio_table */

/* shannon222ap_sleep_gpio_table rev 09 */
static unsigned int shannon222ap_sleep_gpio_table_rev09[][3] = {
};


/* shannon222ap_sleep_gpio_table rev 08 */
static unsigned int shannon222ap_sleep_gpio_table_rev08[][3] = {
	{EXYNOS4_GPM4(7), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_DOWN_ENABLE}, /*MAIN_MIC_EN*/
};

/* shannon222ap_sleep_gpio_table rev 07 */
static unsigned int shannon222ap_sleep_gpio_table_rev07[][3] = {
	{EXYNOS4_GPA1(2), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TSP_SDA*/
	{EXYNOS4_GPA1(3), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*TSP_SCL*/

	{EXYNOS4_GPB(0), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*2MIC_SDA*/
	{EXYNOS4_GPB(1), S5P_GPIO_PD_INPUT, S5P_GPIO_PD_UPDOWN_DISABLE}, /*2MIC_SCL*/

	{EXYNOS4_GPC0(1), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*2MIC_RESET*/
	{EXYNOS4_GPM2(3), S5P_GPIO_PD_PREV_STATE, S5P_GPIO_PD_UPDOWN_DISABLE}, /*RESET_CP*/
};

struct shannon222ap_sleep_table {
	unsigned int (*ptr)[3];
	int size;
};

static struct shannon222ap_sleep_table shannon222ap_sleep_tables[] = {
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE_NULL,
	GPIO_TABLE(shannon222ap_sleep_gpio_table_rev07),/* Rev0.7 (0x07) */
	GPIO_TABLE(shannon222ap_sleep_gpio_table_rev08),/* Rev0.8 (0x08) */
	GPIO_TABLE(shannon222ap_sleep_gpio_table_rev09),/* Rev0.9 (0x09) */
        GPIO_TABLE(shannon222ap_sleep_gpio_table_latest),/* Rev1.0 (0x0A) */
};

static void config_sleep_gpio_table(int array_size,
                                    unsigned int (*gpio_table)[3])
{
        u32 i, gpio;

        for (i = 0; i < array_size; i++) {
                gpio = gpio_table[i][0];
                s5p_gpio_set_pd_cfg(gpio, gpio_table[i][1]);
                s5p_gpio_set_pd_pull(gpio, gpio_table[i][2]);
        }
}

/* To save power consumption, gpio pin set before enterling sleep */
void sec_config_sleep_gpio_table(void)
{
        int i;
        int gpio_rev;

        i = ARRAY_SIZE(shannon222ap_sleep_tables) - 1;

        gpio_rev = system_rev;

        if (i < gpio_rev) {
                pr_err("%s: Error Need sleep table : i %d - gpio_rev %d \n",
                        __func__, i, gpio_rev );
                config_sleep_gpio_table(ARRAY_SIZE(shannon222ap_sleep_gpio_table_latest)
                        ,shannon222ap_sleep_gpio_table_latest);
                return;
        }

        for (; i >= gpio_rev ; i--) {
                if (shannon222ap_sleep_tables[i].ptr == NULL)
                        continue;

                config_sleep_gpio_table(shannon222ap_sleep_tables[i].size,
                                shannon222ap_sleep_tables[i].ptr);
        }
}
#endif

int board_universal_ss222ap_init_gpio(void)
{
	u32 i, gpio;
#ifdef CONFIG_SEC_PM
	exynos_config_sleep_gpio = sec_config_sleep_gpio_table;
#endif
	for (i = 0; i < ARRAY_SIZE(shannon222ap_init_table); i++) {

		gpio = shannon222ap_init_table[i].num;

		s3c_gpio_cfgpin(gpio, shannon222ap_init_table[i].cfg);
		s3c_gpio_setpull(gpio, shannon222ap_init_table[i].pud);

		if (shannon222ap_init_table[i].val != S3C_GPIO_SETPIN_NONE)
			gpio_set_value(gpio, shannon222ap_init_table[i].val);

		s5p_gpio_set_drvstr(gpio, shannon222ap_init_table[i].drv);
	}
	return 0;
}
