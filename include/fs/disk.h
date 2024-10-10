#pragma once

#ifndef __DISK__
#define __DISK__

#include "libs/io/io.h"
#include "utils/typedef.h"
#include "utils/utils.h"

#define ATA_PRIMARY_IO_BASE   0x1F0
#define ATA_PRIMARY_CTRL_BASE 0x3F6
#define ATA_COMMAND_WRITE     0x30
#define ATA_COMMAND_READ      0x20
#define ATA_STATUS_BSY        0x80
#define ATA_STATUS_DRQ        0x08
#define ATA_COMMAND_IDENTIFY 0xEC

namespace SimpleOS {

    void ata_wait_bsy();

    void ata_wait_drq();

    uint32_t ata_get_sector_count();

    void ata_write_to_sector(uint32_t lba, const string& buffer);

    bool ata_can_write_to_sector(uint32_t lba, size_t buffer_size);

    bool ata_append_to_sector(uint32_t lba, const string& buffer);

    void ata_read_sector(uint32_t lba, string& buffer);

    void ata_delete_from_sector(uint32_t lba);

    size_t ata_get_free_space_in_sector(uint32_t lba);
}

#endif // __DISK__