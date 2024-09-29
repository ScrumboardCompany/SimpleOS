#pragma once

#ifndef __DISK__
#define __DISK__

#include <stdint.h>
#include "IRQ/IRQ.h"

#define ATA_PRIMARY_IO_BASE   0x1F0
#define ATA_PRIMARY_CTRL_BASE 0x3F6
#define ATA_COMMAND_WRITE     0x30
#define ATA_COMMAND_READ      0x20
#define ATA_STATUS_BSY        0x80
#define ATA_STATUS_DRQ        0x08

namespace SimpleOS {

    void ata_wait_bsy();

    void ata_wait_drq();

    void ata_write_sector(uint32_t lba, uint8_t* buffer);

    void ata_write_sector(uint32_t lba, char* buffer);

    void ata_read_sector(uint32_t lba, char* buffer);
}

#endif // __DISK__