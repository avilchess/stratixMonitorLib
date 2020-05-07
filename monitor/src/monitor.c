
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "usb.h"

/* Temp relative path hack. */
#include "/usr/share/bittware/520nmx/cots/bw_bmc_usb_lib/bw_bmc_usb_lib.h"
#include "/usr/share/bittware/520nmx/cots/bw_mctp_pldm_lib/bw_mctp_pldm_lib.h"

int8_t bmcUsbWriteFunction(uint8_t* buffer, uint16_t* length);
int8_t bmcUsbReadFunction(uint8_t* buffer, uint16_t* length);

usb_dev_handle* usbHandle = NULL;

char* sensorOperationalStateName(uint8_t operationalState);
char* effecterOperationalStateName(uint8_t operationalState);
void listAll(BW_MCTP_PLDM_HANDLE mctpPldmHandle);
void displayNumericSensor(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t sensorId);
void displayStateSensor(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t sensorId);
void displayNumericEffecter(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t effecterId);
void displayStateEffecter(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t effecterId);



/* Long names of valid command line options. */
static struct option longOptions[] =
{
   {"id",        required_argument, 0, 'i'},
   {"name",      required_argument, 0, 'n'},
   {"value",     required_argument, 0, 'v'},
   {"disable",   no_argument,       0, 'd'},
   {"enable",    no_argument,       0, 'e'},
   {"effecter",  no_argument,       0, 'E'},
   {"sensor",    no_argument,       0, 's'},
   {"list",      no_argument,       0, 'l'},
   {"help",      no_argument,       0, 'h'},
   {0, 0, 0, 0}
};

#define OPTION_STRING  "hi:ldesEn:v:"

#define HELP_STRING                                                           \
"Usage: %s [OPTION] ...\n"                                                    \
"Controls PLDM sensors and effecters.\n"                                      \
"\n"                                                                          \
"Mandatory arguments to long options are mandatory for short options too.\n"  \
"  -i, --id ID_NUMBER       Specify sensor/effecter by its ID instead of\n"   \
"                           name.\n"                                          \
"  -n, --name NAME          Specify sensor/effecter by its name.\n"           \
"  -d, --disable            Disable the specified sensor/effecter.\n"         \
"  -e, --enable             Disable the specified sensor/effecter.\n"         \
"  -s, --sensor             Have -i,--id option target an sensor.\n"          \
"                              This is the default.\n"                        \
"  -E, --effecter           Have -i,--id option target an effecte.\n"         \
"  -l, --list               List Sensors available.\n"                        \
"  -v, --value VALUE        Set specified effector to given value.\n"         \
"  -u, --usb                Look for BMC via USB.\n"                          \
"                              This is the default.\n"                        \
"Exit status is 0 if OK, 1 if minor problems, 2 on failure.\n"                \
"\n"                                                                          \
"Report bugs to <support@bittware.com>\n"


