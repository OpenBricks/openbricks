# -*- coding: iso-8859-1 -*-
# -----------------------------------------------------------------------------
# freevo_config.py - System configuration
# -----------------------------------------------------------------------------
# $Id: local_conf.py.example 8140 2006-04-07 17:20:56Z dmeyer $
#
# WARNING: this file is not up to date!
#
# Notes:
#
#    This is no normal config file, it's Python code. Because of that, you 
#    need to follow some rules to avoid crashes. The examples should explain
#    the settings, but make sure a line starting with a variable has 
#    NO SPACES OR TABS at the beginning.
#
# -----------------------------------------------------------------------------
# Freevo - A Home Theater PC framework
# Copyright (C) 2002-2005 Krister Lagerstrom, Dirk Meyer, et al.
#
# Maintainer:    Dirk Meyer <dischi@freevo.org>
#
# Please see the file doc/CREDITS for a complete list of authors.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MER-
# CHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
# Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#
# -----------------------------------------------------------------------------



CONFIG_VERSION = 8.0

# ======================================================================
# General freevo settings:
# ======================================================================

# AUDIO_DEVICE        = '/dev/dsp'      # e.g.: /dev/dsp0, /dev/audio
# AUDIO_INPUT_DEVICE  = '/dev/dsp1'     # e.g.: /dev/dsp0, /dev/audio
# MAJOR_AUDIO_CTRL    = 'VOL'           # Freevo takes control over one audio ctrl
                                        # 'VOL', 'PCM' 'OGAIN' etc.
# CONTROL_ALL_AUDIO   = 1               # Should Freevo take complete control of audio
# MAX_VOLUME          = 90              # Set what you want maximum volume level to be.
# DEFAULT_VOLUME      = 40              # Set default volume level.
# TV_IN_VOLUME        = 60              # Set this to your preferred level 0-100.
# VCR_IN_VOLUME       = 90              # If you use different input from TV
# DEV_MIXER           = '/dev/mixer'    # mixer device 

# START_FULLSCREEN_X  = 0               # Start in fullscreen mode if using x11 or xv.

# CONFIRM_SHUTDOWN    = 1               # ask before shutdown

#
# Physical ROM drives, multiple ones can be specified
# by adding comma-seperated and quoted entries.
#
# Format [ ('mountdir1', 'devicename1', 'displayed name1'),
#          ('mountdir2', 'devicename2', 'displayed name2'), ...]
#
# Set to None to autodetect drives in during startup from /etc/fstab,
# set to [] to disable rom drive support at all
#
ROM_DRIVES = None

#
# Hide discs from the wrong menu (e.g. VCDs in audio menu) and empty discs.
#
HIDE_UNUSABLE_DISCS = 1               

#
# Attempt to set the speed of the ROM drive. A good value for keeping the
# drive silent while playing movies is 8.
#
ROM_SPEED = 8                

#
# Perform a whole system shutdown at SHUTDOWN! Useful for standalone boxes.
#
# ENABLE_SHUTDOWN_SYS = 0

#
# You can add more keybindings by adding them to the correct hash. 
# e.g. If you want to send 'contrast -100' to mplayer by pressing the '1' key, 
# just add the following line: 
#
# EVENTS['video']['1'] = Event(VIDEO_SEND_MPLAYER_CMD, arg='contrast -100')
#
# See src/event.py for a list of all possible events.

#
# Use arrow keys for back and select (alternate way of navigating)
#
# MENU_ARROW_NAVIGATION = 0

#
# Keymap to map keyboard keys to event strings. You can also add new keys
# here, e.g. KEYMAP[K_x] = 'SUBTITLE'. The K_-names are defined by pygame.
#

#
# Use Internet resources to fetch information?
# For example, Freevo can use CDDB for album information,
# the IMDB movie database for movie info, and Amazon for cover searches. 
# Set this to 0 if your computer isn't connected to a network.
#
USE_NETWORK = 1

#
# Directory location to save files when the normal filesystem
# doesn't allow saving. This directory can save covers and fxd files
# for read only filesystems like ROM drives. Set this variable to your
# old MOVIE_DATA_DIR if you have one. It needs to be set to a directory
# Freevo can write to.
#
# OVERLAY_DIR = os.path.join(FREEVO_CACHEDIR, 'vfs')

#
# Umask setting for all files.
# 022 means only the user has write access. If you share your Freevo
# installation with different users, set this to 002
#
# UMASK = 022

#
# Suffix for playlist files
#
# PLAYLIST_SUFFIX = [ 'm3u' ]

