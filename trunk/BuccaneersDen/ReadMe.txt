                  Buccaneer's Den

This is Free Software, You can get the source and report bugs at:
http://code.google.com/p/buccaneers-den/
If you need help or want to discuss possible feature, go to:
http://groups.google.com/group/buccaneers-den

= General functionality =

  Configuration options are dependants on the currently selected device.

  You can save macro with the "Save macro" button or by dragging them to
  the macro panel. To use a macro, you can right click and choose activate
  or double click on the code column of the macro. Directly activating a
  macro does not but them in the command history. Clicking on the name column
  allow you to change the macro name. You can drag macros to the input edit
  box if you want to modify them. The right click also allow you to create
  folder and delete macro. You can move or copy macro using drag&drop. To
  change between operation, you press shift before beginning the drag.

  Macros are saved automatically in a xml file in your home directory under
  .buccaneers. There is no limit on the number or length of the macros. You
  can have multiples macro with the same name. You can export your macros or
  import some using the two button at the bottom of the macros panel.

  Macros are not shared between device.

  Command history is access with the Up and Down arrow on a single line input
  or Control-Up and Control-Down on a multiline input. There is no limit to
  the history but it is not saved between sessions.


= Bus Pirate =

  In this mode, you can only configure the serial speed and IO color and font.

  = Console tab =

    You can send binary data by beginning your line with "bytes" followed by
    any number of hexadecimal or binary number. You can also use a multiplier
    before each byte to repeat them. Example:

    bytes 5*0xFF b01010101 2*0xe0

    would send 5 time 0xff followed by 0x55 followed by 2 time 0xe0. In the
    console, bytes you send are between [] and bytes you receive between {}
    or as letter.

    The multiline input has a basic script highlighter. If you start your
    script with "basic" written on the first line, unnumbered comment will
    not be sent to the bus pirate; allowing you to comment you script as
    much as you like without clogging the Bus Pirate memory.

    Session are always logged. The logs will be in your home directory under
    .buccaneers/BusPirate/Logs with the format: BDen_Console-DateTime.log.

  = Firmware tab =

    This allow you to update the firmware of your Bus Pirate v3. Please be in
    HiZ when entering this tab so that Buccaneer's Den can correctly identify
    your current Bus Pirate firmware and hardware version.

= Line Console =

  In this mode, you have access to all serial settings. The "bytes" or "basic"
  command syntax are not available. Session are always logged. The logs will
  be in your home directory under .buccaneers/LineDevice/Logs with the format:
  LD_Console-DateTime.log. 
