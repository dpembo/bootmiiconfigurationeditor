-----------------------------------------------------------------------------------------------------

 ____              _   __  __ _ _    ____             __ _         _____    _ _ _             
| __ )  ___   ___ | |_|  \/  (_|_)  / ___|___  _ __  / _(_) __ _  | ____|__| (_) |_ ___  _ __  
|  _ \ / _ \ / _ \| __| |\/| | | | | |   / _ \| '_ \| |_| |/ _` | |  _| / _` | | __/ _ \| '__|
| |_) | (_) | (_) | |_| |  | | | | | |__| (_) | | | |  _| | (_| | | |__| (_| | | || (_) | |   
|____/ \___/ \___/ \__|_|  |_|_|_|  \____\___/|_| |_|_| |_|\__, | |_____\__,_|_|\__\___/|_|   
                                                           |___/    BootMii Config Editor v2.5
                                                        (c)2010 Pembo - http://www.pembo.co.uk

-----------------------------------------------------------------------------------------------------

The BootMii Configuration Editor allows you to modify the BootMii ini file on the SD card 
to change the settings, without having to bring the SD card over to the PC and edit it.  It also
allows you to disable/enable bootmii (via renaming of the bootmii directory to ~bootmii) or switching
of implementations should you have more than one installed (e.g. MIKE)


Once launched from the homebrew channel you can use it to change the 
following options in the bootmii.ini file:

* Video - NSTC/PAL50/PAL60/PROGRESSIVE
* Autoboot - Off / System Menu / Homebrew Channel
* Boot Delay - Time in Seconds from 0 to 10 for how long the BootMii screen shows before autobooting.

Setting Boot Delay to 0 results in the BootMii screen not being shown on startup.
Disabling Autoboot (Setting to off) results in a selection being required on the BootMii screen.

The application is text based and controlled via the wiimote or gamecube d-pad. Use:

    * UP/DOWN to move through menu items
    * LEFT/RIGHT to change the options of the current selection
    * The 'A' button to select the save/discard option.
    * Pressing 'HOME' ('START' on GC Pad) will bring up the settings menu

From within the settings menu you can change the following
    * Selected Language
    * Skin - Style of the application
    * Update Check - whether or not to check for (and download) any new 
      versions of the editor on startup.

NOTE: When the changes are saved, the bootmii.ini file within SD:/bootmii 
is always recreated with the changed values.


Within the application you can also enable or disable bootmii using the BootMii Active option.
    * Setting to Enabled will leave your BootMii directory is named /bootmii.  
    * Setting to disabled will rename /bootmii to /~bootmii therefore disabling bootmii at startup
    * Enabling/disabling bootmii will modify the homebrew channel icon.  This will display with a 
      green circle when enabled or a red circle when disable.
    * If you wish to use an alternative mini implementation (e.g. MIKE) then you can install this
      in a directory called /~bootmii/.  When the editor starts if /bootmii and /~bootmii exist,
      the editor will run in 'switch mode' giving you the option to edit bootmii.ini in whichever
      directory it is found (or a choice if it exists in both) and at startup will give you the
      option to simply swap the two directories (i.e. /~bootmii becomes /bootmii and vice versa.
    * In switch mode the homebrew icon is changed to an orange circle indicating this.

By default, the application assumes it is running in enabled mode, one running it will assess the mode
and update the icon as to what it finds.


Change Log
===================

07 August 2010 - v2.5
- Implementation Of BootMii Switch functionality.  Can enable/disable BootMii through 
  the editor by renaming the directory from /bootmii to /~bootmii and vice versa
- If /bootmii and /~bootmii exist you are given the option to switch the directories 
  allowing you to use an alternate MINI implementation such as MIKE or cboot2.
- If bootmii.ini exists in both directories you are given the choice of which to edit.
- The homebrew channel icon is swapped depending on the mode to give an indication of the 
  bootmii status.  A green circle indicates bootmii is enabled, a red circle indicates 
  bootmii is disabled, and an orange circle indicates you are running in switch mode.
- Several fixes to resolve various minor issues.

-----------------------------------------------------

09 August 2009 - v2.4
- Automatic Updating now available.  From v2.4 the application can now auto-update
  to the latest version automatically.  This is an important addition as this means I can
  now respond quickly to new releases of BootMii if/when they are released to add in any
  new configuration options quickly.

- Additional Settings Option in the settings menu to allow you to enable/disable 
  the auto update checking.  If you run your wii without a network connection, it can take 
  an amount of time to 'time-out' when trying to initialise the network.  
  Setting this option to disabled/off stops the application from initialising the network 
  and performing the online checks, and speeds up the startup for those running without
  an online connection.

- Settings files paths are now calculated from the argc/argv parameters passed via the
  HBC.  If these parameters are not passed, then a hardcoded path is assumed.  This path
  is 'SD:/apps/Bootmii_cfg_ed' - The downloadable zip also now contains this directory as
  the standard (as opposed to 'BootMii Configuration Editor'.  This does not prevent the
  application from being installed to a different directory.

-----------------------------------------------------

30 July 2009 - v2.3
- Changes Skin/language as you change between them.

- Monochrome/Green Skin Skin improved by a new skin option, maindecl and dialog decl
  These values allow for a textual border around the monochrome/green screen skins

- Further changes to resovle sporadic crashes whilst changing language/skins from the
  settings menu.

-----------------------------------------------------

27 July 2009 - v2.2
- Additional Languages, Dutch, Finnish and German.

  Thank you to the following people for providing new translations :)
  Dutch/Nederlands – MarioWaza
  Finnish/Suomi    – JaniN
  German/Deutsch   – Helsionium

- Addition of 2 new skins, Monochrome and Green Screen.

- Correction of the update version checking.

- Resolved pointer corruption issue that resulted in sporadic crashes.

- Separation of XML Files into 3 different files:
  bce_settings.xml  - Contains all the saved settings
  bce_messages.xml  - Contains all the language strings
  bce_skins.xml     - Contains the skin definitions

  This separation makes it easier to change things, debug issues, and add new languages.


-----------------------------------------------------

07 July 2009 - v2.1
- English, French, Italian Portuguese and Spanish langauges available to select from within
  the settings menu.

  Thank you to the following people who have provided translations
  French     - Vlad
  Italian    - Raiden
  Portuguese - zedascouves
  Spanish    - Mokona Modoki/Comepiedras

- Adjustments to the text positioning to cope with some of the longer words that
  the new translations bring.

- Game cube controller compatible.    
  Use the D-Pad to move through the options, 'A' to select a menu option, 
  and 'Start' to bring up the settings menu

-  Boot Delay modified.  It can now be changed from 0 up to 10 seconds

- 576i/480i detection and adjustment.  Vertical positioning is adjusted slightly depending
  on which video mode is selected and the height of the screen

- Wiimote vibration.  Wiimote vibrates when you enter the start menu

- BootMii Configuration Editor Logo always shows in bottom right corner

- Wii Power Button now turns off wii without having to hold for 5 seconds

- Wii Reset Button now resets the console

- Power button on wiimote now powers down the wii

-----------------------------------------------------

02 July 2009 - v2.0
- Settings Dialog to change editor settings.  This can be accessed by pressing the 'HOME' button.
  The settings are persisted to the langauges file, so if you change the language/skin, whenever you
  load BootMii Configuration Editor, your selected options will be the default

- Internationalisation Supported (Currently English & Spanish Langauges available)
  Further langauges can be added by editing the bce_messages.xml file as follows

  1. Add a new language/code to the available langauges section near the top of the file
     (e.g. to add french add the following line:

     <language description="Francais" code="fr" /> 

     to the available section, e.g.:
 
     <available> 
       <language description="English" code="en" /> 
       <language description="Espanol" code="es" /> 
       <language description="Francais" code="fr" /> 
     </available>

  2. For every <entry, add in the french translation using an attribute named the same as the
     value specified in the code for the langauge (e.g. fr) for example

     <entry code="mm1" en="Video:" es="Modo de Video:" fr="Mode visuel:"/>

  Please be careful if you do edit the XML file.  A corrupt/non well-formed XML file will result in 
  a Crash of the Wii and will dump the stack to the screen.  Ensure you take a backup of the original
  before attempting to make any custom changes.

  Currently you can only use characters in the standard ASCII character set, e.g.
  [0-9] [a-z] [A-Z]

  Some puncation characters may also cause issue.  It is recommended that you do not use characters
  with any accents/umlauts/etc as this will cause an issue.

  If you do create a translation for a new language, please drop by my home page (www.pembo.co.uk) and
  let me know.  I'll then include it in the next release!


- Skins/Colour Change Available.  The application can now be presented in a number of look & feels.
  Initially the following skins are available:
     * Windows - Windows Installer/DOS like look/feel with a blue background and grey windows
     * Console - Black background, white text with other colour text to highlight options/selections
     * Inverse Console - White background, black text.

  Custom Skins can be added by editing the bce_messages.xml file as follows.  Find the <skins element
  and within this, select and duplicate a skindetail element,e.g.

  <skindetail id="custom1" name="My Custom Skin"
  consolebgcolor="14" consoletextcolor="17" mainbgcolor="7"
  maintextcolor="0" maintextcolor2="10" maintitlecolor="11"
  maintitlelinecolor="0" mainselectedIndicatorcolor="11"
  mainselectedOptioncolor="15" mainshadow="0" dialogbgcolor="10"
  dialogtextcolor="17" dialogtextcolor2="17" dialogtitlecolor="17"
  dialogtitlelinecolor="7" dialogselectedIndicatorcolor="11"
  dialogselectedOptioncolor="13" dialogshadow="0" />

  Change the id and name.  The id must be unique through all the skins, then modify the colours.
  The following colours/numbers are available to use

    * BLACK 0
    * DARK_RED 1
    * DARK_GREEN 2
    * DARK_YELLOW 3
    * DARK_BLUE 4
    * DARK_MAGENTA 5
    * DARK_CYAN 6
    * GRAY 7
    * DARK_GRAY 10
    * RED 11
    * GREEN 12
    * YELLOW 13
    * BLUE 14
    * MAGENTA 15
    * CYAN 16
    * WHITE 17

- Online New Version Checking.  When the Editor is loaded, it performs a check to see if the version
  running is the latest available version.  If a newer version is available, then a message is shown
  for a few seconds during startup to indicate this.

  This online checking can be disabled if you do not wish BootMii Configuration Editor to check for a
  new version or do not have the wii networked.  

  If you wish to disable this check, amend the check attribute in the following line:
  <update check="1" url="http://www.dpembo.ukfsn.org/BCE/release/bce_update.ver" />

  check="1" means the online checking is enabled
  check="0" means that the online checking will be disabled.


  Please be careful if you do edit the XML file.  A corrupt/non well-formed XML file will result in 
  a Crash of the Wii and will dump the stack to the screen.  Ensure you take a backup of the original
  before attempting to make any custom changes.

-----------------------------------------------------

27 June 2009 - v1.1
- Resolved a bug that caused a code dump if the bootmii directory did not exist.  This is checked on 
  startup and the user is informed if this does not exist
- Improved look & feel

-----------------------------------------------------

20 June 2009 - v1.0
- Additional autoboot option.  Can now change to OFF, SYSMENU (System Menu) or HBC (Homebrew Channel).
  Setting to OFF disables the boot delay option in the editor as this serves no purpose if the autboot
  option has been turned off.

-----------------------------------------------------

18 June 2009 - v0.1b
-Initial Release








  


