/* linux/drivers/video/mdnie.c
 *
 * Register interface file for Samsung mDNIe driver
 *
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/mutex.h>
#include <linux/mm.h>
#include <linux/device.h>
#include <linux/backlight.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/lcd.h>
#include <linux/mdnie.h>
#include <linux/fb.h>
#include <linux/pm_runtime.h>
#include <linux/clk.h>
#include <linux/gpio.h>

#include <plat/fb.h>
#include <plat/regs-mdnie.h>
#include <plat/regs-fb-v4.h>
#include <plat/regs-fb.h>

#include <mach/map.h>
#include <plat/gpio-cfg.h>
#include "mdnie.h"
#if defined(CONFIG_LCD_MIPI_EA8061V)
#include "mdnie_table_kmini.h"
#endif
#include "mdnie_color_tone_shannon222.h"

#if defined(CONFIG_TDMB)
#include "mdnie_dmb.h"
#endif

#undef MDNIE_DEBUG
#ifdef MDNIE_DEBUG
#define PR_ERR(a, ...)	pr_err( "(mdnie) %s.%d : "a"\n", __func__, __LINE__, ## __VA_ARGS__ )
#define PR_INFO(a, ...)	pr_err( "(mdnie) %s.%d : "a"\n", __func__, __LINE__, ## __VA_ARGS__ )
#define PR_DEBUG(a, ...)	pr_err( "(mdnie) %s.%d : "a"\n", __func__, __LINE__, ## __VA_ARGS__ )
#else
#define PR_ERR(a, ...)	pr_err( "%s : "a"\n", __func__, ## __VA_ARGS__ )
#define PR_INFO(a, ...)	pr_info( "%s : "a"\n", __func__, ## __VA_ARGS__ )
#define PR_DEBUG(a, ...)	do {} while(0)
#endif

#define FIMD_REG_BASE			S5P_PA_FIMD0
#ifdef CONFIG_FB_EXYNOS_FIMD_V8
#define FIMD_MAP_SIZE			SZ_256K
#else
#define FIMD_MAP_SIZE			SZ_32K
#endif

static void __iomem *fimd_reg;
static struct resource *s3c_mdnie_mem;
static void __iomem *s3c_mdnie_base;

#define s3c_mdnie_read(addr)		readl(s3c_mdnie_base + addr*4)
#define s3c_mdnie_write(addr, val)	writel(val, s3c_mdnie_base + addr*4)

#define MDNIE_SYSFS_PREFIX		"/sdcard/mdnie/"
#define PANEL_COORDINATE_PATH	"/sys/class/lcd/panel/color_coordinate"

#if defined(CONFIG_TDMB)
#define SCENARIO_IS_DMB(scenario)	(scenario == DMB_NORMAL_MODE)
#else
#define SCENARIO_IS_DMB(scenario)	NULL
#endif

#define SCENARIO_IS_VIDEO(scenario)			(scenario == VIDEO_MODE)
#define SCENARIO_IS_VALID(scenario)			(SCENARIO_IS_DMB(scenario) || scenario < SCENARIO_MAX)

#define ACCESSIBILITY_IS_VALID(accessibility)	(accessibility && (accessibility < ACCESSIBILITY_MAX))
#define IS_HBM(idx)			(idx >= 6)

#define ADDRESS_IS_SCR_WHITE(address)		(address >= S3C_MDNIE_rWHITE_R && address <= S3C_MDNIE_rWHITE_B)
#define ADDRESS_IS_SCR_RGB(address)			(address >= S3C_MDNIE_rRED_R && address <= S3C_MDNIE_rGREEN_B)

#define SCR_RGB_MASK(value)				(value % S3C_MDNIE_rRED_R)

static struct class *mdnie_class;
struct mdnie_info *g_mdnie;

#ifdef CONFIG_FB_S5P_MDNIE_HIJACK
// Yank555.lu : Hijack variables
int hijack = HIJACK_DISABLED;
int black = 0;
int black_r = 0;
int black_g = 0;
int black_b = 0;
#endif

static int mdnie_write(unsigned int addr, unsigned int val)
{
	s3c_mdnie_write(addr, val);

	return 0;
}

static int mdnie_mask(void)
{
	s3c_mdnie_write(S3C_MDNIE_rRFF, 1);

	return 0;
}

static int mdnie_unmask(void)
{
	s3c_mdnie_write(S3C_MDNIE_rRFF, 0);

	return 0;
}

int s3c_mdnie_hw_init(void)
{
	s3c_mdnie_mem = request_mem_region(S3C_MDNIE_PHY_BASE, S3C_MDNIE_MAP_SIZE, "mdnie");
	if (IS_ERR_OR_NULL(s3c_mdnie_mem)) {
		pr_err("%s: fail to request_mem_region\n", __func__);
		return -ENOENT;
	}

	s3c_mdnie_base = ioremap(S3C_MDNIE_PHY_BASE, S3C_MDNIE_MAP_SIZE);
	if (IS_ERR_OR_NULL(s3c_mdnie_base)) {
		pr_err("%s: fail to ioremap\n", __func__);
		return -ENOENT;
	}

	fimd_reg = ioremap(FIMD_REG_BASE, FIMD_MAP_SIZE);
	if (fimd_reg == NULL) {
		pr_err("%s: fail to ioremap - fimd\n", __func__);
		return -ENOENT;
	}
	if (g_mdnie) {
		pr_err("%s.%d g_mdnie->enable = TRUE;\n", __func__, __LINE__);
		g_mdnie->enable = TRUE;
	}

	return 0;
}

void mdnie_s3cfb_suspend(void)
{
	if (g_mdnie) {
		g_mdnie->enable = FALSE;
		PR_DEBUG("mdnie->enable = %d", g_mdnie->enable);
	}

}

void mdnie_s3cfb_resume(void)
{
	if (g_mdnie) {
		g_mdnie->enable = TRUE;
		PR_DEBUG("mdnie->enable = %d", g_mdnie->enable);
	}
}

static void get_lcd_size(unsigned int *xres, unsigned int *yres)
{
	unsigned int cfg;
	void __iomem *base_reg = fimd_reg;

	cfg = readl(base_reg + VIDTCON2);
	*xres = ((cfg & VIDTCON2_HOZVAL_MASK) >> VIDTCON2_HOZVAL_SHIFT) + 1;
	*yres = ((cfg & VIDTCON2_LINEVAL_MASK) >> VIDTCON2_LINEVAL_SHIFT) + 1;
	*xres |= (cfg & VIDTCON2_HOZVAL_E_MASK) ? (1 << 11) : 0;	/* 11 is MSB */
	*yres |= (cfg & VIDTCON2_LINEVAL_E_MASK) ? (1 << 11) : 0;	/* 11 is MSB */
}