#
# Use md5 in mmpython to create unique disc ids. Enable this if you have
# problems with different discs having the same id.
#
# MMPYTHON_CREATE_MD5_ID = 0

#
# Keep metadata in memory
# Setting this variable will keep all cache files in memory. Startup will be
# slower, but for large directories, this will speed up the display.
# 0 = Only keep current dir in memory. Use this if you have too much data
#     and not enough RAM
# 1 = Once loaded, keep cachefile for directory in memory
# 2 = Load all cachefiles on startup
#
# WARNING: you should not run 'freevo cache' when freevo is running.
#
# MEDIAINFO_USE_MEMORY   = 1

#
# Cache images. This uses a lot of disc space but it's a huge speed
# enhancement. The images will be cached in OVERLAY_DIR
#
CACHE_IMAGES = 1


# ======================================================================
# Plugins:
# ======================================================================

# Remove undesired plugins by setting plugin.remove(code). 
# You can also use the name to remove a plugin. But if you do that, 
# all instances of this plugin will be removed.
#
# Examples:
# plugin.remove(plugin_tv) or
# plugin.remove('tv') will remove the tv module from the main menu
# plugin.remove(rom_plugins['image']) will remove the rom drives from the
#   image main menu,
# plugin.remove('rom_drives.rom_items') will remove the rom drives from all
#   menus
#
# See freevo_config.py for a list of loaded plugins

#
# Use ivtv_record instead if you have an ivtv based card (PVR-250/350)
# and want freevo to do everthing for you.  To use you need to set the 
# following two lines:
#
# plugin.remove('record.generic_record')
# plugin.activate('record.ivtv_record')

#
# Enable this for joystick support:
plugin.activate('input.joystick')

# ======================================================================
# Freevo directory settings:
# ======================================================================

# You can change all this variables in the folder.fxd on a per folder
# basis
#
# Example:
# <freevo>
#   <folder title="Title of the directory" img-cover="nice-cover.png">
#     <setvar name="directory_autoplay_single_item" val="0"/>
#     <info>
#       <content>A small description of the directory</content>
#     </info>
#   </folder>
# </freevo>

#
# Should directories sorted by date instead of filename?
# 0 = No, always sort by filename.
# 1 = Yes, sort by date
# 2 = No, don't sory by date for normal directories, 
#     but sort by date for TV_RECORD_DIR.
#
# DIRECTORY_SORT_BY_DATE = 2

#
# Should directory items be sorted in reverse order?
#
# DIRECTORY_REVERSE_SORT = 0

#
# Should we use "smart" sorting?
# Smart sorting ignores the word "The" in item names.
#
# DIRECTORY_SMART_SORT = 0

#
# Should Freevo autoplay an item if only one item is in the directory?
#
# DIRECTORY_AUTOPLAY_SINGLE_ITEM = 1

#
# Force the skin to use a specific layout number. -1 == no force. The layout
# toggle with DISPLAY will be disabled
#
# DIRECTORY_FORCE_SKIN_LAYOUT = -1

#
# Format string for the audio item names. 
#
# Possible strings:
# a = artist, n = tracknumber, t = title, y = year, f = filename
#
# Example:
# This will show the title and the track number:
DIRECTORY_AUDIO_FORMAT_STRING = '%(n)s - %(t)s'

#
# Use media id tags to generate the name of the item. This should be
# enabled all the time. It should only be disabled for directories with 
# broken tags.
#
DIRECTORY_USE_MEDIAID_TAG_NAMES = 1

#
# The following settings determine which features are available for
# which media types.
#
# If you set this variable in a folder.fxd, the value is 1 (enabled)
# or 0 (disabled).
# 
# Examples:
# To enable autoplay for audio and image files:
# DIRECTORY_AUTOPLAY_ITEMS = [ 'audio', 'image' ]
# To disable autoplay entirely:
# DIRECTORY_AUTOPLAY_ITEMS = []

# Make all items a playlist. So when one is finished, the next one will
# start. It's also possible to browse through the list with UP and DOWN
#
DIRECTORY_CREATE_PLAYLIST      = [ 'audio', 'image' ]

# Add playlist files ('m3u') to the directory
#
DIRECTORY_ADD_PLAYLIST_FILES   = [ 'audio', 'image' ]

# Add the item 'Random Playlist' to the directory
#
DIRECTORY_ADD_RANDOM_PLAYLIST  = [ 'audio' ]

