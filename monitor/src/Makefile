

TEMP_LINK_PATH_TO_BW_BMC_USB_LIB="/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/"

TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB="/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/"

BIN_FILE="../bin/monitor"

all: $(BIN_FILE)
monitor.o: monitor.c
	gcc -c monitor.c -o monitor.o

$(BIN_FILE): monitor.o
	mkdir ../bin -p
	gcc -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb monitor.o -o $(BIN_FILE) -Wall -Werror

clean:
	rm monitor.o $(BIN_FILE)