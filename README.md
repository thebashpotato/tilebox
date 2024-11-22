# Tilebox

[![standard-readme compliant](https://img.shields.io/badge/standard--readme-OK-green.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)

> A box (collection) of Xlib powered software that I wrote for myself and use on a daily basis for the sole purpose of further enjoying my computing experience.

## Table of Contents

- [Background](#background)
- [Setup](#install)
- [Docs](#docs)
- [Maintainers](#maintainers)
- [License](#license)

## Background

A Suckless inspired tiling window manager environment

## Setup



### Install dependencies

<details>
<summary>Arch Linux</summary>
<br>
<code>
    pacman -S base-devel libx11 libxft xorg-server-xephyr clang cmake ninja --noconfirm
</code>
</details>

<details>
<summary>Ubuntu Linux</summary>
<br>
<code>
    apt-get install build-essential libx11-dev libxft-dev xserver-xephyr clang clangd clang-format cmake ninja-build -y
</code>
</details>


### Prepare Environment

Copy the example env file, and uncomment the build type you want with the compiler you want (Clang and GCC supported).
The file defaults to a release build using clang

```bash
cp -rv .env.example .env
```


### Build


```bash
# List all available make commands
make list

# Build the source code
make compile

# If you want to run the window manager in a nested xephyr session.
# (test the window manager without logging out)
make embed

# If you selected the debug build in the .env file you can run the tests.
make tests
```

## Docs

In-depth documentation for each source module can be found in the following markdown files in the *docs*
folder.

1. [tilebox-core](./docs/tilebox-core.md) Core library which wraps and provides a modern C++ RAII interface to the original `Xlib` library from 1987. And a low level drawing built on top of `FontConfig` and `XftFont`.

2. [window-manager](./docs/window-manager.md) A non-reparenting tiling window mangager


## Maintainers

[@thebashpotato](https://github.com/thebashpotato)

## License

[MIT](./LICENSE) © 2024 Matt Williams
