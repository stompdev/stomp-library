
#ifndef STOMP_STRINGS_H
#define	STOMP_STRINGS_H

#define STOMP_NULL 0x00
#define STOMP_NEW_LINE 0x0a
#define STOMP_COLON 0x3a

#define STOMP_COMMAND_ABORT_LEN 5
extern const char stomp_command_abort[STOMP_COMMAND_ABORT_LEN + 1];
#define STOMP_COMMAND_ACK_LEN 3
extern const char stomp_command_ack[STOMP_COMMAND_ACK_LEN + 1];
#define STOMP_COMMAND_BEGIN_LEN 5
extern const char stomp_command_begin[STOMP_COMMAND_BEGIN_LEN + 1];
#define STOMP_COMMAND_COMMIT_LEN 6
extern const char stomp_command_commit[STOMP_COMMAND_COMMIT_LEN + 1];
#define STOMP_COMMAND_CONNECT_LEN 7
extern const char stomp_command_connect[STOMP_COMMAND_CONNECT_LEN + 1];
#define STOMP_COMMAND_CONNECTED_LEN 9
extern const char stomp_command_connected[STOMP_COMMAND_CONNECTED_LEN + 1];
#define STOMP_COMMAND_DISCONNECT_LEN 10
extern const char stomp_command_disconnect[STOMP_COMMAND_DISCONNECT_LEN + 1];
#define STOMP_COMMAND_ERROR_LEN 5 
extern const char stomp_command_error[STOMP_COMMAND_ERROR_LEN + 1];
#define STOMP_COMMAND_MESSAGE_LEN 7
extern const char stomp_command_message[STOMP_COMMAND_MESSAGE_LEN + 1];
#define STOMP_COMMAND_NACK_LEN 4
extern const char stomp_command_nack[STOMP_COMMAND_NACK_LEN + 1];
#define STOMP_COMMAND_RECEIPT_LEN 7
extern const char stomp_command_receipt[STOMP_COMMAND_RECEIPT_LEN + 1];
#define STOMP_COMMAND_SEND_LEN 4
extern const char stomp_command_send[STOMP_COMMAND_SEND_LEN + 1];
#define STOMP_COMMAND_STOMP_LEN 5
extern const char stomp_command_stomp[STOMP_COMMAND_STOMP_LEN + 1];
#define STOMP_COMMAND_SUBSCRIBE_LEN 9
extern const char stomp_command_subscribe[STOMP_COMMAND_SUBSCRIBE_LEN + 1];
#define STOMP_COMMAND_UNSUBSCRIBE_LEN 11
extern const char stomp_command_unsubscribe[STOMP_COMMAND_UNSUBSCRIBE_LEN + 1];