# Make 'Play' not 'Browse' the default action when only items and not
# subdirectories are in the directory
#
# DIRECTORY_AUTOPLAY_ITEMS       = [ ]

plugin.remove('mbus')

# ======================================================================
# Freevo movie settings:
# ======================================================================

plugin.remove('video.bookmarker')

#
# Where the movie files can be found.
# This is a list of items (e.g. directories, fxd files). The items themselves
# can also be a list of (title, file)
#
# VIDEO_ITEMS = [ ('action movies', '/files/movies/action'),
#                 ('funny stuff', '/files/movies/comedy') ]

# Some people access movies on a different machine using an automounter.
# To avoid timeouts, you can specify the machine name in the directory
# to check if the machine is alive first
# Directory myserver:/files/server-stuff will show the item for the
# directory /files/server-stuff if the computer myserver is alive.

#
# The list of filename suffixes that are used to match the files that
# are played wih MPlayer.
# 
# VIDEO_MPLAYER_SUFFIX = [ 'avi', 'mpg', 'mpeg', 'wmv', 'bin', 'rm',
#                          'divx', 'ogm', 'vob', 'asf', 'm2v', 'm2p',
#                          'mp4', 'viv', 'nuv', 'mov', 'iso',
#                          'nsv', 'mkv' ]

#
# The list of filename suffixes that are used to match the files that
# are played wih Xine.
# 
# VIDEO_XINE_SUFFIX = [ 'avi', 'mpg', 'mpeg', 'rm', 'divx', 'ogm',
#                       'asf', 'm2v', 'm2p', 'mp4', 'mov', 'cue' ]

#
# Preferred video player
#
VIDEO_PREFERED_PLAYER = 'mplayer'

# ======================================================================
# Freevo audio settings:
# ======================================================================

plugin.remove('mixer')

#
# Where the Audio (mp3, ogg) files can be found.
# This is a list of items (e.g. directories, fxd files). The items themselves
# can also be a list of (title, file)
#
# To add webradio support, add fxd/webradio.fxd to this list
#
# AUDIO_ITEMS = [ ('Music Collection', '/local/mp3/'),
#                 'fxd/webradio.fxd' ]

#
# The list of filename suffixes that are used to match the files that
# are played as audio.
# 
# AUDIO_SUFFIX     = [ 'mp3', 'ogg', 'wav','m4a', 'wma', 'aac', 'flac', 'mka' ]

#
# Regular expression used to recognize filenames which are likely to be 
# covers for an album 
#
# This will match front.jpg and cover-f.jpg, but not back.jpg nor cover-b.jpg:
# AUDIO_COVER_REGEXP = 'front|-f'

#
# Preferred audio player
#
AUDIO_PREFERED_PLAYER = 'mplayer'

#
# Show video files in the audio menu (for music-videos)
#
# AUDIO_SHOW_VIDEOFILES = False

# ======================================================================
# Freevo image viewer settings:
# ======================================================================

#
# Where image files can be found.
# This is a list of items (e.g. directories, fxd files). The items itself
# can also be a list of (title, file)
#
# IMAGE_ITEMS = None

#
# The list of filename suffixes that are used to match the files that
# are used for the image viewer.
# 
# IMAGE_SUFFIX = [ 'jpg','gif','png', 'jpeg','bmp','tiff','psd' ]

#
# The viewer now supports a new type of menu entry, a slideshow file.
# It also has the slideshow alarm signal handler for automated shows.
# It uses a new configuration option:
#
# IMAGE_SSHOW_SUFFIX = [ 'ssr' ]

#
# Mode of the blending effect in the image viewer between two images
# Possible values are:
#
# None: no blending
# -1    random effect
#  0    alpha blending
#  1    wipe effect
#
IMAGEVIEWER_BLEND_MODE = -1
    


# ======================================================================
# Freevo games settings:
# ======================================================================

