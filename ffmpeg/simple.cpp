#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

int main(int argc, char **argv)
{
    std::cout << exec("ffplay -flags low_delay -vf \"setpts=0\" \"rtsp://admin:1q2w3e4r@192.168.1.114:554/cam/realmonitor?channel=1&subtype=0\"");
    return(0);
}