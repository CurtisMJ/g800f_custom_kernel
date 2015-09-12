/* include/linux/drv2603_vibrator.h
 *
 * Copyright (C) 2011 Samsung Electronics Co. Ltd. All Rights Reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#ifndef _LINUX_SEC_VIBRATOR_H
#define _LINUX_SEC_VIBRATOR_H

#include <linux/timed_output.h>
#include <linux/hrtimer.h>

struct dc_motor_platform_data {
	int max_timeout;
	void (*power) (bool on);
};

struct dc_motor_drvdata {
	struct timed_output_dev dev;
	struct hrtimer timer;
	struct work_struct work;
	void (*power) (bool on);
	spinlock_t lock;
	bool running;
	int timeout;
	int max_timeout;
};

#endif