int s3c_mdnie_set_size(void)
{
	unsigned int cfg, xres, yres;

	get_lcd_size(&xres, &yres);

	PR_DEBUG("xyes=%d, yres=%d\n", xres, yres);

	/* Bank0 Select */
	s3c_mdnie_write(S3C_MDNIE_rR0, 0);

	/* Input Data Unmask */
	cfg = s3c_mdnie_read(S3C_MDNIE_rR1);
	cfg &= ~S3C_MDNIE_INPUT_DATA_ENABLE;
	cfg &= ~S3C_MDNIE_INPUT_HSYNC;
	s3c_mdnie_write(S3C_MDNIE_rR1, 0x55);

	/* LCD width */
	s3c_mdnie_write(S3C_MDNIE_rR3, xres);

	/* LCD height */
	s3c_mdnie_write(S3C_MDNIE_rR4, yres);

	/* unmask all */
	mdnie_unmask();

	return 0;
}

static int mdnie_send_sequence(struct mdnie_info *mdnie, const unsigned short *seq)
{
	int ret = 0, i = 0;
	const unsigned short *wbuf = NULL;

	if (IS_ERR_OR_NULL(seq)) {
		dev_err(mdnie->dev, "mdnie sequence is null\n");
		return -EPERM;
	}

	if (IS_ERR_OR_NULL(s3c_mdnie_base)) {
		dev_err(mdnie->dev, "mdnie base is null\n");
		return -EPERM;
	}

	mutex_lock(&mdnie->dev_lock);
#ifdef CONFIG_FB_S5P_MDNIE_HIJACK
	// Yank555.lu : use hijack profile if hijack is enabled
	if (hijack == HIJACK_ENABLED)
		wbuf = &tune_hijack;
	else
#endif
	wbuf = seq;

	mdnie_mask();
	while (wbuf[i] != END_SEQ) {
		ret += mdnie_write(wbuf[i], wbuf[i + 1]);
		i += 2;
	}

	mdnie_unmask();

	mutex_unlock(&mdnie->dev_lock);

	PR_DEBUG("finish = %d", i - 2);

	return ret;
}

static struct mdnie_tuning_info *mdnie_request_table(struct mdnie_info *mdnie)
{
	struct mdnie_tuning_info *table = NULL;

	mutex_lock(&mdnie->lock);

	if (ACCESSIBILITY_IS_VALID(mdnie->accessibility)) {
		table = &accessibility_table[mdnie->accessibility];
		goto exit;
	} else if (IS_HBM(mdnie->auto_brightness)) {
#if defined(CONFIG_LCD_MIPI_EA8061V)
		if((mdnie->scenario == BROWSER_MODE)||(mdnie->scenario == EBOOK_MODE))
			table = &outdoor_table[OUTDOOR_TEXT];
		else
			table = &outdoor_table[OUTDOOR];
#endif
		goto exit;
	} else if (SCENARIO_IS_DMB(mdnie->scenario)) {
#if defined(CONFIG_TDMB)
		table = &tune_dmb[mdnie->mode];
#endif
		goto exit;
	} else if (mdnie->scenario < SCENARIO_MAX) {
		table = &tuning_table[mdnie->mode][mdnie->scenario];
		goto exit;
	}

exit:
	mutex_unlock(&mdnie->lock);