# YET ANOTHER NEW GAMES SYSTEM : (as of Freevo 2)
# ==============================
#
# The GAMES_ITEMS structure is now built as follows :
#
# ( <TYPE>, <LISTING NAME>, ( <ROMPATH1>, <ROMPATH2>, . . . <ROMPATHx> ),
#   [ ( <EMU1_ID>, <EXECUTABLE>, <EMU1_ARGS> ),
#     ( <EMU2_ID>, <EXECUTABLE>, <EMU2_ARGS> ),
#     ( <EMUx_ID>, <EXECUTABLE>, <EMUx_ARGS> ) ],
#   <SCREENSHOTS>, [ <EXT1>, <EXT2>, <EXTx> ] )
#
# where :
#              - <TYPE> : Internal known game types:
#                         NES  - Nintendo Entertainment System
#                         SNES - Super Nintendo
#                         N64  - Nintendo 64
#                         GB   - Gameboy handheld
#                         GBC  - Gameboy Color handheld
#                         GBA  - Gameboy Advance handheld
#                         VBOY - Virtual Boy (or 'that which shall not be
#                                spoken of' at Nintendo HQ)
#                         SMS  - Sega Master System
#                         SMD  - Sega Megadrive (Genesis)
#                         SG1K - Sega SG-1000/SC-3000
#                         GG   - Sega Game Gear handheld
#                         PCE  - NEC PC-Engine (Turbografx-16)
#                         SGX  - NEC Supergrafx
#                         2600 - Atari 2600
#                         5200 - Atari 5200
#                         7800 - Atari 7800
#                         JAG  - Atari Jaguar
#                         LYNX - Atari Lynx handheld
#                         USER - Formerly GENERIC.  For user specified entries
#              - <LISTING NAME> : What will be shown for a USER entry
#                                 in the main games menu.  Ignored for known
#                                 game types.
#              - <ROMPATH>      : A comma seperated list of all the directories
#                                 where games of TYPE can be found.
# *IMPORTANT*  - <EMU_ID>       : This is a completely arbitrary name the user
#                                 specifies for each emulator/argument set.
#                                 The games database will store this value as
#                                 the reference to your preferred emulator and
#                                 command line on a per-rom basis.  This allows
#                                 you to update command line options and even
#                                 change the emulator used while hopefully
#                                 maintaining a desired ability to play certain
#                                 games a certain way.  See the example below
#                                 for an idea of how this could be useful.
#                                 Each EMU_ID needs to be unique per TYPE.
#                                 You could use LIGHTGUN as your EMU_ID for
#                                 both NES and SMS, but not have two LIGHTGUN
#                                 entries for NES.
#              - <EXECUTABLE>   : Emulator command.  May contain the path or
#                                 just the binary if its within your path.
#              - <EMU_ARGS>     : Arguments for the emulator
#              - <SCREENSHOTS>  : Path to a directory containing either
#                                 individual game screenshots in png form,
#                                 named the same as what the game appears as
#                                 within the menu (with .png of course), or
#                                 the path can point directly to a zipfile
#                                 containing all the screenshots for <TYPE>.
#              - <EXT>          : If this is a USER type definition, then you
#                                 must specify the file suffix used by the
#                                 valid roms you need to emulate
#
# GAMES_ITEMS = [
#                 ( 'NES', None, ( '/shared/roms/NES', '/home/user/Nintendo' ),
#                   [ ( 'DEFAULT', 'mednafen', '-fs 1 -nes.stretch 1' ),
#                     ( 'LIGHTGUN', 'mednafen', '-nes.input1 zapper' ),
#                     ( 'FCE', 'fceu', '-somefceopt' ),
#                     ( 'FAKE', 'fakenes', '-fakenesopt1 -fakenesopt2' ) ],
#                   '/shared/roms/NES/.screens', None ),
#
#                 ( 'USER', 'Wanderswan', ( '/shared/roms/WS' ),
#                   [ ( 'DEFAULT', 'xe', '-ws 1' ) ], '/shared/roms/WS/screens.zip',
#                   [ 'ws', 'bws' ] ),
#
#                 # Changing the shell scripts for most linux games from game-name
#                 # to game-name.sh would allow something like this easily enough.
#                 ( 'USER', 'Native Games', ( '/usr/games/bin' ),
#                   [ ( 'DEFAULT', 'bash', '' ) ], '/usr/games/share/screenshots.zip',
#                   [ 'sh' ] )
#               ]
#
# GAMES_ITEMS = None

# Priority of the game process
# 0 = Don't change the priority
# >0 - Lower priority
# <0 - Higher priority
#
# GAMES_NICE = -20

# ======================================================================
# Freevo SKIN settings:
# ======================================================================

#
# Select a way when to switch to text view even if a image menu is there
# 
# 1 = Force text view when all items have the same image and there are no
#     directories
# 2 = Ignore the directories, always switch to text view when all images
#     are the same
#
# GUI_FORCE_TEXTVIEW_STYLE = 1

#
# Force text view for the media menu
# (The media menu is the first menu displayed for video, audio, images 
# and games). 
#
# GUI_MEDIAMENU_FORCE_TEXTVIEW = 0

