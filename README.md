```bash
████████╗ ██████╗ ███╗   ███╗ █████╗ ████████╗         ██████╗  ███████╗
╚══██╔══╝██╔═══██╗████╗ ████║██╔══██╗╚══██╔══╝        ██╔═══██╗ ██╔════╝
   ██║   ██║   ██║██╔████╔██║███████║   ██║    ███╗   ██║   ██║ ███████╗
   ██║   ██║   ██║██║╚██╔╝██║██╔══██║   ██║    ╚══╝   ██║   ██║ ╚════██║
   ██║   ╚██████╔╝██║ ╚═╝ ██║██║  ██║   ██║           ╚██████╔╝ ███████║
   ╚═╝    ╚═════╝ ╚═╝     ╚═╝╚═╝  ╚═╝   ╚═╝            ╚═════╝  ╚══════╝
```
---

![CC0 License](https://img.shields.io/badge/License-CC0-blue.svg)
![GitHub Repo Size](https://img.shields.io/github/repo-size/Zuhaitz-dev/TOMAT-OS)
![GitHub Last Commit](https://img.shields.io/github/last-commit/Zuhaitz-dev/TOMAT-OS)


***Attempt of Operating System (C++) 🍅***

Do not expect greatness. I am learning, and that is amazing. I am using [this great tutorial](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M).

---

## Features

✅ For now it did not explode. A great success.

---

## How It Works

- The kernel is compiled using a cross-compiler targeting i686-elf.  
- It runs on QEMU and VirtualBox for easy testing without rebooting your actual system.  
- GRUB is used as the bootloader to load the kernel.
- The goal is to expand and experiment, adding new features along the way.  

---

## Installation

TOMAT-OS requires GCC for cross-compilation, Binutils for assembling, libc6-dev-i386 for 32-bit development, GRUB for booting, and xorriso for ISO creation. QEMU and VirtualBox are recommended for testing.

### Debian/Ubuntu
```bash
sudo apt update
sudo apt install -y g++ binutils libc6-dev-i386 grub-pc xorriso
```

### Fedora
```bash
sudo dnf install -y gcc-c++ binutils glibc-devel.i686 grub2 xorriso
```

### Arch Linux
```bash
sudo pacman -Sy --needed gcc binutils lib32-glibc grub xorriso
```

### Windows (MSYS2)
```bash
pacman -Sy --needed mingw-w64-x86_64-gcc mingw-w64-x86_64-binutils
```

*(GRUB and xorriso are not natively available on Windows—consider using WSL or a Linux VM.)*

To download QEMU, [click here](https://www.qemu.org/download/). To download VirtualBox, [click here](https://www.virtualbox.org/wiki/Downloads).

---

## Usage

Clone the repository, compile the kernel, and run it in an emulator to see it in action. 

```bash
# Clone the repository
git clone https://github.com/Zuhaitz-dev/TOMAT-OS.git
cd TOMAT-OS

# Compile the kernel (.bin)
make

# Run ISO image in VirtualBox (TextMode)
make run-text

# Run ISO image in VirtualBox (GraphicsMode)
make run-gfx

# Run ISO image in QEMU (TextMode)
make run-qemu-text

# Run ISO image in QEMU (GraphicsMode)
make run-qemu-gfx

# Clean build files
make clean
```

> **NOTE:** text mode in QEMU presents some bugs, related to the keyboard drivers. For now it is recommended to work with *VirtualBox* mainly.

---

## FAQ

> **Q:** Can I update the repository without cloning it again?

> **A:** Of course! To do that, follow these commands

```bash
# If you are not in the folder, get in there
cd /path/to/TOMAT-OS

# For now there's only a main, but if there were other branches, change it to the name of that branch
git checkout main

# Download the last changes
git pull origin main 
```

---

## Useful Commands

### Debian/Ubuntu

- *lspci* -> displays info about system PCI devices.
```bash
lspci

# Show numeric ID's
lspci -n

# Show hex-dump of the standard part of the config space
lspci -x
```

- *nm* -> lists network connections on the system.
```bash

# This was needed to make the interruptstubs.s
nm obj/hardwarecommunication/interrupts.o
```


---

## Contributing

Got ideas? Want to make TOMAT-OS even more tomato-ish? Feel free to contribute!  

1. Fork the repository.  
2. Create a new branch.
```bash
git checkout -b feature/your-feature-name
```    
3. Make your changes.  
4. Commit and push your changes.  
```bash
git commit -m "Add your message here"
git push origin feature/your-feature-name
```    
5. Open a pull request.  

---

## License

This project is licensed under the **CC0 1.0 Universal License**. See the [LICENSE](LICENSE) file for details.  

---

## Acknowledgments

- **The tutorial that got me started** – [Click here](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M)  
- **Tomatoes** – For being an inspiration   

---

## Contact

- **GitHub**: [Zuhaitz-dev](https://github.com/Zuhaitz-dev)  
- **Email**: zuhaitz.zechhub@gmail.com  

---

Stay tomatuned! 🍅
