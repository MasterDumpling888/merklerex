#pragma once

#include <vector>
#include <iostream>
#include <string>

/** CLASS FOR BAR GRAPH */
class Bar
{
public:
  Bar(); // default constructor
  Bar(double _askHighPrice,
      double _bidHighPrice,
      double _askLowPrice,
      double _bidLowPrice,
      std::string _timestamp); // constructor

  double askHighPrice;   // ask high price
  double bidHighPrice;   // bid high price
  double askLowPrice;    // ask low price
  double bidLowPrice;    // bid low price
  std::string timestamp; // timestamp

  void printData();                                                   // prints the data within the bar
  std::pair<double, double> getHighLow(const std::vector<Bar> &bars); // gets the highest and lowest price of the bars vector
  double mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh);
  void drawBarGraph(std::vector<Bar> bars, std::string product); // draws the graph of the bar
};