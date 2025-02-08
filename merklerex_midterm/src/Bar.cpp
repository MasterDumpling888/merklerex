#include "Bar.h"
#include "ColorCodes.h"
#include <iostream>
#include <iomanip>

Bar::Bar()
{
}
Bar::Bar(double _askHighPrice,
         double _bidHighPrice,
         double _askLowPrice,
         double _bidLowPrice,
         std::string _timestamp)
    : askHighPrice(_askHighPrice),
      bidHighPrice(_bidHighPrice),
      askLowPrice(_askLowPrice),
      bidLowPrice(_bidLowPrice),
      timestamp(_timestamp)
{
}

// prints the data of the bar
void Bar::printData()
{
  std::cout << GREEN_BG << "Time: " << timestamp.substr(11, 5) << RESET << CYAN << " Ask High Price: " << askHighPrice << BLUE << " Ask Low Price: " << askLowPrice << MAGENTA << " Bid High Price: " << bidHighPrice << RESET << " Bid Low Price: " << bidLowPrice << std::endl;
}

// gets the highest and lowest price of the bars vector
std::pair<double, double> Bar::getHighLow(const std::vector<Bar> &bars)
{
  double highestAsk = bars[0].askHighPrice;
  double highestBid = bars[0].bidHighPrice;
  double lowestAsk = bars[0].askLowPrice;
  double lowestBid = bars[0].bidLowPrice;

  for (const Bar &bar : bars)
  {
    if (bar.askHighPrice > highestAsk)
    {
      highestAsk = bar.askHighPrice;
    }
    if (bar.bidHighPrice > highestBid)
    {
      highestBid = bar.bidHighPrice;
    }
    if (bar.askLowPrice < lowestAsk)
    {
      lowestAsk = bar.askLowPrice;
    }
    if (bar.bidLowPrice < lowestBid)
    {
      lowestBid = bar.bidLowPrice;
    }
  }

  return {std::max(highestAsk, highestBid), std::min(lowestAsk, lowestBid)};
}

/* Similar to P5.js "map" function; performs linear interpolation */
// maps the value from one range to another
double Bar::mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh)
{
  return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void Bar::drawBarGraph(std::vector<Bar> bars, std::string product)
{

  // Prints a list of the Bars data: FOR VALIDATION
  for (Bar &bar : bars)
  {
    bar.printData();
  }

  // Print the header
  std::string line;
  for (int i = 0; i < bars.size(); i++)
  {
    line += "───────";
  }

  std::cout << "             │" << YELLOW << " Bar Chart for " << product << CYAN << " ask" << MAGENTA << " bid" << RESET << std::endl
            << "─────────────┼──" << line << std::endl;

  // get the highest and lowest price of the bars vector
  std::pair<double, double> highLow = getHighLow(bars);

  // assign pair to corresponding variables
  double highestPrice = highLow.first;
  double lowestPrice = highLow.second;
  const int numRows = 25; // number of rows to be plotted

  for (int row = numRows; row >= 0; --row)
  {
    // Print the price on the y-axis
    double price = mapValue(row, 0, numRows, lowestPrice, highestPrice);
    std::cout << std::setw(12) << price << " │ ";

    // Print the bars
    for (const Bar &bar : bars)
    {
      // map the min and max prices to the number of rows
      int askRowHigh = mapValue(bar.askHighPrice, lowestPrice, highestPrice, 0, numRows); // maps the askHighPrice to the number of rows
      int askRowLow = mapValue(bar.askLowPrice, lowestPrice, highestPrice, 0, numRows);   // maps the askLowPrice to the number of rows
      int bidRowHigh = mapValue(bar.bidHighPrice, lowestPrice, highestPrice, 0, numRows); // maps the bidHighPrice to the number of rows
      int bidRowLow = mapValue(bar.bidLowPrice, lowestPrice, highestPrice, 0, numRows);   // maps the bidLowPrice to the number of rows

      // if the row is between the high and low prices, print a bar
      std::string askBar = (row <= askRowHigh && row >= askRowLow) ? " █" : "  ";
      std::string bidBar = (row <= bidRowHigh && row >= bidRowLow) ? " █ " : "   ";

      // print the askBar, bidBar, then add two spaces between each timeframe
      std::cout << CYAN << askBar << MAGENTA << bidBar << RESET << "  ";
    }

    std::cout << std::endl;
  }

  // Print the x-axis
  std::cout << "─────────────┼──" << line << std::endl
            << "             │";

  // Print the timestamps
  for (Bar &bar : bars)
  {
    // get substring of timestamp to print only HH:MM
    std::cout << " " << std::setw(5) << std::setfill('0')
              << bar.timestamp.substr(11, 5) << " ";
  }

  // padding
  std::cout << std::endl
            << std::endl;
}