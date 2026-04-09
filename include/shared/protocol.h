#ifndef SHARED_PROTOCOL_H
#define SHARED_PROTOCOL_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t node_id_t;
typedef uint8_t button_id_t;
typedef uint8_t output_id_t;

enum
{
    PROTOCOL_START_BYTE = 0xAA,
    PROTOCOL_MAX_PAYLOAD_LENGTH = 8U,
    PROTOCOL_MAX_DATA_LENGTH = 1U + PROTOCOL_MAX_PAYLOAD_LENGTH,
    PROTOCOL_MAX_FRAME_LENGTH = 3U + PROTOCOL_MAX_DATA_LENGTH,
    PROTOCOL_CMD_SET_OUTPUT_STATE = 0x10,
    PROTOCOL_CMD_BUTTON_PRESSED = 0x20,
};

typedef struct
{
    uint8_t command;
    uint8_t payload_length;
    uint8_t payload[PROTOCOL_MAX_PAYLOAD_LENGTH];
} protocol_frame_t;

typedef enum
{
    PROTOCOL_PARSER_WAIT_START = 0,
    PROTOCOL_PARSER_WAIT_LENGTH,
    PROTOCOL_PARSER_WAIT_CRC,
    PROTOCOL_PARSER_WAIT_DATA,
} protocol_parser_state_t;

typedef struct
{
    protocol_parser_state_t state;
    uint8_t expected_length;
    uint8_t received_crc;
    uint8_t data_index;
    uint8_t data[PROTOCOL_MAX_DATA_LENGTH];
} protocol_parser_t;

static inline uint8_t protocol_crc8(const uint8_t *data, uint8_t length)
{
    uint8_t crc = 0x00U;

    for (uint8_t i = 0U; i < length; ++i)
    {
        uint8_t extract = data[i];

        for (uint8_t bit = 0U; bit < 8U; ++bit)
        {
            uint8_t sum = (crc ^ extract) & 0x01U;

            crc >>= 1U;
            if (sum != 0U)
            {
                crc ^= 0x8CU;
            }

            extract >>= 1U;
        }
    }

    return crc;
}

static inline void protocol_parser_init(protocol_parser_t *parser)
{
    parser->state = PROTOCOL_PARSER_WAIT_START;
    parser->expected_length = 0U;
    parser->received_crc = 0U;
    parser->data_index = 0U;
    memset(parser->data, 0, sizeof(parser->data));
}

static inline size_t protocol_build_frame(
    uint8_t command,
    const uint8_t *payload,
    uint8_t payload_length,
    uint8_t *buffer,
    size_t buffer_size)
{
    const uint8_t data_length = (uint8_t)(payload_length + 1U);

    if (payload_length > PROTOCOL_MAX_PAYLOAD_LENGTH)
    {
        return 0U;
    }

    if (buffer_size < (size_t)(3U + data_length))
    {
        return 0U;
    }

    buffer[0] = PROTOCOL_START_BYTE;
    buffer[1] = data_length;
    buffer[3] = command;

    if ((payload_length > 0U) && (payload != NULL))
    {
        memcpy(&buffer[4], payload, payload_length);
    }

    buffer[2] = protocol_crc8(&buffer[3], data_length);

    return (size_t)(3U + data_length);
}

static inline int protocol_parser_push_byte(
    protocol_parser_t *parser,
    uint8_t byte,
    protocol_frame_t *frame_out)
{
    switch (parser->state)
    {
    case PROTOCOL_PARSER_WAIT_START:
        if (byte == PROTOCOL_START_BYTE)
        {
            parser->state = PROTOCOL_PARSER_WAIT_LENGTH;
        }
        break;

    case PROTOCOL_PARSER_WAIT_LENGTH:
        if ((byte == 0U) || (byte > PROTOCOL_MAX_DATA_LENGTH))
        {
            protocol_parser_init(parser);
        }
        else
        {
            parser->expected_length = byte;
            parser->data_index = 0U;
            parser->state = PROTOCOL_PARSER_WAIT_CRC;
        }
        break;

    case PROTOCOL_PARSER_WAIT_CRC:
        parser->received_crc = byte;
        parser->state = PROTOCOL_PARSER_WAIT_DATA;
        break;

    case PROTOCOL_PARSER_WAIT_DATA:
        parser->data[parser->data_index++] = byte;

        if (parser->data_index == parser->expected_length)
        {
            const uint8_t calculated_crc =
                protocol_crc8(parser->data, parser->expected_length);

            if ((calculated_crc == parser->received_crc) && (frame_out != NULL))
            {
                frame_out->command = parser->data[0];
                frame_out->payload_length =
                    (uint8_t)(parser->expected_length - 1U);

                if (frame_out->payload_length > 0U)
                {
                    memcpy(
                        frame_out->payload,
                        &parser->data[1],
                        frame_out->payload_length);
                }
            }

            {
                const int frame_complete =
                    (calculated_crc == parser->received_crc) ? 1 : 0;

                protocol_parser_init(parser);
                return frame_complete;
            }
        }
        break;

    default:
        protocol_parser_init(parser);
        break;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif

#endif
