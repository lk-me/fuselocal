# localfuse - File system implemented on Linux using C and libfuse
***Use pure C to have a simple file system of your own on the linux system***

## Install and build

> sudo apt install libfuse-dev

> mkdir build && cd build

> cmake ..

> make install

## Usage
background operation
> ./localfuse $MOUNT_POINT

foreground operation
> ./localfuse -f $MOUNT_POINT

## Function

1. Multi-level file or folder creation folder
2. Rename folder or file
3. Delete folder or file

## Functions to be implemented
1. Input and output of file content
2. Obtain record of file creation time
3. Log record


