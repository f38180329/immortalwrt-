/* SPDX-License-Identifier: (GPL-2.0 OR MIT) */
/*
 * Copyright (c) 2018 Synopsys, Inc. and/or its affiliates.
 * Copyright (c) 2022 SiFlower Ltd.
 */
#ifndef _SIFLOWER_ETH_H
#define _SIFLOWER_ETH_H

#include <linux/bitfield.h>
#include <linux/bitops.h>
#include <linux/netdevice.h>

struct phylink_pcs *xpcs_port_get(struct platform_device *, unsigned int);
void xpcs_port_put(struct platform_device *);

#define reg_read(p, reg)		readl((p)->ioaddr + (reg))
#define reg_write(p, reg, val)		writel(val, (p)->ioaddr + (reg))
#define reg_rmw(p, reg, clear, set)				\
	do {							\
		void __iomem *ioaddr = (p)->ioaddr + (reg);	\
		u32 val = readl(ioaddr);		\
								\
		val &= ~(clear);				\
		val |= (set);					\
		writel(val, ioaddr);			\
	} while (0)
#define reg_set(p, reg, set)		reg_rmw(p, reg, 0, set)
#define reg_clear(p, reg, clear)	reg_rmw(p, reg, clear, 0)

#define priv_to_netdev(p) \
	((struct net_device*)((void*)(p) - ALIGN(sizeof(struct net_device), NETDEV_ALIGN)))

#define offset_to_id(addr)	FIELD_GET(GENMASK(19, 14), addr)

/* Maximum L2 frame size, including FCS */
#define MAX_FRAME_SIZE		16383
#define TSO_MAX_BUFF_SIZE	MAX_FRAME_SIZE

/* Ethernet sysm defines */
#define ETHSYS_MAC(n)		((n) * 4)
#define ETHSYS_PHY_INTF_SEL	GENMASK(17, 16)
#define ETHSYS_PHY_INTF_RGMII   FIELD_PREP(ETHSYS_PHY_INTF_SEL, 1)
#define ETHSYS_MAC5_CTRL        0xdc
#define MAC5_PHY_INTF_SEL       GENMASK(17, 16)
#define MAC5_RX_DELAY_EN        BIT(24)
#define MAC5_RX_DELAY           GENMASK(23, 16)
#define MAC5_TX_DELAY_EN        BIT(8)
#define MAC5_TX_DELAY           GENMASK(7, 0)
#define MAC5_DELAY_MASK         (MAC5_TX_DELAY_EN | MAC5_TX_DELAY | MAC5_RX_DELAY | MAC5_RX_DELAY_EN)
#define MAC5_DELAY_STEP         49
#define MAC5_DELAY_DEFAULT      (0x41 * MAC5_DELAY_STEP)
#define ETHSYS_QSG_CTRL		0x6c
#define ETHSYS_SG_CTRL		0x70
#define ETHSYS_REF_RPT_EN	BIT(10)
#define ETHSYS_RST          0x88
#define ETHSYS_RST_MAC5	    BIT(9)
#define ETHSYS_RATIO_LOAD   0xec
#define ETHSYS_NPU_BYPASS	0x8c
#define ETHSYS_RX_QUEUE_ENABLE	0xb4
#define ETHSYS_MRI_Q_MODE	GENMASK(31, 30)
#define ETHSYS_MRI_OVPORT_MAX	GENMASK(21, 16)
#define ETHSYS_MRI_OVPORT_MIN	GENMASK(13, 8)
#define ETHSYS_MRI_Q_EN	0xb8
#define ETHSYS_MRI_OVPORT_TOP_PRIO	GENMASK(5, 0)
#define XGMAC_MTL_RXQ_DMA_MAP0	0x00001030
#define ETHSYS_TX_DIS		0xd4
#define ETHSYS_QS_SGMII_STATUS		0x128
#define XGMAC_PORT_DH(n)      (BIT(12) >> ((n)*4))
#define XGMAC_PORT_LINK(n)    (BIT(13)  >> ((n)*4))
#define XGMAC_PORT0_SPD_MASK    GENMASK(3, 2)

#define GMAC_HI_REG_AE		BIT(31)

/* XGMAC Registers */
#define XGMAC_TX_CONFIG			0x00000000
#define XGMAC_CONFIG_SS_OFF		29
#define XGMAC_CONFIG_SS_MASK		GENMASK(31, 29)
#define XGMAC_CONFIG_SS_10000		(0x0 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_2500_GMII	(0x2 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_1000_GMII	(0x3 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_100_MII		(0x4 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_5000		(0x5 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_2500		(0x6 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SS_10_MII		(0x7 << XGMAC_CONFIG_SS_OFF)
#define XGMAC_CONFIG_SARC		GENMASK(22, 20)
#define XGMAC_CONFIG_SARC_SHIFT		20
#define XGMAC_CONFIG_JD			BIT(16)
#define XGMAC_CONFIG_IFP		BIT(11)
enum inter_packet_gap {
	XGMAC_CONTROL_IPG_88 = 0x00000100,
	XGMAC_CONTROL_IPG_80 = 0x00000200,
	XGMAC_CONTROL_IPG_72 = 0x00000300,
	XGMAC_CONTROL_IPG_64 = 0x00000400,
};
#define XGMAC_CONFIG_TE			BIT(0)
#define XGMAC_CORE_INIT_TX		(XGMAC_CONTROL_IPG_88)
#define XGMAC_RX_CONFIG			0x00000004
#define XGMAC_CONFIG_ARPEN		BIT(31)
#define XGMAC_CONFIG_GPSL		GENMASK(29, 16)
#define XGMAC_CONFIG_GPSL_SHIFT		16
#define XGMAC_CONFIG_HDSMS		GENMASK(14, 12)
#define XGMAC_CONFIG_HDSMS_SHIFT	12
#define XGMAC_CONFIG_HDSMS_256		(0x2 << XGMAC_CONFIG_HDSMS_SHIFT)
#define XGMAC_CONFIG_S2KP		BIT(11)
#define XGMAC_CONFIG_LM			BIT(10)
#define XGMAC_CONFIG_IPC		BIT(9)
#define XGMAC_CONFIG_JE			BIT(8)
#define XGMAC_CONFIG_WD			BIT(7)
#define XGMAC_CONFIG_GPSLCE		BIT(6)
#define XGMAC_CONFIG_DCRCC		BIT(3)
#define XGMAC_CONFIG_CST		BIT(2)
#define XGMAC_CONFIG_ACS		BIT(1)
#define XGMAC_CONFIG_RE			BIT(0)
#define XGMAC_CORE_INIT_RX		(XGMAC_CONFIG_ACS | XGMAC_CONFIG_CST |\
					XGMAC_CONFIG_IPC)
