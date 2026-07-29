--[[
============================================
            GENERATED FILE
============================================
]]

---@meta

-- ============================================================================
-- VERSION & CONSTANTS & ENUMS
-- ============================================================================

---@type string
VERSION = "1.0.0"

-- Pivot alignments for Shapes
---@type integer
CORNER_TOP_LEFT = 0
---@type integer
CORNER_TOP_RIGHT = 1
---@type integer
CORNER_BOTTOM_LEFT = 2
---@type integer
CORNER_BOTTOM_RIGHT = 3
---@type integer
CENTER = 4

-- Backward compatibility aliases for pivot alignments
---@type integer
TOP_LEFT_CORNER = 0
---@type integer
TOP_RIGHT_CORNER = 1
---@type integer
BOTTOM_LEFT_CORNER = 2
---@type integer
BOTTOM_RIGHT_CORNER = 3

-- Linux Input/uinput Key Codes
---@type integer
KEY_RESERVED = 0
---@type integer
KEY_ESC = 1
---@type integer
KEY_1 = 2
---@type integer
KEY_2 = 3
---@type integer
KEY_3 = 4
---@type integer
KEY_4 = 5
---@type integer
KEY_5 = 6
---@type integer
KEY_6 = 7
---@type integer
KEY_7 = 8
---@type integer
KEY_8 = 9
---@type integer
KEY_9 = 10
---@type integer
KEY_0 = 11
---@type integer
KEY_MINUS = 12
---@type integer
KEY_EQUAL = 13
---@type integer
KEY_BACKSPACE = 14
---@type integer
KEY_TAB = 15
---@type integer
KEY_Q = 16
---@type integer
KEY_W = 17
---@type integer
KEY_E = 18
---@type integer
KEY_R = 19
---@type integer
KEY_T = 20
---@type integer
KEY_Y = 21
---@type integer
KEY_U = 22
---@type integer
KEY_I = 23
---@type integer
KEY_O = 24
---@type integer
KEY_P = 25
---@type integer
KEY_LEFTBRACE = 26
---@type integer
KEY_RIGHTBRACE = 27
---@type integer
KEY_ENTER = 28
---@type integer
KEY_LEFTCTRL = 29
---@type integer
KEY_A = 30
---@type integer
KEY_S = 31
---@type integer
KEY_D = 32
---@type integer
KEY_F = 33
---@type integer
KEY_G = 34
---@type integer
KEY_H = 35
---@type integer
KEY_J = 36
---@type integer
KEY_K = 37
---@type integer
KEY_L = 38
---@type integer
KEY_SEMICOLON = 39
---@type integer
KEY_APOSTROPHE = 40
---@type integer
KEY_GRAVE = 41
---@type integer
KEY_LEFTSHIFT = 42
---@type integer
KEY_BACKSLASH = 43
---@type integer
KEY_Z = 44
---@type integer
KEY_X = 45
---@type integer
KEY_C = 46
---@type integer
KEY_V = 47
---@type integer
KEY_B = 48
---@type integer
KEY_N = 49
---@type integer
KEY_M = 50
---@type integer
KEY_COMMA = 51
---@type integer
KEY_DOT = 52
---@type integer
KEY_SLASH = 53
---@type integer
KEY_RIGHTSHIFT = 54
---@type integer
KEY_KPASTERISK = 55
---@type integer
KEY_LEFTALT = 56
---@type integer
KEY_SPACE = 57
---@type integer
KEY_CAPSLOCK = 58
---@type integer
KEY_F1 = 59
---@type integer
KEY_F2 = 60
---@type integer
KEY_F3 = 61
---@type integer
KEY_F4 = 62
---@type integer
KEY_F5 = 63
---@type integer
KEY_F6 = 64
---@type integer
KEY_F7 = 65
---@type integer
KEY_F8 = 66
---@type integer
KEY_F9 = 67
---@type integer
KEY_F10 = 68
---@type integer
KEY_NUMLOCK = 69
---@type integer
KEY_SCROLLLOCK = 70
---@type integer
KEY_KP7 = 71
---@type integer
KEY_KP8 = 72
---@type integer
KEY_KP9 = 73
---@type integer
KEY_KPMINUS = 74
---@type integer
KEY_KP4 = 75
---@type integer
KEY_KP5 = 76
---@type integer
KEY_KP6 = 77
---@type integer
KEY_KPPLUS = 78
---@type integer
KEY_KP1 = 79
---@type integer
KEY_KP2 = 80
---@type integer
KEY_KP3 = 81
---@type integer
KEY_KP0 = 82
---@type integer
KEY_KPDOT = 83
---@type integer
KEY_ZENKAKUHANKAKU = 85
---@type integer
KEY_102ND = 86
---@type integer
KEY_F11 = 87
---@type integer
KEY_F12 = 88
---@type integer
KEY_RO = 89
---@type integer
KEY_KATAKANA = 90
---@type integer
KEY_HIRAGANA = 91
---@type integer
KEY_HENKAN = 92
---@type integer
KEY_KATAKANAHIRAGANA = 93
---@type integer
KEY_MUHENKAN = 94
---@type integer
KEY_KPJPCOMMA = 95
---@type integer
KEY_KPENTER = 96
---@type integer
KEY_RIGHTCTRL = 97
---@type integer
KEY_KPSLASH = 98
---@type integer
KEY_SYSRQ = 99
---@type integer
KEY_RIGHTALT = 100
---@type integer
KEY_LINEFEED = 101
---@type integer
KEY_HOME = 102
---@type integer
KEY_UP = 103
---@type integer
KEY_PAGEUP = 104
---@type integer
KEY_LEFT = 105
---@type integer
KEY_RIGHT = 106
---@type integer
KEY_END = 107
---@type integer
KEY_DOWN = 108
---@type integer
KEY_PAGEDOWN = 109
---@type integer
KEY_INSERT = 110
---@type integer
KEY_DELETE = 111
---@type integer
KEY_MACRO = 112
---@type integer
KEY_MIN_INTERESTING = 113
---@type integer
KEY_MUTE = 113
---@type integer
KEY_VOLUMEDOWN = 114
---@type integer
KEY_VOLUMEUP = 115
---@type integer
KEY_POWER = 116
---@type integer
KEY_KPEQUAL = 117
---@type integer
KEY_KPPLUSMINUS = 118
---@type integer
KEY_PAUSE = 119
---@type integer
KEY_SCALE = 120
---@type integer
KEY_KPCOMMA = 121
---@type integer
KEY_HANGEUL = 122
---@type integer
KEY_HANGUEL = 122
---@type integer
KEY_HANJA = 123
---@type integer
KEY_YEN = 124
---@type integer
KEY_LEFTMETA = 125
---@type integer
KEY_RIGHTMETA = 126
---@type integer
KEY_COMPOSE = 127
---@type integer
KEY_STOP = 128
---@type integer
KEY_AGAIN = 129
---@type integer
KEY_PROPS = 130
---@type integer
KEY_UNDO = 131
---@type integer
KEY_FRONT = 132
---@type integer
KEY_COPY = 133
---@type integer
KEY_OPEN = 134
---@type integer
KEY_PASTE = 135
---@type integer
KEY_FIND = 136
---@type integer
KEY_CUT = 137
---@type integer
KEY_HELP = 138
---@type integer
KEY_MENU = 139
---@type integer
KEY_CALC = 140
---@type integer
KEY_SETUP = 141
---@type integer
KEY_SLEEP = 142
---@type integer
KEY_WAKEUP = 143
---@type integer
KEY_FILE = 144
---@type integer
KEY_SENDFILE = 145
---@type integer
KEY_DELETEFILE = 146
---@type integer
KEY_XFER = 147
---@type integer
KEY_PROG1 = 148
---@type integer
KEY_PROG2 = 149
---@type integer
KEY_WWW = 150
---@type integer
KEY_MSDOS = 151
---@type integer
KEY_COFFEE = 152
---@type integer
KEY_SCREENLOCK = 152
---@type integer
KEY_DIRECTION = 153
---@type integer
KEY_ROTATE_DISPLAY = 153
---@type integer
KEY_CYCLEWINDOWS = 154
---@type integer
KEY_MAIL = 155
---@type integer
KEY_BOOKMARKS = 156
---@type integer
KEY_COMPUTER = 157
---@type integer
KEY_BACK = 158
---@type integer
KEY_FORWARD = 159
---@type integer
KEY_CLOSECD = 160
---@type integer
KEY_EJECTCD = 161
---@type integer
KEY_EJECTCLOSECD = 162
---@type integer
KEY_NEXTSONG = 163
---@type integer
KEY_PLAYPAUSE = 164
---@type integer
KEY_PREVIOUSSONG = 165
---@type integer
KEY_STOPCD = 166
---@type integer
KEY_RECORD = 167
---@type integer
KEY_REWIND = 168
---@type integer
KEY_PHONE = 169
---@type integer
KEY_ISO = 170
---@type integer
KEY_CONFIG = 171
---@type integer
KEY_HOMEPAGE = 172
---@type integer
KEY_REFRESH = 173
---@type integer
KEY_EXIT = 174
---@type integer
KEY_MOVE = 175
---@type integer
KEY_EDIT = 176
---@type integer
KEY_SCROLLUP = 177
---@type integer
KEY_SCROLLDOWN = 178
---@type integer
KEY_KPLEFTPAREN = 179
---@type integer
KEY_KPRIGHTPAREN = 180
---@type integer
KEY_NEW = 181
---@type integer
KEY_REDO = 182
---@type integer
KEY_F13 = 183
---@type integer
KEY_F14 = 184
---@type integer
KEY_F15 = 185
---@type integer
KEY_F16 = 186
---@type integer
KEY_F17 = 187
---@type integer
KEY_F18 = 188
---@type integer
KEY_F19 = 189
---@type integer
KEY_F20 = 190
---@type integer
KEY_F21 = 191
---@type integer
KEY_F22 = 192
---@type integer
KEY_F23 = 193
---@type integer
KEY_F24 = 194
---@type integer
KEY_PLAYCD = 200
---@type integer
KEY_PAUSECD = 201
---@type integer
KEY_PROG3 = 202
---@type integer
KEY_PROG4 = 203
---@type integer
KEY_ALL_APPLICATIONS = 204
---@type integer
KEY_DASHBOARD = 204
---@type integer
KEY_SUSPEND = 205
---@type integer
KEY_CLOSE = 206
---@type integer
KEY_PLAY = 207
---@type integer
KEY_FASTFORWARD = 208
---@type integer
KEY_BASSBOOST = 209
---@type integer
KEY_PRINT = 210
---@type integer
KEY_HP = 211
---@type integer
KEY_CAMERA = 212
---@type integer
KEY_SOUND = 213
---@type integer
KEY_QUESTION = 214
---@type integer
KEY_EMAIL = 215
---@type integer
KEY_CHAT = 216
---@type integer
KEY_SEARCH = 217
---@type integer
KEY_CONNECT = 218
---@type integer
KEY_FINANCE = 219
---@type integer
KEY_SPORT = 220
---@type integer
KEY_SHOP = 221
---@type integer
KEY_ALTERASE = 222
---@type integer
KEY_CANCEL = 223
---@type integer
KEY_BRIGHTNESSDOWN = 224
---@type integer
KEY_BRIGHTNESSUP = 225
---@type integer
KEY_MEDIA = 226
---@type integer
KEY_SWITCHVIDEOMODE = 227
---@type integer
KEY_KBDILLUMTOGGLE = 228
---@type integer
KEY_KBDILLUMDOWN = 229
---@type integer
KEY_KBDILLUMUP = 230
---@type integer
KEY_SEND = 231
---@type integer
KEY_REPLY = 232
---@type integer
KEY_FORWARDMAIL = 233
---@type integer
KEY_SAVE = 234
---@type integer
KEY_DOCUMENTS = 235
---@type integer
KEY_BATTERY = 236
---@type integer
KEY_BLUETOOTH = 237
---@type integer
KEY_WLAN = 238
---@type integer
KEY_UWB = 239
---@type integer
KEY_UNKNOWN = 240
---@type integer
KEY_VIDEO_NEXT = 241
---@type integer
KEY_VIDEO_PREV = 242
---@type integer
KEY_BRIGHTNESS_CYCLE = 243
---@type integer
KEY_BRIGHTNESS_AUTO = 244
---@type integer
KEY_BRIGHTNESS_ZERO = 244
---@type integer
KEY_DISPLAY_OFF = 245
---@type integer
KEY_WIMAX = 246
---@type integer
KEY_WWAN = 246
---@type integer
KEY_RFKILL = 247
---@type integer
KEY_MICMUTE = 248
---@type integer
KEY_OK = 352
---@type integer
KEY_SELECT = 353
---@type integer
KEY_GOTO = 354
---@type integer
KEY_CLEAR = 355
---@type integer
KEY_POWER2 = 356
---@type integer
KEY_OPTION = 357
---@type integer
KEY_INFO = 358
---@type integer
KEY_TIME = 359
---@type integer
KEY_VENDOR = 360
---@type integer
KEY_ARCHIVE = 361
---@type integer
KEY_PROGRAM = 362
---@type integer
KEY_CHANNEL = 363
---@type integer
KEY_FAVORITES = 364
---@type integer
KEY_EPG = 365
---@type integer
KEY_PVR = 366
---@type integer
KEY_MHP = 367
---@type integer
KEY_LANGUAGE = 368
---@type integer
KEY_TITLE = 369
---@type integer
KEY_SUBTITLE = 370
---@type integer
KEY_ANGLE = 371
---@type integer
KEY_FULL_SCREEN = 372
---@type integer
KEY_ZOOM = 372
---@type integer
KEY_MODE = 373
---@type integer
KEY_KEYBOARD = 374
---@type integer
KEY_ASPECT_RATIO = 375
---@type integer
KEY_SCREEN = 375
---@type integer
KEY_PC = 376
---@type integer
KEY_TV = 377
---@type integer
KEY_TV2 = 378
---@type integer
KEY_VCR = 379
---@type integer
KEY_VCR2 = 380
---@type integer
KEY_SAT = 381
---@type integer
KEY_SAT2 = 382
---@type integer
KEY_CD = 383
---@type integer
KEY_TAPE = 384
---@type integer
KEY_RADIO = 385
---@type integer
KEY_TUNER = 386
---@type integer
KEY_PLAYER = 387
---@type integer
KEY_TEXT = 388
---@type integer
KEY_DVD = 389
---@type integer
KEY_AUX = 390
---@type integer
KEY_MP3 = 391
---@type integer
KEY_AUDIO = 392
---@type integer
KEY_VIDEO = 393
---@type integer
KEY_DIRECTORY = 394
---@type integer
KEY_LIST = 395
---@type integer
KEY_MEMO = 396
---@type integer
KEY_CALENDAR = 397
---@type integer
KEY_RED = 398
---@type integer
KEY_GREEN = 399
---@type integer
KEY_YELLOW = 400
---@type integer
KEY_BLUE = 401
---@type integer
KEY_CHANNELUP = 402
---@type integer
KEY_CHANNELDOWN = 403
---@type integer
KEY_FIRST = 404
---@type integer
KEY_LAST = 405
---@type integer
KEY_AB = 406
---@type integer
KEY_NEXT = 407
---@type integer
KEY_RESTART = 408
---@type integer
KEY_SLOW = 409
---@type integer
KEY_SHUFFLE = 410
---@type integer
KEY_BREAK = 411
---@type integer
KEY_PREVIOUS = 412
---@type integer
KEY_DIGITS = 413
---@type integer
KEY_TEEN = 414
---@type integer
KEY_TWEN = 415
---@type integer
KEY_VIDEOPHONE = 416
---@type integer
KEY_GAMES = 417
---@type integer
KEY_ZOOMIN = 418
---@type integer
KEY_ZOOMOUT = 419
---@type integer
KEY_ZOOMRESET = 420
---@type integer
KEY_WORDPROCESSOR = 421
---@type integer
KEY_EDITOR = 422
---@type integer
KEY_SPREADSHEET = 423
---@type integer
KEY_GRAPHICSEDITOR = 424
---@type integer
KEY_PRESENTATION = 425
---@type integer
KEY_DATABASE = 426
---@type integer
KEY_NEWS = 427
---@type integer
KEY_VOICEMAIL = 428
---@type integer
KEY_ADDRESSBOOK = 429
---@type integer
KEY_MESSENGER = 430
---@type integer
KEY_BRIGHTNESS_TOGGLE = 431
---@type integer
KEY_DISPLAYTOGGLE = 431
---@type integer
KEY_SPELLCHECK = 432
---@type integer
KEY_LOGOFF = 433
---@type integer
KEY_DOLLAR = 434
---@type integer
KEY_EURO = 435
---@type integer
KEY_FRAMEBACK = 436
---@type integer
KEY_FRAMEFORWARD = 437
---@type integer
KEY_CONTEXT_MENU = 438
---@type integer
KEY_MEDIA_REPEAT = 439
---@type integer
KEY_10CHANNELSUP = 440
---@type integer
KEY_10CHANNELSDOWN = 441
---@type integer
KEY_IMAGES = 442
---@type integer
KEY_NOTIFICATION_CENTER = 444
---@type integer
KEY_PICKUP_PHONE = 445
---@type integer
KEY_HANGUP_PHONE = 446
---@type integer
KEY_LINK_PHONE = 447
---@type integer
KEY_DEL_EOL = 448
---@type integer
KEY_DEL_EOS = 449
---@type integer
KEY_INS_LINE = 450
---@type integer
KEY_DEL_LINE = 451
---@type integer
KEY_FN = 464
---@type integer
KEY_FN_ESC = 465
---@type integer
KEY_FN_F1 = 466
---@type integer
KEY_FN_F2 = 467
---@type integer
KEY_FN_F3 = 468
---@type integer
KEY_FN_F4 = 469
---@type integer
KEY_FN_F5 = 470
---@type integer
KEY_FN_F6 = 471
---@type integer
KEY_FN_F7 = 472
---@type integer
KEY_FN_F8 = 473
---@type integer
KEY_FN_F9 = 474
---@type integer
KEY_FN_F10 = 475
---@type integer
KEY_FN_F11 = 476
---@type integer
KEY_FN_F12 = 477
---@type integer
KEY_FN_1 = 478
---@type integer
KEY_FN_2 = 479
---@type integer
KEY_FN_D = 480
---@type integer
KEY_FN_E = 481
---@type integer
KEY_FN_F = 482
---@type integer
KEY_FN_S = 483
---@type integer
KEY_FN_B = 484
---@type integer
KEY_FN_RIGHT_SHIFT = 485
---@type integer
KEY_BRL_DOT1 = 497
---@type integer
KEY_BRL_DOT2 = 498
---@type integer
KEY_BRL_DOT3 = 499
---@type integer
KEY_BRL_DOT4 = 500
---@type integer
KEY_BRL_DOT5 = 501
---@type integer
KEY_BRL_DOT6 = 502
---@type integer
KEY_BRL_DOT7 = 503
---@type integer
KEY_BRL_DOT8 = 504
---@type integer
KEY_BRL_DOT9 = 505
---@type integer
KEY_BRL_DOT10 = 506
---@type integer
KEY_NUMERIC_0 = 512
---@type integer
KEY_NUMERIC_1 = 513
---@type integer
KEY_NUMERIC_2 = 514
---@type integer
KEY_NUMERIC_3 = 515
---@type integer
KEY_NUMERIC_4 = 516
---@type integer
KEY_NUMERIC_5 = 517
---@type integer
KEY_NUMERIC_6 = 518
---@type integer
KEY_NUMERIC_7 = 519
---@type integer
KEY_NUMERIC_8 = 520
---@type integer
KEY_NUMERIC_9 = 521
---@type integer
KEY_NUMERIC_STAR = 522
---@type integer
KEY_NUMERIC_POUND = 523
---@type integer
KEY_NUMERIC_A = 524
---@type integer
KEY_NUMERIC_B = 525
---@type integer
KEY_NUMERIC_C = 526
---@type integer
KEY_NUMERIC_D = 527
---@type integer
KEY_CAMERA_FOCUS = 528
---@type integer
KEY_WPS_BUTTON = 529
---@type integer
KEY_TOUCHPAD_TOGGLE = 530
---@type integer
KEY_TOUCHPAD_ON = 531
---@type integer
KEY_TOUCHPAD_OFF = 532
---@type integer
KEY_CAMERA_ZOOMIN = 533
---@type integer
KEY_CAMERA_ZOOMOUT = 534
---@type integer
KEY_CAMERA_UP = 535
---@type integer
KEY_CAMERA_DOWN = 536
---@type integer
KEY_CAMERA_LEFT = 537
---@type integer
KEY_CAMERA_RIGHT = 538
---@type integer
KEY_ATTENDANT_ON = 539
---@type integer
KEY_ATTENDANT_OFF = 540
---@type integer
KEY_ATTENDANT_TOGGLE = 541
---@type integer
KEY_LIGHTS_TOGGLE = 542
---@type integer
KEY_ALS_TOGGLE = 560
---@type integer
KEY_ROTATE_LOCK_TOGGLE = 561
---@type integer
KEY_REFRESH_RATE_TOGGLE = 562
---@type integer
KEY_BUTTONCONFIG = 576
---@type integer
KEY_TASKMANAGER = 577
---@type integer
KEY_JOURNAL = 578
---@type integer
KEY_CONTROLPANEL = 579
---@type integer
KEY_APPSELECT = 580
---@type integer
KEY_SCREENSAVER = 581
---@type integer
KEY_VOICECOMMAND = 582
---@type integer
KEY_ASSISTANT = 583
---@type integer
KEY_KBD_LAYOUT_NEXT = 584
---@type integer
KEY_EMOJI_PICKER = 585
---@type integer
KEY_DICTATE = 586
---@type integer
KEY_CAMERA_ACCESS_ENABLE = 587
---@type integer
KEY_CAMERA_ACCESS_DISABLE = 588
---@type integer
KEY_CAMERA_ACCESS_TOGGLE = 589
---@type integer
KEY_ACCESSIBILITY = 590
---@type integer
KEY_DO_NOT_DISTURB = 591
---@type integer
KEY_BRIGHTNESS_MIN = 592
---@type integer
KEY_EPRIVACY_SCREEN_ON = 594
---@type integer
KEY_EPRIVACY_SCREEN_OFF = 595
---@type integer
KEY_ACTION_ON_SELECTION = 596
---@type integer
KEY_CONTEXTUAL_INSERT = 597
---@type integer
KEY_CONTEXTUAL_QUERY = 598
---@type integer
KEY_KBDINPUTASSIST_PREV = 608
---@type integer
KEY_KBDINPUTASSIST_NEXT = 609
---@type integer
KEY_KBDINPUTASSIST_PREVGROUP = 610
---@type integer
KEY_KBDINPUTASSIST_NEXTGROUP = 611
---@type integer
KEY_KBDINPUTASSIST_ACCEPT = 612
---@type integer
KEY_KBDINPUTASSIST_CANCEL = 613
---@type integer
KEY_RIGHT_UP = 614
---@type integer
KEY_RIGHT_DOWN = 615
---@type integer
KEY_LEFT_UP = 616
---@type integer
KEY_LEFT_DOWN = 617
---@type integer
KEY_ROOT_MENU = 618
---@type integer
KEY_MEDIA_TOP_MENU = 619
---@type integer
KEY_NUMERIC_11 = 620
---@type integer
KEY_NUMERIC_12 = 621
---@type integer
KEY_AUDIO_DESC = 622
---@type integer
KEY_3D_MODE = 623
---@type integer
KEY_NEXT_FAVORITE = 624
---@type integer
KEY_STOP_RECORD = 625
---@type integer
KEY_PAUSE_RECORD = 626
---@type integer
KEY_VOD = 627
---@type integer
KEY_UNMUTE = 628
---@type integer
KEY_FASTREVERSE = 629
---@type integer
KEY_SLOWREVERSE = 630
---@type integer
KEY_DATA = 631
---@type integer
KEY_ONSCREEN_KEYBOARD = 632
---@type integer
KEY_PRIVACY_SCREEN_TOGGLE = 633
---@type integer
KEY_SELECTIVE_SCREENSHOT = 634
---@type integer
KEY_NEXT_ELEMENT = 635
---@type integer
KEY_PREVIOUS_ELEMENT = 636
---@type integer
KEY_AUTOPILOT_ENGAGE_TOGGLE = 637
---@type integer
KEY_MARK_WAYPOINT = 638
---@type integer
KEY_SOS = 639
---@type integer
KEY_NAV_CHART = 640
---@type integer
KEY_FISHING_CHART = 641
---@type integer
KEY_SINGLE_RANGE_RADAR = 642
---@type integer
KEY_DUAL_RANGE_RADAR = 643
---@type integer
KEY_RADAR_OVERLAY = 644
---@type integer
KEY_TRADITIONAL_SONAR = 645
---@type integer
KEY_CLEARVU_SONAR = 646
---@type integer
KEY_SIDEVU_SONAR = 647
---@type integer
KEY_NAV_INFO = 648
---@type integer
KEY_BRIGHTNESS_MENU = 649
---@type integer
KEY_MACRO1 = 656
---@type integer
KEY_MACRO2 = 657
---@type integer
KEY_MACRO3 = 658
---@type integer
KEY_MACRO4 = 659
---@type integer
KEY_MACRO5 = 660
---@type integer
KEY_MACRO6 = 661
---@type integer
KEY_MACRO7 = 662
---@type integer
KEY_MACRO8 = 663
---@type integer
KEY_MACRO9 = 664
---@type integer
KEY_MACRO10 = 665
---@type integer
KEY_MACRO11 = 666
---@type integer
KEY_MACRO12 = 667
---@type integer
KEY_MACRO13 = 668
---@type integer
KEY_MACRO14 = 669
---@type integer
KEY_MACRO15 = 670
---@type integer
KEY_MACRO16 = 671
---@type integer
KEY_MACRO17 = 672
---@type integer
KEY_MACRO18 = 673
---@type integer
KEY_MACRO19 = 674
---@type integer
KEY_MACRO20 = 675
---@type integer
KEY_MACRO21 = 676
---@type integer
KEY_MACRO22 = 677
---@type integer
KEY_MACRO23 = 678
---@type integer
KEY_MACRO24 = 679
---@type integer
KEY_MACRO25 = 680
---@type integer
KEY_MACRO26 = 681
---@type integer
KEY_MACRO27 = 682
---@type integer
KEY_MACRO28 = 683
---@type integer
KEY_MACRO29 = 684
---@type integer
KEY_MACRO30 = 685
---@type integer
KEY_MACRO_RECORD_START = 688
---@type integer
KEY_MACRO_RECORD_STOP = 689
---@type integer
KEY_MACRO_PRESET_CYCLE = 690
---@type integer
KEY_MACRO_PRESET1 = 691
---@type integer
KEY_MACRO_PRESET2 = 692
---@type integer
KEY_MACRO_PRESET3 = 693
---@type integer
KEY_KBD_LCD_MENU1 = 696
---@type integer
KEY_KBD_LCD_MENU2 = 697
---@type integer
KEY_KBD_LCD_MENU3 = 698
---@type integer
KEY_KBD_LCD_MENU4 = 699
---@type integer
KEY_KBD_LCD_MENU5 = 700
---@type integer
KEY_PERFORMANCE = 701

