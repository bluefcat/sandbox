#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <format> 

namespace Deflate{
	using std::vector;
	using std::string;
	using std::ostream;
	using std::ifstream;
	using std::ofstream;
	
	using LONG = long long;
	using SHORT = unsigned short;
	using BYTE = unsigned char;

	struct LLD{
		SHORT distance_;
		SHORT length_;
		BYTE literal_;
		
		LLD(SHORT distance, SHORT length, BYTE literal);
		friend ostream& operator<<(ostream& os, const LLD& data);
		friend ofstream& operator<<(ofstream& of, const LLD& data);
	};

	template <typename T> 
	concept HasLength = requires(T x){
		x.length();
	};
	template <typename T>
	concept HasSubstr = requires(T x){
		x.substr();
	};

	template <typename T>
	requires HasLength<T> && HasSubstr<T>
	class LZ77{
		public:
			LZ77(long wsize, long lsize);
			LZ77(long wsize, long lsize, vector<LLD> contents);

			unsigned long _get_size(const T& target) const;
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
