/*
 *  derived from "zhenhua.c"
 *  derived from "twidjoy.c"
 *
 *  Copyright (c) 2012-2024 Michael A. Kohn
 *  Copyright (c) 2008 Martin Kebert
 *  Copyright (c) 2001 Arndt Schoenewald
 *  Copyright (c) 2000-2001 Vojtech Pavlik
 *  Copyright (c) 2000 Mark Fletcher
 *
 */

/*
 * Driver for the Joytar project. Guitar connected to an ESP32 uses
 * DCT to convert sound played by the guitar into joystick commands.
 * A sync byte of 0xff and then the 3 bytes is sent over UART to
 * a PC. The packet format is:
 *
 * 0: sync (0xff)
 * 1: yaw
 * 2: pitch
 * 3: throttle  (bit 7 of the throttle has the state of the A/B switch)
 *
 * More info: https://www.mikekohn.net/
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/serio.h>
#include <linux/init.h>

#define DRIVER_DESC "Joytar guitar/DCT joystick driver"

MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

/*
 * Constants.
 */

#define SYMA_S107_PACKET_LEN 4

struct joytar
{
  struct input_dev *dev;
  int idx;
  unsigned char data[SYMA_S107_PACKET_LEN];
  char phys[32];
};

/*
 * joytar_interrupt() is called by the low level driver when characters
 * are ready for us. We then buffer them for further processing, or call the
 * packet processing routine.
 */

static irqreturn_t joytar_interrupt(struct serio *serio, unsigned char data, unsigned int flags)
{
  struct joytar *joytar = serio_get_drvdata(serio);

  data = data & 0x7f;

printk("interrupt data %d\n", data);

  if (data == 0xff)
  {
    /* this byte starts a new packet */
    joytar->idx = 0;
  }
    else
  {
    struct input_dev *dev = joytar->dev;
    const int idx = joytar->idx;

    switch(idx)
    {
      case 0:
        if (data != joytar->data[idx])
        {
          joytar->data[idx] = data;
          input_report_abs(dev, ABS_X, data);
        }
        break;
      case 1:
        if (data != joytar->data[idx])
        {
          joytar->data[idx] = data;
          input_report_abs(dev, ABS_Y, data);
        }
        break;
      case 2:
        if (data != joytar->data[idx])
        {
          joytar->data[idx] = data;
          input_report_abs(dev, ABS_Z, data);
        }
        break;
      default:
        break;
    }

    joytar->idx++;
  }

  return IRQ_HANDLED;
}

/*
 * joytar_disconnect() is the opposite of joytar_connect()
 */

static void joytar_disconnect(struct serio *serio)
{
  struct joytar *joytar = serio_get_drvdata(serio);

  printk("Disconnect joytar\n");

  serio_close(serio);
  serio_set_drvdata(serio, NULL);
  input_unregister_device(joytar->dev);
  kfree(joytar);
}

/*
 * joytar_connect() is the routine that is called when someone adds a
 * new serio device. It looks for the joytar, and if found, registers
 * it as an input device.
 */

static int joytar_connect(struct serio *serio, struct serio_driver *drv)
{
  struct joytar *joytar;
  struct input_dev *input_dev;
  int err = -ENOMEM;

  printk("Connecting joytar\n");

  joytar = kzalloc(sizeof(struct joytar), GFP_KERNEL);
  input_dev = input_allocate_device();
  if (!joytar || !input_dev) goto fail1;

  joytar->idx = 4;
  joytar->dev = input_dev;
  snprintf(joytar->phys, sizeof(joytar->phys), "%s/input0", serio->phys);

  input_dev->name = "guitar joystick";
  input_dev->phys = joytar->phys;
  input_dev->id.bustype = BUS_RS232;
  input_dev->id.vendor = SERIO_ZHENHUA;
  input_dev->id.product = 0x0001;
  input_dev->id.version = 0x0100;
  input_dev->dev.parent = &serio->dev;

  input_dev->evbit[0] = BIT(EV_ABS);
  input_set_abs_params(input_dev, ABS_X, 0, 127, 0, 0);
  input_set_abs_params(input_dev, ABS_Y, 0, 127, 0, 0);
  input_set_abs_params(input_dev, ABS_Z, 0, 127, 0, 0);
  //input_set_abs_params(input_dev, ABS_RZ, 0, 127, 0, 0);

  serio_set_drvdata(serio, joytar);

  printk("joytar: serio_open()\n");

  err = serio_open(serio, drv);
  if (err) goto fail2;

  printk("joytar: input_register()\n");
  err = input_register_device(joytar->dev);
  if (err) goto fail3;

  printk("Success: joytar connection\n");

  return 0;

  fail3:serio_close(serio);
  fail2:serio_set_drvdata(serio, NULL);
  fail1:input_free_device(input_dev);
  kfree(joytar);

  printk("Failed: joytar connection\n");

  return err;
}

/*
 * The serio driver structure.
 */

static struct serio_device_id joytar_serio_ids[] =
{
  {
    .type = SERIO_RS232,
    .proto = SERIO_ZHENHUA,
    .id = SERIO_ANY,
    .extra = SERIO_ANY,
  },
  { 0 }
};

MODULE_DEVICE_TABLE(serio, joytar_serio_ids);

static struct serio_driver joytar_drv =
{
  .driver =
  {
    .name = "joytar",
  },
  .description = DRIVER_DESC,
  .id_table = joytar_serio_ids,
  .interrupt = joytar_interrupt,
  .connect = joytar_connect,
  .disconnect = joytar_disconnect,
};

/*
 * The functions for inserting/removing us as a module.
 */

static int __init joytar_init(void)
{
  printk("Init joytar\n");
  return serio_register_driver(&joytar_drv);
}

static void __exit joytar_exit(void)
{
  printk("Exit joytar\n");
  serio_unregister_driver(&joytar_drv);
}

module_init(joytar_init);
module_exit(joytar_exit);

