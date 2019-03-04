//
// Created by sf on 6/28/17.
//

#ifndef XPLATDEV_LOGGER_H
#define XPLATDEV_LOGGER_H

#ifdef __ANDROID__

#include <android/log.h>

#define LOG_TAG "cppImSDL"

#define alogF(...) __android_log_print(ANDROID_LOG_FATAL,   LOG_TAG, __VA_ARGS__)
#define alogE(...) __android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)
#define alogW(...) __android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)
#define alogI(...) __android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define alogD(...) __android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define alogV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)

#else
#include <iostream>
#endif

#include <sstream>

namespace _Logger 
{
    enum Severity 
    {
        LOG_VERBOSE = 0,
        LOG_DEBUG,
        LOG_INFO,
        LOG_WARN,
        LOG_ERROR,
        LOG_FATAL
    };

    class Logger 
    {
      private:
        std::stringstream logbuf;
        Severity severity;

        std::string sevStr()
        {
            switch(severity)
            {
                case LOG_VERBOSE:   return "[VERBOSE] ";
                case LOG_DEBUG:     return "[DEBUG] ";
                case LOG_INFO:      return "[INFO] ";
                case LOG_WARN:      return "[WARN] ";
                case LOG_ERROR:     return "[ERROR] ";
                case LOG_FATAL:     return "[FATAL] ";
                default:            return "[unknown err level] ";
            }
        }

#ifdef __ANDROID__
        int getPrio()
        {
            switch(severity)
            {
                case LOG_VERBOSE:   return ANDROID_LOG_VERBOSE;
                case LOG_DEBUG:     return ANDROID_LOG_DEBUG;
                case LOG_INFO:      return ANDROID_LOG_INFO;
                case LOG_WARN:      return ANDROID_LOG_WARN;
                case LOG_ERROR:     return ANDROID_LOG_ERROR;
                case LOG_FATAL:     return ANDROID_LOG_FATAL;
                default:            return ANDROID_LOG_DEFAULT;
            }
        }
#else
        static std::ostream& getStream(Severity s)
        {
            if (s < LOG_ERROR)  
            {
                return std::cout;
            }
            else
            {
                return std::cerr;
            }
        }
#endif

    public:
        static Severity& minSeverity()
        {
            static Severity minSeverity = LOG_DEBUG; // LOG_VERBOSE ?
            return minSeverity;
        }

        Logger(Severity s) : severity(s)
        {
        }
        
        ~Logger()
        {
#ifdef __ANDROID__
            __android_log_print(getPrio(), LOG_TAG, "%s", logbuf.str().c_str());
#else
            getStream(severity) << sevStr() << logbuf.str() << std::endl;
#endif
        }
        
        std::stringstream& log()
        {
            return logbuf;
        }

    };  // class Logger 
};

#define Log(LOGLEVEL) _Logger::Logger(_Logger::LOGLEVEL).log()

#endif //XPLATDEV_LOGGER_H
