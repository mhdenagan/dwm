/* See LICENSE file for copyright and license details. */

/* Constants */
#define TERMINAL "st"
#define TERMCLASS "St"

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static unsigned int snap      = 32;       /* snap pixel */
static unsigned int gappih    = 20;       /* horiz inner gap between windows */
static unsigned int gappiv    = 10;       /* vert inner gap between windows */
static unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=8", "JoyPixels:pixelsize=11:antialias=true:autohint=true", "FontAwesome:size=9" };
static char normbgcolor[]           = "#111222"; //#111111
static char normbordercolor[]       = "#000000"; //#444444
static char normfgcolor[]           = "#bbbbbb"; //#bbbbbb
static char selfgcolor[]            = "#eeeeee"; //#eeeeee
static char selbordercolor[]        = "#770000"; //#770000
static char selbgcolor[]            = "#000000"; //#000000
static const unsigned int baralpha  = 0xab ; //0xd0 [0xbe]
static const unsigned int borderalpha = OPAQUE;
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalpha },
	[SchemeSel]  = { OPAQUE, baralpha, borderalpha },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	*/
	/* class    instance      title       	 tags mask    isfloating   isterminal  noswallow  monitor */
	{ "Gimp",     NULL,       NULL,       	    1 << 8,       0,           0,         0,        -1 },
	{ TERMCLASS,  NULL,       NULL,       	    0,            0,           1,         0,        -1 },
	{ NULL,       NULL,       "Event Tester",   0,            0,           0,         1,        -1 },
	{ NULL,      "bg",        NULL,       	    0,            0,           1,    1 << 7,        -1 },
	{ NULL,      "spterm",    NULL,       	    SPTAG(0),     1,           1,         0,        -1 },
	{ NULL,      "spcalc",    NULL,       	    SPTAG(1),     1,           1,         0,        -1 },
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",	tile },			/* Default: Master on left, slaves on right */
	{ "TTT",	bstack },		/* Master on top, slaves on bottom */

	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },		/* Decreasing in size right and leftward */

	{ "[D]",	deck },			/* Master on left, slaves in monocle-like mode on right */
	{ "[M]",	monocle },		/* All windows on top of eachother */

	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },	/* Same but master floats */

	{ "><>",	NULL },			/* no layout function means floating behavior */
	{ NULL,		NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
	{ MOD,	44,	ACTION##stack,	{.i = INC(+1) } }, \
	{ MOD,	45,	ACTION##stack,	{.i = INC(-1) } }, \
	{ MOD,  55,   ACTION##stack,  {.i = 0 } }, \
	/* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
	/* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
	/* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
	/* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "color4",		STRING,	&normfgcolor },
		{ "color0",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "gappih",		INTEGER, &gappih },
		{ "gappiv",		INTEGER, &gappiv },
		{ "gappoh",		INTEGER, &gappoh },
		{ "gappov",		INTEGER, &gappov },
		{ "swallowfloating",	INTEGER, &swallowfloating },
		{ "smartgaps",		INTEGER, &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
	/* modifier                     key        function        argument */
	STACKKEYS(MODKEY,                          focus)
	STACKKEYS(MODKEY|ShiftMask,                push)
	/* { MODKEY|ShiftMask,		XK_Escape,	spawn,	SHCMD("") }, */
    { MODKEY,			49,	spawn,	{.v = (const char*[]){ "dmenuunicode", NULL } } }, //grave
	/* { MODKEY|ShiftMask,		XK_grave,	togglescratch,	SHCMD("") }, */
	TAGKEYS(			10,		0) //1
	TAGKEYS(			11,		1) //2
	TAGKEYS(			12,		2) //3
	TAGKEYS(			13,		3) //4
	TAGKEYS(			14,		4) //5
	TAGKEYS(			15,		5) //6
	TAGKEYS(			16,		6) //7
	TAGKEYS(			17,		7) //8
	TAGKEYS(			18,		8) //9
	{ MODKEY,			19,		view,		{.ui = ~0 } }, //0
	{ MODKEY|ShiftMask,		19,		tag,		{.ui = ~0 } },
	{ MODKEY,			20,	spawn,		SHCMD("pamixer --allow-boost -d 5; kill -44 $(pidof dwmblocks)") }, //minus
	{ MODKEY|ShiftMask,		20,	spawn,		SHCMD("pamixer --allow-boost -d 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			21,	spawn,		SHCMD("pamixer --allow-boost -i 5; kill -44 $(pidof dwmblocks)") }, //equal
	{ MODKEY|ShiftMask,		21,	spawn,		SHCMD("pamixer --allow-boost -i 15; kill -44 $(pidof dwmblocks)") },
	{ MODKEY,			22,	spawn,		SHCMD("sysact") }, //backspace
	{ MODKEY|ShiftMask,		22,	spawn,		SHCMD("sysact") },

	{ MODKEY,			23,		view,		{0} }, //Tab
	/* { MODKEY|ShiftMask,		23,		spawn,		SHCMD("") }, */
	{ MODKEY,			24,		killclient,	{0} }, //q
	{ MODKEY|ShiftMask,		24,		spawn,		SHCMD("sysact") },
	{ MODKEY,			25,		spawn,		SHCMD("$BROWSER") }, //w
	{ MODKEY|ShiftMask,		25,		spawn,		SHCMD("sxiv -b -t ~/pics/wallpapers") },
	{ MODKEY,			26,		spawn,		SHCMD("setxkbmap -layout us && pkill -RTMIN+22 dwmblocks") }, //e
	/*{ MODKEY|ShiftMask,		26,		spawn,		SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },*/
	{ MODKEY,			27,		spawn,		SHCMD("setxkbmap -layout ru && pkill -RTMIN+22 dwmblocks") }, //r
	{ MODKEY|ShiftMask,		27,		spawn,		SHCMD("setxkbmap -layout ru -variant csl && pkill -RTMIN+22 dwmblocks") },
	{ MODKEY,			28,		setlayout,	{.v = &layouts[0]} }, /* tile */ //t
	/*{ MODKEY|ShiftMask,		28,		setlayout,	{.v = &layouts[1]} }, bstack */
	{ MODKEY,			29,		setlayout,	{.v = &layouts[1]} }, /* bstack ### 2 = spiral (fibonacci) */ //y
	{ MODKEY|ShiftMask,		29,		togglesticky,	{0} },
	{ MODKEY,			30,		setlayout,	{.v = &layouts[4]} }, /* deck */ //u
	{ MODKEY|ShiftMask,		30,		setlayout,	{.v = &layouts[5]} }, /* monocle */
	{ MODKEY,			31,		setlayout,	{.v = &layouts[6]} }, /* centeredmaster */ //i
	{ MODKEY|ShiftMask,		31,		setlayout,	{.v = &layouts[7]} }, /* centeredfloatingmaster */
	/*{ MODKEY,			32,		incnmaster,     {.i = +1 } }, */ //o
	/*{ MODKEY|ShiftMask,		32,		incnmaster,     {.i = -1 } }, */
	{ MODKEY,			33,			spawn,		SHCMD("mpc toggle") }, //p
	{ MODKEY|ShiftMask,		33,			spawn,		SHCMD("mpc pause ; pauseallmpv") },
	{ MODKEY,			34,		spawn,		SHCMD("mpc seek -10") }, //bracketleft
	{ MODKEY|ShiftMask,		34,		spawn,		SHCMD("mpc seek -60") },
	{ MODKEY,			35,	spawn,		SHCMD("mpc seek +10") }, //bracketright
	{ MODKEY|ShiftMask,		35,	spawn,		SHCMD("mpc seek +60") },
	{ MODKEY,			51,		view,		{0} }, //backslash
	/* { MODKEY|ShiftMask,		51,		spawn,		SHCMD("") }, */

    { MODKEY,			38,		spawn,	SHCMD(TERMINAL " -e lfub") }, //a
	{ MODKEY|ShiftMask,		38,		spawn,		SHCMD(TERMINAL " -e abook -C ~/.config/abook/abookrc --datafile ~/.config/abook/addressbook") },
	{ MODKEY,			39,		spawn,  SHCMD("setxkbmap -layout rs && pkill -RTMIN+22 dwmblocks") }, //s
	{ MODKEY|ShiftMask,		39,		spawn,		SHCMD("setxkbmap -layout rs -variant latin && pkill -RTMIN+22 dwmblocks") },
	{ MODKEY,			40,		spawn,          SHCMD("dmenu_run") }, //d
	/*{ MODKEY|ShiftMask,		40,		spawn,		SHCMD("passmenu") },*/
	{ MODKEY,			41,		togglefullscr,	{0} }, //f
	{ MODKEY|ShiftMask,		41,		setlayout,	{.v = &layouts[8]} },
	{ MODKEY,			42,		spawn,	SHCMD("setxkbmap -layout gr && pkill -RTMIN+22 dwmblocks") }, //g
	/*{ MODKEY|ShiftMask,		42,		spawn,	SHCMD("dmenu_run -i -fn 'LiberationSans:size=9' -nb '#000000' & setxkbmap -layout gr") },*/
	{ MODKEY,			43,		setmfact,	{.f = -0.05} }, //h
	/* J and K are automatically bound above in STACKEYS */
	{ MODKEY,			46,		setmfact,      	{.f = +0.05} }, //l
	/*{ MODKEY,			47,	shiftview,	{ .i = 1 } },*/ //semicolon
	/*{ MODKEY|ShiftMask,		47,	shifttag,	{ .i = 1 } },*/
	{ MODKEY,			48,	togglescratch,	{.ui = 1} }, //apostrophe
	/* { MODKEY|ShiftMask,		48,	spawn,		SHCMD("") }, */
	{ MODKEY|ShiftMask,		48,	togglesmartgaps,	{0} },
	{ MODKEY,			36,	spawn,		{.v = termcmd } }, //Return
	{ MODKEY|ShiftMask,		36,	togglescratch,	{.ui = 0} },

	{ MODKEY,			52,		incrgaps,	{.i = +3 } }, //z
	/* { MODKEY|ShiftMask,		52,		spawn,		SHCMD("") }, */
	{ MODKEY,			53,		incrgaps,	{.i = -3 } }, //x
	/* { MODKEY|ShiftMask,		53,		spawn,		SHCMD("") }, */
	{ MODKEY,			54,		togglegaps,		{0} }, //c
	{ MODKEY|ShiftMask,		54,		defaultgaps,		{0} },
	/* V is automatically bound above in STACKKEYS */
	{ MODKEY,			56,		togglebar,	{0} }, //b
	{ MODKEY|ShiftMask,		56,		spawn,		SHCMD(TERMINAL " -e tremc; pkill -RTMIN+7 dwmblocks") },
	{ MODKEY,			57,		spawn,		SHCMD(TERMINAL " -e ncmpcpp; pkill -RTMIN+11 dwmblocks") }, //n
	{ MODKEY|ShiftMask,		57,		spawn,		SHCMD(TERMINAL " -e newsboat; pkill -RTMIN+6 dwmblocks") },
	{ MODKEY,			58,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") }, //m
	/*{ MODKEY|ShiftMask,		58,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },*/
	{ MODKEY,			59,	spawn,		SHCMD("mpc prev") }, //comma
	/*{ MODKEY|ShiftMask,		59,	spawn,		SHCMD("mpc seek 0%") },*/
	{ MODKEY,			60,	spawn,		SHCMD("mpc next") }, //period
	/*{ MODKEY|ShiftMask,		60,	spawn,		SHCMD("mpc repeat") },*/

	{ MODKEY,			113,	focusmon,	{.i = -1 } }, //Left
	{ MODKEY|ShiftMask,		113,	tagmon,		{.i = -1 } },
	{ MODKEY,			114,	focusmon,	{.i = +1 } }, //Right
	{ MODKEY|ShiftMask,		114,	tagmon,		{.i = +1 } },

	/*{ MODKEY,			XK_Page_Up,	shiftview,	{ .i = -1 } }, */
	/*{ MODKEY|ShiftMask,		XK_Page_Up,	shifttag,	{ .i = -1 } }, */
	/*{ MODKEY,			XK_Page_Down,	shiftview,	{ .i = +1 } }, */
	/*{ MODKEY|ShiftMask,		XK_Page_Down,	shifttag,	{ .i = +1 } },*/
	{ MODKEY,			118,	spawn,		SHCMD("xdotool type $(grep -v '^#' ~/.local/share/larbs/snippets | dmenu -i -l 50 | cut -d' ' -f1)") }, //XK_Insert

	/*{ MODKEY,			67,		spawn,		SHCMD("groff -mom /usr/local/share/dwm/larbs.mom -Tpdf | zathura -") },*/ //F1
	/*{ MODKEY,			XK_F2,		spawn,		SHCMD("tutorialvids") },*/ //F2
	{ MODKEY,			69,		spawn,		SHCMD("displayselect") }, //F3
	{ MODKEY,			70,		spawn,		SHCMD(TERMINAL " -e pulsemixer; kill -44 $(pidof dwmblocks)") }, //F4
	/* { MODKEY,		XK_F5,		xrdb,		{.v = NULL } }, */ //F5
	{ MODKEY,			72,		spawn,		SHCMD("torwrap") }, //F6
	{ MODKEY,			73,		spawn,		SHCMD("td-toggle") }, //F7
	{ MODKEY,			74,		spawn,		SHCMD("mw -Y") }, //F8
	{ MODKEY,			75,		spawn,		SHCMD("dmenumount") }, //F9
	{ MODKEY,			76,		spawn,		SHCMD("dmenuumount") }, //F10
	{ MODKEY,			95,		spawn,		SHCMD("mpv --untimed --no-cache --no-osc --no-input-default-bindings --profile=low-latency --input-conf=/dev/null --title=webcam $(ls /dev/video[0,2,4,6,8] | tail -n 1)") }, //F11
	/*{ MODKEY,			XK_F12,		spawn,		SHCMD("remaps & notify-send \\\"⌨️ Keyboard remapping...\\\" \\\"Re-running keyboard defaults for any newly plugged-in keyboards.\\\"") },*/ //F12
	{ MODKEY,			65,	zoom,		{0} }, //space
	{ MODKEY|ShiftMask,		65,	togglefloating,	{0} },

    { 0,                107,    spawn,          {.v = (const char*[]){ "maimpick", NULL } } },//print
    { ShiftMask,				107,	spawn,		SHCMD("maim ~/pics/pic-full-$(date '+%y%m%d-%H%M-%S').png && notify-send -i ~/pics/screenshot.png 'Screenshot taken.' 'Saved in ~/pics.'") },
    { MODKEY,			107,	spawn,		{.v = (const char*[]){ "dmenurecord", NULL } } },
    { MODKEY|ShiftMask,		107,	spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } },
    { MODKEY,			119,	spawn,		{.v = (const char*[]){ "dmenurecord", "kill", NULL } } }, //delete
	/*{ MODKEY,			XK_Scroll_Lock,	spawn,		SHCMD("killall screenkey || screenkey &") },*/

	{ 0, 121,		spawn,		SHCMD("pamixer -t; kill -44 $(pidof dwmblocks)") },
	{ 0, 123,	    spawn,		SHCMD("pamixer --allow-boost -i 3; kill -44 $(pidof dwmblocks)") },
	{ 0, 122,	    spawn,		SHCMD("pamixer --allow-boost -d 3; kill -44 $(pidof dwmblocks)") },
	/*{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD(TERMINAL " -e ncmpcpp") }, */
	{ 0, 198,	    spawn,		SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },
	/*{ 0, XF86XK_PowerOff,		spawn,		SHCMD("sysact") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD(TERMINAL " -e bc -l") },
	{ 0, XF86XK_Sleep,		spawn,		SHCMD("sudo -A zzz") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_DOS,		spawn,		SHCMD(TERMINAL) },
	{ 0, XF86XK_ScreenSaver,	spawn,		SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") },
	{ 0, XF86XK_TaskPane,		spawn,		SHCMD(TERMINAL " -e htop") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MyComputer,		spawn,		SHCMD(TERMINAL " -e lf /") },
	{ 0, XF86XK_Battery,		spawn,		SHCMD("") },
	{ 0, XF86XK_Launch1,		spawn,		SHCMD("xset dpms force off") },
	{ 0, XF86XK_TouchpadToggle,	spawn,		SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOff,	spawn,		SHCMD("synclient TouchpadOff=1") },
	{ 0, XF86XK_TouchpadOn,		spawn,		SHCMD("synclient TouchpadOff=0") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("xbacklight -inc 15") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("xbacklight -dec 15") }, */

	/* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
	/* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
#ifndef __OpenBSD__
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
#endif
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD(TERMINAL " -e nvim ~/.local/src/dwmblocks/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        defaultgaps,	{0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkClientWin,		MODKEY,		Button4,	incrgaps,	{.i = +1} },
	{ ClkClientWin,		MODKEY,		Button5,	incrgaps,	{.i = -1} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
	{ ClkTagBar,		0,		Button4,	shiftview,	{.i = -1} },
	{ ClkTagBar,		0,		Button5,	shiftview,	{.i = 1} },
	{ ClkRootWin,		0,		Button2,	togglebar,	{0} },
};

