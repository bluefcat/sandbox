#include "lz77.h"

namespace Deflate{
//LLD

	LLD::LLD(long distance, long length, BYTE literal)
		:distance_(distance), length_(length), literal_(literal){}

	ostream& operator<<(ostream& os, const LLD& data){
		os << "(" << data.distance_ << 
			  ", " << data.length_ <<
			  ", " << data.literal_ << ")";

		return os;
	}

	ofstream& operator<<(ofstream& of, const LLD& data){
		of << (long)data.distance_ << (long)data.length_ << data.literal_;
		return of;
	}
}

namespace Deflate{
//LZ77<string>
	template<>
	LZ77<string>::LZ77(long wsize, long lsize)
		:wsize_(wsize), lsize_(lsize){}

	template<>
	LZ77<string>::LZ77(long wsize, long lsize, vector<LLD> contents)
		:wsize_(wsize), lsize_(lsize), contents_(contents){}

	template<>
	unsigned long LZ77<string>::_get_size(const string& target){
		return target.length();
	}

	template<>
	string LZ77<string>::_get_substr(	const string& target, 
										string::size_type pos, 
										string::size_type count) const{
		return target.substr(pos, count);
	}

	template<>
	int LZ77<string>::encode(string& target){
		long idx = lsize_;
		long bsize = lsize_ + wsize_; //buffer size

		unsigned long target_size = _get_size(target);

		while(idx <= target_size + lsize_){
			long buffer_start = std::max((long)0, idx - bsize);
			long partition = std::min(idx - lsize_, wsize_);

			string buffer = _get_substr(target, buffer_start, std::min(idx, bsize));
			string lookahead = _get_substr(buffer, partition, lsize_);

			//find long pattern in window
			int length = _get_size(lookahead);
			string::size_type n = std::string::npos;

			for(length; length >= 1; length --){
				string::size_type tmp = buffer.find(lookahead.substr(0, length));

				if(tmp != string::npos and
				   tmp < partition and
				   // buffer_start + partition is "lookahead" start index in "target"
				   buffer_start + partition + length <= target_size - 1){
					n = tmp; break;
				}
			}

			long distance = length != 0? partition - n: 0;
			
			contents_.push_back(
				LLD(
					distance, 
					length, 
					target[buffer_start + partition + length]
				)
			);

			idx += length + 1;
		}
		return 0;
	}

	template<>
	string LZ77<string>::decode(){
		string result;

		
		int p = 0;
		for(auto &i: contents_){
			long distance = i.distance_;
			long length = i.length_ + 1;
			BYTE literal = (BYTE)i.literal_;

			if(!literal) break;

			for(int i = 0; i < length; i ++){
				if(i == length - 1) result += literal;
				else{
					result += result[p-distance+i];
				}
			}

			p += length;

		}
		
		return result;
	}

	ostream& operator<<(ostream& os, const LZ77<string>& data){
		for(auto& i: data.contents_){
			operator<<(os, i);
		}

		return os;
	}

	ofstream& operator<<(ofstream& of, const LZ77<string>& data){
		for(auto& i: data.contents_){
			operator<<(of, i);
		}

		return of;
	}
}



