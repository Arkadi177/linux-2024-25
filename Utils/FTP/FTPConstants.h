#pragma once

constexpr auto OK_220       = "220 Service ready\r\n";
constexpr auto OK_200       = "200 Command OK\r\n";
constexpr auto USER_331     = "331 Username OK, need password\r\n";
constexpr auto PASS_230     = "230 Login successful\r\n";
constexpr auto NOTLOGGED_530 = "530 Not logged in\r\n";
constexpr auto GOODBYE_221  = "221 Goodbye\r\n";
constexpr auto START_150    = "150 Opening data connection\r\n";
constexpr auto COMPLETE_226 = "226 Transfer complete\r\n";
constexpr auto FILENF_550   = "550 File not found\r\n";
constexpr auto UNKNOWN_500  = "500 Unknown command\r\n";
