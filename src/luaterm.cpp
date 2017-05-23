#include <xkbcommon/xkbcommon-keysyms.h>

#include "rwte/window.h"
#include "rwte/luastate.h"
#include "rwte/luaterm.h"
#include "rwte/logging.h"
#include "rwte/term.h"

#define LOGGER() (logging::get("luaterm"))

static int term_mouse_press_ref = LUA_NOREF;
static int term_key_press_ref = LUA_NOREF;

static int luaterm_mode(lua_State *l)
{
    LuaState L(l);
    auto mode = (term_mode_enum) L.checkinteger(1);
    L.pushbool(g_term->mode()[mode]);
    return 1;
}

static int luaterm_send(lua_State *l)
{
    LuaState L(l);
    size_t len = 0;
    const char * buffer = L.checklstring(1, &len);
    g_term->send(buffer, len);
    return 0;
}

static int luaterm_mouse_press(lua_State *l)
{
    LuaState L(l);
    term_mouse_press_ref = L.setfuncref(1, term_mouse_press_ref);
    return 0;
}

static int luaterm_key_press(lua_State *l)
{
    LuaState L(l);
    term_key_press_ref = L.setfuncref(1, term_key_press_ref);
    return 0;
}

static int luaterm_clipcopy(lua_State *l)
{
    g_term->clipcopy();
    return 0;
}

static int luaterm_clippaste(lua_State *l)
{
    // todo: seems clumsy to initiate paste w/ window
    // (weird abstraction)
    window.clippaste();
    return 0;
}

static int luaterm_selpaste(lua_State *l)
{
    // todo: seems clumsy to initiate paste w/ window
    // (weird abstraction)
    window.selpaste();
    return 0;
}


// functions for term library
static const luaL_Reg term_funcs[] = {
    {"mode", luaterm_mode},
    {"send", luaterm_send},
    {"mouse_press", luaterm_mouse_press},
    {"key_press", luaterm_key_press},
    {"clipcopy", luaterm_clipcopy},
    {"clippaste", luaterm_clippaste},
    {"selpaste", luaterm_selpaste},
    {NULL, NULL}
};

