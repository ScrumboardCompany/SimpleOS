#include "fs/fs.h"
#include "libs/string/string.h"
#include "fs/disk.h"

using namespace SimpleOS;

vector<FileSystem::File> FileSystem::files;
vector<size_t> FileSystem::taken_sectors;

bool FileSystem::create_file(const char* name, const char* data) {
	File new_file;

	strcpy(new_file.name,name);

	if (data) {
		new_file.size = strlen(data);

		ssize_t freesector = free_sector(new_file.size);
		if (freesector == -1) return false;

		new_file.sectors.push(freesector);
	}
	files.push(new_file);

	return true;
}

bool FileSystem::delete_file(const char* name) {
	
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