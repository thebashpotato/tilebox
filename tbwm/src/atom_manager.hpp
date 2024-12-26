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
        WmProtocols, // Always first entry
        WmDeleteWindow,
        WmState,
        WmTakeFocus,
        NetWmName,
        NetWmState,
        NetWmStateFullScreen,
        NetWmWindowType,
        NetWmWindowTypeDialog,
        NetActiveWindow,
        NetSupported,
        NetSupportingWmCheck,
        NetClientList,
        Utf8String,
        End, // Specifies end of atom entries, not an atom itself.
    };

  public:
    AtomManager() noexcept = default;

  public:
    /// @brief Initilizes all atoms
    void Init(const X11DisplaySharedResource &dpy) noexcept;

    [[nodiscard]] auto GetAtom(Type type) const noexcept -> Atom;

  private:
    using AtomTypeIterator = etl::EnumerationIterator<Type, Type::WmProtocols, Type::End>;

  private:
    std::array<Atom, AtomTypeIterator::size()> m_atoms{};
};

} // namespace Tilebox::Twm
