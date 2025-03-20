/*********************************************************************/
/* PhotoLab_client.c: Assignment 8 for CENG231, Spring 2023          */
/*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netdb.h>
#include <netdb.h>
#define BUF_SIZE 512
/* Displays an error message to the command line */
void FatalError(const char *Program, const char *ErrorMsg);
/* Sends a request to the specified socket */
int SendRequest(const int SocketFD, const char *ReqMsg, const char *Program)
    {
        int n;
        char buffer[BUF_SIZE];
        memset(buffer, 0, sizeof(buffer));
        n = write(SocketFD, ReqMsg, strlen(ReqMsg));
        if (n < 0)
            {
                FatalError(Program, "Write to socket failed");
            }
        printf("%s: Waiting for response...\n", Program);
        memset(buffer, 0, sizeof(buffer));
        n = read(SocketFD, buffer, BUF_SIZE -1);
        if (n < 0)
            {
                FatalError(Program, "Reading from socket failed");
            }
        buffer[n] = 0;
        printf("%s: Recieved response: %s\n", Program, buffer);
        return n;
    }
/* Prints out the available command flags */
void PrintUsage();
int main(int argc, char *argv[])
{
  int x=1, socket_fd, port_no, autotestCommand = 0, printCommand = 0;
  char *server_ip;
  struct sockaddr_in
        server_addr;
  struct hostent
        *server;
  if (argc < 6 || argc > 8)
    {
        FatalError(argv[0], "Usage error");
    }
  while (x < argc) {
    /* the server ip address option */
    if (strcmp(argv[x], "-ip") == 0) {
      if (x < argc - 1) {
        server_ip = argv[x+1];
      } /*fi*/
      else {
        printf("Missing argument for server IP address!\n");
        return 6;
      } /*else*/
      x += 2;
      continue;
    } /*fi*/
  /**************************************************************/
  /* Please add support for the -p option here...               */
  /**************************************************************/
    if (strcmp(argv[x],"-p") == 0)
        {
            if ( argc < x + 1)
            {
                FatalError(argv[0], "Missing argument for port number");
            }
            else if (sscanf(argv[x+1], "%d", &port_no) == 1)
            {
                port_no = atoi(argv[x+1]);
            }
            else
            {
                FatalError(argv[0], "Missing agrument for port number");
            }
            if (port_no <= 2000)
            {
                FatalError(argv[0], "Ivalid port number, must be greater than 2000");
            }
            x+=2;
        }
    if(strcmp(argv[x], "-h") == 0)
    {
        PrintUsage();
        return 0;
    }
  /********************************************************************/
  /* Please add support for the -print & -autotest options here...    */
  /********************************************************************/
if (strcmp(argv[x], "-print")==0)
{
    printCommand = 1;
}
else if (strcmp(argv[x], "-autotest") == 0)
{
    autotestCommand = 1;
}
else
{
    FatalError(argv[0], "Unrecognized command-line argument");
}
x++;
  } /*elihw*/
  /********************************************************************/
  /* Set up sockets and begin connecting to the server here...        */
  /********************************************************************/
    if (argc < 3)
    {
        FatalError(argv[0], "Usage hostname port error\n");
    }
    printf("%s: Creating a socket...\n", argv[0]);
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0)
    {
        FatalError(argv[0], "Socket creation failed\n");
    }
    server = gethostbyname(server_ip);
  if (server == NULL)
  {
    FatalError(argv[0], "No such host name\n");
  }
  printf("%s: Using port %d...\n", argv[0], port_no);
  if (port_no <= 2000)
  {
    FatalError(argv[0], "Port should be greater than 2000\n");
  }
  printf("%s: Preparing the server address...\n", argv[0]);
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port_no);
  memcpy(&server_addr.sin_addr.s_addr,server->h_addr_list[0], server->h_length);
  printf("%s: Connecting to the server...\n", argv[0]);
  if (connect(socket_fd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
  {
    FatalError(argv[0], "Connection Failed\n");
  }
  if(autotestCommand == 1)
  {
    printf("%s: Sending request AUTOTEST\n", argv[0]);
    if (SendRequest(socket_fd, "AUTOTEST",argv[0]) < 0)
    {
        FatalError(argv[0],"Writing to socket failed");
    }
  }
   if(printCommand == 1)
  {
    printf("%s: Sending request PRINT\n", argv[0]);
    if (SendRequest(socket_fd, "PRINT",argv[0]) < 0)
    {
        FatalError(argv[0],"Writing to socket failed");
    }
  }
  printf("%s: Sending request CLOSE\n", argv[0]);
  if (SendRequest(socket_fd, "CLOSE", argv[0]) < 0)
  {
    FatalError(argv[0], "Failed to close connection");
  }
  printf("%s: Closing...\n", argv[0]);
  close(socket_fd);
  return 0;
}
  /************************************************************************/
  /* After a successful connection, send commands to the server here...   */
  /************************************************************************/
void FatalError(const char *Program, const char *ErrorMsg)
{
    fprintf(stderr, "%s: %s\n", Program, ErrorMsg);
    exit(1);
}
void PrintUsage()
{
    printf("Usage: PhotoLab_client -ip <server_ip> -p <port_no> -print -autotest\n");
    printf("Options:\n");
    printf("-ip         Specify the server IP address\n");
    printf("-p          Specify the server port number\n");
    printf("-print      Send request to server to print supported DIP operations\n");
    printf("-autotest   Send request to server to run AutoTest functionality\n");
    printf("-h          Display this usage information\n");
}
/********************************************************************/
/* Complete the funtion implementations here...                     */
/********************************************************************/
