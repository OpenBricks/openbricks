/*
This is LIRC, Linux Infra-red Remote Control support for CIR Port of chips 
"NCT677x" / "W836x7" of Nuvoton Technology Co. (or Winbond in old time).

It was produced by Nuvoton PS00 group. Website : http://www.nuvoton.com

The whole thing is licensed under the GPL v2 or later.

    This package is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This package is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this package; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

Adapted for usage in GeeXboX by the GeeXboX team.
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/version.h>
#include <linux/delay.h>
#include <linux/input.h>
#include <linux/platform_device.h>
#include <linux/pnp.h>

//#include <asm/semaphore.h>
#include <linux/semaphore.h>

#include <linux/device.h>
#include <asm-generic/ioctl.h>

#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include "lirc.h"
#include "lirc_dev.h"
#include "kcompat.h"

/* allow show debug messages 
 * without additional debug setting, only show current IRQ number, I/O port and final decode output */
//#define IR_667_DEBUG

/* show debug message while decode mce keyboard and mouse signals */
#ifdef IR_667_DEBUG
//#define ALLOW_DEBUG_DECODE
#endif

/* show incoming interrupt signals
 * may block machine, mask it if sure interrupt signal is good */
#ifdef IR_667_DEBUG
//#define ALLOW_DEBUG_INTERRUPT
#endif

/* show received infrared pulse and cancel decode functions, mask it in stable version
 * it will stop decode MCE controller, keyboard and mouse
 * mask it if want to decode incoming signals
 */
#ifdef IR_667_DEBUG
//#define ALLOW_DEBUG_PRINT_PULSE
#endif

/* show debug message while set WAKE UP key
 * it will print cir_wake buf message while setting wake up key
 * mask it in stable version
 */
#ifdef IR_667_DEBUG
#define ALLOW_DEBUG_WAKE
#endif

/* show STUDY debug messages */
#ifdef IR_667_DEBUG
#define ALLOW_DEBUG_STUDY
#endif

/* show ioctl debug messages */
#ifdef IR_667_DEBUG
#define ALLOW_DEBUG_IOCTL
#endif

/* step debug */
#ifdef IR_667_DEBUG
#define ALLOW_DEBUG_STEP
#endif

#ifdef ALLOW_DEBUG_STEP
#define STEP_DBG \
	do {		\
		printk("w83667hg_ir step debug: %s, %d\n", __FILE__, __LINE__); \
	} while (0);
#else
#define STEP_DBG 
#endif

#ifdef ALLOW_DEBUG_PRINT_PULSE
/* print current received ir pulse 
 * it will output all data, clean CIR_IRSTS so make all following irq_handler ignore
 */
void debug_print_pulse(void);

void debug_print_wake_pulse(void);
#endif


/* CIR settings */

/* total length of CIR and CIR WAKE */
#define CIR_IOREG_LENGTH 0x0f

/* RX limit length, 8 high bits for SLCH, 8 low bits for SLCL
 * 0x7d0 = 2000 */
#define CIR_RX_LIMIT_COUNT 0x7d0

/* CIR Regs */
#define CIR_IRCON   0x00
#define CIR_IRSTS   0x01
#define CIR_IREN    0x02
#define CIR_RXFCONT 0x03
#define CIR_CP      0x04
#define CIR_CC      0x05
#define CIR_SLCH    0x06
#define CIR_SLCL    0x07
#define CIR_FIFOCON 0x08
#define CIR_IRFIFOSTS 0x09
#define CIR_SRXFIFO 0x0A
#define CIR_TXFCONT 0x0B
#define CIR_STXFIFO 0x0C
#define CIR_FCCH    0x0D
#define CIR_FCCL    0x0E
#define CIR_IRFSM   0x0F

/* CIR IRCON settings */
#define CIR_IRCON_RECV   0x80
#define CIR_IRCON_WIREN  0x40
#define CIR_IRCON_TXEN   0x20
#define CIR_IRCON_RXEN   0x10
#define CIR_IRCON_WRXINV 0x08
#define CIR_IRCON_RXINV  0x04

#define CIR_IRCON_SAMPLE_PERIOD_SEL_1   0x00
#define CIR_IRCON_SAMPLE_PERIOD_SEL_25  0x01
#define CIR_IRCON_SAMPLE_PERIOD_SEL_50  0x02
#define CIR_IRCON_SAMPLE_PERIOD_SEL_100 0x03

/* select sample period as 50us */
#define CIR_IRCON_SAMPLE_PERIOD_SEL CIR_IRCON_SAMPLE_PERIOD_SEL_50