	return table;
}

static void mdnie_update_sequence(struct mdnie_info *mdnie, struct mdnie_tuning_info *table)
{
	unsigned short *wbuf = NULL;
	int ret;

	PR_DEBUG("name = %s, tuning = %d", table->name, mdnie->tuning);

	if (unlikely(mdnie->tuning)) {
		ret = mdnie_request_firmware(mdnie->path, &wbuf, table->name);
		if (ret < 0 && IS_ERR_OR_NULL(wbuf))
			mdnie_send_sequence(mdnie, table->sequence);
		else
			mdnie_send_sequence(mdnie, wbuf);
		kfree(wbuf);
	} else
		mdnie_send_sequence(mdnie, table->sequence);
}

void mdnie_update(struct mdnie_info *mdnie, u8 force)
{
	struct mdnie_tuning_info *table = NULL;

	if (!mdnie->enable && !force) {
		dev_err(mdnie->dev, "mdnie state is off\n");
		return;
	}
	table = mdnie_request_table(mdnie);
	if (!IS_ERR_OR_NULL(table) && !IS_ERR_OR_NULL(table->sequence)) {
		mdnie_update_sequence(mdnie, table);
		PR_DEBUG("name = %s", table->name);
	} else {
		PR_ERR("name = %s, but FAIL", table->name);
	}

	return;
}


static void update_color_position(struct mdnie_info *mdnie, u16 idx)
{
	u8 mode, scenario, i;
	unsigned short *wbuf;

	PR_INFO("idx=%d", idx);

	mutex_lock(&mdnie->lock);

	for (mode = 0; mode < MODE_MAX; mode++) {
		for (scenario = 0; scenario < SCENARIO_MAX; scenario++) {
			wbuf = tuning_table[mode][scenario].sequence;
			if (IS_ERR_OR_NULL(wbuf))
				continue;
			i = 0;
			while (wbuf[i] != END_SEQ) {
				if (ADDRESS_IS_SCR_WHITE(wbuf[i]))
					break;
				i += 2;
			}
			if ((wbuf[i] == END_SEQ) || IS_ERR_OR_NULL(&wbuf[i + 5]))
				continue;
			if ((wbuf[i + 1] == 0xff) && (wbuf[i + 3] == 0xff) && (wbuf[i + 5] == 0xff)) {
				wbuf[i + 1] = tune_scr_setting[idx][0];
				wbuf[i + 3] = tune_scr_setting[idx][1];
				wbuf[i + 5] = tune_scr_setting[idx][2];
			}
		}
	}

	mutex_unlock(&mdnie->lock);
}

static int get_panel_coordinate(struct mdnie_info *mdnie, int *result)
{
	int ret = 0;
	char *fp = NULL;
	unsigned int coordinate[2] = { 0, };

	PR_INFO("path=%s", PANEL_COORDINATE_PATH);

	ret = mdnie_open_file(PANEL_COORDINATE_PATH, &fp);
	if (IS_ERR_OR_NULL(fp) || ret <= 0) {
		dev_info(mdnie->dev, "%s: open skip: %s, %d\n", __func__, PANEL_COORDINATE_PATH, ret);
		ret = -EINVAL;
		goto skip_color_correction;
	}

	ret = sscanf(fp, "%d, %d", &coordinate[0], &coordinate[1]);
	if (!(coordinate[0] + coordinate[1]) || ret != 2) {
		PR_ERR("%d, %d", coordinate[0], coordinate[1]);
		ret = -EINVAL;
		goto skip_color_correction;
	}

	ret = mdnie_calibration(coordinate[0], coordinate[1], result);
	PR_INFO("%d, %d, idx=%d", coordinate[0], coordinate[1], ret - 1);

skip_color_correction:
	mdnie->color_correction = 1;
	if (!IS_ERR_OR_NULL(fp))
		kfree(fp);

	return ret;
}

static ssize_t mode_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", mdnie->mode);
}

static ssize_t mode_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	unsigned int value = 0;
	int ret;
	int result[5] = { 0, };

	ret = kstrtoul(buf, 0, (unsigned long *)&value);
	if (ret < 0)
		return ret;

	PR_INFO("value=%d", value);

	if (value >= MODE_MAX) {
		value = STANDARD;
		return -EINVAL;
	}

	mutex_lock(&mdnie->lock);
	mdnie->mode = value;
	mutex_unlock(&mdnie->lock);

	if (!mdnie->color_correction) {
		ret = get_panel_coordinate(mdnie, result);
		if (ret > 0)
			update_color_position(mdnie, ret - 1);
	}

	mdnie_update(mdnie, 0);

	return count;
}


