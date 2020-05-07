#! /bin/bash

pushd $(dirname $0)

# Add current path to library loading path to test libbw_bmc_usb.so
export LD_LIBRARY_PATH="../../bw_bmc_usb_lib/;../../bw_mctp_pldm_lib/"

# Run the monitor utility

./bin/monitor $*

popd


# EOF - run_monitor.sh