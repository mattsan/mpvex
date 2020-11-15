PREFIX = $(MIX_APP_PATH)/priv
BUILD = $(MIX_APP_PATH)/obj

NIF = $(PREFIX)/mpvex_nif.so

CFLAGS = -std=c++11 -fpic `pkg-config --cflags mpv`
LDFLAGS = -lpthread -dynamiclib -undefined dynamic_lookup `pkg-config --libs mpv`

ERL_CFLAGS = -I$(ERL_EI_INCLUDE_DIR)
ERL_LDFLAGS = -L$(ERL_EI_LIBDIR) -lei

SRC = $(wildcard c_src/*.cpp)
HEADERS = $(wildcard c_src/*.h)
OBJ = $(SRC:c_src/%.cpp=$(BUILD)/%.o)

all: install

install: $(PREFIX) $(BUILD) $(NIF)

$(OBJ): $(HEADERS) Makefile

$(BUILD)/%.o: c_src/%.cpp
	$(CC) -c $(CFLAGS) $(ERL_CFLAGS) -o $@ $<

$(NIF): $(OBJ)
	$(CC) $(LDFLAGS) $(ERL_LDFLAGS) -o $@ $^

$(PREFIX) $(BUILD):
	mkdir -p $@

clean:
	$(RM) $(NIF) $(OBJ)

.PHONY: all clean install
