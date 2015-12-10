How to Exit Vim
===============

Introduction
-------------

At some point you have run the command `vim` followed by some file in your shell. Perhaps you just wanted to take a look, or you made some modifications, but now you're stuck, and need to get out. However, there's nothing to worry about; exiting Vim is as easy as entering a few commands which depend on your situation. Before you begin, remember to press `esc` so that you are in command mode, so as to not start typing anything by accident.

Leave Nothing but Footsteps
---------------------------
### You made no edits:

If you entered the file but made absolutely no modifications, all you have to do to exit the file is enter `:q` and hit `enter`. That's it! you're done.

### You made some edits you regret:

Uh oh. You made some changes to something you shouldn't have touched. However, there's nothing to worry. Enter `:q!` and hit `enter`. The extra bang forces Vim to discard any changes you made to your file.

Set it in Stone
---------------
### Traditionalism:

You made some changes, and you want to exit the file the way everyone around you does. You want to save your changes, and exit the file. To do this, enter `:wq` and hit the `enter` key. You like this command. It's clear: `w` writes the file, and `q` quits it.

### Need for Speed:
You made some changes, and you're ready to forget about your project as quickly as possible; you'll conserve your keystrokes as much as possible just to save that millisecond of time. To do this, enter `:x` and hit `enter`. Bam. You're out of the file, and it's saved. Now go and be productive.

Captain Goes Down With Their Ship!
---------------------------------
You're extremely embarrassed that you can't exit Vim. You're somewhat angry at your computer, and want revenge. Well, you're in luck. enter `:! sudo shutdown -h now` and hit `enter`, followed by your password. You'll shut down Vim for sure... and everything else with it.
