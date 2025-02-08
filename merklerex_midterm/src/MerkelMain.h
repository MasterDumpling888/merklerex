#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "Candlestick.h"
#include "Bar.h"
#include "Volume.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();

    // START STUDENT CODE
    void printCandlestick(); // prints the candlestick graph
    void printBar();         // prints the bar graph
    void printVolumeGraph(); // prints the volume graph
    // END STUDENT CODE
    void gotoNextTimeframe();
    int getUserOption(int size);
    void processUserOption(int userOption);
    // START STUDENT CODE
    void printOptions(const std::vector<std::string> &options); // prints the vector of options
    // END STUDENT CODE
    std::string currentTime;

    OrderBook orderBook{"20200601.csv"};

    Wallet wallet;
    // START STUDENT CODE
    Candlestick candlestick;
    Bar bar;
    Volume volume;
    // END STUDENT CODE
};