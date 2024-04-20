#pragma once

#if defined(__GNUC__) && __GNUC__ >= 4
#define TILEBOX_EXPORT [[gnu::visibility("default")]]
#define TILEBOX_INTERNAL [[gnu::visibility("hidden")]]
#define TILEBOX_DEPRECATED __attribute__((deprecated))

#elif defined(__clang__)
#define TILEBOX_EXPORT [[clang::visibility("default")]]
#define TILEBOX_INTERNAL [[clang::visibility("hidden")]]
#define TILEBOX_DEPRECATED [[deprecated]]

#else
#define TILEBOX_EXPORT
#define TILEBOX_INTERNAL
#define TILEBOX_DEPRECATED

#endif
