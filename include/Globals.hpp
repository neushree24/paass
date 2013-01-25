/*! \file Globals.hpp
  \brief constant parameters used in pixie16 analysis
  
*/

#ifndef __GLOBALS_HPP_
#define __GLOBALS_HPP_

#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdint.h>

#include "pixie16app_defs.h"

#include "Exceptions.hpp"


/** "Constant" constants, i.e. those who won't change going from different 
 * verison (revision) of board, some magic numbers used in code etc. 
 * For "variable" constants i.e. revision related or experiment related see
 * Globals class.*/
namespace pixie {
    typedef uint32_t word_t; //< a pixie word
    typedef uint16_t halfword_t; //< a half pixie word
    typedef uint32_t bufword_t; //< word in a pixie buffer

    /** buffer and module data are terminated with a "-1" value
    *   also used to indicate when a quantity is out of range or peculiar
    *   this should theoretically be the same as UINT_MAX in climits header
    */
    const pixie::word_t U_DELIMITER = (pixie::word_t)-1;

    /** THIS SHOULD NOT BE SET LARGER THAN 1,000,000
     * this defines the maximum amount of data that will be
     * received in a spill. 
     */
    const unsigned int TOTALREAD = 1000000;

    /** an arbitrary vsn used to pass clock data */
    const pixie::word_t clockVsn = 1000; 
/*
#ifdef REVF
    const double clockInSeconds = 8e-9; //< one pixie clock is 8 ns
    const double adcClockInSeconds = 4e-9; //< one ADC clock is 4 ns
    const double filterClockInSeconds = 8e-9; //< one filter clock is 8 ns
#else 
    const double clockInSeconds = 10e-9; //< one pixie clock is 10 ns
    const double adcClockInSeconds = 10e-9; //< one ADC clock is 10 ns
    const double filterClockInSeconds = 10e-9; //< one filter clock is 10 ns
#endif

    // The time widht of an event in seconds.
    const double eventInSeconds = 100e-6;
    // The time width of an event in units of pixie16 clock ticks
    const int eventWidth = eventInSeconds / pixie::clockInSeconds;

    // Energies from pixie16 are contracted by this number.
    // Was 2.0 for older LeRIBBS
    // changed to 4.0 for LeRIBBS experiment (93Br)
    // Set to 1.0, looks like ADC range is 16K
    const double energyContraction = 1.0; 
*/
   
    const size_t numberOfChannels = 16; //< number of channels in a module
};


namespace readbuff {
    const int STATS = -10;
    const int ERROR = -100;
}

/** Some common string conversion operations */
namespace strings {
    /** Converts string to double or throws an exception if not 
        * succesful */
    inline double to_double (std::string s) { 
        std::istringstream iss(s);
        double value;
        if (!(iss >> value)) {
            std::stringstream ss;
            ss << "strings::to_double: Could not convert string '" 
               << s << "' to double" << std::endl;
            throw ConversionException(ss.str());
        }
        return value;
    }

    /** Converts string to int or throws an exception if not 
        * succesful */
    inline int to_int (std::string s) {
        std::istringstream iss(s);
        int value;
        if (!(iss >> value)) {
            std::stringstream ss;
            ss << "strings::to_int: Could not convert string '" 
               << s << "' to int" << std::endl;
            throw ConversionException(ss.str());
        }
        return value;
    }

    /** Converts string to bool (True, true, 1 and False, false, 0) are 
      * accepted; throws an exception if not succesful. Notice tolower
      * will work only with ascii, not with utf-8, but shouldn't be a 
      * problem for true and false words. */
    inline bool to_bool (std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        if (s == "true" || s == "1")
            return true;
        else if (s == "false" || s == "0")
            return false;
        else {
            std::stringstream ss;
            ss << "strings::to_bool: Could not convert string '" 
               << s << "' to bool" << std::endl;
            throw ConversionException(ss.str());
        }
    }

    /** Tokenizes the string, splitting it on a given delimiter.
      * delimiters are removed from returned vector of tokens.*/
    inline std::vector<std::string> tokenize(std::string str,
                                             std::string delimiter) {
        std::string temp;
        std::vector<std::string> tokenized;
        while (str.find(delimiter) != std::string::npos) {
            size_t pos = str.find(delimiter);
            temp = str.substr(0, pos);
            str.erase(0, pos + 1);
            tokenized.push_back(temp);
        }
        tokenized.push_back(str);
        return tokenized;
    }
};


/** Singleton class holding global parameters.*/
class Globals {
    public:
        /** Returns only instance of TreeCorrelator class.*/
        static Globals* get();
        ~Globals();

        double clockInSeconds() const {
            return clockInSeconds_;
        }

        double adcClockInSeconds() const {
            return adcClockInSeconds_;
        }

        double filterClockInSeconds() const {
            return filterClockInSeconds_;
        }

        double eventInSeconds() const {
            return eventInSeconds_;
        }

        int eventWidth() const {
            return eventWidth_;
        }

        double energyContraction() const {
            return energyContraction_;
        }

        unsigned int maxWords() const {
            return maxWords_;
        }


        std::string revision() const {
            return revision_;
        }

        /** Returns joined path to the passed filename by
         * adding the configPath_
         * This is temporary solution as long as there are some 
         * files not incorporated into Config.xml
         */
        std::string configPath(std::string fileName) {
            std::stringstream ss;
            ss << configPath_ << fileName;
            return ss.str();
        }

    private:
        /** Make constructor, copy-constructor and operator =
         * private to complete singleton implementation.*/
        Globals();
        /* Do not implement*/
        Globals(Globals const&);
        void operator=(Globals const&);
        static Globals* instance;

        void SanityCheck();

        std::string revision_;
        double clockInSeconds_;
        double adcClockInSeconds_;
        double filterClockInSeconds_;
        double eventInSeconds_;
        int eventWidth_;
        double energyContraction_;
        unsigned int maxWords_;
        std::string configPath_;
};


#endif // __GLOBALS_HPP_
