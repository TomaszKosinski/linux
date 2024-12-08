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
    key = ftok("someFileToGenerateKey.txt", 65);  /* 65 -> optional id */
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

    /* Create message */
    message.msg_type = 1;
    message.number = 42;
    strcpy(message.msg_text, "Hello, this is a structure!");

    /* Send message -> you can find it in the console using "ipcs -q" */
    if (msgsnd(msgid, &message, sizeof(message) - sizeof(message.msg_type), 0) == -1) {
        perror("msgsnd failed");
        exit(1);
    }

    printf("Message sent: %d, %s\n", message.number, message.msg_text);

    return 0;
}