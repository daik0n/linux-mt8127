/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>

#include "clk-mtk.h"
#include "clk-mux.h"

#include <dt-bindings/clock/mediatek,mt8127-topckgen.h>

#define CLK_CFG_0           0x040
#define CLK_CFG_1           0x050
#define CLK_CFG_2           0x060
#define CLK_CFG_3           0x070
#define CLK_CFG_4           0x080
#define CLK_CFG_4_SET       0x084
#define CLK_CFG_4_CLR       0x088
#define CLK_CFG_5           0x090
#define CLK_CFG_6           0x0A0
#define CLK_CFG_8           0x100
#define CLK_CFG_9           0x104
#define CLK_CFG_10          0x108
#define CLK_CFG_11          0x10C
#define CLK_SCP_CFG_0       0x200
#define CLK_SCP_CFG_1       0x204

static DEFINE_SPINLOCK(mt8127_topckgen_lock);

static const struct mtk_fixed_factor topckgen_factors[] = {
	FACTOR(CLK_TOP_AUDPLL_D4, "audpll_d4", "audpll", 1, 4),
	FACTOR(CLK_TOP_AUDPLL_D8, "audpll_d8", "audpll", 1, 8),
	FACTOR(CLK_TOP_AUDPLL_D16, "audpll_d16", "audpll", 1, 16),
	FACTOR(CLK_TOP_AUDPLL_D24, "audpll_d24", "audpll", 1, 24),

	FACTOR(CLK_TOP_DMPLL_D2, "dmpll_d2", "dmpll_ck", 1, 2),
	FACTOR(CLK_TOP_DMPLL_D4, "dmpll_d4", "dmpll_ck", 1, 4),
	FACTOR(CLK_TOP_DMPLL_X2_CK, "dmpll_x2_ck", "dmpll_ck", 2, 1),

	FACTOR(CLK_TOP_HDMIPLL_D2, "hdmipll_d2", "hdmipll", 1, 2),
	FACTOR(CLK_TOP_HDMIPLL_D3, "hdmipll_d3", "hdmipll", 1, 3),

	FACTOR(CLK_TOP_LVDSPLL_D2, "lvdspll_d2", "lvdspll", 1, 2),
	FACTOR(CLK_TOP_LVDSPLL_D4, "lvdspll_d4", "lvdspll", 1, 4),
	FACTOR(CLK_TOP_LVDSPLL_D8, "lvdspll_d8", "lvdspll", 1, 8),
//	FACTOR(CLK_TOP_LVDSPLL_ETH, "lvdspll_eth", "lvdspll", 1, ?),

//	FACTOR(CLK_TOP_MIPIPLL_D2, "mipipll_d2", "mipipll", 1, 2),
//	FACTOR(CLK_TOP_MIPIPLL_D4, "mipipll_d4", "mipipll", 1, 4),

//	FACTOR(CLK_TOP_MMPLL_D2, "mmpll_d2", "mmpll_ck", 1, 2),

//	FACTOR(CLK_TOP_MSDCPLL_D2, "msdcpll_d2", "msdcpll_ck", 1, 2),

	FACTOR(CLK_TOP_SYSPLL_D3, "syspll_d3", "mainpll", 1, 3),
	FACTOR(CLK_TOP_SYSPLL_D5, "syspll_d5", "mainpll", 1, 5),
	FACTOR(CLK_TOP_SYSPLL_D7, "syspll_d7", "mainpll", 1, 7),
	FACTOR(CLK_TOP_SYSPLL1_D2, "syspll1_d2", "mainpll", 1, 2*2),
	FACTOR(CLK_TOP_SYSPLL1_D4, "syspll1_d4", "mainpll", 1, 2*4),
	FACTOR(CLK_TOP_SYSPLL1_D8, "syspll1_d8", "mainpll", 1, 2*8),
	FACTOR(CLK_TOP_SYSPLL1_D16, "syspll1_d16", "mainpll", 1, 2*16),
	FACTOR(CLK_TOP_SYSPLL2_D2, "syspll2_d2", "mainpll", 1, 3*2),
	FACTOR(CLK_TOP_SYSPLL2_D4, "syspll4_d4", "mainpll", 1, 3*4),
	FACTOR(CLK_TOP_SYSPLL2_D8, "syspll8_d8", "mainpll", 1, 3*8),
	FACTOR(CLK_TOP_SYSPLL3_D2, "syspll2_d2", "mainpll", 1, 5*2),
	FACTOR(CLK_TOP_SYSPLL3_D4, "syspll4_d4", "mainpll", 1, 5*4),
	FACTOR(CLK_TOP_SYSPLL4_D2, "syspll4_d2", "mainpll", 1, 7*2),
	FACTOR(CLK_TOP_SYSPLL4_D4, "syspll4_d4", "mainpll", 1, 7*4),

//	FACTOR(CLK_TOP_TVDPLL_D2, "tvdpll_d2", "tvdpll", 1, 2),
//	FACTOR(CLK_TOP_TVDPLL_D4, "tvdpll_d4", "tvdpll", 1, 4),

	FACTOR(CLK_TOP_UNIVPLL_D3, "univpll_d3", "univpll", 1, 3),
	FACTOR(CLK_TOP_UNIVPLL_D5, "univpll_d5", "univpll", 1, 5),
	FACTOR(CLK_TOP_UNIVPLL_D26, "univpll_d26", "univpll", 1, 26),
	FACTOR(CLK_TOP_UNIVPLL1_D2, "univpll1_d2", "univpll", 1, 2*2),
	FACTOR(CLK_TOP_UNIVPLL1_D2, "univpll1_d4", "univpll", 1, 2*4),
	FACTOR(CLK_TOP_UNIVPLL1_D2, "univpll1_d8", "univpll", 1, 2*8),
	FACTOR(CLK_TOP_UNIVPLL2_D2, "univpll2_d2", "univpll", 1, 3*2),
	FACTOR(CLK_TOP_UNIVPLL2_D2, "univpll2_d4", "univpll", 1, 3*4),
	FACTOR(CLK_TOP_UNIVPLL2_D2, "univpll2_d8", "univpll", 1, 3*8),
	FACTOR(CLK_TOP_UNIVPLL3_D2, "univpll3_d2", "univpll", 1, 5*2),
	FACTOR(CLK_TOP_UNIVPLL3_D2, "univpll3_d4", "univpll", 1, 5*4),
	FACTOR(CLK_TOP_UNIVPLL3_D2, "univpll3_d8", "univpll", 1, 5*8)
};