/* CIR IRSTS settings */
#define CIR_IRSTS_RDR  0x80
#define CIR_IRSTS_RTR  0x40
#define CIR_IRSTS_PE   0x20
#define CIR_IRSTS_RFO  0x10
#define CIR_IRSTS_TE   0x08
#define CIR_IRSTS_TTR  0x04
#define CIR_IRSTS_TFU  0x02
#define CIR_IRSTS_GH   0x01

/* CIR IREN settings */
#define CIR_IREN_RDR  0x80
#define CIR_IREN_RTR  0x40
#define CIR_IREN_PE   0x20
#define CIR_IREN_RFO  0x10
#define CIR_IREN_TE   0x08
#define CIR_IREN_TTR  0x04
#define CIR_IREN_TFU  0x02
#define CIR_IREN_GH   0x01

/* CIR FIFOCON settings */
#define CIR_FIFOCON_TXFIFOCLR  0x80

#define CIR_FIFOCON_TX_TRIGGER_LEV_31 0x00
#define CIR_FIFOCON_TX_TRIGGER_LEV_24 0x10
#define CIR_FIFOCON_TX_TRIGGER_LEV_16 0x20
#define CIR_FIFOCON_TX_TRIGGER_LEV_8  0x30

/* select TX trigger level as 16 */
#define CIR_FIFOCON_TX_TRIGGER_LEV CIR_FIFOCON_TX_TRIGGER_LEV_16

#define CIR_FIFOCON_RXFIFOCLR  0x08

#define CIR_FIFOCON_RX_TRIGGER_LEV_1  0x00
#define CIR_FIFOCON_RX_TRIGGER_LEV_8  0x01
#define CIR_FIFOCON_RX_TRIGGER_LEV_16 0x02
#define CIR_FIFOCON_RX_TRIGGER_LEV_24 0x03

/* select RX trigger level as 1 */
#define CIR_FIFOCON_RX_TRIGGER_LEV CIR_FIFOCON_RX_TRIGGER_LEV_1

/* CIR IRFIFOSTS settings */
#define CIR_IRFIFOSTS_IR_PENDING  0x80
#define CIR_IRFIFOSTS_RX_GS       0x40
#define CIR_IRFIFOSTS_RX_FTA      0x20
#define CIR_IRFIFOSTS_RX_EMPTY    0x10
#define CIR_IRFIFOSTS_RX_FULL     0x08
#define CIR_IRFIFOSTS_TX_FTA      0x04
#define CIR_IRFIFOSTS_TX_EMPTY    0x02
#define CIR_IRFIFOSTS_TX_FULL     0x01


/* CIR WAKE UP Regs */
#define CIR_WAKE_IRCON   0x00
#define CIR_WAKE_IRSTS   0x01
#define CIR_WAKE_IREN    0x02
#define CIR_WAKE_FIFO_CMP_DEEP 0x03
#define CIR_WAKE_FIFO_CMP_TOL  0x04
#define CIR_WAKE_FIFO_COUNT    0x05
#define CIR_WAKE_SLCH    0x06
#define CIR_WAKE_SLCL    0x07
#define CIR_WAKE_FIFOCON 0x08
#define CIR_WAKE_SRXFSTS 0x09
#define CIR_WAKE_SAMPLE_RX_FIFO 0x0A
#define CIR_WAKE_WR_FIFO_DATA   0x0B
#define CIR_WAKE_RD_FIFO_ONLY   0x0C
#define CIR_WAKE_RD_FIFO_ONLY_IDX 0x0D
#define CIR_WAKE_FIFO_IGNORE 0x0E
#define CIR_WAKE_IRFSM   0x0F

/* CIR WAKE UP IRCON settings */
#define CIR_WAKE_IRCON_DEC_RST 0x80
#define CIR_WAKE_IRCON_MODE1   0x40
#define CIR_WAKE_IRCON_MODE0   0x20
#define CIR_WAKE_IRCON_RXEN    0x10
#define CIR_WAKE_IRCON_R       0x08
#define CIR_WAKE_IRCON_RXINV   0x04

/* select a same sample period like cir register */
#define CIR_WAKE_IRCON_SAMPLE_PERIOD_SEL CIR_IRCON_SAMPLE_PERIOD_SEL_50

/* CIR WAKE UP IRSTS settings */
#define CIR_WAKE_IRSTS_RDR        0x80
#define CIR_WAKE_IRSTS_RTR        0x40
#define CIR_WAKE_IRSTS_PE         0x20
#define CIR_WAKE_IRSTS_RFO        0x10
#define CIR_WAKE_IRSTS_GH         0x08
#define CIR_WAKE_IRSTS_IR_PENDING 0x01

