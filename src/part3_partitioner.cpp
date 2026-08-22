#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include<cstring>
#include <signal.h>
#include <cstdio>
#include<chrono>

using namespace std;

int lc=0;
int rc=0;
int searcher_pid=0;

void sigterm_handler(int signum){
	if(lc>0)kill(-lc,SIGTERM);
	if(rc>0)kill(-rc,SIGTERM);
	if(searcher_pid>0)kill(-searcher_pid, SIGTERM);
	
	cout << "[" << getpid() << "] received SIGTERM\n"; //applicable for Part III of the assignment
	exit(0);
}

int main(int argc, char **argv)
{

	signal(SIGTERM, sigterm_handler);
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
		searcher_pid=fork();

		if(searcher_pid==0){
			setpgid(0,0);
			int pattern_len=strlen(pattern_to_search_for);
			search_end_position=min((long long)(search_end_position + pattern_len -1), total_file_size-1);
			string temp=to_string(search_end_position);
			execl("./part3_searcher.out", "part3_searcher.out",argv[1], argv[2], argv[3], temp.c_str(),NULL);
		}


		cout << "[" << my_pid << "] forked searcher child " << searcher_pid << "\n";

		int status;
		waitpid(searcher_pid,&status,0);

		cout << "[" << my_pid << "] searcher child returned \n";

		int found=0;
		if(WIFEXITED(status)){
			found=WEXITSTATUS(status);
		}
		return found;
	}

	int mid= search_start_position + (search_end_position-search_start_position)/2;
	
	string str_left_start=to_string(search_start_position);
	string str_left_end=to_string(mid);
	string str_right_start=to_string(mid+1);
	string str_right_end=to_string(search_end_position);


	lc=fork();
	if(lc==0){
		setpgid(0,0);
		execl("./part3_partitioner.out", "part3_partitioner.out", argv[1],argv[2],  str_left_start.c_str(),str_left_end.c_str(), argv[5],NULL);
	}


	cout << "[" << my_pid << "] forked left child " << lc << "\n";


	rc=fork();
	if(rc==0){
		setpgid(0,0);
		execl("./part3_partitioner.out", "part3_partitioner.out",argv[1],argv[2], str_right_start.c_str(), str_right_end.c_str(), argv[5],NULL);

	}

	cout << "[" << my_pid << "] forked right child " << rc << "\n";
	
	int overall_found=0;
	int firststatus;
	pid_t finish=wait(&firststatus);
	firststatus= WIFEXITED(firststatus) ? WEXITSTATUS(firststatus) : 0;
	if(finish==lc){
		cout << "[" << my_pid << "] left child returned\n";
		if(firststatus==1){
			overall_found=1;
			kill(-rc,SIGTERM);
		}
		
		waitpid(rc,&firststatus, 0);
		cout << "[" << my_pid << "] right child returned\n";
		if(WIFEXITED(firststatus) && WEXITSTATUS(firststatus)==1)overall_found=1;

	}
	else if(finish==rc){
		cout << "[" << my_pid << "] right child returned\n";
		if(firststatus==1){
			overall_found=1;
			kill(-lc,SIGTERM);
		}
		waitpid(lc,&firststatus,0);
		cout << "[" << my_pid << "] left child returned\n";
		if(WIFEXITED(firststatus) && WEXITSTATUS(firststatus)==1)overall_found=1;
	}
		
	

	//cout << "[" << my_pid << "] received SIGTERM\n"; //applicable for Part III of the assignment
	auto end_time=std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration=end_time-start_time;
	if(search_start_position==0 && search_end_position==total_file_size-1)cout << "\nExecution Time: " << duration.count() << endl;
	return overall_found;
}
