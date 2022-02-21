#include "main.hpp"
#include "socket.hpp"

class Proxy {
 private:
  int client_connect_fd;
  int server_connect_fd;
  std::vector<char> response_vec;
  std::string response_str;

 public:
  std::string getRequest();
  bool check502(std::string request);
  bool checkMethod(std::string method);
  std::string generateSendMessage(std::string method,
                                  std::string path,
                                  std::string information);

  void doGet(std::string toSend_str, std::string host_str, std::string port_str);
  std::vector<char> getResponseVec() { return response_vec; }
  std::string getResponseStr() { return response_str; }
  //Note that in cpp, static only added in header file
  static void becomeDaemon();
};
