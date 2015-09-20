symbol-file ./vm.elf
target remote | openocd -f /usr/share/openocd/scripts/board/st_nucleo_f4.cfg -c "gdb_port pipe; log_output /dev/null"
monitor reset halt
monitor reset init
monitor flash write_image erase vm.hex
echo \nUse "continue" to start the program\n
