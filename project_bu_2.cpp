#include <fcntl.h>

#include <iostream>

#include <sstream>

#include <unistd.h>

#include <sys/stat.h>

using namespace std;



struct process {

  int id;

  int value;

  int atime;

  int unit_count;

};

int process_file_open(const char* filename);

int process_file_creat(const char* filename);

int process_file_write(int file_descriptor);

void read_dispatch(int fd);

int process_file_read(const char* filename);

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

    const char* file_name=user_input.c_str();

    int fd=process_file_open(file_name);

    if (fd==-1){

      cout<<"File not found"<<endl;

      cout<<"For creat new file press C"<<endl<<"Press R to restart"<<endl<<"Press H to exit"<<endl;

      cin>>user_input;

      if (user_input=="C"){

        cout<<"Enter the name of file you want to Create and Read"<<endl;

        cin>>user_input;

        const char* file_name=user_input.c_str();

        int fd=process_file_creat(file_name);  

        process_file_write(fd);

        process_file_read(file_name);

    }

      else if (user_input == "R"){

        console();

      }

      else if (user_input == "H"){

        return 0;

      }

      else{

        process_file_read(file_name);

      }

      }

    else{

      process_file_read(file_name);

    }  

    }

  else if(user_input == "C"){

      string user_input;

      cout<<"Enter the name of file you want to Create and Read"<<endl;

        cin>>user_input;

        const char* file_name=user_input.c_str();  

        int fd=process_file_creat(file_name);  

        process_file_write(fd);

        process_file_read(file_name);

    }

 else{

  return 0;

 }  

  return 0;  

  }







int process_file_open( const char* filename) {

  cout << "Opening process file" << endl;

      //const char filename;

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



         int bytes_written=write(file_descriptor, buffer.c_str(), buffer_size);
         cout<<bytes_written<<endl;

        // buffer[attr_num]=ss.str();

        ss.str("");

      }

      ss << "\n";

      buffer = ss.str();

      size_t buffer_size = buffer.size();

     int bytes_written=write(file_descriptor, buffer.c_str(), buffer_size);
      cout<<bytes_written<<endl;

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



int process_file_read(const char* filename) {

  int fd=process_file_open(filename);

  cout << "Process file is being read" << endl;

  int buffer_size = string_len(fd);

  // cout<<"Size of the string"<<buffer_size<<endl;

  fd=process_file_open(filename);

  char buffer[buffer_size];

  stringstream ss;

  int process_attribute_value;

  ssize_t bytes_read = read(fd, buffer, buffer_size);

  cout << "Buffer value" << buffer << endl;

  ss << buffer[2];

  ss >> process_attribute_value;

  cout << "Value that is read from file" << process_attribute_value << endl

       << "Bytes read" << bytes_read << endl;

  return 0;

}

int string_len(int fd) {

  char buffer[1];

  int count=0;

  while (read(fd, buffer, sizeof(buffer)) > 0) {

    if (buffer[0] == '\n') {

      return count;

    } else {

      count++;

    }

  }

  close(fd);

  return count;

}



int main() {

  int inp_file = console();
  cout<<inp_file<<endl;

  // cout << "File descritor" << inp_file << endl;

}
