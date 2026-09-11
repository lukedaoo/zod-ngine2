#pragma once

// runs a block when the enclosing scope ends,
// regardless of how it ends (return, break, exception).
// e.g.
//   FILE* f = fopen(path, "r");
//   defer { fclose(f); };
//   defer_fn([&]{ fclose(f); }); // same thing, pass a callable directly
template <class _func_>
struct zDeferred {
    _func_ func;
    ~zDeferred() { func(); }
};

struct zDeferHelp {
    template <class _func_>
    zDeferred<_func_> operator+(_func_ func) {
        return zDeferred<_func_>{func};
    }
};

#define defer auto CONCAT(_zdefer_, __LINE__) = zDeferHelp{} + [&]()
#define defer_fn(fn) auto CONCAT(_zdefer_, __LINE__) = zDeferred{fn}