GUI_DEFAULT_XML_FILE = 'geexbox'


# ======================================================================
# Freevo OSD settings:
# ======================================================================

#
# System Path to search for fonts not included in the Freevo distribution
#
# GUI_FONT_PATH  = [ '/usr/X11R6/lib/X11/fonts/truetype/' ]

#
# Font aliases 
# All names must be lowercase! All alternate fonts must be in './share/fonts/'
#
# GUI_FONT_ALIASES = { 'arial_bold.ttf' : 'VeraBd.ttf' }

#
# Number of seconds to wait until the busy icon is shown in the menu.
# Busy icon can also be shown right away when there is more than a certain
# number of files in a directory.
# 
# Set this to None to disable this.
# (seconds, files)
#
# GUI_BUSYICON_TIMER = (0.7, 200)

# GUI_OVERSCAN_X = 0
# GUI_OVERSCAN_Y = 0

#
# Execute a script on OSD startup.
#
# GUI_SDL_EXEC_AFTER_STARTUP = ""

#
# Execute a script on OSD close.
#
# GUI_SDL_EXEC_AFTER_CLOSE = ""

GUI_DISPLAY="SDL"

#
# Stop the osd before playing a movie with xine or mplayer. Some output
# devices need this. After playback, the osd will be restored
#
# GUI_STOP_WHEN_PLAYING = 0

#
# Dim text that doesn't fit instead of using ellipses. 
# OSD_DIM_TEXT = 1
    
# ======================================================================
# Freevo remote control settings:
# ======================================================================

# Disable LIRC plugin
plugin.remove('input.lirc')

#
# Location of the lircrc file
#
# For remote control support, Freevo needs a lircrc file, like this:
#
# begin
#       prog = freevo
#       button = select
#       config = SELECT
# end
#
# Check contrib/lirc for examples and helpers/freevo2lirc.pl for a converter
# script.
#
# LIRCRC = '/etc/freevo/lircrc'

#
# Joystick support
# 0 = Disable joystick support 
# 1 = Use js0, 
# 2 = Use js1, 
#  ... etc
#
JOY_DEV = 1

# Freevo can support as many buttons as your controller has
# as long as there is a corresponding entry in JOY_CMDS.
# You will also need to plugin.activate('joy').
# FYI: new kernels use /dev/input/jsX, but joy.py will fall back on /dev/jsX
#
JOY_CMDS = {
     'up'             : 'UP',
     'down'           : 'DOWN',
     'left'           : 'LEFT',
     'right'          : 'RIGHT',
     'button 1'       : 'PLAY',
     'button 2'       : 'PAUSE',
     'button 3'       : 'STOP',
     'button 4'       : 'ENTER',
     }


# ======================================================================
# TVtime settings:
# ======================================================================

#
# Location of the TV time program
# Default: Use the value in freevo.conf
# 
# TVTIME_CMD = CONF.tvtime


# ======================================================================
# MPlayer settings:
# ======================================================================

# MPLAYER_AO_DEV       = 'oss:/dev/dsp'  # e.g.: oss,sdl,alsa, see mplayer docs

# MPLAYER_VO_DEV_OPTS  = ''	         # e.g.: ':some_var=vcal'

# DVD_LANG_PREF        = 'en,se,no'      # Order of preferred languages on DVD.
# DVD_SUBTITLE_PREF    = ''              # Order of preferred subtitles on DVD.

# Priority of mplayer process. 0 is unchanged, <0 is higher prio, >0 lower prio.
# prio <0 has no effect unless run as root.
# MPLAYER_NICE         = -20             

#
# Mplayer options to use the software scaler. If your CPU is fast enough, you
# might try a software scaler. You can disable it later for some larger files
# with the mplayer option '-nosws'. If you have -framedrop or -hardframedrop
# as mplayer option, the software scaler will also not be used.
# A good value for this variable is:
# MPLAYER_SOFTWARE_SCALER = "-subfont-text-scale 5 -fs -sws 2 -vf scale=%s:-3,"\
#                           "expand=%s:%s " % ( CONF.width, CONF.width, CONF.height )
# older versions of mplayer may need
# MPLAYER_SOFTWARE_SCALER = '-xy %s -sws 2 -vop scale:-1:-1:-1:100' % CONF.width
#
# MPLAYER_SOFTWARE_SCALER = ''

