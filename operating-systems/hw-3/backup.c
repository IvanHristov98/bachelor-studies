#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <inttypes.h>
#include <regex.h>
#include <math.h>

// todo: documentation, reading numbers - int to string


/*==== CONFIGURATIONS ====*/

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

// returns whether the architecture is little or big endian based
int isLilEndian();

// returns the size of each option based on the set of segment format macros
int getOptionSize(int format);

// compiles a regex from pattern and compares it to data. It exits with an appropriate message when there is no match.
void validateWithRegex(const char * pattern, const char * data);

// converts a string to an integer and returns the integer
int stringToInt(const char * num);

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

int main(const int argc, const char * argv[])
{
	// setting endiannes
	isLittleEndian = isLilEndian();

	if (argc == 2 && strcmp(argv[1], "-h") == 0)
	{
		// print help information
		write(1, "help", 5);
		exit(0);
	}

	if (argc < 3)
	{
		errx(1, "Incorrect number of arguments.");
	}

	if (strcmp(argv[2], "-c") == 0)
	{
		createConfigFile(argv, argc);
		exit(0);
	}

	if (strcmp(argv[2], "-s") == 0)
	{
		changeOption(argv, argc, 1);
	}

	if (strcmp(argv[2], "-S") == 0)
	{
		changeOption(argv, argc, 0);
	}

	if (strcmp(argv[2], "-l") == 0 || strcmp(argv[2], "-L") == 0)
	{
		int useActivityMask = (strcmp(argv[2], "-l") == 0) ? 1 : 0;

		if (argc < 4)
		{
			listOptions(argv, useActivityMask);
		}
		else
		{
			int * optionsMap = genOptionsMap(argc, argv);
			listOptionValues(argv, useActivityMask, optionsMap);

			free(optionsMap);
		}
	}

	if (strcmp(argv[2], "-g") == 0 || strcmp(argv[2], "-G") == 0)
	{
		int useActivityMask = (strcmp(argv[2], "-g") == 0) ? 1 : 0;

		if (argc != 4)
		{
			errx(2, "Wrong number of arguments for functionality -g. Probably you meant -l?");
		}

		int * optionsMap = genOptionsMap(argc, argv);
		listOptionValues(argv, useActivityMask, optionsMap);

		free(optionsMap);
	}

	if (strcmp(argv[2], "-b") == 0)
	{
		setActivityBits(argv, argc);
	}

	exit(0);
}