/* CIR WAKE UP IREN settings */
#define CIR_WAKE_IREN_RDR  0x80
#define CIR_WAKE_IREN_RTR  0x40
#define CIR_WAKE_IREN_PE   0x20
#define CIR_WAKE_IREN_RFO  0x10
#define CIR_WAKE_IREN_TE   0x08
#define CIR_WAKE_IREN_TTR  0x04
#define CIR_WAKE_IREN_TFU  0x02
#define CIR_WAKE_IREN_GH   0x01

/* CIR WAKE FIFOCON settings */
#define CIR_WAKE_FIFOCON_RXFIFOCLR  0x08

#define CIR_WAKE_FIFOCON_RX_TRIGGER_LEV_67 0x00
#define CIR_WAKE_FIFOCON_RX_TRIGGER_LEV_66 0x01
#define CIR_WAKE_FIFOCON_RX_TRIGGER_LEV_65 0x02
#define CIR_WAKE_FIFOCON_RX_TRIGGER_LEV_64 0x03

/* select WAKE UP RX trigger level as 67 */
#define CIR_WAKE_FIFOCON_RX_TRIGGER_LEV CIR_WAKE_FIFOCON_RX_TRIGGER_LEV_67

/* CIR WAKE SRXFSTS settings */
#define CIR_WAKE_IRFIFOSTS_RX_GS       0x80
#define CIR_WAKE_IRFIFOSTS_RX_FTA      0x40
#define CIR_WAKE_IRFIFOSTS_RX_EMPTY    0x20
#define CIR_WAKE_IRFIFOSTS_RX_FULL     0x10




/* Config Regs */

/* Chip Control Regs */
#define cr_cfg_idx    0x2e
#define cr_cfg_dat    0x2f

#define cr_cfg_idx2   0x4e
#define cr_cfg_dat2   0x4f

static u8 CFG_idx = cr_cfg_idx;
static u8 CFG_dat = cr_cfg_dat;

#define CHIP_ID_HIGH_ADDR 0x20
#define CHIP_ID_LOW_ADDR  0x21

#define CHIP_ID_HIGH 0xB4
#define CHIP_ID_LOW  0x73

#define ACTIVE_LOG_DEV   0x01
#define DEACTIVE_LOG_DEV 0x0

#define CIR_LOG_DEV 0x06

#define ACPI_LOG_DEV 0x0A

#define ENABLE_CIR_WAKE 0x08
#define DISABLE_CIR_WAKE 0xF7

#define ENABLE_CIR_INTR_OF_MOUSE_IRQ 0x80
#define DISABLE_CIR_INTR_OF_MOUSE_IRQ 0x7F

#define ENABLE_PME_INTR_OF_CIR_PASS 0x08
#define DISABLE_PME_INTR_OF_CIR_PASS 0xF7

#define CIR_WAKE_LOG_DEV 0x0E


/* read/update registers functions */

/* enter extended function mode */
static inline void cr_enter_ext(void);

/* exit extended function mode */
static inline void cr_exit_ext(void);

/* select logical device */
static inline void cr_select_log_dev(int cr);

static inline void cr_update(int dat, int cr);

static inline u8 cr_read(int cr);

/* update cr register without change other bits */
static inline void cr_safe_update(u8 dat, int cr);

/* clear cr register without change other bits */
static inline void cr_safe_clear(u8 dat, int cr);



/* read/write cir registers */

static inline void cir_update(u8 dat, int cir);

static u8 cir_read(int cir);

/* read/write cir wake registers */

static inline void cir_wake_update(u8 dat, int cir);

static u8 cir_wake_read(int cir);


/* dump current cir registers */
static void cir_dump_reg(void);

/* dump current cir wake up registers */
static void cir_wake_dump_reg(void);





/* driver module load/unload functions */

/* Config Registers init */
/*
 * initialize cr, cir, apci, cir wake logical devices
 * open these devices and irq
 */
static int w83667hg_cr_init(void);

/* Config Registers uninit */
/*
 * close cir and cir wake logical devices
 */
static void w83667hg_cr_uninit(void);


/* register input device (keyboard and mouse)
 *
 */

/* this is the keycode table of US-EN layout keyboard
 * modify it to support other layout keyboard
 */
