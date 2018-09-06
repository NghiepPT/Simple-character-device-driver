# Simple-character-device-driver
## An example of a linux character device driver

We will create a simple C program as below with name is device_char.c. When loading into the kernel, it will read/write to a buffer and print to log.

## Compiling linux kernel module
In order to compile a linux kernel module, we will create a make file as below:

obj-m:=device_char.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean


## Loading and unloading a linux kernel module
Now we can insert the module to test it. To do this, run:

$  sudo insmod device_char.ko 

To remove the module, run:

$  sudo rmmod device_char

For detail, you can access to link:
http://sandan.live/index.php/2018/09/06/linux-kernelhow-to-make-a-simple-character-device-drivers/
