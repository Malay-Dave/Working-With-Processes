build-part1: part1.out

part1.out: part1_searcher.cpp
	g++ -g part1_searcher.cpp -o part1.out

run-part1: part1.out
	./part1.out file.txt NGTNIJGK 0 67108863 || true

part2_partitioner.out: part2_partitioner.cpp
	g++ -g part2_partitioner.cpp -o part2_partitioner.out

part2_searcher.out: part2_searcher.cpp
	g++ -g part2_searcher.cpp -o part2_searcher.out

run-part2: part2_partitioner.out part2_searcher.out
	./part2_partitioner.out file.txt NGTNIJGK 0 67108863 8388608

part3_partitioner.out: part3_partitioner.cpp
	g++ -g part3_partitioner.cpp -o part3_partitioner.out

part3_searcher.out: part3_searcher.cpp
	g++ -g part3_searcher.cpp -o part3_searcher.out

run-part3:part3_partitioner.out part3_searcher.out
	./part3_partitioner.out file.txt NGTNIJGK 0 67108863 8388608 || true

clean-part1:
	rm part1.out

clean-part2:
	rm part2_*.out

clean-part3:
	rm part_*.out

clean:
	rm *.out