int main(int argc, char** argv)
{
   int listAllFlag = 0;
   int disableFlag = 0;
   int enableFlag = 0;

   int effecterFlag = 0;
   int sensorFlag = 0;

   int valueSpecified = 0;

   char nameBuffer[100];

   char valueStringBuffer[100];

   BW_MCTP_PLDM_HANDLE mctpPldmHandle;

   uint16_t specifiedId = 0;
   uint32_t bmcVersion;


   nameBuffer[0] = 0;

   /* If we've been given no arguments display the help message and exit. */
   if (argc == 1)
   {
      /* Display the help string. */
      printf(HELP_STRING, argv[0]);

      /* Exit successfully, so user can read the help message. */
      exit(0);
   }


   /* Interpret and apply each of the given command line arguments. */
   while (1)
   {
      /* Will hold the type of the option given. */
      int option;

      /* Will hold the value of the option given. */
      int optionValue;

      /* getopt_long() stores the option index here. */
      int optionIndex = 0;

      /* Parse the command line for the next option. */
      option = getopt_long(argc, argv, OPTION_STRING, longOptions,
                           &optionIndex);

      /* If there are no more options we can proceed with the test. */
      if (option == -1)
         break;

      /* Apply the option to the test settings structure, if it is valid. */
      switch (option)
      {
         /* -i, --id option (integer).
            Sets the ID number of target sensor. */
         case 'i':
            /* Read id argument. */
            sscanf(optarg, "%i", &optionValue);

            specifiedId = optionValue;
            
            break;

         /* -l, --list (no argument).
            List all sensors */
         case 'l':
            listAllFlag = 1;
            break;

         /* -d, --disable (no argument).
            Disable specified sensor. */
         case 'd':
            disableFlag = 1;
            break;

         /* -e, --enable (no argument).
            Enable specified sensor. */
         case 'e':
            enableFlag = 1;
            break;

         /* -E, --effecter (no argument).
            Target an effecter rather than sensor. */
         case 'E':
            effecterFlag = 1;
            break;

         /* -s, --sensor (no argument).
            Target an sensor rather than effecter. */
         case 's':
            sensorFlag = 1;
            break;

         /* -n, --name (string).
            Name of target sensor/effecter. */
         case 'n':
            if (strlen(optarg) != 0)
               sscanf(optarg, "%s", nameBuffer);
            break;

         /* -v, --value (string).
            Value to set target effecter to.  We take the argument as a string
            at first, even though it might be a float, because we don't know yet
            if the target effecter is a state or numeric effecter. */
         case 'v':
            if (strlen(optarg) != 0)
               sscanf(optarg, "%s", valueStringBuffer);

            valueSpecified = 1;
            break;

         /* -h, --help (no argument).
            Displays a help text and exits. */
         case 'h':
            /* Display the help string. */
            printf(HELP_STRING, argv[0]);

            /* Exit successfully, so user can read the help message. */
            exit(0);

         /* Unrecognised option character. */
         case '?':
            /* If we get here getopt_long() will have already printed an error
               message, so we'll just exit unsuccessfully, given that the user
               input was invalid. */
            exit(1);

         /* Catch other options here. */
         default:
            /* Long name options with no short equivalent (like --loopback) will
               bring us to this point, we let these cases pass silently. */
            break;
      }
   }

   //printf("specifiedId = %d\n", specifiedId);

   if (effecterFlag && sensorFlag)
   {
      printf("Can only target a --sensor OR an --effecter, not both!\n");
      printf("See --help for more details.\n");

      exit(1);
   }

   if (!sensorFlag && !effecterFlag)
      sensorFlag = 1;

   /* Get a USB connection to the BMC. */
   usbHandle = bw_bmc_usb_open();

   if (usbHandle == NULL)
   {
      printf("Failed to open USB interface to BMC!\n");
      return -1;
   }

   /* Open the BwMctpPldm library, we pass it two functions for reading and
      writing to USB so that it can communicate with the BMC. */
   mctpPldmHandle = BwMctpPldm_initialise(bmcUsbWriteFunction, bmcUsbReadFunction);

   if (mctpPldmHandle == NULL)
   {
      printf("Failed to initialise MCTP/PLDM Library!\n");
      return -1;
   }

   /* Display BMC version. */
   BwMctpPldm_oemBmcVersion(mctpPldmHandle, &bmcVersion);

   printf("BMC Version: %d\n", bmcVersion);

   if (strlen(nameBuffer) != 0)
   {
      uint8_t searchId;
      int foundName = 0;

      if (BwMctpPldm_getEffecterIdFromName(mctpPldmHandle, &searchId, nameBuffer) == BW_MCTP_PLDM_SUCCESS)
      {
         specifiedId  = searchId;
         sensorFlag   = 1;
         effecterFlag = 0;
         foundName    = 1;
      }

      if (BwMctpPldm_getSensorIdFromName(mctpPldmHandle, &searchId, nameBuffer) == BW_MCTP_PLDM_SUCCESS)
      {
         specifiedId  = searchId;
         sensorFlag   = 1;
         effecterFlag = 0;
         foundName    = 1;
      }

      if (!foundName)
      {
         printf("Couldn't find any sensor or effecters named \"%s\".\n", nameBuffer);

         exit(1);
      }
   }

   /* If user has used the -d/--disable option we'll try to disable what ever
      sensor/effecter has been specified by the user. */
   if (disableFlag)
   {
      if (sensorFlag)
      {
         if (BwMctpPldm_validNumericSensorId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setNumericSensorOperationalStateById(mctpPldmHandle,
                                                            BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_DISABLED,
                                                            specifiedId);
         }
      
         if (BwMctpPldm_validStateSensorId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setStateSensorOperationalStateById(mctpPldmHandle,
                                                          BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_DISABLED,
                                                          specifiedId);
         }
      }

      if (effecterFlag)
      {
         if (BwMctpPldm_validNumericEffecterId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setNumericEffecterOperationalStateById(mctpPldmHandle,
                                                              BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_DISABLED,
                                                              specifiedId);

         }
      
         if (BwMctpPldm_validStateEffecterId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setStateEffecterOperationalStateById(mctpPldmHandle,
                                                            BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_DISABLED,
                                                            specifiedId);
         }
      }
   }

   /* If user has used the -e/--enable option we'll try to enable what ever
      sensor/effecter has been specified by the user. */
   if (enableFlag)
   {
      if (sensorFlag)
      {
         if (BwMctpPldm_validNumericSensorId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setNumericSensorOperationalStateById(mctpPldmHandle,
                                                            BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_ENABLED,
                                                            specifiedId);
         }

         if (BwMctpPldm_validStateSensorId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setStateSensorOperationalStateById(mctpPldmHandle,
                                                          BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_ENABLED,
                                                          specifiedId);
         }
      }


      if (effecterFlag)
      {
         if (BwMctpPldm_validNumericEffecterId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setNumericEffecterOperationalStateById(mctpPldmHandle,
                                                              BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_NO_UPDATE_PENDING,
                                                              specifiedId);
         }
      
         if (BwMctpPldm_validStateEffecterId(mctpPldmHandle, specifiedId))
         {
            BwMctpPldm_setStateEffecterOperationalStateById(mctpPldmHandle,
                                                            BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_NO_UPDATE_PENDING,
                                                            specifiedId);
         }
      }
   }

   /* If user has used the -v/--value option in conjunction with -E/--effecter
      we'll try to update the effecter with the value specified. */
   if (effecterFlag && valueSpecified)
   {
      /* If the effecter if a numeric effecter we'll interpret the user input
         string, then use the numeric effecter value setter function. */
      if (BwMctpPldm_validNumericEffecterId(mctpPldmHandle, specifiedId))
      {
         int8_t result;
         float floatValue;

         sscanf(valueStringBuffer, "%f", &floatValue);

         result = BwMctpPldm_setNumericEffecterValueById(mctpPldmHandle,
                                                         floatValue,
                                                         specifiedId);

         switch (result)
         {
            default:
            case BW_MCTP_PLDM_SUCCESS: 
               break;

            case BW_MCTP_PLDM_INVALID_PARAMETER:
               printf("Invalid parameter for effecter.\n");
               break;
         }

         if (result)
            printf("BwMctpPldm_setNumericEffecterValueById() = %d\n", result);


      }

      /* If the effecter is a state effecter, we use the state effecter value
         setter function.  */
      if (BwMctpPldm_validStateEffecterId(mctpPldmHandle, specifiedId))
      {
         int8_t result;

         result = BwMctpPldm_setStateEffecterValueNameById(mctpPldmHandle,
                                                           valueStringBuffer,
                                                           specifiedId);
         if (result)
            printf("BwMctpPldm_setStateEffecterValueNameById() = %d\n", result);
      }
   }

   /* If user has specified --list, dump out all sensor/effecter values. */
   if (listAllFlag)
      listAll(mctpPldmHandle);

   if (!listAllFlag)
   {
      if (sensorFlag)
         if (specifiedId != 0)
         {
            if (BwMctpPldm_validNumericSensorId(mctpPldmHandle, specifiedId))
               displayNumericSensor(mctpPldmHandle, specifiedId);

            if (BwMctpPldm_validStateSensorId(mctpPldmHandle, specifiedId))
               displayStateSensor(mctpPldmHandle, specifiedId);
         }

      if (effecterFlag)
         if (specifiedId != 0)
         {
            if (BwMctpPldm_validNumericEffecterId(mctpPldmHandle, specifiedId))
               displayNumericEffecter(mctpPldmHandle, specifiedId);

            if (BwMctpPldm_validStateEffecterId(mctpPldmHandle, specifiedId))
               displayStateEffecter(mctpPldmHandle, specifiedId);
         }    
   }

   BwMctpPldm_close(mctpPldmHandle);

   usb_close(usbHandle);

   return 0;
}


