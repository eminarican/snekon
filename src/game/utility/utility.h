#ifndef SNEKON_UTILITY_H
#define SNEKON_UTILITY_H

#define try_expected_(expected) { auto r = expected; if (!r.has_value()) return r; }

#define unwrap_expected_(var, expected) \
{ \
    auto r = expected; \
    if (!r.has_value()) { \
        std::cerr << "error: " << expected.error().to_string() << std::endl; \
        std::exit(1); \
    } else { var = r.value(); } \
}

#endif //SNEKON_UTILITY_H
