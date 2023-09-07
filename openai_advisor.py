import openai
import sys

openai.api_key = "INSERT KEY HERE"

def get_advice(percentage_change, current_price):
    # Prepare the input for GPT-3 based on the received data
    prompt = f"""The current price is {current_price}. 
It has changed by {percentage_change}% since the last trade. 
According to a simple trading strategy, if the price has increased by more than 5% since the last trade, it should be sold.
If it has decreased by more than 5%, it should be bought.
Otherwise, the position should be held.
Based on this information and strategy, should the stock be bought, sold, or held?"""

    # Make a completion call to OpenAI GPT-3
    response = openai.Completion.create(engine="davinci", prompt=prompt, max_tokens=50)

    # Return the generated advice
    return response.choices[0].text.strip()


if __name__ == "__main__":
    # Read input data from command line arguments
    percentage_change = float(sys.argv[1])
    current_price = float(sys.argv[2])
    
    # Get the advice based on the input data
    advice = get_advice(percentage_change, current_price)

    # Print the advice to the console
    print(advice)

