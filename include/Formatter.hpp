#pragma once

#include <unordered_map>
#include <sstream>
#include <iostream>

class Formatter
{
    public:
        static std::unordered_map< std::string, double > convertDateValueMap( std::string &str );
        static void makeValidDate( std::string &date );
    private:
        static int dayofweek(int d, int m, int y);
};

std::unordered_map< std::string, double > Formatter::convertDateValueMap(std::string &str)
{
    std::unordered_map< std::string, double > result;
    std::istringstream f(str);
    std::string line;
    getline(f, line); // first line is headers do not read
    while( true )
    {
        if( !getline(f, line) ) break;
        // simple split (alternatively, use strtok)
        std::string key = line.substr(0,line.find(',')); //date
        line.substr(line.find(',')+1); //Open
        line.substr(line.find(',')+1); //High
        line.substr(line.find(',')+1); //low
        std::string value = line.substr(line.find(',')+1); //Close
        try{
            result[key] = std::stod(value);
        }
        catch(...)
        {
        }
        
    }
    return result;
}

int Formatter::dayofweek(int d, int m, int y)
{
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}

void Formatter::makeValidDate( std::string &date )
{
    std::istringstream f(date);
    std::string line;
    getline(f, line);
    int year = std::stoi( line.substr(0,line.find('-')));
    int month = std::stoi( line.substr(line.find('-')+1));
    int day = std::stoi( line.substr(line.find('-')+1));

    int weekday =dayofweek(day, month, year);
    if( weekday > 5 )
    {
        std::cerr << "This date is weekend. Could not calculate :(";
        exit(-1);
    }
}