#define XGMAC_PACKET_FILTER		0x00000008
#define XGMAC_FILTER_RA			BIT(31)
#define XGMAC_FILTER_IPFE		BIT(20)
#define XGMAC_FILTER_VTFE		BIT(16)
#define XGMAC_FILTER_HPF		BIT(10)
#define XGMAC_FILTER_PCF		BIT(7)
#define XGMAC_FILTER_PM			BIT(4)
#define XGMAC_FILTER_HMC		BIT(2)
#define XGMAC_FILTER_PR			BIT(0)
#define XGMAC_WD_JB_TIMEOUT		0xc
#define XGMAC_PJE			BIT(24)
#define XGMAC_JTO			GENMASK(19, 16)
#define XGMAC_PWE			BIT(8)
#define XGMAC_WTO			GENMASK(3, 0)
#define XGMAC_HASH_TABLE(x)		(0x00000010 + (x) * 4)
#define XGMAC_MAX_HASH_TABLE		8
#define XGMAC_VLAN_TAG			0x00000050
#define XGMAC_VLAN_EDVLP		BIT(26)
#define XGMAC_VLAN_VTHM			BIT(25)
#define XGMAC_VLAN_DOVLTC		BIT(20)
#define XGMAC_VLAN_ESVL			BIT(18)
#define XGMAC_VLAN_ETV			BIT(16)
#define XGMAC_VLAN_VID			GENMASK(15, 0)
#define XGMAC_VLAN_HASH_TABLE		0x00000058
#define XGMAC_VLAN_INCL			0x00000060
#define XGMAC_VLAN_VLTI			BIT(20)
#define XGMAC_VLAN_CSVL			BIT(19)
#define XGMAC_VLAN_VLC			GENMASK(17, 16)
#define XGMAC_VLAN_VLC_SHIFT		16
#define XGMAC_RXQ_CTRL0			0x000000a0
#define XGMAC_RXQEN(x)			GENMASK((x) * 2 + 1, (x) * 2)
#define XGMAC_RXQEN_SHIFT(x)		((x) * 2)
#define XGMAC_RXQ_CTRL1			0x000000a4
#define XGMAC_RQ			GENMASK(7, 4)
#define XGMAC_RQ_SHIFT			4
#define XGMAC_RXQ_CTRL2			0x000000a8
#define XGMAC_RXQ_CTRL3			0x000000ac
#define XGMAC_PSRQ(x)			GENMASK((x) * 8 + 7, (x) * 8)
#define XGMAC_PSRQ_SHIFT(x)		((x) * 8)
#define XGMAC_INT_STATUS		0x000000b0
#define XGMAC_RGMII_LS			BIT(27)
#define XGMAC_RGMII_SPD			GENMASK(26, 25)
#define XGMAC_RGMII_DM			BIT(24)
#define XGMAC_LS			GENMASK(25, 24)
#define XGMAC_MMCRXIPIS			BIT(11)
#define XGMAC_MMCTXIS			BIT(10)
#define XGMAC_MMCRXIS			BIT(9)
#define XGMAC_MMCIS			(XGMAC_MMCRXIPIS | XGMAC_MMCTXIS | XGMAC_MMCRXIS)
#define XGMAC_LPIIS			BIT(5)
#define XGMAC_PMTIS			BIT(4)
#define XGMAC_SMI			BIT(1)
#define XGMAC_LSI			BIT(0)
#define XGMAC_INT_EN			0x000000b4
#define XGMAC_TSIE			BIT(12)
#define XGMAC_LPIIE			BIT(5)
#define XGMAC_PMTIE			BIT(4)
#define XGMAC_INT_DEFAULT_EN		(XGMAC_LPIIE | XGMAC_PMTIE)
#define XGMAC_Qx_TX_FLOW_CTRL(x)	(0x00000070 + (x) * 4)
#define XGMAC_PT			GENMASK(31, 16)
#define XGMAC_PT_SHIFT			16
#define XGMAC_TFE			BIT(1)
#define XGMAC_RX_FLOW_CTRL		0x00000090
#define XGMAC_UP			BIT(1)
#define XGMAC_RFE			BIT(0)
#define XGMAC_PMT			0x000000c0
#define XGMAC_GLBLUCAST			BIT(9)
#define XGMAC_RWKPKTEN			BIT(2)
#define XGMAC_MGKPKTEN			BIT(1)
#define XGMAC_PWRDWN			BIT(0)
#define XGMAC_LPI_CTRL			0x000000d0
#define XGMAC_TXCGE			BIT(21)
#define XGMAC_LPIATE			BIT(20)
#define XGMAC_LPITXA			BIT(19)
#define XGMAC_PLS			BIT(17)
#define XGMAC_LPITXEN			BIT(16)
#define XGMAC_RLPIEX			BIT(3)
#define XGMAC_RLPIEN			BIT(2)
#define XGMAC_TLPIEX			BIT(1)
#define XGMAC_TLPIEN			BIT(0)
#define XGMAC_LPI_TIMER_CTRL		0x000000d4
#define XGMAC_LPI_LST			GENMASK(25, 16)
#define XGMAC_LPI_LST_DEFAULT		1000
#define XGMAC_LPI_TWT			GENMASK(15, 0)
#define XGMAC_LPI_TWT_DEFAULT		30
#define XGMAC_LPI_AUTO_EN		0x000000d8
#define XGMAC_LPI_AUTO_EN_MAX		0xffff8
#define XGMAC_LPI_AUTO_EN_DEFAULT	10000
#define XGMAC_LPI_1US			0x000000dc
#define XGMAC_VERSION			0x00000110
#define XGMAC_VERSION_USER		GENMASK(23, 16)
#define XGMAC_VERSION_ID_MASK		GENMASK(15, 0)
#define XGMAC_VERSION_ID		0x7631
#define XGMAC_HW_FEATURE0		0x0000011c
#define XGMAC_HWFEAT_SAVLANINS		BIT(27)
#define XGMAC_HWFEAT_RXCOESEL		BIT(16)
#define XGMAC_HWFEAT_TXCOESEL		BIT(14)
#define XGMAC_HWFEAT_EEESEL		BIT(13)
#define XGMAC_HWFEAT_TSSEL		BIT(12)
#define XGMAC_HWFEAT_AVSEL		BIT(11)
#define XGMAC_HWFEAT_RAVSEL		BIT(10)
#define XGMAC_HWFEAT_ARPOFFSEL		BIT(9)
#define XGMAC_HWFEAT_MMCSEL		BIT(8)
#define XGMAC_HWFEAT_MGKSEL		BIT(7)
#define XGMAC_HWFEAT_RWKSEL		BIT(6)
#define XGMAC_HWFEAT_VLHASH		BIT(4)
#define XGMAC_HWFEAT_GMIISEL		BIT(1)
#define XGMAC_HW_FEATURE1		0x00000120
#define XGMAC_HWFEAT_L3L4FNUM		GENMASK(30, 27)
#define XGMAC_HWFEAT_HASHTBLSZ		GENMASK(25, 24)
#define XGMAC_HWFEAT_RSSEN		BIT(20)
#define XGMAC_HWFEAT_TSOEN		BIT(18)
#define XGMAC_HWFEAT_SPHEN		BIT(17)
#define XGMAC_HWFEAT_ADDR64		GENMASK(15, 14)
#define XGMAC_HWFEAT_TXFIFOSIZE		GENMASK(10, 6)
#define XGMAC_HWFEAT_RXFIFOSIZE		GENMASK(4, 0)
#define XGMAC_HW_FEATURE2		0x00000124
#define XGMAC_HWFEAT_PPSOUTNUM		GENMASK(26, 24)
#define XGMAC_HWFEAT_TXCHCNT		GENMASK(21, 18)
#define XGMAC_HWFEAT_RXCHCNT		GENMASK(15, 12)
#define XGMAC_HWFEAT_TXQCNT		GENMASK(9, 6)
#define XGMAC_HWFEAT_RXQCNT		GENMASK(3, 0)
#define XGMAC_HW_FEATURE3		0x00000128
#define XGMAC_HWFEAT_TBSSEL		BIT(27)
#define XGMAC_HWFEAT_FPESEL		BIT(26)
#define XGMAC_HWFEAT_ESTWID		GENMASK(24, 23)
#define XGMAC_HWFEAT_ESTDEP		GENMASK(22, 20)
#define XGMAC_HWFEAT_ESTSEL		BIT(19)
#define XGMAC_HWFEAT_ASP		GENMASK(15, 14)
#define XGMAC_HWFEAT_DVLAN		BIT(13)
#define XGMAC_HWFEAT_FRPES		GENMASK(12, 11)
#define XGMAC_HWFEAT_FRPPB		GENMASK(10, 9)
#define XGMAC_HWFEAT_FRPSEL		BIT(3)
#define XGMAC_MAC_EXT_CONFIG		0x00000140
#define XGMAC_HD			BIT(24)
#define XGMAC_MAC_DPP_FSM_INT_STATUS	0x00000150
#define XGMAC_MAC_FSM_CONTROL		0x00000158
#define XGMAC_PRTYEN			BIT(1)
#define XGMAC_TMOUTEN			BIT(0)
#define XGMAC_FPE_CTRL_STS		0x00000280
#define XGMAC_EFPE			BIT(0)
#define XGMAC_ADDRx_HIGH(x)		(0x00000300 + (x) * 0x8)
#define XGMAC_ADDR_MAX			32
#define XGMAC_AE			BIT(31)
#define XGMAC_DCS			GENMASK(19, 16)
#define XGMAC_DCS_SHIFT			16
#define XGMAC_ADDRx_LOW(x)		(0x00000304 + (x) * 0x8)
#define XGMAC_L3L4_ADDR_CTRL		0x00000c00
#define XGMAC_IDDR			GENMASK(15, 8)
#define XGMAC_IDDR_SHIFT		8
#define XGMAC_IDDR_FNUM			4
#define XGMAC_TT			BIT(1)
#define XGMAC_XB			BIT(0)
#define XGMAC_L3L4_DATA			0x00000c04
#define XGMAC_L3L4_CTRL			0x0
#define XGMAC_L4DPIM0			BIT(21)
#define XGMAC_L4DPM0			BIT(20)
#define XGMAC_L4SPIM0			BIT(19)
#define XGMAC_L4SPM0			BIT(18)
#define XGMAC_L4PEN0			BIT(16)
#define XGMAC_L3HDBM0			GENMASK(15, 11)
#define XGMAC_L3HSBM0			GENMASK(10, 6)
#define XGMAC_L3DAIM0			BIT(5)
#define XGMAC_L3DAM0			BIT(4)
#define XGMAC_L3SAIM0			BIT(3)
#define XGMAC_L3SAM0			BIT(2)
#define XGMAC_L3PEN0			BIT(0)
#define XGMAC_L4_ADDR			0x1
#define XGMAC_L4DP0			GENMASK(31, 16)
#define XGMAC_L4DP0_SHIFT		16
#define XGMAC_L4SP0			GENMASK(15, 0)
#define XGMAC_L3_ADDR0			0x4
#define XGMAC_L3_ADDR1			0x5
#define XGMAC_L3_ADDR2			0x6
#define XGMAC_L3_ADDR3			0x7
#define XGMAC_ARP_ADDR			0x00000c10
#define XGMAC_RSS_CTRL			0x00000c80
#define XGMAC_UDP4TE			BIT(3)
#define XGMAC_TCP4TE			BIT(2)
#define XGMAC_IP2TE			BIT(1)
#define XGMAC_RSSE			BIT(0)
#define XGMAC_RSS_ADDR			0x00000c88
#define XGMAC_RSSIA_SHIFT		8
#define XGMAC_ADDRT			BIT(2)
#define XGMAC_CT			BIT(1)
#define XGMAC_OB			BIT(0)
#define XGMAC_RSS_DATA			0x00000c8c
#define XGMAC_TIMESTAMP_STATUS		0x00000d20
#define XGMAC_TXTSC			BIT(15)
#define XGMAC_TXTIMESTAMP_NSEC		0x00000d30
#define XGMAC_TXTSSTSLO			GENMASK(30, 0)
#define XGMAC_TXTIMESTAMP_SEC		0x00000d34
#define XGMAC_PPS_CONTROL		0x00000d70
#define XGMAC_PPS_MAXIDX(x)		((((x) + 1) * 8) - 1)
#define XGMAC_PPS_MINIDX(x)		((x) * 8)
#define XGMAC_PPSx_MASK(x)		\
	GENMASK(XGMAC_PPS_MAXIDX(x), XGMAC_PPS_MINIDX(x))
