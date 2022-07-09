#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

namespace Deflate{
	using std::vector;
	using std::string;
	using std::ostream;
	using std::ifstream;
	using std::ofstream;
	

	using BYTE = unsigned char;

	struct LLD{
		long distance_;
		long length_;
		BYTE literal_;
		
		LLD(long distance, long length, BYTE literal);
		friend ostream& operator<<(ostream& os, const LLD& data);
		friend ofstream& operator<<(ofstream& of, const LLD& data);
	};

	template <typename T>
	class LZ77{
		public:
			LZ77(long wsize, long lsize);
			LZ77(long wsize, long lsize, vector<LLD> contents);

			unsigned long _get_size(const T& target);
			T _get_substr(const T& target, string::size_type pos, string::size_type count = string::npos) const;

			int encode(T& target);
			T decode();

			friend ostream& operator<<(ostream& os, const LZ77<string>& data);
			friend ofstream& operator<<(ofstream& of, const LZ77<string>& data);

		private:
			long wsize_;	//window buffer size
			long lsize_;	//look-ahead buffer size

			vector<LLD> contents_;
	};
};
