#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 1025 
#include <string.h>
#include <arpa/inet.h>   
#include <iostream>
#include <fstream>
#include <errno.h>
#include<vector>

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// take pics from a folder and send them to client, send picture size right before sending each picture.

using namespace std;


int main(int argc, char const *argv[]) 
{ 
int s, new_socket, valread; 
struct sockaddr_in address; 

if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
{ 
    perror("socket failed"); 
    exit(1); 
} 
address.sin_family = AF_INET;
address.sin_addr.s_addr = INADDR_ANY;
address.sin_port = htons(PORT);
   
    //  attach socket to port 
if (bind(s,(struct sockaddr *)&address , sizeof(address)) < 0)
    { 
        perror("bind failed"); 
        exit(1); 
    } 
    listen(s, 3);
    
    
if(new_socket= accept(s, (struct sockaddr *) 0, 0))
{puts("Connection accepted");}
        
        char buff[24] ;
        char *msg=(char*)"hi from server";
      
        int received_bytes = 0;
        int remaining_bytes = sizeof(buff);
        send(new_socket , msg , strlen(msg) , 0 ); 
        printf("Message sent ");    

            
while((remaining_bytes > 0) )
{
        
   
    if (new_socket == -1)                  
    { 
        perror("accept"); 
        exit(1); 
    } 
    else
        {
        
        int res = recv(new_socket , &buff[received_bytes],remaining_bytes,0);
        received_bytes += res;
        remaining_bytes -= res;
     }

}printf("%s\n\n",buff);

// open folder and send image sizes to client//
const char *dir = "/home/Desktop/pics";

if (chdir(dir) == -1) {
    perror(dir);
    return 1;
}

DIR *dirp = opendir(".");

for (struct dirent *dent; (dent = readdir(dirp)) != NULL; )
{
const char *nm = dent->d_name;

if (strcmp(nm, ".") == 0 || strcmp(nm, "..") == 0)
    { continue;}

struct stat file_stats;
if (stat(nm, &file_stats) == -1)
   { perror(nm);}

else
{//printf("%9u: %s\n", (unsigned)file_stats.st_size, nm);

long converted_number = htonl((unsigned)file_stats.st_size); //convert size to integer
int size_chunks=0;
size_chunks=send(new_socket, &converted_number, sizeof(converted_number), 0); //send image  sizes 
cout<<size_chunks<<"=====1=1=1=1========"<<"bytes"<<endl; // print how many bytes
//break;
}



ifstream stream(nm, std::ios::in | std::ios::binary);  
if(stream.is_open())                                     //open folder
    {
vector<char> imageDataVec((istreambuf_iterator<char>(stream)), istreambuf_iterator<char>());
cout << "Size=of=image=== " << imageDataVec.size() << " bytes";
long conv_num= htonl(imageDataVec.size());

int nbytes=0;  //start integer at 0 for counting

while (1) //infinite loop
{  
     nbytes = send(new_socket, &imageDataVec, imageDataVec.size(), 0); //send images

    if (nbytes <= 0) {
        std::clog << "error: while sending image\n";
        break;
}
    else

    cout<<nbytes<<"=====1=1=1=1========"<<"bytes"<<endl; //print how many bytes
}
                      break;
     
        }
    
}
else
{cout<<"can't open folder"<<endl;}


}


closedir(dirp); //close directory
close(s);
close(new_socket); //close socket
return 0; 
} 

   