#define XGMAC_TRGTMODSELx(x, val)	\
	GENMASK(XGMAC_PPS_MAXIDX(x) - 1, XGMAC_PPS_MAXIDX(x) - 2) & \
	((val) << (XGMAC_PPS_MAXIDX(x) - 2))
#define XGMAC_PPSCMDx(x, val)		\
	GENMASK(XGMAC_PPS_MINIDX(x) + 3, XGMAC_PPS_MINIDX(x)) & \
	((val) << XGMAC_PPS_MINIDX(x))
#define XGMAC_PPSCMD_START		0x2
#define XGMAC_PPSCMD_STOP		0x5
#define XGMAC_PPSEN0			BIT(4)
#define XGMAC_PPSx_TARGET_TIME_SEC(x)	(0x00000d80 + (x) * 0x10)
#define XGMAC_PPSx_TARGET_TIME_NSEC(x)	(0x00000d84 + (x) * 0x10)
#define XGMAC_TRGTBUSY0			BIT(31)
#define XGMAC_PPSx_INTERVAL(x)		(0x00000d88 + (x) * 0x10)
#define XGMAC_PPSx_WIDTH(x)		(0x00000d8c + (x) * 0x10)

#define XGMAC_MDIO_ADDR			0x00000200
#define XGMAC_MDIO_DATA			0x00000204
#define XGMAC_MDIO_INT_STATUS		0x00000214
#define XGMAC_MDIO_INT_EN		0x00000218
#define XGMAC_MDIO_INT_EN_SINGLE	BIT(12)
#define XGMAC_MDIO_C22P			0x00000220