static unsigned int usb_kbd_keycode[256] = {
	  0,  0,  0,  0, 30, 48, 46, 32, 18, 33, 34, 35, 23, 36, 37, 38,
	 50, 49, 24, 25, 16, 19, 31, 20, 22, 47, 17, 45, 21, 44,  2,  3,
	  4,  5,  6,  7,  8,  9, 10, 11, 28,  1, 14, 15, 57, 12, 13, 26,
	 27, 43, 43, 39, 40, 41, 51, 52, 53, 58, 59, 60, 61, 62, 63, 64,
	 65, 66, 67, 68, 87, 88, 99, 70,119,110,102,104,111,107,109,106,
	105,108,103, 69, 98, 55, 74, 78, 96, 79, 80, 81, 75, 76, 77, 71,
	 72, 73, 82, 83, 86,127,116,117,183,184,185,186,187,188,189,190,
	191,192,193,194,134,138,130,132,128,129,131,137,133,135,136,113,
	115,114,  0,  0,  0,121,  0, 89, 93,124, 92, 94, 95,  0,  0,  0,
	122,123, 90, 91, 85,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 29, 42, 56,125, 97, 54,100,126,164,166,165,163,161,115,114,113,
	150,158,159,128,136,177,178,176,142,152,173,140
};

/* input device name, locate in /sys/input/inputNUM/name */
static char *INPUTNAME = "MCE Remote Keyboard";

static struct input_id w83667hg_input_id = {
	.bustype = BUS_HOST, /* comunicate 667 by HOST, or I2C ? */
	.vendor  = 0x1050, /* Winbond PCI VENDOR ID */
	.product = 1, /* for basic devices, like keyboard & mouse */
	.version = 0, /* ignore it */
};


static int w83667hg_input_open(struct input_dev *cur_input_dev);

static void w83667hg_input_close(struct input_dev *cur_input_dev);

static struct input_dev *w83667hg_input_init(void);

static void w83667hg_input_uninit(struct input_dev *cur_input_dev);

/* internal call for register lirc */
static int lirc_set_use_inc(void *data);

static void lirc_set_use_dec(void *data);

/* register lirc device & buffer */

/* mininum value 76, recommend 256 */
#define IRCTL_BUF_LIMIT 2048

static struct irctl {
	struct input_dev *input_dev; /* allocate by input_init */
	struct lirc_buffer *lirc_buffer;
	struct lirc_driver *lirc_plugin;
	spinlock_t lock;
	/* for controller */
	lirc_t lircdata;
	u8 ctrl_fix_head; // fix controller not sync problem
	/* for keyboard */
	u8 pressed_keycode;	// initialize as 0
	u8 pressed_shiftmask;	// initialize as 0
	/* for decode */
	u8 buf[IRCTL_BUF_LIMIT]; 
	unsigned int buf_count; // init as 0, reset 0 after decode
	unsigned int cur_buf_num; // init as 0, set as 0 while clean
	/* for study */
#define ST_STUDY_NONE      0x0
#define ST_STUDY_START     0x01
#define ST_STUDY_CARRIER   0x02
#define ST_STUDY_ALL_RECV  0x04
	u8 study_state;
	/* carrier period = 1 / frequency */
	unsigned int carrier;
	wait_queue_head_t queue;
} w83667hg_irctl;

static struct ir_recv_t {
	spinlock_t lock;
	u8 buf[IRCTL_BUF_LIMIT];
	unsigned int buf_count;
	wait_queue_head_t queue;
	/* for wake */
#define ST_WAKE_NONE      0x0
#define ST_WAKE_START     0x01
#define ST_WAKE_FINISH    0x02
	u8 wake_state;
	/* for recv */
#define ST_RECV_WAIT_7F    0x01
#define ST_RECV_WAIT_HEAD  0x02
#define ST_RECV_WAIT_SILENT_END  0x04
	u8 recv_state;
} w83667hg_ir_recv;

static struct ir_send_t {
	spinlock_t lock;
	u8 buf[IRCTL_BUF_LIMIT];
	unsigned int buf_count;
	unsigned int cur_buf_num;
	wait_queue_head_t queue;
	/* for send */
#define ST_SEND_NONE      0x0
#define ST_SEND_REQUEST   0x02
#define ST_SEND_REPLY     0x04
	u8 send_state;
} w83667hg_ir_send;

static struct input_dev *w83667hg_input_dev = NULL;

//static struct lirc_plugin *w83667hg_lirc_plugin = NULL;
static struct lirc_driver *w83667hg_lirc_plugin = NULL;

#define LIRCBUF_SIZE 256

static struct lirc_buffer *w83667hg_lirc_buffer = NULL;

static void w83667hg_ir_recv_init(struct ir_recv_t *ir_recv);

static void w83667hg_ir_send_init(struct ir_send_t *ir_send);

