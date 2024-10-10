#include "fs/fs.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

bool FileSystem::create_dir(const string& name) {

	if (current_directory->directories.has(name)) {
		Terminal::lnprint(name + " already exist");
		return false;
	}

	Directory new_dir;
	new_dir.parent = current_directory;
	current_directory->directories.insert(name, new_dir);

	return true;
}

bool FileSystem::delete_dir(const string& name) {

	if (!__check_dir_exist(name)) return false;

	current_directory->directories.erase(name);

	return true;
}

bool FileSystem::cd(const string& name) {
	if (string(name) == "..") {
		return cd_up();
	}

	if (!__check_dir_exist(name)) return false;

	current_directory = &current_directory->directories[name];

	current_path.push(name);

	return true;
}

bool FileSystem::cd_up() {
	if (current_directory->parent) {
		current_directory = current_directory->parent;
		current_path.pop();

		return true;
	}
	Terminal::lnprint("Already at root directory");
	return false;
}

void FileSystem::tree(const Directory& dir, ssize_t level) {
	//Terminal::lnprint(get_current_path());
	print_tree(dir, level);
	//print_tree(dir, level);
}

void FileSystem::print_tree(const Directory& dir, ssize_t level) {
	string indent = string(level * 2, ' ');

	dir.files.forEach([&](const string& name, const FileSystem::File&) {
		Terminal::lnprint(indent + "|-- " + name);
		});

	dir.directories.forEach([&](const string& name, const FileSystem::Directory& sub_dir) {
		Terminal::lnprint(indent + "|-- " + name + "/");

		print_tree(sub_dir, level + 1);
		});

}

string FileSystem::get_current_path() {

	string path;
	for (size_t i = 0; i < current_path.size(); i++) {
		path = path + current_path[i] + "/";
	}

	return path;
}

FileSystem::Directory* FileSystem::get_current_directory() {
	return current_directory;
}

FileSystem::Directory FileSystem::get_root() {
	return root;
}