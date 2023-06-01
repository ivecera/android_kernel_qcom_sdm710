// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2017-2018, The Linux Foundation. All rights reserved.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/soc/qcom/llcc-qcom.h>

/*
 * SCT(System Cache Table) entry contains of the following members:
 * usecase_id: Unique id for the client's use case
 * slice_id: llcc slice id for each client
 * max_cap: The maximum capacity of the cache slice provided in KB
 * priority: Priority of the client used to select victim line for replacement
 * fixed_size: Boolean indicating if the slice has a fixed capacity
 * bonus_ways: Bonus ways are additional ways to be used for any slice,
 *		if client ends up using more than reserved cache ways. Bonus
 *		ways are allocated only if they are not reserved for some
 *		other client.
 * res_ways: Reserved ways for the cache slice, the reserved ways cannot
 *		be used by any other client than the one its assigned to.
 * cache_mode: Each slice operates as a cache, this controls the mode of the
 *             slice: normal or TCM(Tightly Coupled Memory)
 * probe_target_ways: Determines what ways to probe for access hit. When
 *                    configured to 1 only bonus and reserved ways are probed.
 *                    When configured to 0 all ways in llcc are probed.
 * dis_cap_alloc: Disable capacity based allocation for a client
 * retain_on_pc: If this bit is set and client has maintained active vote
 *               then the ways assigned to this client are not flushed on power
 *               collapse.
 * activate_on_init: Activate the slice immediately after the SCT is programmed
 */
#define SCT_ENTRY(uid, sid, mc, p, fs, bway, rway, cmod, ptw, dca, rp, a) \
	{					\
		.usecase_id = uid,		\
		.slice_id = sid,		\
		.max_cap = mc,			\
		.priority = p,			\
		.fixed_size = fs,		\
		.bonus_ways = bway,		\
		.res_ways = rway,		\
		.cache_mode = cmod,		\
		.probe_target_ways = ptw,	\
		.dis_cap_alloc = dca,		\
		.retain_on_pc = rp,		\
		.activate_on_init = a,		\
	}

static struct llcc_slice_config sdm670_data[] =  {
	SCT_ENTRY(LLCC_CPUSS,   1,  512, 1, 0, 0xf, 0x0, 0, 0, 1, 1, 1),
	SCT_ENTRY(LLCC_ROTATOR, 4,  384, 2, 1, 0x0, 0xe, 2, 0, 1, 1, 0),
	SCT_ENTRY(LLCC_VOICE,   5,  512, 1, 0, 0xf, 0x0, 0, 0, 1, 1, 0),
	SCT_ENTRY(LLCC_AUDIO,   6,  512, 1, 0, 0xf, 0x0, 0, 0, 1, 1, 0),
	SCT_ENTRY(LLCC_MDM,     8,  512, 1, 0, 0xf, 0x0, 0, 0, 1, 1, 0),
	SCT_ENTRY(LLCC_GPU,     12, 384, 1, 1, 0x0, 0x0, 0, 0, 1, 1, 0),
	SCT_ENTRY(LLCC_MMUHWT,  13, 512, 1, 0, 0xf, 0x0, 0, 0, 1, 0, 1),
	SCT_ENTRY(LLCC_AUDHW,   22, 512, 1, 1, 0xf, 0x0, 0, 0, 1, 1, 0),
};

static int sdm670_qcom_llcc_probe(struct platform_device *pdev)
{
	return qcom_llcc_probe(pdev, sdm670_data, ARRAY_SIZE(sdm670_data));
}

static const struct of_device_id sdm670_qcom_llcc_of_match[] = {
	{ .compatible = "qcom,sdm670-llcc", },
	{ }
};

static struct platform_driver sdm670_qcom_llcc_driver = {
	.driver = {
		.name = "sdm670-llcc",
		.of_match_table = sdm670_qcom_llcc_of_match,
	},
	.probe = sdm670_qcom_llcc_probe,
};
module_platform_driver(sdm670_qcom_llcc_driver);

MODULE_DESCRIPTION("QCOM sdm670 LLCC driver");
MODULE_LICENSE("GPL v2");
