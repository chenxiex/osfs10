#!/bin/bash
if [[ "$1" == "run" ]]; then
    qemu-system-i386 -m 32M -drive format=raw,if=floppy,file=a.img -drive format=raw,if=ide,file=80m.img -boot a
elif [[ "$1" == "debug" ]]; then
    qemu-system-i386 -m 32M -drive format=raw,if=floppy,file=a.img -drive format=raw,if=ide,file=80m.img -boot a -s -S
else
    echo "Usage: $0 [run|debug]"
fi