# FTP Client-Server in C++ (Active Mode, Multithreaded)

This project implements a basic FTP server and client in C++ supporting active mode (using the `PORT` command), file transfer, directory listing, and multithreading using a custom thread pool.

## Features

- 🔐 User authentication (`USER`, `PASS`)
- 📁 Directory navigation (`PWD`, `MKD`, `CWD`)
- 📤 File upload (`STOR`)
- 📥 File download (`RETR`)
- 📄 Directory listing (`LIST`)
- 🔌 Active mode data connection (`PORT`)
- 🧵 Multithreaded server using a custom `ThreadPool`