static const char * const mm_parents[] = {
	"clk26m",
	"vencpll_ck",
	"syspll1_d2",
	"syspll1_d4",
	"univpll_d5",
	"univpll1_d2",
	"univpll2_d2",
	"dmpll_ck"
};

static const char * const ddrphycfg_parents[] = {
	"clk26m",
	"syspll1_d8"
};

static const char * const mem_parents[] = {
	"clk26m",
	"dmpll_ck"
};

static const char * const axi_parents[] = {	
	"clk26m",
	"syspll1_d2",
	"syspll_d5",
	"syspll1_d4",
	"univpll_d5",
	"univpll2_d2",
	"dmpll_ck",
	"dmpll_d2"
};

static const char * const camtg_parents[] = {
	"clk26m",
	"univpll_d26",
	"univpll2_d2",
	"syspll3_d2",
	"syspll3_d4",
	"msdcpll_d2",
	"mmpll_d2"
};

static const char * const mfg_parents[] = {
	"clk26m",
	"mmpll_ck",
	"dmpll_x2_ck",
	"msdcpll_ck",
	"clk26m",
	"syspll_d3",
	"univpll_d3",
	"univpll1_d2"
};

static const char * const vdec_parents[] = {
	"clk26m",
	"syspll1_d2",
	"syspll_d5",
	"syspll1_d4",
	"univpll_d5",
	"univpll2_d2",
	"univpll2_d4",
	"msdcpll_d2",
	"mmpll_d2"
};

static const char * const pwm_parents[] = {
	"clk26m",
	"univpll2_d4",
	"univpll3_d2",
	"univpll1_d4"
};

static const char * const msdc30_0_parents[] = {
	"clk26m",
	"msdcpll_d2",
	"syspll2_d2",
	"syspll1_d4",
	"univpll1_d4",
	"univpll2_d4"
};

static const char * const usb20_parents[] = {
	"clk26m",
	"univpll1_d8",
	"univpll3_d4"
};

static const char * const spi_parents[] = {
	"clk26m",
	"syspll3_d2",
	"syspll4_d2",
	"univpll2_d4",
	"univpll1_d8"
};

