
#include "proxy.hpp"
#include "socket.hpp"

//#include "request.hpp"

int main() {
  /*
  Proxy p;
    std::string init_str = p.getRequest();
    Request r(init_str);
    
    std::cout << r.getRequest() << std::endl;
    std::cout << r.getMethod() << std::endl;
    std::cout << r.getUri() << std::endl;
    std::cout << r.getHost() << std::endl;
    std::cout << r.getPath() << std::endl;
    std::cout << r.getInforamtion() << std::endl;
    
  std::string m = r.getMethod();
  std::string pa = r.getPath();
  std::string in = r.getInforamtion();
  std::cout << "Information is: " << std::endl << in << std::endl;
  std::string toSend = p.generateSendMessage(m, pa, in);
  std::cout << "toSend is: " << std::endl << toSend << std::endl;
  std::string po = r.getPort();
  std::string ho = r.getHost();
  p.doGet(toSend, ho, po);
  vector<char> res = p.getResponseVec();
  // for(int i = 0; i < res.size(); i++){
  //     std::cout << res[i];
  // }
  // std::cout << std::endl;
  */

  Proxy Proxy;
  Proxy.becomeDaemon();
  while (true) {
    Server Server(PORT);  //PORT is 12345
  }
}
