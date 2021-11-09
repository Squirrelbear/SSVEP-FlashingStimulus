#include <iostream>
#include <fstream>

int main()
{
	std::ofstream myfile;
	myfile.open ("batchconv.bat");

	myfile << "cd C:\\outputVideo\n";
	for(int i = 0; i <= 922; i++)
		myfile << "texconv -f DXT1 -o \"C:\\\\outputVideo\\\\dds2\" -nologo video" << i << ".png\n";
	myfile.close();
}