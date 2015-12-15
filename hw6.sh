adb shell rm -rf /data/misc/hmwk6

dd if=/dev/zero of=hmwk6.fs bs=1M count=2
sudo losetup /dev/loop0 hmwk6.fs
sudo ./userspace/e2fsprogs/misc/mke2fs -I 256 -t ext4 -L w4118.hmwk6 /dev/loop0
sudo losetup -d /dev/loop0

adb shell ln -s /dev/block/loop0 /dev/loop0
adb shell ln -s /dev/block/loop1 /dev/loop1
adb shell ln -s /dev/block/loop2 /dev/loop2
adb shell ln -s /dev/block/loop3 /dev/loop3
adb shell ln -s /dev/block/loop4 /dev/loop4
adb shell ln -s /dev/block/loop5 /dev/loop5
adb shell ln -s /dev/block/loop6 /dev/loop6
adb shell ln -s /dev/block/loop7 /dev/loop7

adb push hmwk6.fs /data/misc
adb shell mkdir /data/misc/hmwk6

adb shell mount -o loop, gps_aware_inode -t ext4 /data/misc/hmwk6.fs /data/misc/hmwk6
adb shell mount -o loop -t ext4 /data/misc/hmwk6.fs /data/misc/hmwk6