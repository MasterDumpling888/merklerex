#include "OrderBook.h"
#include "CSVReader.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <ctime>

/** construct, reading a csv data file */
OrderBook::OrderBook(std::string filename)
{
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
std::vector<std::string> OrderBook::getKnownProducts()
{
    std::vector<std::string> products;

    std::map<std::string, bool> prodMap;

    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : prodMap)
    {
        products.push_back(e.first);
    }

    return products;
}

// START STUDENT CODE
/** return vector of timestamps */
std::vector<std::string> OrderBook::getKnownTimestamps()
{
    std::vector<std::string> timestamps;

    std::map<std::string, bool> timeMap;

    for (OrderBookEntry &e : orders)
    {
        timeMap[e.timestamp] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e : timeMap)
    {
        timestamps.push_back(e.first);
    }

    return timestamps;
}

/** return vector of all time by minutes only */
std::vector<std::string> OrderBook::getKnownTimestampsByMinute()
{
    std::vector<std::string> timestamps;

    std::map<std::string, bool> timeMap;

    for (OrderBookEntry &e : orders)
    {
        timeMap[e.timestamp.substr(0, 16)] = true; // only take the first 16 characters
    }

    // now flatten the map to a vector of strings
    for (auto const &e : timeMap)
    {
        timestamps.push_back(e.first);
    }

    return timestamps;
}
// END STUDENT CODE

/** return vector of Orders according to the sent filters*/
std::vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type,
                                                 std::string product,
                                                 std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

// START STUDENT CODE
/** return vector of Orders according to the sent filters; by minute only */
std::vector<OrderBookEntry> OrderBook::getOrderByMinute(OrderBookType type,
                                                        std::string product,
                                                        std::string timestamp)
{
    std::vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp.substr(0, 16) == timestamp) // only take the first 16 characters
        {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}
// END STUDENT CODE

double OrderBook::getHighPrice(std::vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
            max = e.price;
    }
    return max;
}

double OrderBook::getLowPrice(std::vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
            min = e.price;
    }
    return min;
}

std::string OrderBook::getEarliestTime()
{
    return orders[0].timestamp; // assume orders are sorted by timestamp
}

std::string OrderBook::getNextTime(std::string timestamp)
{
    std::string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    std::sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp);
}

std::vector<OrderBookEntry> OrderBook::matchAsksToBids(std::string product, std::string timestamp)
{
    // asks = orderbook.asks
    std::vector<OrderBookEntry> asks = getOrders(OrderBookType::ask,
                                                 product,
                                                 timestamp);
    // bids = orderbook.bids
    std::vector<OrderBookEntry> bids = getOrders(OrderBookType::bid,
                                                 product,
                                                 timestamp);

    // sales = []
    std::vector<OrderBookEntry> sales;

    // I put in a little check to ensure we have bids and asks
    // to process.
    if (asks.size() == 0 || bids.size() == 0)
    {
        std::cout << " OrderBook::matchAsksToBids no bids or asks" << std::endl;
        return sales;
    }

    // sort asks lowest first
    std::sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);
    // sort bids highest first
    std::sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc);
    // for ask in asks:
    std::cout << "max ask " << asks[asks.size() - 1].price << std::endl;
    std::cout << "min ask " << asks[0].price << std::endl;
    std::cout << "max bid " << bids[0].price << std::endl;
    std::cout << "min bid " << bids[bids.size() - 1].price << std::endl;

    for (OrderBookEntry &ask : asks)
    {
        //     for bid in bids:
        for (OrderBookEntry &bid : bids)
        {
            //         if bid.price >= ask.price # we have a match
            if (bid.price >= ask.price)
            {
                //             sale = new order()
                //             sale.price = ask.price
                OrderBookEntry sale{ask.price, 0, timestamp,
                                    product,
                                    OrderBookType::asksale};

                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }

                //             # now work out how much was sold and
                //             # create new bids and asks covering
                //             # anything that was not sold
                //             if bid.amount == ask.amount: # bid completely clears ask
                if (bid.amount == ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # can do no more with this ask
                    //                 # go onto the next ask
                    //                 break
                    break;
                }
                //           if bid.amount > ask.amount:  # ask is completely gone slice the bid
                if (bid.amount > ask.amount)
                {
                    //                 sale.amount = ask.amount
                    sale.amount = ask.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # we adjust the bid in place
                    //                 # so it can be used to process the next ask
                    //                 bid.amount = bid.amount - ask.amount
                    bid.amount = bid.amount - ask.amount;
                    //                 # ask is completely gone, so go to next ask
                    //                 break
                    break;
                }

                //             if bid.amount < ask.amount # bid is completely gone, slice the ask
                if (bid.amount < ask.amount &&
                    bid.amount > 0)
                {
                    //                 sale.amount = bid.amount
                    sale.amount = bid.amount;
                    //                 sales.append(sale)
                    sales.push_back(sale);
                    //                 # update the ask
                    //                 # and allow further bids to process the remaining amount
                    //                 ask.amount = ask.amount - bid.amount
                    ask.amount = ask.amount - bid.amount;
                    //                 bid.amount = 0 # make sure the bid is not processed again
                    bid.amount = 0;
                    //                 # some ask remains so go to the next bid
                    //                 continue
                    continue;
                }
            }
        }
    }
    return sales;
}

