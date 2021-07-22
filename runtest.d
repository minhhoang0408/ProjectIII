import std.stdio;

auto runProgram(int k, int square){
	import std.exception,  std.process, std.conv;

	auto s = "python generateBig.py " ~ to!string(square) ~ " | ./a.out -k " ~ to!string(k) ~ " -p > test/test.svg";

	auto result = executeShell(s);

	return result.output;
}

void main(){
	int i = 8;
	//for(int i=0;i<=25;i++)
	{
		//write(i," ",runProgram(i,i));
		runProgram(i, i);
	}
}
