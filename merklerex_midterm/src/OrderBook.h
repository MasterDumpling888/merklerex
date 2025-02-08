#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include "Bar.h"
#include "Volume.h"
#include <string>
#include <vector>

// START STUDENT CODE
/* data structure for averge, high, low for graphs */
struct PriceStats
{
    double mean;
    double high;
    double low;
};
// END STUDENT CODE
class OrderBook
{
public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();

    // START STUDENT CODE
    /** return vector of all timestamps */
    std::vector<std::string> getKnownTimestamps();

    /** return vector of all time by minutes only */
    std::vector<std::string> getKnownTimestampsByMinute();
    // END STUDENT CODE

    /** return vector of Orders according to the sent filters*/
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                          std::string product,
                                          std::string timestamp);

    // START STUDENT CODE
    /** return vector of Orders according to the sent filters by minute*/
    std::vector<OrderBookEntry> getOrderByMinute(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp);
    // END STUDENT CODE
    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();

    /** returns the next time after the
     * sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     * */
    std::string getNextTime(std::string timestamp);

    void insertOrder(OrderBookEntry &order);

    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    static double getHighPrice(std::vector<OrderBookEntry> &orders);
    static double getLowPrice(std::vector<OrderBookEntry> &orders);

    // START STUDENT CODE
    static PriceStats getPriceStats(std::vector<OrderBookEntry> &orders);                                     // gets the price stats of the orders
    std::vector<std::string> getSelectedTimestamps(std::string currentTime, int size);                        // gets the selected timestamps set
    std::vector<Candlestick> getCandlesticks(std::string currentTime, std::string product, std::string type); // gets the candlesticks and return vector of candlesticks
    std::vector<Bar> getBars(std::string currentTime, std::string product);                                   // gets the bars and return vector of bars
    std::vector<Volume> getVolume(std::string currentTime, std::string product, std::string type);            // gets the volume and return vector of volume
    // END STUDENT CODE

private:
    std::vector<OrderBookEntry> orders;
};
