#include <iostream>
#include <filesystem>
#include "httplib.h"

using namespace httplib;
namespace fs = std::filesystem;

const std::string SHARED_DIRECTORY = "/home/alinourzadeh/Downloads";

void serve_file(const Request &request, Response &response) {
    std::string file_path = SHARED_DIRECTORY + request.path;
    if (fs::exists(file_path)) {
        std::ifstream ifs(file_path, std::ios::binary);
        std::stringstream buffer;
        buffer << ifs.rdbuf();
        response.set_content(buffer.str(), "application/octet-stream");
    } else {
        response.status = 404;
        response.set_content("File not found", "text/plain");
    }
}

void upload_file(const Request &request, Response &response) {
    auto it = request.files.find("file");
    if (it != request.files.end()) {
        const auto &file = it->second;
        std::ofstream ofs(SHARED_DIRECTORY + "/" + file.name, std::ios::binary);
        ofs.write(file.content.data(), file.content.size());
        response.set_content("File uploaded successfully", "text/plain");
    } else {
        response.status = 400;
        response.set_content("No file uploaded", "text/plain");
    }
}

void list_file(const Request &request, Response &response) {
    std::string file_list = "<html><body><h2>Shared Files</h2><ul>";
    for (const auto &file: fs::directory_iterator(SHARED_DIRECTORY)) {
        file_list += "<li><a href='/" + file.path().filename().string() + "'>" + file.path().filename().string() +
                     "</a></li>";
    }
    file_list += "</ul></body></html>";
    response.set_content(file_list, "text/html");
}

int main() {
    if (!fs::exists(SHARED_DIRECTORY)) fs::create_directory(SHARED_DIRECTORY);

    Server server;
    server.Get("/", list_file);
    server.Get(R"(/(.*))", serve_file);
    server.Post("/upload", upload_file);

    std::cout << "Server started on http://0.0.0.0:8080\n";
    server.listen("0.0.0.0", 8080);
}
