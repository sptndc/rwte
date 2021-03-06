#include "lua/state.h"
#include "lua/window.h"
#include "rwte/logging.h"
#include "rwte/window.h"

#include <xkbcommon/xkbcommon-keysyms.h>

/// Window module; `window` is the global window object.
// @module window

#define LOGGER() (logging::get("luawindow"))

static int window_mouse_press_ref = LUA_NOREF;
static int window_key_press_ref = LUA_NOREF;

/// Sets the function to be called when a mouse button is pressed
// (except when the mouse button is handled internally, such as for selection).
//
// The function should return true if the mouse press was handled, something
// falsy otherwise.
//
// @function mouse_press
// @int col Terminal column where the event occurred
// @int row Terminal row where the event occurred
// @int button Mouse button (integer, 1-5)
// @tab mod Keyboard modifier flags
// @bool mod.shift Shift key
// @bool mod.ctrl Ctrl key
// @bool mod.alt Alt key
// @bool mod.logo Logo key
// @usage
// window.mouse_press(function(col, row, button, mod)
//     if button == 4 then -- mouse wheel up
//         logging.get("example"):info("WHEEEEEL UP!")
//         return true
//     end
// end)
static int luawindow_mouse_press(lua_State *l)
{
    lua::State L(l);
    window_mouse_press_ref = L.setfuncref(1, window_mouse_press_ref);
    return 0;
}

/// Sets the function to be called when a key is pressed (except when
// the key button is handled internally).
//
// The function should return true if the key press was handled, something
// falsy otherwise.
//
// @function key_press
// @int sym Key symbol. See values in @{keys}
// @tab mod Keyboard modifier flags
// @bool mod.shift Shift key
// @bool mod.ctrl Ctrl key
// @bool mod.alt Alt key
// @bool mod.logo Logo key
// @usage
// window.key_press(function(sym, mod)
//     if mod.shift then
//         if sym == window.keys.Q then
//             logging.get("example"):info("Q!!!")
//             return true
//         elseif sym == window.keys.W then
//             logging.get("example"):info("W!!")
//             return true
//         end
//     end
// end)
static int luawindow_key_press(lua_State *l)
{
    lua::State L(l);
    window_key_press_ref = L.setfuncref(1, window_key_press_ref);
    return 0;
}

/// Initiates paste of the system clipboard to the terminal.
//
// @function clippaste
// @usage window.clippaste()
static int luawindow_clippaste(lua_State *l)
{
    window.clippaste();
    return 0;
}

/// Initiates paste of the system selection to the terminal.
//
// @function selpaste
// @usage window.selpaste()
static int luawindow_selpaste(lua_State *l)
{
    window.selpaste();
    return 0;
}

/// Window identifier.
//
// (implemented via `__index`)
//
// @class field
// @name id
static int luawindow_index(lua_State *l)
{
    lua::State L(l);
    const char * key = L.checkstring(2);

    if (std::strcmp(key, "id") == 0)
        L.pushinteger(window.windowid());
    else
        L.pushnil();

    return 1;
}

// functions for window library
static const luaL_Reg window_funcs[] = {
    {"mouse_press", luawindow_mouse_press},
    {"key_press", luawindow_key_press},
    {"clippaste", luawindow_clippaste},
    {"selpaste", luawindow_selpaste},
    {nullptr, nullptr}
};

static int window_openf(lua_State *l)
{
    lua::State L(l);

    // make the lib (4 funcs, 1 value)
    L.newlib(window_funcs, 5);

    // add keys table. Note that this table is quite
    // incomplete; there's no reason for this other than
    // laziness

    /// Key symbol table; maps key names to key symbols.
    // @class field
    // @name keys
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
    PUSH_ENUM_FIELD(Up);
    PUSH_ENUM_FIELD(Down);
    PUSH_ENUM_FIELD(Left);
    PUSH_ENUM_FIELD(Right);
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
    PUSH_ENUM_FIELD(KP_Page_Up); //Prior
    PUSH_ENUM_FIELD(KP_Begin);
    PUSH_ENUM_FIELD(KP_End);
    PUSH_ENUM_FIELD(KP_Page_Down); // Next
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

    // set lib's metatable, including an __index
    // function so window.x can be used
    L.newtable();
    L.pushcfunction(luawindow_index);
    L.setfield(-2, "__index");
    L.setmetatable(-2);

    return 1;
}

void lua::register_luawindow(lua::State *L)
{
    L->requiref("window", window_openf, true);
    L->pop();
}

bool lua::window::call_mouse_press(lua::State *L, int col, int row, int button,
        const keymod_state& mods)
{
    // first, try to push the mouse_press ref
    if (!L->pushfuncref(window_mouse_press_ref))
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

bool lua::window::call_key_press(lua::State *L, int keysym,
        const keymod_state& mods)
{
    // first, try to push the key_press ref
    if (!L->pushfuncref(window_key_press_ref))
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

