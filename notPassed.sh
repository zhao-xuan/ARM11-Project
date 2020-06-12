ASSEMBLE=../../arm11_04/bin/assemble
EMULATE=../../arm11_04/bin/emulate

mkdir -p analysis/emulator/memory; mkdir -p analysis/emulator/profiler; 
mkdir -p analysis/assembler/memory; mkdir -p analysis/assembler/profiler; 

make clean all &>/dev/null; 
shopt -s extglob;
cd ../arm11_testsuite/test_cases/;
for file in !(*.*); do
	$ASSEMBLE $file.s /tmp/$file &>/dev/null;
	if ! cmp -s "/tmp/$file" "$file" ; then
		printf "\n\n$file DIFF:\n"
		xxd -b "/tmp/$file" > "/tmp/$file.bin"
		xxd -b "$file" > "/tmp/their$file.bin"
		diff "/tmp/$file.bin" "/tmp/their$file.bin"
	fi

  valgrind --leak-check=full $ASSEMBLE $file.s /tmp/$file &> ../../arm11_04/analysis/assembler/memory/$file.memtest.txt
  valgrind --leak-check=full $EMULATE $file &> ../../arm11_04/analysis/emulator/memory/$file.memtest.txt
done

