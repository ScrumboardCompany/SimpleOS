#include "fs/disk.h"
#include "libs/string/string.h"

using namespace SimpleOS;

void SimpleOS::ata_wait_bsy() {
    while (inb(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_BSY);
}

void SimpleOS::ata_wait_drq() {
    while (!(inb(ATA_PRIMARY_IO_BASE + 7) & ATA_STATUS_DRQ));
}

uint32_t SimpleOS::ata_get_sector_count() {
    uint16_t buffer[256];

    outb(ATA_PRIMARY_IO_BASE + 6, 0xA0);
    outb(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_IDENTIFY);

    ata_wait_bsy();

    for (int i = 0; i < 256; i++) {
        buffer[i] = inw(ATA_PRIMARY_IO_BASE);
    }

    uint32_t sector_count = buffer[60] | (buffer[61] << 16);

    return sector_count;
}

void SimpleOS::ata_write_to_sector(uint32_t lba, const string& buffer) {
    outb(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO_BASE + 2, 1);
    outb(ATA_PRIMARY_IO_BASE + 3, (uint8_t)lba);
    outb(ATA_PRIMARY_IO_BASE + 4, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO_BASE + 5, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_WRITE);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        outw(ATA_PRIMARY_IO_BASE, ((uint16_t*)buffer.c_str())[i]);
    }

    ata_wait_bsy();
}

bool SimpleOS::ata_can_write_to_sector(uint32_t lba, size_t buffer_size) {
    string data;
    ata_read_sector(lba, data);

    size_t data_length = strnlen(data.c_str(), 512);
    size_t space_left = 512 - data_length;

    return buffer_size <= space_left;
}

bool SimpleOS::ata_append_to_sector(uint32_t lba, const string& buffer) {
    string data;
    ata_read_sector(lba, data);

    size_t data_length = strnlen(data.c_str(), 512);
    size_t space_left = 512 - data_length;
    size_t buffer_size = buffer.size();

    if (buffer_size > space_left) {
        return false;
    }

    memcpy((char*)data + data_length, buffer.c_str(), buffer_size);
    ata_write_to_sector(lba, data);

    return true;
}

bool SimpleOS::ata_empty_sector(uint32_t lba) {
    return ata_get_free_space_in_sector(lba) == 512;
}

void SimpleOS::ata_read_sector(uint32_t lba, string& buffer) {
    outb(ATA_PRIMARY_IO_BASE + 6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(ATA_PRIMARY_IO_BASE + 2, 1);
    outb(ATA_PRIMARY_IO_BASE + 3, (uint8_t)lba);
    outb(ATA_PRIMARY_IO_BASE + 4, (uint8_t)(lba >> 8));
    outb(ATA_PRIMARY_IO_BASE + 5, (uint8_t)(lba >> 16));
    outb(ATA_PRIMARY_IO_BASE + 7, ATA_COMMAND_READ);

    ata_wait_bsy();
    ata_wait_drq();

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buffer.c_str())[i] = inw(ATA_PRIMARY_IO_BASE);
    }

    ata_wait_bsy();
}

void SimpleOS::ata_delete_from_sector(uint32_t lba) {
    char empty_data[512];

    memset(empty_data, 0, 512);

    ata_write_to_sector(lba, empty_data);
}

size_t SimpleOS::ata_get_free_space_in_sector(uint32_t lba) {
    string data;
    ata_read_sector(lba, data);

    size_t free_bytes = 0;
    for (int i = 0; i < 512; i++) {
        if (data[i] == 0) {
            free_bytes++;
        }
    }

    return free_bytes;
}