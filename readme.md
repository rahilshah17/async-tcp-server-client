# TCP Socket Project

This project consists of an asynchronous TCP server and client for transferring files using Boost.Asio in C++.

## Table of Contents

- [Introduction](#introduction)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [References](#references)

## Introduction

This project implements an asynchronous TCP server and client using the Boost.Asio library. The server accepts incoming connections and sends a specified file to the connected client. The client connects to the server, receives the file, and saves it locally.

## Requirements

- C++11 or later
- Boost library
- pthread library

## Installation

1. **Clone the repository:**

   ```sh
   git clone https://github.com/yourusername/tcpsocket.git
   cd tcpsocket
   ```
2. **Install Boost**
    ```sh
    sudo apt-get install libboost-all-dev
    ```
## Usage

1. **Compile the server and the client**
    ```sh
    g++ -o server server.cpp -lboost_system -lpthread
    g++ -o client client.cpp -lboost_system -lpthread
    ```
2. **Server**
    ```sh
    ./server <file_path>
    ```
3. **Client**
    ```sh
    ./client <host> <port> <output_file>
    ```
## References

- [Boost.Asio Overview](https://www.boost.org/doc/libs/1_85_0/doc/html/boost_asio/overview.html): This project utilizes Boost.Asio, a cross-platform C++ library for network and low-level I/O programming. For more information on Boost.Asio, please refer to the official documentation.
