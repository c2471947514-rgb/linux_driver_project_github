# Linux Character Device Driver

一个简单的 Linux 字符设备驱动程序项目，用于学习 BSP/Linux 驱动程序
---

# Features

- Linux 内核模块

- 字符设备驱动程序

- 读写支持

- 用户内核通信

- 自动创建 /dev 节点
---

# Project Structure

```text
linux_driver_project_github/
├── driver/
│   ├── chrdevbase.c
│   └── Makefile
│
├── app/
│   └── chrdevbaseAPP.c
│
├── docs/
│
├── screenshots/
│
└── README.md
```

---

# Build Driver

```bash
cd driver
make
```

---

# Load Driver

```bash
sudo insmod chrdevbase.ko
```

---

# Check Device

```bash
ls /dev/chrdevbase
```

---

# Build APP

```bash
cd app
gcc chrdevbaseAPP.c -o app
```

---

# Test Read

```bash
sudo ./app /dev/chrdevbase 1
```

---

# Test Write

```bash
sudo ./app /dev/chrdevbase 2
```

---

# Kernel Log

```bash
sudo dmesg | tail
```

---

# Communication Flow

APP -> system call -> VFS -> driver -> kernel buffer

---

# Environment

- Ubuntu 24.04
- Linux Kernel 6.x
- GCC 13

---

# Future Work

- ioctl
- cdev
- GPIO driver
- platform driver
- device tree

---

# Author

Cerole