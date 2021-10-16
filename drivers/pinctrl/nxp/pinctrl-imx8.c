// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2018 NXP
 */

#include <common.h>
#include <asm/global_data.h>
#include <dm/device.h>
#include <dm/pinctrl.h>

#include "pinctrl-imx.h"

DECLARE_GLOBAL_DATA_PTR;

static struct imx_pinctrl_soc_info imx8_pinctrl_soc_info = {
	.flags = IMX8_USE_SCU,
};

static int imx8_pinctrl_probe(struct udevice *dev)
{
	struct imx_pinctrl_soc_info *info =
		(struct imx_pinctrl_soc_info *)dev_get_driver_data(dev);
	/*
	 * imx_pinctrl_soc_info is stored in .rodata as on XIP
	 * boot we cannot put it to .data. So malloc it and copy
	 * to malloced place the buildtime setting.
	 */
	memcpy(&priv->info, info, sizeof(struct imx_pinctrl_soc_info));
	return imx_pinctrl_probe(dev);
}

static const struct udevice_id imx8_pinctrl_match[] = {
	{ .compatible = "fsl,imx8qxp-iomuxc", .data = (ulong)&imx8_pinctrl_soc_info },
	{ .compatible = "fsl,imx8qm-iomuxc", .data = (ulong)&imx8_pinctrl_soc_info },
	{ /* sentinel */ }
};

U_BOOT_DRIVER(imx8_pinctrl) = {
	.name = "imx8_pinctrl",
	.id = UCLASS_PINCTRL,
	.of_match = of_match_ptr(imx8_pinctrl_match),
	.probe = imx8_pinctrl_probe,
	.remove = imx_pinctrl_remove,
	.priv_auto	= sizeof(struct imx_pinctrl_priv),
	.ops = &imx_pinctrl_ops,
	.flags = DM_FLAG_PRE_RELOC,
};
