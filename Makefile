TEMP_LINK_PATH_TO_BW_BMC_USB_LIB=/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/
TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB=/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/

# Specify the target file and the install directory
OUTPUTFILELIB=libStratixMonitor.a
BINARYDIR=./binaries
INCLUDEDIR=./include
SRCDIR=./src

BIN_FILE=${BINARYDIR}/main

all: ${SRCDIR}/main.cpp libStratixMonitor.so
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} ${SRCDIR}/main.cpp -L${BINARYDIR} -lStratixMonitor -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm $(BINARYDIR)/${BINFILE} $(BINARYDIR)/*.o $(BINARYDIR)/*.so

# libStratixMonitor.a: StratixMonitor.o FPGAEnergyCounterState.o
# 	mkdir -p $(BINARYDIR)
# 	ar rcs $(BINARYDIR)/libStratixMonitor.a $(BINARYDIR)/FPGAEnergyCounterState.o $(BINARYDIR)/StratixMonitor.o

# FPGAEnergyCounterState.o: ${INCLUDEDIR}/FPGAEnergyCounterState.h FPGAEnergyCounterState.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -c FPGAEnergyCounterState.cpp -o ${BINARYDIR}/FPGAEnergyCounterState.o
#
# StratixMonitor.o: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGAEnergyCounterState.h FPGAEnergyCounterState.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -c StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11

# FPGAEnergyCounterState.so: ${INCLUDEDIR}/FPGAEnergyCounterState.h FPGAEnergyCounterState.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -fpic -c FPGAEnergyCounterState.cpp -o ${BINARYDIR}/FPGAEnergyCounterState.o
# 	g++  -shared libFPGACounters.so ${BINARYDIR}/FPGAEnergyCounterState.o

libStratixMonitor.so: ${INCLUDEDIR}/SensorIds.h ${INCLUDEDIR}/StratixMonitor.h ${SRCDIR}/StratixMonitor.cpp ${INCLUDEDIR}/FPGACounters.h ${SRCDIR}/FPGACounters.cpp
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} -I${TEMP_LINK_PATH_TO_BW_BMC_USB_LIB} -I${TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB} -fpic -c ${SRCDIR}/StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11
	g++ -I${INCLUDEDIR} -fpic -c ${SRCDIR}/FPGACounters.cpp -o ${BINARYDIR}/FPGACounters.o -std=c++11
	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -shared -o ${BINARYDIR}/libStratixMonitor.so ${BINARYDIR}/StratixMonitor.o ${BINARYDIR}/FPGACounters.o -std=c++11

install:
	mv ${BINARYDIR}/libStratixMonitor.so /usr/local/lib/libStratixMonitor.so