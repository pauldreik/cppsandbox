# cppsandbox
Sandbox for playing around with c++. Most of it is for me experimenting with language features, template metaprogramming techniques or just some interesting idea. Most of it is written in c++14.
The license is  according to [LICENSE](LICENSE), unless something else is specified in the individual source file.

Author: [Paul Dreik](https://www.pauldreik.se/)

## [callwithindex](callwithindex)
Experiment interleaving indexes with function calls, what later became accepted into [SQLiteCpp](https://github.com/SRombauts/SQLiteCpp) 

## [compiletimelogarithm](compiletimelogarithm)
Trying to select an integer type based on range, for instance to select the smallest type that can cover the given value.

## [fizzbuzz](fizzbuzz)
This shows how to do [fizzbuzz](https://en.wikipedia.org/wiki/Fizz_buzz#Programming_interviews) at compile time.

## [threadlocallogger](threadlocallogger)
Experiments with getting the amount of thread synchronization down, by utilizing thread local storage.

## [tuplereorganizer](tuplereorganizer)
Playing around with tuples.

## [parallel_partial_sum](parallel_partial_sum)
This started as a discussion on [Meeting c++ 2016](http://www.meetingcpp.com/index.php/mcpp2016.html), if [std::partial_sum](http://en.cppreference.com/w/cpp/algorithm/partial_sum) could be made parallel. This is a clumsy attempt to show that it can be done, but the speedup is less than proportional to the number of threads and sometimes worse.