/* MDIO defines */
#define MII_GMAC_PA			GENMASK(15, 11)
#define MII_GMAC_RA			GENMASK(10, 6)
#define MII_GMAC_CR			GENMASK(5, 2)
#define MII_GMAC_WRITE			BIT(1)
#define MII_GMAC_BUSY			BIT(0)
#define MII_DATA_MASK			GENMASK(15, 0)
#define MII_XGMAC_DA			GENMASK(25, 21)
#define MII_XGMAC_PA			GENMASK(20, 16)
#define MII_XGMAC_RA			GENMASK(15, 0)
#define MII_XGMAC_BUSY			BIT(22)
#define MII_XGMAC_CR			GENMASK(21, 19)
#define MII_XGMAC_SADDR			BIT(18)
#define MII_XGMAC_CMD_SHIFT		16
#define MII_XGMAC_WRITE			(1 << MII_XGMAC_CMD_SHIFT)
#define MII_XGMAC_READ			(3 << MII_XGMAC_CMD_SHIFT)
#define MII_XGMAC_PSE			BIT(30)
#define MII_XGMAC_CRS			BIT(31)

/* XGMAC MMC Registers */
#define MMC_XGMAC_CONTROL		0x800
#define MMC_XGMAC_CONTROL_RSTONRD	BIT(2)
#define MMC_XGMAC_CONTROL_RESET		BIT(0)
#define MMC_XGMAC_RX_INT_EN		0x80c
#define MMC_XGMAC_TX_INT_EN		0x810

#define MMC_XGMAC_TX_OCTET_GB		0x814
#define MMC_XGMAC_TX_PKT_GB		0x81c
#define MMC_XGMAC_TX_BROAD_PKT_G	0x824
#define MMC_XGMAC_TX_MULTI_PKT_G	0x82c
#define MMC_XGMAC_TX_64OCT_GB		0x834
#define MMC_XGMAC_TX_65OCT_GB		0x83c
#define MMC_XGMAC_TX_128OCT_GB		0x844
#define MMC_XGMAC_TX_256OCT_GB		0x84c
#define MMC_XGMAC_TX_512OCT_GB		0x854
#define MMC_XGMAC_TX_1024OCT_GB		0x85c
#define MMC_XGMAC_TX_UNI_PKT_GB		0x864
#define MMC_XGMAC_TX_MULTI_PKT_GB	0x86c
#define MMC_XGMAC_TX_BROAD_PKT_GB	0x874
#define MMC_XGMAC_TX_UNDER		0x87c
#define MMC_XGMAC_TX_OCTET_G		0x884
#define MMC_XGMAC_TX_PKT_G		0x88c
#define MMC_XGMAC_TX_PAUSE		0x894
#define MMC_XGMAC_TX_VLAN_PKT_G		0x89c
#define MMC_XGMAC_TX_LPI_USEC		0x8a4
#define MMC_XGMAC_TX_LPI_TRAN		0x8a8

#define MMC_XGMAC_RX_PKT_GB		0x900
#define MMC_XGMAC_RX_OCTET_GB		0x908
#define MMC_XGMAC_RX_OCTET_G		0x910
#define MMC_XGMAC_RX_BROAD_PKT_G	0x918
#define MMC_XGMAC_RX_MULTI_PKT_G	0x920
#define MMC_XGMAC_RX_CRC_ERR		0x928
#define MMC_XGMAC_RX_RUNT_ERR		0x930
#define MMC_XGMAC_RX_JABBER_ERR		0x934
#define MMC_XGMAC_RX_UNDER		0x938
#define MMC_XGMAC_RX_OVER		0x93c
#define MMC_XGMAC_RX_64OCT_GB		0x940
#define MMC_XGMAC_RX_65OCT_GB		0x948
#define MMC_XGMAC_RX_128OCT_GB		0x950
#define MMC_XGMAC_RX_256OCT_GB		0x958
#define MMC_XGMAC_RX_512OCT_GB		0x960
#define MMC_XGMAC_RX_1024OCT_GB		0x968
#define MMC_XGMAC_RX_UNI_PKT_G		0x970
#define MMC_XGMAC_RX_LENGTH_ERR		0x978
#define MMC_XGMAC_RX_RANGE		0x980
#define MMC_XGMAC_RX_PAUSE		0x988
#define MMC_XGMAC_RX_FIFOOVER_PKT	0x990
#define MMC_XGMAC_RX_VLAN_PKT_GB	0x998
#define MMC_XGMAC_RX_WATCHDOG_ERR	0x9a0
#define MMC_XGMAC_RX_LPI_USEC		0x9a4
#define MMC_XGMAC_RX_LPI_TRAN		0x9a8
#define MMC_XGMAC_RX_DISCARD_PKT_GB	0x9ac
#define MMC_XGMAC_RX_DISCARD_OCT_GB	0x9b4
#define MMC_XGMAC_RX_ALIGN_ERR_PKT	0x9bc

