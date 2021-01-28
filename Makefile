.PHONY: test coverage
.DEFAULT_GOAL := test

test: ## run tests quickly with ctest
	rm -rf build/
	mkdir build
	cd build && cmake .. -Drviews_ENABLE_UNIT_TESTING=1 -Drviews_ENABLE_EXAMPLES=1
	cmake --build build --config Release
	cd build/test && ctest -C Release -VV

coverage: ## check code coverage quickly GCC
	rm -rf build/
	rm -rf coverage/
	mkdir build
	cd build && cmake .. -Drviews_ENABLE_UNIT_TESTING=1 -Drviews_ENABLE_CODE_COVERAGE=1
	cmake --build build --config Release
	cd build/test && ctest -C Release -VV
	mkdir coverage
	find . -type f -name '*.gcno' -exec gcov -pb {} +
	gcovr -b -k -r . --html --html-details -o coverage/index.html
	mv *.gcov coverage


