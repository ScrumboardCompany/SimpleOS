#include "fs/fs.h"
#include "libs/string/string.h"
#include "libs/string/class.h"
#include "fs/disk.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

map<string, FileSystem::File> FileSystem::files;
vector<size_t> FileSystem::taken_sectors;

void FileSystem::init_fs() {
	if (!read_superblock()) {
		Superblock block;

		write_superblock(block);
	}
	else {
		Superblock block;

		read_superblock(&block);

		files = block.files;
		taken_sectors = block.taken_sectors;
	}
	/*if (!read_superblock()) {
		Terminal::lnprint("Superblock error, creating new one");

		superblock.total_sectors = ata_get_sector_count();
		superblock.used_sectors = 0;
		superblock.file_count = 0;
		superblock.file_table_start = 1;

		write_superblock();
	}

	return true;*/
}

bool FileSystem::create_file(const char* name, const char* data) {
	if (files.has(name)) {
		Terminal::lnprint(string(name) + " already exist");
		return false;
	}

	File new_file;

	if (data) {
		new_file.size = strlen(data);

		ssize_t freesector = free_sector(new_file.size);
		if (freesector == -1) {
			Terminal::lnprint("No free space");
			return false;
		}

		ata_write_to_sector(freesector, data);
		new_file.sectors.push(freesector);

		superblock.used_sectors++;
	}

	files.insert(name, new_file);

	superblock.file_count++;

	write_superblock();

	return true;
}

bool FileSystem::delete_file(const char* name) {
	if (!files.has(name)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}

	File file = files[name];

	for (size_t i = 0, size = file.sectors.size(); i < size; ++i) {
		ata_delete_from_sector(file.sectors[i]);

		if (taken_sectors.has(file.sectors[i]))
			taken_sectors.pop(taken_sectors.find(file.sectors[i]));

		superblock.used_sectors--;
	}
	files.erase(name);

	superblock.file_count--;

	write_superblock();

	return true;
}

bool FileSystem::read_file(const char* name, char* buffer) {
	if (!files.has(name)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}

	File file = files[name];
	size_t size = file.sectors.size();
	string temp;

	for (size_t i = 0; i < size; i++) {
		char* data = (char*)malloc(512);
		ata_read_sector(file.sectors[i], data);
		temp = temp + data;
		free(data);
	}

	free(buffer);
	buffer = (char*)malloc(temp.size() + 1);
	strcpy(buffer, temp.c_str());

	return true;
}

ssize_t FileSystem::free_sector(size_t size) {

	for (uint32_t i = 1, sector_count = ata_get_sector_count(); i < sector_count; i++) {
		if (!taken_sectors.has(i) && ata_can_write_to_sector(i, size)) {
			taken_sectors.push(i);
			return i;
		}
	}
	return -1;
}

bool FileSystem::read_superblock(Superblock* block) {

	if ()

	char buffer[512];
	ata_read_sector(0, buffer);

	block = reinterpret_cast<Superblock*>(&buffer);

	return true;
	/*char buffer[512];
	ata_read_sector(0, buffer); 

	memcpy(&superblock, buffer, sizeof(Superblock));

	if (superblock.total_sectors == 0 || superblock.file_count == 0) {
		return false; 
	}
	return true;*/
}

bool FileSystem::write_superblock(Superblock& block) {
	char* data = reinterpret_cast<char*>(&block);

	ata_write_to_sector(0, data);
	return true;
	/*char buffer[512]; 

	memcpy(buffer, &superblock, sizeof(Superblock));

	ata_write_to_sector(0, buffer);
	return true;*/
}