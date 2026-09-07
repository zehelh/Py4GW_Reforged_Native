#include "base/error_handling.h"

#include "GW/trade/trade.h"

#include "base/CrashHandler.h"
#include "base/logger.h"

#include <atomic>

namespace GW::trade {

std::atomic<bool> g_initialized = false;

bool Initialize() {
    CrashContextScope context("startup", "trade", "initialize");
    if (g_initialized) {
        return true;
    }
    g_initialized = true;
    Logger::Instance().LogInfo("[trade] Trade module initialized.");
    return true;
}

void Shutdown() {
    CrashContextScope context("shutdown", "trade", "shutdown");
    if (!g_initialized) {
        return;
    }
    g_initialized = false;
    Logger::Instance().LogInfo("[trade] Trade module shutdown.");
}

}  // namespace GW::trade
