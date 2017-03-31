#ifndef _MESON_GX_MMC_PARTITIONS_H
#define _MESON_GX_MMC_PARTITIONS_H

#include<linux/genhd.h>

#include <linux/mmc/host.h>
#include <linux/mmc/card.h>
#include <linux/mmc/mmc.h>
#include <linux/mmc/core.h>

#define CONFIG_DTB_SIZE  (256*1024U)
#define	STORE_CODE				1
#define	STORE_CACHE				(1<<1)
#define	STORE_DATA				(1<<2)

#define     MAX_PART_NAME_LEN               16
#define     MAX_MMC_PART_NUM                32

/* MMC Partition Table */
#define     MMC_PARTITIONS_MAGIC            "MPT"
#define     MMC_RESERVED_NAME               "reserved"

#define     SZ_1M                           0x00100000

/* the size of bootloader partition */
#define     MMC_BOOT_PARTITION_SIZE         (4*SZ_1M)

/* the size of reserve space behind bootloader partition */
#define     MMC_BOOT_PARTITION_RESERVED     (32*SZ_1M)

#define     RESULT_OK                       0
#define     RESULT_FAIL                     1
#define     RESULT_UNSUP_HOST               2
#define     RESULT_UNSUP_CARD               3

#define	 SPI_BOOT_FLAG				   0
#define	 NAND_BOOT_FLAG				  1
#define	 EMMC_BOOT_FLAG				  2
#define	 CARD_BOOT_FLAG				  3
#define	 SPI_NAND_FLAG				   4
#define	 SPI_EMMC_FLAG				   5

#define	ASSIST_POR_CONFIG	0x1f55

#define R_BOOT_DEVICE_FLAG  (readl(ASSIST_POR_CONFIG))


#define POR_BOOT_VALUE ((((R_BOOT_DEVICE_FLAG>>9)&1)<<2)|\
		((R_BOOT_DEVICE_FLAG>>6)&3)) /* {poc[9],poc[7:6]} */

#define POR_NAND_BOOT() ((POR_BOOT_VALUE == 7) \
		|| (POR_BOOT_VALUE == 6))
#define POR_SPI_BOOT() ((POR_BOOT_VALUE == 5) || (POR_BOOT_VALUE == 4))
/* #define POR_EMMC_BOOT() (POR_BOOT_VALUE == 3) */
#define POR_EMMC_BOOT()	(POR_BOOT_VALUE == 3)

#define POR_CARD_BOOT() (POR_BOOT_VALUE == 0)

#define print_tmp(fmt, args...) \
{ \
	pr_info("[%s] " fmt, __func__, ##args); \
}

#define print_dbg(fmt, args...) \
{ \
	pr_info("[%s] " fmt, __func__, ##args); \
}

struct partitions {
	/* identifier string */
	char name[MAX_PART_NAME_LEN];
	/* partition size, byte unit */
	uint64_t size;
	/* offset within the master space, byte unit */
	uint64_t offset;
	/* master flags to mask out for this partition */
	unsigned mask_flags;
};

struct mmc_partitions_fmt {
	char magic[4];
	unsigned char version[12];
	int part_num;
	int checksum;
	struct partitions partitions[MAX_MMC_PART_NUM];
};

int aml_emmc_partition_ops(struct mmc_card *card, struct gendisk *disk);
unsigned int mmc_capacity(struct mmc_card *card);
int mmc_read_internal(struct mmc_card *card,
		unsigned dev_addr, unsigned blocks, void *buf);
int mmc_write_internal(struct mmc_card *card,
		unsigned dev_addr, unsigned blocks, void *buf);
int get_reserve_partition_off_from_tbl(void);
int get_reserve_partition_off(struct mmc_card *card);/* byte unit */

#endif

extern struct mmc_partitions_fmt *pt_fmt;