static int w83667hg_irctl_init(struct irctl *ir);

static void w83667hg_irctl_uninit(struct irctl *ir);


#define BIT_PULSE_MASK 0x80
#define BIT_LEN  0x7f

/* MCE CIR signal length, related on sample period */

/* MCE CIR controller signal length: about 43ms 
 * 43ms / 50us (sample period) * 0.85 (inaccuracy)
 */
#define CONTROLLER_BUF_LEN_MIN 830

/* MCE CIR keyboard signal length: about 26ms
 * 26ms / 50us (sample period) * 0.85 (inaccuracy)
 */
#define KEYBOARD_BUF_LEN_MAX 650
#define KEYBOARD_BUF_LEN_MIN 610

/* MCE CIR mouse signal length: about 24ms 
 * 24ms / 50us (sample period) * 0.85 (inaccuracy)
 */
#define MOUSE_BUF_LEN_MIN 565 


#define CIR_SAMPLE_PERIOD 50
#define CIR_SAMPLE_LOW_INACCURACY 0.85


/* "10"=>1 (+2), "01"=>0 (+2) */
#define ST_DECODE_NEW  0x01
#define ST_DECODE_ZERO 0x02
#define ST_DECODE_ONE  0x04

/* RC6 sync head: 1111 11 
 * 6 * 444us / 50us (sample period) * 0.85 (inaccuracy)
 */
#define HEAD_SYNC_LEN_LOW  45 
/* 7 pulses long, for inaccuracy */
#define HEAD_SYNC_LEN_HIGH 71 

#define ONE_PULSE_LEN_LOW   2
#define ONE_PULSE_LEN_HIGH  10
#define TWO_PULSE_LEN_LOW   10
#define TWO_PULSE_LEN_HIGH  15


/* fops */
/* set carrier frequency by ioctl IR_IOSETCARRIER
 *
 * that value can be get by ioctl value IR_IOGETCARRIER
 * on 667, set carrier on 2 registers: CP & CC
 * always set CP as 0x81
 * set CC by SPEC, CC = 3MHz/carrier - 1
 */
static int w83667hg_set_carrier(unsigned int *carrier);

static int lirc_ioctl(struct inode *node,struct file *filep,unsigned int cmd,
		unsigned long arg);

/* lirc_write
 *
 * 1) clean TX fifo first (handled by AP)
 * 2) copy data from user space 
 * 3) open TX interrupt: TTR & TFU
 * 4) send 9 pulses to open TTR
 * in interrupt_handler:
 * 5) send all data out
 * go back to write():
 * 6) close TX interrupt
 *
 * The key problem of this function is user space data may larger than 
 * driver's data buf length. So lirc_wirte() only copy IRCTL_BUF_LIMIT data to
 *  buf. And keep current copied data buf num in cur_buf_num. But driver's buf
 *  number may larger than TXFCONT (0xFF). So in interrupt_handler, it has to 
 * set TXFCONT as 0xff, until buf_count less than 0xff.
 * The confustion is cur_buf_num in decode, study and send is designed on 
 * different meaning.
 *
 */
static ssize_t lirc_write(struct file *file, const char *buf, size_t n, loff_t * ppos);



/* 1) clean lircdata and buf_count in irctl struct 
 * 2) clean RX and TX fifo
 */
static void w83667hg_clean_data(struct ir_recv_t *ir_recv, struct irctl *ir);



/* jump asked frame number, can use on key, mouse and controller signals
 *
 * after jump frame_num, current frame number = frame_num + 1
 * record current buf number of irctl buf
 * record current pulse's bit and decode_status
 *
 * problem 1: 
 * When jump_frame() stops on a two_width_len pulse, the buf_num will be the 
 * second pulse of the double_width_len pulse.
 * So jump_frame() must increase buf_num 1 on problem 1.
 * For sample period is 50us, a 444us piulse's minimal sample value is 7, and 
 * two piulses' minimal sample value is 15. So just increase buf_num 1 works 
 * fine. If sample period changes, this solution also need change.
 *
 * one_width_len pulse:
 *   _   _
 * .| |_| |_.      (1010)
 *     ^
 *     |
 *   stop: jumped frame_num
 *
 *
 * problem 1: double_width_len pulse:
 *   __    _
 * .|  |__| |.      (11001)
 *       ^
 *       |
 *      stop: jumped frame_num + 1, buf_num + 1
 *
 * so if cur buf len > TWO_PULSE_LEN_LOW, buf_num --
 * and recheck polar bit and decode_status
 * decode_status go back 1 buf,
 * NEW -> ONE/ZERO; ONE/ZERO -> NEW
 *
 * jump out of buf limit, return < 0
 */