static ssize_t scenario_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", mdnie->scenario);
}

static ssize_t scenario_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	unsigned int value;
	int ret;

	ret = kstrtoul(buf, 0, (unsigned long *)&value);
	if (ret < 0)
		return ret;

	PR_INFO("value=%d", value);

	if (!SCENARIO_IS_VALID(value))
		value = UI_MODE;

	mutex_lock(&mdnie->lock);
	mdnie->scenario = value;
	mutex_unlock(&mdnie->lock);

	mdnie_update(mdnie, 0);

	return count;
}

static void mdnie_update_table(struct mdnie_info *mdnie)
{
	struct mdnie_tuning_info *table = NULL;
	unsigned short *wbuf = NULL;
	u8 mode, scenario,  i;
	int ret;

	if (!mdnie->enable) {
		dev_err(mdnie->dev, "mdnie state is off\n");
		return;
	}

	mutex_lock(&mdnie->lock);

	for (mode = 0; mode < MODE_MAX; mode++) {
		for (scenario = 0; scenario < SCENARIO_MAX; scenario++) {
			table = &tuning_table[mode][scenario];
			ret = mdnie_request_firmware(mdnie->path, &wbuf, table->name);
			if (ret < 0 && IS_ERR_OR_NULL(wbuf))
				goto exit;
			table->sequence = wbuf;

			PR_DEBUG("++ %s", table->name);
			i = 0;
			while (wbuf[i] != END_SEQ) {
				PR_DEBUG("0x%04x, 0x%04x\n", table->sequence[i], table->sequence[i + 1]);
				i += 2;
			}
			PR_DEBUG("-- %s is updated", table->name);
		}
	}

	mutex_unlock(&mdnie->lock);

	table = mdnie_request_table(mdnie);
	if (!IS_ERR_OR_NULL(table) && !IS_ERR_OR_NULL(table->sequence)) {
		mdnie_send_sequence(mdnie, table->sequence);
		dev_info(mdnie->dev, "%s\n", table->name);
	}

exit:
	return;
}

static ssize_t tuning_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	char *pos = buf;
	struct mdnie_tuning_info *table;
	int ret, i;
	unsigned short *wbuf;

	pos += sprintf(pos, "++ %s: %s\n", __func__, mdnie->path);

	if (!mdnie->tuning) {
		pos += sprintf(pos, "tunning mode is off\n");
		goto exit;
	}

	if (strncmp(mdnie->path, MDNIE_SYSFS_PREFIX, sizeof(MDNIE_SYSFS_PREFIX) - 1)) {
		pos += sprintf(pos, "file path is invalid, %s\n", mdnie->path);
		goto exit;
	}

	table = mdnie_request_table(mdnie);
	if (!IS_ERR_OR_NULL(table)) {
		ret = mdnie_request_firmware(mdnie->path, &wbuf, table->name);
		i = 0;
		while (wbuf[i] != END_SEQ) {
			pos += sprintf(pos, "0x%04x, 0x%04x\n", wbuf[i], wbuf[i + 1]);
			i += 2;
		}
		if (!IS_ERR_OR_NULL(wbuf))
			kfree(wbuf);
		pos += sprintf(pos, "%s\n", table->name);
	}

exit:
	pos += sprintf(pos, "-- %s\n", __func__);

	return pos - buf;
}

static ssize_t tuning_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	int ret;

	if (sysfs_streq(buf, "0") || sysfs_streq(buf, "1")) {
		ret = kstrtoul(buf, 0, (unsigned long *)&mdnie->tuning);
		if (ret < 0)
			return ret;
		if (!mdnie->tuning)
			memset(mdnie->path, 0, sizeof(mdnie->path));
		PR_INFO("%s", mdnie->tuning ? "enable" : "disable");
	} else {
		if (!mdnie->tuning)
			return count;

		if (count > (sizeof(mdnie->path) - sizeof(MDNIE_SYSFS_PREFIX))) {
			dev_err(dev, "file name %s is too long\n", mdnie->path);
			return -ENOMEM;
		}

		memset(mdnie->path, 0, sizeof(mdnie->path));
		snprintf(mdnie->path, sizeof(MDNIE_SYSFS_PREFIX) + count - 1, "%s%s", MDNIE_SYSFS_PREFIX, buf);
		PR_INFO("path=%s", mdnie->path);

		mdnie_update_table(mdnie);
	}

	return count;
}