#define MMC_XGMAC_TX_SINGLE_COL_G	0xa40
#define MMC_XGMAC_TX_MULTI_COL_G	0xa44
#define MMC_XGMAC_TX_DEFER		0xa48
#define MMC_XGMAC_TX_LATE_COL		0xa4c
#define MMC_XGMAC_TX_EXCESSIVE_COL	0xa50
#define MMC_XGMAC_TX_CARRIER		0xa54
#define MMC_XGMAC_TX_EXCESSIVE_DEFER	0xa58

#define MMC_XGMAC_RX_IPC_INTR_MASK	0xa5c

#define MMC_XGMAC_RX_IPV4_PKT_G		0xa64
#define MMC_XGMAC_RX_IPV4_HDRERR_PKT	0xa6c
#define MMC_XGMAC_RX_IPV4_NOPAY_PKT	0xa74
#define MMC_XGMAC_RX_IPV4_FRAG_PKT	0xa7c
#define MMC_XGMAC_RX_IPV4_UDSBL_PKT	0xa84
#define MMC_XGMAC_RX_IPV6_PKT_G		0xa8c
#define MMC_XGMAC_RX_IPV6_HDRERR_PKT	0xa94
#define MMC_XGMAC_RX_IPV6_NOPAY_PKT	0xa9c
#define MMC_XGMAC_RX_UDP_PKT_G		0xaa4
#define MMC_XGMAC_RX_UDP_ERR_PKT	0xaac
#define MMC_XGMAC_RX_TCP_PKT_G		0xab4
#define MMC_XGMAC_RX_TCP_ERR_PKT	0xabc
#define MMC_XGMAC_RX_ICMP_PKT_G		0xac4
#define MMC_XGMAC_RX_ICMP_ERR_PKT	0xacc
#define MMC_XGMAC_RX_IPV4_OCTET_G	0xad4
#define MMC_XGMAC_RX_IPV4_HDRERR_OCTET	0xadc
#define MMC_XGMAC_RX_IPV4_NOPAY_OCTET	0xae4
#define MMC_XGMAC_RX_IPV4_FRAG_OCTET	0xaec
#define MMC_XGMAC_RX_IPV4_UDSBL_OCTET	0xaf4
#define MMC_XGMAC_RX_IPV6_OCTET_G	0xafc
#define MMC_XGMAC_RX_IPV6_HDRERR_OCTET	0xb04
#define MMC_XGMAC_RX_IPV6_NOPAY_OCTET	0xb0c
#define MMC_XGMAC_RX_UDP_OCTET_G	0xb14
#define MMC_XGMAC_RX_UDP_ERR_OCTET	0xb1c
#define MMC_XGMAC_RX_TCP_OCTET_G	0xb24
#define MMC_XGMAC_RX_TCP_ERR_OCTET	0xb2c
#define MMC_XGMAC_RX_ICMP_OCTET_G	0xb34
#define MMC_XGMAC_RX_ICMP_ERR_OCTET	0xb3c

