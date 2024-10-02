#include "fs/fs.h"
#include "libs/string/string.h"
#include "libs/string/class.h"
#include "fs/disk.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

map<string, FileSystem::File> FileSystem::files;
vector<size_t> FileSystem::taken_sectors;

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
	}

	//Terminal::lnprint((int)new_file.sectors.size());

	files.insert(name, new_file);

	/*Terminal::lnprint("New file: ");
	File& file = files[name];
	Terminal::lnprint((int)file.size);
	Terminal::lnprint("file.sectors size: ");
	Terminal::print((int)file.sectors.size());

	for (size_t i = 0; i < file.sectors.size(); ++i) {
		Terminal::lnprint((int)i);
		Terminal::print(" el of file.sectors: ");
		Terminal::print((int)file.sectors[i]);
	}*/

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
	}
	files.erase(name);

	return true;
}

bool FileSystem::read_file(const char* name, char* buffer) {
	if (!files.has(name)) {
		Terminal::lnprint("File don`t exists");
		return false;
	}
	
	/*files.forEach([](const string& key, File file) {
		Terminal::lnprint(key);
		Terminal::lnprint((int)file.size);
		for (size_t i = 0; i < file.sectors.size(); i++) {
			Terminal::lnprint((int)file.sectors[i]);
		}
		});*/

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

	/*Terminal::lnprint("temp: ");
	Terminal::print(buffer);
	Terminal::lnprint("buffer: ");
	Terminal::print(buffer);*/

	return true;
}

ssize_t FileSystem::free_sector(size_t size) {

	for (uint32_t i = 0, sector_count = ata_get_sector_count(); i < sector_count; i++) {
		if (!taken_sectors.has(i) && ata_can_write_to_sector(i, size)) {
			taken_sectors.push(i);
			return i;
		}
	}
	return -1;
}