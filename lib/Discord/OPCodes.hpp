#ifndef DISCORD_OPCODES_HPP
#define DISCORD_OPCODES_HPP

namespace Hideyoshi {
    namespace Discord {
        enum OPCodes {
            DISPATCH,
            HEARTBEAT,
            IDENTIFY,
            STATUS_UPDATE,
            VOICE_STATE_UPDATE,
            VOICE_SERVER_PING,
            RESUME,
            RECONNECT,
            REQUEST_GUILD_MEMBERS,
            INVALID_SESSION,
            HELLO,
            HEARTBEAT_ACK
        };
    }
}

#endif