static ssize_t accessibility_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	char *pos = buf;
	unsigned short *wbuf;
	int i = 0;

	pos += sprintf(pos, "%d, ", mdnie->accessibility);
	if (mdnie->accessibility == COLOR_BLIND) {
		if (!IS_ERR_OR_NULL(accessibility_table[COLOR_BLIND].sequence)) {
			wbuf = accessibility_table[COLOR_BLIND].sequence;
			while (wbuf[i] != END_SEQ) {
				if (ADDRESS_IS_SCR_RGB(wbuf[i]))
					pos += sprintf(pos, "0x%04x, ", wbuf[i + 1]);
				i += 2;
			}
		}
	}
	pos += sprintf(pos, "\n");

	return pos - buf;
}

static ssize_t accessibility_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	unsigned int value, s[9], i, len = 0;
	int ret;
	unsigned short *wbuf;
	char str[100] = { 0, };

	ret = sscanf(buf, "%d %x %x %x %x %x %x %x %x %x",
		     &value, &s[0], &s[1], &s[2], &s[3], &s[4], &s[5], &s[6], &s[7], &s[8]);

	PR_INFO("value=%d", value);

	if (ret < 0)
		return ret;
	else {
		if (value >= ACCESSIBILITY_MAX)
			value = ACCESSIBILITY_OFF;

		mutex_lock(&mdnie->lock);
		mdnie->accessibility = value;
		if (value == COLOR_BLIND) {
			if (ret != 10) {
				mutex_unlock(&mdnie->lock);
				return -EINVAL;
			}

			wbuf = accessibility_table[COLOR_BLIND].sequence;
			if (IS_ERR_OR_NULL(wbuf))
				return count;
			i = 0;
			while (wbuf[i] != END_SEQ) {
				if (ADDRESS_IS_SCR_RGB(wbuf[i]))
					wbuf[i + 1] = s[SCR_RGB_MASK(wbuf[i])];
				i += 2;
			}

			i = 0;
			len = sprintf(str + len, "%s: ", __func__);
			while (len < sizeof(str) && i < ARRAY_SIZE(s)) {
				len += sprintf(str + len, "0x%04x, ", s[i]);
				i++;
			}
			dev_info(dev, "%s\n", str);
		}
		mutex_unlock(&mdnie->lock);

		mdnie_update(mdnie, 0);
	}

	return count;
}

static ssize_t color_correct_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	char *pos = buf;
	int i, idx, result[5] = { 0, };

	if (!mdnie->color_correction)
		return -EINVAL;

	idx = get_panel_coordinate(mdnie, result);

	for (i = 1; i < ARRAY_SIZE(result); i++)
		pos += sprintf(pos, "F%d= %d, ", i, result[i]);
	pos += sprintf(pos, "TUNE_%d\n", idx);

	return pos - buf;
}

#ifdef CONFIG_FB_S5P_MDNIE_HIJACK
/* Yank555.lu : Hijack sysfs interface (Thanx to WhatHub for the base work on n9005)
 * Modified for S5 Mini by 
 * SysFs interface :
 * -----------------
 *
 * /sys/class/mdnie/mdnie/hijack : 0 = disbaled / 1 = enabled
 *
 *    Enable / Disable hijacking of mdnie settings (default = disabled)
 *
 * /sys/class/mdnie/mdnie/sharpen : 0 = disbaled / 1 = enabled NOT ON S5 MINI
 *
 *    Enable / Disable screen sharpness (default = enabled, stock Samsung Update 12+) NOT ON S5 MINI
 *
 * /sys/class/mdnie/mdnie/black : -128 to +128
 *
 *    Global black level, will shift black RGB setting by this value (default = 0)
 *
 *    NB: Effectively applied values for black will never be below 0 nor above 255 !
 *
 *        So there is no use setting Black RGB to (0,0,0) and this to -10, the result
 *        will still be (0,0,0).
 *
 * /sys/class/mdnie/mdnie/{color}_red   : 0 to 255
 * /sys/class/mdnie/mdnie/{color}_green : 0 to 255
 * /sys/class/mdnie/mdnie/{color}_blue  : 0 to 255
 *
 *    Allows to set the RGB values for the base colors :
 *
 *      - red
 *      - green
 *      - blue
 *      - cyan
 *      - magenta
 *      - yellow
 *      - black
 *      - white
 *
 *    (default = Samsung's natural mode)
 *
 */

/* hijack */

static ssize_t hijack_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", hijack);
}

static ssize_t hijack_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	switch (new_val) {
		case HIJACK_DISABLED:
		case HIJACK_ENABLED:	hijack = new_val;
					mdnie_update(mdnie);
					return size;
		default:		return -EINVAL;
	}
}

/* sharpen */

//~ static ssize_t sharpen_show(struct device *dev, struct device_attribute *attr, char *buf)
//~ {
	//~ return sprintf(buf, "%d\n", (tune_hijack[3] & 0x0004) >> 2);
//~ }
//~ 
//~ static ssize_t sharpen_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
//~ {
	//~ int new_val;
	//~ struct mdnie_info *mdnie = dev_get_drvdata(dev);