static int w83667hg_jump_frame(struct irctl *ir, u8 frame_num, unsigned int *buf_num, u8 *out_bit, u8 *decode_status);

/* jump MCE head sync signal (1111 11) 
 *
 * after jump signal head, current pulse number = 7
 * record current buf number of irctl buf
 * record current pulse's bit and decode_status
 *
 * jump out of buf limit, return < 0
 */
static int w83667hg_jump_head(struct irctl *ir, unsigned int *buf_num);

#define KEY_IDENTIFICATION        0x04
#define MOUSE_IDENTIFICATION      0x01
#define IDENTIFICATION_CHECK_BIT  0xF

/* check mce identification
 *
 * compare identification and cur_iden
 * same return 0
 * else return < 0, need continue check
 */
static int w83667hg_check_identification(u8 val, u8 identification, u8 *cur_iden);

/* jump mce identification
 * can use on key, mouse and controller signals
 *
 * jump and search identification parts
 * record current buf number of irctl buf
 *
 * jump all pulse until find identification
 *
 *
 *
 * problem 1: 
 * When jump_identification() stops on a double_width_len pulse, the buf_num will be the 
 * second pulse of the doule_width_len frame.
 *
 * one_width_len pulse:
 *   _    _
 * .| |_| |_.      (1010)
 *     ^
 *     |
 *   stop: jumped frame_num
 *
 *
 * problem 1: double_width_len pulse:
 *   __    _
 * .|  |__| |.      (11001)
 *       ^
 *       |
 *      stop: jumped frame_num + 1, buf_num + 2
 *
 * solution:
 * Now buf_num is the next double-pulse-width buf.
 * Just decrease buf_num by one can fix this problem.
 *
 *
 * After jump iden, output may diff by decode status:
 * 1) 
 * For status ONE and ZERO, all decode work is done.
 * Set status as NEW and inverse out_bit
 * in fact, bit is not important, for a buf can indicate one frame, whether its
 * length is one or two, the bit can be ignored
 *
 * after jump frame, all status is NEW! reset them as inverse polar bit
 *
 *
 *
 *
 * can not find identification, return < 0
 */
static int w83667hg_jump_identification(struct irctl *ir, u8 identification, unsigned int *buf_num, u8 *out_bit, u8 *decode_status);


/* the frame number between identification and code */
#define KEY_KEYCODE_FRAME_AFTER_I_START   16  //32
#define KEY_FRAME_AFTER_JUMP              16//64
#define KEY_SUBMIT_KEYCODE_FRAME_NUM      8


#define MOUSE_DELTAY_PULSE_AFTER_I_START  8
#define MOUSE_CHECKSUM_FRAME_AFTER_JUMP   16
#define MOUSE_SUBMIT_DELTAY_FRAME_NUM     7
#define MOUSE_SUBMIT_DELTAX_FRAME_NUM     14
#define MOUSE_SUBMIT_R_FRAME_NUM          15
#define MOUSE_SUBMIT_L_FRAME_NUM          16


// FIXME, not sure this value, select a large number
#define PULSE_SILENT_LEN_LOW  90


/* set keycode and shiftmask for keyboard */
static void w83667hg_set_key(u8 *set_frame, u8 val, u8 *keycode, u8 *shiftmask);

/* decode mce keyboard signal and send data to input
 *
 * 1) jump mce keyboard signal sync head (1111 11)
 * 2) jump identification (0100)
 * 	pulse number before identification is not sure
 * 3) jump "unknown", "C" and "CodeSet" parts in mce keyboard signal
 * 4) deocde "KeyCode" and "ShiftMask" parts in mce key signal
 * 5) send keycode to input by array usb_kbd_keycode
 * 6) send shiftmask to input by array usb_kbd_keycode
 */
static void w83667hg_submit_key(struct irctl *ir);

/* set deltax, deltay, x, y for mce mouse */
static void	w83667hg_set_mouse(u8 *set_frame, u8 val, u8 *deltay, u8 *deltax, u8 *clickrl);

/* decode mce mouse signal and send data to input
 *
 * 1) jump mce mouse signal sync head (1111 11)
 * 2) jump identification (0001)
 * 	pulse number before identification is not sure
 * 3) jump "unknown" part in mce mouse signal
 * 4) deocde "deltay", "deltax", "r" and "l" parts in mce mouse signal
 * 5) send these parts to input
 */
static void w83667hg_submit_mouse(struct irctl *ir);


