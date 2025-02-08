#pragma once

#include <vector>
#include <iostream>
#include <string>

/** CLASS FOR CANDLESTICK GRAPH */
class Candlestick
{
public:
  Candlestick(); // default constructor
  Candlestick(double _open,
              double _high,
              double _low,
              double _close,
              std::string _timestamp); // constructor

  double open;           // open value; price * amount; previous close value
  double high;           // highest price
  double low;            // lowest price
  double close;          // close value; price * amount; current value
  std::string timestamp; // timestamp

  void printData();                                                                                          // prints the data within the candlestick
  static std::pair<double, double> getHighLow(const std::vector<Candlestick> &candlesticks);                 // gets the highest and lowest price of the candlesticks vector
  double mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh);               // maps the value from one range to another
  void drawCandlestick(std::vector<Candlestick> candlesticks, std::string product_in, std::string offer_in); // draws the graph of the candlestick
};