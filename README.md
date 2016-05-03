# sizeof-linux
A cli-tool for GNU/Linux making it easy to find files bigger than the specified threshold.
## Installation
To install the tool, simply run make in the directory of the source files.
```
$ make
```
## Usage

```
$ ./sizeof [-rtv] [-p path] [-s size_format] [-h threshold]
```
|switch|argument|description|
|:----:|:------:|:---------:|
|**-r**|no|operate recursively|
|**-v**|no|verbose output|
|**-t**|no|time the operation|
|**-s**|*size_format*|values in *b* -> bytes, *k* -> kilobytes, *m* -> megabytes, *g* -> gigabytes (default: *k*)|
|**-p**|*path*|set the search path|
|**-n**|no|colorless output (for piping etc.)|
|**-h**|*threshold*|search for files >= *threshold*|

### Example
```
$ ./sizeof -t -p ./ -s k -h 20  
```
###### output:
```
<./>
  |-- sizeof  [62.071999]
  |-- LICENSE  [35.140999]

--------------------------------------

[SEARCH TIME] : 0s
[PRINT TIME] : 0s
[TOTAL ELAPSED TIME] : 0s
```
##### Note
>  1. *threshold* is a float. It is given in the specified size_format.
>  2. The output file size is a float. It is given in the specified size_format.

## Known Issues/Bugs
If the tool is not run as root it occasionally produces **segfaults** when trying to read files it doesn't have permission to.
This is most likely caused by the function ```getFileSize(char*)```, which opens every file to determine its size.
