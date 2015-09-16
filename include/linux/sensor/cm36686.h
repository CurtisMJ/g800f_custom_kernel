#ifndef __LINUX_CM36686_H
#define __CM36686_H__

#include <linux/types.h>
#include <linux/wakelock.h>

#ifdef __KERNEL__
#define CYTTSP5_DT2W
struct cm36686_platform_data {
	void (*cm36686_light_power)(bool);
	void (*cm36686_proxi_power)(bool);
	void (*cm36686_led_on)(bool);
	int irq;		/* proximity-sensor irq gpio */
	int default_hi_thd;
	int default_low_thd;
	int cancel_hi_thd;
	int cancel_low_thd;
};

/* driver data */
struct cm36686_data {
	struct i2c_client *i2c_client;
	struct wake_lock prx_wake_lock;
	struct input_dev *proximity_input_dev;
	struct input_dev *light_input_dev;
	struct cm36686_platform_data *pdata;
	struct mutex power_lock;
	struct mutex read_lock;
	struct hrtimer light_timer;
	struct hrtimer prox_timer;
	struct workqueue_struct *light_wq;
	struct workqueue_struct *prox_wq;
	struct work_struct work_light;
	struct work_struct work_prox;
	struct device *proximity_dev;
	struct device *light_dev;
	ktime_t light_poll_delay;
	ktime_t prox_poll_delay;
	int irq;
	u8 power_state;
	int avg[3];
	u16 als_data;
	u16 white_data;
	int count_log_time;
	unsigned int uProxCalResult;

	void (*cm36686_light_vddpower)(bool);
	void (*cm36686_proxi_vddpower)(bool);
#ifdef CYTTSP5_DT2W
	bool single_pump_l;
	bool single_pump_p;
#endif
};
#ifdef CYTTSP5_DT2W
void cm36686_pump_data(struct cm36686_data *cd);
#endif
#endif
#endif
