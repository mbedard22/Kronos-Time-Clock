// Copyright

#include <iostream>
#include <fstream>
#include <string>
#include <boost/regex.hpp>
#include "boost/date_time/gregorian/gregorian.hpp"
#include "boost/date_time/posix_time/posix_time.hpp"

using std::cout;
using std::endl;
using std::string;
using std::getline;
using std::stoi;
using boost::regex;
using boost::smatch;
using boost::regex_match;
using boost::gregorian::from_string;
using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;

int main(int argc, const char* argv[]) {
     std::ifstream myFile(argv[1]);
     if (myFile.fail()) {
          cout << "could not open file" << endl;
          exit(1);
     }
     string outFile(argv[1]);
     outFile += ".rpt";
     std::ofstream output(outFile);

     regex startup("([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+):([0-9]+): \\(log.c.166\\) server started.*");//NOLINT
     regex success("([0-9]+)-([0-9]+)-([0-9]+) ([0-9]+):([0-9]+):([0-9]+).([0-9]+):INFO:oejs.AbstractConnector:Started SelectChannelConnector@0.0.0.0:9080.*");//NOLINT

     string line;
     int i = 1;
     ptime t;
     ptime t2;
     date d;
     smatch sm;
     date tempD;
     bool repeat = false;
     while (getline(myFile, line)) {
          if (regex_match(line, sm, startup)) {
               if (repeat) {
                    output << "**** Incomplete boot ****\n" << endl;
               }
               output << "=== Device Boot ===" << endl;
               output << i << "(" << argv[1] << "): ";
               output << (sm[1]) << '-' << (sm[2]) << '-' << (sm[3])
               << ' ' << (sm[4]) << ':' << (sm[5]) << ':' << (sm[6])
               << ' ' << "Boot Start" << endl;
               tempD = boost::gregorian::from_undelimited_string(sm[1]+sm[2]+sm[3]);//NOLINT
               d = tempD;
               ptime tempT = boost::posix_time::from_iso_string(sm[1]+sm[2]+sm[3]+'T'+sm[4] + sm[5] + sm[6]);//NOLINT
               t = tempT;
               repeat = true;
          } else if (regex_match(line, sm, success)) {
               output << i << "(" << argv[1] << "): ";
               output << (sm[1]) << '-' << (sm[2]) << '-' << (sm[3])
               << ' ' << (sm[4]) << ':' << (sm[5]) << ':' << (sm[6])
               << ' ' << "Boot Completed" << endl;
               tempD = boost::gregorian::from_undelimited_string(sm[1]+sm[2]+sm[3]);//NOLINT
               d = tempD;
               ptime tempT = boost::posix_time::from_iso_string(sm[1]+sm[2]+sm[3]+'T'+sm[4] + sm[5] + sm[6]);//NOLINT
               t2 = tempT;
               time_duration td = t2-t;
               output <<"\tBoot Time: " << td.total_milliseconds() << "ms" << endl << endl;//NOLINT
               repeat = false;
          }
          i += 1;
     }
     return 0;
}
