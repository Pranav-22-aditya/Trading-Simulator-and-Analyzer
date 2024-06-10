# # Default target
# .PHONY: all

# # Run app.py with specified parameters based on strategy
# all:
# 	@if [ "$(strategy)" = "BASIC" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(n) $(x) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "DMA" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(n) $(x) $(p) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "DMA++" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "MACD" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(x) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "RSI" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "ADX" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(x) $(n) $(adx_threshold) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "LINEAR_REGRESSION" ]; then \
# 		python3 app.py $(strategy) $(symbol) $(x) $(p) "$(train_start_date)" "$(train_end_date)" "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "BEST_OF_ALL" ]; then \
# 		python3 app.py $(strategy) $(symbol) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "PAIRS" ]; then \
# 		if [ -z "$(stop_loss_threshold)" ]; then \
# 			python3 app.py $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) "$(start_date)" "$(end_date)"; \
# 		else \
# 			python3 app.py $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) "$(start_date)" "$(end_date)"; \
# 		fi \
# 	fi

# # Clean rule
# clean:
# 	rm -f basic_strategy dma_strategy dma_plus_plus_strategy macd_strategy rsi_strategy adx_strategy pairs_strategy

# Default target
# .PHONY: all clean

# # Compile subtask3.cpp with OpenMP support into an executable named subtask3.out
# subtask3.out: subtask3_sahu_old.cpp
# 	g++ -fopenmp $< -o $@  all
	
# # Run subtask3.out with specified parameters based on strategy
# run: subtask3.out
	
# 	@echo "ghusa kya?"
# 	@if [ "$(strategy)" = "BASIC" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(n) $(x) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "DMA" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(n) $(x) $(p) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "DMA++" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(x) $(p) $(n) $(max_hold_days) $(c1) $(c2) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "MACD" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(x) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "RSI" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(x) $(n) $(oversold_threshold) $(overbought_threshold) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "ADX" ]; then \
# 		 g++ subtask3_sahu_old.cpp ./subtask3.out $(strategy) $(symbol) $(x) $(n) $(adx_threshold) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "LINEAR_REGRESSION" ]; then \
# 		./subtask3.out $(strategy) $(symbol) $(x) $(p) "$(train_start_date)" "$(train_end_date)" "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "BEST_OF_ALL" ]; then \
# 		./subtask3.out $(strategy) $(symbol) "$(start_date)" "$(end_date)"; \
# 	elif [ "$(strategy)" = "PAIRS" ]; then \
# 		if [ -z "$(stop_loss_threshold)" ]; then \
# 			./subtask3.out $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) "$(start_date)" "$(end_date)"; \
# 		else \
# 			./subtask3.out $(strategy) $(symbol1) $(symbol2) $(x) $(n) $(threshold) $(stop_loss_threshold) "$(start_date)" "$(end_date)"; \
# 		fi \
# 	fi

# all: subtask3.out run
# # # Clean rule
# clean:
# 	rm -f subtask3.out

# Define default target

	
all:install_dependencies analysis

# Define a generic target to handle all types of queries
analysis: main.out
	./main.out $(strategy) $(symbol) $(symbol1) $(symbol2) $(x) $(n) $(p) $(max_hold_days) $(c1) $(c2) $(train_start_date) $(train_end_date) $(threshold) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(stop_loss_threshold) $(start_date) $(end_date)

# Compile C++ code
main.out: subtask3.cpp
	g++ -o main.out subtask3.cpp

install_dependencies:
	pip install matplotlib jugaad-data pandas seaborn

#BASIC target
BASIC: analysis

# MAC target
MAC: analysis

# RSI target
RSI: analysis

# ADX target
ADX: analysis

# LINEAR_REGRESSION target
LINEAR_REGRESSION: analysis

# Clean target to remove compiled executable
clean:
	rm -f main.out
