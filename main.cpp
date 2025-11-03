#include <source_location>
#include <tuple>
#include <iostream>
#include <functional>

#include "logger.h"

void event_output(LogEvent& evt);

static Logger main_logger("main", event_output);

#if 0
constexpr int TRACE = 1;
constexpr int DEBUG = 2;
constexpr int INFO = 3;
constexpr int OFF = 10;

using std::cout;

template<typename... T>
void do_real_log(std::tuple<T...> tup){
	std::ostream& os = std::cout;
	std::apply (
		[&os](T const&... tupleArgs) {
			os << '[';
			std::size_t n{0};
			((os << tupleArgs << (++n != sizeof...(T) ? ", " : "")), ...);
			os << ']';
		}, tup
	);
	os << "\n";
}

template<typename... T>
void log(T... args){
	auto tup = std::make_tuple(args...);

	do_real_log(tup);
}

// https://stackoverflow.com/questions/60909588/brace-initialize-tuple-in-function-argument
template<typename... T>
auto source_log(T... args){
	auto tup = std::make_tuple(args...);

	return [tup](const std::source_location location = std::source_location::current()){
		std::cout << "line is: " << location.line() << " ";
		do_real_log(tup);
	};
}

class Logger{
private:
	template<typename... T>
	void do_log(std::tuple<T...> tup, std::source_location loc = std::source_location::current()){
		std::cout << "line is: " << loc.line() << " ";
		do_real_log(tup);
	}

	void do_nothing(){}

public:
	int level = OFF;

	template<typename... T>
	auto debug1(T... args){
		if(level > DEBUG){
			// we try to do nothing here, but the compiler does not like that we return
			// two different types of lambdas in the two branches, even though they are
			// effectively the same.
			//return [](){};
			
		}
		
		auto tup = std::make_tuple(args...);
		// This lambda correctly prints out the source location
		return [tup](const std::source_location location = std::source_location::current()){
			std::cout << "[debug] line " << location.line() << ": ";
			do_real_log(tup);
		};
	}

	template<typename... T>
	std::function<void()> debug(T... args){
		if(level > DEBUG){
			// do nothing
			return [](){};
			
		}
		
		auto tup = std::make_tuple(args...);
		// This lambda does not work, because when using std::function the value is not inserted:
		// https://www.reddit.com/r/cpp_questions/comments/ncn3mi/stdfunction_with_function_that_has_default/
		return [tup](const std::source_location location = std::source_location::current()){
			std::cout << "[debug] line " << location.line() << ": ";
			do_real_log(tup);
		};
	}

	template<typename... T>
	auto debug_fmt(const std::format_string<T...> fmt, T... args){
		bool do_log = level <= DEBUG;
		auto tup = std::make_tuple(args...);
		auto sv = fmt.get();
		return [sv, tup, do_log](const std::source_location location = std::source_location::current()){
			if(!do_log){
				// do nothing
				return;
			}

			auto format_args = std::apply([](auto&&... args) {
				return std::make_format_args(args...);
			}, tup);
			std::cout << "[debug] line " << location.line() << ": ";
			std::cout << std::vformat(sv, format_args) << "\n";
		};
	}

};

#endif

void event_output(LogEvent& evt){
    std::cout << std::format("[{:%FT%R:%S}] {} {} - {}", evt.m_time, evt.level_to_string(), evt.m_logger_name, evt.render()) << "\n";
//    std::cout << evt.m_stacktrace << "\n";
}

int main(int argc, char** argv){
    int level = Logger::OFF;
	if(argc >= 2){
		level = std::stoi(argv[1]);
	}

	std::cout << "level is " << level << "\n";

    main_logger.set_level(level);
    main_logger.info("Info something something")();
    main_logger.debug("this is a test", __LINE__)();
    main_logger.debug("this is another test {}", __LINE__)();
    main_logger.debug("foo test")();
	//l.debug_fmt("hi{", 5)(); // compile error due to mismatched { in format string, g++ 14.2
    main_logger.debug("hi {}", "todd")();

	return 0;
}