static int term_openf(lua_State *l)
{
    LuaState L(l);

    // make the lib (4 funcs, 2 values)
    L.newlib(term_funcs, 6);

    // add modes table
    L.newtable();
#define PUSH_ENUM_FIELD(nm)\
    L.pushinteger(nm); L.setfield(-2, #nm)
    PUSH_ENUM_FIELD(MODE_WRAP);
    PUSH_ENUM_FIELD(MODE_INSERT);
    PUSH_ENUM_FIELD(MODE_APPKEYPAD);
    PUSH_ENUM_FIELD(MODE_ALTSCREEN);
    PUSH_ENUM_FIELD(MODE_CRLF);
    PUSH_ENUM_FIELD(MODE_MOUSEBTN);
    PUSH_ENUM_FIELD(MODE_MOUSEMOTION);
    PUSH_ENUM_FIELD(MODE_REVERSE);
    PUSH_ENUM_FIELD(MODE_KBDLOCK);
    PUSH_ENUM_FIELD(MODE_HIDE);
    PUSH_ENUM_FIELD(MODE_ECHO);
    PUSH_ENUM_FIELD(MODE_APPCURSOR);
    PUSH_ENUM_FIELD(MODE_MOUSESGR);
    PUSH_ENUM_FIELD(MODE_8BIT);
    PUSH_ENUM_FIELD(MODE_BLINK);
    PUSH_ENUM_FIELD(MODE_FBLINK);
    PUSH_ENUM_FIELD(MODE_FOCUS);
    PUSH_ENUM_FIELD(MODE_MOUSEX10);
    PUSH_ENUM_FIELD(MODE_MOUSEMANY);
    PUSH_ENUM_FIELD(MODE_BRCKTPASTE);
    PUSH_ENUM_FIELD(MODE_PRINT);
    PUSH_ENUM_FIELD(MODE_UTF8);
    PUSH_ENUM_FIELD(MODE_SIXEL);
#undef PUSH_ENUM_FIELD
    L.setfield(-2, "modes");

    // add keys table. Note that this table is quite
    // incomplete; there's no reason for this other than
    // laziness
    L.newtable();
#define PUSH_ENUM_FIELD(nm)\
    L.pushinteger(XKB_KEY_##nm); L.setfield(-2, #nm)
    // latin 1 keys (note that keys above U+007E are missing)
    PUSH_ENUM_FIELD(space);
    PUSH_ENUM_FIELD(exclam);
    PUSH_ENUM_FIELD(quotedbl);
    PUSH_ENUM_FIELD(numbersign);
    PUSH_ENUM_FIELD(dollar);
    PUSH_ENUM_FIELD(percent);
    PUSH_ENUM_FIELD(ampersand);
    PUSH_ENUM_FIELD(apostrophe);
    PUSH_ENUM_FIELD(parenleft);
    PUSH_ENUM_FIELD(parenright);
    PUSH_ENUM_FIELD(asterisk);
    PUSH_ENUM_FIELD(plus);
    PUSH_ENUM_FIELD(comma);
    PUSH_ENUM_FIELD(minus);
    PUSH_ENUM_FIELD(period);
    PUSH_ENUM_FIELD(slash);
    PUSH_ENUM_FIELD(0);
    PUSH_ENUM_FIELD(1);
    PUSH_ENUM_FIELD(2);
    PUSH_ENUM_FIELD(3);
    PUSH_ENUM_FIELD(4);
    PUSH_ENUM_FIELD(5);
    PUSH_ENUM_FIELD(6);
    PUSH_ENUM_FIELD(7);
    PUSH_ENUM_FIELD(8);
    PUSH_ENUM_FIELD(9);
    PUSH_ENUM_FIELD(colon);
    PUSH_ENUM_FIELD(semicolon);
    PUSH_ENUM_FIELD(less);
    PUSH_ENUM_FIELD(equal);
    PUSH_ENUM_FIELD(greater);
    PUSH_ENUM_FIELD(question);
    PUSH_ENUM_FIELD(at);
    PUSH_ENUM_FIELD(A);
    PUSH_ENUM_FIELD(B);
    PUSH_ENUM_FIELD(C);
    PUSH_ENUM_FIELD(D);
    PUSH_ENUM_FIELD(E);
    PUSH_ENUM_FIELD(F);
    PUSH_ENUM_FIELD(G);
    PUSH_ENUM_FIELD(H);
    PUSH_ENUM_FIELD(I);
    PUSH_ENUM_FIELD(J);
    PUSH_ENUM_FIELD(K);
    PUSH_ENUM_FIELD(L);
    PUSH_ENUM_FIELD(M);
    PUSH_ENUM_FIELD(N);
    PUSH_ENUM_FIELD(O);
    PUSH_ENUM_FIELD(P);
    PUSH_ENUM_FIELD(Q);
    PUSH_ENUM_FIELD(R);
    PUSH_ENUM_FIELD(S);
    PUSH_ENUM_FIELD(T);
    PUSH_ENUM_FIELD(U);
    PUSH_ENUM_FIELD(V);
    PUSH_ENUM_FIELD(W);
    PUSH_ENUM_FIELD(X);
    PUSH_ENUM_FIELD(Y);
    PUSH_ENUM_FIELD(Z);
    PUSH_ENUM_FIELD(bracketleft);
    PUSH_ENUM_FIELD(backslash);
    PUSH_ENUM_FIELD(bracketright);
    PUSH_ENUM_FIELD(asciicircum);
    PUSH_ENUM_FIELD(underscore);
    PUSH_ENUM_FIELD(grave);
    PUSH_ENUM_FIELD(a);
    PUSH_ENUM_FIELD(b);
    PUSH_ENUM_FIELD(c);
    PUSH_ENUM_FIELD(d);
    PUSH_ENUM_FIELD(e);
    PUSH_ENUM_FIELD(f);
    PUSH_ENUM_FIELD(g);
    PUSH_ENUM_FIELD(h);
    PUSH_ENUM_FIELD(i);
    PUSH_ENUM_FIELD(j);
    PUSH_ENUM_FIELD(k);
    PUSH_ENUM_FIELD(l);
    PUSH_ENUM_FIELD(m);
    PUSH_ENUM_FIELD(n);
    PUSH_ENUM_FIELD(o);
    PUSH_ENUM_FIELD(p);
    PUSH_ENUM_FIELD(q);
    PUSH_ENUM_FIELD(r);
    PUSH_ENUM_FIELD(s);
    PUSH_ENUM_FIELD(t);
    PUSH_ENUM_FIELD(u);
    PUSH_ENUM_FIELD(v);
    PUSH_ENUM_FIELD(w);
    PUSH_ENUM_FIELD(x);
    PUSH_ENUM_FIELD(y);
    PUSH_ENUM_FIELD(z);
    PUSH_ENUM_FIELD(braceleft);
    PUSH_ENUM_FIELD(bar);
    PUSH_ENUM_FIELD(braceright);
    PUSH_ENUM_FIELD(asciitilde);
    // cusor motion and func keys
    PUSH_ENUM_FIELD(Home);
    PUSH_ENUM_FIELD(Insert);
    PUSH_ENUM_FIELD(Delete);
    PUSH_ENUM_FIELD(End);
    PUSH_ENUM_FIELD(Page_Up); // Prior
    PUSH_ENUM_FIELD(Page_Down); // Next
    PUSH_ENUM_FIELD(Return);
    PUSH_ENUM_FIELD(ISO_Left_Tab);
    PUSH_ENUM_FIELD(BackSpace);
    // F keys
    PUSH_ENUM_FIELD(F1);
    PUSH_ENUM_FIELD(F2);
    PUSH_ENUM_FIELD(F3);
    PUSH_ENUM_FIELD(F4);
    PUSH_ENUM_FIELD(F5);
    PUSH_ENUM_FIELD(F6);
    PUSH_ENUM_FIELD(F7);
    PUSH_ENUM_FIELD(F8);
    PUSH_ENUM_FIELD(F9);
    PUSH_ENUM_FIELD(F10);
    PUSH_ENUM_FIELD(F11);
    PUSH_ENUM_FIELD(F12);
    // KP keys
    PUSH_ENUM_FIELD(KP_Home);
    PUSH_ENUM_FIELD(KP_Up);
    PUSH_ENUM_FIELD(KP_Down);
    PUSH_ENUM_FIELD(KP_Left);
    PUSH_ENUM_FIELD(KP_Right);
    PUSH_ENUM_FIELD(KP_Prior);
    PUSH_ENUM_FIELD(KP_Begin);
    PUSH_ENUM_FIELD(KP_End);
    PUSH_ENUM_FIELD(KP_Next);
    PUSH_ENUM_FIELD(KP_Insert);
    PUSH_ENUM_FIELD(KP_Delete);
    PUSH_ENUM_FIELD(KP_Multiply);
    PUSH_ENUM_FIELD(KP_Add);
    PUSH_ENUM_FIELD(KP_Enter);
    PUSH_ENUM_FIELD(KP_Subtract);
    PUSH_ENUM_FIELD(KP_Decimal);
    PUSH_ENUM_FIELD(KP_Divide);
    PUSH_ENUM_FIELD(KP_0);
    PUSH_ENUM_FIELD(KP_1);
    PUSH_ENUM_FIELD(KP_2);
    PUSH_ENUM_FIELD(KP_3);
    PUSH_ENUM_FIELD(KP_4);
    PUSH_ENUM_FIELD(KP_5);
    PUSH_ENUM_FIELD(KP_6);
    PUSH_ENUM_FIELD(KP_7);
    PUSH_ENUM_FIELD(KP_8);
    PUSH_ENUM_FIELD(KP_9);
#undef PUSH_ENUM_FIELD
    L.setfield(-2, "keys");

	return 1;
}

void register_luaterm(LuaState *L)
{
    L->requiref("term", term_openf, true);
    L->pop();
}

bool luaterm_mouse_press(LuaState *L, int col, int row, int button,
        const keymod_state& mods)
{
    // first, try to push the mouse_press ref
    if (!L->pushfuncref(term_mouse_press_ref))
        return false;

    L->pushinteger(col);
    L->pushinteger(row);
    L->pushinteger(button);
    L->newtable();
    L->pushbool(mods[MOD_SHIFT]);
    L->setfield(-2, "shift");
    L->pushbool(mods[MOD_CTRL]);
    L->setfield(-2, "ctrl");
    L->pushbool(mods[MOD_ALT]);
    L->setfield(-2, "alt");
    L->pushbool(mods[MOD_LOGO]);
    L->setfield(-2, "logo");
    // todo: include num

    if (L->pcall(4, 1, 0) == LUA_OK)
    {
        bool result = L->tobool(-1);
        L->pop(1);
        return result;
    }
    else
    {
        LOGGER()->error("mouse_press: {}", L->tostring(-1));
        L->pop(1);
        return false;
    }
}

bool luaterm_key_press(LuaState *L, int keysym,
        const keymod_state& mods)
{
    // first, try to push the key_press ref
    if (!L->pushfuncref(term_key_press_ref))
        return false;

    L->pushinteger(keysym);
    L->newtable();
    L->pushbool(mods[MOD_SHIFT]);
    L->setfield(-2, "shift");
    L->pushbool(mods[MOD_CTRL]);
    L->setfield(-2, "ctrl");
    L->pushbool(mods[MOD_ALT]);
    L->setfield(-2, "alt");
    L->pushbool(mods[MOD_LOGO]);
    L->setfield(-2, "logo");
    // todo: include num

    if (L->pcall(2, 1, 0) == LUA_OK)
    {
        bool result = L->tobool(-1);
        L->pop(1);
        return result;
    }
    else
    {
        LOGGER()->error("key_press: {}", L->tostring(-1));
        L->pop(1);
        return false;
    }
}