static const char * const uart_parents[] = {
	"clk26m",
	"univpll2_d8"
};

static const char * const audintbus_parents[] = {
	"clk26m",
	"syspll1_d4",
	"syspll3_d2",
	"syspll4_d2",
	"univpll3_d2",
	"univpll2_d4"
};

static const char * const audio_parents[] = {
	"f_f26m_ck",
	"syspll1_d16"
};

static const char * const msdc30_2_parents[] = {
	"clk26m",
	"msdcpll_d2",
	"syspll2_d2",
	"syspll1_d4",
	"univpll1_d4",
	"univpll2_d4"
};

static const char * const msdc30_1_parents[] = {
	"clk26m",
	"msdcpll_d2",
	"syspll2_d2",
	"syspll1_d4",
	"univpll1_d4",
	"univpll2_d4"
};

static const char * const dpi1_parents[] = {
	"clk26m",
	"tvdpll",
	"tvdpll_d2",
	"tvdpll_d4"
};

static const char * const dpi0_parents[] = {
	"clk26m",
	"mipipll",
	"mipipll_d2",
	"mipipll_d4",
	"lvdspll",
	"lvdspll_d2",
	"lvdspll_d4",
	"lvdspll_d8"
};

static const char * const scp_parents[] = {
	"clk26m",
	"syspll1_d8",
	"dmpll_d2",
	"dmpll_d4"
};
 
static const char * const pmicspi_parents[] = {
	"clk26m",
	"syspll1_d8",
	"syspll2_d4",
	"syspll4_d2",
	"syspll3_d4",
	"syspll2_d8",
	"syspll1_d16",
	"univpll3_d4",
	"univpll_d26",
	"dmpll_d2",
	"dmpll_d4"
};

static const char * const dpilvds_parents[] = {
	"clk26m",
	"lvdspll",
	"lvdspll_d2",
	"lvdspll_d4",
	"lvdspll_d8",
	"fpc_ck",
	"clk26m",
	"clk26m"
};

static const char * const apll_parents[] = {
	"clk26m",
	"audpll",
	"audpll_d4",
	"audpll_d8",
	"audpll_d16",
	"audpll_d24",
	"clk26m",
	"clk26m"
};

static const char * const hdmi_parents[] = {
	"clk26m",
	"hdmipll",
	"hdmipll_d2",
	"hdmipll_d3"
};

static const char * const tve_parents[] = {
	"clk26m",
	"mipipll",
	"mipipll_d2",
	"mipipll_d4",
	"clk26m",
	"tvdpll",
	"tvdpll_d2",
	"tvdpll_d4"
};

static const char * const eth_50m_parents[] = {
	"clk26m",
	"syspll3_d4",
	"univpll2_d8",
	"lvdspll_eth",
	"univpll_d26",
	"syspll2_d8",
	"syspll4_d4",
	"univpll3_d8"
};

static const char * const nfi2x_parents[] = {
	"clk26m",
	"syspll2_d2",
	"syspll_d7",
	"univpll3_d2",
	"syspll2_d4",
	"univpll3_d4",
	"syspll4_d4",
	"clk26m"
};

static const char * const rtc_parents[] = {
	"internal_32k",
	"external_32k",
	"clk26m",
	"univpll3_d8"
};