//~ 
	//~ sscanf(buf, "%d", &new_val);
//~ 
	//~ if (new_val != ((tune_hijack[3] & 0x0004) >> 2)) {
		//~ if (new_val < 0 || new_val > 1)
			//~ return -EINVAL;
		//~ tune_hijack[3] = (new_val << 2) + (tune_hijack[3] & 0xFFFB);
		//~ if (hijack == HIJACK_ENABLED)
			//~ mdnie_update(mdnie);
	//~ }
	//~ return size;
//~ }
// NOT ON S5 MINI

/* red */

static ssize_t red_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[31] & 0xFF00) >> 8));
}

static ssize_t red_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[31] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[31] = (new_val << 8) + (tune_hijack[31] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t red_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[33] & 0xFF00) >> 8));
}

static ssize_t red_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[33] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[33] = (new_val << 8) + (tune_hijack[33] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t red_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[35] & 0xFF00) >> 8));
}

static ssize_t red_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[35] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[35] = (new_val << 8) + (tune_hijack[35] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* cyan */

static ssize_t cyan_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[31] & 0x00FF));
}

static ssize_t cyan_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[31] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[31] = new_val + (tune_hijack[31] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t cyan_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[33] & 0x00FF));
}

static ssize_t cyan_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[33] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[33] = new_val + (tune_hijack[33] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t cyan_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[35] & 0x00FF));
}

static ssize_t cyan_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[35] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[35] = new_val + (tune_hijack[35] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* green */

static ssize_t green_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[37] & 0xFF00) >> 8));
}

static ssize_t green_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[37] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[37] = (new_val << 8) + (tune_hijack[37] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t green_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[39] & 0xFF00) >> 8));
}

static ssize_t green_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[39] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[39] = (new_val << 8) + (tune_hijack[39] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t green_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[41] & 0xFF00) >> 8));
}

static ssize_t green_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[41] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[41] = (new_val << 8) + (tune_hijack[41] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* magenta */

static ssize_t magenta_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[37] & 0x00FF));
}

static ssize_t magenta_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[37] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[37] = new_val + (tune_hijack[37] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t magenta_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[39] & 0x00FF));
}

static ssize_t magenta_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[39] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[39] = new_val + (tune_hijack[39] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t magenta_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[41] & 0x00FF));
}

static ssize_t magenta_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[41] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[41] = new_val + (tune_hijack[41] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* blue */

static ssize_t blue_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[43] & 0xFF00) >> 8));
}

static ssize_t blue_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[43] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[43] = (new_val << 8) + (tune_hijack[43] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t blue_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[45] & 0xFF00) >> 8));
}

static ssize_t blue_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[45] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[45] = (new_val << 8) + (tune_hijack[45] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t blue_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", ((tune_hijack[47] & 0xFF00) >> 8));
}

static ssize_t blue_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != ((tune_hijack[47] & 0xFF00) >> 8)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[47] = (new_val << 8) + (tune_hijack[47] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* yellow */

static ssize_t yellow_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[43] & 0x00FF));
}

static ssize_t yellow_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[43] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[43] = new_val + (tune_hijack[43] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t yellow_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[45] & 0x00FF));
}

static ssize_t yellow_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[45] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[45] = new_val + (tune_hijack[45] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t yellow_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[47] & 0x00FF));
}

static ssize_t yellow_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[47] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[47] = new_val + (tune_hijack[47] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* black */

static ssize_t black_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", black);
}

static ssize_t black_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != black) {
		if (new_val < -128 || new_val > 128)
			return -EINVAL;
		black = new_val;
		tune_hijack[49] = ((max(0,min(255, black_r + black))) << 8) + (tune_hijack[49] & 0x00FF);
		tune_hijack[51] = ((max(0,min(255, black_r + black))) << 8) + (tune_hijack[51] & 0x00FF);
		tune_hijack[53] = ((max(0,min(255, black_b + black))) << 8) + (tune_hijack[53] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t black_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", black_r);
}

static ssize_t black_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != black_r) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		black_r = new_val;
		tune_hijack[49] = ((max(0,min(255, black_r + black))) << 8) + (tune_hijack[49] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t black_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", black_g);
}

static ssize_t black_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != black_g) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		black_g = new_val;
		tune_hijack[51] = ((max(0,min(255, black_r + black))) << 8) + (tune_hijack[51] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t black_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", max(0, black_b));
}

static ssize_t black_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != black_b) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		black_b = new_val;
		tune_hijack[53] = ((max(0,min(255, black_b + black))) << 8) + (tune_hijack[53] & 0x00FF);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

/* white */

static ssize_t white_red_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[49] & 0x00FF));
}

static ssize_t white_red_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[49] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[49] = new_val + (tune_hijack[49] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t white_green_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[51] & 0x00FF));
}

static ssize_t white_green_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[51] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[51] = new_val + (tune_hijack[51] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}

