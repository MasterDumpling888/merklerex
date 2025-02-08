#include "Candlestick.h"
#include "ColorCodes.h"
#include <iostream>
#include <iomanip>

Candlestick::Candlestick()
{
}
Candlestick::Candlestick(double _open,
                         double _high,
                         double _low,
                         double _close,
                         std::string _timestamp)
    : open(_open),
      high(_high),
      low(_low),
      close(_close),
      timestamp(_timestamp)
{
}

// prints the data of the candlestick
void Candlestick::printData()
{
  std::cout << GREEN_BG << "Timestamp: " << timestamp.substr(11, 5) << RESET << CYAN << " Open: " << open
            << MAGENTA << " High: " << high << BLUE << " Low: " << low << RESET << " Close: " << close << std::endl;
}

// gets the highest and lowest price of the candlesticks vector
std::pair<double, double> Candlestick::getHighLow(const std::vector<Candlestick> &candlesticks)
{
  double minPrice = candlesticks[0].low;
  double maxPrice = candlesticks[0].high;

  for (const Candlestick &candlestick : candlesticks)
  {
    if (candlestick.low < minPrice)
    {
      minPrice = candlestick.low;
    }
    if (candlestick.high > maxPrice)
    {
      maxPrice = candlestick.high;
    }
  }

  return {minPrice, maxPrice};
}

/* Similar to P5.js "map" function; performs linear interpolation */
// maps the value from one range to another
double Candlestick::mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh)
{
  return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

void Candlestick::drawCandlestick(std::vector<Candlestick> candlesticks, std::string product_in, std::string offer_in)
{
  // Prints list of Candlesticks data: FOR VALIDATION
  for (Candlestick &candle : candlesticks)
  {
    candle.printData();
  }

  // print header
  std::string line;
  for (int i = 0; i < candlesticks.size(); i++)
  {
    line += "───────";
  }

  std::cout << "             │" << BOLDBLUE << " Candlestick Chart for " << product_in << " " << YELLOW << offer_in << RESET << std::endl
            << "─────────────┼──" << line << std::endl;

  // get highest and lowest price of candlesticks
  std::pair<double, double> prices = getHighLow(candlesticks);

  // assign pair to corresponding variables
  double lowestPrice = prices.first;
  double highestPrice = prices.second;
  const int numRows = 25; // number of rows to be plotted

  // print candlestick chart; rows are prices, columns are timestamps
  for (int row = numRows; row >= 0; --row) // loop through rows
  {
    // print y-axis; prices
    double price = mapValue(row, 0, numRows, lowestPrice, highestPrice);

    /** setFill() to ensure that price will have spaces instead of zeros to the left of the first digit
     * setw() to ensure that price will have 12 spaces*/
    std::cout << std::setfill(' ') << std::setw(12) << price << " │ ";

    // print candles in candlesticks vector
    for (Candlestick &candle : candlesticks)
    {
      // set color of candle; red if open > close, green if close > open
      std::string color = (candle.open > candle.close) ? RED : GREEN;

      // gets the min and max open/close of the candle
      double minPrice = std::min(candle.open, candle.close);
      double maxPrice = std::max(candle.open, candle.close);

      // map the min and max prices to the number of rows
      int minPriceRow = mapValue(minPrice, lowestPrice, highestPrice, 0, numRows); // maps min open/close to number of rows
      int maxPriceRow = mapValue(maxPrice, lowestPrice, highestPrice, 0, numRows); // maps max open/close to number of rows
      int highRow = mapValue(candle.high, lowestPrice, highestPrice, 0, numRows);  // maps high to number of rows
      int lowRow = mapValue(candle.low, lowestPrice, highestPrice, 0, numRows);    // maps low to number of rows

      // plotting candlestick
      if (row >= minPriceRow && row <= maxPriceRow) // row is within open/close; candlestick
      {
        std::cout << color << " ███ "; // within open/close range - print candle
      }
      else if ((row >= highRow && row <= lowRow) || (row <= highRow && row >= lowRow)) // row is within high/low; wick
      {
        std::cout << color << "  ┃  "; // outside open/close range - print stick
      }
      else // no candle. print blank spaces
      {
        std::cout << "     ";
      }
      std::cout << RESET << "  "; // space between candles
    }

    std::cout << std::endl;
  }

  // print x-axis
  // print line separating candlesticks from timestamps
  std::cout << "─────────────┼──" << line << std::endl
            << "Time in HH:MM│";

  // print timestamps
  for (Candlestick &candle : candlesticks)
  {
    // get substring of timestamp to print only HH:MM
    std::cout << " " << std::setw(5) << std::setfill('0')
              << candle.timestamp.substr(11, 5) << " ";
  }

  // padding
  std::cout << std::endl
            << std::endl;
}