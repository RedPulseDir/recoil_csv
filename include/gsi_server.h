#pragma once
#include "types.h"
#include <memory>
#include <functional>
#include <thread>

namespace recoil {

class GSIServer {
public:
    using Callback = std::function<void(const GSIPlayerState&)>;
    
    GSIServer();
    ~GSIServer();
    
    bool Start(int port, const std::string& host = "127.0.0.1");
    void Stop();
    bool IsRunning() const;
    
    void SetCallback(Callback cb);
    void SetAuthToken(const std::string& token);
    
private:
    void ServerThread(int port, const std::string& host);
    void HandleRequest(const std::string& body);
    
    struct Impl;
    std::unique_ptr<Impl> impl_;
    std::thread server_thread_;
    Callback callback_;
    std::string auth_token_;
    bool running_ = false;
};

} // namespace recoil
