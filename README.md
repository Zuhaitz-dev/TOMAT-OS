# TOMAT-OS
Attempt of Operating System (C++) 🍅

![CC0 License](https://img.shields.io/badge/License-CC0-blue.svg)
![GitHub Repo Size](https://img.shields.io/github/repo-size/Zuhaitz-dev/TOMAT-OS)
![GitHub Last Commit](https://img.shields.io/github/last-commit/Zuhaitz-dev/TOMAT-OS)

Do not expect greatness. I am learning, and that is amazing. I am using [this great tutorial](https://www.youtube.com/watch?v=1rnA6wpF0o4&list=PLHh55M_Kq4OApWScZyPl5HhgsTJS9MZ6M).

---

## Features

✅ For now it did not explode. A great success.

---

## How It Works

- The kernel is compiled using a cross-compiler targeting i686-elf.  
- It runs on QEMU for easy testing without rebooting your actual system.  
- GRUB is used as the bootloader to load the kernel.  
- The goal is to expand and experiment, adding new features along the way.  

---

## Installation & Usage

TOMAT-OS requires a GCC cross-compiler, GRUB for booting, and QEMU for testing.  
Clone the repository, compile the kernel, and run it in an emulator to see it in action. 

```bash
# Clone the repository
git clone https://github.com/Zuhaitz-dev/TOMAT-OS.git
cd TOMAT-OS

# Compile the kernel
make

# Run in QEMU
make run
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
