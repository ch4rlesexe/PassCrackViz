# 🔐 Password Cracker (C++)

This is a C++ console-based password cracker tool that supports **Brute Force** and **Dictionary Attack** methods. It allows you to input a hashed password (e.g., MD5) and attempts to crack it by comparing it against generated or listed potential matches. This is meant to help people that are learning visualize how such a tool works.

Here’s your updated **📁 Project Structure** section based on your actual directory layout and use of CMake:

## 📁 Project Structure

```
├── build/                         # CMake build artifacts and compiled binary
│   └── PassCrackViz               # Final compiled executable
├── src/                           # Source code
│   ├── main.cpp                   # Program entry point and menu interface
│   ├── BruteForceCracker.{h,cpp} # Brute-force password guessing
│   ├── DictionaryCracker.{h,cpp} # Wordlist-based password guessing
│   ├── HashUtils.{h,cpp}         # Hashing and comparison utilities
│   └── UIManager.{h,cpp}         # Handles user I/O and menus
├── wordlist.txt                  # Dictionary file for cracking passwords
├── smoke.cpp                     # Quick test script for validating hash cracking
├── smoke_test                    # Compiled binary for smoke testing
├── CMakeLists.txt                # CMake build configuration
```
---

## 🧠 Features

- 🔍 Crack passwords using:
  - Brute Force (every possible character combination)
  - Dictionary Attack (compare against a file of common passwords)
- 💬 User-friendly terminal interface
- 🔑 MD5 hash comparison (can be extended)

---

## 🚀 Getting Started

### Prerequisites

- A C++ compiler (`g++`)
- C++17 or higher recommended

### 🔧 Build Instructions

Open your terminal and run:

```bash
mkdir -p build
cd build
cmake ..
make
````

This compiles the program into an executable called `cracker`.

---

## 🖥️ Usage

Run the executable:

```bash
./cracker
```

You’ll be prompted to:

1. Choose between Brute Force or Dictionary Attack
2. Enter a hashed password (e.g., `098f6bcd4621d373cade4e832627b4f6` for "test")
3. (If Dictionary) Provide a dictionary file path

---

## 🧪 Example Hash

To test:

* Use the MD5 of the password "test":
  `098f6bcd4621d373cade4e832627b4f6`

* Try cracking it with a dictionary file that includes "password", or brute-force it.

---

## 📚 Notes

* Brute force is **slow** for long or complex passwords.
* Dictionary cracking depends on the quality of your wordlist (e.g., `wordlist.txt`).
* Currently supports MD5; easy to extend to SHA256 or others using a hash library.

---

## 🛡️ Disclaimer

This tool is intended for **educational** and **ethical** purposes only. Do not use it against systems or hashes you do not own or have permission to test.

---
