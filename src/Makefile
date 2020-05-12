TEMP_LINK_PATH_TO_BW_BMC_USB_LIB=/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/
TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB=/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/

# Specify the target file and the install directory
OUTPUTFILELIB=libStratixMonitor.a
BINARYDIR=../binaries
INCLUDEDIR=../include

BIN_FILE=${BINARYDIR}/main

all: main.cpp libStratixMonitor.so
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} main.cpp -L${BINARYDIR} -lStratixMonitor -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm $(BINARYDIR)/${BINFILE} $(BINARYDIR)/*.o $(BINARYDIR)/*.so

# libStratixMonitor.a: StratixMonitor.o FPGAEnergyCounters.o
# 	mkdir -p $(BINARYDIR)
# 	ar rcs $(BINARYDIR)/libStratixMonitor.a $(BINARYDIR)/FPGAEnergyCounters.o $(BINARYDIR)/StratixMonitor.o

# FPGAEnergyCounters.o: ${INCLUDEDIR}/FPGAEnergyCounters.h FPGAEnergyCounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -c FPGAEnergyCounters.cpp -o ${BINARYDIR}/FPGAEnergyCounters.o
#
# StratixMonitor.o: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGAEnergyCounters.h FPGAEnergyCounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -c StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11

# FPGAEnergyCounters.so: ${INCLUDEDIR}/FPGAEnergyCounters.h FPGAEnergyCounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -fpic -c FPGAEnergyCounters.cpp -o ${BINARYDIR}/FPGAEnergyCounters.o
# 	g++  -shared libFPGACounters.so ${BINARYDIR}/FPGAEnergyCounters.o

libStratixMonitor.so: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGAEnergyCounters.h FPGAEnergyCounters.cpp
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} -fpic -c StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11
	g++ -I${INCLUDEDIR} -fpic -c FPGAEnergyCounters.cpp -o ${BINARYDIR}/FPGAEnergyCounters.o -std=c++11
	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -shared -o ${BINARYDIR}/libStratixMonitor.so ${BINARYDIR}/StratixMonitor.o ${BINARYDIR}/FPGAEnergyCounters.o -std=c++11

install:
	mv ${BINARYDIR}/libStratixMonitor.so /usr/local/lib/libStratixMonitor.so