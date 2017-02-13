# In order to update ts files, you need to run cmd next to your ts files and call
#   ...\lupdate source gui -ts lang_en.ts
#   ...\lupdate source gui -ts lang_de.ts
# where 'source gui' are examples for subdirs that you want to search for strings in.
# A "pre-CMake.bat" would be a nice place for such work.
# Note:
#	This part could also be done automatically by CMake via qt5_create_translation, but
#	first you need to find out how to prevent CMake from completely removing your hard handcrafted
#	ts files when making a clean rebuild. I tried several approaches, but none seem to work.

# You can open and edit ts files with linguist.exe. Then ts files have to be compiled into qm files.
# This done automatically by this file.
# (It could also be achieved within the linguist via 'Freigeben' (in German version).)
# IMPORTANT NOTICE: make sure that all entries in the ts file get the green checkmark by Qt Linguist.
# Otherwise the resulting qm file might not open successfully.
# 
# Then your including CMakeLists file should postbuild copy the qm files (finally to be found in
# TargetAppName_TRANSLATIONS) to the translations subdir of your bin folder.
# Note that this copy step will be carried out only, if you actually build an updated exe ;).
# Furthermore, note that the translation ts->qm itself by this file might only be carried out when
# re-running CMake.
#
# For recap an easy step-by-step:
#   (1) Run a lang_ts_update.bat containing the lupdate calls described above.
#       *But* first check, whether the ts's already got updated automatically after the last build.
#   (2) Translate ts files using Qt Linguist. IMPORTANT: give green checkmarks to every entry!
#   (3) Run CMake and enforce a tiny build of your exe.

set(TargetAppName_LANGS
    lang_en.ts
    lang_de.ts
)

qt5_add_translation(TargetAppName_TRANSLATIONS ${TargetAppName_LANGS})
