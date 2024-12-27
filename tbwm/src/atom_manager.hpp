#pragma once

#include <etl.hpp>
#include <tilebox/x11/display.hpp>

#include <X11/X.h>

#include <array>
#include <cstdint>

namespace Tilebox::Twm
{

/// @brief Provides an API to initializae and fetch all atoms that twm supports.
class AtomManager
{
  public:
    enum class Type : std::uint8_t
    {
        // Lists which protocols (like close/focus) the client can handle.
        WmProtocols,

        // Lets the client handle “close window” requests from the WM.
        WmDeleteWindow,

        // Describes a window’s current state (e.g., minimized).
        WmState,

        // Informs the WM that the client can handle focus changes.
        WmTakeFocus,

        // Extended property naming a window (replaces WM_NAME).
        NetWmName,

        // Manages additional window states (maximized, etc.).
        NetWmState,

        // Indicates a full-screen state for the window.
        NetWmStateFullScreen,

        // Declares the window’s type (dialog, normal, etc.).
        NetWmWindowType,

        // Specifies a dialog window type.
        NetWmWindowTypeDialog,

        // Identifies the currently active window.
        NetActiveWindow,

        // Lists the extended window properties the WM supports.
        NetSupported,

        // Used to confirm the WM supports EWMH standards.
        NetSupportingWmCheck,

        // Holds the list of all client windows.
        NetClientList,

        // Defines UTF-8 encoding for text properties.
        Utf8String, // should remain the last entry in the enum
    };

  public:
    AtomManager() noexcept = default;

  public:
    /// @brief Initilizes all atoms
    void Init(const X11DisplaySharedResource &dpy) noexcept;

    [[nodiscard]] auto GetAtom(const Type type) const noexcept -> Atom;

  private:
    using AtomTypeIterator = etl::EnumerationIterator<Type, Type::WmProtocols, Type::Utf8String>;

  private:
    std::array<Atom, AtomTypeIterator::size() + 1> m_atoms{};
};

} // namespace Tilebox::Twm
