convert: src/csv_to_hty.cpp
	g++ -std=c++20 \
		-o bin/convert.out \
		src/csv_to_hty.cpp;

analyze: src/analyze.cpp
	g++ -std=c++20 \
		-o bin/analyze.out \
		src/analyze.cpp;