#
# Mplayer arguments for different media formats. (eg DVDs, CDs, AVI files, etc)
# Uses a default value if nothing else matches.
#
# MPLAYER_ARGS = { 'dvd'    : '-cache 8192',
#                  'vcd'    : '-cache 4096',
#                  'cd'     : '-cache 1024 -cdda speed=2',
#                  'tv'     : '-nocache',
#                  'ivtv'   : '-cache 8192',
#                  'avi'    : '-cache 5000 -idx',
#                  'rm'     : '-cache 5000 -forceidx',
#                  'rmvb'   : '-cache 5000 -forceidx',
#                  'webcam' : 'tv:// -tv driver=v4l:width=352:height=288:outfmt=yuy2:device=/dev/video2',
#                  'default': '-cache 5000'
#                  }

#
# Number of seconds before seek value times out. This is used when
# seeking a specified number of minutes into a movie. If you make
# a mistake or change your mind, the seek value will timeout after
# this many seconds.
#
# MPLAYER_SEEK_TIMEOUT = 8

#
# Autocrop files when playing. This is useful for files in 4:3 with black
# bars on a 16:9 tv
#
# MPLAYER_AUTOCROP = 0

#
# Try to set correct 'delay' and 'mc' values for mplayer based on the delay
# from mmpython. 
#
# This should correct av sync problems with mplayer for some files, but 
# may also break things. (I don't know, that's why it's disabled by default). 
# WARNING: When seeking, the playback is out of sync for some seconds! 
#
# MPLAYER_SET_AUDIO_DELAY = 0


# ======================================================================
# Freevo TV settings:
# ======================================================================

plugin.remove('tv')
plugin.remove('tv.xine')

#
# This is where recorded video is written.
#
# XXX the path doesn't work from the www cgi scripts!
# TV_RECORD_DIR = '/records'

#
# Watching TV
#

#
# Size (in MB) of the timeshift buffer. (ie: how long you can pause tv for.)  
# This is set to a low default because the default buffer location is 
# under FREEVO_CACHEDIR and we don't want to blow /var or /tmp.
# TIMESHIFT_BUFFER_SIZE = 128

# TIMESHIFT_ENCODE_CMD = None

# TIMESHIFT_BUFFER = '%s/timeshift.mpeg' % FREEVO_CACHEDIR

# TV_CHANNEL_PROG = './chchan %(channel)s %(norm)s %(freqtable)s'

# TV_DATEFORMAT = '%e-%b' # Day-Month: 11-Jun
# TV_TIMEFORMAT = '%H:%M' # Hour-Minute 14:05
# TV_DATETIMEFORMAT = '%A %b %d %I:%M %p' # Thursday September 24 8:54 am

# This is the filename format for files recorded using Freevo.
# You can use any of the strftime variables in it, provided you
# put two '%%' at the beginning. 
# 
# Some examples:
# %%A - Full weekday name.
# %%H - Hour (24-hour clock) as a decimal number [00,23].
# %%M - Minute as a decimal number [00,59].
# %%m - Month as a decimal number [01,12].
# %%d - Day of the month as a decimal number [01,31].
# %%p - Locale's equivalent of either AM or PM.
# 
# More can be found at: http://www.python.org/doc/current/lib/module-time.html

# TV_RECORD_FILEMASK = '%%m-%%d %%H:%%M %(progname)s - %(title)s'


# if using the persitant recordserver
# TV_RECORD_SCHEDULE = '%s/record_schedule.xml' % FREEVO_CACHEDIR

# TV_RECORD_SERVER_IP = 'localhost'
# TV_RECORD_SERVER_PORT = 18001

# start every recording X minutes before scheduled,
# and stop X minutes after scheduled - default to zero minutes.
# This must be a value in seconds although at the moment only has
# the percision of one minute.
# TV_RECORD_PADDING = 0 * 60

# VCR_AUDIO = (':adevice=%s' % AUDIO_DEVICE +
#              ':audiorate=32000' +         # 44100 for better sound
#              ':forceaudio:forcechan=1:' + # Forced mono for bug in my driver
#              'buffersize=64')             # 64MB capture buffer, change?

# TV capture size for viewing and recording. Max 768x480 for NTSC,
# 768x576 for PAL. Set lower if you have a slow computer!
#
# For the 'tvtime' TV viewing application, only the horizontal size is used.
# Set the horizontal size to 400 or 480 if you have a slow (~500MHz) computer,
# it still looks OK, and the picture will not be as jerky.
# The vertical size is always either fullscreen or 480/576 (NTSC/PAL)
# for tvtime.
# TV_VIEW_SIZE = (640, 480)
# TV_REC_SIZE = (320, 240)   # Default for slower computers