-- ============================================================================
-- CLASSES & TYPES
-- ============================================================================

---@class Vector2D
---@field x number
---@field y number
---@operator add(Vector2D|number): Vector2D
---@operator sub(Vector2D|number): Vector2D
---@operator mul(Vector2D|number): Vector2D
---@operator div(Vector2D|number): Vector2D
---@overload fun(): Vector2D
---@overload fun(val: number): Vector2D
---@overload fun(x: number, y: number): Vector2D
---@overload fun(args: {x?: number, y?: number}): Vector2D
Vector2D = {}

---@param x number
function Vector2D:setX(x) end

---@param y number
function Vector2D:setY(y) end

---@param x number
---@param y number
function Vector2D:setBoth(x, y) end

---@param amount number
function Vector2D:moveX(amount) end

---@param amount number
function Vector2D:moveY(amount) end

---@param x number
---@param y number
function Vector2D:moveBoth(x, y) end

---@return number
function Vector2D:getX() end

---@return number
function Vector2D:getY() end

---@param other Vector2D
---@return number
function Vector2D:distanceTo(other) end

---@return string
function Vector2D:toString() end

---@class Shape
---@field position Vector2D
---@field dimensions Vector2D
---@field pivote integer
Shape = {}

---@return Vector2D
function Shape:getPosition() end

