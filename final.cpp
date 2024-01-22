#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>
#include <thread>
#include <queue>

using namespace std;
struct process {
  int id;
  int value;
  int atime;
  int btime;
  int unit_count;
  int unit_1;
  int unit_2;
  int unit_3;
  int unit_4;
  int unit_5;
};
int process_file_open(const char *filename);
int process_file_creat(const char *filename);
int process_file_write(int file_descriptor);
int process_file_read(const char *filename);
int dispatch(queue <process>& waiting_queue);
int unit_1(queue <process>& queue);
int unit_2(queue <process>& queue);
int unit_3(queue <process>& queue);
int unit_4(queue <process>& queue);
int print_unit(int req_val, process p);
int string_len(int fd);
int console();
int console() {
  string user_input;
  cout << "For reading a process file press R" << endl
       << "For creating and reading new process file press C" << endl;
  
  cin >> user_input;

  if (user_input == "R") {
    string user_input;
    cout << "--------------------" << endl
         << "Enter name of file tou want to read" << endl;
    cin >> user_input;
    const char *file_name = user_input.c_str();
    int fd = process_file_open(file_name);

    if (fd == -1) {
      cout << "File not found" << endl;
      cout << "For creat new file press C" << endl
           << "Press R to restart" << endl
           << "Press H to exit" << endl;
      cin >> user_input;

      if (user_input == "C") {
        cout << "Enter the name of file you want to Create and Read" << endl;
        cin >> user_input;
        const char *file_name = user_input.c_str();
        int fd = process_file_creat(file_name);
        process_file_write(fd);
        process_file_read(file_name);
      }

      else if (user_input == "R") {
        console();
      }

      else if (user_input == "H") {
        return 0;
      }

      else {
        process_file_read(file_name);
      }
    }

    else {
      process_file_read(file_name);
    }
  }

  else if (user_input == "C") {
    string user_input;
    cout << "Enter the name of file you want to Create and Read" << endl;
    cin >> user_input;
    const char *file_name = user_input.c_str();
    int fd = process_file_creat(file_name);
    process_file_write(fd);
    process_file_read(file_name);
  }

  else {
    return 0;
  }
  return 0;
}

int process_file_open(const char *filename) {
  cout << "Opening process file" << endl;
  // const char filename;
  int fd = open(filename, O_RDONLY, 0777);

  if (fd == -1) {
    cout << "Such file doesn't exist" << endl;
    return fd;
  }

  else {
    return fd;
  }
}

int process_file_creat(const char *filename) {
  cout << "--------------------------" << endl
       << "Creating and opening new file " << endl;
  int fd = open(filename, O_RDWR | O_CREAT | O_APPEND, 0777);
  
  if (fd == -1) {
    cout << "Failed to create and open file";
    return -1;
  }

  else {
    return fd;
  }
}

int process_file_write(int file_descriptor) {
  int number_of_attributes;
 
  cout << "Enter number of attributes of each process you want to add" << endl;
  cin >> number_of_attributes;
  // string buffer[number_of_attributes];
  string user_input;
  int attribute_value;
  string buffer;
  
  while (1) {
    cout << "Press E to enter detail of a procees" << endl
         << "Press H to quit" << endl;
    cin >> user_input;
    
    if (user_input == "E") {
      stringstream ss;
      for (int attr_num = 0; attr_num <= number_of_attributes - 1; attr_num++) {
        cout << "Enter value of attribute" << ' ' << attr_num << endl;
        cin >> attribute_value;
        ss << attribute_value;
        ss << "\t";
        cout << ss.str();
        buffer = ss.str();
        size_t buffer_size = buffer.size();
        int bytes_written = write(file_descriptor, buffer.c_str(), buffer_size);
        cout << bytes_written << endl;
        // buffer[attr_num]=ss.str();
        ss.str("");
      }
      ss << "\n";
      buffer = ss.str();
      size_t buffer_size = buffer.size();
      int bytes_written = write(file_descriptor, buffer.c_str(), buffer_size);
      cout << bytes_written << endl;
      cout << "Buffer valuee is" << buffer << endl;
      // buffer[number_of_attributes]="\n";
      // out<<"Size of buffer"<<sizeof(buffer)<<endl;
      // cout<<"Bytes written"<<bytes_written<<endl;
    }

    else if (user_input == "H") {
      close(file_descriptor);
      return 0;
    }
  }
}
// void read_dispatch(int fd) { process waiting_queue[1]; }

