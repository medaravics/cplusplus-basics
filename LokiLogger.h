#ifndef LOKI_LOGGER_H
#define LOKI_LOGGER_H

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <sys/time.h>
#include <curl/curl.h>

class LokiLogger
{
private:
    std::string lokiUrl;
    std::string jobLabel;

    static bool curlInitialized;
    static void globalCurlInitOnce()
    {
        if (!curlInitialized) {
            curl_global_init(CURL_GLOBAL_ALL);
            curlInitialized = true;
        }
    }

    std::string getCurrentTimeInNanoseconds()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        long long nanos = ((long long)tv.tv_sec * 1000000000LL) + ((long long)tv.tv_usec * 1000LL);
        std::ostringstream oss;
        oss << nanos;
        return oss.str();
    }

    std::string escapeJson(const std::string& input)
    {
        std::string output;
        for (size_t i = 0; i < input.length(); ++i)
        {
            char c = input[i];
            switch (c)
            {
                case '\"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b";  break;
                case '\f': output += "\\f";  break;
                case '\n': output += "\\n";  break;
                case '\r': output += "\\r";  break;
                case '\t': output += "\\t";  break;
                default:
                    if ('\x00' <= c && c <= '\x1f') {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        output += buf;
                    } else {
                        output += c;
                    }
            }
        }
        return output;
    }

public:
    LokiLogger(){   } //enabling default constructor
    LokiLogger(const std::string& url, const std::string& job)
        : lokiUrl(url), jobLabel(job)
    {
        globalCurlInitOnce();  // ensures only one global init
    }

    ~LokiLogger()
    {
        // Do NOT call curl_global_cleanup() here!
        // Only call it once at end of main program
    }
    
    void init(const std::string& url, const std::string& job)
    {
        lokiUrl = url;
        jobLabel = job;
        globalCurlInitOnce();  // ensures only one global init
    }
    
     static void shutdown()
    {
        if (curlInitialized) {
            curl_global_cleanup();
            curlInitialized = false;
        }
    }
    
    std::string getLogLevelLabel(int level)
    {
      switch (level)
      {
          case 1: return "fatal";
          case 2: return "error";
          case 3: return "warn";
          case 4: return "info";
          case 5: return "debug";
          default: return "unknown";
      }
    }
	
	void fatal(const std::string& message)
    {
		log(message, 1);
    }
	
	void error(const std::string& message)
    {
		log(message, 2);
    }
	
	void warn(const std::string& message)
    {
		log(message, 3);
    }
	
	void info(const std::string& message)
    {
		log(message, 4);
    }    
	
	void debug(const std::string& message)
    {
		log(message, 5);
    }
	
    void log(const std::string& message, int loglevel = 5)
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            return;
        }

        std::string timestamp = getCurrentTimeInNanoseconds();
        std::string escapedMsg = escapeJson(message);

        std::ostringstream oss;
        
        oss << "{ \"streams\": [ { \"stream\": { \"job\": \"" << jobLabel << "\", \"level\": \""  << getLogLevelLabel(loglevel) << "\" }, "
            << "\"values\": [[ \"" << timestamp << "\", \"" << escapedMsg << "\" ]] } ] }";
        std::string jsonPayload = oss.str();

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, lokiUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, jsonPayload.length());

        CURLcode res = curl_easy_perform(curl);

	if (res != CURLE_OK)
        {
            std::cerr << "CURL error: " << curl_easy_strerror(res) << std::endl;
        }
        else
        {
           long httpCode = 0;
           curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
};

// Define static variable
bool LokiLogger::curlInitialized = false;

#endif // LOKI_LOGGER_H
