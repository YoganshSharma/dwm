/* See LICENSE file for copyright and license details. */
#define TERMINAL "st"
#define BROWSER "firefox"
/* appearance */
static const unsigned int baralpha        = 0xff;
static const unsigned int borderalpha     = OPAQUE;
static unsigned int borderpx  = 5;        /* border pixel of windows */
static unsigned int gappx     = 6;        /* gaps between windows */
static unsigned int snap      = 32;       /* snap pixel */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft = 0;   	/* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar       = 1;          /* 0 means no bar */
static const int topbar        = 1;          /* 0 means bottom bar */
static const int usealtbar     = 0;          /* 1 means use non-dwm status bar */
static const char *altbarclass = "Polybar";  /* Alternate bar class name */
static const char *altbarcmd        = "$HOME/.config/polybar/launch.sh"; /* Alternate bar launch command */
static char *fonts[]          = { "monospace:size=11", "Iosevka Nerd Font:pixelsize=18:antialias=true:autohint=true","JoyPixels:pixelsize=15:antialias=true:autohint=true"  };
static const char dmenufont[]       = "JetBrains Mono:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3] = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "90x30", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spfm", "-g", "124x40", "-e", "lf", NULL };
const char *spcmd3[] = {"keepassxc", NULL };
const char *spcmd4[] = {TERMINAL, "-n", "spqalc", "-g", "60x20", "-e", "qalc", NULL };
const char *spcmd5[] = {TERMINAL, "-n", "spallen", "-g", "90x30", "-e", "fzlbpms", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spranger",    spcmd2},
	{"keepassxc",   spcmd3},
	{"spqalc",      spcmd4},
	{"spallen",     spcmd5},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class																				instance  		title								tags mask  		isfloating  isterminal   noswallow  monitor */
	{ "Gimp",																				NULL,						NULL,								0,              1,          0,           0,      	 -1 },
	{ "firefox",																		NULL,						NULL,								1 << 0,         0,          0,          -1,      	 -1 },
	{ "St",																					NULL,						NULL,								0,              0,          1,           0,      	 -1 },
	{ "Alacritty",																	NULL,						NULL,								0,              0,          0,           0,      	 -1 },
	{ "discord",																		NULL,						NULL,					      1 << 3,         0,          0,           0,      	 -1 },
	{ "Ferdi",																		  NULL,						NULL,								1 << 3,         0,          0,           0,      	 -1 },
	{ "Spotify",																		NULL,						NULL,								1 << 7,         0,          0,           1,      	 -1 },
	{ "broken", 																		NULL,						NULL,								1 << 7,         0,          0,           1,      	 -1 },
	{ "mpv",																		    NULL,						NULL,								1 << 2,         0,          0,           0,      	 -1 },
	{ NULL,																					NULL,					 "Event Tester",			0,              0,          0,           1,      	 -1 }, /* xev */
	{ NULL,																					"spterm",		    NULL,								SPTAG(0),				1,          0,           0,				 -1 },
	{ NULL,																					"spfm",					NULL,								SPTAG(1),				1,          0,           0,				 -1 },
	{ NULL,																					"keepassxc",		NULL,								SPTAG(2),				0,          0,           0,				 -1 },
	{ NULL,																					"spqalc",				NULL,								SPTAG(3),				1,          0,           0,				 -1 },
	{ NULL,																					"spallen",			NULL,								SPTAG(4),				1,          0,           0,				 -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },

};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
/* helper for spawning shell commands in the pre dwm-5.0 fashion */ 
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } } 

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "normbgcolor",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "normfgcolor",        STRING,  &normfgcolor },
		{ "selbgcolor",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selfgcolor",         STRING,  &selfgcolor },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "baralpha",          	INTEGER, &baralpha },
		{ "gappx",          	INTEGER, &gappx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
		{ "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
		{ "swallowfloating",	INTEGER, &swallowfloating },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"
#include "movestack.c"
static Key keys[] = {
	/* modifier                     key												  function        argument */
	{ MODKEY,												XK_grave,										spawn,					SHCMD("dmenuunicode") },
	{ MODKEY|ShiftMask,							XK_q,												spawn,		      SHCMD("sysact") },
	{ MODKEY|ShiftMask,							XK_f,												togglefullscr,	{0} },
	{ MODKEY|ShiftMask,							XK_n,												spawn,		      SHCMD(TERMINAL " -e newsboat") },
	{ MODKEY,												XK_q,												spawn,		      SHCMD("dmkill") },
	{ MODKEY,												XK_F1,											spawn,					SHCMD("sxiv -r -q -t -o /home/yogansh/wallpapers|xargs -r nitrogen --set-scaled --save") },
	{ MODKEY,												XK_F9,											spawn,					SHCMD("dmenumount") },
	{ MODKEY,												XK_F10,											spawn,					SHCMD("dmenuumount") },
	{ MODKEY,												XK_v,												spawn,					SHCMD("clipmagic") },
	{ MODKEY,												XK_r,												spawn,					SHCMD("launcher") },
	{ MODKEY,												XK_w,												spawn,					SHCMD(BROWSER) },
	{ MODKEY|ShiftMask,							XK_c,												spawn,					SHCMD("ferdi") },
	{ MODKEY|ShiftMask,							XK_d,												spawn,					SHCMD("discord") },
	{ MODKEY|ShiftMask,							XK_m,												spawn,					SHCMD("LD_PRELOAD=/usr/lib/spotify-adblock.so spotify") },
	{ MODKEY|ControlMask,						XK_space,										spawn,					SHCMD("dunstctl close-all") },
	{ MODKEY|Mod1Mask,              XK_space,										spawn,					SHCMD("dunstctl close") },
	{ ControlMask|Mod1Mask,	 		  XK_x,										    spawn,					SHCMD("xkill") },
	{ MODKEY|ControlMask,						XK_grave,										spawn,					SHCMD("dunstctl history-pop") },
	{ MODKEY|ControlMask,				    XK_Return,									spawn,				  SHCMD("dunstctl context") },
	{ MODKEY|ShiftMask,							XK_s,												spawn,					SHCMD("~/.config/rofi/bin/applet_screenshot") },
	{ 0,                            XF86XK_AudioMute,						spawn,					SHCMD("pamixer -t;kill -44 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_AudioRaiseVolume,		spawn,					SHCMD("pamixer --allow-boost -i 3;kill -44 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_AudioLowerVolume,		spawn,					SHCMD("pamixer --allow-boost -d 3;kill -44 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_MonBrightnessUp,			spawn,					SHCMD("kill -48 $(pidof dwmblocks)") },
	{ 0,                            XF86XK_MonBrightnessDown,		spawn,					SHCMD("kill -48 $(pidof dwmblocks)") },
	{ MODKEY,												XK_z,												togglescratch,  {.ui = 0 } },
	{ MODKEY,												XK_n,												togglescratch,  {.ui = 1 } },
	{ MODKEY,												XK_x,												togglescratch,  {.ui = 2 } },
	{ MODKEY,												XK_c,												togglescratch,  {.ui = 3 } },
	{ MODKEY,												XK_a,												togglescratch,  {.ui = 4 } },
	{ MODKEY,                       XK_p,												spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return,									spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,												togglebar,      {0} },
	{ MODKEY,                       XK_j,												focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,												focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,												incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,												incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,												setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,												setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,												movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,												movestack,      {.i = -1 } },
	{ MODKEY,                       XK_space,										zoom,           {0} },
	{ MODKEY,                       XK_s,												togglesticky,   {0} },
	{ MODKEY,                       XK_Tab,											view,           {0} },
	{ MODKEY|ShiftMask,             XK_w,												killclient,     {0} },
	{ MODKEY,                       XK_t,												setlayout,      {.v = &layouts[0]} }, /* tile */
	{ MODKEY,                       XK_f,												setlayout,      {.v = &layouts[1]} }, /*floating */
	{ MODKEY,                       XK_m,												setlayout,      {.v = &layouts[2]} }, /* monocle */
	{ MODKEY,                       XK_y,												setlayout,      {.v = &layouts[3]} }, /* deck */
	{ MODKEY,                       XK_u,												setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_u,												setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_space,										togglefloating, {0} },
	{ MODKEY,                       XK_0,												view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,												tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,										focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,									focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,										tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,									tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,																 0)
	TAGKEYS(                        XK_2,																 1)
	TAGKEYS(                        XK_3,																 2)
	TAGKEYS(                        XK_4,																 3)
	TAGKEYS(                        XK_5,																 4)
	TAGKEYS(                        XK_6,																 5)
	TAGKEYS(                        XK_7,																 6)
	TAGKEYS(                        XK_8,																 7)
	TAGKEYS(                        XK_9,																 8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};


static const char *ipcsockpath = "/tmp/dwm.sock";
static IPCCommand ipccommands[] = {
  IPCCOMMAND(  view,                1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggleview,          1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tag,                 1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  toggletag,           1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  tagmon,              1,      {ARG_TYPE_UINT}   ),
  IPCCOMMAND(  focusmon,            1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  focusstack,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  zoom,                1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  incnmaster,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  killclient,          1,      {ARG_TYPE_SINT}   ),
  IPCCOMMAND(  togglefloating,      1,      {ARG_TYPE_NONE}   ),
  IPCCOMMAND(  setmfact,            1,      {ARG_TYPE_FLOAT}  ),
  IPCCOMMAND(  setlayoutsafe,       1,      {ARG_TYPE_PTR}    ),
  IPCCOMMAND(  quit,                1,      {ARG_TYPE_NONE}   )
};

