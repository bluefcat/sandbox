#include <string>
#include <ostream>
#include "include/lz77.h"

using std::string;

int main(){
	int p =3200, q = 3200;
	Deflate::LZ77<string> lz{p, q};
	std::ifstream i{
		"target.c",
		std::ios::binary | std::ios::in
	};
	std::string target{};
	while(!i.eof()){
		std::string tmp{};
		std::getline(i, tmp);
		target += tmp +"\n";
	}

	lz.encode(target);

	std::ofstream f{
		"result.lz77", 
		std::ios::binary | std::ios::out
	};
	f << 'L' << 'Z';
	f << lz;
	f.close();
	
	std::ifstream g{
		"result.lz77",
		std::ios::binary | std::ios::in
	};
	std::vector<Deflate::LLD> x;
	char a[2] = { 0, };
	g.read(a, sizeof(char)*2);
	while(!g.eof()){
		Deflate::LLD buffer{0, 0, 0};
		g.read((char*)&buffer, sizeof(Deflate::LLD));
		x.push_back(buffer);
	}
	g.close();
	Deflate::LZ77<string> u{p, q, x};
	std::cout << u.decode() << std::endl;

	return 0;
}
