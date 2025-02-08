#pragma once

#include <vector>
#include <iostream>
#include <string>

/** CLASS FOR VOLUME GRAPH */
class Volume
{
public:
  Volume(); // default constructor
  Volume(double _amount,
         std::string _timestamp); // constructor

  double amount;         // amount/volume
  std::string timestamp; // timestamp

  void printData(); // prints the data within the volume

  std::pair<double, double> getHighLow(const std::vector<Volume> &volumes);                    // gets the highest and lowest amount of the volumes vector
  double mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh); // maps the value from one range to another
  void drawGraph(const std::vector<Volume> &volumes, std::string product, std::string offer);  // draws the graph of the volume
};