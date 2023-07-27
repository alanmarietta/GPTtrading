# GPT Trading Bot

A trading bot powered by OpenAI's GPT-3 and the Alpaca trading API.

## Overview

This bot fetches real-time stock information using the Alpaca API, processes data, and then seeks guidance on trading decisions based on the processed data from OpenAI's GPT-3. It's an integration of cutting-edge NLP technology and stock trading for decision-making assistance.

## Features

- **Real-time Data**: Fetches current stock prices for a set of predefined stock symbols.
  
- **Decision Assistance**: Uses OpenAI's GPT-3 to get advice on whether to buy, sell, or hold a stock based on its recent price change.

- **Automated Trades** (upcoming): Integration with Alpaca's trading functionality to automatically execute trades based on GPT-3's recommendations.

## Technologies

- **Alpaca API**: For fetching real-time stock data.
  
- **OpenAI GPT-3**: Provides trading advice based on the current stock price and its change.

- **C++**: Primary programming language for the bot.
  
- **Python**: Used for interfacing with OpenAI GPT-3.

- **nlohmann/json**: A C++ library for handling JSON data.

## How it Works

1. **Data Retrieval**: Fetch real-time stock data for predefined symbols using the Alpaca API.

2. **Data Processing**: Parse the JSON response from Alpaca to extract the stock's current price.

3. **Decision Making**: If the stock price has a significant change from the last known price, consult GPT-3. A python script processes this data and sends a prompt to GPT-3 asking for trading advice.

4. **Display**: Display GPT-3's recommendation in the console.

5. **Automated Trades** (upcoming): Use Alpaca's trading functionality to automatically execute trades based on GPT-3's recommendations.

## Setup and Usage

1. Clone the repository.
2. Set up your Alpaca API keys and OpenAI API key in their respective places.
3. Compile and run the C++ program.
4. View GPT-3's trading recommendations in the console.

*Note*: Ensure you have the required libraries and dependencies installed, including curl for C++ and OpenAI for Python.

## Contributing

If you have suggestions or improvements, please fork this repository and create a pull request.

## License

MIT License

