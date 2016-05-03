# sizeof-linux
A cli-tool for GNU/Linux making it easy to find files bigger than the specified threshold.
## Installation
To install the tool, simply run make in the directory of the source files.
```
$ make
```
## Usage

```
$ ./sizeof [-rtv] [-p path] [-s size_format] [-h threshhold]
```
size_format : {(b)ytes,(k)ilobytes,(m)egabytes,(g)igabytes}
threshold : if in verbose

## Known Issues/Bugs
If the tool is not run as root it occasionally produces **segfaults** when trying to read files it doesn't have permission to.
This is most likely caused by the function ```getFileSize(char*)```, which opens every file to determine its size.
