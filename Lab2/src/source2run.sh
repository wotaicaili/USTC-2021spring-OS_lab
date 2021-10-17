#!/bin/sh
SRC_RT=$(shell pwd)
echo $SRC_RT

make clean

make

if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	qemu-system-i386 -kernel output/myOS.elf -serial stdio
fi
