#pragma once

typedef enum even_e {
    EVENT_MESSAGE_RECIEVED_FINAL,
    EVENT_MESSAGE_RECIEVED_TO_TRANSFER,
    EVENT_MESSAGE_SEND,
    EVENT_HELP_MESSAGE_RECIEVED,
    EVENT_HELP_MESSAGE_SEND,
    EVENT_UPDATE_RECIEVED,
    EVENT_UPDATE_SEND,
    EVENT_PAIR,
    EVENT_CONNECTION_REQUEST_BLUETOOTH_SEND,
    EVENT_CONNECTION_REQUEST_BLUETOOTH_RECIEVE,
    EVENT_CONNECTION_DETAILS_WIFI_DIRECT_SEND,
    EVENT_CONNECTION_DETAILS_WIFI_DIRECT_RECIEVE,
} event;
