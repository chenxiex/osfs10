#!/bin/bash
dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
LOOPDEV=$(udisksctl loop-setup -f a.img | grep -o '/dev/loop[0-9]*')
MOUNTPOINT=$(udisksctl mount -b ${LOOPDEV} | grep -o "/run/media/$USER/[a-zA-Z0-9]*")
cp -fv boot/loader.bin ${MOUNTPOINT}
cp -fv kernel.bin ${MOUNTPOINT}
udisksctl unmount -b ${LOOPDEV}
udisksctl loop-delete -b ${LOOPDEV}