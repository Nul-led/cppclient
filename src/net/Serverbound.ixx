export module Serverbound;

export enum class ServerboundHeaders {
    Ping = 0, // will be sent automatically in the js when receiving pong, will never be encrypted, isn't sent as part of the packet buffer
    Init,
    Auth,
    Spawn,
    Attribute,
    Tank,
    Input
};