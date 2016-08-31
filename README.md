# gba_dumper

![Metroid Intro Changed](https://i.imgur.com/rI0fjlz.png)

gba_dumper is a set of utilities designed to make text modifications for Game Boy Advanced (gba) roms easier. Since gba games don't use ASCII and instead each use their own homebrew-encoding, gba_dumper streamlines the approach of translating rom byte-codes into human-readable characters.

###Intended workflow
1. Search for a string of text.
2. Using the matches, let gba_dumper create a translation file. If there are multiple conflicting matches, gba_dumper will prompt you to choose the matching set.
3. Using that translation file, dump a "translated" version of the rom.
4. Using that dump, add special characters (:!@#, spaces, etc.) to the translation file and re-translate.
5. Using the dump, note the areas of memory that hold strings you want to edit, and note the character used as a newline character in the game.
6. Dump the memory noted in 5, edit all the strings you want, and re-upload them to a new dump.
7. Using the modified dump, create a new rom.

###Options
```
-f	Path to the rom to perform operations on. This must be the uncompressed .gba file.
-r	Text to search for within the rom.
-z	When searching for text, the amount of fuzz (or allowed error) to apply.
-d	Dump the formatted bytes of the rom.
-t	Path to a valid translation file, which is newline separated list of byte_value : character values (e.g., 00FF : b).
-m	Path to a valid dump file, which is automatically generated after translating the rom.
-b	The two-byte value that the game uses for segment breaks within contiguous memory spaces.
-a	When dumping strings, the start address of memory to start dumping at.
-e	When dumping strings, the end address of memory to end dumping at.
-s	Path to a valid strings file to write back to the dump, which is a newline separated list of strings.
```

###Example Usage
```bash
# Step 1
$ gba_dumper -f roms/rom.gba -r "Example"

	# Step 2 will be prompted automatically after a successful search.
	1: 0x00721940	C100D300D300C900C700CE00C500C400
	Attempt to generate a translation file? (Yes, type file name. No, type nothing):
	rom.tra

# Step 3	 
$ gba_dumper -f roms/rom.gba -t rom.tra > rom.dmp

# Step 4 
# Manually inspect the dump and use context-clues from strings you know to add symbols to the translation set.
# When you have a complete translation set, re-dump.

# Step 5	 
$ gba_dumper -m rom.dmp -b 00FF -a 0x00721920 -e 0x007226A0 > rom.str

# Step 6	 
$ gba_dumper -m rom.dmp -s rom.str -b 00FF -a 0x00721920 -e 0x007226A0 > rom2.dmp

# Step 7	 
$ gba_dumper -m rom2.dmp -t rom.tra > rom2.gba
```

###Translation Files
Translation files are newline separated lists of byte_values : character pairs that map byte_values to a specific character. Multiple byte_values can map to the same character, but multiple characters can not be mapped to the same byte_value. When writing back changes to a new rom, characters that are mapped to by multiple bytes will pick the first byte pair to write back. To avoid this, simply remove these extra encodings from your translation file and they will be ignored.

If you select an invalid mapping in step 2 (which can happen if your fuzz is too high), gba_dumper will generate a noticeably incomplete translation file.

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
Dump files represent the formatted bytes of the rom with translations applied (if applicable). An example line is
```
0x003DD120 |	800080FF 81007FFF 81007FFF 82007EFF 82007EFF 82007EFF 83007DFF 83007DFF 	|   A A B B B C C 
```

When modifying dump files, always modify the text section (farthest right section). The byte section is used as a sanity check when writing back changes.

###String files
String files are newline separated lists of strings from a section of the rom's memory, broken up by the applied "break" character in the game. When making changes, it is important to note the length of the original line. If you substitute in a smaller string, gba_dumper will pad out the remaining memory with spaces; with a larger string, gba_dumper will simply cut off any letters that exceed the size. 

#Tutorial
To illustrate the major steps of gba_dumper, let's modify the opening text of Metroid Fusion.

###Step 1
Load up a new game of Metroid Fusion. The starting text displayed is:
```
I'd been assigned to watch over Biologic's research team,
```

gba_dumper will automatically generate a translation set for all upper- and lowercase letters provided you search for a word with both in it. So we will search for "Biologic":
```
$ gba_dumper -f metroid.gba -r "Biologic"
No matches found. Try adding fuzz - or your text might actually be a graphic.
```

Often times, uppercase letters in games won't be separated the expected distance away from lowercase letters if the developers  place special characters between the two in the character set. As a result, we'll apply fuzzing:
```
$ gba_dumper -f metroid.gba -r "Biologic" -z 1
1: 0x00700AF6	C200C900CF00CC00CF00C700C900C300
2: 0x0071E23E	420449044F044C044F04470449044304
3: 0x0071FB4A	8200C900CF00CC00CF00C700C900C300
4: 0x0072196E	8200C900CF00CC00CF00C700C900C300
5: 0x00721DB6	8200C900CF00CC00CF00C700C900C300
6: 0x0072272C	8200C900CF00CC00CF00C700C900C300
7: 0x00725338	8200C900CF00CC00CF00C700C900C300
8: 0x007258E8	8200C900CF00CC00CF00C700C900C300
9: 0x0072B23C	C200C900CF00CC00CF00C700C900C300
```

###Step 2
So the fuzz has introduced us to several options - how do we choose? The easiest way is to look for multiple results of the same series of bytes (in this case, 3-8). In the case there is not that giveaway, simply start generating from the first result: incorrect mappings will be evident as the translation file won't contain all the characters in the alphabet.

In this case, we will choose set 3:
```
Attempt to generate a translation file? (Yes, type file name. No, type nothing):
metroid.tra
Ambiguous data set provided. Please select a data set to use: 
3
```

Looking at metroid.tra, we can tell that this was a correct dataset to use:
```
8100 : A
8200 : B
8300 : C
8400 : D
8500 : E
8600 : F
8700 : G
8800 : H
8900 : I
8A00 : J
8B00 : K
8C00 : L
8D00 : M
8E00 : N
8F00 : O
9000 : P
9100 : Q
9200 : R
9300 : S
9400 : T
9500 : U
9600 : V
9700 : W
9800 : X
9900 : Y
9A00 : Z
C100 : a
C200 : b
C300 : c
C400 : d
C500 : e
C600 : f
C700 : g
C800 : h
C900 : i
CA00 : j
CB00 : k
CC00 : l
CD00 : m
CE00 : n
CF00 : o
D000 : p
D100 : q
D200 : r
D300 : s
D400 : t
D500 : u
D600 : v
D700 : w
D800 : x
D900 : y
DA00 : z

```

###Step 3
Next we will use this metroid.tra to translate the rom:
```
$ gba_dumper -f roms/met.gba -t metroid.tra > metroid.dmp
```

Opening up metroid.dmp will reveal these first few lines if you did it correctly:
```
0x00000000 |	2E0000EA 24FFAE51 699AA221 3D84820A 84E409AD 11248B98 C0817F21 A352BE19 	|                 
0x00000020 |	9309CE20 10464A4A F82731EC 58C7E833 82E3CEBF 85F4DF94 CE4B09C1 94568AC0 	|                 
0x00000040 |	1372A7FC 9F844D73 A3CA9A61 5897A327 FC039876 231DC761 0304AE56 BF388400 	|                D
0x00000060 |	40A70EFD FF52FE03 6F9530F1 97FBC085 60D68025 A963BE03 014E38E2 F9A234FF 	|                 
0x00000080 |	BB3E0344 780090CB 88113A94 65C07C63 87F03CAF D625E48B 380AAC72 21D4F807 	|                 
0x000000A0 |	4D455452 4F494434 55534100 414D5445 30319600 00000000 00000000 00980000 	|          V    
```

###Step 4
Now we have to use our minds a bit to fill in special symbols that gba_dumper can't guess. In metroid.dmp, do a search for "Biologic" and look at the one that reflects our opening text:
```
0x00721920 |	4E004E00 4E001F04 14E100FC 00FF8900 4700C400 4000C200 C500C500 CE004000 	|        I d been 
0x00721940 |	C100D300 D300C900 C700CE00 C500C400 4000D400 CF004000 D700C100 D400C300 	| assigned to watc
0x00721960 |	C80000FE CF00D600 C500D200 40008200 C900CF00 CC00CF00 C700C900 C3004700 	| h over Biologic 
0x00721980 |	D3004000 D200C500 D300C500 C100D200 C300C800 4000D400 C500C100 CD004C00 	| s research team 
0x007219A0 |	00FC00FD D300CF00 40008900 4000CF00 CE00C300 C5004000 C100C700 C100C900 	|   so I once agai
```

Very quickly, we can see that we are missing apostrophes. Opening up metroid.tra, we see that 'c' is represented by 'C300;' with this, we can determine that '4700' is our apostrophe byte value. Let's add it into metroid.tra:
```
4700 : '
```

Save, and follow Step 3 to re-dump the rom:
```
0x00721920 |	4E004E00 4E001F04 14E100FC 00FF8900 4700C400 4000C200 C500C500 CE004000 	|        I'd been 
0x00721940 |	C100D300 D300C900 C700CE00 C500C400 4000D400 CF004000 D700C100 D400C300 	| assigned to watc
0x00721960 |	C80000FE CF00D600 C500D200 40008200 C900CF00 CC00CF00 C700C900 C3004700 	| h over Biologic'
0x00721980 |	D3004000 D200C500 D300C500 C100D200 C300C800 4000D400 C500C100 CD004C00 	| s research team 
0x007219A0 |	00FC00FD D300CF00 40008900 4000CF00 CE00C300 C5004000 C100C700 C100C900 	|   so I once agai
```

Continue to do this for any other special characters you encounter. Note that '4000' is being used as a space. Here's a full translation file with most of the opening text translated for comparison:
```
8100 : A
8200 : B
8300 : C
8400 : D
8500 : E
8600 : F
8700 : G
8800 : H
8900 : I
8A00 : J
8B00 : K
8C00 : L
8D00 : M
8E00 : N
8F00 : O
9000 : P
9100 : Q
9200 : R
9300 : S
9400 : T
9500 : U
9600 : V
9700 : W
9800 : X
9900 : Y
9A00 : Z
C100 : a
C200 : b
C300 : c
C400 : d
C500 : e
C600 : f
C700 : g
C800 : h
C900 : i
CA00 : j
CB00 : k
CC00 : l
CD00 : m
CE00 : n
CF00 : o
D000 : p
D100 : q
D200 : r
D300 : s
D400 : t
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

###Step 5
With an acceptable translation, let's move on to dumping the opening section's text so we can edit it all at once. The numbers on the far-left of the dump represent the address of the bytes. Judging by the text, it should be easy to identify the start and end:
```
0x00721900 |	D40000FE D000CF00 D300D300 C900C200 CC00C500 1F044000 8100C400 C100CD00 	| t possible  Adam
0x00721920 |	4E004E00 4E001F04 14E100FC 00FF8900 4700C400 4000C200 C500C500 CE004000 	| ...    I'd been 
...
0x00722680 |	C5004000 C600CF00 D2004000 C400CF00 C300CB00 C900CE00 C7004E00 00FC00FF 	| e for docking.  
0x007226A0 |	8400C500 D2004000 9300C300 C800C900 C600C600 D300C300 CF00CD00 D000D500 	| Der Schiffscompu
```

From this we can see that the beginning starts a bit past 0x00721920. However, we will just dump the whole line and simply not edit the starting characters to make it easy. Likewise, we can see it ends at 0x007226A0.

Next we need the break character, or the byte-values that separate strings of text in memory. The easiest way to do this is to watch the introduction and note when a new cut-scene or dialog box appears. Another approach we can use with Metroid Fusion is looking where the language switches:
```
0x00722680 |	C5004000 C600CF00 D2004000 C400CF00 C300CB00 C900CE00 C7004E00 00FC00FF 	| e for docking.  
0x007226A0 |	8400C500 D2004000 9300C300 C800C900 C600C600 D300C300 CF00CD00 D000D500 	| Der Schiffscompu
```

We know '4E00' is a '.' which means either 00FF has to represent the last character or our break character. Let's try it and see what our dump looks like:
```
$ gba_dumper -m metroid.dmp -b 00FF -a 0x00721920 -e 0x007226A0 > metroid.str
```

The result if done correctly:
```
...   
I'd been assigned to watch over Biologic's research team,  so I once again found myself on the surface of SR388. 
It was there that I was attacked by a life-form I had  never encountered before. 
It was only later that I learned the identity of my  attacker: the parasitic organism we now know as X.  Unaware of my condition, I was returning to the station  when disaster struck. 
Once the X had infested my central nervous system, I lost  consciousness and my ship drifted into an asteroid belt. 
The ship's emergency systems automatically ejected the  escape pod before impact. Biologic's vessel recovered it  and transported me to Galactic Federation HQ.    However, during the journey, the X multiplied within me,  corrupting large areas of my Power Suit. 
It then came to light that the organic components of my  Power Suit had become so integrated with my system  that it could not be removed while I was unconscious. 
Large portions of my suit had to be surgically removed,  dramatically altering my physical appearance. However,  the X in my central nervous system were too embedded to  be removed safely. I was given a minimal chance of survival. 
Then, someone found a cure.   They proposed using a Metroid cell to make an anti-X vaccine.  It seems that the Federation had managed to preserve a  cell culture from the last infant Metroid from SR388.  The serum was prepared and injected without delay. 
The X parasites were immediately and completely  destroyed.   As for me, one life ended... yet I survived, reborn  as something different. 
Pondering this fact, I realize...  I owe the Metroid hatchling my life twice over. 
We'll soon be arriving at the B.S.L research station.  I must prepare for docking. 
```

###Step 6
We can modify metroid.str however we want, but for now, we'll keep it simple:
```
...   
HI,  
I
LIKE
CAKES
MOO!
It then came to light that the organic components of my  Power Suit had become so integrated with my system  that it could not be removed while I was unconscious. 
Large portions of my suit had to be surgically removed,  dramatically altering my physical appearance. However,  the X in my central nervous system were too embedded to  be removed safely. I was given a minimal chance of survival. 
Then, someone found a cure.   They proposed using a Metroid cell to make an anti-X vaccine.  It seems that the Federation had managed to preserve a  cell culture from the last infant Metroid from SR388.  The serum was prepared and injected without delay. 
The X parasites were immediately and completely  destroyed.   As for me, one life ended... yet I survived, reborn  as something different. 
Pondering this fact, I realize...  I owe the Metroid hatchling my life twice over. 
We'll soon be arriving at the B.S.L research station.  I must prepare for docking. 

```

With the modifications done, we will re-upload our changes to the dump file:
```
$ gba_dumper -m metroid.dmp -s metroid.str -b 00FF -a 0x00721920 -e 0x007226A0 > metroid2.dmp
```

gba_dumper will pad out strings that are too short with spaces, as you can see when looking at metroid2.dmp:
```

0x00721920 |	4E004E00 4E001F04 14E100FC 00FF8900 4700C400 4000C200 C500C500 CE004000 	| ...    HI,      
0x00721940 |	C100D300 D300C900 C700CE00 C500C400 4000D400 CF004000 D700C100 D400C300 	|                 
0x00721960 |	C80000FE CF00D600 C500D200 40008200 C900CF00 CC00CF00 C700C900 C3004700 	|                 
0x00721980 |	D3004000 D200C500 D300C500 C100D200 C300C800 4000D400 C500C100 CD004C00 	|                 
0x007219A0 |	00FC00FD D300CF00 40008900 4000CF00 CE00C300 C5004000 C100C700 C100C900 	|                 
0x007219C0 |	CE004000 C600CF00 D500CE00 C4004000 CD00D900 D300C500 CC00C600 00FECF00 	|                 
0x007219E0 |	CE004000 D400C800 C5004000 D300D500 D200C600 C100C300 C5004000 CF00C600 	|                 
0x00721A00 |	40009300 92005300 58005800 4E0000FC 00FF8900 D4004000 D700C100 D3004000 	|          I      
0x00721A20 |	D400C800 C500D200 C5004000 D400C800 C100D400 40008900 4000D700 C100D300 	|                 
0x00721A40 |	00FEC100 D400D400 C100C300 CB00C500 C4004000 C200D900 4000C100 4000CC00 	|                 
0x00721A60 |	C900C600 C5004D00 C600CF00 D200CD00 40008900 4000C800 C100C400 00FC00FD 	|                 
0x00721A80 |	CE00C500 D600C500 D2004000 C500CE00 C300CF00 D500CE00 D400C500 D200C500 	|                 
0x00721AA0 |	C4004000 C200C500 C600CF00 D200C500 4E0000FC 00FF8900 D4004000 D700C100 	|            LIKE 
```

###Step 7
With all this done, we are ready to create a new rom with our changes:
```
$ gba_dumper -m metroid2.dmp -t metroid.tra > metroid2.gba
```

Open metroid2.gba with your favorite emulator, and you should immediately notice the changes.
