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

	new_file.size = strlen(data);

	ssize_t freesector = free_sector();

	if (freesector == _INVALID_SECTOR) {
		Terminal::lnprint("No free space");
		return false;
	}

	new_file.sectors.push(freesector);

	if (data) {
		distr_to_sectors(new_file, data);
	}

	for (size_t i = 0; i < new_file.sectors.size(); i++) {
		Terminal::lnprint((int)i);
		Terminal::print(" sector: ");
		Terminal::print((int)new_file.sectors[i]);
	}
	Terminal::new_line();
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
	
	if (!__check_exist(name)) return false;

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

bool FileSystem::write_to_file(const char* name, const char* data) {

	if (!delete_file(name)) {
		return false;
	}

	return create_file(name, data);
}

bool FileSystem::append_to_file(const char* name, const char* data) {

	if (!__check_exist(name)) return false;

	return distr_to_sectors(files[name], data);
}

bool FileSystem::read_file(const char* name, string& buffer) {

	if (!__check_exist(name)) return false;

	File file = files[name];
	size_t size = file.sectors.size();
	string temp;

	for (size_t i = 0; i < size; i++) {
		char* data = (char*)malloc(512);
		ata_read_sector(file.sectors[i], data);
		temp = temp + data;
		free(data);
	}

	Terminal::lnprint("size: ");
	Terminal::println((int)file.size);

	buffer = temp;
	free(buffer);
	buffer = (char*)malloc(temp.size() + 1);
	strcpy(buffer, temp.c_str());

	return true;
}

bool FileSystem::format() {
	bool has_error = false;
	files.forEach([&has_error](const string& key, File) {
		if (!delete_file(key.c_str()))
			has_error = true;
		});
	return !has_error;
}

bool FileSystem::exist(const char* name) {
	return files.has(name);
}

ssize_t FileSystem::free_sector(size_t size) {

	for (uint32_t i = 0, sector_count = ata_get_sector_count(); i < sector_count; i++) {
        if (!taken_sectors.has(i) && ata_can_write_to_sector(i, size)) {
            taken_sectors.push(i);
            return i;
        }
    }
    return _INVALID_SECTOR;
}

ssize_t FileSystem::free_sector() {

	for (uint32_t i = 0, sector_count = ata_get_sector_count(); i < sector_count; i++) {
		if (!taken_sectors.has(i)) {
			taken_sectors.push(i);
			return i;
		}
	}
	return _INVALID_SECTOR;
}

bool FileSystem::distr_to_sectors(File& file, const char* data) {
	size_t free_space = ata_get_free_space_in_sector(file.sectors.back());
	if (free_space) {
		char buffer[free_space];

		strncpy(buffer, data, free_space - 1);

		buffer[free_space - 1] = '\0';

		ata_append_to_sector(file.sectors.back(), buffer);

		data += free_space;
	}


	//Terminal::lnprint("begin");
	while (strlen(data) > 0) {

		ssize_t freesector = free_sector();
		if (freesector == _INVALID_SECTOR) {
			Terminal::lnprint("No free space");
			return false;
		}

		/*Terminal::lnprint("freesector: ");
		Terminal::print((int)freesector);*/

		file.sectors.push(freesector);

		char buffer[512];
		size_t data_to_write = min(strlen(data), size_t(512));
		memcpy(buffer, data, data_to_write);

		if (data_to_write < 512) {
			buffer[data_to_write] = '\0';
		}
		else {
			buffer[511] = '\0';
		}

		/*Terminal::lnprint("Data to write: ");
		Terminal::print(buffer);*/
		ata_write_to_sector(freesector, buffer);

		data += data_to_write;
	}
	return true;
}

bool FileSystem::__check_exist(const char* name) {
	if (!files.has(name)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}
	return true;
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