BUILD_DIR = build

.PHONY: all
all:
	cmake -GNinja -B $(BUILD_DIR)
	ninja -C $(BUILD_DIR)

.PHONY: clean
clean:
	-rm -rf $(BUILD_DIR)
