#include <iostream>
#include <string>
#include <map>
#include <cstdio> // For popen and pclose
#include <curl/curl.h>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp) {
    size_t totalSize = size * nmemb;
    userp->append((char *)contents, totalSize);
    return totalSize;
}

int main() {
    // Getting information
    std::map<std::string, double> lastTradePrices;
    std::vector<std::string> symbols = {
        "AAPL", "MSFT", "AMZN", "FB", "GOOGL",
        "GOOG", "TSLA", "NVDA", "BRK.B", "JPM",
        "JNJ", "V", "PG", "HD", "UNH",
        "MA", "DIS", "BAC", "VZ", "XOM"
    };

    CURL *curl = curl_easy_init();

    if (curl) {
        for (const std::string &symbol : symbols) {
            std::string responseString;
            std::string url = "https://data.alpaca.markets/v1/last/stocks/" + symbol;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "APCA-API-KEY-ID: INSERT KEY HERE");
            headers = curl_slist_append(headers, "APCA-API-SECRET-KEY: INSERT KEY HERE");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

            CURLcode res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            } else {
                json jsonResponse = json::parse(responseString);
                std::cout << "Symbol: " << jsonResponse["last"]["symbol"] << std::endl;
                double latestPrice = jsonResponse["last"]["price"];
                std::cout << "Latest price: " << latestPrice << std::endl;

                if (lastTradePrices.find(symbol) != lastTradePrices.end()) {
                    double lastTradePrice = lastTradePrices[symbol];
                    double percentageChange = 100.0 * (latestPrice - lastTradePrice) / lastTradePrice;

                    std::string command = "python openai_advisor.py " + std::to_string(percentageChange) + " " + std::to_string(latestPrice);
                    FILE* pipe = popen(command.c_str(), "r");
                    if (!pipe) throw std::runtime_error("popen() failed!");
                    char buffer[128];
                    std::string result = "";
                    while (!feof(pipe)) {
                        if (fgets(buffer, 128, pipe) != NULL)
                            result += buffer;
                    }
                    pclose(pipe);

                    std::cout << "GPT-3's recommendation for " << symbol << ": " << result << std::endl;
                }
                lastTradePrices[symbol] = latestPrice;
            }
            curl_slist_free_all(headers);
        }
        curl_easy_cleanup(curl);
    }

    return 0;
}

