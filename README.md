# WAL Viewer (ncurses-based)

A terminal-based **Write-Ahead Log (WAL) viewer** built in **C** using **ncurses**, providing an interactive interface for exploring WAL records in both **text** and **binary** modes.

This project is designed as a **low-level systems tool** to help understand how WAL records are structured and navigated, with an emphasis on **performance, clarity, and terminal UX**.

---

## Features

* Interactive **TUI (Terminal User Interface)** using `ncurses`
* Scrollable list of WAL records
* Two display modes:

  * **Text mode** (human-readable descriptions)
  * **Binary mode** (hexadecimal representation)
* Record details popup
* Incremental search by record type or description
* Keyboard-driven navigation
* Clean separation between:

  * Parsing logic
  * UI logic
  * Search functionality

---

## Screenshots

> ![Text Mode](<Screenshot from 2025-12-28 12-23-49.png>)
> ![Binary Mode](<Screenshot from 2025-12-28 12-23-54.png>)
> ![Search Mode](<Screenshot from 2025-12-28 12-24-26.png>)
---

## Controls

| Key     | Action                |
| ------- | --------------------- |
| ↑ / ↓   | Navigate records      |
| `T`     | Switch to text mode   |
| `B`     | Switch to binary mode |
| `Enter` | Show record details   |
| `S`     | Search records        |
| `Q`     | Quit                  |

---

## Project Structure

```
wal-viewer/
├── include/
│   ├── search.h
│   ├── ui.h
│   └── utils.h
│   └── wal_parser.h
│
├── src/
│   ├── main.c
│   ├── search.c
│   ├── ui.c
│   ├── utils.c
│   └── wal_parser.c
│
├── build/
│
├── Makefile
└── README.md
```

---

## Architecture Overview

### 1. WAL Parsing Layer

Responsible for:

* Reading WAL data
* Extracting records
* Normalizing fields (LSN, type, length, description, binary)

This layer is **UI-independent**.

---

### 2. UI Layer (ncurses)

Manages:

* Window layout (header, main view, footer)
* Navigation state
* Display mode switching
* Record selection and scrolling

Key data structure:

```c
typedef struct {
    wal_data_t* data;
    int current_row;
    int scroll_offset;
    display_mode_t mode;
    WINDOW* main_win;
    WINDOW* header_win;
    WINDOW* footer_win;
    char* filename;
} ui_state_t;
```

---

### 3. Search Module

* Interactive search window
* Wrap-around search
* Auto-adjusts scroll position
* Matches against:

  * Record type
  * Record description

---

## Building the Project

### Requirements

* GCC or Clang
* `ncurses` development library

On Ubuntu / Debian:

```bash
sudo apt install libncurses-dev
```

---

### Build

```bash
make
```

This produces the executable inside the `build/` directory.

---

## Running

```bash
./build/wal-viewer <wal_file>
```

Example:

```bash
./build/wal-viewer sample.wal
```

---

## Design Goals

* Minimal dependencies
* Clear separation of concerns
* Keyboard-first UX
* Suitable for:

  * Database internals exploration
  * Systems programming practice
  * WAL / logging education
  * ncurses-based UI reference

---

## Limitations

* WAL format is simplified / project-specific
* No editing or write-back support
* Large WAL files may require optimization (future work)

---

## Possible Improvements

* Paging large WAL files
* Color themes
* Filtering by record type
* Export selected records
* Support for real PostgreSQL WAL decoding
* Mouse support (optional)

---

## Why ncurses?

* Portable
* Lightweight
* Ideal for low-level tooling
* Matches the workflow of system developers and DB engineers

---