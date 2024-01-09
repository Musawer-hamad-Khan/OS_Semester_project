#include<iostream>
#include<fcntl.h>
#include<unistd.h>
#include<sstream>
using namespace std;

int process_file_creat();

int process_file_write(int file_descriptor);

int process_file_open(){

	string file_name;
	string user_input;
	cout<<"For reading a process file press R"<<endl<<"For creating and reading new process file press C"<<endl;
	cin>>user_input;

	if (user_input == "R"){
		cout<<"Opening process file"<<endl;
		int fd=open("process_file.txt",O_RDONLY);
		
		if (fd == -1){
			cout<<"Such file doesn't exist"<<endl<<"For creating new process file press C or Press any key to exit"<<endl;
			cin>>user_input;

			if (user_input == "C"){
				fd=process_file_creat();
				return fd;

			}
			else{
				return 0;
			}
		}
		else{

			cout<<"File is open"<<endl;
			return fd;
		}
		}

	else if (user_input == "C"){
		int fd=process_file_creat();
		return fd;

		//return 0;
		}
	return 0;	

		}

int process_file_creat(){
	cout<<"--------------------------"<<endl<<"Creating and opening new file "<<endl;
	int fd=open("process_file.txt",O_RDWR|O_CREAT|O_APPEND,0777);
	
	if (fd == -1){
		cout<<"Failed to create and open file";
		return 0;

	}

	else{
		return process_file_write(fd);

	}}

int process_file_write(int file_descriptor){
	int number_of_attributes;
	cout<<"Enternumber of attributes of each process you want to add"<<endl;
	cin>> number_of_attributes;
	//string buffer[number_of_attributes];
	string user_input;
	int attribute_value;
	string buffer;
	
	while (1){
		cout<<"Press E to enter detail of a procees"<<endl<<"Press H to quit"<<endl;
		cin>>user_input;
		
		if (user_input == "E"){
			stringstream ss;
		for (int attr_num=0; attr_num <= number_of_attributes-1;attr_num++){
			
			cout<<"Enter value of attribute"<<' '<<attr_num<<endl;
			cin>>attribute_value;
			ss<<attribute_value;
			ss<<"\t";
			cout<<ss.str();
			buffer=ss.str();
			size_t buffer_size=buffer.size();
			
			int bytes_written=write(file_descriptor,buffer.c_str(),buffer_size);
			
			//buffer[attr_num]=ss.str();
			ss.str("");
			}
			ss<<"\n";
			buffer=ss.str();
			size_t buffer_size=buffer.size();
			int bytes_written=write(file_descriptor,buffer.c_str(),buffer_size);
			
	
		cout<<"Buffer valuee is"<<buffer<<endl;	
		//buffer[number_of_attributes]="\n";	

		//out<<"Size of buffer"<<sizeof(buffer)<<endl;
		
		//cout<<"Bytes written"<<bytes_written<<endl;
		
		}

		else if(user_input == "H"){

			return file_descriptor;

		}

	}

}	

	

		

int main(){

	int inp_file=process_file_open();

	cout<<"File descritor"<<inp_file<<endl;

	

}
