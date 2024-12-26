#include "atom_manager.hpp"

#include <X11/X.h>
#include <X11/Xlib.h>
#include <tilebox/x11/display.hpp>

#include <type_traits>

namespace Tilebox::Twm
{

void AtomManager::Init(const X11DisplaySharedResource &dpy) noexcept
{
    // clang-format off
    m_atoms[std::underlying_type_t<Type>(Type::WmProtocols)] = XInternAtom(dpy->Raw(), "WM_PROTOCOLS", False);
    m_atoms[std::underlying_type_t<Type>(Type::WmDeleteWindow)] = XInternAtom(dpy->Raw(), "WM_DELETE_WINDOW", False);
    m_atoms[std::underlying_type_t<Type>(Type::WmState)] = XInternAtom(dpy->Raw(), "WM_STATE", False);
    m_atoms[std::underlying_type_t<Type>(Type::WmTakeFocus)] = XInternAtom(dpy->Raw(), "WM_TAKE_FOCUS", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetWmName)] = XInternAtom(dpy->Raw(), "_NET_WM_NAME", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetWmState)] = XInternAtom(dpy->Raw(), "_NET_WM_STATE", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetWmStateFullScreen)] = XInternAtom(dpy->Raw(), "_NET_WM_STATE_FULL_SCREEN", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetWmWindowType)] = XInternAtom(dpy->Raw(), "_NET_WM_WINDOW_TYPE", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetWmWindowTypeDialog)] = XInternAtom(dpy->Raw(), "_NET_WM_WINDOW_TYPE_DIALOG", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetActiveWindow)] = XInternAtom(dpy->Raw(), "_NET_ACTIVE_WINDOW", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetSupported)] = XInternAtom(dpy->Raw(), "_NET_SUPPORTED", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetSupportingWmCheck)] = XInternAtom(dpy->Raw(), "_NET_SUPPORTING_WM_CHECK", False);
    m_atoms[std::underlying_type_t<Type>(Type::NetClientList)] = XInternAtom(dpy->Raw(), "_NET_CLIENT_LIST", False);
    m_atoms[std::underlying_type_t<Type>(Type::Utf8String)] = XInternAtom(dpy->Raw(), "UTF8_STRING", False);
    // clang-format on
}

auto AtomManager::GetAtom(const Type type) const noexcept -> Atom
{
    return m_atoms[std::underlying_type_t<Type>(type)];
}

} // namespace Tilebox::Twm
