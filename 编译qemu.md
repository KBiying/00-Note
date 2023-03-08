sudo apt install ninja-build libpixman-1-dev

# 

sudo systemctl start systemd-network



sudo qemu-system-aarch64 -M raspi3b -append "rw earlyprintk loglevel=8 console=ttyAMA0,115200 dwc_otg.lpm_enable=0 root=/dev/mmcblk0p2 rootdelay=1" -dtb bcm2710-rpi-3-b-plus.dtb -  
sd 2020-02-13-raspbian-buster.img -kernel kernel8.img -m 1G -smp 4 -serial stdio -usb -device usb-mouse -device usb-kbd -device usb-host,vendorid=0x0cf3,productid=0x9271 -net nic -n  
et tap,ifname=tap0,script=no,downscript=no -netdev tap, id=virbr0
