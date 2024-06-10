# Trading-Simulator-and-Analyzer

## Insights and Analysis

### 1.1 Basic Strategy

**Intuition:**
- The strategy's intuition is straightforward – if the price has been consistently increasing for the last n days, there is an expectation that this upward trend will continue. Hence, buying shares at the current moment anticipates selling them in the future at a higher price. The same reasoning applies to selling shares in the case of a monotonically decreasing trend.

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal is triggered when the current price is consistently higher than the price n days ago, indicating a sustained upward trend. The rationale is grounded in the expectation that the current upward momentum will persist, making it an opportune time to buy shares.
- **Sell Signal Rationale:** Conversely, a sell signal is generated when the current price is consistently lower than the price n days ago, signaling a prolonged downward trend. This suggests a potential future decline in prices, prompting the decision to sell shares.

**Mathematical Foundation:**
- This strategy's mathematical foundation lies in identifying and capitalizing on sustained price trends. It offers a simplistic yet intuitive approach to trading based on historical price movements, providing traders with a clear framework for making buy or sell decisions.

**Run Instructions:**
```bash
make strategy=BASIC symbol=SBIN n=5 x=2 start_date="b" end_date="a"
```

### 1.2. Trend-based Strategy, n-Day Moving Average (DMA)

**Mathematical Analysis:**
- This strategy is useful for making a profit by identifying potential trend changes. The DMA serves as a smoothed representation of the historical average price, offering a trend direction. The inclusion of the standard deviation provides a measure of market volatility.

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal occurs when the current price significantly exceeds the expected average, indicating an upward trend. The inclusion of p standard deviations adds a confidence level, ensuring that the price change is substantial enough to justify a buy.
- **Sell Signal Rationale:** Conversely, a sell signal is triggered when the current price falls significantly below the expected average, suggesting a potential downward trend. The p standard deviations act as a buffer, filtering out noise and confirming a substantial price decrease.

**Mathematical Approach:**
- This mathematical approach allows traders to capitalize on potential trend changes while mitigating the impact of market noise. The strategy is particularly useful in capturing trend reversals and exploiting periods of heightened volatility, contributing to its profit-making potential in dynamic market conditions. The adaptability of the strategy, controlled by parameters n and p, provides a degree of customization to suit different market environments and risk preferences.

**Run Instructions:**
```bash
make strategy=DMA symbol=SBIN n=50 x=3 p=2 start_date="a" end_date="b"
```
### 1.3 Improving DMA

**Mathematical Insights:**
- **Stop-loss Limits:** This feature forcefully closes positions after `max_hold_days`, limiting potential losses.
- **Adaptive Moving Average (AMA):** AMA adapts to market efficiency with a dynamically adjusting SF.
- **Buy/Sell Decisions:** Decisions are based on the percentage difference between the current price and the calculated AMA, offering a nuanced approach to capturing trends.

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal is generated when the current price exceeds the Adaptive Moving Average (AMA) by a significant percentage (≥ p %). This indicates potential upward momentum, aligning with the expectation of further price increases.
- **Sell Signal Rationale:** Conversely, a sell signal is triggered when the current price falls below the AMA by a substantial percentage (≥ p %). This suggests a potential downward trend, and selling shares helps mitigate losses or secure profits.

**Risk Management:**
- The inclusion of stop-loss helps manage risk by automatically closing positions after a set holding period (`max_hold_days`), limiting potential losses.

**Adaptability:**
- The Adaptive Moving Average introduces adaptability to changing market conditions, making the strategy responsive to evolving trends and providing traders with a more dynamic tool for decision-making.

**Run Instructions:**
```bash
make strategy="DMA++" symbol=SBIN x=4 p=5 n=14 max_hold_days=28 c1=2 c2=0.2 start_date="a" end_date="b"

```
### 1.4.1 MACD

**Mathematical Insights:**
- This strategy incorporates Exponential Weighted Means (EWM) to capture short-term and long-term trends in stock prices.
- The Moving Average Convergence Divergence (MACD) represents the difference between short-term and long-term trends.
- The Signal Line smoothens the MACD, providing clearer buy or sell signals.

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal occurs when the Short EWM (representing short-term trend) is higher than the Long EWM (representing long-term trend). This suggests a potential upward trend, prompting the consideration of buying shares.
- **Sell Signal Rationale:** Conversely, a sell signal is triggered when the Short EWM is lower than the Long EWM, indicating a potential downward trend. This implies that it might be a suitable time to sell shares.

**Adaptability:**
- The inclusion of Exponential Weighted Means allows the strategy to adapt more quickly to recent price changes, making it responsive to short-term fluctuations.
- The use of MACD and the Signal Line enhances the strategy's sophistication, providing traders with a more comprehensive tool for trend analysis and decision-making.

**Run Instructions:**
```bash
make strategy=MACD symbol=SBIN x=3 start_date="a" end_date="b"
```
### 1.4.2 Relative Strength Index (RSI)

**Mathematical Rationale:**
- **RS Calculation:** The RS represents the ratio of average gains to average losses. A higher RS indicates stronger upward momentum, while a lower RS suggests stronger downward momentum.
- **RSI Calculation:** The RSI is calculated as 100 minus the relative strength normalized between 0 and 1. This normalizing factor ensures that the RSI value falls within the range of 0 to 100.

