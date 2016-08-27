#!/bin/bash

diff_and_cleanup()
{
	if [ "$1" ] && [ "$2" ] && [ "$3" ]
	then
		if cmp -s "$1" "$2"
		then
			echo "\"$3\" test passed."
		else
			echo "\"$3\" test failed." 
		fi
	else
		echo "Not enough parameters passed to diff_and_cleanup() function."
	fi
	rm "$2"
}

# Basic dumping of roms. 
../gba_dumper -f test.gba -d > generated_test1.dmp
diff_and_cleanup "correct/test1.dmp" "generated_test1.dmp" "Dumping"

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