static ssize_t white_blue_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", (tune_hijack[53] & 0x00FF));
}

static ssize_t white_blue_store(struct device * dev, struct device_attribute * attr, const char * buf, size_t size)
{
	int new_val;
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	sscanf(buf, "%d", &new_val);

	if (new_val != (tune_hijack[53] & 0x00FF)) {
		if (new_val < 0 || new_val > 255)
			return -EINVAL;
		tune_hijack[53] = new_val + (tune_hijack[53] & 0xFF00);
		if (hijack == HIJACK_ENABLED)
			mdnie_update(mdnie);
	}
	return size;
}
#endif

static ssize_t bypass_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", mdnie->bypass);
}

static ssize_t bypass_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	struct mdnie_tuning_info *table = NULL;
	unsigned int value;
	int ret;

	ret = kstrtoul(buf, 0, (unsigned long *)&value);
	if (ret)
		return ret;

	if (ret < 0)
		return ret;
	else {
		if (value >= BYPASS_MAX)
			value = BYPASS_OFF;
		value = (value) ? BYPASS_ON : BYPASS_OFF;

		mutex_lock(&mdnie->lock);
		mdnie->bypass = value;
		mutex_unlock(&mdnie->lock);

		PR_INFO("bypass=%d", mdnie->bypass);

		table = &bypass_table[value];
		if (!IS_ERR_OR_NULL(table)) {
			mdnie_update_sequence(mdnie, table);
			dev_info(mdnie->dev, "%s\n", table->name);
		}
	}

	return count;
}

static ssize_t auto_brightness_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);

	return sprintf(buf, "%d, hbm: %d\n", mdnie->auto_brightness, mdnie->hbm);
}

static ssize_t auto_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t count)
{
	struct mdnie_info *mdnie = dev_get_drvdata(dev);
	unsigned int value;
	int ret;

	ret = kstrtoul(buf, 0, (unsigned long *)&value);
	if (ret < 0)
		return ret;

	dev_info(dev, "%s: value=%d\n", __func__, value);

	mutex_lock(&mdnie->lock);
	mdnie->hbm = IS_HBM(value) ? HBM_ON : HBM_OFF;
	mdnie->auto_brightness = value;
	mutex_unlock(&mdnie->lock);

	mdnie_update(mdnie, 0);

	return count;
}

static struct device_attribute mdnie_attributes[] = {
	__ATTR(mode, 0664, mode_show, mode_store),
	__ATTR(scenario, 0664, scenario_show, scenario_store),
	__ATTR(tuning, 0664, tuning_show, tuning_store),
	__ATTR(accessibility, 0664, accessibility_show, accessibility_store),
	__ATTR(color_correct, 0444, color_correct_show, NULL),
	__ATTR(bypass, 0664, bypass_show, bypass_store),
	__ATTR(auto_brightness, 0664, auto_brightness_show, auto_brightness_store),
#ifdef CONFIG_FB_S5P_MDNIE_HIJACK
	// Yank555.lu : add hijack sysfs interface
	__ATTR(hijack, 0664, hijack_show, hijack_store),
	//~ __ATTR(sharpen, 0664, sharpen_show, sharpen_store), NOT ON S5 MINI
	__ATTR(red_red, 0664, red_red_show, red_red_store),
	__ATTR(red_green, 0664, red_green_show, red_green_store),
	__ATTR(red_blue, 0664, red_blue_show, red_blue_store),
	__ATTR(cyan_red, 0664, cyan_red_show, cyan_red_store),
	__ATTR(cyan_green, 0664, cyan_green_show, cyan_green_store),
	__ATTR(cyan_blue, 0664, cyan_blue_show, cyan_blue_store),
	__ATTR(green_red, 0664, green_red_show, green_red_store),
	__ATTR(green_green, 0664, green_green_show, green_green_store),
	__ATTR(green_blue, 0664, green_blue_show, green_blue_store),
	__ATTR(magenta_red, 0664, magenta_red_show, magenta_red_store),
	__ATTR(magenta_green, 0664, magenta_green_show, magenta_green_store),
	__ATTR(magenta_blue, 0664, magenta_blue_show, magenta_blue_store),
	__ATTR(blue_red, 0664, blue_red_show, blue_red_store),
	__ATTR(blue_green, 0664, blue_green_show, blue_green_store),
	__ATTR(blue_blue, 0664, blue_blue_show, blue_blue_store),
	__ATTR(yellow_red, 0664, yellow_red_show, yellow_red_store),
	__ATTR(yellow_green, 0664, yellow_green_show, yellow_green_store),
	__ATTR(yellow_blue, 0664, yellow_blue_show, yellow_blue_store),
	__ATTR(black, 0664, black_show, black_store),
	__ATTR(black_red, 0664, black_red_show, black_red_store),
	__ATTR(black_green, 0664, black_green_show, black_green_store),
	__ATTR(black_blue, 0664, black_blue_show, black_blue_store),
	__ATTR(white_red, 0664, white_red_show, white_red_store),
	__ATTR(white_green, 0664, white_green_show, white_green_store),
	__ATTR(white_blue, 0664, white_blue_show, white_blue_store),
#endif
	__ATTR_NULL,
};