void displayNumericSensor(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t sensorId)
{
   float reading;
   uint8_t operationalState;

   char nameBuffer[100];
   char unitsBuffer[100];
   char stateNameBuffer[100];

   BwMctpPldm_getNumericSensorOperationalStateById(mctpPldmHandle,
                                                   &operationalState,
                                                   sensorId);

   BwMctpPldm_getSensorName(mctpPldmHandle, sensorId, nameBuffer);
   BwMctpPldm_getNumericSensorUnitsById(mctpPldmHandle, sensorId, unitsBuffer);
   BwMctpPldm_getNumericSensorReadingById(mctpPldmHandle, &reading, sensorId);

   if (operationalState == BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_ENABLED)
      printf("% 4d: %s = %.2f %s\n", sensorId, nameBuffer, reading,
             unitsBuffer);
   else
      printf("% 4d: %s <numeric sensor %s>\n", sensorId, nameBuffer,
             sensorOperationalStateName(operationalState));

}


void displayStateSensor(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t sensorId)
{
   float reading;
   uint8_t operationalState;

   char nameBuffer[100];
   char unitsBuffer[100];
   char stateNameBuffer[100];

   BwMctpPldm_getStateSensorOperationalStateById(mctpPldmHandle,
                                                 &operationalState,
                                                 sensorId);

   BwMctpPldm_getSensorName(mctpPldmHandle, sensorId, nameBuffer);
   BwMctpPldm_getStateSensorValueNameById(mctpPldmHandle, stateNameBuffer,
                                          sensorId);

   if (operationalState == BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_ENABLED)
      printf("% 4d: %s = %s\n", sensorId, nameBuffer, stateNameBuffer);
   else
      printf("% 4d: %s <state sensor %s>\n", sensorId, nameBuffer,
             sensorOperationalStateName(operationalState));
}

