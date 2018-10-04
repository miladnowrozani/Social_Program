CC = g++
BIN_DIR = bin
CPPFLAGS = -Wall -g -std=c++11
TARGETS =  $(BIN_DIR)/twiteng.o $(BIN_DIR)/datetime.o $(BIN_DIR)/user.o $(BIN_DIR)/tweet.o $(BIN_DIR)/cmdhandler.o $(BIN_DIR)/handler.o $(BIN_DIR)/util.o $(BIN_DIR)/hash.o

all: $(BIN_DIR)/.dirstamp twitter
	-@echo "--- All Built!---"

twitter: twitter.cpp $(TARGETS)
	$(CC) $(CPPFLAGS) $^ -o $@

$(BIN_DIR)/cmdhandler.o: cmdhandler.cpp cmdhandler.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/handler.o: handler.cpp handler.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/util.o: util.cpp util.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/twiteng.o: twiteng.cpp twiteng.h msort.h heap.h tagdata.h 
	$(CC) $(CPPFLAGS) -c $< -o $@ 

$(BIN_DIR)/datetime.o: datetime.cpp datetime.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/user.o: user.cpp user.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/tweet.o: tweet.cpp tweet.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/hash.o: hash.cpp hash.h
	$(CC) $(CPPFLAGS) -c $< -o $@

$(BIN_DIR)/.dirstamp:
	-@mkdir -p $(BIN_DIR)
	-@touch $@

.PHONY: clean

clean:
	-@rm -rf $(BIN_DIR) twitter twitter.dSYM *.feed *.mentions 
	-@echo "--- All Clean!---"