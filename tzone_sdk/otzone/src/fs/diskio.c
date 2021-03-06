#include <diskio.h>
#include <mmc.h>
#include <block_dev.h>
#include <sw_debug.h>
#include <sw_board.h>

/**
* @brief reads contents from MMC card  
*
* @return 
*/
u32 read_from_disk(){
	struct mmc *mmc;
	enable_l1_cache();
	if(mmc_initialize()){
		DIE_NOW(0, "Not able to read from MMC\n");
	}

	mmc = find_mmc_device(0);
	block_dev_desc_t *block_dev = &mmc->block_dev;
	u32 x = 0, ret = 0;
	u32 fs_start = (u32)sw_vir_page_alloc(SW_FS_SIZE);
	sw_printf("Reading ramfs from emmc... 0x%x\n", fs_start);
	for(x; x < SW_FS_BLKCNT; x++){
		u32 blk = SW_FS_STARTBLK + x;
		u32 dest = fs_start + (x * 512);
		ret += block_dev->block_read(block_dev->dev, blk, 1, (void*)dest);
	}
	return fs_start;
}


