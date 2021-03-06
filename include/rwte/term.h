#ifndef RWTE_TERM_H
#define RWTE_TERM_H

#include <bitset>
#include <memory>

enum mouse_event_enum
{
	MOUSE_MOTION = 0,
	MOUSE_PRESS = 1,
	MOUSE_RELEASE = 2
};

enum keymod_state_enum
{
    MOD_SHIFT,
    MOD_CTRL,
    MOD_ALT,
    MOD_LOGO,
    MOD_LAST = MOD_LOGO
};

using keymod_state = std::bitset<MOD_LAST+1>;

enum glyph_attribute_enum
{
    ATTR_BOLD,
    ATTR_FAINT,
    ATTR_ITALIC,
    ATTR_UNDERLINE,
    ATTR_BLINK,
    ATTR_REVERSE,
    ATTR_INVISIBLE,
    ATTR_STRUCK,
    ATTR_WRAP,
    ATTR_WIDE,
    ATTR_WDUMMY,
    ATTR_LAST = ATTR_WDUMMY
    // ATTR_BOLD_FAINT = ATTR_BOLD | ATTR_FAINT,
};

using glyph_attribute = std::bitset<ATTR_LAST+1>;

enum term_mode_enum
{
    MODE_WRAP,
    MODE_INSERT,
    MODE_APPKEYPAD,
    MODE_ALTSCREEN,
    MODE_CRLF,
    MODE_MOUSEBTN,
    MODE_MOUSEMOTION,
    MODE_REVERSE,
    MODE_KBDLOCK,
    MODE_HIDE,
    MODE_ECHO,
    MODE_APPCURSOR,
    MODE_MOUSESGR,
    MODE_8BIT,
    MODE_BLINK,
    MODE_FBLINK,
    MODE_FOCUS,
    MODE_MOUSEX10,
    MODE_MOUSEMANY,
    MODE_BRCKTPASTE,
    MODE_PRINT,
    MODE_UTF8,
    MODE_SIXEL,
    MODE_LAST = MODE_SIXEL
};

using term_mode = std::bitset<MODE_LAST+1>;

const term_mode mouse_modes(
        1 << MODE_MOUSEBTN | 1 << MODE_MOUSEMOTION |
        1 << MODE_MOUSEX10 | 1 << MODE_MOUSEMANY);

struct Glyph
{
    char32_t u = ' ';           // character code
    glyph_attribute attr;    // attribute flags
    uint32_t fg = 0;      // foreground
    uint32_t bg = 0;      // background
};

enum cursor_type
{
    CURSOR_BLINK_BLOCK,
    CURSOR_STEADY_BLOCK,
    CURSOR_BLINK_UNDER,
    CURSOR_STEADY_UNDER,
    CURSOR_BLINK_BAR,
    CURSOR_STEADY_BAR
};

struct Cursor
{
    Glyph attr; // current char attributes
    int row = 0, col = 0;
    char state = 0;
};

struct Selection;
class TermImpl;

class Term
{
public:
    Term(int cols, int rows);
    ~Term();

    const Glyph& glyph(int col, int row) const;

    void reset();
    void resize(int cols, int rows);

    void setprint();

    const term_mode& mode() const;

    void blink();

    const Selection& sel() const;
    const Cursor& cursor() const;
    cursor_type cursortype() const;

    bool isdirty(int row) const;
    void setdirty();
    void cleardirty(int row);

    void putc(char32_t u);
    void mousereport(int col, int row, mouse_event_enum evt, int button,
            const keymod_state& mod);

    int rows() const;
    int cols() const;

    // default colors
    uint32_t deffg() const;
    uint32_t defbg() const;
    uint32_t defcs() const;
    uint32_t defrcs() const;

    void setfocused(bool focused);
    bool focused() const;

    void selclear();
    void clipcopy();

    void send(const char *data, std::size_t len = 0);

private:
    std::unique_ptr<TermImpl> impl;
};

extern std::unique_ptr<Term> g_term;

#endif // RWTE_TERM_H
