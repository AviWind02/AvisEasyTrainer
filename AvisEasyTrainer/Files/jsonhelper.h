//#pragma once
//#include "Vendor/json-src/single_include/nlohmann/json.hpp"
//
//namespace JsonHelper {
//
//    inline bool EnsureJsonExists(const std::string& path, bool autoRename = false) {
//        std::filesystem::path filePath(path);
//
//        try {
//            std::filesystem::create_directories(filePath.parent_path());
//
//            if (std::filesystem::is_directory(filePath)) {
//                loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] A directory already exists at %s", path.c_str());
//                return false;
//            }
//
//            std::filesystem::path finalPath = filePath;
//            const int maxAttempts = 50;
//
//            if (autoRename && std::filesystem::exists(finalPath)) {
//                int counter = 1;
//                std::string baseName = finalPath.stem().string();
//                std::string extension = finalPath.extension().string();
//                std::filesystem::path parent = finalPath.parent_path();
//
//                do {
//                    std::ostringstream newName;
//                    newName << baseName << " (" << counter << ")" << extension;
//                    finalPath = parent / newName.str();
//                    counter++;
//                } while (std::filesystem::exists(finalPath) && counter <= maxAttempts);
//
//                if (counter > maxAttempts) {
//                    loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Too many auto-rename attempts.");
//                    return false;
//                }
//
//                loghandler::sdk->logger->Info(loghandler::handle, "[JsonHelper] Auto-renamed to %s", finalPath.string().c_str());
//            }
//
//            if (!std::filesystem::exists(finalPath)) {
//                std::ofstream out(finalPath);
//                if (out.is_open()) {
//                    out << "{}";
//                    out.close();
//                    loghandler::sdk->logger->Info(loghandler::handle, "[JsonHelper] Created JSON file at %s", finalPath.string().c_str());
//                    return true;
//                }
//                loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Failed to open file for writing: %s", finalPath.string().c_str());
//                return false;
//            }
//
//            return true;
//        }
//        catch (const std::exception& e) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Exception: %s", e.what());
//            return false;
//        }
//    }
//
//    inline bool LoadJson(const std::string& path, nlohmann::json& out) {
//        if (!std::filesystem::exists(path)) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] File not found: %s", path.c_str());
//            return false;
//        }
//
//        std::ifstream inFile(path);
//        if (!inFile.is_open()) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Failed to open: %s", path.c_str());
//            return false;
//        }
//
//        try {
//            inFile >> out;
//            return true;
//        }
//        catch (const std::exception& e) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Parse error: %s", e.what());
//            return false;
//        }
//    }
//
//    inline bool SaveJsonOverride(const std::string& path, const nlohmann::json& data) {
//        EnsureJsonExists(path);
//
//        std::ofstream outFile(path);
//        if (!outFile.is_open()) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Cannot write to: %s", path.c_str());
//            return false;
//        }
//
//        try {
//            outFile << data.dump(4);
//            loghandler::sdk->logger->Info(loghandler::handle, "[JsonHelper] Saved JSON: %s", path.c_str());
//            return true;
//        }
//        catch (const std::exception& e) {
//            loghandler::sdk->logger->Warning(loghandler::handle, "[JsonHelper] Write failed: %s", e.what());
//            return false;
//        }
//    }
//
//    inline void LogInvalidJsonField(const std::string& label, const std::string& expectedType) {
//        loghandler::sdk->logger->Warning(loghandler::handle,
//            "[JsonHelper] Invalid/missing field: %s (Expected: %s)", label.c_str(), expectedType.c_str());
//    }
//
//}