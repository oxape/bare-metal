set confirm off
set architecture arm:arm32
target remote 127.0.0.1:1234
symbol-file kernel.bin
set disassemble-next-line auto
