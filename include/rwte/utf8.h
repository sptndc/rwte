#ifndef UTF8_H
#define UTF8_H

#include <cstdint>

typedef uint_least32_t Rune;

const int utf_size = 4;
const Rune utf_invalid = 0xFFFD;

std::size_t utf8validate(Rune *u, std::size_t i);
std::size_t utf8decode(const char *c, Rune *u, std::size_t clen);
std::size_t utf8encode(Rune u, char *c);
const char * utf8strchr(const char *s, Rune u);

#endif // UTF8_H