# Input formats for viewing and recording. The format affect viewing
# and recording performance. It is specific to your hardware, so read
# the MPlayer docs and experiment with mplayer to see which one fits
# your computer best.
# TV_VIEW_OUTFMT = 'yuy2'   # Better quality, slower on pure FB/X11
# TV_REC_OUTFMT = 'yuy2'

# XXX Please see the mencoder docs for more info about the settings
# XXX below. Some stuff must be changed (adevice), others probably
# XXX should be ("Change"), or could be in some cases ("change?")
# VCR_CMD = (CONF.mencoder + ' ' +
#            'tv:// ' +                      # New mplayer requires this.
#            '-tv driver=%s:input=%d' % (TV_DRIVER, TV_INPUT) +
#            ':norm=%s' % CONF.tv +
#            ':channel=%(channel)s' +        # Filled in by Freevo
#            ':chanlist=%s' % CONF.chanlist +
#            ':width=%d:height=%d' % (TV_REC_SIZE[0], TV_REC_SIZE[1]) +
#            ':outfmt=%s' % TV_REC_OUTFMT +
#            ':device=%s' % TV_DEVICE +
#            VCR_AUDIO +                     # set above
#            ' -ovc lavc -lavcopts ' +       # Mencoder lavcodec video codec
#            'vcodec=mpeg4' +                # lavcodec mpeg-4
#            ':vbitrate=1200:' +             # Change lower/higher, bitrate
#            'keyint=30 ' +                  # Keyframe every 10 secs, change?
#            '-oac mp3lame -lameopts ' +     # Use Lame for MP3 encoding, must be enabled in mencoder!
#            'br=128:cbr:mode=3 ' +          # MP3 const. bitrate, 128 kbit/s
#            '-ffourcc divx ' +              # Force 'divx' ident, better compat.
#            '-endpos %(seconds)s ' +        # only mencoder uses this so do it here.
#            '-o %(filename)s')         # Filled in by Freevo


#
# FREQUENCY_TABLE - This is only used when Freevo changes the channel natively.
# This is only the case if you are using V4L2 and any of the following plugins:
# timeshift, ivtv_record, ivtv_basic_tv.
# For the standard frequancy tables see src/tv/freq.py.  To add your own just 
# replace tuner_id in the following example with a valid tuner id (ie: '5' or
# 'BBC1') and a frequency in KHz.  You may have as many entries as you like,
# anything here will simply override a corresponding entry in your standard
# frequency table and you can also have entries here that are not present in
# there.

# FREQUENCY_TABLE = {
#     'tuner_id'   :    55250,
# }


#
# TV Channels. This list contains a mapping from the displayed channel name
# to the actual channel name as used by the TV watching application.
# The display name must match the names from the XMLTV guide,
# and the TV channel name must be what the tuner expects (usually a number).
#
# The TV menu is supposed to be supported by the XMLTV application for
# up to date listings, but can be used without it to just display
# the available channels.
#
# This list also determines the order in which the channels are displayed!
# N.B.: You must delete the XMLTV cache file (e.g. /var/cache/freevo/TV.xml.pickled)
#       if you make changes here and restart!
#
# Format: [('xmltv channel id', 'freevo display name', 'tv channel name'), ...]
#
# If this variable is set to None (default), Freevo will try to auto-detect
# the channel list based on the xmltv file. This doesn't work for all
# xmltv grabber, e.g. the German list doesn't contain station lists. In this
# case Freevo will output the possible list for you to add them manually.
#
# If auto-detection doesn't work or you want to edit the list, run
# freevo tv_grab -query.
#
# If you don't have a tv card, you may also want to add plugin.remove('tv') to 
# remove the whole tv menu.
#
# All channels listed here will be displayed on the TV menu, even if they're
# not present in the XMLTV listing.
# 
#
# Timedependent channels:
#
# The TV_CHANNELS-list can look like this:
#
# TV_CHANNELS = [('21', 'SVT1',              'E5'),
#                ('22', 'SVT2',              'E3'),
#                ('26', 'TV3',               'E10'),
#                ('27', 'TV4',               'E6'),
#                ('10', 'Kanal 5',           'E7'),
#                ('60', 'Fox Kids',          'E8', ('1234567','0600','1659')),
#                ('16', 'TV6',               'E8', ('1234567','1700','2359'), 
#                                                  ('1234567','0000','0300')),
#                ('14', 'MTV Europe',        'E11') ]
#
# As you can see the list takes optional tuples:
# ( 'DAYS', 'START','END')
#
# 1234567 in days means all days.
# 12345 would mean monday to friday.
#
# It will display "Fox Kids" from 06:00 to 16:59 and "TV6" from 17:00 to 03:00. 
# 03:00 to 06:00 it won't be displayed at all.
#

