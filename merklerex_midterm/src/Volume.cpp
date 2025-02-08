#include "Volume.h"
#include "ColorCodes.h"
#include <iostream>
#include <iomanip>

Volume::Volume()
{
}
Volume::Volume(double _amount,
               std::string _timestamp)
    : amount(_amount),
      timestamp(_timestamp)
{
}

// prints the data of the volume
void Volume::printData()
{
  std::cout << GREEN_BG << "Timestamp: " << timestamp.substr(11, 5) << RESET << CYAN << " Volume: " << amount << RESET << std::endl;
}

// gets the highest and lowest price of the volumes vector
std::pair<double, double> Volume::getHighLow(const std::vector<Volume> &volumes)
{
  double highestVolume = volumes[0].amount;
  double lowestVolume = volumes[0].amount;

  for (const Volume &volume : volumes)
  {
    if (volume.amount > highestVolume)
    {
      highestVolume = volume.amount;
    }
    if (volume.amount < lowestVolume)
    {
      lowestVolume = volume.amount;
    }
  }
  return {highestVolume, lowestVolume};
}

/* Similar to P5.js "map" function; performs linear interpolation */
// maps the value from one range to another
double Volume::mapValue(double value, double fromLow, double fromHigh, double toLow, double toHigh)
{
  return (value - fromLow) / (fromHigh - fromLow) * (toHigh - toLow) + toLow;
}

void Volume::drawGraph(const std::vector<Volume> &volumes, std::string product, std::string offer)
{

  // Print the header
  std::string line;
  for (int i = 0; i < volumes.size(); i++)
  {
    line += "───";
  }

  std::cout << "             │" << CYAN << " Volume Chart for " << product << YELLOW << " " << offer << RESET << std::endl
            << "─────────────┼──" << line << std::endl;

  // Get the highest and lowest volume
  std::pair<double, double> highLow = getHighLow(volumes);

  // assign the highest and lowest volume to variables
  double highestVolume = highLow.first;
  double lowestVolume = highLow.second;
  const int numRows = 30; // number of rows in the graph

  // Print the rows
  for (int row = numRows; row >= 0; --row)
  {

    // print the y-axis; volumes
    double volume = mapValue(row, 0, numRows - 1, lowestVolume, highestVolume);

    /** setFill() to ensure that price will have spaces instead of zeros to the left of the first digit
     * setw() to ensure that price will have 12 spaces*/
    std::cout << std::setfill(' ') << std::setw(12) << volume << " │";

    // print the volumes
    for (const Volume &volume : volumes)
    {
      // map the volume to the row
      double amount = mapValue(volume.amount, lowestVolume, highestVolume, 0, numRows);

      if (amount >= row && amount < row + 1) // if the volume is in the row
      {
        std::cout << BLUE << " ◆ ";
      }
      else
      {
        std::cout << "   ";
      }
    }
    std::cout << RESET << " " << std::endl;
  }

  // print the x-axis; timestamps
  std::cout << "─────────────┼──" << line << std::endl
            << " Time in :SS │";

  // print the timestamps on the x-axis in seconds
  int i = 0;
  for (const Volume &volume : volumes)
  {
    if (i % 2 == 0)
    {
      std::cout << std::setw(3) << std::setfill('0')
                << volume.timestamp.substr(13, 3);
    }
    else
    {
      std::cout << "   ";
    }
    i++;
  }

  // padding
  std::cout << std::endl
            << std::endl;
}