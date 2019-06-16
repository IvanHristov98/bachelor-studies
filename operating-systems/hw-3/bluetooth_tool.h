// format identifiers
#define TEXT_F 0
#define DIGIT_F 1
#define BYTE_F 2
// option sizes
#define TEXT_OPTION_SIZE 16
#define DIGIT_OPTION_SIZE 4
#define BYTE_OPTION_SIZE 1
// segment part sizes in bytes
#define SEGMENT_TYPE_SIZE 1
#define SEGMENT_ACTIVE_BITS_SIZE 7
#define SEGMENT_DATA_SIZE 56
// total number of segments
#define NUM_SEGMENTS 5

// error numbers
#define INCORRECT_NUM_ARGS 1
#define WRONG_ARGS 2
#define INVALID_OPTION_VALUES 3
#define INVALID_ARG_VALUES 4
#define INVALID_OPTION_NAME 5
#define INVALID_FILE 6

#define FILE_READING_ERROR 10
#define WRONG_SEGMENT 11
#define FORKING_ERROR 12
#define UNAVAILABLE_RESOURCES 13
#define INTERNAL_PROCESS_ERROR 14
#define REGEX_COMP_ERROR 15

#define INACTIVE_OPTION 20


// regex patterns - PV stands for Permitted Values
char * const DEVICE_NAME_PV = "^[a-zA-Z0-9_-]*$|^$";
char * const ROM_REVISION_PV = "^[a-zA-Z0-9._-]*$|^$";
char * const SERIAL_NUMBER_PV = "^[A-Z0-9]*$|^$";

char * const BD_ADDR_PART0_PV = "^[A-Z0-9:]*$|^$";
char * const BD_ADDR_PART1_PV = "^[A-Z0-9:]*$|^$";

char * const BD_PASS_PART0_PV = "^[a-z0-9]*$|^$";
char * const BD_PASS_PART1_PV = "^[a-z0-9]*$|^$";

char * const ROM_CHECKSUM_PART0_PV = "^[a-z0-9]*$|^$";
char * const ROM_CHECKSUM_PART1_PV = "^[a-z0-9]*$|^$";

char * const SERIAL_BAUDRATE_PV = "^1200$|^2400$|^4800$|^9600$|^19200$|^115200$|^$";
char * const AUDIO_BITRATE_PV = "^32$|^96$|^128$|^160$|^192$|^256$|^320$|^$";
char * const SLEEP_PERIOD_PV = "^100$|^500$|^1000$|^5000$|^10000$|^$";
char * const SERIAL_PARITY_PV = "^N$|^E$|^O$|^$";
char * const SERIAL_DATA_BIT_PV = "^5$|^6$|^7$|^8$|^$";
char * const SERIAL_STOP_BIT_PV = "^0$|^1$|^$";

// helper file
char * const HELP_FILE = "help.me";

/*==== FUNCTION DECLARATIONS ====*/

// generates a configuration file based on the given arguments
void createConfigFile(const char * options[], const int optionsCount);

// used as an argument interpreter function that calls changeOptionUtil on each valid pair of arguments
// for more info see changeOptionUtil
void changeOption(const char * options[], const int optionsCount, int setBit);

// utility function used by changeOption to the change the value of a given option
void changeOptionUtil(const char * file, const char * optionName, const char * optionVal, int setBit);

// lists the options with their corresponding values
// onlyActive flag is set whenever a user wants to print out only the active configurations
void listOptionValues(const char * args[], const int onlyActive, const int * optionsMap);

// lists the available options
// onlyActive flag is set whenever a user wants to print only the active configurations
void listOptions(const char * args[], const int onlyActive);

// used as an argument interpreter function that calls setActivityBitsUtil on each valid pair of arguments.
// for more info see setActivityBitsUtil
void setActivityBits(const char * args[], const int numArgs);

// utility function used by setActivityBits to set the activity bits of a option within a segment
void setActivityBitsUtil(const char * filePath, const int optionPos, const int bitVal);

// outputs file manual page to less
void help();

// returns whether the architecture is little or big endian based
int isLilEndian();

// returns the size of each option based on the set of segment format macros
int getOptionSize(int format);

// compiles a regex from pattern and compares it to data. It exits with an appropriate message when there is no match.
void validateWithRegex(const char * pattern, const char * data);

// opens file safely and returns a file descriptor
int openFile(const char * const path, const int flags, const mode_t mode);

// lseek but with handled errors
void lSeekSafe(const int fd, const int offset, const int FLAGS);

// writes a number in a file handling both little and big endian formats
void writeNum(const int fd, const int num);

// reads a number from a file handling both little endian and big endian formats
int readNum(const int fd);

// returns the power of x by y
int powi(int x, int y);

// prints a cstring synchronously
void printCStr(const int fd, const char * const str);

// generates a map of available options. Memory is located onto the heap and must therefore be freed.
int * genOptionsMap(const int argc, const char * argv[]);

// returns the total number of arguments based onto the configuration
int getNumOptions();


// segment type map
const int segmentTypes[NUM_SEGMENTS] = {
	TEXT_F, TEXT_F, DIGIT_F, BYTE_F, TEXT_F
};

// segment size map
const int numOptionsPerRow[NUM_SEGMENTS]= {
	3, 3, 3, 3, 3
};

// parameter name map
const char * const paramNames[] = {
	"device_name", "rom_revision", "serial_number",
	"bd_addr_part0", "bd_addr_part1", "bd_pass_part0",
	"serial_baudrate", "audio_bitrate", "sleep_period",
	"serial_parity", "serial_data_bit", "serial_stop_bit",
	"bd_pass_part1", "rom_checksum_part0", "rom_checksum_part1",
};

// valid parameter values map
const char * const validVals[] = {
	DEVICE_NAME_PV, ROM_REVISION_PV, SERIAL_NUMBER_PV,
	BD_ADDR_PART0_PV, BD_ADDR_PART1_PV, BD_PASS_PART0_PV,
	SERIAL_BAUDRATE_PV, AUDIO_BITRATE_PV, SLEEP_PERIOD_PV,
	SERIAL_PARITY_PV, SERIAL_DATA_BIT_PV, SERIAL_STOP_BIT_PV,
	BD_PASS_PART1_PV, ROM_CHECKSUM_PART0_PV, ROM_CHECKSUM_PART1_PV
};

// global variable to store whether the architecture uses little or big endian format
int isLittleEndian;