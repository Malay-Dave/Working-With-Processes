#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include<cstring>
#include <signal.h>
#include <cstdio>
#include<chrono>

using namespace std;

int main(int argc, char **argv)
{
	auto start_time=std::chrono::high_resolution_clock::now();
	if(argc != 6)
	{
		cout <<"usage: ./partitioner.out <path-to-file> <pattern> <search-start-position> <search-end-position> <max-chunk-size>\nprovided arguments:\n";
		for(int i = 0; i < argc; i++)
			cout << argv[i] << "\n";
		return -1;
	}
	pid_t my_pid=getpid();
	
	char *file_to_search_in = argv[1];
	char *pattern_to_search_for = argv[2];
	int search_start_position = atoi(argv[3]);
	int search_end_position = atoi(argv[4]);
	int max_chunk_size = atoi(argv[5]);
	
	// FILE *file=fopen(argv[1], "r");
	// fseek(file, 0, SEEK_END);
	// long long total_file_size=ftell(file);
	// fclose(file);	
	long long total_file_size=67108864;

	//TODO
	cout << "[" << my_pid << "] start position = " << search_start_position << " ; end position = " << search_end_position << "\n";
	int current_chunk_size=search_end_position-search_start_position+1;
	if(current_chunk_size<=max_chunk_size){
		int searcher_pid=fork();

		if(searcher_pid==0){
			int pattern_len=strlen(pattern_to_search_for);
			search_end_position=min((long long)(search_end_position + pattern_len -1), total_file_size-1);
			string temp=to_string(search_end_position);
			execl("./part2_searcher.out", "part2_searcher.out",argv[1], argv[2], argv[3], temp.c_str(),NULL);
		}
		cout << "[" << my_pid << "] forked searcher child " << searcher_pid << "\n";
		waitpid(searcher_pid,NULL,0);
		cout << "[" << my_pid << "] searcher child returned \n";
		return 0;
	}

	int mid= search_start_position + (search_end_position-search_start_position)/2;
	
	string str_left_start=to_string(search_start_position);
	string str_left_end=to_string(mid);
	string str_right_start=to_string(mid+1);
	string str_right_end=to_string(search_end_position);


	int lc=fork();
	if(lc==0){
		execl("./part2_partitioner.out", "part2_partitioner.out", argv[1],argv[2],  str_left_start.c_str(),str_left_end.c_str(), argv[5],NULL);
	}


	cout << "[" << my_pid << "] forked left child " << lc << "\n";


	int rc=fork();
	if(rc==0){
		execl("./part2_partitioner.out", "part2_partitioner.out",argv[1],argv[2], str_right_start.c_str(), str_right_end.c_str(), argv[5],NULL);

	}

	cout << "[" << my_pid << "] forked right child " << rc << "\n";
	
	waitpid(lc,NULL, 0);
	cout << "[" << my_pid << "] left child returned\n";

	waitpid(rc,NULL,0);

	cout << "[" << my_pid << "] right child returned\n";
	//cout << "[" << my_pid << "] left child returned\n";
	//cout << "[" << my_pid << "] right child returned\n";*/
	//cout << "[" << my_pid << "] forked searcher child " << searcher_pid << "\n";
	//cout << "[" << my_pid << "] searcher child returned \n";
	//cout << "[" << my_pid << "] received SIGTERM\n"; //applicable for Part III of the assignment
	auto end_time=std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration=end_time-start_time;
	if(search_start_position==0 && search_end_position==total_file_size-1)cout << "\nExecution Time: " << duration.count() << endl;
	return 0;
}
