# SIDI
An Arduino-powered MIDI interface for the Commodore 6581 SID chip and compatibles.  So far I have only used it with the ARMSID, but if you can build the circuit to support a real SID then I don't see why this shouldn't work.

Forked in September 2019 by doctea from therealjaypo's original project to implement a compatible MIDI layer and hopefuly to extend it further including supporting the ARMSID extra features.

Thanks to therealjaypo for his circuit design and code that seems a quicker and cheaper way of getting a working SID synth than a full-blown MIDIBOX, and the retrocomp.cz eshop and ARMSID (https://www.retrocomp.cz/eshop / http://dzi.n.cz/8bit/armsid/index_en.php) for the great resilient and cheaper-than-the-real-thing chip that's been fantastic fun and very satisfying as my first foray into this sort of thing!

Has a polyphonic mode and a mono mode and you can detune the voices as well as tranpose them.  Also has a filter LFO pinched from https://www.notesandvolts.com/ that can also modulate the pulse width of each voice.

TODO:

ensure all features of the SID/ARMSID are exposed (started branch for this but didn't get anywhere)

possible to do sample playback..?

external in? not sure this works for ARMSID..>?
	voltage controlled feedback?

contributions for stuff i don't understand are welcomed..!

-doctea



Original text follows.

--------

I've done my best to keep this as clean as possible but some places are still quite a mess. Large swaths of the main sketch are seriously overdo for a complete overhaul and I'll be getting to that as soon as the other parts of my BeagleBoargan project are more stable.

Futher information on this library as well as the project for which it was created can be found [here](http://hatestheinternet.com/kraslophone/midi-sid-arduino/sidi).
