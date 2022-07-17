#pragma once

struct ValueOfStock
{
    double value;
    double paid;
};

inline std::string getCurrentDate()
{
    time_t theTime = time(NULL);
    struct tm *aTime = localtime(&theTime);

    int day = aTime->tm_mday;
    int month = aTime->tm_mon + 1; // Month is 0 - 11, add 1 to get a jan-dec 1-12 concept
    int year = aTime->tm_year + 1900; // Year is # years since 1900
    std::string date;
    if( day > 9 && month > 9 )
        date = std::to_string(year)+"-" + std::to_string(month)+"-"+ std::to_string(day);
    else if( day < 10 && month > 9 )
        date = std::to_string(year)+"-" + std::to_string(month)+"-0"+ std::to_string(day);
    else if( day > 10 && month < 9 )
        date = std::to_string(year)+"-0" + std::to_string(month)+"-"+ std::to_string(day);
    else
        date = std::to_string(year)+"-0" + std::to_string(month)+"-0"+ std::to_string(day);
    return date;
}