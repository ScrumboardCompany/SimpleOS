#include "fs/disk.h"
#include "libs/string/string.h"

using namespace SimpleOS;

void SimpleOS::ata_wait_bsy() {
    while (IRQ::port_byte_in(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_BSY);
}

void SimpleOS::ata_wait_drq() {
    while (!(IRQ::port_byte_in(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_DRQ));
}

uint32_t SimpleOS::ata_get_sector_count() {
    uint16_t buffer[256];

    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 6, 0xA0);
    IRQ::port_byte_out(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_IDENTIFY);

    ata_wait_bsy();

    for (int i = 0; i < 256; i++) {
        buffer[i] = IRQ::inw(ATA_PRIMARY_IO_BASE);
    }

    uint32_t sector_count = buffer[60] | (buffer[61] << 16);

    return sector_count;
}

void SimpleOS::ata_write_to_sector(uint32_t lba, const char* buffer) {
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

bool SimpleOS::ata_can_write_to_sector(uint32_t lba, size_t buffer_size) {
    char data[512];
    ata_read_sector(lba, data);

    size_t data_length = strnlen(data, 512);
    size_t space_left = 512 - data_length;

    return buffer_size <= space_left;
}

bool SimpleOS::ata_append_to_sector(uint32_t lba, const char* buffer) {
    char data[512];
    ata_read_sector(lba, data);

    size_t data_length = strnlen(data, 512);
    size_t space_left = 512 - data_length;
    size_t buffer_size = strlen(buffer);

    if (buffer_size > space_left) {
        return false;
    }

    memcpy(data + data_length, buffer, buffer_size);
    ata_write_to_sector(lba, data);

    return true;
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

void SimpleOS::ata_delete_from_sector(uint32_t lba) {
    char empty_data[512];

    memset(empty_data, 0, 512);

    ata_write_to_sector(lba, empty_data);
}

void SimpleOS::ata_delete_from_sector(uint32_t lba, size_t start, size_t length) {
    char data[512];

    ata_read_sector(lba, data);

    if (start >= 512 || start + length > 512) {
        return;
    }

    memset(data + start, 0, length);

    ata_write_to_sector(lba, data);
}