// TODO: Find out critical clocks (axi, mem, ddrphycfg, rtc, ...?)
static const struct mtk_composite topckgen_muxes[] = {
	MUX_GATE(CLK_TOP_MM_SEL, "mm_sel", mm_parents, CLK_CFG_0, 24, 3, 31),
	MUX_GATE(CLK_TOP_DDRPHYCFG_SEL, "ddrphycfg_sel", ddrphycfg_parents, 
		CLK_CFG_0, 16, 1, 23),	
	MUX_GATE(CLK_TOP_MEM_SEL, "mem_sel", mem_parents, CLK_CFG_0, 8, 1, 15),
	MUX_GATE(CLK_TOP_AXI_SEL, "axi_sel", axi_parents, CLK_CFG_0, 0, 3, 7),
	MUX_GATE(CLK_TOP_CAMTG_SEL, "camtg_sel", camtg_parents, CLK_CFG_1, 24, 3, 31),
	MUX_GATE(CLK_TOP_MFG_SEL, "mfg_sel", mfg_parents, CLK_CFG_1, 16, 3, 23),
	MUX_GATE(CLK_TOP_VDEC_SEL, "vdec_sel", vdec_parents, CLK_CFG_1, 8, 4, 15),
	MUX_GATE(CLK_TOP_PWM_SEL, "pwm_sel", pwm_parents, CLK_CFG_1, 0, 2, 7),
	MUX_GATE(CLK_TOP_MSDC30_0_SEL, "msdc30_0_sel", msdc30_0_parents, CLK_CFG_2, 24, 3, 31),
	MUX_GATE(CLK_TOP_USB20_SEL, "usb20_sel", usb20_parents, CLK_CFG_2, 16, 2, 23),
	MUX_GATE(CLK_TOP_SPI_SEL, "spi_sel", spi_parents, CLK_CFG_2, 8, 3, 15),
	MUX_GATE(CLK_TOP_UART_SEL, "uart_sel", uart_parents, CLK_CFG_2, 0, 1, 7),
	MUX_GATE(CLK_TOP_AUDINTBUS_SEL, "audintbus_sel", audintbus_parents, CLK_CFG_3, 24, 3, 31),
	MUX_GATE(CLK_TOP_AUDIO_SEL, "audio_sel", audio_parents, CLK_CFG_3, 16, 1, 23),
	MUX_GATE(CLK_TOP_MSDC30_2_SEL, "msdc30_2_sel", msdc30_2_parents, CLK_CFG_3, 8, 3, 15),
	MUX_GATE(CLK_TOP_MSDC30_1_SEL, "msdc30_1_sel", msdc30_1_parents, CLK_CFG_3, 0, 3, 7),
	MUX_GATE(CLK_TOP_DPI1_SEL, "dpi1_sel", dpi1_parents, CLK_CFG_4, 24, 2, 31),
	MUX_GATE(CLK_TOP_DPI0_SEL, "dpi0_sel", dpi0_parents, CLK_CFG_4, 16, 3, 23),
	MUX_GATE(CLK_TOP_SCP_SEL, "scp_sel", scp_parents, CLK_CFG_4, 8, 2, 15),
	MUX_GATE(CLK_TOP_PMICSPI_SEL, "pmicspi_sel", pmicspi_parents, CLK_CFG_4, 0, 4, 7),
	MUX_GATE(CLK_TOP_DPILVDS_SEL, "dpilvds_sel", dpilvds_parents, CLK_CFG_5, 24, 3, 31),
	MUX_GATE(CLK_TOP_APLL_SEL, "apll_sel", apll_parents, CLK_CFG_5, 16, 3, 23),
	MUX_GATE(CLK_TOP_HDMI_SEL, "hdmi_sel", hdmi_parents, CLK_CFG_5, 8, 2, 15),
	MUX_GATE(CLK_TOP_TVE_SEL, "tve_sel", tve_parents, CLK_CFG_5, 0, 3, 7),
	MUX_GATE(CLK_TOP_ETH_50M_SEL, "eth_50m_sel", eth_50m_parents, CLK_CFG_6, 16, 3, 23),
	MUX_GATE(CLK_TOP_NFI2X_SEL, "nfi2x_sel", nfi2x_parents, CLK_CFG_6, 8, 3, 15),
	MUX_GATE_FLAGS(CLK_TOP_RTC_SEL, "rtc_sel", rtc_parents, CLK_CFG_6, 0, 2, 7, CLK_IS_CRITICAL)
};

static const struct mtk_clk_desc topckgen_desc = {
	.factor_clks = topckgen_factors,
	.num_factor_clks = ARRAY_SIZE(topckgen_factors),
	.composite_clks = topckgen_muxes,
	.num_composite_clks = ARRAY_SIZE(topckgen_muxes),
	.clk_lock = &mt8127_topckgen_lock,
};

static const struct of_device_id of_match_clk_topckgen_mt8127[] = {
	{ .compatible = "mediatek,mt8127-topckgen", .data = &topckgen_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_topckgen_mt8127);

static struct platform_driver clk_mt8127_topckgen_drv = {
	.driver = {
		.name = "clk-mt8127-topckgen",
		of_match_table = of_match_clk_topckgen_mt8127,
	},
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove,
};

module_platform_driver(clk_mt8127_topckgen_drv);

MODULE_AUTHOR("Zakariya Hadrami <zkh@mailbox.org>");
MODULE_DESCRIPTION("Mediatek MT8127 topckgen clock driver");
MODULE_LICENSE("GPL");
