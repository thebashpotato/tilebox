define _list =
	cat << EOF
		==============================
		| Command   ||  Description  |
		==============================
		list        --  Lists all available commands
		embed       --  Embeds the window manager in a Xephyr window for testing and debugging
		tests       --  Runs ctest on the test suite
		install     --  Installs optimized binaries, libraries, files and scripts
		uninstall   --  Uninstalls all binararies, libraries, files and scripts
		compile     --  Compiles with the settings defined in the .env file
		clean       --  Cleans all build artifacts
	EOF
endef

define _install =
	if [ ! -d build ]; then
		echo "Cannot install, no build folder found"
		echo "Try running => make install"
		exit 1
	else
		cmake --build build --target install
	fi
endef

define _uninstall =
	if [ ! -d build ] || [ ! -f build/install_manifest.txt ]; then
		echo "Cannot uninstall, no build folder found or no install_manifest.txt file found"
		echo "Try running => make install"
		exit 1
	else
		echo "Removing installed files.."
		cat build/install_manifest.txt
		echo
		xargs rm < build/install_manifest.txt
	fi
endef

define _clean =
	[ -d build ] && rm -rf build/
	[ -d .cache ] && rm -rf .cache/
	[ -L compile_commands.json ] && rm compile_commands.json

	echo "Project cleaned.."
endef

define _compile =
	if [ ! -f compile_commands.json ]; then
		[ -d build ] && ln -s ./build/compile_commands.json .
	fi
	cmake --build build -j $(nproc --ignore 1)
endef

# Embeds the window manager in a Xephyr window for testing and debugging
define _embed =
	if [ ! -f build/window-manager/tilebox-wm]; then
		echo "Please build the window manager first"
		exit 0
	fi

	if ! command -v Xephyr 1>/dev/null 2>&1; then
		echo "Please install Xephyr"
		exit 0
	fi

	echo "Press Ctrl+Shift to grab mouse and keyboard events in the Xephyr window, and Ctrl+Shift again to release"

	Xephyr :1 -ac -br -noreset -screen 1024x768 &
	XEPHYR_PID=$!
	sleep 1
	DISPLAY=:1 ./build/window-manager/tilbox-wm &
	wait $$XEPHYR_PID
endef
