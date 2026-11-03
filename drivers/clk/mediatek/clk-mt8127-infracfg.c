/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

#include <dt-bindings/clock/mediatek,mt8127-infracfg.h>

#define INFRA_RST0	0x30
#define INFRA_PDN_SET	0x40
#define INFRA_PDN_CLR	0x44
#define INFRA_PDN_STA	0x48

static const struct mtk_gate_regs infra_cg_regs = {
	.set_ofs = INFRA_PDN_SET,
	.clr_ofs = INFRA_PDN_CLR,
	.sta_ofs = INFRA_PDN_STA
};

#define GATE_ICG(_id, _name, _parent, _shift)	\
	GATE_MTK(_id, _name, _parent, &infra_cg_regs, _shift, &mtk_clk_gate_ops_setclr)

#define GATE_ICG_AXI_SEL(_id, _name, _shift)	\
	GATE_MTK(_id, _name, "axi_sel", &infra_cg_regs, _shift, &mtk_clk_gate_ops_setclr)

static const struct mtk_gate infracfg_clks[] = {
	GATE_ICG_AXI_SEL(CLK_INFRA_DBGCLK, "dbgclk", 0), 
	GATE_ICG_AXI_SEL(CLK_INFRA_SMI, "smi", 1),
	GATE_ICG(CLK_INFRA_AUDIO, "audio", "audintbus_sel", 5),
	GATE_ICG_AXI_SEL(CLK_INFRA_EFUSE, "efuse", 6),
	GATE_ICG_AXI_SEL(CLK_INFRA_L2C_SRAM, "l2c_sram", 7),
	GATE_ICG_AXI_SEL(CLK_INFRA_M4U, "m4u", 8),
	GATE_ICG_AXI_SEL(CLK_INFRA_CONNMCU, "connmcu", 12),
	GATE_ICG_AXI_SEL(CLK_INFRA_TRNG, "trng", 13),
	GATE_ICG_AXI_SEL(CLK_INFRA_CPUM, "cpum", 15),
	GATE_ICG_AXI_SEL(CLK_INFRA_KP, "kp", 16),
	GATE_ICG_AXI_SEL(CLK_INFRA_CEC, "cec", 18),
	GATE_ICG_AXI_SEL(CLK_INFRA_IRRX, "irrx", 19),
	GATE_ICG(CLK_INFRA_PMICSPI_SHARE, "pmicspi_share", "pmicspi_sel", 22), 
	GATE_ICG_AXI_SEL(CLK_INFRA_PMICWRAP, "pmicwrap", 23)
};

static u16 infracfg_rst_bank_ofs[] = { INFRA_RST0 };

static const struct mtk_clk_rst_desc infracfg_rst_desc = {
	.VERSION = MTK_RST_SIMPLE,
	.rst_bank_ofs = infracfg_rst_bank_ofs,
	.rst_bank_nr = ARRAY_SIZE(infracfg_rst_bank_ofs)
};

static const struct mtk_clk_desc infracfg_desc = {
	.clks = infracfg_clks,
	.num_clks = ARRAY_SIZE(infracfg_clks),
	.rst_desc = &infracfg_rst_desc
};

static const struct of_device_id of_match_clk_infracfg_mt8127[] = {
	{ .compatible = "mediatek,mt8127-infracfg", .data = &infracfg_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_infracfg_mt8127);

static struct platform_driver clk_mt8127_infracfg_drv = {
	.driver = {
		.name = "clk-mt8127-infracfg",
		of_match_table = of_match_clk_infracfg_mt8127,
	},
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove
};

module_platform_driver(clk_mt8127_infracfg_drv);

MODULE_AUTHOR("Zakariya Hadrami <zkh@mailbox.org>");
MODULE_DESCRIPTION("Mediatek MT8127 infracfg clock driver");
MODULE_LICENSE("GPL");
