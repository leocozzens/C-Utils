#include <queue.h>
#include <time.h>

#define Q_COUNT 3

int main(void) {
    int out;

    srand(time(NULL));
    Queue q[Q_COUNT]; 
    for(int i = 0; i < Q_COUNT; i++) make_queue(&q[i]);

    for(int qIndex = 0; qIndex < Q_COUNT; qIndex++) {
        printf("\nQueue index: %d\n", qIndex + 1);
        for(int i = 10; i <= 100; i += 10) {
            enqueue(&q[qIndex], rand() % (i + 1));
        }

        while(dequeue(&q[qIndex], &out)) {
            printf("out = %d\n", out);
        }
        if(q[qIndex].head == NULL && q[qIndex].tail == NULL) printf("Empty!\n");
    }
    return 0;
}