#pragma once

#include <etl.hpp>
#include <tilebox/x11/display.hpp>

#include <X11/X.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace Tilebox::Twm
{

/// @brief Provides an API to initialize and fetch all atoms that twm supports.
class AtomManager
{
  public:
    /// @brief Supported WM Atoms
    enum class Wm : std::uint8_t
    {
        // Lists which protocols (like close/focus) the client can handle.
        Protocols,

        // Lets the client handle “close window” requests from the WM.
        DeleteWindow,

        // Describes a window’s current state (e.g., minimized).
        State,

        // Informs the WM that the client can handle focus changes.
        TakeFocus,
    };

    /// @brief Supported Net Atoms
    enum class Net : std::uint8_t
    {
        // Extended property naming a window (replaces WM_NAME).
        WmName,

        // Manages additional window states (maximized, etc.).
        WmState,

        // Indicates a full-screen state for the window.
        WmStateFullScreen,

        // Declares the window’s type (dialog, normal, etc.).
        WmWindowType,

        // Specifies a dialog window type.
        WmWindowTypeDialog,

        // Identifies the currently active window.
        ActiveWindow,

        // Lists the extended window properties the WM supports.
        Supported,

        // Used to confirm the WM supports EWMH standards.
        SupportingWmCheck,

        // Holds the list of all client windows.
        ClientList,
    };

    using WmAtomIterator = etl::EnumerationIterator<Wm, Wm::Protocols, Wm::TakeFocus>;
    using NetAtomIterator = etl::EnumerationIterator<Net, Net::WmName, Net::ClientList>;

    [[nodiscard]] static auto ToUnderlying(const Wm atom) noexcept -> std::uint8_t
    {
        return std::underlying_type_t<Wm>(atom);
    }

    [[nodiscard]] static auto ToUnderlying(const Net atom) noexcept -> std::uint8_t
    {
        return std::underlying_type_t<Net>(atom);
    }

  public:
    explicit AtomManager(const X11DisplaySharedResource &dpy) noexcept;

  public:
    [[nodiscard]] auto GetWwAtom(const Wm wm_atom) const noexcept -> Atom;

    [[nodiscard]] auto GetNetAtom(const Net net_atom) const noexcept -> Atom;

    [[nodiscard]] auto GetNetAtomArrayInfo() noexcept -> std::tuple<std::uint64_t *, std::size_t>;

    [[nodiscard]] auto GetUtf8Atom() const noexcept -> Atom;

  private:
    void Init(const X11DisplaySharedResource &dpy) noexcept;

  private:
    std::array<Atom, WmAtomIterator::size()> m_wm_atoms{};
    std::array<Atom, NetAtomIterator::size()> m_net_atoms{};
    Atom m_utf8_string_atom{};
};

} // namespace Tilebox::Twm