/* send 1 recorded controller signal to lirc */
static void w83667hg_send_packet_to_lirc_1(struct irctl *ir, lirc_t *val);

/* this value copy from lirc_mod_mce */
#if CIR_IRCON_SAMPLE_PERIOD_SEL ==  CIR_IRCON_SAMPLE_PERIOD_SEL_100
	#define MCE_TIME_UNIT 100
#elif CIR_IRCON_SAMPLE_PERIOD_SEL ==  CIR_IRCON_SAMPLE_PERIOD_SEL_50
	#define MCE_TIME_UNIT 50
#elif CIR_IRCON_SAMPLE_PERIOD_SEL ==  CIR_IRCON_SAMPLE_PERIOD_SEL_25
	#define MCE_TIME_UNIT 25
#else
	#define MCE_TIME_UNIT 1
#endif

/* recode mce controller signal and send to lirc
 *
 * recode steps:
 * 1) high pulse, +128
 * 2) low pulse, do nothing
 * 3) keycode = pulse * 50
 * 4) sync head: PULSE_MASK (0xFF FFFF)
 * 5) 1, |PULSE_BIT (0x100 0000)
 *    0, +keycode
 *
 * function steps:
 * 0) clean lircdata[]
 * 1) jump head, put PULSE_BIT in lircdata[0]
 * 2) normal decode, "10"=>1 |PULSE_BIT
 *    save in lircdata[]
 * 3) send lircdata[] to lirc
 */
static void w83667hg_submit_controller(struct irctl *ir);


/* copy data from hardware fifo to driver buf 
 *
 * 1. initial state is WAIT_7F 
 * 2. if data is not 0x7f, copy data from hardware fifo to struct ir_recv buf
 * 3. if data is 0x7f, set recv_state as WAIT_HEAD, copy data from ir_recv to irctl
 * 4. depend on received buf length to select submit_ function
 * 5. set irctl buf len as 0
 * 5. receive data until data is not 0x7f, drop received 0x7f data, set recv_state as WAIT_7F again
 *
 * in normal, there are 3 parts in one infrared signal:
 * 1. not silent pulse      (ST_WAIT_7F)
 * 2. 0x7f silent pulse     (ST_WAIT_HEAD)
 * 3. silent pulse shorter than 0x7f   (ST_WAIT_HEAD)
 *
 * so this function copy signal part 1 and a 0x7f (for decode) to submit_() functions.
 * when receive 0x7f, this function copy data from ir_recv to irctl, and wait for next high pulse.
 */
static void w83667hg_recv(struct ir_recv_t *ir_recv, struct irctl *ir);


/* as VISTA MCE definition, valid carrier value */
#define MAX_CARRIER 60000
#define MIN_CARRIER 30000

/* receive function for STUDY
 *
 * 0. not receive 0x80, copy received data to ir_recv buf
 * when get 0x80, it means user released controller, and only need process received data
 * 1. get carrier
 * 2. get pulse
 */
static void w83667hg_study_recv(struct ir_recv_t *ir_recv, struct irctl *ir);

static irqreturn_t w83667hg_wake_interrupt_handler(int irq, void *dev);

static irqreturn_t w83667hg_interrupt_handler(int irq, void *dev);


/* platform device, NON-PNP */

static struct platform_device *lirc_wb667_dev;

static int __devinit lirc_wb667_probe(struct platform_device *dev);

static int __devexit lirc_wb667_remove(struct platform_device *dev);

/* before suspend 
 * 1. set all states as NONE
 * 2. close CIR and WAKE interrupt
 * 3. close CIR and WAKE cr
 * 4. close input
 */
static int lirc_wb667_suspend(struct platform_device *dev, pm_message_t state);

/* resume all register and input device
 */
static int lirc_wb667_resume(struct platform_device *dev);


/* pnp device */
#ifdef CONFIG_PNP


/* CIR and CIR WAKE */
static const struct pnp_device_id pnp_dev_table[] = {
	{ "WEC0530", 0 },   /* CIR */
	{ "NTN0530", 0 },   /* CIR for new chip's pnp id*/		
	{ "", 0 },
};


MODULE_DEVICE_TABLE(pnp, pnp_dev_table);

static int __devinit lirc_wb667_pnp_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id);

static void __devexit lirc_wb667_pnp_remove(struct pnp_dev *dev);

/* before suspend 
 * 1. set all states as NONE
 * 2. close CIR and WAKE interrupt
 * 3. close CIR and WAKE cr
 * 4. close input
 */
static int lirc_wb667_pnp_suspend(struct pnp_dev *dev, pm_message_t state);

/* resume all register and input device
 */
