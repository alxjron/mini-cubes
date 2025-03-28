
TARGET = $(BUILD)/mini-cube
BUILD = build

LIBS = -lSDL2 -I./src/include
OBJS = $(BUILD)/main.o $(BUILD)/glad.o
OBJS_DEBUG = $(BUILD)/debug/main.o $(BUILD)/debug/glad.o

debug: $(OBJS_DEBUG)
	gcc $(LIBS) $^ -Wall -o $(TARGET)

release: $(OBJS)
	gcc $(LIBS) $^ -Wall -o $(TARGET)

$(BUILD)/debug/%.o: src/%.c | dirs
	gcc $(LIBS) -c -Wall -g $< -o $@

$(BUILD)/%.o: src/%.c | dirs
	gcc $(LIBS) -c -Wall -o3 $< -o $@


dirs:
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/debug

clean:
	rm -rf $(BUILD)

.PHONY: dirs clean release debug