static int fb_notifier_callback(struct notifier_block *self,
		unsigned long event, void *data)
{
	struct mdnie_info *mdnie;
	struct fb_event *evdata = data;
	int fb_blank;

	/* If we aren't interested in this event, skip it immediately ... */
	switch (event) {
		case FB_EVENT_BLANK:
			break;
		default:
			return 0;
	}

	mdnie = container_of(self, struct mdnie_info, fb_notif);
	if (!mdnie)
		return 0;

	fb_blank = *(int *)evdata->data;

	if (fb_blank == FB_BLANK_UNBLANK) {
		pm_runtime_get_sync(mdnie->dev);

		mutex_lock(&mdnie->lock);
		mdnie->enable = TRUE;
		mutex_unlock(&mdnie->lock);

		PR_DEBUG("enable=%d, fb_blank=%d", mdnie->enable, fb_blank);

		mdnie_update(mdnie, 1);
	} else if (fb_blank == FB_BLANK_POWERDOWN) {
		mutex_lock(&mdnie->lock);
		mdnie->enable = FALSE;
		mutex_unlock(&mdnie->lock);

		PR_DEBUG("enable=%d, fb_blank=%d", mdnie->enable, fb_blank);

		pm_runtime_put_sync(mdnie->dev);
	} else {
		PR_DEBUG("called");
	}

	return 0;
}

static int mdnie_register_fb(struct mdnie_info *mdnie)
{
	memset(&mdnie->fb_notif, 0, sizeof(mdnie->fb_notif));
	mdnie->fb_notif.notifier_call = fb_notifier_callback;
	return fb_register_client(&mdnie->fb_notif);
}

static int mdnie_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct mdnie_info *mdnie;

	mdnie_class = class_create(THIS_MODULE, dev_name(&pdev->dev));
	if (IS_ERR_OR_NULL(mdnie_class)) {
		pr_err("failed to create mdnie class\n");
		ret = -EINVAL;
		goto error0;
	}

	mdnie_class->dev_attrs = mdnie_attributes;

	mdnie = kzalloc(sizeof(struct mdnie_info), GFP_KERNEL);
	if (!mdnie) {
		pr_err("failed to allocate mdnie\n");
		ret = -ENOMEM;
		goto error1;
	}

	mdnie->dev = device_create(mdnie_class, &pdev->dev, 0, &mdnie, "mdnie");
	if (IS_ERR_OR_NULL(mdnie->dev)) {
		pr_err("failed to create mdnie device\n");
		ret = -EINVAL;
		goto error2;
	}

	mdnie->scenario = UI_MODE;
	mdnie->mode = STANDARD;
	mdnie->enable = FALSE;
	mdnie->tuning = FALSE;
	mdnie->accessibility = ACCESSIBILITY_OFF;
 	mdnie->bypass = BYPASS_OFF;

	mutex_init(&mdnie->lock);
	mutex_init(&mdnie->dev_lock);

	platform_set_drvdata(pdev, mdnie);
	dev_set_drvdata(mdnie->dev, mdnie);

	pm_runtime_enable(mdnie->dev);
	pm_runtime_get_sync(mdnie->dev);

	mdnie_register_fb(mdnie);

	mdnie->enable = true;
	PR_DEBUG("enable=%d", mdnie->enable);
	mdnie_update(mdnie, 1);

	g_mdnie = mdnie;

	dev_info(mdnie->dev, "registered successfully\n");

	return 0;

error2:
	kfree(mdnie);
error1:
	class_destroy(mdnie_class);
error0:
	return ret;
}

static int mdnie_remove(struct platform_device *pdev)
{
	struct mdnie_info *mdnie = dev_get_drvdata(&pdev->dev);

	PR_DEBUG("called");

	class_destroy(mdnie_class);
	kfree(mdnie);

#if 0
	pm_runtime_disable(mdnie->dev);
#endif

	return 0;
}

static struct platform_driver mdnie_driver = {
	.driver		= {
		.name	= "mdnie",
		.owner	= THIS_MODULE,
	},
	.probe		= mdnie_probe,
	.remove		= mdnie_remove,
};

static int __init mdnie_init(void)
{
	return platform_driver_register(&mdnie_driver);
}

late_initcall(mdnie_init);

static void __exit mdnie_exit(void)
{
	platform_driver_unregister(&mdnie_driver);
}

module_exit(mdnie_exit);

MODULE_DESCRIPTION("mDNIe Driver");
MODULE_LICENSE("GPL");

