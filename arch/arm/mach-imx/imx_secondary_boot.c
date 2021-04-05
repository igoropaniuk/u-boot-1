// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2021 Foundries.io Ltd
 */

#include <common.h>
#include <asm/arch/sys_proto.h>
#include <asm/io.h>
#include <asm/mach-imx/sys_proto.h>
#include <command.h>
#include <elf.h>
#include <env.h>
#include <imx_sip.h>
#include <linux/compiler.h>
#include <cpu_func.h>

static int do_secondary_boot(cmd_tbl_t *cmdtp, int flag,
			     int argc, char * const argv[])
{
	u32 secondary = 0;

	/* If we just want to retrieve current value */
	if (argc == 1) {
		secondary = call_imx_sip(IMX_SIP_SRC,
					 IMX_SIP_SRC_IS_SECONDARY_BOOT, 0,
					 0, 0);

		printf("secondary flag = %d\n", secondary);
		if (secondary)
			return CMD_RET_SUCCESS;
		return CMD_RET_FAILURE;
	}

	secondary = simple_strtoul(argv[1], NULL, 10);

	if (!(secondary == 0 || secondary == 1))
		return CMD_RET_USAGE;

	call_imx_sip(IMX_SIP_SRC, IMX_SIP_SRC_SET_SECONDARY_BOOT,
		     secondary, 0, 0);

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	secondary_boot, CONFIG_SYS_MAXARGS, 1, do_secondary_boot,
	"Get/Set PERSIST_SECONDARY_BOOT bit",
	"[0|1]\n"
	"   no param  - get current bit value\n"
	"   0 - set primary image\n"
	"   1 - set secondary image\n"
);

static int do_imx_is_closed(cmd_tbl_t *cmdtp, int flag,
			    int argc, char * const argv[])
{
	int ret;

	if (boot_mode_is_closed()) {
		printf("Board is in closed state\n");

		ret = env_set("board_is_closed", "1");
		if (ret)
			return CMD_RET_FAILURE;
	} else {
		printf("Board is in open state\n");
	}

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	imx_is_closed, CONFIG_SYS_MAXARGS, 1, do_imx_is_closed,
	"Check if the board is closed", ""
);

static int do_warm_reset(cmd_tbl_t *cmdtp, int flag,
			 int argc, char * const argv[])
{
	unsigned long ret = 0;

	ret = call_imx_sip(IMX_SIP_WARM_RESET, 0, 0, 0, 0);
	if (ret)
		CMD_RET_FAILURE;

	return CMD_RET_SUCCESS;
}

U_BOOT_CMD(
	imx_warm_reset, CONFIG_SYS_MAXARGS, 1, do_warm_reset,
	"Assert WDOG Software Reset Signal (internal reset) via TF-A",
	"\n"
);
