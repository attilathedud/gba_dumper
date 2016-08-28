#!/bin/bash

#
#	Given two test files and an output string, compare the files and output the string based on that conditional.
#
#	Delete the second file passed.
#
#	$1:	Path to the correct test.
#	$2:	Path to the generated test.
#	$3:	Test description.	
#
diff_and_cleanup()
{
	if [ "$1" ] && [ "$2" ] && [ "$3" ]
	then
		if cmp -s "$1" "$2"
		then
			echo "$(tput setaf 2)Passed: $(tput sgr0) \"$3\" test"
		else
			echo "$(tput setaf 1)Failed: $(tput sgr0) \"$3\" test" 
		fi
	else
		echo "Not enough parameters passed to diff_and_cleanup() function."
	fi
	rm "$2"
}

# Basic dumping of roms. 
../gba_dumper -f test.gba -d > generated_test1.dmp
diff_and_cleanup "correct/test1.dmp" "generated_test1.dmp" "Dumping rom"

# Searching for a simple lower-case string with one result.
../gba_dumper -f test.gba -r "assigned" > /dev/null <<< generated_test2.tra
diff_and_cleanup "correct/test2.tra" "generated_test2.tra" "Lower-case search"

# Searching for a string with fuzz and an ambiguous result set.
../gba_dumper -f test.gba -r "Security" -z 1 > /dev/null << 'EOF'
generated_test3.tra
1
EOF
diff_and_cleanup "correct/test3.tra" "generated_test3.tra" "Fuzz and ambiguous search"

# Translating rom
../gba_dumper -f test.gba -t correct/test3.tra > generated_test4.dmp
diff_and_cleanup "correct/test4.dmp" "generated_test4.dmp" "Translation"

# Dumping strings
../gba_dumper -m correct/test4.dmp -b 00FF -a 0x00307920 -e 0x003086A0 > generated_test5.str
diff_and_cleanup "correct/test5.str" "generated_test5.str" "Dumping strings"

# Reuploading modified strings
../gba_dumper -m correct/test4.dmp -s input/test6.str -b 00FF -a 0x00307920 -e 0x003086A0 > generated_test6.dmp
diff_and_cleanup "correct/test6.dmp" "generated_test6.dmp" "Reuploading modified strings"

# Writing back changes to a new rom
../gba_dumper -m correct/test6.dmp -t input/test7.tra > generated_test7.gba
../gba_dumper -f generated_test7.gba -t input/test7.tra > generated_test7.dmp
rm generated_test7.gba
diff_and_cleanup "correct/test7.dmp" "generated_test7.dmp" "Writing back changes to new rom"
