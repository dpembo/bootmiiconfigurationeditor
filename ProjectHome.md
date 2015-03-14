# BootMii Configuration Editor #

Copyright (C)2009-2010 pembo.co.uk
http://www.pembo.co.uk

The BootMii Configuration Editor allows you to modify the BootMii ini file on the SD card to change the settings, without having to bring the SD card over to the PC and edit it.

Once launched from the homebrew channel/bannerbomb/etc you can use it to change the following options in the bootmii.ini file:

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
  * Update Check - whether or not to check for (and download) any new versions of the editor on startup.

**NOTE: When the changes are saved, the bootmii.ini file within SD:/bootmii is always recreated with the changed** values.