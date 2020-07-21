/*
 * Copyright (C) 2015-2020 Yuki MIZUNO
 * https://github.com/Harekaze/pvr.epgstation/
 * SPDX-License-Identifier: GPL-3.0-only
 */
#include "schedule.h"
#include "api.h"
#include "epgstation/types.h"
#include "kodi/libXBMC_addon.h"
#include "json/json.hpp"

extern ADDON::CHelper_libXBMC_addon* XBMC;

namespace epgstation {
bool Schedule::refresh()
{
    nlohmann::json response;

    programs.clear();
    channels.clear();

    std::vector<std::string> types = { "GR", "BS", "CS" };
    for (const auto type : types) {
        if (api::getSchedule(type, response) == api::REQUEST_FAILED) {
            return false;
        }

        for (const auto& o : response) {
            if (o["programs"].empty()) {
                continue;
            }
            const auto ch = o["channel"].get<channel>();
            channels.push_back(ch);

            for (const program& p : o["programs"]) {
                programs.push_back(p);
            }
        }
    }

    XBMC->Log(ADDON::LOG_NOTICE, "Updated schedule: channel ammount = %d", 8);
    return true;
}
}
