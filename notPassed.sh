make clean all &>/dev/null; 
shopt -s extglob;
cd ../arm11_testsuite/test_cases/;
for file in !(*.*); do
	../../arm11_04/bin/emulate $file > /tmp/$file.out;
	diff "/tmp/$file.out" "$file.out";
done