int process_file_read(const char *filename) {
  int fd = process_file_open(filename);
  cout << "Process file is being read" << endl;
  int buffer_size = string_len(fd);
  fd = process_file_open(filename);
  char buffer[1];
  int process_attribute_value;
  int attribute_array[buffer_size];
  int next_position = 0;
  queue <process> waiting_queue;
  int waiting_queue_size = 0;
  //cout << "The is " << buffer_size << endl;
  // stringstream to hold digit characters
  string previous_str;
  while (size_t bytes_read=read(fd, buffer, sizeof(buffer)) != 0) {
    stringstream ss;
    
    if (isdigit(buffer[0])) {
      //cout << buffer[0] << endl;
      // cout<<"this SS value"<<ss.str()<<endl;
      previous_str = previous_str + buffer[0];
      //cout << "Pre str" << previous_str << endl;
    } 
    
    else if (buffer[0] == '\t' && previous_str != "") {
      // cout<<"SS value"<<ss.str()<<endl;
      //cout << "Required value" << previous_str << endl;
      ss << previous_str;
      ss >> process_attribute_value;
      attribute_array[next_position] = process_attribute_value;
      previous_str = "";
      next_position++;

    } else if (buffer[0] == '\n') {
      ss << previous_str;
      ss >> process_attribute_value;
      attribute_array[next_position] = process_attribute_value;
      cout << "i'm here"<< waiting_queue_size << endl;
      next_position = 0;
      previous_str = "";
      process p;
      p.id = attribute_array[0];
      p.value = attribute_array[1];
      p.atime = attribute_array[2];
      p.btime = attribute_array[3];
      p.unit_count = attribute_array[4];
      p.unit_1 = attribute_array[5];
      p.unit_2 = attribute_array[6];
      p.unit_3 = attribute_array[7];
      p.unit_4 = attribute_array[8];
      p.unit_5 = attribute_array[9];
      waiting_queue.push(p);
      cout<<"waiting queue size is "<<waiting_queue_size<<endl;
      if (waiting_queue_size == 5){
        dispatch(waiting_queue);
        for (int process=0; process <=waiting_queue_size; process++){
        	waiting_queue.pop();	
        	}
        waiting_queue_size=0;	
        
      }
      else{
      	  waiting_queue_size++;
      }
      
    }
  
      }
   if (waiting_queue_size !=0 ){
      dispatch(waiting_queue);
      return 0;
      }   
   else{
   	return 0;
   }   
   
  }
  
  
 


int dispatch(queue <process>& w_queue) {
  queue <process> tempqueue=w_queue;	
  cout <<"Dispactcherrrrr here" <<tempqueue.front().id << endl;
  	
    thread thread1(unit_1, ref(w_queue));
    thread1.join();
  
    thread thread2(unit_2, ref(w_queue));
    thread2.join();

    thread thread3(unit_3, ref(w_queue));
    thread3.join();

    thread thread4(unit_4, ref(w_queue));
    thread4.join();
  
    return 0;
}
int unit_1(queue<process>& inputqueue){	
  queue<process>unit_queue= inputqueue;
  process p[5]; 
  int index=0;
  for (int process=0; process<=5; process++){
      if (unit_queue.front().unit_1>0){
      	//cout<<"Unittttt queueeeeee value "<<unit_queue.front().id<<endl;
        p[index]=unit_queue.front();
        unit_queue.pop();
        index++;
      }
      else{
        unit_queue.pop();
      }
  //cout<<"This is theeeeeeeeee indexxxxxx"<<index<<endl;     
  }
  for (int current_process_no=0; current_process_no < index; current_process_no++){
    for (int next_process_no=0; next_process_no < index;next_process_no++){
      if (p[current_process_no].btime < p[next_process_no].btime){
          process current_process=p[current_process_no];
          p[current_process_no]=p[next_process_no];
          p[next_process_no]=current_process;
      }
      
    }
   
     }
      
  for(int p_no=0; p_no < index; p_no++){
    
    int req_val=p[p_no].value;
    req_val=req_val+7;
    req_val=req_val%10000;
    sleep(double(0.5));
    cout<<"This unit 1"<<endl<<"Process with id"<<' '<<p[p_no].id<<' '<<"with burst time"<<' '<<p[p_no].btime<<' '<<"is completed"<<endl;
    print_unit(req_val,p[p_no]);
  }
  return 0;
  }
  //int req_val = p.value;
  //req_val = req_val + 7;
  //req_val = req_val % 10000;
  //sleep(double(0.5));
  //print_unit(req_val, p);
  //return 0;