**Profit-Making Potential:**
- **Buy Signal:** Generated when the RSI crosses below the oversold threshold. This implies that the stock may be undervalued, and there is potential for a price reversal or a bullish trend.
- **Sell Signal:** Generated when the RSI crosses above the overbought threshold. This indicates that the stock may be overvalued, and a bearish trend or price correction might be imminent.

**Systematic Approach:**
- The RSI-based strategy provides a systematic approach to identifying potential trend reversals based on the momentum of gains and losses.
- It is particularly useful for traders seeking signals in overbought or oversold market conditions.
- Visualizing stock prices alongside RSI graphs can offer valuable insights into the strategy's effectiveness under different market scenarios.

**Run Instructions:**
```bash
make strategy=RSI symbol=SBIN x=3 n=14 oversold_threshold=30 overbought_threshold=70 start_date="a" end_date="b"
```
### 1.4.3 ADX

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal is generated when the ADX surpasses the specified threshold. This indicates a trend is strengthening, providing a potential opportunity to buy shares.
- **Sell Signal Rationale:** Conversely, a sell signal occurs when the ADX falls below the threshold, suggesting a weakening trend. This may prompt the decision to sell shares.

**Trend Strength Measurement:**
- This strategy utilizes the ADX as a measure of trend strength, allowing traders to identify favorable market conditions for buying or selling.
- The inclusion of True Range and Directional Movement components enhances the strategy's ability to capture directional trends and filter out noise.

**Sophisticated Trend-Based Trading:**
- Provides a more sophisticated approach to trend-based trading, incorporating ADX for measuring trend strength.
- True Range and Directional Movement components contribute to better capturing directional trends.

**Run Instructions:**
```bash
make strategy=ADX symbol=SBIN x=3 n=14 adx_threshold=25 start_date="a" end_date="b"
```
### 2. Linear Regression

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal is triggered when the predicted price is higher than the actual price by at least p%. This indicates an expectation that the actual price will converge to the predicted price, providing an opportunity to buy the stock and potentially sell it later at a higher price.
- **Sell Signal Rationale:** Conversely, a sell signal occurs when the predicted price is lower than the actual price by at least p%. This suggests an anticipation of a price decrease, prompting a decision to sell the stock.

**Machine Learning-Based Decision-Making:**
- This strategy leverages machine learning, specifically linear regression, to predict stock prices and make informed trading decisions.
- The rationale behind buy and sell signals lies in the expectation that the actual price will move towards the predicted price, offering profit opportunities for the trader.

**Run Instructions:**
```bash
make strategy="LINEAR_REGRESSION" symbol=SBIN x=3 p=2 train_start_date="a" train_end_date="b" start_date="c" end_date="d"
```
### 3. Best of All

**Strategy Overview:**
- This strategy does not have its own specific trading algorithm. Instead, it compares the final profit and loss obtained by running all the strategies.
- The strategy selects the one that yields the maximum profit and executes that particular strategy.

**Run Instructions:**
```bash
make strategy="BEST_OF_ALL" symbol=SBIN start_date="a" end_date="b"
```
### 4. Mean-Reverting Pairs Trading Strategy

**Strategy Overview:**
- The Mean-Reverting Pairs Trading Strategy focuses on the spread between the prices of a pair of stocks rather than individual stock prices.
- The strategy assumes that the spread tends to revert to its mean over time.
- A sell signal is generated when the z-score is significantly below the negative threshold, indicating that the spread is below its mean.
- Conversely, a buy signal is triggered when the z-score is significantly above the positive threshold, suggesting an expectation of a reversion to the average.

**Profit-Making Potential:**
- **Buy Signal Rationale:** A buy signal occurs when the z-score is significantly below the negative threshold, indicating that the spread is below its mean. This implies an expectation that the spread will increase, leading to a profitable trade by buying the spread.
- **Sell Signal Rationale:** Conversely, a sell signal is triggered when the z-score is significantly above the positive threshold. This suggests that the spread is above its mean, and there is an anticipation of a reversion to the average. Selling the spread under these conditions is expected to yield profit.

**Run Instructions:**
```bash
make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date="a" end_date="b"
```
### 4.1 Stop-Loss in Pairs Trading Strategy

**Strategy Overview:**
- The Loss-Based Stop-Loss Strategy addresses the challenge of unwanted positions in pairs trading.
- When a position is initiated based on the z-score crossing a threshold, the expectation is that the spread will revert to the mean.
- If the z-score moves in the unexpected direction, indicating a potential loss, the strategy aims to close the position to limit losses.

**Profit-Making Potential:**
- **Stop-Loss Rationale:** If the z-score moves in the unexpected direction and crosses the stop_loss_threshold, it indicates a deviation from the expected mean reversion. Closing the position at this point helps mitigate potential losses and allows traders to exit the trade before further unfavorable movement occurs.

**Run Instructions:**
```bash
make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 stop_loss_threshold=4 start_date="a" end_date="b"
```
## LIBRARIES USED

**Python File (Data Retrieval):**
- **Library Used:** jugaad-data
    - **Purpose:** Used to retrieve stock data for analysis.

**Python File (Date and Time Handling):**
- **Library Used:** datetime
    - **Purpose:** Utilized for accessing and manipulating date and time information.

**C++ File (Parallel Execution):**
- **Header File Used:** omp.h
    - **Purpose:** Employed to enable parallel execution of stock trading strategies, facilitating the concurrent implementation of multiple strategies.
