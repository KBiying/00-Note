## linux主机：

### 安装node.js\(安装xpackopenocd 的前提)

安装教程：https://xpack.github.io/install/

### 安装 xpackopenocd 

地址：https://github.com/xpack-dev-tools/openocd-xpack

安装教程：https://xpack.github.io/dev-tools/openocd/install/

最后添加软链，使用openocd即可启动程序

` sudo ln -s ~/.local/xPacks/@xpack-dev-tools/openocd/0.12.0-1.1/.content/bin/openocd /usr/local/bin/openocd`

### 连接引脚

### 使用`sudo` 启动`openocd`

` sudo openocd -f jlink.cfg`

`jlink.cfg`内容如下：

```
# 
# SEGGER J-Link
#
# http://www.segger.com/jlink.html
#

adapter driver jlink
transport select jtag

# The serial number can be used to select a specific device in case more than
# one is connected to the host.
#
# Example: Select J-Link with serial number 123456789
#
# jlink serial 123456789
```

###  

### 树莓派设置jtag模式

注意使用sudo执行 

```c
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define BCM2835_PERI_BASE 0x3f000000
#define GPIO_BASE (BCM2835_PERI_BASE + 0x200000)

#define GPIO_ALT_FUNCTION_4 0b011
#define GPIO_ALT_FUNCTION_5 0b010

volatile unsigned *m_pAltFunctionRegisters;

void SetGPIOFunction(int GPIO, int functionCode)
{
	int registerIndex = GPIO / 10;
	int bit = (GPIO % 10) * 3;

	unsigned oldValue = m_pAltFunctionRegisters[registerIndex];
	unsigned mask = 0b111 << bit;
	printf("Changing function of GPIO%d from %x to %x\n", GPIO, (oldValue >> bit) & 0b111, functionCode);
	m_pAltFunctionRegisters[registerIndex] = (oldValue & ~mask) | ((functionCode << bit) & mask);
}

int main()
{
	int fd;

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0)
		return -1;

	m_pAltFunctionRegisters = (volatile unsigned *)mmap(NULL, 0x18, PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE);
	if (m_pAltFunctionRegisters == MAP_FAILED)
		return -1;

	close(fd);

	if (m_pAltFunctionRegisters == MAP_FAILED)
	{
		printf("Cannot open /dev/mem! Re-run me as root!\n");
		return 1;
	}

	SetGPIOFunction(22, GPIO_ALT_FUNCTION_4);
	SetGPIOFunction(4, GPIO_ALT_FUNCTION_5);
	SetGPIOFunction(27, GPIO_ALT_FUNCTION_4);
	SetGPIOFunction(25, GPIO_ALT_FUNCTION_4);
	SetGPIOFunction(23, GPIO_ALT_FUNCTION_4);
	SetGPIOFunction(24, GPIO_ALT_FUNCTION_4);

	if (m_pAltFunctionRegisters != MAP_FAILED)
		munmap((void *)m_pAltFunctionRegisters, 0x18);

	printf("Successfully enabled JTAG pins. You can start debugging now.\n");

	return 0;
}

```



我的文件存储路径

opencd 配置文件

cd ~/.local/share/Trash/files/04-debug/runningkernel

gdb-multiarch /home/bailey/vmlinux 

remote 

树莓派开机后，再插上jlink









