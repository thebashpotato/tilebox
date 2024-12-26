PROJECT_ROOT:=$(shell pwd)
EMBED_SCRIPT:=$(PROJECT_ROOT)/scripts/embed.sh
BUILD_DIR?=build
GENERATOR?=Ninja

compile:
	# Builds the configuration
	cmake --build $(BUILD_DIR) --parallel

clean_rebuild:
	# Only for debug mode, cleans build files only
	cmake --build $(BUILD_DIR) --config Debug --target clean --parallel

configure_clang_debug:
	# Configures the project with Clang compiler in Debug mode.
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -B $(BUILD_DIR) -G $(GENERATOR)

configure_clang_rel:
	# Configures the project with Clang compiler in Release mode.
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -B $(BUILD_DIR) -G $(GENERATOR)

configure_gnu_debug:
	# Configures the project with GNU compiler in Debug mode.
	cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -B $(BUILD_DIR) -G $(GENERATOR)

configure_gnu_rel:
	# Configures the project with GNU compiler in Release mode.
	cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -B $(BUILD_DIR) -G $(GENERATOR)

embed:
	# Embeds twm in a Xephyr instance for testing
	$(EMBED_SCRIPT)


tests:
	ctest --test-dir $(BUILD_DIR)

clean:
	# Scorthed Earth, deletes all build artifact folders
	rm -rf $(BUILD_DIR) && rm -rf .cache

.ONESHEL:
