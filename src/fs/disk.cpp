#include "fs/disk.h"

using namespace SimpleOS;

void SimpleOS::ata_wait_bsy() {
    while (IRQ::port_byte_in(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_BSY);
}

void SimpleOS::ata_wait_drq() {
    while (!(IRQ::port_byte_in(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_DRQ));
}

void SimpleOS::ata_write_sector(uint32_t lba, uint8_t* buffer) {
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 2, 1);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 3, (uint8_t)lba);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 4, (uint8_t)(lba >> 8));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 5, (uint8_t)(lba >> 16));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_WRITE);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        IRQ::outw(ATA_PRIMARY_IO_BASE, ((uint16_t*)buffer)[i]);
    }

    ata_wait_bsy();
}

void SimpleOS::ata_write_sector(uint32_t lba, char* buffer) {
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 2, 1);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 3, (uint8_t)lba);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 4, (uint8_t)(lba >> 8));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 5, (uint8_t)(lba >> 16));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_WRITE);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        IRQ::outw(ATA_PRIMARY_IO_BASE, ((uint16_t*)buffer)[i]);
    }

    ata_wait_bsy();
}

void SimpleOS::ata_read_sector(uint32_t lba, char* buffer) {
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 2, 1);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 3, (uint8_t)lba);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 4, (uint8_t)(lba >> 8));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 5, (uint8_t)(lba >> 16));
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_READ);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buffer)[i] = IRQ::inw(ATA_PRIMARY_IO_BASE);
    }

    ata_wait_bsy();
}