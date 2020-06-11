make clean all &>/dev/null; 
shopt -s extglob;
cd ../arm11_testsuite/test_cases/;
for file in !(*.*); do
	../../arm11_04/bin/assemble $file.s /tmp/$file &>/dev/null;
	if ! cmp -s "/tmp/$file" "$file" ; then
		printf "\n\n$file DIFF:\n"
		xxd -b "/tmp/$file" > "/tmp/$file.bin"
		xxd -b "$file" > "/tmp/their$file.bin"
		diff "/tmp/$file.bin" "/tmp/their$file.bin"
	fi
done