# gba_dumper

gba_dumper is a set of utilites designed to make text modifications for Game Boy Advanced (gba) roms easier. Since gba games don't use ascii and instead each use their own homebrew-encoding, gba_dumper attempts to streamline the approach of translating rom byte-codes into human-readable characters.

###Intended workflow
1. Search for a string of text.
2. Using the matches, let gba_dumper create a translation file. If there are multiple conflicting matches, gba_dumper will prompt you to choose the matching set.
3. Using that translation file, dump a "translated" version of the rom.
4. Using that dump, add special characters (:!@#, spaces, etc.) to the translation file and retranslate.
5. Using the dump, note the areas of memory that hold strings you want to edit, and note the character used as a newline character in the game.
6. Dump the memory noted in 5, edit all the strings you want, and reupload them to a new dump.
7. Using the modified dump, create a new rom.

###Options
-f	Path to the rom to perform operations on. This must be the uncompressed .gba file.
-r	Text to search for within the rom.
-z	When searching for text, the amount of fuzz (or allowed error) to apply.
-d	Dump the formatted bytes of the rom.
-t	Path to a valid translation file, which is newline separated list of byte_value : character values (e.g., 00FF : b).
-m	Path to a valid dump file, which is automatically generated after translating the rom.
-b	The two-byte value that the game uses for segment breaks within contiguous memory spaces
-a	When dumping strings, the start address of memory to start dumping at.
-e	When dumping strings, the end address of memory to end dumping at.
-s	Path to a valid strings file to write back to the dump, which is a newline separated list of strings.

###Example Usage
```bash
# Step 1
$ gba_dumper -f roms/rom.gba -r "Example"

	# Step 2 will be prompted automatically after a successful search
	1: 0x00721940	C100D300D300C900C700CE00C500C400
	Attempt to generate a translation file? (if yes, type file name. For no, type nothing):
	rom.tra

# Step 3	 
$ gba_dumper -f roms/rom.gba -t rom.tra > rom.dmp

# Step 4 
# Manually inspect the dump and use context-clues from strings you know to add symbols to the translation set.
# When you have a complete translation set, redump

# Step 5	 
$ gba_dumper -m rom.dmp -b 00FF -a 0x00721920 -e 0x007226A0 > rom.str

# Step 6	 
$ gba_dumper -m rom.dmp -s rom.str -b 00FF -a 0x00721920 -e 0x007226A0 > rom2.dmp

# Step 7	 
$ gba_dumper -m rom2.dmp -t rom.tra > rom2.gba
```

###Translation Files
Translation files are newline separated lists of byte_values : character pairs that map byte_values to a specific character. Multiple byte_values can map to the same character, but multiple characters can not be mapped to the same byte_value. When writing back changes to a new rom, characters that are mapped to by multiple bytes will pick the first byte pair to write back. To avoid this, simply remove these extra encodings from your translation file and they will be ignored.

If you select an invalid mapping in step 2 (which can happen if your fuzz is too high), gba_dumper will generate a noticably incomplete translation file.

To write a modified dump back to a rom, you must add the space(' ') character to your translation set. Its format is shown below.

An example translation file:
```
D400 : A
D500 : u
D600 : v
D700 : w
D800 : x
D900 : y
DA00 : z
4E00 : .
4700 : '
4C00 : ,
5300 : 3
5800 : 8
4D00 : -
5A00 : :
4000 :  
```

Other example translation files can be found in the tests/ directory.

###Dump Files
Dump files represent the formatted bytes of the rom with translations applied (if applicable). An example line is:
0x003DD120 |	800080FF 81007FFF 81007FFF 82007EFF 82007EFF 82007EFF 83007DFF 83007DFF 	|   A A B B B C C 

When modifying dump files, always modify the text section (farthest right section). The byte section is used as a sanity check when writing back changes.

###String files
String files are newline separated lists of strings from a section of the rom's memory, broken up by the applied "break" character in the game. When making changes, it is important to note the length of the original line. If you subsitute in a smaller string, gba_dumper will pad out the remaining memory with spaces; with a larger string, gba_dumper will simply cut off any letters that exceed the size. 