# TV_CHANNELS = []

# If you would like to change the display format of your channels and prefer
# not to list your entire TV_CHANNELS then you can apply rules using
# TV_CHANNELS_DISPLAY_FORMAT.  This defaults to the plain name of the channel
# in order to not mess with TV_CHANNELS.      
# Valid keywords are:
#     %(id)s
#     %(tunerid)s
#     %(name)s
# For example, if you prefer to see the channel number with the name you
# could use '%(tunerid)s %(name)s'.

# TV_CHANNELS_DISPLAY_FORMAT = '%(name)s'

# Sometimes we can't control the contents of our XMLTV file and it may contain
# channels we don't want.  Here you can specify a list of channel_id values
# to explicitly remove from Freevo.

# TV_CHANNELS_EXCLUDE = []


#
# Program to grab xmltv listings. To get a grabber, you need to download
# xmltv. A possible value for users in the USA is tv_grab_na
# Use the tv_grab helper to grab the listings and cache them. Start
# 'freevo tv_grab --help' for more informations.

# XMLTV_GRABBER = '/usr/bin/tv_grab_fr'

#
# If you want to run tv_sort on your listings add the path to tv_sort here.
# tv_sort will make sure all your programs have proper stop times, otherwise
# programs might get cut off at midnight.
#
# XMLTV_SORT = ''

#
# Number of days the grabber should get
#
# XMLTV_DAYS = 3


# ======================================================================
# VDR settings for Freevo plugins and programs that interface to VDR.
# You can read about VDR at http://www.cadsoft.de/vdr/.
# ======================================================================

# The directory where the VDR config files can be found.
# VDR_DIR = '/video'

# If VDR is on another host, which one.
# VDR_HOST = 'localhost'

# The port (SVDRP) it is listening on.
# VDR_PORT = 2001

# The EPG filename.
# VDR_EPG = 'epg.data'

# The configuration file for VDR channels.
# VDR_CHANNELS = 'channels.conf'

# The property of a channel we are going to access it with.  The options here
# are 'name', 'sid', and 'rid'.
# VDR_ACCESS_ID = 'sid'


# ======================================================================
# Freevo builtin WWW server settings:
# ======================================================================

#
# To activate the built in web server, uncomment the following line
#
# plugin.activate('www')

#
# Web server port number. 80 is the standard port, but is often
# taken already by apache, and cannot be used unless the server
# runs as root. Port 8080 is the default, change to 80 if
# needed.
#
# WWW_PORT = 8080

#
# Username / Password combinations to login to the web interface.
# 
# WWW_USERS = { "user1" : "changeme", 
#            "optional" : "changeme2" }
#

#
# Divide the TV guide into intervals of this length (in minutes)
#
# WWW_GUIDE_INTERVAL = 30

#
# Precision for TV guide (in minutes)
#
# WWW_GUIDE_PRECISION = 5 #

#
# Show this many blocks at once
#
# WWW_GUIDE_COLS = 6

# WWW_STYLESHEET = 'styles/main.css'

# WWW_JAVASCRIPT = 'scripts/display_prog-head.js'


# ======================================================================
# Internal stuff, you shouldn't change anything here unless you know
# what you are doing
# ======================================================================

#
# Config for xml support in the movie browser
# the regexp has to be with ([0-9]|[0-9][0-9]) so we can get the numbers
#
# VIDEO_SHOW_REGEXP = "s?([0-9]|[0-9][0-9])[xe]([0-9]|[0-9][0-9])[^0-9]"


#
# Remote control daemon. The server is in the Freevo main application,
# and the client is a standalone application in rc_client/
#
# ENABLE_NETWORK_REMOTE = 0
# REMOTE_CONTROL_HOST = '127.0.0.1'
# REMOTE_CONTROL_PORT = 16310


#
# XMLTV File
#
# This is the XMLTV file that can be optionally used for TV listings
#
# XMLTV_FILE = '/tmp/TV.xml'

#
# XML TV Logo Location
#
# Use the "makelogos.py" script to download all the
# Station logos into a directory. And then put the path
# to those logos here
# TV_LOGOS = OS_CACHEDIR + '/xmltv/logos'

# LOCALE='latin-1'
