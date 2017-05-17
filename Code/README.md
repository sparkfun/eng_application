EL Dance Suit
==============

This project was for a performance with Streetside Studio's bboys/bgirls on May 13th, 2017. The code contained in this repository was used to wirelessly control 2x 3 meters of EL Wire attached to each dancer's hoodie and pants. There are a total of 7x dancers that will be wearing the EL Dance suits and one controller. 8x XBees were configured in a point-to-multipoint network to trigger the EL Sequencers.

Code Contents
-------------------
**<> Code**
- **/XBee_ELSequencerV3** - Code used to read the slave XBee's received character and sequence each dancer's hoodie or pants.
- **/XBee_ELSequencer_ControllerV3** - Code used to arm the master XBee and tell the slave XBees when to begin a sequence based on the button press.

Documentation
-------------------

* [SparkFun EL Sequencer Hookup Guide](https://learn.sparkfun.com/tutorials/el-sequencerescudo-dos-hookup-guide)
* [GitHub Repository for EL Sequencer](https://github.com/sparkfun/EL_Sequencer/tree/master)


License Information
-------------------

This project is _**open source**_! 

Please review the LICENSE.md file for license information. 

Distributed as-is; no warranty is given.
