TEMP_LINK_PATH_TO_BW_BMC_USB_LIB="/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/"
TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB="/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/"

# Specify the target file and the install directory
OUTPUTFILELIB  = libStratixMonitor.a
MAINFILE = main
INSTALLDIR  = ../binaries

BIN_FILE="./main"

all: $(BIN_FILE)

$(BIN_FILE): main.cpp
    mkdir -p $(INSTALLDIR)
	g++ -I../include main.cpp -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm $(INSTALLDIR)/main $(INSTALLDIR)/libStratixMonitor.a


StratixMonitor: StratixMonitor.h StratixMonitor.cpp FPGACounters.h FPGACounters.cpp
    mkdir -p $(INSTALLDIR)
    g++  -I../include -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -c StratixMonitor.cpp FPGACounters.cpp -o ${OUTPUTFILELIB}