static int lirc_wb667_pnp_resume(struct pnp_dev *dev);


	#if 0

/* CIR */

static const struct pnp_device_id pnp_dev_cir_table[] = {
	{ "WEC0530", 0 },   /* CIR */
	{ "", 0 },
};

MODULE_DEVICE_TABLE(pnp, pnp_dev_cir_table);

static int __devinit lirc_wb667_pnp_cir_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id);

static void __devexit lirc_wb667_pnp_cir_remove(struct pnp_dev *dev);

/* before suspend 
 * 1. set all states as NONE
 * 2. close CIR and WAKE interrupt
 * 3. close CIR and WAKE cr
 * 4. close input
 */
static int lirc_wb667_pnp_cir_suspend(struct pnp_dev *dev, pm_message_t state);

/* resume all register and input device
 */
static int lirc_wb667_pnp_cir_resume(struct pnp_dev *dev);


/* CIR WAKE */

static const struct pnp_device_id pnp_dev_cir_wake_table[] = {
	{ "WEC0531", 0 },   /* CIR WAKE */
	{ "", 0 },
};

MODULE_DEVICE_TABLE(pnp, pnp_dev_cir_wake_table);

static int __devinit lirc_wb667_pnp_cir_wake_probe(struct pnp_dev *dev, const struct pnp_device_id *dev_id);

static void __devexit lirc_wb667_pnp_cir_wake_remove(struct pnp_dev *dev);

	#endif

static unsigned int nopnp = 0;

#else

static const unsigned int nopnp = 1;

#endif


/* create io region for cir and cir wake
 * create irq handler
 * create dev and data struct
 */
static int w83667hg_cir_probe(void);

/* uninit cir, cir wake, dev and data struct, release irq and io port */
static void w83667hg_cir_remove(void);


static int lirc_wb667_init(void);

void lirc_wb667_uninit(void);


/* 
 * 1. init cr
 * 2. init input
 * 3. init lirc buffer, register, irctl
 * 4. init 667 cir dev, req_region, req_irq
 */
//int init_module(void);
//void cleanup_module(void);
int init_module_wb667(void);
void cleanup_module_wb667(void);

MODULE_DESCRIPTION("LIRC driver for Winbond W83677HG CIR");
MODULE_AUTHOR("");
MODULE_LICENSE("GPL");

/* device file name, locate in /dev/ directory */
//static char *DRVNAME = "w83667hg_ir";
// FIXME, stable name
static char *DRVNAME = "lirc_wb677";
module_param(DRVNAME, charp, S_IRUGO);
MODULE_PARM_DESC(DRVNAME, "Device file name in /dev/ and /sys/class/.");

/* platform driver name to register */
#define PLATNAME "lirc_wb667_cir"
#define PLATNAME_W "lirc_wb667_wake"

/* device file major number */
#define CIR_MAJOR 229

/* cir device base address */
//static u16 CIR_BASE = 0x380;
static u16 CIR_BASE = 0x240;
module_param(CIR_BASE, ushort, S_IRUGO);
MODULE_PARM_DESC(CIR_BASE, "Base IO port address of 667 CIR device.");

/* cir base i/o address */
static unsigned int cir_address;

/* cir irq */
static ushort CIR_IRQ_NUM = 5;
module_param(CIR_IRQ_NUM, ushort, S_IRUGO);
MODULE_PARM_DESC(CIR_IRQ_NUM, "IRQ number for 667 CIR device.");

/* handle cir wake up request in driver or not */
#define ENABLE_CIR_WAKE_IRQ

/* cir wake up device base address */
static u16 CIR_WAKE_BASE = 0x250;
//static u16 CIR_WAKE_BASE = 0x380;
module_param(CIR_WAKE_BASE, ushort, S_IRUGO);
MODULE_PARM_DESC(CIR_WAKE_BASE, "Base IO port address of 667 CIR WAKE device.");

/* cir wake up base i/o address */
static unsigned int cir_wake_address;

/* cir wake up irq */
//static ushort CIR_WAKE_IRQ_NUM = 10;
static ushort CIR_WAKE_IRQ_NUM = 5;
module_param(CIR_WAKE_IRQ_NUM, ushort, S_IRUGO);
MODULE_PARM_DESC(CIR_WAKE_IRQ_NUM, "IRQ number for 667 CIR WAKE device.");

/* nopnp option */
#ifdef CONFIG_PNP
module_param(nopnp, uint, S_IRUGO);
MODULE_PARM_DESC(nopnp, "Scan for device instead of relying on PNP. (default 0)");
#endif

//EXPORT_NO_SYMBOLS;



