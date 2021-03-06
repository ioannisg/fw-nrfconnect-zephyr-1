/*
 * Copyright (c) 2018 Alexander Wachter
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <syscall_handler.h>
#include <drivers/can.h>

static inline int z_vrfy_can_configure(struct device *dev, enum can_mode mode,
				       u32_t bitrate) {

	Z_OOPS(Z_SYSCALL_DRIVER_CAN(dev, configure));

	return z_impl_can_configure((struct device *)dev, (enum can_mode)mode,
				   (u32_t)bitrate);
}
#include <syscalls/can_configure_mrsh.c>

static inline int z_vrfy_can_send(struct device *dev,
				  const struct zcan_frame *msg,
				  s32_t timeout,
				  can_tx_callback_t callback_isr,
				  void *callback_arg) {

	Z_OOPS(Z_SYSCALL_DRIVER_CAN(dev, send));

	Z_OOPS(Z_SYSCALL_MEMORY_READ((const struct zcan_frame *)msg,
				      sizeof(struct zcan_frame)));
	Z_OOPS(Z_SYSCALL_MEMORY_READ(((struct zcan_frame *)msg)->data,
				     sizeof((struct zcan_frame *)msg)->data));
	Z_OOPS(Z_SYSCALL_VERIFY_MSG(callback_isr == 0,
				    "callbacks may not be set from user mode"));

	Z_OOPS(Z_SYSCALL_MEMORY_READ((void *)callback_arg, sizeof(void *)));

	return z_impl_can_send((struct device *)dev,
			      (const struct zcan_frame *)msg,
			      (s32_t)timeout, (can_tx_callback_t) callback_isr,
			      (void *)callback_arg);
}
#include <syscalls/can_send_mrsh.c>

static inline int z_vrfy_can_attach_msgq(struct device *dev,
					 struct k_msgq *msgq,
					 const struct zcan_filter *filter) {
	Z_OOPS(Z_SYSCALL_OBJ(dev, K_OBJ_DRIVER_CAN));

	Z_OOPS(Z_SYSCALL_MEMORY_READ((struct zcan_filter *)filter,
				     sizeof(struct zcan_filter)));
	Z_OOPS(Z_SYSCALL_OBJ(msgq, K_OBJ_MSGQ));

	return z_impl_can_attach_msgq((struct device *)dev,
				     (struct k_msgq *)msgq,
				     (const struct zcan_filter *) filter);
}
#include <syscalls/can_attach_msgq_mrsh.c>

static inline void z_vrfy_can_detach(struct device *dev, int filter_id)

	Z_OOPS(Z_SYSCALL_DRIVER_CAN(dev, detach));

	z_impl_can_detach((struct device *)dev, (int)filter_id);
}
#include <syscalls/can_detach_mrsh.c>
