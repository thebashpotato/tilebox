#!/bin/env sh

BUILD_DIR=$(pwd)/build/twm
BINARY=twm

echo "Embedding ${BUILD_DIR}/${BINARY}"

if [ ! -f "${BUILD_DIR}"/"${BINARY}" ]; then
    echo "Binary not found: Please build ${BINARY} first"
    exit 0
fi

if ! command -v Xephyr 1>/dev/null 2>&1; then
    echo "Program not found: Install Xephyr"
    exit 0
fi

echo "Press Ctrl+Shift to grab mouse and keyboard events in the Xephyr window, and Ctrl+Shift again to release"
echo

Xephyr :1 -ac -br -noreset -screen 1000x600 &
XEPHYR_PID=$!
sleep 1
DISPLAY=:1 "${BUILD_DIR}"/${BINARY} &
wait $XEPHYR_PID

exit 0
