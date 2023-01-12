SET(SOURCE_FILE_DIR ${PROJECT_SOURCE_DIR}/kernel)

ADD_CUSTOM_TARGET(
        kernel ALL
        COMMAND i686-elf-as -g ${SOURCE_FILE_DIR}/boot.asm -o boot.o
        COMMAND i686-elf-gcc -g -c ${SOURCE_FILE_DIR}/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
        COMMAND i686-elf-gcc -g -T ${SOURCE_FILE_DIR}/linker.ld -o FabOS.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
        COMMAND mv FabOS.bin ${PROJECT_SOURCE_DIR}/
)