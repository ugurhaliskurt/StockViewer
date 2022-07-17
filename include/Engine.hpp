#pragma once

#include <Interval.hpp>

class Engine
{
public:
    void prepareDataForRest( Interval interval, std::map<std::string, double> &stockAll, std::map<std::string, double> &usdAll);
private:
    std::map<std::string, double> stockDataInUsd;
    std::map<std::string, double> spentUsd;
};

void Engine::prepareDataForRest( Interval interval, std::map<std::string, double> &stockAll, std::map<std::string, double> &usdAll  )
{
    switch (interval)
    {
    case Interval::DAILY :
        stockDataInUsd = stockAll;
        spentUsd = usdAll;
        break;
    case Interval::WEEKLY :
        {
            auto it = stockAll.begin();
            auto itUsd = usdAll.begin();
            for( int i=0; i<stockAll.size()/5; ++i )
            {
                stockDataInUsd.insert( *it );
                spentUsd.insert( *itUsd );
                std::advance(it,5);
                std::advance(itUsd,5);
            }
        break;
        }
    case Interval::MONTHLY :
        {
            auto it = stockAll.begin();
            auto itUsd = usdAll.begin();
            for( int i=0; i<stockAll.size()/30; ++i )
            {
                stockDataInUsd.insert( *it );
                spentUsd.insert( *itUsd );
                std::advance(it,30);
                std::advance(itUsd,30);
            }
        break;
        }
    default:
        break;
    }
}