void displayNumericEffecter(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t effecterId)
{
   float reading;
   uint8_t operationalState;

   char nameBuffer[100];
   char unitsBuffer[100];
   char stateNameBuffer[100];

   BwMctpPldm_getNumericEffecterOperationalStateById(mctpPldmHandle,
                                                     &operationalState,
                                                     effecterId);


   BwMctpPldm_getEffecterName(mctpPldmHandle, effecterId, nameBuffer);
   BwMctpPldm_getNumericEffecterUnitsById(mctpPldmHandle, effecterId, unitsBuffer);
   BwMctpPldm_getNumericEffecterValueById(mctpPldmHandle, &reading, effecterId);
         
   if (operationalState == BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_NO_UPDATE_PENDING)
      printf("% 4d: %s = %.2f %s\n", effecterId, nameBuffer, reading, unitsBuffer);
   else
      printf("% 4d: %s <numeric effecter %s>\n", effecterId, nameBuffer,
             effecterOperationalStateName(operationalState));
}

void displayStateEffecter(BW_MCTP_PLDM_HANDLE mctpPldmHandle, uint16_t effecterId)
{
   float reading;
   uint8_t operationalState;

   char nameBuffer[100];
   char unitsBuffer[100];
   char stateNameBuffer[100];

   BwMctpPldm_getStateEffecterOperationalStateById(mctpPldmHandle,
                                                   &operationalState,
                                                   effecterId);

   BwMctpPldm_getEffecterName(mctpPldmHandle, effecterId, nameBuffer);
   BwMctpPldm_getStateEffecterValueNameById(mctpPldmHandle, stateNameBuffer,
                                            effecterId);
         
   if (operationalState == BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_NO_UPDATE_PENDING)
      printf("% 4d: %s = %s\n", effecterId, nameBuffer, stateNameBuffer);
   else
      printf("% 4d: %s <state effecter %s>\n", effecterId, nameBuffer,
             effecterOperationalStateName(operationalState));
}


