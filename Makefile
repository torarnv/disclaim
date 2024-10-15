
disclaim: main.cpp
	@clang -Wall -O3 -o $@ $^

clean:
	@rm -f disclaim