#define STOMP_HEADER_ACCEPT_VERSION_LEN 14
extern const char stomp_header_accept_version[STOMP_HEADER_ACCEPT_VERSION_LEN + 1];
#define STOMP_HEADER_ACK_LEN 3
extern const char stomp_header_ack[STOMP_HEADER_ACK_LEN + 1];
#define STOMP_HEADER_AMQ_MSG_TYPE_LEN 12
extern const char stomp_header_amq_msg_type[STOMP_HEADER_AMQ_MSG_TYPE_LEN + 1];
#define STOMP_HEADER_AUTO_LEN 4
extern const char stomp_header_auto[STOMP_HEADER_AUTO_LEN + 1];
#define STOMP_HEADER_CLIENT_LEN 6
extern const char stomp_header_client[STOMP_HEADER_CLIENT_LEN + 1];
#define STOMP_HEADER_CLIENT_ID_LEN 9
extern const char stomp_header_client_id[STOMP_HEADER_CLIENT_ID_LEN + 1];
#define STOMP_HEADER_CONTENT_LENGTH_LEN 14
extern const char stomp_header_content_length[STOMP_HEADER_CONTENT_LENGTH_LEN + 1];
#define STOMP_HEADER_CONTENT_TYPE_LEN 12
extern const char stomp_header_content_type[STOMP_HEADER_CONTENT_TYPE_LEN + 1];
#define STOMP_HEADER_CORRELATION_ID_LEN 14
extern const char stomp_header_correlation_id[STOMP_HEADER_CORRELATION_ID_LEN + 1];
#define STOMP_HEADER_DESTINATION_LEN 11
extern const char stomp_header_destination[STOMP_HEADER_DESTINATION_LEN + 1];
#define STOMP_HEADER_EXPIRES_LEN 7
extern const char stomp_header_expires[STOMP_HEADER_EXPIRES_LEN + 1];
#define STOMP_HEADER_HOST_LEN 4
extern const char stomp_header_host[STOMP_HEADER_HOST_LEN + 1];
#define STOMP_HEADER_ID_LEN 2
extern const char stomp_header_id[STOMP_HEADER_ID_LEN + 1];
#define STOMP_HEADER_CLIENT_INDIVIDUAL_LEN 17
extern const char stomp_header_client_individual[STOMP_HEADER_CLIENT_INDIVIDUAL_LEN + 1];
#define STOMP_HEADER_LOGIN_LEN 5
extern const char stomp_header_login[STOMP_HEADER_LOGIN_LEN + 1];
#define STOMP_HEADER_MESSAGE_ID_LEN 10
extern const char stomp_header_message_id[STOMP_HEADER_MESSAGE_ID_LEN + 1];
#define STOMP_HEADER_MESSAGE_LEN 7
extern const char stomp_header_message[STOMP_HEADER_MESSAGE_LEN + 1];
#define STOMP_HEADER_ORIGINAL_DESTINATION_LEN 20
extern const char stomp_header_original_destination[STOMP_HEADER_ORIGINAL_DESTINATION_LEN + 1];
#define STOMP_HEADER_PASSCODE_LEN 8
extern const char stomp_header_passcode[STOMP_HEADER_PASSCODE_LEN + 1];
#define STOMP_HEADER_PERSISTENT_LEN 10
extern const char stomp_header_persistent[STOMP_HEADER_PERSISTENT_LEN + 1];
#define STOMP_HEADER_PRIORITY_LEN 8
extern const char stomp_header_priority[STOMP_HEADER_PRIORITY_LEN + 1];
#define STOMP_HEADER_RECEIPT_ID_LEN 10
extern const char stomp_header_receipt_id[STOMP_HEADER_RECEIPT_ID_LEN + 1];
#define STOMP_HEADER_RECEIPT_LEN 7
extern const char stomp_header_receipt[STOMP_HEADER_RECEIPT_LEN + 1];
#define STOMP_HEADER_REDELIVERED_LEN 11
extern const char stomp_header_redelivered[STOMP_HEADER_REDELIVERED_LEN + 1];
#define STOMP_HEADER_REPLY_TO_LEN 8
extern const char stomp_header_reply_to[STOMP_HEADER_REPLY_TO_LEN + 1];
#define STOMP_HEADER_REQUEST_ID_LEN 10
extern const char stomp_header_request_id[STOMP_HEADER_REQUEST_ID_LEN + 1];
#define STOMP_HEADER_RESPONSE_ID_LEN 11
extern const char stomp_header_response_id[STOMP_HEADER_RESPONSE_ID_LEN + 1];
#define STOMP_HEADER_SELECTOR_LEN 8
extern const char stomp_header_selector[STOMP_HEADER_SELECTOR_LEN + 1];
#define STOMP_HEADER_SESSION_LEN 7
extern const char stomp_header_session[STOMP_HEADER_SESSION_LEN + 1];
#define STOMP_HEADER_SUBSCRIPTION_LEN 12
extern const char stomp_header_subscription[STOMP_HEADER_SUBSCRIPTION_LEN + 1];
#define STOMP_HEADER_TIMESTAMP_LEN 9
extern const char stomp_header_timestamp[STOMP_HEADER_TIMESTAMP_LEN + 1];
#define STOMP_HEADER_TRANSACTION_LEN 11
extern const char stomp_header_transaction[STOMP_HEADER_TRANSACTION_LEN + 1];
#define STOMP_HEADER_TRANSFORMATION_ERROR_LEN 20
extern const char stomp_header_transformation_error[STOMP_HEADER_TRANSFORMATION_ERROR_LEN + 1];
#define STOMP_HEADER_TRANSFORMATION_LEN 14
extern const char stomp_header_transformation[STOMP_HEADER_TRANSFORMATION_LEN + 1];
#define STOMP_HEADER_TYPE_LEN 4
extern const char stomp_header_type[STOMP_HEADER_TYPE_LEN + 1];
#define STOMP_HEADER_JMSXUSERID_LEN 10
extern const char stomp_header_jmsxuserid[STOMP_HEADER_JMSXUSERID_LEN + 1];

#define STOMP_HEADER_VERSION_LEN 7
extern const char stomp_header_version[STOMP_HEADER_VERSION_LEN + 1];
#define STOMP_HEADER_SERVER_LEN 6
extern const char stomp_header_server[STOMP_HEADER_SERVER_LEN + 1];
#define STOMP_HEADER_HOST_ID_LEN 7
extern const char stomp_header_host_id[STOMP_HEADER_HOST_ID_LEN + 1];
#define STOMP_HEADER_HEART_BEAT_LEN 10
extern const char stomp_header_heart_beat[STOMP_HEADER_HEART_BEAT_LEN + 1];
#define STOMP_HEADER_USER_ID_LEN 7
extern const char stomp_header_user_id[STOMP_HEADER_USER_ID_LEN + 1];

#endif	/* STOMP_STRINGS_H */