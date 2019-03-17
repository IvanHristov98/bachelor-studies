#include <iostream>
#include <string>
#include <vector>

struct DriveTime
{
    int startHours;
    int startMinutes;
    int finishHours;
    int finishMinutes;

    const unsigned int MINUTES_IN_A_HOUR = 60;
    const unsigned int HOURS_IN_A_DAY = 24;

    unsigned int getTimeGapInMinutes()
    {
        int gap = (finishHours * DriveTime::MINUTES_IN_A_HOUR + finishMinutes) - (startHours * DriveTime::MINUTES_IN_A_HOUR + startMinutes);
    
        if (gap < 0)
        {
            gap += DriveTime::HOURS_IN_A_DAY * DriveTime::MINUTES_IN_A_HOUR;
        }
        
        return gap;
    }
};

// Gets the maximum or minimum value of an uint vector.
unsigned int getVectorPoles(std::vector<unsigned int>& vector, bool searchMin = true);

void printTimeString(unsigned int driveTime);

int main()
{
    const unsigned int MAX_CARS = 3;
    const unsigned int HOURS_CAP = 24;
    const unsigned int MINUTES_CAP = 60;

    std::vector<unsigned int> gaps;
    unsigned int carsCounter = 0;

    while (carsCounter < MAX_CARS)
    {
        DriveTime driveTime = DriveTime();
        
        std::cin >> driveTime.startHours >> driveTime.startMinutes >> driveTime.finishHours >> driveTime.finishMinutes;

        gaps.push_back(driveTime.getTimeGapInMinutes());
        carsCounter++;
    }

    unsigned int minTimeInMinutes = getVectorPoles(gaps);
    unsigned int maxTimeInMinutes = getVectorPoles(gaps, false);

    printTimeString(minTimeInMinutes);
    std::cout << std::endl;
    printTimeString(maxTimeInMinutes);

    return 0;
}

unsigned int getVectorPoles(std::vector<unsigned int>& vector, bool searchMin)
{
    unsigned int result = vector[0];

    for (size_t i = 0; i < vector.size(); i++)
    {
        if (searchMin)
        {
            result = (result > vector[i]) ? vector[i] : result;
        }
        else
        {
            result = (result < vector[i]) ? vector[i] : result;
        }
    }

    return result;
}

void printTimeString(unsigned int driveTime)
{
    const unsigned int MINUTES_IN_HOUR = 60;

    unsigned int minutes = driveTime % MINUTES_IN_HOUR;
    unsigned int hours = (driveTime - minutes) / MINUTES_IN_HOUR;

    std::cout << hours << ":";

    if (minutes < 10)
    {
        std::cout << "0" << minutes;
    }
    else { std::cout << minutes; }
}
