TEMP_LINK_PATH_TO_BW_BMC_USB_LIB=/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/
TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB=/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/

# Specify the target file and the install directory
OUTPUTFILELIB=libStratixMonitor.a
MAINFILE=main
BINARYDIR=../binaries
INCLUDEDIR=../include

BIN_FILE="./main"

all: $(BIN_FILE)

$(BIN_FILE): main.cpp libStratixMonitor.a
	mkdir -p $(INSTALLDIR)
	g++ -I../include main.cpp ${BINARYDIR}/libStratixMonitor.a -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm $(BINARYDIR)/${BINFILE} $(BINARYDIR)/*.o


libStratixMonitor.a: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
	mkdir -p $(BINARYDIR)
	ar -cvq $(BINARYDIR)/libStratixMonitor.a $(BINARYDIR)/*.o


FPGACounters.o: ${INCLUDEDIR}/FPGACounters.h FPGACounters.cpp
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR}/ -c FPGACounters.cpp -o ${BINARYDIR}/FPGACounters.o

StratixMonitor.o: ${INCLUDEDIR}/StratixMonitor.h StratixMonitor.cpp
	mkdir -p $(BINARYDIR)
	g++ -I${INCLUDEDIR} -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -c FPGACounters.cpp -o ${BINARYDIR}/StratixMonitor.o