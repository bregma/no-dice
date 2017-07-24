No-Dice Third Party Source Modules
==================================

This subdirectory contains thrid-party source modules used to build the No-Dice
project.  Copyrights for these modules belong to the respective authors and
licensing may vary (see individual projects).

The source modules are included here for provenance.  The actual files used get
manually copied out into the main project.

Getting the Submodules
----------------------

You all might notice that the submodules don't come automatically when you check
out the main project.

Once you've cloned the main project, in the top-level directory, you need to
init and update the submodules.

    git submodule init
    git submodule update

You should be left with a fresh checkout of the upstream source.

