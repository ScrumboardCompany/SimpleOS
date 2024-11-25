<div align="center">
    <h1>SimpleOS</h1>
    <p>A 32-bit terminal-based operating system with a basic filesystem</p>
</div>

## How to launch on VM
For testing, we've been using QEMU.\
```qemu-system-i386 -hda "path to disk image" -cdrom "path to SimpleOS.iso" -boot d -m 1024```\
You can also try it on VirtualBox.

## Known Issues and Planned Fixes
- Memory leaks
- Scrolling(likely caused by the memory leaks)
- The help command (currently affected by scrolling issues, so we recommend using this README for command information)
- Text mode

## How to avoid potentional bugs?
- If you enter commands that produce 25 or more lines of output, use the clear command to ensure the console displays content correctly.

## Commands
[] - required arguments\
() - optional arguments
- ```color [text/bg] [number]``` - Changes the console text or background color
- ```clear``` - Clears all text from the console
- ```reset``` - Clears the console and resets all settings (e.g., color)
- ```mkfile [path] (data)``` - Creates a file at the specified path, optionally with initial data
- ```cpfile [source path] [destination path]``` - Copies a file from the source path to the destination path
- ```rdfile [path]``` - Reads and displays the data of the file at the specified path
- ```rmfile [path]``` - Deletes the file at the specified path
- ```wrfile [path] [data]``` - Overwrites the file at the specified path with new data
- ```apfile [path] [file]``` - Appends new data to the file at the specified path
- ```open [path]``` - Opens the file at the specified path in text mode (BETA)
- ```mkdir [path]``` - Creates a directory at the specified path
- ```cpdir [source path] [destination path]``` - Copies a directory and its contents to the destination path
- ```exdir [path]``` - Lists the contents of the directory at the specified path
- ```cd [path]``` - Changes the current directory to the specified path
- ```dir``` - Lists the contents of the current directory
- ```tree``` - Displays the directory structure in a tree-like format starting from the current directory
- ```path``` - Displays the path of the current directory
- ```format``` - Deletes all files and directories in the system
- ```time``` - prints the current world time
- ```cat [number]``` - Displays a cat ASCII art based on the specified number

## Other features
- You can select text using Shift + arrow keys, then copy, paste, or delete the highlighted text.
- You can navigate through recent commands using the Up and Down arrow keys and paste them.