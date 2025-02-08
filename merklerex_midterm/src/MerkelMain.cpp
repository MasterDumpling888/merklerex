#include "MerkelMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "ColorCodes.h"
#include "CSVReader.h"
#include <stdlib.h>
// please use this if you're running the program on Windows
// #include <windows.h>

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 10);

    while (true)
    {
        printMenu();
        input = getUserOption(10); // pass the number of options in the menu
        processUserOption(input);
    }
}

void MerkelMain::printMenu()
{
    std::cout << MAGENTA << "=============================" << std::endl;
    std::vector<std::string> options = {
        "Print help",
        "Print exchange stats",
        "Make an offer",
        "Make a bid",
        "Print wallet",
        "Print candlestick",
        "Print ask/bid bar graph",
        "Print volume graph",
        "Continue to next timeframe",
        "Exit"};
    printOptions(options);
    std::cout << MAGENTA << "=============================" << std::endl;
    std::cout << GREEN_BG << "Current time is: " << currentTime << "\033[0m" << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{

    std::cout << "Welcome to the Market Stats!" << std::endl;
    std::cout << "Please enter either 'ask' or 'bid' to see their stats!" << std::endl; // TODO: can use printOptions() and getUserOption() here?
    std::string input;
    std::getline(std::cin, input);

    for (std::string const &p : orderBook.getKnownProducts())
    {
        std::vector<OrderBookEntry> entries;
        if (input == "ask")
        {
            entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        }
        else if (input == "bid")
        {
            entries = orderBook.getOrders(OrderBookType::bid, p, currentTime);
        }
        else
        {
            std::cout << "Invalid input!" << std::endl;
            break;
        }

        std::cout << "======= Product: " << p << " =======" << std::endl;
        std::cout << "===================================" << std::endl;
        std::cout << "Offers seen: " << entries.size() << std::endl;
        std::cout << "Max price: " << orderBook.getHighPrice(entries) << std::endl;
        std::cout << "Min price: " << orderBook.getLowPrice(entries) << std::endl;
    }
}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product, price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask);
            obe.username = "simuser";
            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid);
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
            }
            else
            {
                std::cout << "Wallet has insufficient funds . " << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void MerkelMain::printWallet()
{
    std::cout << wallet.toString() << std::endl;
}

// START STUDENT CODE
void MerkelMain::printCandlestick() // prints candlestick graph of values of chosen product and offer type
{
    // ask user to choose product - list all known products and validate user input
    std::vector<std::string> products = orderBook.getKnownProducts();
    std::cout << "Please choose the product you want to graph:" << std::endl;
    printOptions(products);
    int productIndex = getUserOption(products.size()) - 1;
    std::string product_in = products[productIndex];

    // ask user to choose type (ask/bid) - validate user input
    std::vector<std::string> offers = {"ask", "bid"};
    std::cout << "Please choose the offer type you want to graph:" << std::endl;
    printOptions(offers);
    int offersIndex = getUserOption(offers.size()) - 1;
    std::string offer_in = offers[offersIndex];

    // get the candlesticks for the chosen product and offer type
    std::vector<Candlestick> candlesticks = orderBook.getCandlesticks(currentTime, product_in, offer_in);

    if (candlesticks.size() == 0)
    {
        std::cout << "No candlesticks found for this product and offer type." << std::endl;
        return;
    }

    // draw the candlestick
    candlestick.drawCandlestick(candlesticks, product_in, offer_in);
}

void MerkelMain::printBar() // prints bar graph high/low prices for ask and bid
{
    // ask user to choose product - list all known products and validate user input
    std::vector<std::string> products = orderBook.getKnownProducts();
    std::cout << "Please choose the product you want to graph:" << std::endl;
    printOptions(products);
    int productIndex = getUserOption(products.size()) - 1;
    std::string product_in = products[productIndex];

    // get the bars for the chosen product
    std::vector<Bar> bars = orderBook.getBars(currentTime, product_in);

    if (bars.size() == 0)
    {
        std::cout << "No bars found for this product." << std::endl;
        return;
    }

    // draw the bar graph
    bar.drawBarGraph(bars, product_in);
}

void MerkelMain::printVolumeGraph() // prints dot graph of amount of chosen product and offer type
{
    // ask user to choose product - list all known products and validate user input
    std::vector<std::string> products = orderBook.getKnownProducts();
    std::cout << "Please choose the product you want to graph:" << std::endl;
    printOptions(products);
    int productIndex = getUserOption(products.size()) - 1;
    std::string product_in = products[productIndex];

    // ask user to choose type (ask/bid) - validate user input
    std::vector<std::string> offers = {"ask", "bid"};
    std::cout << "Please choose the offer type you want to graph:" << std::endl;
    printOptions(offers);
    int offersIndex = getUserOption(offers.size()) - 1;
    std::string offer_in = offers[offersIndex];

    // get the volumes for the chosen product and offer type
    std::vector<Volume> volumes = orderBook.getVolume(currentTime, product_in, offer_in);

    if (volumes.size() == 0)
    {
        std::cout << "No volumes found for this product." << std::endl;
        return;
    }

    // draw the volume graph
    volume.drawGraph(volumes, product_in, offer_in);
}
// END STUDENT CODE
void MerkelMain::gotoNextTimeframe()
{
    std::cout << GREEN << "Going to next time frame..." << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Matching... " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry &sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                // update the wallet
                wallet.processSale(sale);
            }
        }
    }

    currentTime = orderBook.getNextTime(currentTime);
}

int MerkelMain::getUserOption(int size)
{
    int userOption = 0;
    std::string line;
    while (true)
    {
        std::cout << "Type in menu choice (1-" << size << "): ";
        std::getline(std::cin, line);

        try
        {
            userOption = std::stoi(line);
            if (userOption >= 1 && userOption <= size)
            {
                break;
            }
            else
            {
                std::cerr << "\033[1;31m"
                          << "Invalid input! Please enter a number between 1 and " << size << "."
                          << "\033[0m" << std::endl;
                continue;
            };
        }
        catch (const std::exception &e)
        {
            std::cout << "ERROR! " << e.what() << std::endl
                      << "Press ENTER/RETURN to continue... " << std::endl;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    // START STUDENT CODE
    // map user input to the corresponding void function
    std::map<int, void (MerkelMain::*)()> menu;

    menu[1] = &MerkelMain::printHelp;
    menu[2] = &MerkelMain::printMarketStats;
    menu[3] = &MerkelMain::enterAsk;
    menu[4] = &MerkelMain::enterBid;
    menu[5] = &MerkelMain::printWallet;
    menu[6] = &MerkelMain::printCandlestick;
    menu[7] = &MerkelMain::printBar;
    menu[8] = &MerkelMain::printVolumeGraph;
    menu[9] = &MerkelMain::gotoNextTimeframe;

    if (userOption == 10)
    {
        std::cout << "Exiting...Goodbye! ☺" << std::endl;
        exit(0);
    }
    else
    {
        (this->*(menu[userOption]))();
    }
    // END STUDENT CODE
}

// START STUDENT CODE
void MerkelMain::printOptions(const std::vector<std::string> &options) // prints the vector of options for user to choose from
{
    for (int i = 0; i < options.size(); i++)
    {
        std::cout << CYAN << i + 1 << ": " << options[i] << RESET << std::endl;
    }
}
// END STUDENT CODE