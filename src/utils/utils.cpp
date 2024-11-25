#include "utils/utils.h"
#include "utils/typedef.h"
#include "libs/io/io.h"

using namespace SimpleOS;

time SimpleOS::get_time() {
    auto read_rtc = [](u8 reg) {
        outb(0x70, reg);
        return inb(0x71);
        };

    auto bcd_to_binary = [](unsigned char bcd) {
        return (bcd & 0x0F) + ((bcd / 16) * 10);
        };

    time current_time;

    current_time.second = bcd_to_binary(read_rtc(0x00));
    current_time.minute = bcd_to_binary(read_rtc(0x02));
    current_time.hour   = bcd_to_binary(read_rtc(0x04));
    current_time.day    = bcd_to_binary(read_rtc(0x07));
    current_time.month  = bcd_to_binary(read_rtc(0x08));
    current_time.year   = bcd_to_binary(read_rtc(0x09));

    return current_time;
}

void* SimpleOS::memset(void* ptr, int value, size_t num) {
    unsigned char* p = static_cast<unsigned char*>(ptr);
    for (size_t i = 0; i < num; i++) {
        p[i] = static_cast<unsigned char>(value);
    }
    return ptr;
}

vector<string> SimpleOS::split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    for(size_t i = 0; i < str.size(); i++) {
        if (str[i] == delimiter) {
            if (!token.empty()) {
                tokens.push(token);
                token = "";
            }
        }
        else {
            token.push(str[i]);
        }
    }

    if (!token.empty()) {
        tokens.push(token);
    }

    return tokens;
}

size_t SimpleOS::abs(int x) {
    return x < 0 ? -x : x;
}