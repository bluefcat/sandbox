#include <vector>

template<typename T, typename = void>
struct is_iterator{
	static constexpr bool value = false;
};

template<typename T>
struct is_iterator<
	T, typename std::enable_if_t<
		!std::is_same_v<
			typename std::iterator_traits<T>::value_type, void
		>, void
	>
>{
	static constexpr bool value = true;
};

template<typename T>
inline constexpr bool is_iterator_v = is_iterator<T>::value;

template<typename T, typename = void >
struct is_container{
	static constexpr bool value = false;
};

template<typename T>
struct is_container<
	T, typename std::enable_if_t<
		is_iterator_v<typename T::iterator>, void
	>
>{
	static constexpr bool value = true;
};

template<typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template<
	typename T,
	typename std::enable_if_t<is_container_v<T>, T>* = nullptr
>
auto zip(const T& x){
	std::vector<std::tuple<typename T::value_type>> result{};
	auto iter = x.begin();
	for(;iter != x.end(); ++iter)
		result.push_back({*iter});
	return result;
}

template<
	typename T, 
	typename... Args,
	typename std::enable_if_t<is_container_v<T>, T>* = nullptr
>
auto zip(const T& x, Args... args){
	std::vector<
		std::tuple<typename T::value_type, typename Args::value_type...>
	> result{};
	
	auto leftiter = x.begin();
	auto y = zip(args...);
	auto rightiter = y.begin();

	for(; 
		leftiter != x.end() && rightiter != y.end();
		++leftiter, ++rightiter
	){
		result.push_back(
			std::tuple_cat(std::make_tuple(*leftiter), *rightiter)
		);
	}

	return result;
}

