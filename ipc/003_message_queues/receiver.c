#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSG_SIZE 100

/* message structure */
struct msg_buffer {
    long msg_type;              /* Msg tybe != 0 */
    int number;                 /* some user data */
    char msg_text[MSG_SIZE];    /* Msg body */
};

int main() {
    key_t key;
    int msgid;
    struct msg_buffer message;

    /* Create unique message queue key, file is not important */
    key = ftok("someFileToGenerateKey.txt", 65);
    if (key == -1) {
        perror("ftok failed");
        exit(1);
    }

    /* Create or open queue */
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    /* Get message */
    if (msgrcv(msgid, &message, sizeof(message) - sizeof(message.msg_type), 1, 0) == -1) {
        perror("msgrcv failed");
        exit(1);
    }

    printf("Message received: %d, %s\n", message.number, message.msg_text);

    /* Destroy queue*/
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(1);
    }

    return 0;
}