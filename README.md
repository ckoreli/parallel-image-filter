# Parallel Image Filter
The goal of this program is to implement an image filter using a 3x3 filter kernel. The filter program shall 
distribute the workload over a specifyable number of processes in order to parallelize some of the required work.
## Interface Specification
The program provides the following command-line interface
```sh
imfilter [ -p NUMBER_OF_PROCESSES ] -k KERNEL -i INPUT_IMAGE -o OUTPUT_IMAGE [-h]
```
* `-p` specifies the number of processes as a decimal number. If it is not provided, the program shall use the number obtained by `get_nprocs()`.
* `-k` specifies the filter kernel. This shall be a string of 9 `double` numbers specifying the coefficients of the filter matrix separated by commas.
* if `-h` is provided, the program shall provide information about how to execute and exit.
Example execution:
```sh
imfilter -p 4 -k "0,-1,0,-1,4,-1,0,-1,0" -i input.ppm -o output.ppm
```
### The program works exclusively on P6 ppm images.