/* MTL Registers */
#define XGMAC_MTL_OPMODE		0x00001000
#define XGMAC_FRPE			BIT(15)
#define XGMAC_ETSALG			GENMASK(6, 5)
#define XGMAC_WRR			(0x0 << 5)
#define XGMAC_WFQ			(0x1 << 5)
#define XGMAC_DWRR			(0x2 << 5)
#define XGMAC_RAA			BIT(2)
#define XGMAC_FTS			BIT(1)
#define XGMAC_MTL_INT_STATUS		0x00001020
#define XGMAC_MTL_RXQ_DMA_MAP0		0x00001030
#define XGMAC_MTL_RXQ_DMA_MAP1		0x00001034
#define XGMAC_QxMDMACH(x)		GENMASK((x) * 8 + 7, (x) * 8)
#define XGMAC_QxMDMACH_SHIFT(x)		((x) * 8)
#define XGMAC_QDDMACH			BIT(7)
#define XGMAC_TC_PRTY_MAP0		0x00001040
#define XGMAC_TC_PRTY_MAP1		0x00001044
#define XGMAC_PSTC(x)			GENMASK((x) * 8 + 7, (x) * 8)
#define XGMAC_PSTC_SHIFT(x)		((x) * 8)
#define XGMAC_MTL_EST_CONTROL		0x00001050
#define XGMAC_PTOV			GENMASK(31, 23)
#define XGMAC_PTOV_SHIFT		23
#define XGMAC_SSWL			BIT(1)
#define XGMAC_EEST			BIT(0)
#define XGMAC_MTL_EST_GCL_CONTROL	0x00001080
#define XGMAC_BTR_LOW			0x0
#define XGMAC_BTR_HIGH			0x1
#define XGMAC_CTR_LOW			0x2
#define XGMAC_CTR_HIGH			0x3
#define XGMAC_TER			0x4
#define XGMAC_LLR			0x5
#define XGMAC_ADDR_SHIFT		8
#define XGMAC_GCRR			BIT(2)
#define XGMAC_SRWO			BIT(0)
#define XGMAC_MTL_EST_GCL_DATA		0x00001084
#define XGMAC_MTL_RXP_CONTROL_STATUS	0x000010a0
#define XGMAC_RXPI			BIT(31)
#define XGMAC_NPE			GENMASK(23, 16)
#define XGMAC_NVE			GENMASK(7, 0)
#define XGMAC_MTL_RXP_IACC_CTRL_ST	0x000010b0
#define XGMAC_STARTBUSY			BIT(31)
#define XGMAC_WRRDN			BIT(16)
#define XGMAC_ADDR			GENMASK(9, 0)
#define XGMAC_MTL_RXP_IACC_DATA		0x000010b4
#define XGMAC_MTL_ECC_CONTROL		0x000010c0
#define XGMAC_MTL_SAFETY_INT_STATUS	0x000010c4
#define XGMAC_MEUIS			BIT(1)
#define XGMAC_MECIS			BIT(0)
#define XGMAC_MTL_ECC_INT_ENABLE	0x000010c8
#define XGMAC_RPCEIE			BIT(12)
#define XGMAC_ECEIE			BIT(8)
#define XGMAC_RXCEIE			BIT(4)
#define XGMAC_TXCEIE			BIT(0)
#define XGMAC_MTL_ECC_INT_STATUS	0x000010cc
#define XGMAC_MTL_TXQ_OPMODE(x)		(0x00001100 + 0x80 * (x))
#define XGMAC_TQS			GENMASK(25, 16)
#define XGMAC_TQS_SHIFT			16
#define XGMAC_Q2TCMAP			GENMASK(10, 8)
#define XGMAC_Q2TCMAP_SHIFT		8
#define XGMAC_TTC			GENMASK(6, 4)
#define XGMAC_TTC_SHIFT			4
#define XGMAC_TXQEN			GENMASK(3, 2)
#define XGMAC_TXQEN_SHIFT		2
#define XGMAC_TSF			BIT(1)
#define XGMAC_FTQ			BIT(0)
#define XGMAC_MTL_TXQ_DEBUG(x)		(0x00001108 + 0x80 * (x))
#define XGMAC_TRCPSTS			BIT(5)
#define XGMAC_TXQSTS			BIT(4)
#define XGMAC_TWCSTS			BIT(3)
#define XGMAC_TRCSTS			GENMASK(2, 1)
#define XGMAC_TCPAUSED			BIT(0)
#define XGMAC_MTL_TCx_ETS_CONTROL(x)	(0x00001110 + 0x80 * (x))
#define XGMAC_MTL_TCx_QUANTUM_WEIGHT(x)	(0x00001118 + 0x80 * (x))
#define XGMAC_MTL_TCx_SENDSLOPE(x)	(0x0000111c + 0x80 * (x))
#define XGMAC_MTL_TCx_HICREDIT(x)	(0x00001120 + 0x80 * (x))
#define XGMAC_MTL_TCx_LOCREDIT(x)	(0x00001124 + 0x80 * (x))
#define XGMAC_CC			BIT(3)
#define XGMAC_TSA			GENMASK(1, 0)
#define XGMAC_SP			(0x0 << 0)
#define XGMAC_CBS			(0x1 << 0)
#define XGMAC_ETS			(0x2 << 0)
#define XGMAC_MTL_RXQ_OPMODE(x)		(0x00001140 + 0x80 * (x))
#define XGMAC_RQS			GENMASK(25, 16)
#define XGMAC_RQS_SHIFT			16
#define XGMAC_EHFC			BIT(7)
#define XGMAC_RSF			BIT(5)
#define XGMAC_RTC			GENMASK(1, 0)
#define XGMAC_RTC_SHIFT			0
#define XGMAC_MTL_RXQ_OVF_CNT(x)		(0x00001144 + 0x80 * (x))
#define XGMAC_MISCNTOVF			BIT(31)
#define XGMAC_MISPKTCNT			GENMASK(26, 16)
#define XGMAC_OVFCNTOVF			BIT(15)
#define XGMAC_OVFPKTCNT			GENMASK(10, 0)
#define XGMAC_MTL_RXQ_DEBUG(x)			(0x00001148 + 0x80 * (x))
#define XGMAC_PRXQ				GENMASK(29, 16)
#define XGMAC_RXQSTS			GENMASK(5, 4)
#define XGMAC_RRCSTS			GENMASK(2, 1)
#define XGMAC_RWCSTS			BIT(0)
#define XGMAC_MTL_RXQ_FLOW_CONTROL(x)	(0x00001150 + 0x80 * (x))
#define XGMAC_RFD			GENMASK(31, 17)
#define XGMAC_RFD_SHIFT			17
#define XGMAC_RFA			GENMASK(15, 1)
#define XGMAC_RFA_SHIFT			1
#define XGMAC_MTL_QINTEN(x)		(0x00001170 + 0x80 * (x))
#define XGMAC_RXOIE			BIT(16)
#define XGMAC_MTL_QINT_STATUS(x)	(0x00001174 + 0x80 * (x))
#define XGMAC_RXOVFIS			BIT(16)
#define XGMAC_ABPSIS			BIT(1)
#define XGMAC_TXUNFIS			BIT(0)
#define XGMAC_MAC_REGSIZE		(XGMAC_MTL_QINT_STATUS(15) / 4)

