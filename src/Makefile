TEMP_LINK_PATH_TO_BW_BMC_USB_LIB=/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/
TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB=/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/

# Specify the target file and the install directory
OUTPUTFILELIB=libStratixMonitor.a
BINARYDIR=../binaries
INCLUDEDIR=../include

BIN_FILE=${BINARYDIR}/main

all: main.cpp libStratixMonitor.a
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} main.cpp -L${BINARYDIR} -lStratixMonitor -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm $(BINARYDIR)/${BINFILE} $(BINARYDIR)/*.o

# libStratixMonitor.a: StratixMonitor.o FPGACounters.o
# 	mkdir -p $(BINARYDIR)
# 	ar rcs $(BINARYDIR)/libStratixMonitor.a $(BINARYDIR)/FPGACounters.o $(BINARYDIR)/StratixMonitor.o

# FPGACounters.o: ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -c FPGACounters.cpp -o ${BINARYDIR}/FPGACounters.o
#
# StratixMonitor.o: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -c StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11

# FPGACounters.so: ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
# 	mkdir -p $(BINARYDIR)
# 	g++ -I${INCLUDEDIR} -fpic -c FPGACounters.cpp -o ${BINARYDIR}/FPGACounters.o
# 	g++  -shared libFPGACounters.so ${BINARYDIR}/FPGACounters.o

StratixMonitor.so: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} -fpic -c StratixMonitor.cpp -o ${BINARYDIR}/StratixMonitor.o -std=c++11
	g++ -I${INCLUDEDIR} -fpic -c FPGACounters.cpp -o ${BINARYDIR}/FPGACounters.o -std=c++11
	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -shared libStratixMonitor.so ${BINARYDIR}/StratixMonitor.o ${BINARYDIR}/FPGACounters.o -std=c++11