// START STUDENT CODE
// returns the mean, high, and low price of the orders
PriceStats OrderBook::getPriceStats(std::vector<OrderBookEntry> &orders)
{
    double sum_value = 0;
    double sum_amount = 0;
    double high = orders[0].price;
    double low = orders[0].price;

    for (OrderBookEntry &e : orders)
    {
        sum_amount += e.amount;
        sum_value += e.price * e.amount;
        if (e.price > high)
        {
            high = e.price;
        }
        if (e.price < low)
        {
            low = e.price;
        }
    }

    double mean = sum_value / sum_amount;

    return PriceStats{mean, high, low};
}

/** return a set number of succeeding timeframes from currentTime */
std::vector<std::string> OrderBook::getSelectedTimestamps(std::string currentTime, int size)
{
    std::string currentMinute = currentTime.substr(0, 16);                              // only take the first 16 characters
    std::vector<std::string> timestamps = getKnownTimestampsByMinute();                 // get all timestamps by minute
    auto index = std::lower_bound(timestamps.begin(), timestamps.end(), currentMinute); // find the first element that is not less than currentMinute

    if (index == timestamps.end()) // no match
    {
        return timestamps;
    }

    int currentIndex = std::distance(timestamps.begin(), index);
    int endIndex = std::min(currentIndex + size, static_cast<int>(timestamps.size()));

    std::vector<std::string> selected_timestamps;
    selected_timestamps.assign(timestamps.begin() + currentIndex, timestamps.begin() + endIndex); // assign the selected timestamps to the vector

    return selected_timestamps;
}

/** return vector of candlesticks by selected timestamps and by minute */
std::vector<Candlestick> OrderBook::getCandlesticks(std::string currentTime, std::string product, std::string type)
{
    OrderBookType OBEtype = OrderBookEntry::stringToOrderBookType(type);
    std::vector<Candlestick> candlesticks;
    double open = 0.0;

    std::vector<std::string> selected_timestamps = getSelectedTimestamps(currentTime, 20);

    for (std::string const &timestamp : selected_timestamps) // iterate through the selected timestamps
    {
        // get the orders by minute
        std::vector<OrderBookEntry> entries = getOrderByMinute(OBEtype, product, timestamp);
        if (entries.size() == 0)
        {
            continue;
        }

        PriceStats stats = getPriceStats(entries);
        double high = stats.high;
        double low = stats.low;
        double close = stats.mean;

        if (!candlesticks.empty()) // not the first iteration
        {
            open = candlesticks.back().close;
        }
        else
        {
            open = close;
        }

        // creat Candlestick object and add it to the vector
        Candlestick candle{open, high, low, close, timestamp};
        candlesticks.push_back(candle); // add the candlestick to the vector
    }
    return candlesticks;
}

/** return vector of bars by selected timestamps and by minute */
std::vector<Bar> OrderBook::getBars(std::string currentTime, std::string product)
{
    std::vector<Bar> bars;
    std::vector<std::string> selected_timestamps = getSelectedTimestamps(currentTime, 20);

    for (std::string const &timestamp : selected_timestamps) // iterate through the selected timestamps
    {
        // get the ask orders by minute
        std::vector<OrderBookEntry> ask_entries = getOrderByMinute(OrderBookType::ask, product, timestamp);

        // get the bid orders by minute
        std::vector<OrderBookEntry> bid_entries = getOrderByMinute(OrderBookType::bid, product, timestamp);

        if (ask_entries.size() == 0 || bid_entries.size() == 0)
        {
            continue;
        }

        double askHighPrice = getHighPrice(ask_entries);
        double bidHighPrice = getHighPrice(bid_entries);
        double askLowPrice = getLowPrice(ask_entries);
        double bidLowPrice = getLowPrice(bid_entries);

        // create Bar object and add it to the vector
        Bar bar{askHighPrice, bidHighPrice, askLowPrice, bidLowPrice, timestamp};
        bars.push_back(bar); // add the bar to the vector
    }
    return bars;
}

/** return vector of volumes by selected timestamps and by minute */
std::vector<Volume> OrderBook::getVolume(std::string currentTime, std::string product, std::string type)
{
    OrderBookType OBEtype = OrderBookEntry::stringToOrderBookType(type);
    std::vector<Volume> volumes;
    double amount = 0.0;

    std::vector<std::string> selected_timestamps = getSelectedTimestamps(currentTime, 60);

    for (std::string const &timestamp : selected_timestamps) // iterate through the selected timestamps
    {
        std::vector<OrderBookEntry> entries = getOrderByMinute(OBEtype, product, timestamp);
        if (entries.size() == 0)
        {
            continue;
        }

        for (OrderBookEntry &e : entries) // iterate through the orders
        {
            amount += e.amount; // add the amount to the total amount
        }

        // create Volume object and add it to the vector
        Volume vol{amount, timestamp};
        volumes.push_back(vol); // add the volume to the vector
        amount = 0.0;           // reset amount
    }
    return volumes;
}
// END STUDENT CODE