#define XGMAC_DMA_MODE			0x00003000
#define XGMAC_INTM			GENMASK(13, 12)
#define XGMAC_SWR			BIT(0)
#define XGMAC_DMA_SYSBUS_MODE		0x00003004
#define XGMAC_WR_OSR_LMT		GENMASK(29, 24)
#define XGMAC_WR_OSR_LMT_SHIFT		24
#define XGMAC_RD_OSR_LMT		GENMASK(21, 16)
#define XGMAC_RD_OSR_LMT_SHIFT		16
#define XGMAC_EN_LPI			BIT(15)
#define XGMAC_LPI_XIT_PKT		BIT(14)
#define XGMAC_AAL			BIT(12)
#define XGMAC_EAME			BIT(11)
#define XGMAC_BLEN			GENMASK(7, 1)
#define XGMAC_BLEN256			BIT(7)
#define XGMAC_BLEN128			BIT(6)
#define XGMAC_BLEN64			BIT(5)
#define XGMAC_BLEN32			BIT(4)
#define XGMAC_BLEN16			BIT(3)
#define XGMAC_BLEN8			BIT(2)
#define XGMAC_BLEN4			BIT(1)
#define XGMAC_UNDEF			BIT(0)
#define XGMAC_DMA_INT_STATUS		0x00003008
#define XGMAC_MTLIS			BIT(16)
#define XGMAC_DMA_DEBUG_STATUS(x)	(0x00003020 + 4 * (x))
#define XGMAC_DMA_DBG_STS3_RDAS			BIT(0)
#define XGMAC_DMA_DBG_STS1_TDAS			BIT(0)
#define XGMAC_TX_EDMA_CTRL		0x00003040
#define XGMAC_TEDM			GENMASK(31, 30)
#define XGMAC_TDPS			GENMASK(29, 0)
#define XGMAC_RX_EDMA_CTRL		0x00003044
#define XGMAC_REDM			GENMASK(31, 30)
#define XGMAC_RDPS			GENMASK(29, 0)
#define XGMAC_DMA_TBS_CTRL0		0x00003054
#define XGMAC_DMA_TBS_CTRL1		0x00003058
#define XGMAC_DMA_TBS_CTRL2		0x0000305c
#define XGMAC_DMA_TBS_CTRL3		0x00003060
#define XGMAC_FTOS			GENMASK(31, 8)
#define XGMAC_FTOV			BIT(0)
#define XGMAC_DEF_FTOS			(XGMAC_FTOS | XGMAC_FTOV)
#define XGMAC_DMA_SAFETY_INT_STATUS	0x00003064
#define XGMAC_MCSIS			BIT(31)
#define XGMAC_MSUIS			BIT(29)
#define XGMAC_MSCIS			BIT(28)
#define XGMAC_DEUIS			BIT(1)
#define XGMAC_DECIS			BIT(0)
#define XGMAC_DMA_ECC_INT_ENABLE	0x00003068
#define XGMAC_DCEIE			BIT(1)
#define XGMAC_TCEIE			BIT(0)
#define XGMAC_DMA_ECC_INT_STATUS	0x0000306c
#define XGMAC_DMA_CH_CONTROL(x)		(0x00003100 + 0x80 * (x))
#define XGMAC_SPH			BIT(24)
#define XGMAC_PBLx8			BIT(16)
#define XGMAC_DMA_CH_TX_CONTROL(x)	(0x00003104 + 0x80 * (x))
#define XGMAC_EDSE			BIT(28)
#define XGMAC_TxPBL			GENMASK(21, 16)
#define XGMAC_TxPBL_SHIFT		16
#define XGMAC_TSE			BIT(12)
#define XGMAC_OSP			BIT(4)
#define XGMAC_TXST			BIT(0)
#define XGMAC_DMA_CH_RX_CONTROL(x)	(0x00003108 + 0x80 * (x))
#define XGMAC_RPF			BIT(31)
#define XGMAC_RxPBL			GENMASK(21, 16)
#define XGMAC_RxPBL_SHIFT		16
#define XGMAC_RBSZ			GENMASK(14, 1)
#define XGMAC_RBSZ_SHIFT		1
#define XGMAC_RXST			BIT(0)
#define XGMAC_DMA_CH_TxDESC_LADDR(x)	(0x00003114 + 0x80 * (x))
#define XGMAC_DMA_CH_RxDESC_LADDR(x)	(0x0000311c + 0x80 * (x))
#define XGMAC_DMA_CH_TxDESC_TAIL_LPTR(x)	(0x00003124 + 0x80 * (x))
#define XGMAC_DMA_CH_RxDESC_TAIL_LPTR(x)	(0x0000312c + 0x80 * (x))
#define XGMAC_DMA_CH_TxDESC_RING_LEN(x)		(0x00003130 + 0x80 * (x))
#define XGMAC_DMA_CH_RxDESC_RING_LEN(x)		(0x00003134 + 0x80 * (x))
#define XGMAC_OWRQ                     GENMASK(26, 24)
#define XGMAC_DMA_CH_INT_EN(x)		(0x00003138 + 0x80 * (x))
#define XGMAC_NIE			BIT(15)
#define XGMAC_AIE			BIT(14)
#define XGMAC_CDEE			BIT(13)
#define XGMAC_FBEE			BIT(12)
#define XGMAC_DDEE			BIT(9)
#define XGMAC_RSE			BIT(8)
#define XGMAC_RBUE			BIT(7)
#define XGMAC_RIE			BIT(6)
#define XGMAC_TBUE			BIT(2)
#define XGMAC_TXSE			BIT(1)
#define XGMAC_TIE			BIT(0)
#define XGMAC_DMA_INT_DEFAULT_EN	(XGMAC_DMA_INT_NORMAL_EN | \
				XGMAC_DMA_INT_ABNORMAL_EN)
#define XGMAC_DMA_INT_NORMAL_EN		(XGMAC_NIE | XGMAC_TIE | XGMAC_RIE)
#define XGMAC_DMA_INT_ABNORMAL_EN	(XGMAC_AIE | XGMAC_RBUE | XGMAC_CDEE | XGMAC_DDEE | XGMAC_FBEE)
#define XGMAC_DMA_CH_Rx_WATCHDOG(x)	(0x0000313c + 0x80 * (x))
#define XGMAC_PSEL                     BIT(31)
#define XGMAC_RBCT                     GENMASK(25, 16)
#define XGMAC_RWTU                     GENMASK(13, 12)
#define XGMAC_RWT			GENMASK(7, 0)
#define XGMAC_DMA_CH_CUR_TxDESC_LADDR(x)	(0x00003144 + 0x80 * (x))
#define XGMAC_DMA_CH_CUR_RxDESC_LADDR(x)	(0x0000314c + 0x80 * (x))
#define XGMAC_DMA_CH_CUR_TxBUFF_LADDR(x)	(0x00003154 + 0x80 * (x))
#define XGMAC_DMA_CH_CUR_RxBUFF_LADDR(x)	(0x0000315c + 0x80 * (x))
#define XGMAC_DMA_CH_STATUS(x)		(0x00003160 + 0x80 * (x))
#define XGMAC_NIS			BIT(15)
#define XGMAC_AIS			BIT(14)
#define XGMAC_CDE			BIT(13)
#define XGMAC_FBE			BIT(12)
#define XGMAC_DDE			BIT(9)
#define XGMAC_RPS			BIT(8)
#define XGMAC_RBU			BIT(7)
#define XGMAC_RI			BIT(6)
#define XGMAC_TBU			BIT(2)
#define XGMAC_TPS			BIT(1)
#define XGMAC_TI			BIT(0)
#define XGMAC_DMA_CH_DEBUG_STATUS(x)	(0x00003164 + 0x80 * (x))
#define XGMAC_RDTS			GENMASK(27, 19)
#define XGMAC_RDFS			GENMASK(18, 16)
#define XGMAC_TDTS			GENMASK(11, 8)
#define XGMAC_TDRS			GENMASK(7, 6)
#define XGMAC_TDXS			GENMASK(5, 3)
#define XGMAC_TDFS			GENMASK(2, 0)
#define XGMAC_REGSIZE			((0x0000317c + (0x80 * 15)) / 4)

