#include <iostream>
#include <string>
#include <map>
// Libcurl to make HTTP requests
#include <curl/curl.h>
// JSON for C++
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

// This is a callback function that will be used to write the response data
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *userp)
{
    size_t totalSize = size * nmemb;
    userp->append((char *)contents, totalSize);
    return totalSize;
}

// Initialize OpenAI
openai::api::Client client("{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}");

int main()
{
    // Create a map to store the prices at the time of the last trade
    std::map<std::string, double> lastTradePrices;

    // List of stock symbols to fetch data for
    std::vector<std::string> symbols = {
        "AAPL", "MSFT", "AMZN", "FB", "GOOGL",
        "GOOG", "TSLA", "NVDA", "BRK.B", "JPM",
        "JNJ", "V", "PG", "HD", "UNH",
        "MA", "DIS", "BAC", "VZ", "XOM"};

    // Create a curl instance
    CURL *curl = curl_easy_init();

    // Check if the curl instance was created successfully
    if (curl)
    {
        for (const std::string &symbol : symbols)
        {
            // The result string where the response will be stored
            std::string responseString;

            // Set the URL for the HTTP request
            std::string url = "https://data.alpaca.markets/v1/last/stocks/" + symbol;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

            // Set HTTP headers
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "APCA-API-KEY-ID: PKO3RZBLQ0QU5NSTGBDY");
            headers = curl_slist_append(headers, "APCA-API-SECRET-KEY: YpvjSz8Z34IEpdu8MZ1BdGUAPDOfqan7VmKmQqyV");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

            // This function will be used to write the response data
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);

            // This is where curl should write the response data
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseString);

            // Perform the HTTP request
            CURLcode res = curl_easy_perform(curl);

            // Check if the request was successful
            if (res != CURLE_OK)
            {
                fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            }
            else
            {
                // Parse the JSON response
                json jsonResponse = json::parse(responseString);

                // Extract and print some values from the JSON
                std::cout << "Symbol: " << jsonResponse["last"]["symbol"] << std::endl;
                double latestPrice = jsonResponse["last"]["price"];
                std::cout << "Latest price: " << latestPrice << std::endl;

                // Check if we have a last trade price for this stock
                if (lastTradePrices.find(symbol) != lastTradePrices.end())
                {
                    // Get the price at the time of the last trade
                    double lastTradePrice = lastTradePrices[symbol];

                    // Calculate the percentage change since the last trade
                    double percentageChange = 100.0 * (latestPrice - lastTradePrice) / lastTradePrice;

                    // Format the data for GPT-3
                    std::string gpt3Input = "The current price of " + symbol + " is " + std::to_string(latestPrice) + ". " + "It has changed by " + std::to_string(percentageChange) + "% since the last trade. " + "According to a simple trading strategy, if the price has increased by more than 5% since the last trade, it should be sold. " + "If it has decreased by more than 5%, it should be bought. " + "Otherwise, the position should be held. " + "Based on this information and strategy, should the stock be bought, sold, or held?";

                    // Request a prediction from GPT-3
                    openai::api::TextCompletionResponse gpt3Response = client.textCompletion(gpt3Input);

                    // Print the recommendation from GPT-3
                    std::cout << "GPT-3's recommendation for " << symbol << ": " << gpt3Response.choices[0].text.raw << std::endl;
                }

                // Update the last trade price
                lastTradePrices[symbol] = latestPrice;
            }

            // Clean up headers list
            curl_slist_free_all(headers);
        }

        // Always cleanup
        curl_easy_cleanup(curl);
    }

    return 0;
}
