#ifndef DISCORD_USER_HPP
#define DISCORD_USER_HPP

#include <string>

using namespace std;

namespace Hideyoshi {
    namespace Discord {
        struct user_t {
            string avatar;
            string id;
            string username;
            string discriminator;

            bool bot;
        };
    }
}

#endif