void listAll(BW_MCTP_PLDM_HANDLE mctpPldmHandle)
{
   uint8_t sensorCount;
   uint8_t sensorId;
   uint8_t effecterCount;
   uint8_t effecterId;

   float reading;
   uint8_t operationalState;

   char nameBuffer[100];
   char unitsBuffer[100];
   char stateNameBuffer[100];

   {
      BwMctpPldm_getNumericSensorCount(mctpPldmHandle, &sensorCount);

      printf("Numeric Sensors (%d):\n", sensorCount);

      sensorId = 0;
      BwMctpPldm_nextNumericSensorId(mctpPldmHandle, &sensorId);

      while (sensorId != 0)
      {
         displayNumericSensor(mctpPldmHandle, sensorId);

         BwMctpPldm_nextNumericSensorId(mctpPldmHandle, &sensorId);
      }

      BwMctpPldm_getStateSensorCount(mctpPldmHandle, &sensorCount);

      printf("State Sensors (%d):\n", sensorCount);

      sensorId = 0;
      BwMctpPldm_nextStateSensorId(mctpPldmHandle, &sensorId);

      while (sensorId != 0)
      {
         displayStateSensor(mctpPldmHandle, sensorId);

         BwMctpPldm_nextStateSensorId(mctpPldmHandle, &sensorId);
      }


      BwMctpPldm_getNumericEffecterCount(mctpPldmHandle, &effecterCount);
      
      printf("Numeric Effecters (%d):\n", effecterCount);


      effecterId = 0;
      BwMctpPldm_nextNumericEffecterId(mctpPldmHandle, &effecterId);

      while (effecterId != 0)
      {
         displayNumericEffecter(mctpPldmHandle, effecterId);

         BwMctpPldm_nextNumericEffecterId(mctpPldmHandle, &effecterId);
      }


      BwMctpPldm_getStateEffecterCount(mctpPldmHandle, &effecterCount);
      
      printf("State Effecters (%d):\n", effecterCount);

      effecterId = 0;
      BwMctpPldm_nextStateEffecterId(mctpPldmHandle, &effecterId);

      while (effecterId != 0)
      {
         displayStateEffecter(mctpPldmHandle, effecterId);
         
         BwMctpPldm_nextStateEffecterId(mctpPldmHandle, &effecterId);
      }
   }
}




int8_t bmcUsbWriteFunction(uint8_t* buffer, uint16_t* length)
{
   int writeResult = bw_bmc_usb_write(usbHandle, buffer, *length);

   ///printf("bw_bmc_usb_write(%p, %p, %d) = %d\n", usbHandle, buffer, *length, writeResult);

   if (writeResult == *length)
      return 0;

   return -1;
}


int8_t bmcUsbReadFunction(uint8_t* buffer, uint16_t* length)
{
   int readResult;


   readResult = bw_bmc_usb_read(usbHandle, buffer, 1000);

   //printf("bw_bmc_usb_read(%p, %p, %d) = %d\n", usbHandle, buffer, 1000, readResult);

   *length = readResult;

   return 0;
}


char* sensorOperationalStateName(uint8_t operationalState)
{
   switch (operationalState)
   {
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_ENABLED:        return "enabled";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_DISABLED:       return "disabled";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_UNAVAILABLE:    return "unavailable";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_STATUS_UKNOWN:  return "status unknown";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_FAILED:         return "failed";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_INITIALIZING:   return "initializing";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_SHUTTING_DOWN:  return "shutting down";
      case BW_MCTP_PLDM_SENSOR_OPERATIONAL_STATE_IN_TEST:        return "in test";
      default:                                                   return "";
   }
}

char* effecterOperationalStateName(uint8_t operationalState)
{
   switch (operationalState)
   {
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_UPDATE_PENDING:     return "enabled update pending";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_ENABLED_NO_UPDATE_PENDING:  return "enabled no update pending";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_DISABLED:                   return "disabled";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_UNAVAILABLE:                return "unavailable";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_STATUS_UKNOWN:              return "status unknown";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_FAILED:                     return "failed";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_INITIALIZING:               return "initializing";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_SHUTTING_DOWN:              return "shutting down";
      case BW_MCTP_PLDM_EFFECTER_OPERATIONAL_STATE_IN_TEST:                    return "in test";
      default:                                                                 return "";
   }
}


/* EOF - monitor.c */
