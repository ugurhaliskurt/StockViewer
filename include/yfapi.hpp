#include <string.h>
#include <curl/curl.h>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <stdio.h>

#include "Interval.hpp"
#include "Formatter.hpp"
#include "Engine.hpp"
#include "utilities.hpp"

namespace yfapi
{
    class YahooFinanceAPI
    {
        public:
            YahooFinanceAPI();
            void set_interval(Interval interval);
            void set_col_name(std::string col_name);
            std::string get_ticker_data(std::string ticker, std::string start_date, std::string end_date);
            double calculate( std::string stockName, std::string start_date, int quantityOfStock );

            std::map< std::string, ValueOfStock >& getStockUsdMap();

        private:
            std::string _base_url;
            Interval _interval;
            std::string _col_name;
            std::map< std::string, ValueOfStock > stockUsd; //represent stock and dollar quantity for that

            std::string build_url(std::string ticker, std::string start_date, std::string end_date);
            bool string_replace(std::string& str, const std::string from, const std::string to);
            std::string timestamp_from_string(std::string date);
            std::string download_file(std::string url);
            static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
            void addNewStock(std::map< std::string, double > &myMap, double paidInUsd );
    };

    YahooFinanceAPI::YahooFinanceAPI()
    {
        this->_base_url =
            "https://query1.finance.yahoo.com/v7/finance/download/{ticker}?period1={start_time}&period2={end_time}&interval={interval}&events=history";
        this->_interval = DAILY;
        this->_col_name = "Open";
    }

    std::string YahooFinanceAPI::timestamp_from_string(std::string date)
    {
        struct std::tm time = {0,0,0,0,0,0,0,0,0};
        std::istringstream ss(date);
        ss >> std::get_time(&time, "%Y-%m-%d");
        if(ss.fail())
        {
            std::cerr  << "ERROR: Cannot parse date string (" << date <<"); required format %Y-%m-%d" << std::endl;
            exit(1);
        }
        time.tm_hour = 0;
        time.tm_min = 0;
        time.tm_sec = 0;
        std::time_t epoch = std::mktime(&time);

        return std::to_string(epoch);
    }

    bool YahooFinanceAPI::string_replace(std::string& str, const std::string from, const std::string to)
    {
        size_t start = str.find(from);
        if(start == std::string::npos)
        {
            return false;
        }
        str.replace(start, from.length(), to);
        return true;
    }

    std::string YahooFinanceAPI::build_url(std::string ticker, std::string start_date, std::string end)
    {
        std::string url = this->_base_url;
        string_replace(url, "{ticker}", ticker);
        string_replace(url, "{start_time}", timestamp_from_string(start_date));
        string_replace(url, "{end_time}", timestamp_from_string(end));
        string_replace(url, "{interval}", get_api_interval_value(this->_interval));
        return url;
    }


    void YahooFinanceAPI::set_interval(Interval interval)
    {
        this->_interval = interval;
    }

    void YahooFinanceAPI::set_col_name(std::string name)
    {
        this->_col_name = name;
    }

    size_t YahooFinanceAPI::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
    { 
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }

    std::string YahooFinanceAPI::download_file(std::string url)
    {
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        std::string readBuffer;
        if (curl)
        {
            //fp = fopen(filename.c_str(), "wb");
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);

            /* always cleanup */
            curl_easy_cleanup(curl);
        }
        return readBuffer;
    }

    std::string YahooFinanceAPI::get_ticker_data(std::string ticker, std::string start_date, std::string end)
    {
        std::string url = build_url(ticker, start_date, end);
        std::time_t now = std::time(0); // now

        return download_file(url);
    }


    double YahooFinanceAPI::calculate( std::string stockName, std::string start_date, int totalPrice )
    {
        std::string current_date = getCurrentDate();
        std::string result = get_ticker_data( stockName, start_date, current_date );
        auto dateValuemap = Formatter::convertDateValueMap(result);

        std::string usdTl = get_ticker_data( "TRY=X", start_date, current_date );
        auto usdTlmap = Formatter::convertDateValueMap(usdTl);

        Formatter::makeValidDate( start_date, dateValuemap );
        Formatter::makeValidDate( current_date, dateValuemap );
        int quantityOfStock = totalPrice / dateValuemap[start_date];



        std::map< std::string, double > tmpStockUsd;
        std::for_each(dateValuemap.begin(), dateValuemap.end(), [&tmpStockUsd, &quantityOfStock, &usdTlmap]( std::pair<std::string, double> pair ){
            tmpStockUsd.insert( std::make_pair(pair.first, (quantityOfStock * pair.second / usdTlmap[pair.first])) );
        });

        addNewStock(tmpStockUsd, (double)totalPrice / usdTlmap[start_date] );
        return 0;
    }

    void YahooFinanceAPI::addNewStock(std::map< std::string, double > &stockMap, double paidInUsd)
    {
        auto it = stockUsd.find(stockMap.begin()->first);
        if( it == stockUsd.end() )
        {
            for( auto pair : stockMap )
            {
                stockUsd[pair.first].paid = paidInUsd;
                stockUsd[pair.first].value = pair.second;
            }
        }
        else
        {
            for( auto pair : stockMap )
            {
                it->second.value += pair.second;
                it->second.paid += paidInUsd;
                ++it;
            }
        }
    }

    std::map< std::string, ValueOfStock >& YahooFinanceAPI::getStockUsdMap()
    {
        return stockUsd;
    }
}