int unit_2(queue<process>& inputqueue) {
  queue<process>unit_queue= inputqueue;
  process p[5]; 
  int index=0;
  for (int process=0; process<=5; process++){
      if (unit_queue.front().unit_2>0){
      	//cout<<"Unittttt queueeeeee value "<<unit_queue.front().id<<endl;
        p[index]=unit_queue.front();
        unit_queue.pop();
        index++;
      }
      else{
        unit_queue.pop();
      }
  //cout<<"This is theeeeeeeeee indexxxxxx"<<index<<endl;     
  }
  for (int current_process_no=0; current_process_no < index; current_process_no++){
    for (int next_process_no=0; next_process_no < index;next_process_no++){
      if (p[current_process_no].btime > p[next_process_no].btime){
          process current_process=p[current_process_no];
          p[current_process_no]=p[next_process_no];
          p[next_process_no]=current_process;
      }
      
    }
   
     }
      
  for(int p_no=0; p_no < index; p_no++){
    int req_val=p[p_no].value;
    req_val = 2*req_val;
    req_val=req_val%10000;
    sleep(double(0.5));
    cout<<"This unit 2"<<endl<<"Process with id"<<' '<<p[p_no].id<<' '<<"with burst time"<<' '<<p[p_no].btime<<' '<<"is completed"<<endl;
    print_unit(req_val,p[p_no]);
  }
  return 0;
}

int unit_3(queue<process>& inputqueue) {
  queue<process>unit_queue= inputqueue;
  process p[5]; 
  int index=0;
  for (int process=0; process<=5; process++){
      if (unit_queue.front().unit_3>0){
      	//cout<<"Unittttt queueeeeee value "<<unit_queue.front().id<<endl;
        p[index]=unit_queue.front();
        unit_queue.pop();
        index++;
      }
      else{
        unit_queue.pop();
      }
  //cout<<"This is theeeeeeeeee indexxxxxx"<<index<<endl;     
  }
      
  for(int p_no=0; p_no < index; p_no++){
    int req_val=p[p_no].value;
    int exponent = 5;
    req_val = pow(req_val, exponent);
    req_val=req_val%10000;
    sleep(double(0.5));
    cout<<"This unit 3"<<endl<<"Process with id"<<' '<<p[p_no].id<<' '<<"with burst time"<<' '<<p[p_no].btime<<' '<<"is completed"<<endl;
    print_unit(req_val,p[p_no]);
  }
  return 0;	
}

int unit_4(queue<process>& inputqueue) {
  queue<process>unit_queue= inputqueue;
  process p[5]; 
  int index=0;
  for (int process=0; process<=5; process++){
      if (unit_queue.front().unit_4>0){
      	//cout<<"Unittttt queueeeeee value "<<unit_queue.front().id<<endl;
        p[index]=unit_queue.front();
        unit_queue.pop();
        index++;
      }
      else{
        unit_queue.pop();
      }
  //cout<<"This is theeeeeeeeee indexxxxxx"<<index<<endl;     
  }
      
  for(int p_no=0; p_no < index; p_no++){
    int req_val=p[p_no].value;
    req_val=req_val-19;
    req_val=req_val%10000;
    sleep(double(0.5));
    cout<<"This unit 4"<<endl<<"Process with id"<<' '<<p[p_no].id<<' '<<"with burst time"<<' '<<p[p_no].btime<<' '<<"is completed"<<endl;
    print_unit(req_val,p[p_no]);
  }
  return 0;	
}

int print_unit(int req_val, process p) {
  cout << "Orignal value"<<' '<< p.value<<' '<< "New value" <<' '<< req_val << endl;
  cout <<"-----------------------------------------------"<<endl;
  return 0;
}

int string_len(int fd) {
  char buffer[1];
  int count = 0;
  while (read(fd, buffer, sizeof(buffer)) > 0) {
    
    if (buffer[0] == '\n') {
      return count;
      
    } else if (buffer[0] != '\t') {
      count++;
    }
  }
  close(fd);
  return count;
}

int main() {
  int inp_file = console();
  cout << inp_file << endl;
  // cout << "File descritor" << inp_file << endl;
}
