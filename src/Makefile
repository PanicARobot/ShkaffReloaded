DOWNLOADS=./downloads

CMSIS=${DOWNLOADS}/CMSIS
TOOLCHAIN=${DOWNLOADS}/gcc-arm-none-eabi-4.8.3-2014q1

CORES_PATH=./ArduinoCore-samd/cores/arduino
VARIANTS_PATH=./ArduinoCore-samd/variants/mkr1000
LIBRARIES_PATH=./ArduinoCore-samd/libraries

BOSSAC=./BOSSA/bin/bossac
PORT=ttyACM0
PORT_DEVICE=/dev/${PORT}
SERIAL_BAUD=115200

CC=${TOOLCHAIN}/bin/arm-none-eabi-gcc
CXX=${TOOLCHAIN}/bin/arm-none-eabi-g++
AR=${TOOLCHAIN}/bin/arm-none-eabi-ar
OBJCOPY=${TOOLCHAIN}/bin/arm-none-eabi-objcopy

BUILD_DIR=build

TARGET_ELF=${BUILD_DIR}/result.elf
TARGET_BIN=${BUILD_DIR}/result.bin

INCLUDE_DIRS=\
	${CMSIS}/CMSIS/Include/ \
	${CMSIS}/Device/ATMEL/ \
	${CORES_PATH} \
	${VARIANTS_PATH} \
	${LIBRARIES_PATH}/Wire/ \
	${LIBRARIES_PATH}/SPI/


INCLUDE_FLAGS=$(addprefix -I,${INCLUDE_DIRS})

DEFINES=-DMICROS_PER_SECOND=1000000 \
	-DF_CPU=48000000L -DARDUINO=10611 -DARDUINO_SAMD_MKR1000 -DARDUINO_ARCH_SAMD  -D__SAMD21G18A__ -DUSB_VID=0x2341 -DUSB_PID=0x804e -DUSBCON '-DUSB_MANUFACTURER="Arduino LLC"' '-DUSB_PRODUCT="Arduino MKR1000"'

OPTIMISE_FLAGS=-mcpu=cortex-m0plus -mthumb -Os
WARNING_FLAGS=-Wall -Wextra
DEBUG_FLAGS=-g
COMMON_FLAGS=${OPTIMISE_FLAGS} ${DEBUG_FLAGS} ${WARNING_FLAGS} -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 ${DEFINES} ${INCLUDE_FLAGS} -MMD -MP

ASMFLAGS=${DEBUG_FLAGS} -x assembler-with-cpp ${DEFINES} ${INCLUDE_FLAGS}
CFLAGS=-std=gnu11 ${COMMON_FLAGS} -Wstrict-prototypes
CXXFLAGS=-std=gnu++11 ${COMMON_FLAGS} -fno-threadsafe-statics -fno-rtti -fno-exceptions
LDFLAGS=${OPTIMISE_FLAGS} -Wl,--gc-sections -save-temps "-T${VARIANTS_PATH}/linker_scripts/gcc/flash_with_bootloader.ld" --specs=nano.specs --specs=nosys.specs -Wl,--check-sections -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align # -Wl,--cref


SRC_DIRS=\
	./ShkaffReloaded/ \
	${CORES_PATH} \
	${VARIANTS_PATH} \
	${LIBRARIES_PATH}/Wire/ \
	${LIBRARIES_PATH}/SPI/

SRC_FILES=$(shell find ${SRC_DIRS} -name \*.S -or -name \*.c -or -name \*.cpp)

OBJ_FILES=$(SRC_FILES:%=${BUILD_DIR}/%.o)
DEPENDENCIES=${OBJ_FILES:.o=.d}


all: link
compile: ${TOOLCHAIN} ${CMSIS} ${OBJ_FILES}
link: compile ${TARGET_ELF}
bin: link ${TARGET_BIN}

${TARGET_ELF}: ${OBJ_FILES}
	${CC} ${LDFLAGS} -Wl,--start-group -lm $^ -Wl,--end-group -o $@

${TARGET_BIN}: ${TARGET_ELF} ${TOOLCHAIN}
	${OBJCOPY} -O binary $< $@


${BUILD_DIR}/%.S.o: %.S
	mkdir -p $(dir $@)
	${CC} ${ASMFLAGS} -c $< -o $@

${BUILD_DIR}/%.c.o: %.c
	mkdir -p $(dir $@)
	${CC} ${CFLAGS} -c $< -o $@

${BUILD_DIR}/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	${CXX} ${CXXFLAGS} -c $< -o $@


-include ${DEPENDENCIES}


${BOSSAC}:
	make -C ./BOSSA strip-bossac

program: ${TARGET_BIN} ${BOSSAC}
	while [[ ! -c ${PORT_DEVICE} ]]; do sleep .1; done
	stty -F ${PORT_DEVICE} 1200
	sleep .5
	while [[ ! -c ${PORT_DEVICE} ]]; do sleep .1; done
	${BOSSAC} -i -d --port=${PORT} -U true -i -e -w -v $< -R


${CMSIS}:
	mkdir -p ${DOWNLOADS}
	curl -s http://downloads.arduino.cc/CMSIS-4.0.0.tar.bz2 | tar xjf - -C ${DOWNLOADS}
${TOOLCHAIN}:
	mkdir -p ${DOWNLOADS}
	curl -s http://downloads.arduino.cc/gcc-arm-none-eabi-4.8.3-2014q1-linux64.tar.gz | tar xzf - -C ${DOWNLOADS}


clean:
	rm -rf ${BUILD_DIR}

clean_tools:
	make -C ./BOSSA clean

clean_downloads:
	rm -rf ${DOWNLOADS}

.PHONY: all, program, serial, clean, clean_tools, clean_downloads, compile, link, bin
