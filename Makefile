
TARGET = $(BUILD)/mini-cube
BUILD = build
RELEASE = $(BUILD)/release
DEBUG = $(BUILD)/debug

LIBS = -lSDL2 -lm -I./src/include
OBJS = main.o glad.o shader.o mesh.o camera.o region.o
OBJS_RELEASE = $(addprefix $(RELEASE)/, $(OBJS))
OBJS_DEBUG = $(addprefix $(DEBUG)/, $(OBJS));

debug: $(OBJS_DEBUG)
	gcc $(LIBS) $^ -Wall -o $(TARGET)

release: $(OBJS_RELEASE)
	gcc $(LIBS) $^ -Wall -o $(TARGET)

$(BUILD)/debug/%.o: src/%.c | dirs
	gcc $(LIBS) -c -Wall -g $< -o $@

$(BUILD)/%.o: src/%.c | dirs
	gcc $(LIBS) -c -Wall -o3 $< -o $@


dirs:
	mkdir -p $(BUILD)
	mkdir -p $(DEBUG)
	mkdir -p $(RELEASE)

clean:
	rm -rf $(BUILD)

.PHONY: dirs clean release debug