void createConfigFile(const char * options[], const int optionsCount)
{
	const int START = 3;
	int cnt = 0;

	// validation
	if ((optionsCount - 3) % 2 != 0)
	{
		errx(8, "Wrong input values. There should be a corresponding type of segment to each segment number");
	}

	int confFD = openFile(options[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);

	int mwa; // macro work around

	for (int i = START; i < optionsCount; ++i)
	{
		if (i % 2 == 0)
		{
			if (strcmp(options[i], "t") == 0)
			{
				mwa = TEXT_F; 
			}
			else if (strcmp(options[i], "n") == 0)
			{
				mwa = DIGIT_F;
			}
			else if (strcmp(options[i], "b") == 0)
			{
				mwa = BYTE_F;
			}
			// validation
			else
			{
				close(confFD);
				errx(8, "Invalid options values.");
			}

			write(confFD, (char *)&mwa, 1);

			for (int meta = 0, i = 0; i < SEGMENT_ACTIVE_BITS_SIZE + SEGMENT_DATA_SIZE; ++i)
			{
				write(confFD, (char *)&meta, 1);
			}
		}
		else
		{
			// validation
			if (cnt != stringToInt(options[i]))
			{
				close(confFD);
				errx(8, "Incorrect argument values. There should be no holes between the numbers.");
			}

			++cnt;
		}
	}

	close(confFD);
}

void changeOption(const char * options[], const int optionsCount, int setBit)
{
	if ((optionsCount - 2) % 2 != 1)
	{
		errx(2, "Incorrect number of arguments");
	}

	for (int i = 3; i < optionsCount; i += 2)
	{
		changeOptionUtil(options[1], options[i], options[i+1], setBit);
	}
}

void changeOptionUtil(const char * file, const char * optionName, const char * optionVal, int setBit)
{
	int confFD = openFile(file, O_RDWR, 0777);

	int segmentNum = -1;
	int paramNumWithinSegment = -1;

	// cnt is for better agility
	for (int cnt = 0, i = 0; i < NUM_SEGMENTS; ++i)
	{
		for (int j = 0; j < numOptionsPerRow[i]; ++j)
		{
			if (strcmp(paramNames[cnt], optionName) == 0)
			{
				segmentNum = i;
				paramNumWithinSegment = j;

				// before anything else validate data
				validateWithRegex(validVals[cnt], optionVal);
			}

			++cnt;
		}
	}

	if (segmentNum == -1 || paramNumWithinSegment == -1)
	{
		errx(2, "Not existing option name.");
	}

	lSeekSafe(confFD, 
		(SEGMENT_TYPE_SIZE 
		+ SEGMENT_ACTIVE_BITS_SIZE 
		+ SEGMENT_DATA_SIZE) * segmentNum
		+ SEGMENT_TYPE_SIZE, SEEK_SET);

	lSeekSafe(confFD, paramNumWithinSegment / 8, SEEK_CUR);

	// if setBit flag is up then change bit acitivity
	if (setBit == 1)
	{
		uint8_t activityByte;

		if (read(confFD, &activityByte, 1) != -1)
		{
			if (strcmp(optionVal, "") == 0)
			{
				activityByte -= (uint8_t)powi(2, paramNumWithinSegment);
			}
			else
			{
				activityByte = activityByte | (uint8_t)powi(2, paramNumWithinSegment);
			}
			// move backwards by one position
			lSeekSafe(confFD, -1, SEEK_CUR);

			// update activity byte
			write(confFD, &activityByte, 1);
		}
	}

	// move to the data part of the segment
	lSeekSafe(confFD, SEGMENT_ACTIVE_BITS_SIZE - paramNumWithinSegment / 8 - 1, SEEK_CUR);

	// getting option size
	uint32_t optionSize = getOptionSize(segmentTypes[segmentNum]);

	// case handles only text segments
	uint32_t cmpSize = (optionSize == TEXT_OPTION_SIZE) ? optionSize -1 : optionSize;

	if (strlen(optionVal) > cmpSize && optionSize != DIGIT_OPTION_SIZE)
	{
		errx(2, "Argument value too long.");
	}

	// move to the location of the option
	lseek(confFD, optionSize * paramNumWithinSegment, SEEK_CUR);

	// clear data
	for (uint32_t i = 0; i < optionSize; ++i)
	{
		char emptyByte = '\0';

		write(confFD, &emptyByte, 1);
	}

	lseek(confFD, -((int)optionSize), SEEK_CUR);

	if (optionSize == DIGIT_OPTION_SIZE)
	{
		writeNum(confFD, stringToInt(optionVal));
	}
	else
	{
		write(confFD, optionVal, strlen(optionVal));
	}

	// closing file descriptor and exiting
	close(confFD);
}

void listOptionValues(const char * args[], const int onlyActive, const int * optionsMap)
{
	int confFD = openFile(args[1], O_RDONLY, 0777);
	int cnt = 0;

	for (int i = 0; i < NUM_SEGMENTS; ++i)
	{
		uint8_t num;

		// validations
		if (read(confFD, &num, 1) == -1)
		{
			errx(2, "File reading error.");
		}

		// matching the expected type
		if ((int)num != segmentTypes[i])
		{
			errx(2, "Unexpected segment type.");
		}

		// META DATA
		int optionSize = getOptionSize(num);
		uint8_t metaData[7];

		if (read(confFD, metaData, 7) == -1)
		{
			errx(2, "Error reading config file.");
		}

		int buffSize = (num == BYTE_F) ? optionSize + 1 : optionSize;

		char * data = (char *)malloc(buffSize);

		for (int j = 0; j < numOptionsPerRow[i]; ++j)
		{
			if (num == TEXT_F && read(confFD, data, optionSize) == -1)
			{
				errx(2, "Error reading file.");
			}
			// handling byte case
			else if (num == BYTE_F && read(confFD, data, optionSize))
			{
				data[buffSize - 1] = '\0';
			}
			else if (num == DIGIT_F)
			{
				int test = readNum(confFD);
				sprintf(data, "%d", test);
			}

			// problem with terminating 0
			validateWithRegex(validVals[cnt], data);

			if (optionsMap[cnt] == 1 && !((onlyActive == 1 
				&& (metaData[j / 8] & (j % 8 + 1)) == (j % 8 + 1)) || onlyActive == 0))
			{
				errx(2, "Given option is not active.");
			}

			if (optionsMap[cnt] == 1 && ((onlyActive == 1 
				&& (metaData[j / 8] & (j % 8 + 1)) == (j % 8 + 1)) || onlyActive == 0))
			{
				printCStr(1, paramNames[cnt]);
				write(1, "> ", 2);
				write(1, data, optionSize);
				write(1, "\n", 1);
			}

			++cnt;
		}

		free(data);

		if (lseek(confFD, SEGMENT_DATA_SIZE - numOptionsPerRow[i] * optionSize, SEEK_CUR) == -1)
		{
			errx(1, "Seeked wrongly!");
		}
	}

	close(confFD);
}

void listOptions(const char * args[], const int onlyActive)
{
	int confFD = openFile(args[1], O_RDONLY, 0777);
	int numOptions = getNumOptions();
	int * optionAvailabilityMap = malloc(numOptions * sizeof(int));

	for (int i = 0; i < numOptions; ++i)
	{
		optionAvailabilityMap[i] = 0;
	}

	int cnt = 0;

	for (int i = 0; i < NUM_SEGMENTS; ++i)
	{
		if (lseek(confFD, SEGMENT_TYPE_SIZE, SEEK_CUR) == -1)
		{
			errx(2, "Seeked wrongly.");
		}

		uint8_t metaData[7];
		
		if (read(confFD, metaData, 7) == -1)
		{
			errx(2, "Error reading config file.");
		}

		for (int j = 0; j < numOptionsPerRow[i]; ++j)
		{
			int cmp = powi(2, j % 8);

			if ((metaData[j / 8] & cmp) == cmp)
			{
				optionAvailabilityMap[cnt] = 1;
			}

			cnt++;
		}

		if (lseek(confFD, SEGMENT_DATA_SIZE, SEEK_CUR) == -1)
		{
			errx(2, "Seeked wrongly.");
		}
	}

	for (int i = 0; i < numOptions; ++i)
	{
		if (optionAvailabilityMap[i] == 1 || !(onlyActive == 1))
		{
			printCStr(1, paramNames[i]);
			write(1, "\n", 1);
		}
	}

	close(confFD);
}

void setActivityBits(const char * args[], const int numArgs)
{
	if (numArgs % 2 != 1)
	{
		errx(2, "Incorrect number of arguments");
	}

	int numOptions = getNumOptions();

	for (int i = 3; i < numArgs; i += 2)
	{
		for (int j = 0; j < numOptions; j++)
		{
			if (strcmp(args[i], paramNames[j]) == 0
				&& (strcmp(args[i + 1], "1") == 0 || strcmp(args[i + 1], "0") == 0))
			{
				setActivityBitsUtil(args[1], j, stringToInt(args[i + 1]));

				break;
			}

			if (j >= numOptions - 1)
			{
				errx(2, "Invalid option name or bit value. Note out that bit value must be either 1 or 0.");
			}
		}
	}
}

void setActivityBitsUtil(const char * filePath, const int optionPos, const int bitVal)
{
	int confFD = openFile(filePath, O_RDWR, 0777);
	int cnt = 0;

	for (int i = 0; i < NUM_SEGMENTS; ++i)
	{
		lSeekSafe(confFD, 1, SEEK_CUR);


		uint8_t metaData[7];
		
		if (read(confFD, metaData, SEGMENT_ACTIVE_BITS_SIZE) == -1)
		{
			errx(2, "Error reading config file.");
		}

		for (int j = 0; j < numOptionsPerRow[i]; ++j)
		{
			if (optionPos == cnt)
			{
				int pos = j / 8;
				int bit = powi(2, j % 8);

				lSeekSafe(confFD, pos-SEGMENT_ACTIVE_BITS_SIZE, SEEK_CUR);

				if (bitVal == 0)
				{
					metaData[pos] = ((metaData[pos] & bit) == bit) ? metaData[pos] ^ bit : metaData[pos];				
				}
				else { metaData[pos] |= bit; }

				write(confFD, (char *)&(metaData[pos]), 1);

				lSeekSafe(confFD, SEGMENT_ACTIVE_BITS_SIZE - pos - 1, SEEK_CUR);
			}

			cnt++;
		}

		lSeekSafe(confFD, SEGMENT_DATA_SIZE, SEEK_CUR);
	}

	close(confFD);
}

int stringToInt(const char * num)
{
	int cnt = 0;
	int result = 0;
	int pow = 1;

	// setting number of digits
	while (num[cnt] != '\0') { ++cnt; pow *= 10; }

	// resetting counter
	cnt = 0;

	while (num[cnt] != '\0')
	{
		//printf("%c", num[cnt]);
		if ((int)num[cnt] < (int)'0' || (int)num[cnt] > (int)'9')
		{
			errx(8, "Expected a digit. Received a mixture.");
		}

		// calculating the number
		pow /= 10;
		result += pow * ((int)num[cnt] - (int)'0');
		++cnt;
	}

	return result;
}

int openFile(const char * const path, const int flags, const mode_t mode)
{
	int fileFD = open(path, flags, mode);

	if (fileFD == -1)
	{
		errx(1, "File could not be open!");
	}

	return fileFD;
}

void lSeekSafe(const int fd, const int offset, const int FLAGS)
{
	if (lseek(fd, offset, FLAGS) == -1)
	{
		errx(2, "Sorry but something went wrong when 'seeking' the file.");
	}
}

void writeNum(const int fd, const int num)
{
	if (isLittleEndian == 1)
	{
		write(fd, (char *)&num, 4);
		return;
	}

	// maintaining little endian file format
	for (int i = 3; i >= 0; --i)
	{
		write(fd, ((char *)&num + i), 1);
	}
}

int readNum(const int fd)
{
	const int INT_SIZE = 4;

	int num;

	if (read(fd, (char *)&num, INT_SIZE) != INT_SIZE)
	{
		errx(2, "Error reading number from file.");
	}

	if (isLittleEndian == 1) { return num; }

	int bigEndianNum = 0;

	for (int i = 0; i < INT_SIZE; ++i)
	{
		((char *)&bigEndianNum)[INT_SIZE - 1 - i] = ((char *)&num)[i];
	}

	return 1;
}

int powi(int x, int y)
{
	if (y == 1) { return x; }

	int res = 1;
	int cnt = 1;
	int accum = x;

	while (2*cnt <= y)
	{
		res = accum*accum;
		accum = res;
		cnt *= 2;
	}

	for (int i = 0; i < y - cnt; ++i)
	{
		res *= x;
	}

	return res;
}

int isLilEndian()
{
	uint32_t num = 0x01234567;

	return *((uint8_t *)&num) == 0x67;
}

int getOptionSize(int format)
{
	switch(format)
	{
		case TEXT_F:
			return TEXT_OPTION_SIZE;
		case DIGIT_F:
			return DIGIT_OPTION_SIZE;
		case BYTE_F:
			return BYTE_OPTION_SIZE;
	}

	return -1;
}

void validateWithRegex(const char * pattern, const char * data)
{
	// declaring regex
	regex_t regex;

	// compiling regex
	if (regcomp(&regex, pattern, REG_EXTENDED) != 0)
	{
		errx(4, "Regex could not compile.");
	}

	if (!(regexec(&regex, data, 0, NULL, 0) == 0))
	{
		printf("%s\n", data);
		errx(5, "Input file segment data is invalid.");
	}

	regfree(&regex);
}

void printCStr(const int fd, const char * const str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		write(fd, (char *)&(str[i]), 1);

		++i;
	}
}

int * genOptionsMap(const int argc, const char * argv[])
{
	// Calculating the total number of options
	int numOptions = getNumOptions();


	int * map = (int *)malloc(numOptions * sizeof(int));
	for (int i = 0; i < numOptions; ++i) { map[i] = 0; }

	for (int i = 3; i < argc; ++i)
	{
		for (int j = 0; j < numOptions; ++j)
		{
			if (strcmp(paramNames[j], argv[i]) == 0)
			{
				map[j] = 1;
				break;
			}

			if (j == numOptions - 1)
			{
				errx(2, "Incorrect arguments.");
			}
		}
	}

	return map;
}

int getNumOptions()
{
	int numOptions = 0;

	for (int i = 0; i < NUM_SEGMENTS; ++i)
	{
		numOptions += numOptionsPerRow[i];
	}

	return numOptions;
}