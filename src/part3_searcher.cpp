#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <signal.h>

using namespace std;

void sigterm_handler(int signum){
	cout << "[" << getpid() << "] received SIGTERM\n";
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGTERM, sigterm_handler);
	if(argc != 5)
	{
		cout <<"usage: ./partitioner.out <path-to-file> <pattern> <search-start-position> <search-end-position>\nprovided arguments:\n";
		for(int i = 0; i < argc; i++)
			cout << argv[i] << "\n";
		return -1;
	}
	
	char *file_to_search_in = argv[1];
	char *pattern_to_search_for = argv[2];
	int search_start_position = atoi(argv[3]);
	int search_end_position = atoi(argv[4]);

	//TODO
	FILE *file=fopen(file_to_search_in,"r");
	if(file==nullptr){
		perror("Failed to open File");
		return 0;
	}

	int len=0;
	fseek(file, search_start_position, SEEK_SET);
	while(pattern_to_search_for[len]!='\0')len++;
	bool has=false;
	for(int i=search_start_position;i<=search_end_position;i++){
		fseek(file, i, SEEK_SET);
		bool match=true;
		for(int j=0;j<len;j++){
			int ch=fgetc(file);
			if(ch==EOF || ch!=pattern_to_search_for[j]){
				match=false;
				break;
			}
		}
		if(match){
			cout << getpid() << " found at " << i << endl;
			return 1;
		}
	}
	cout << "[-1] didn't find\n";
	return 0;
}
