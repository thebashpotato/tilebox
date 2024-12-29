#include "atom_manager.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <tilebox/x11/display.hpp>

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace Tilebox::Twm
{

AtomManager::AtomManager(const X11DisplaySharedResource &dpy) noexcept
{
    Init(dpy);
}

void AtomManager::Init(const X11DisplaySharedResource &dpy) noexcept
{
    // clang-format off
    m_wm_atoms[std::underlying_type_t<Wm>(Wm::Protocols)] = XInternAtom(dpy->Raw(), "WM_PROTOCOLS", False);
    m_wm_atoms[std::underlying_type_t<Wm>(Wm::DeleteWindow)] = XInternAtom(dpy->Raw(), "WM_DELETE_WINDOW", False);
    m_wm_atoms[std::underlying_type_t<Wm>(Wm::TakeFocus)] = XInternAtom(dpy->Raw(), "WM_TAKE_FOCUS", False);
    m_wm_atoms[std::underlying_type_t<Wm>(Wm::State)] = XInternAtom(dpy->Raw(), "WM_STATE", False);

    m_net_atoms[std::underlying_type_t<Net>(Net::WmName)] = XInternAtom(dpy->Raw(), "_NET_WM_NAME", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::WmState)] = XInternAtom(dpy->Raw(), "_NET_WM_STATE", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::WmStateFullScreen)] = XInternAtom(dpy->Raw(), "_NET_WM_STATE_FULL_SCREEN", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::WmWindowType)] = XInternAtom(dpy->Raw(), "_NET_WM_WINDOW_TYPE", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::WmWindowTypeDialog)] = XInternAtom(dpy->Raw(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::ActiveWindow)] = XInternAtom(dpy->Raw(), "_NET_ACTIVE_WINDOW", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::Supported)] = XInternAtom(dpy->Raw(), "_NET_SUPPORTED", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::SupportingWmCheck)] = XInternAtom(dpy->Raw(), "_NET_SUPPORTING_WM_CHECK", False);
    m_net_atoms[std::underlying_type_t<Net>(Net::ClientList)] = XInternAtom(dpy->Raw(), "_NET_CLIENT_LIST", False);

    m_utf8_string_atom = XInternAtom(dpy->Raw(), "UTF8_STRING", False);
    // clang-format on
}

auto AtomManager::GetWwAtom(const Wm wm_atom) const noexcept -> Atom
{
    return m_wm_atoms[std::underlying_type_t<Wm>(wm_atom)];
}

auto AtomManager::GetNetAtom(const Net net_atom) const noexcept -> Atom
{
    return m_net_atoms[std::underlying_type_t<Net>(net_atom)];
}

auto AtomManager::GetNetAtomArrayInfo() noexcept -> std::tuple<std::uint64_t *, std::size_t>
{
    return std::make_tuple(m_net_atoms.data(), m_net_atoms.size());
}

auto AtomManager::GetUtf8Atom() const noexcept -> Atom
{
    return m_utf8_string_atom;
}

} // namespace Tilebox::Twm
