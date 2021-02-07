g++ AST.cpp ../Onegin/foperations.cpp -w -o run
./run
cd ../CPU
g++ assembler.cpp -w -o run
echo "--------------"
./run ../LexTex/test.myasm runcode