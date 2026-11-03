/* SPDX-License-Identifier: GPL-2.0-only */

#include <linux/clk-provider.h>
#include <linux/platform_device.h>

#include "clk-gate.h"
#include "clk-mtk.h"

#include <dt-bindings/clock/mediatek,mt8127-pericfg.h>

#define PERI_RST0	0x00
#define PERI_RST1	0x04
#define PERI_PDN0_SET	0x08
#define PERI_PDN0_CLR	0x10
#define PERI_PDN0_STA	0x18
#define PERI_PDN1_SET	0x0C
#define PERI_PDN1_CLR	0x14
#define PERI_PDN1_STA	0x1C


static const struct mtk_gate_regs peri0_cg_regs = {
	.set_ofs = PERI_PDN0_SET,
	.clr_ofs = PERI_PDN0_CLR,
	.sta_ofs = PERI_PDN0_STA
};

static const struct mtk_gate_regs peri1_cg_regs = {
	.set_ofs = PERI_PDN1_SET,
	.clr_ofs = PERI_PDN1_CLR,
	.sta_ofs = PERI_PDN1_STA
};

#define GATE_PERI0(_id, _name, _parent, _shift)	\
	GATE_MTK(_id, _name, _parent, &peri0_cg_regs, _shift, &mtk_clk_gate_ops_setclr)

#define GATE_PERI0_AXI_SEL(_id, _name, _shift)	\
	GATE_MTK(_id, _name, "axi_sel", &peri0_cg_regs, _shift, &mtk_clk_gate_ops_setclr)

#define GATE_PERI1(_id, _name, _parent, _shift)	\
	GATE_MTK(_id, _name, _parent, &peri1_cg_regs, _shift, &mtk_clk_gate_ops_setclr)

static const struct mtk_gate pericfg_clks[] = {
	GATE_PERI0(CLK_PERI_NFI, "nfi", "nfi2x_sel", 0),
	GATE_PERI0_AXI_SEL(CLK_PERI_THERM, "therm", 1),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM1, "pwm1", 2),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM2, "pwm2", 3),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM3, "pwm3", 4),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM4, "pwm4", 5),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM5, "pwm5", 6),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM6, "pwm6", 7),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM7, "pwm7", 8),
	GATE_PERI0_AXI_SEL(CLK_PERI_PWM, "pwm", 9),
	GATE_PERI0(CLK_PERI_USB0, "usb0", "usb20_sel", 10),
	GATE_PERI0(CLK_PERI_USB1, "usb1", "usb20_sel", 11),
	GATE_PERI0_AXI_SEL(CLK_PERI_AP_DMA, "ap_dma", 12),
	GATE_PERI0(CLK_PERI_MSDC30_0, "msdc30_0", "msdc30_0", 13),
	GATE_PERI0(CLK_PERI_MSDC30_1, "msdc30_1", "msdc30_1", 14),
	GATE_PERI0(CLK_PERI_MSDC30_2, "msdc30_2", "msdc30_2", 15),
	GATE_PERI0_AXI_SEL(CLK_PERI_NLI, "nli", 16),
	GATE_PERI0(CLK_PERI_UART0, "uart0", "uart_sel", 17),
	GATE_PERI0(CLK_PERI_UART1, "uart1", "uart_sel", 18),
	GATE_PERI0(CLK_PERI_UART2, "uart2", "uart_sel", 19),
	GATE_PERI0(CLK_PERI_UART3, "uart3", "uart_sel", 20),
	GATE_PERI0_AXI_SEL(CLK_PERI_BTIF, "btif", 21),
	GATE_PERI0_AXI_SEL(CLK_PERI_I2C0, "i2c0", 22),
	GATE_PERI0_AXI_SEL(CLK_PERI_I2C1, "i2c1", 23),
	GATE_PERI0_AXI_SEL(CLK_PERI_I2C2, "i2c2", 24),
	GATE_PERI0_AXI_SEL(CLK_PERI_I2C3, "i2c3", 25),
	GATE_PERI0_AXI_SEL(CLK_PERI_AUXADC, "auxadc", 26),
	GATE_PERI0(CLK_PERI_SPI0, "spi0", "spi_sel", 27),
	GATE_PERI0(CLK_PERI_ETH, "eth", "eth_50m_sel", 28),
	GATE_PERI0_AXI_SEL(CLK_PERI_USB0_MCU, "usb0_mcu", 29),
	GATE_PERI0_AXI_SEL(CLK_PERI_USB1_MCU, "usb1_mcu", 30),
	GATE_PERI0_AXI_SEL(CLK_PERI_USB_SLV, "usb_slv", 31),
	GATE_PERI1(CLK_PERI_GCPU, "gcpu", "axi_sel", 0),
	GATE_PERI1(CLK_PERI_NFI_ECC, "nfi_ecc", "nfi2x_sel", 1),
	GATE_PERI1(CLK_PERI_NFIPAD, "nfipad", "nfi2x_sel", 2)
};

static u16 pericfg_rst_bank_ofs[] = { INFRA_RST0 };

static const struct mtk_clk_rst_desc pericfg_rst_desc = {
	.VERSION = MTK_RST_SIMPLE,
	.rst_bank_ofs = pericfg_rst_bank_ofs,
	.rst_bank_nr = ARRAY_SIZE(pericfg_rst_bank_ofs)
};

static const struct mtk_clk_desc pericfg_desc = {
	.clks = pericfg_clks,
	.num_clks = ARRAY_SIZE(pericfg_clks),
	.rst_desc = &pericfg_rst_desc
};

static const struct of_device_id of_match_clk_pericfg_mt8127[] = {
	{ .compatible = "mediatek,mt8127-pericfg", .data = &pericfg_desc },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, of_match_clk_pericfg_mt8127);

static struct platform_driver clk_mt8127_pericfg_drv = {
	.driver = {
		.name = "clk-mt8127-pericfg",
		of_match_table = of_match_clk_pericfg_mt8127,
	},
	.probe = mtk_clk_simple_probe,
	.remove = mtk_clk_simple_remove
};

module_platform_driver(clk_mt8127_pericfg_drv);

MODULE_AUTHOR("Zakariya Hadrami <zkh@mailbox.org>");
MODULE_DESCRIPTION("Mediatek MT8127 pericfg clock driver");
MODULE_LICENSE("GPL");