---@return Vector2D
function Shape:getDimensions() end

---@param point Vector2D
---@return boolean
function Shape:contains(point) end

---@return string
function Shape:toString() end

---@class Rectangle : Shape
---@overload fun(): Rectangle
---@overload fun(position: Vector2D, dimensions: Vector2D, pivote?: integer): Rectangle
---@overload fun(args: {position: Vector2D, dimensions: Vector2D, pivote?: integer}): Rectangle
Rectangle = {}


---@class Circle : Shape
---@field radius number
---@overload fun(): Circle
---@overload fun(position: Vector2D, radius: number): Circle
---@overload fun(args: {position: Vector2D, radius: number}): Circle
Circle = {}


-- Opaque Region object returned by the region function
---@class Region


-- ============================================================================
-- GLOBAL FUNCTIONS
-- ============================================================================

---Creates a 2D Vector
---@overload fun(value: number): Vector2D
---@overload fun(x: number, y: number): Vector2D
---@return Vector2D
function vec2() end

---Creates a rectangular shape region
---@param args {position: Vector2D, dimensions: Vector2D, pivote?: integer}
---@return Rectangle
function rect(args) end

---Creates a circular shape region
---@param args {position: Vector2D, radius: number}
---@return Circle
function circle(args) end

---@class RegionOpts
---@field shape Shape
---@field holdDuration? integer Optional hold time required in milliseconds
---@field performAfterRelease? boolean Action triggers upon release instead of press (default: false)
---@field action? fun()|nil Optional callback action handler

---Defines an interactive touch/click region
---@param args RegionOpts
---@return Region|nil
function region(args) end

---@class ActivationOpts
---@field shape Shape
---@field holdDuration? integer Optional hold time required in milliseconds
---@field idleTimeoutSeconds? integer Optional duration after which, the numberpad turns off
---@field toggleOffOnIdle? boolean Optional toggle off the numberpad on idle (default: false)

---Sets the activation region and hold duration
---@param args ActivationOpts
function activation(args) end

---Triggers a keypress event
---@param keyCode integer
---@return (fun(): nil)|nil
function keyPress(keyCode) end

---Executes a system shell command
---@param command string
---@return (fun(): nil)|nil
function execute(command) end

---Switches to the specified profile
---@param profileName string
---@return (fun(): nil)|nil
function useProfile(profileName) end

---@class ProfileOpts
---@field name string Name of the profile
---@field regions Region[] List of regions belonging to this profile

---Registers a profile configuration
---@param args ProfileOpts
function profile(args) end
