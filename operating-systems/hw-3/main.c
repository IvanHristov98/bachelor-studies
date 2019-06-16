#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <inttypes.h>
#include <regex.h>
#include <math.h>

#include "bluetooth_tool.h"

int main(const int argc, const char * argv[])
{
	// setting endiannes
	isLittleEndian = isLilEndian();

	if (argc == 2 && strcmp(argv[1], "-h") == 0)
	{
		help();
		exit(0);
	}

	if (argc < 3)
	{
		errx(INCORRECT_NUM_ARGS, "Incorrect number of arguments.");
	}

	if (strcmp(argv[2], "-c") == 0)
	{
		createConfigFile(argv, argc);
		exit(0);
	}
	else if (strcmp(argv[2], "-s") == 0)
	{
		changeOption(argv, argc, 1);
	}
	else if (strcmp(argv[2], "-S") == 0)
	{
		changeOption(argv, argc, 0);
	}
	else if (strcmp(argv[2], "-l") == 0 || strcmp(argv[2], "-L") == 0)
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
	else if (strcmp(argv[2], "-g") == 0 || strcmp(argv[2], "-G") == 0)
	{
		int useActivityMask = (strcmp(argv[2], "-g") == 0) ? 1 : 0;

		if (argc != 4)
		{
			errx(INCORRECT_NUM_ARGS, "Wrong number of arguments for functionality -g. Probably you meant -l?");
		}

		int * optionsMap = genOptionsMap(argc, argv);
		listOptionValues(argv, useActivityMask, optionsMap);

		free(optionsMap);
	}
	else if (strcmp(argv[2], "-b") == 0)
	{
		setActivityBits(argv, argc);
	}
	else
	{
		errx(WRONG_ARGS, "Wrong argument");
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
		errx(INCORRECT_NUM_ARGS, "Wrong input values. There should be a corresponding type of segment to each segment number");
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
				errx(INVALID_OPTION_VALUES, "Invalid options values.");
			}

			write(confFD, (char *)&mwa, 1);

			for (int empty = 0, i = 0; i < SEGMENT_ACTIVE_BITS_SIZE + SEGMENT_DATA_SIZE; ++i)
			{
				write(confFD, (char *)&empty, 1);
			}
		}
		else
		{
			// validation
			if (cnt != atoi(options[i]))
			{
				close(confFD);
				errx(INVALID_ARG_VALUES, "Incorrect argument values. There should be no holes between the numbers.");
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
		errx(INCORRECT_NUM_ARGS, "Incorrect number of arguments");
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
		errx(INVALID_OPTION_NAME, "Not existing option name.");
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
		errx(WRONG_ARGS, "Argument value too long.");
	}

	// move to the location of the option
	lSeekSafe(confFD, optionSize * paramNumWithinSegment, SEEK_CUR);

	// clear data
	for (uint32_t i = 0; i < optionSize; ++i)
	{
		char emptyByte = '\0';

		write(confFD, &emptyByte, 1);
	}

	lSeekSafe(confFD, -((int)optionSize), SEEK_CUR);

	if (optionSize == DIGIT_OPTION_SIZE)
	{
		writeNum(confFD, atoi(optionVal));
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
			errx(FILE_READING_ERROR, "File reading error.");
		}

		// matching the expected type
		if ((int)num != segmentTypes[i])
		{
			errx(WRONG_SEGMENT, "Unexpected segment type.");
		}

		// META DATA
		int optionSize = getOptionSize(num);
		uint8_t metaData[7];

		if (read(confFD, metaData, 7) == -1)
		{
			errx(FILE_READING_ERROR, "Error reading config file.");
		}

		int buffSize = (num == BYTE_F) ? optionSize + 1 : optionSize;

		char * data = (char *)malloc(buffSize);

		for (int j = 0; j < numOptionsPerRow[i]; ++j)
		{
			// in case it is an integer
			// sprintf is buffer size dependant and itoa is not standard
			int val;

			if (num == TEXT_F && read(confFD, data, optionSize) == -1)
			{
				errx(FILE_READING_ERROR, "Error reading file.");
			}
			// handling byte case
			else if (num == BYTE_F && read(confFD, data, optionSize))
			{
				data[buffSize - 1] = '\0';
			}
			else if (num == DIGIT_F)
			{
				val = readNum(confFD);
				sprintf(data, "%d", val);
			}

			// problem with terminating 0
			validateWithRegex(validVals[cnt], data);

			if (optionsMap[cnt] == 1 && !((onlyActive == 1 
				&& (metaData[j / 8] & (j % 8 + 1)) == (j % 8 + 1)) || onlyActive == 0))
			{
				errx(INACTIVE_OPTION, "Given option is not active.");
			}

			if (optionsMap[cnt] == 1 && ((onlyActive == 1 
				&& (metaData[j / 8] & (j % 8 + 1)) == (j % 8 + 1)) || onlyActive == 0))
			{
				printCStr(1, paramNames[cnt]);
				write(1, "> ", 2);
				if (num == DIGIT_F)
				{
					// \n flushes the buffer thus making it synchronized
					printf("%d\n", val);
				}
				else 
				{
				 	write(1, data, optionSize); 
				 	write(1, "\n", 1);
				}	
			}

			++cnt;
		}

		free(data);
		lSeekSafe(confFD, SEGMENT_DATA_SIZE - numOptionsPerRow[i] * optionSize, SEEK_CUR);
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
		lSeekSafe(confFD, SEGMENT_TYPE_SIZE, SEEK_CUR);

		uint8_t metaData[7];
		
		if (read(confFD, metaData, 7) == -1)
		{
			errx(FILE_READING_ERROR, "Error reading config file.");
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

		lSeekSafe(confFD, SEGMENT_DATA_SIZE, SEEK_CUR);
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
		errx(INCORRECT_NUM_ARGS, "Incorrect number of arguments");
	}

	int numOptions = getNumOptions();

	for (int i = 3; i < numArgs; i += 2)
	{
		for (int j = 0; j < numOptions; j++)
		{
			if (strcmp(args[i], paramNames[j]) == 0
				&& (strcmp(args[i + 1], "1") == 0 || strcmp(args[i + 1], "0") == 0))
			{
				setActivityBitsUtil(args[1], j, atoi(args[i + 1]));

				break;
			}

			if (j >= numOptions - 1)
			{
				errx(WRONG_ARGS, "Invalid option name or bit value. Note out that bit value must be either 1 or 0.");
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
			errx(FILE_READING_ERROR, "Error reading config file.");
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

void help()
{
	int helpFD = openFile(HELP_FILE, O_RDONLY, 0777);

	int pid = fork();

	if (pid == -1)
	{
		errx(FORKING_ERROR, "Internal error when forking. Please try again.");
	}

	if (pid == 0)
	{
		dup2(helpFD, 0);
		execlp("less", "less", (char *)NULL);
	}

	close(helpFD);

	int status = 0;

	waitpid(pid, &status, 0);

	if (!(WIFEXITED(status) && WEXITSTATUS(status) == 0))
	{
		errx(INTERNAL_PROCESS_ERROR, "Internal error. Something went wrong with documentation printing.");
	}
}

int openFile(const char * const path, const int flags, const mode_t mode)
{
	int fileFD = open(path, flags, mode);

	if (fileFD == -1)
	{
		errx(UNAVAILABLE_RESOURCES, "File could not be open!");
	}

	return fileFD;
}

void lSeekSafe(const int fd, const int offset, const int FLAGS)
{
	if (lseek(fd, offset, FLAGS) == -1)
	{
		errx(FILE_READING_ERROR, "Sorry but something went wrong when 'seeking' the file.");
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
		errx(FILE_READING_ERROR, "Error reading number from file.");
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
		errx(REGEX_COMP_ERROR, "Regex could not compile.");
	}

	if (!(regexec(&regex, data, 0, NULL, 0) == 0))
	{
		errx(INVALID_FILE, "Input file segment data is invalid.");
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
				errx(WRONG_ARGS, "Incorrect arguments.");
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