#define XGMAC_DMA_STATUS_MSK_COMMON	(XGMAC_NIS | XGMAC_AIS | XGMAC_FBE)
#define XGMAC_DMA_STATUS_MSK_RX		(XGMAC_RBU | XGMAC_RI | \
					 XGMAC_DMA_STATUS_MSK_COMMON)
#define XGMAC_DMA_STATUS_MSK_TX		(XGMAC_TBU | XGMAC_TPS | XGMAC_TI | \
					 XGMAC_DMA_STATUS_MSK_COMMON)

/* Descriptors */
#define XGMAC_TDES0_LTV			BIT(31)
#define XGMAC_TDES0_LT			GENMASK(7, 0)
#define XGMAC_TDES1_LT			GENMASK(31, 8)
#define XGMAC_TDES2_IVT			GENMASK(31, 16)
#define XGMAC_TDES2_IVT_SHIFT		16
#define XGMAC_TDES2_IOC			BIT(31)
#define XGMAC_TDES2_TTSE		BIT(30)
#define XGMAC_TDES2_B2L			GENMASK(29, 16)
#define XGMAC_TDES2_B2L_SHIFT		16
#define XGMAC_TDES2_VTIR		GENMASK(15, 14)
#define XGMAC_TDES2_VTIR_SHIFT		14
#define XGMAC_TDES2_B1L			GENMASK(13, 0)
#define XGMAC_TDES3_OWN			BIT(31)
#define XGMAC_TDES3_CTXT		BIT(30)
#define XGMAC_TDES3_FD			BIT(29)
#define XGMAC_TDES3_LD			BIT(28)
#define XGMAC_TDES3_CPC			GENMASK(27, 26)
#define XGMAC_TDES3_CPC_SHIFT		26
#define XGMAC_TDES3_TCMSSV		BIT(26)
#define XGMAC_TDES3_SAIC		GENMASK(25, 23)
#define XGMAC_TDES3_SAIC_SHIFT		23
#define XGMAC_TDES3_TBSV		BIT(24)
#define XGMAC_TDES3_THL			GENMASK(22, 19)
#define XGMAC_TDES3_THL_SHIFT		19
#define XGMAC_TDES3_IVTIR		GENMASK(19, 18)
#define XGMAC_TDES3_IVTIR_SHIFT		18
#define XGMAC_TDES3_TSE			BIT(18)
#define XGMAC_TDES3_IVLTV		BIT(17)
#define XGMAC_TDES3_CIC			GENMASK(17, 16)
#define XGMAC_TDES3_CIC_SHIFT		16
#define XGMAC_TDES3_TPL			GENMASK(17, 0)
#define XGMAC_TDES3_VLTV		BIT(16)
#define XGMAC_TDES3_VT			GENMASK(15, 0)
#define XGMAC_TDES3_FL			GENMASK(14, 0)
#define XGMAC_TDES3_PIDV		BIT(25)
#define XGMAC_TDES0_QUEUE_ID	GENMASK(19, 17)
#define XGMAC_TDES0_FAST_MODE	BIT(16)
#define XGMAC_TDES0_OVPORT		GENMASK(12, 8)
#define XGMAC_TDES0_IVPORT		GENMASK(4, 0)


#define XGMAC_RDES0_IP_FRAG  	GENMASK(31, 30)
enum {
	FRAG_NONE,
	FRAG_FIRST,
	FRAG_MIDDLE,
	FRAG_LAST,
};
#define XGMAC_RDES0_L3_TYPE		GENMASK(29, 28)
enum {
	L3_TYPE_IPV4,
	L3_TYPE_IPV6,
	L3_TYPE_IPIP6,
	L3_TYPE_UNKNOWN,
};
#define XGMAC_RDES0_L4_TYPE		GENMASK(27, 26)
enum {
	L4_TYPE_TCP,
	L4_TYPE_UDP,
	L4_TYPE_ICMP,
	L4_TYPE_UNKNOWN,
};
#define XGMAC_RDES0_RPT_INDEX	GENMASK(25, 22)
#define XGMAC_RDES0_STA_INDEX	GENMASK(21, 12)
#define XGMAC_RDES0_OVPORT   	GENMASK(11, 6)
#define XGMAC_RDES0_IVPORT   	GENMASK(5, 0)
#define XGMAC_RDES2_DFRAG		BIT(31)
#define XGMAC_RDES2_OVID		GENMASK(27, 16)
#define XGMAC_RDES3_OWN			BIT(31)
#define XGMAC_RDES3_CTXT		BIT(30)
#define XGMAC_RDES3_FD			BIT(29)
#define XGMAC_RDES3_LD			BIT(28)
#define XGMAC_RDES3_CDA			BIT(27)
#define XGMAC_RDES3_RSV			BIT(26)
#define XGMAC_RDES3_TCI_PRI		GENMASK(22, 20)
#define XGMAC_RDES3_ET			GENMASK(19, 16)
#define XGMAC_RDES3_ES			BIT(15)
#define XGMAC_RDES3_PL			GENMASK(13, 0)

#define XGMAC_RDES0_SPORT   	GENMASK(31, 16)
#define XGMAC_RDES0_ETH_TYPE	GENMASK(15, 0)
#define XGMAC_RDES1_UP_REASON   GENMASK(31, 24)
#define XGMAC_RDES1_RXHASH      GENMASK(23, 8)
#define XGMAC_RDES1_TNP     	BIT(6)
#define XGMAC_RDES1_DSCP        GENMASK(5, 0)
#define XGMAC_RDES2_SMAC_0_31   GENMASK(31, 0)
#define XGMAC_RDES3_SMAC_32_47  GENMASK(15, 0)
#define XGMAC_RDES3_PKT_TYPE    GENMASK(17, 16)
enum {
	PKT_TYPE_UCAST,
	PKT_TYPE_MCAST,
	PKT_TYPE_UNKNOWN,
	PKT_TYPE_BCAST,
};
#define XGMAC_RDES3_IOC			BIT(30)

#endif