TEMP_LINK_PATH_TO_BW_BMC_USB_LIB="/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/"

TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB="/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/"

BIN_FILE="./main"

all: $(BIN_FILE)

$(BIN_FILE): main.cpp
	g++ -L$(TEMP_LINK_PATH_TO_BW_BMC_USB_LIB) -lbw_bmc_usb -L$(TEMP_LINK_PATH_TO_BW_MCTP_PLDM_LIB) -lbw_mctp_pldm -lusb -lm -I../include main.cpp -o $(BIN_FILE) -Wall -Werror -std=c++11

clean:
	rm main